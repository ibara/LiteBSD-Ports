/* LzFind.c -- Match finder for LZ algorithms
2009-04-22 : Igor Pavlov : Public domain */

#define _FILE_OFFSET_BITS 64

#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lzip.h"
#include "LzFind.h"

#define kHash2Size (1 << 10)
#define kHash3Size (1 << 16)
#define kHash4Size (1 << 20)

#define kFix3HashSize (kHash2Size)
#define kFix4HashSize (kHash2Size + kHash3Size)

#define HASH2_CALC hashValue = cur[0] | ((uint32_t)cur[1] << 8);

#define HASH3_CALC { \
  uint32_t temp = crc32[cur[0]] ^ cur[1]; \
  hash2Value = temp & (kHash2Size - 1); \
  hashValue = (temp ^ ((uint32_t)cur[2] << 8)) & p->hashMask; }

#define HASH4_CALC { \
  uint32_t temp = crc32[cur[0]] ^ cur[1]; \
  hash2Value = temp & (kHash2Size - 1); \
  hash3Value = (temp ^ ((uint32_t)cur[2] << 8)) & (kHash3Size - 1); \
  hashValue = (temp ^ ((uint32_t)cur[2] << 8) ^ (crc32[cur[3]] << 5)) & p->hashMask; }

#define kEmptyHashValue 0
#define kMaxValForNormalize ((uint32_t)0xFFFFFFFF)
#define kNormalizeStepMin (1 << 10) /* it must be power of 2 */
#define kNormalizeMask (~(kNormalizeStepMin - 1))

#define kStartMaxLen 3


static void Mf_ReadBlock(CMatchFinder *p)
{
  if (p->streamEndWasReached || p->result != SZ_OK)
    return;
  for (;;)
  {
    uint8_t * const dest = p->buffer + (p->streamPos - p->pos);
    const int size = (p->bufferBase + p->blockSize - dest);
    int rd;
    if (size == 0)
      return;
    rd = readblock( p->infd, dest, size );
    if (rd != size && errno)
      { p->result = SZ_ERROR_READ; return; }
    if (rd == 0)
    {
      p->streamEndWasReached = true;
      return;
    }
    CRC32_update_buf( &p->crc, dest, rd );
    p->streamPos += rd;
    if (p->streamPos - p->pos > p->keepSizeAfter)
      return;
  }
}


static void Mf_CheckAndMoveAndRead(CMatchFinder *p)
{
  if ((uint32_t)(p->bufferBase + p->blockSize - p->buffer) <= p->keepSizeAfter)
    {
    memmove(p->bufferBase,
      p->buffer - p->keepSizeBefore,
      p->streamPos - p->pos + p->keepSizeBefore);
    p->buffer = p->bufferBase + p->keepSizeBefore;
    }
  Mf_ReadBlock(p);
}


void Mf_Free(CMatchFinder *p)
{
  free(p->hash);
  p->hash = 0;
  free(p->bufferBase);
  p->bufferBase = 0;
}

static CLzRef* AllocRefs(uint32_t num)
{
  uint32_t sizeInBytes = num * sizeof(CLzRef);
  if (sizeInBytes / sizeof(CLzRef) != num)
    return 0;
  return (CLzRef *)malloc(sizeInBytes);
}

static void Mf_SetLimits(CMatchFinder *p)
{
  uint32_t limit = kMaxValForNormalize - p->pos;
  uint32_t limit2 = p->cyclicBufferSize - p->cyclicBufferPos;
  if (limit2 < limit)
    limit = limit2;
  limit2 = p->streamPos - p->pos;
  if (limit2 <= p->keepSizeAfter)
  {
    if (limit2 > 0)
      limit2 = 1;
  }
  else
    limit2 -= p->keepSizeAfter;
  if (limit2 < limit)
    limit = limit2;
  {
    uint32_t lenLimit = p->streamPos - p->pos;
    if (lenLimit > p->matchMaxLen)
      lenLimit = p->matchMaxLen;
    p->lenLimit = lenLimit;
  }
  p->posLimit = p->pos + limit;
}


int Mf_Init(CMatchFinder *p, const int ifd, const int mc, uint32_t historySize,
    uint32_t keepAddBufferBefore, uint32_t matchMaxLen, uint32_t keepAddBufferAfter)
{
  const uint32_t sizeReserv = ( historySize >> 1 ) +
    (keepAddBufferBefore + matchMaxLen + keepAddBufferAfter) / 2 + (1 << 19);

  p->hash = 0;
  p->cutValue = mc;
  p->infd = ifd;
  p->btMode = true;
  p->numHashBytes = 4;
  p->crc = 0xFFFFFFFFU;
  p->keepSizeBefore = historySize + keepAddBufferBefore + 1;
  p->keepSizeAfter = matchMaxLen + keepAddBufferAfter;
  /* we need one additional byte, since we use MoveBlock after pos++ and before dictionary using */
  /* keepSizeBefore + keepSizeAfter + sizeReserv must be < 4G) */
  p->blockSize = p->keepSizeBefore + p->keepSizeAfter + sizeReserv;
  p->buffer = p->bufferBase = (uint8_t *)malloc(p->blockSize);
  if( p->bufferBase )
  {
    uint32_t newCyclicBufferSize = historySize + 1;
    uint32_t hs;
    p->matchMaxLen = matchMaxLen;
    {
      if (p->numHashBytes == 2)
        hs = (1 << 16) - 1;
      else
      {
        hs = historySize - 1;
        hs |= (hs >> 1);
        hs |= (hs >> 2);
        hs |= (hs >> 4);
        hs |= (hs >> 8);
        hs >>= 1;
        hs |= 0xFFFF; /* don't change it! It's required for Deflate */
        if (hs > (1 << 24))
        {
          if (p->numHashBytes == 3)
            hs = (1 << 24) - 1;
          else
            hs >>= 1;
        }
      }
      p->hashMask = hs;
      hs++;
      if (p->numHashBytes > 2) hs += kHash2Size;
      if (p->numHashBytes > 3) hs += kHash3Size;
      if (p->numHashBytes > 4) hs += kHash4Size;
    }

    {
      uint32_t newSize;
      p->historySize = historySize;
      p->hashSizeSum = hs;
      p->cyclicBufferSize = newCyclicBufferSize;
      p->numSons = (p->btMode ? newCyclicBufferSize * 2 : newCyclicBufferSize);
      newSize = p->hashSizeSum + p->numSons;
      p->hash = AllocRefs(newSize);
      if (p->hash != 0)
      {
        uint32_t i;
        p->son = p->hash + p->hashSizeSum;
        for (i = 0; i < p->hashSizeSum; i++)
          p->hash[i] = kEmptyHashValue;
        p->cyclicBufferPos = 0;
        p->pos = p->streamPos = p->cyclicBufferSize;
        p->result = SZ_OK;
        p->streamEndWasReached = false;
        Mf_ReadBlock(p);
        Mf_SetLimits(p);
        return 1;
      }
    }
  }
  Mf_Free(p);
  return 0;
}

static void Mf_Normalize3(uint32_t subValue, CLzRef *items, uint32_t numItems)
{
  uint32_t i;
  for (i = 0; i < numItems; i++)
  {
    uint32_t value = items[i];
    if (value <= subValue)
      value = kEmptyHashValue;
    else
      value -= subValue;
    items[i] = value;
  }
}

static void Mf_Normalize(CMatchFinder *p)
{
  uint32_t subValue = (p->pos - p->historySize - 1) & kNormalizeMask;
  Mf_Normalize3(subValue, p->hash, p->hashSizeSum + p->numSons);
  p->posLimit -= subValue;
  p->pos -= subValue;
  p->streamPos -= subValue;
}

static void Mf_CheckLimits(CMatchFinder *p)
{
  if (p->pos == kMaxValForNormalize)
    Mf_Normalize(p);
  if (!p->streamEndWasReached && p->keepSizeAfter == p->streamPos - p->pos)
    Mf_CheckAndMoveAndRead(p);
  if (p->cyclicBufferPos == p->cyclicBufferSize)
    p->cyclicBufferPos = 0;
  Mf_SetLimits(p);
}

static uint32_t * Hc_GetMatchesSpec(uint32_t lenLimit, uint32_t curMatch, uint32_t pos, const uint8_t *cur, CLzRef *son,
    uint32_t _cyclicBufferPos, uint32_t _cyclicBufferSize, uint32_t cutValue,
    uint32_t *distances, uint32_t maxLen)
{
  son[_cyclicBufferPos] = curMatch;
  for (;;)
  {
    uint32_t delta = pos - curMatch;
    if (cutValue-- == 0 || delta >= _cyclicBufferSize)
      return distances;
    {
      const uint8_t *pb = cur - delta;
      curMatch = son[_cyclicBufferPos - delta + ((delta > _cyclicBufferPos) ? _cyclicBufferSize : 0)];
      if (pb[maxLen] == cur[maxLen] && *pb == *cur)
      {
        uint32_t len = 0;
        while (++len != lenLimit)
          if (pb[len] != cur[len])
            break;
        if (maxLen < len)
        {
          *distances++ = maxLen = len;
          *distances++ = delta - 1;
          if (len == lenLimit)
            return distances;
        }
      }
    }
  }
}


static uint32_t * GetMatchesSpec1( uint32_t lenLimit, uint32_t curMatch,
    uint32_t pos, const uint8_t *cur, CLzRef *son,
    uint32_t _cyclicBufferPos, uint32_t _cyclicBufferSize, uint32_t cutValue,
    uint32_t *distances, uint32_t maxLen )
{
  CLzRef *ptr0 = son + (_cyclicBufferPos << 1) + 1;
  CLzRef *ptr1 = son + (_cyclicBufferPos << 1);
  uint32_t len0 = 0, len1 = 0;
  for (;;)
  {
    uint32_t delta = pos - curMatch;
    if (cutValue-- == 0 || delta >= _cyclicBufferSize)
    {
      *ptr0 = *ptr1 = kEmptyHashValue;
      return distances;
    }
    {
      CLzRef *pair = son + ((_cyclicBufferPos - delta + ((delta > _cyclicBufferPos) ? _cyclicBufferSize : 0)) << 1);
      const uint8_t *pb = cur - delta;
      uint32_t len = (len0 < len1 ? len0 : len1);
      if (pb[len] == cur[len])
      {
        if (++len != lenLimit && pb[len] == cur[len])
          while (++len != lenLimit)
            if (pb[len] != cur[len])
              break;
        if (maxLen < len)
        {
          *distances++ = maxLen = len;
          *distances++ = delta - 1;
          if (len == lenLimit)
          {
            *ptr1 = pair[0];
            *ptr0 = pair[1];
            return distances;
          }
        }
      }
      if (pb[len] < cur[len])
      {
        *ptr1 = curMatch;
        ptr1 = pair + 1;
        curMatch = *ptr1;
        len1 = len;
      }
      else
      {
        *ptr0 = curMatch;
        ptr0 = pair;
        curMatch = *ptr0;
        len0 = len;
      }
    }
  }
}

static void SkipMatchesSpec(uint32_t lenLimit, uint32_t curMatch, uint32_t pos, const uint8_t *cur, CLzRef *son,
    uint32_t _cyclicBufferPos, uint32_t _cyclicBufferSize, uint32_t cutValue)
{
  CLzRef *ptr0 = son + (_cyclicBufferPos << 1) + 1;
  CLzRef *ptr1 = son + (_cyclicBufferPos << 1);
  uint32_t len0 = 0, len1 = 0;
  for (;;)
  {
    uint32_t delta = pos - curMatch;
    if (cutValue-- == 0 || delta >= _cyclicBufferSize)
    {
      *ptr0 = *ptr1 = kEmptyHashValue;
      return;
    }
    {
      CLzRef *pair = son + ((_cyclicBufferPos - delta + ((delta > _cyclicBufferPos) ? _cyclicBufferSize : 0)) << 1);
      const uint8_t *pb = cur - delta;
      uint32_t len = (len0 < len1 ? len0 : len1);
      if (pb[len] == cur[len])
      {
        while (++len != lenLimit)
          if (pb[len] != cur[len])
            break;
        {
          if (len == lenLimit)
          {
            *ptr1 = pair[0];
            *ptr0 = pair[1];
            return;
          }
        }
      }
      if (pb[len] < cur[len])
      {
        *ptr1 = curMatch;
        ptr1 = pair + 1;
        curMatch = *ptr1;
        len1 = len;
      }
      else
      {
        *ptr0 = curMatch;
        ptr0 = pair;
        curMatch = *ptr0;
        len0 = len;
      }
    }
  }
}

#define MOVE_POS \
  ++p->cyclicBufferPos; \
  p->buffer++; \
  if (++p->pos == p->posLimit) Mf_CheckLimits(p);

#define MOVE_POS_RET MOVE_POS return offset;

static void Mf_MovePos(CMatchFinder *p) { MOVE_POS; }

#define GET_MATCHES_HEADER2(minLen, ret_op) \
  uint32_t lenLimit; uint32_t hashValue; const uint8_t *cur; uint32_t curMatch; \
  lenLimit = p->lenLimit; { if (lenLimit < minLen) { Mf_MovePos(p); ret_op; }} \
  cur = p->buffer;

#define GET_MATCHES_HEADER(minLen) GET_MATCHES_HEADER2(minLen, return 0)
#define SKIP_HEADER(minLen)        GET_MATCHES_HEADER2(minLen, continue)

#define MF_PARAMS(p) p->pos, p->buffer, p->son, p->cyclicBufferPos, p->cyclicBufferSize, p->cutValue

#define GET_MATCHES_FOOTER(offset, maxLen) \
  offset = (uint32_t)(GetMatchesSpec1(lenLimit, curMatch, MF_PARAMS(p), \
  distances + offset, maxLen) - distances); MOVE_POS_RET;

#define SKIP_FOOTER \
  SkipMatchesSpec(lenLimit, curMatch, MF_PARAMS(p)); MOVE_POS;

static uint32_t Bt2_MatchFinder_GetMatches(CMatchFinder *p, uint32_t *distances)
{
  uint32_t offset;
  GET_MATCHES_HEADER(2)
  HASH2_CALC;
  curMatch = p->hash[hashValue];
  p->hash[hashValue] = p->pos;
  offset = 0;
  GET_MATCHES_FOOTER(offset, 1)
}

static uint32_t Bt3_MatchFinder_GetMatches(CMatchFinder *p, uint32_t *distances)
{
  uint32_t hash2Value, delta2, maxLen, offset;
  GET_MATCHES_HEADER(3)

  HASH3_CALC;

  delta2 = p->pos - p->hash[hash2Value];
  curMatch = p->hash[kFix3HashSize + hashValue];

  p->hash[hash2Value] =
  p->hash[kFix3HashSize + hashValue] = p->pos;


  maxLen = 2;
  offset = 0;
  if (delta2 < p->cyclicBufferSize && *(cur - delta2) == *cur)
  {
    for (; maxLen != lenLimit; maxLen++)
      if (cur[(ptrdiff_t)maxLen - delta2] != cur[maxLen])
        break;
    distances[0] = maxLen;
    distances[1] = delta2 - 1;
    offset = 2;
    if (maxLen == lenLimit)
    {
      SkipMatchesSpec(lenLimit, curMatch, MF_PARAMS(p));
      MOVE_POS_RET;
    }
  }
  GET_MATCHES_FOOTER(offset, maxLen)
}

static uint32_t Bt4_MatchFinder_GetMatches(CMatchFinder *p, uint32_t *distances)
{
  uint32_t hash2Value, hash3Value, delta2, delta3, maxLen, offset;
  GET_MATCHES_HEADER(4)

  HASH4_CALC;

  delta2 = p->pos - p->hash[                hash2Value];
  delta3 = p->pos - p->hash[kFix3HashSize + hash3Value];
  curMatch = p->hash[kFix4HashSize + hashValue];

  p->hash[                hash2Value] =
  p->hash[kFix3HashSize + hash3Value] =
  p->hash[kFix4HashSize + hashValue] = p->pos;

  maxLen = 1;
  offset = 0;
  if (delta2 < p->cyclicBufferSize && *(cur - delta2) == *cur)
  {
    distances[0] = maxLen = 2;
    distances[1] = delta2 - 1;
    offset = 2;
  }
  if (delta2 != delta3 && delta3 < p->cyclicBufferSize && *(cur - delta3) == *cur)
  {
    maxLen = 3;
    distances[offset + 1] = delta3 - 1;
    offset += 2;
    delta2 = delta3;
  }
  if (offset != 0)
  {
    for (; maxLen != lenLimit; maxLen++)
      if (cur[(ptrdiff_t)maxLen - delta2] != cur[maxLen])
        break;
    distances[offset - 2] = maxLen;
    if (maxLen == lenLimit)
    {
      SkipMatchesSpec(lenLimit, curMatch, MF_PARAMS(p));
      MOVE_POS_RET;
    }
  }
  if (maxLen < 3)
    maxLen = 3;
  GET_MATCHES_FOOTER(offset, maxLen)
}

static uint32_t Hc4_MatchFinder_GetMatches(CMatchFinder *p, uint32_t *distances)
{
  uint32_t hash2Value, hash3Value, delta2, delta3, maxLen, offset;
  GET_MATCHES_HEADER(4)

  HASH4_CALC;

  delta2 = p->pos - p->hash[                hash2Value];
  delta3 = p->pos - p->hash[kFix3HashSize + hash3Value];
  curMatch = p->hash[kFix4HashSize + hashValue];

  p->hash[                hash2Value] =
  p->hash[kFix3HashSize + hash3Value] =
  p->hash[kFix4HashSize + hashValue] = p->pos;

  maxLen = 1;
  offset = 0;
  if (delta2 < p->cyclicBufferSize && *(cur - delta2) == *cur)
  {
    distances[0] = maxLen = 2;
    distances[1] = delta2 - 1;
    offset = 2;
  }
  if (delta2 != delta3 && delta3 < p->cyclicBufferSize && *(cur - delta3) == *cur)
  {
    maxLen = 3;
    distances[offset + 1] = delta3 - 1;
    offset += 2;
    delta2 = delta3;
  }
  if (offset != 0)
  {
    for (; maxLen != lenLimit; maxLen++)
      if (cur[(ptrdiff_t)maxLen - delta2] != cur[maxLen])
        break;
    distances[offset - 2] = maxLen;
    if (maxLen == lenLimit)
    {
      p->son[p->cyclicBufferPos] = curMatch;
      MOVE_POS_RET;
    }
  }
  if (maxLen < 3)
    maxLen = 3;
  offset = (uint32_t)(Hc_GetMatchesSpec(lenLimit, curMatch, MF_PARAMS(p),
    distances + offset, maxLen) - (distances));
  MOVE_POS_RET
}


static void Bt2_MatchFinder_Skip(CMatchFinder *p, uint32_t num)
{
  do
  {
    SKIP_HEADER(2)
    HASH2_CALC;
    curMatch = p->hash[hashValue];
    p->hash[hashValue] = p->pos;
    SKIP_FOOTER
  }
  while (--num != 0);
}


static void Bt3_MatchFinder_Skip(CMatchFinder *p, uint32_t num)
{
  do
  {
    uint32_t hash2Value;
    SKIP_HEADER(3)
    HASH3_CALC;
    curMatch = p->hash[kFix3HashSize + hashValue];
    p->hash[hash2Value] =
    p->hash[kFix3HashSize + hashValue] = p->pos;
    SKIP_FOOTER
  }
  while (--num != 0);
}

static void Bt4_MatchFinder_Skip(CMatchFinder *p, uint32_t num)
{
  do
  {
    uint32_t hash2Value, hash3Value;
    SKIP_HEADER(4)
    HASH4_CALC;
    curMatch = p->hash[kFix4HashSize + hashValue];
    p->hash[                hash2Value] =
    p->hash[kFix3HashSize + hash3Value] = p->pos;
    p->hash[kFix4HashSize + hashValue] = p->pos;
    SKIP_FOOTER
  }
  while (--num != 0);
}

static void Hc4_MatchFinder_Skip(CMatchFinder *p, uint32_t num)
{
  do
  {
    uint32_t hash2Value, hash3Value;
    SKIP_HEADER(4)
    HASH4_CALC;
    curMatch = p->hash[kFix4HashSize + hashValue];
    p->hash[                hash2Value] =
    p->hash[kFix3HashSize + hash3Value] =
    p->hash[kFix4HashSize + hashValue] = p->pos;
    p->son[p->cyclicBufferPos] = curMatch;
    MOVE_POS
  }
  while (--num != 0);
}


void Mf_CreateVTable(CMatchFinder *p, IMatchFinder *vTable)
{
  if (!p->btMode)
  {
    vTable->GetMatches = (Mf_GetMatches_Func)Hc4_MatchFinder_GetMatches;
    vTable->Skip = (Mf_Skip_Func)Hc4_MatchFinder_Skip;
  }
  else if (p->numHashBytes == 2)
  {
    vTable->GetMatches = (Mf_GetMatches_Func)Bt2_MatchFinder_GetMatches;
    vTable->Skip = (Mf_Skip_Func)Bt2_MatchFinder_Skip;
  }
  else if (p->numHashBytes == 3)
  {
    vTable->GetMatches = (Mf_GetMatches_Func)Bt3_MatchFinder_GetMatches;
    vTable->Skip = (Mf_Skip_Func)Bt3_MatchFinder_Skip;
  }
  else
  {
    vTable->GetMatches = (Mf_GetMatches_Func)Bt4_MatchFinder_GetMatches;
    vTable->Skip = (Mf_Skip_Func)Bt4_MatchFinder_Skip;
  }
}
