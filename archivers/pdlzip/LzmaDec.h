/* LzmaDec.h -- LZMA Decoder
2009-02-07 : Igor Pavlov : Public domain */

/* ---------- LZMA Properties ---------- */

#define LZMA_PROPS_SIZE 5


/* ---------- LZMA Decoder state ---------- */

/* LZMA_REQUIRED_INPUT_MAX = number of required input bytes for worst case.
   Num bits = log2((2^11 / 31) ^ 22) + 26 < 134 + 26 = 160; */

#define LZMA_REQUIRED_INPUT_MAX 20

typedef struct
{
  int *probs;
  uint8_t *dic;
  const uint8_t *buf;
  uint32_t range, code;
  uint32_t dicPos;
  uint32_t dicBufSize;
  uint32_t processedPos;
  uint32_t checkDicSize;
  unsigned lc, lp, pb;
  State state;
  uint32_t reps[4];
  unsigned remainLen;
  uint32_t numProbs;
  unsigned tempBufSize;
  bool needFlush;
  uint8_t tempBuf[LZMA_REQUIRED_INPUT_MAX];
} CLzmaDec;


/* There are two types of LZMA streams:
     0) Stream with end mark. That end mark adds about 6 bytes to compressed size.
     1) Stream without end mark. You must know exact uncompressed size to decompress such stream. */

typedef enum
{
  LZMA_FINISH_ANY,   /* finish at any point */
  LZMA_FINISH_END    /* block must be finished at the end */
} ELzmaFinishMode;

/* ELzmaFinishMode has meaning only if the decoding reaches output limit !!!

   You must use LZMA_FINISH_END, when you know that current output buffer
   covers last bytes of block. In other cases you must use LZMA_FINISH_ANY.

   If LZMA decoder sees end marker before reaching output limit, it returns SZ_OK,
   and output value of destLen will be less than output buffer size limit.
   You can check status result also.

   You can use multiple checks to test data integrity after full decompression:
     1) Check Result and "status" variable.
     2) Check that output(destLen) = uncompressedSize, if you know real uncompressedSize.
     3) Check that output(srcLen) = compressedSize, if you know real compressedSize.
        You must use correct finish mode in that case. */

typedef enum
{
  LZMA_STATUS_NOT_SPECIFIED,               /* use main error code instead */
  LZMA_STATUS_FINISHED_WITH_MARK,          /* stream was finished with end mark. */
  LZMA_STATUS_NOT_FINISHED,                /* stream was not finished */
  LZMA_STATUS_NEEDS_MORE_INPUT,            /* you must provide more input bytes */
  LZMA_STATUS_MAYBE_FINISHED_WITHOUT_MARK  /* there is probability that stream was finished without end mark */
} ELzmaStatus;

/* ELzmaStatus is used only as output value for function call */


bool LzmaDec_Init(CLzmaDec *p, const uint8_t *raw_props);
void LzmaDec_Free(CLzmaDec *p);


/* ---------- Buffer Interface ---------- */

/* It's zlib-like interface.

finishMode:
  It has meaning only if the decoding reaches output limit (*destLen).
  LZMA_FINISH_ANY - Decode just destLen bytes.
  LZMA_FINISH_END - Stream must be finished after (*destLen).
*/

bool LzmaDec_DecodeToBuf( CLzmaDec *p, uint8_t *dest, uint32_t *destLen,
                          const uint8_t *src, uint32_t *srcLen,
                          ELzmaFinishMode finishMode, ELzmaStatus *status );
