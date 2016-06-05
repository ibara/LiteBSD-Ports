/* LzFind.h -- Match finder for LZ algorithms
2009-04-22 : Igor Pavlov : Public domain */

typedef uint32_t CLzRef;

typedef struct
{
  uint8_t *bufferBase;
  uint8_t *buffer;
  CLzRef *hash;
  CLzRef *son;
  uint32_t pos;
  uint32_t posLimit;
  uint32_t streamPos;
  uint32_t lenLimit;

  uint32_t cyclicBufferPos;
  uint32_t cyclicBufferSize; /* it must be = (historySize + 1) */

  uint32_t matchMaxLen;
  uint32_t hashMask;
  uint32_t cutValue;

  uint32_t blockSize;
  uint32_t keepSizeBefore;
  uint32_t keepSizeAfter;

  uint32_t numHashBytes;
  uint32_t historySize;
  uint32_t hashSizeSum;
  uint32_t numSons;
  int infd;
  int result;
  uint32_t crc;
  bool btMode;
  bool streamEndWasReached;
} CMatchFinder;


/* Conditions:
     historySize <= 3 GB
     keepAddBufferBefore + matchMaxLen + keepAddBufferAfter < 511MB
*/
int Mf_Init(CMatchFinder *p, const int ifd, const int mc, uint32_t historySize,
    uint32_t keepAddBufferBefore, uint32_t matchMaxLen, uint32_t keepAddBufferAfter);

void Mf_Free(CMatchFinder *p);


/*
Conditions:
  Mf_GetNumAvailableBytes_Func must be called before each Mf_GetMatchLen_Func.
  Mf_GetPointerToCurrentPos_Func's result must be used only before any other function
*/

typedef uint32_t (*Mf_GetMatches_Func)(void *object, uint32_t *distances);
typedef void (*Mf_Skip_Func)(void *object, uint32_t);

typedef struct _IMatchFinder
{
  Mf_GetMatches_Func GetMatches;
  Mf_Skip_Func Skip;
} IMatchFinder;

void Mf_CreateVTable(CMatchFinder *p, IMatchFinder *vTable);

static inline uint32_t Mf_GetNumAvailableBytes(CMatchFinder *p)
  { return p->streamPos - p->pos; }

static inline uint8_t Mf_GetIndexByte(CMatchFinder *p, int index)
  { return p->buffer[index]; }

static inline uint8_t * Mf_GetPointerToCurrentPos(CMatchFinder *p)
  { return p->buffer; }
