/*  Pdlzip - LZMA lossless data compressor
    Copyright (C) 2010-2016 Antonio Diaz Diaz.

    This program is free software. Redistribution and use in source and
    binary forms, with or without modification, are permitted provided
    that the following conditions are met:

    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#ifndef max
  #define max(x,y) ((x) >= (y) ? (x) : (y))
#endif
#ifndef min
  #define min(x,y) ((x) <= (y) ? (x) : (y))
#endif

typedef int State;

enum {
  min_dictionary_bits = 12,
  min_dictionary_size = 1 << min_dictionary_bits,
  max_dictionary_bits = 27,			/* kDicLogSizeMaxCompress */
  max_dictionary_size = 1 << max_dictionary_bits,
  literal_context_bits = 3,
  literal_pos_state_bits = 0,				/* not used */
  pos_state_bits = 2,

  len_low_bits = 3,
  len_mid_bits = 3,
  len_high_bits = 8,
  len_low_symbols = 1 << len_low_bits,
  len_mid_symbols = 1 << len_mid_bits,
  len_high_symbols = 1 << len_high_bits,
  max_len_symbols = len_low_symbols + len_mid_symbols + len_high_symbols,

  min_match_len = 2,					/* must be 2 */
  max_match_len = min_match_len + max_len_symbols - 1,	/* 273 */
  min_match_len_limit = 5 };


struct Pretty_print
  {
  const char * name;
  const char * stdin_name;
  unsigned longest_name;
  bool first_post;
  };

static inline void Pp_init( struct Pretty_print * const pp,
                            const char * const filenames[],
                            const int num_filenames, const int verbosity )
  {
  unsigned stdin_name_len;
  int i;
  pp->name = 0;
  pp->stdin_name = "(stdin)";
  pp->longest_name = 0;
  pp->first_post = false;
  stdin_name_len = strlen( pp->stdin_name );

  if( verbosity <= 0 ) return;
  for( i = 0; i < num_filenames; ++i )
    {
    const char * const s = filenames[i];
    const unsigned len = (strcmp( s, "-" ) == 0) ? stdin_name_len : strlen( s );
    if( len > pp->longest_name ) pp->longest_name = len;
    }
  if( pp->longest_name == 0 ) pp->longest_name = stdin_name_len;
  }

static inline void Pp_set_name( struct Pretty_print * const pp,
                                const char * const filename )
  {
  if( filename && filename[0] && strcmp( filename, "-" ) != 0 )
    pp->name = filename;
  else pp->name = pp->stdin_name;
  pp->first_post = true;
  }

static inline void Pp_reset( struct Pretty_print * const pp )
  { if( pp->name && pp->name[0] ) pp->first_post = true; }
void Pp_show_msg( struct Pretty_print * const pp, const char * const msg );


typedef uint32_t CRC32[256];	/* Table of CRCs of all 8-bit messages. */

extern CRC32 crc32;

static inline void CRC32_init( void )
  {
  unsigned n;
  for( n = 0; n < 256; ++n )
    {
    unsigned c = n;
    int k;
    for( k = 0; k < 8; ++k )
      { if( c & 1 ) c = 0xEDB88320U ^ ( c >> 1 ); else c >>= 1; }
    crc32[n] = c;
    }
  }

static inline void CRC32_update_buf( uint32_t * const crc,
                                     const uint8_t * const buffer,
                                     const int size )
  {
  int i;
  for( i = 0; i < size; ++i )
    *crc = crc32[(*crc^buffer[i])&0xFF] ^ ( *crc >> 8 );
  }


static inline bool isvalid_ds( const unsigned dictionary_size )
  { return ( dictionary_size >= min_dictionary_size &&
             dictionary_size <= max_dictionary_size ); }


static inline int real_bits( unsigned value )
  {
  int bits = 0;
  while( value > 0 ) { value >>= 1; ++bits; }
  return bits;
  }


static const uint8_t magic_string[4] = { 0x4C, 0x5A, 0x49, 0x50 }; /* "LZIP" */

typedef uint8_t File_header[6];		/* 0-3 magic bytes */
					/*   4 version */
					/*   5 coded_dict_size */
enum { Fh_size = 6 };

static inline void Fh_set_magic( File_header data )
  { memcpy( data, magic_string, 4 ); data[4] = 1; }

static inline bool Fh_verify_magic( const File_header data )
  { return ( memcmp( data, magic_string, 4 ) == 0 ); }

/* detect truncated header */
static inline bool Fh_verify_prefix( const File_header data, const int size )
  {
  int i; for( i = 0; i < size && i < 4; ++i )
    if( data[i] != magic_string[i] ) return false;
  return ( size > 0 );
  }

static inline uint8_t Fh_version( const File_header data )
  { return data[4]; }

static inline bool Fh_verify_version( const File_header data )
  { return ( data[4] == 1 ); }

static inline unsigned Fh_get_dictionary_size( const File_header data )
  {
  unsigned sz = ( 1 << ( data[5] & 0x1F ) );
  if( sz > min_dictionary_size )
    sz -= ( sz / 16 ) * ( ( data[5] >> 5 ) & 7 );
  return sz;
  }

static inline bool Fh_set_dictionary_size( File_header data, const unsigned sz )
  {
  if( !isvalid_ds( sz ) ) return false;
  data[5] = real_bits( sz - 1 );
  if( sz > min_dictionary_size )
    {
    const unsigned base_size = 1 << data[5];
    const unsigned fraction = base_size / 16;
    int i;
    for( i = 7; i >= 1; --i )
      if( base_size - ( i * fraction ) >= sz )
        { data[5] |= ( i << 5 ); break; }
    }
  return true;
  }


typedef uint8_t File_trailer[20];
			/*  0-3  CRC32 of the uncompressed data */
			/*  4-11 size of the uncompressed data */
			/* 12-19 member size including header and trailer */

enum { Ft_size = 20 };

static inline unsigned Ft_get_data_crc( const File_trailer data )
  {
  unsigned tmp = 0;
  int i; for( i = 3; i >= 0; --i ) { tmp <<= 8; tmp += data[i]; }
  return tmp;
  }

static inline void Ft_set_data_crc( File_trailer data, unsigned crc )
  { int i; for( i = 0; i <= 3; ++i ) { data[i] = (uint8_t)crc; crc >>= 8; } }

static inline unsigned long long Ft_get_data_size( const File_trailer data )
  {
  unsigned long long tmp = 0;
  int i; for( i = 11; i >= 4; --i ) { tmp <<= 8; tmp += data[i]; }
  return tmp;
  }

static inline void Ft_set_data_size( File_trailer data, unsigned long long sz )
  { int i; for( i = 4; i <= 11; ++i ) { data[i] = (uint8_t)sz; sz >>= 8; } }

static inline unsigned long long Ft_get_member_size( const File_trailer data )
  {
  unsigned long long tmp = 0;
  int i; for( i = 19; i >= 12; --i ) { tmp <<= 8; tmp += data[i]; }
  return tmp;
  }

static inline void Ft_set_member_size( File_trailer data, unsigned long long sz )
  { int i; for( i = 12; i <= 19; ++i ) { data[i] = (uint8_t)sz; sz >>= 8; } }


/* defined in main.c */
int readblock( const int fd, uint8_t * const buf, const int size );
int writeblock( const int fd, const uint8_t * const buf, const int size );

/* defined in main.c */
extern int verbosity;
void cleanup_and_fail( const int retval );
void show_error( const char * const msg, const int errcode, const bool help );
void internal_error( const char * const msg );

#define SZ_OK 0

#define SZ_ERROR_READ 8
#define SZ_ERROR_WRITE 9
