/* LzmaEnc.h -- LZMA Encoder
2009-02-07 : Igor Pavlov : Public domain */


/* ---------- CLzmaEncHandle Interface ---------- */

/* LzmaEnc_* functions can return the following exit codes:
Returns:
  SZ_OK           - OK
  SZ_ERROR_WRITE  - Write callback error.
*/

typedef void * CLzmaEncHandle;

CLzmaEncHandle LzmaEnc_Init( const int dict_size, const int match_len_limit,
                             const int infd, const int outfd );
void LzmaEnc_Free(CLzmaEncHandle p);
int LzmaEnc_Encode(CLzmaEncHandle p);
