/*  Pdlzip - LZMA lossless data compressor
    2009-08-14 : Igor Pavlov : Public domain
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
/*
    Exit status: 0 for a normal exit, 1 for environmental problems
    (file not found, invalid flags, I/O errors, etc), 2 to indicate a
    corrupt or invalid input file, 3 for an internal consistency error
    (eg, bug) which caused pdlzip to panic.
*/

#define _FILE_OFFSET_BITS 64

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <utime.h>
#include <sys/stat.h>
#if defined(__MSVCRT__)
#include <io.h>
#define fchmod(x,y) 0
#define fchown(x,y,z) 0
#define SIGHUP SIGTERM
#define S_ISSOCK(x) 0
#define S_IRGRP 0
#define S_IWGRP 0
#define S_IROTH 0
#define S_IWOTH 0
#endif
#if defined(__OS2__)
#include <io.h>
#endif

#include "carg_parser.h"
#include "lzip.h"
#include "LzmaDec.h"
#include "LzmaEnc.h"

#ifndef O_BINARY
#define O_BINARY 0
#endif

#if CHAR_BIT != 8
#error "Environments where CHAR_BIT != 8 are not supported."
#endif

int verbosity = 0;

const char * const Program_name = "Pdlzip";
const char * const program_name = "pdlzip";
const char * const program_year = "2016";
const char * invocation_name = 0;

struct { const char * from; const char * to; } const known_extensions[] = {
  { ".lz",   ""     },
  { ".tlz",  ".tar" },
  { ".lzma", ""     },
  { 0,       0      } };

struct Lzma_options
  {
  int dictionary_size;		/* 4 KiB .. 512 MiB */
  int match_len_limit;		/* 5 .. 273 */
  };

enum Mode { m_compress, m_decompress, m_test };

char * output_filename = 0;
int outfd = -1;
bool delete_output_on_interrupt = false;


static void show_help( void )
  {
  printf( "%s - A permissively licensed implementation of the lzip data\n", Program_name );
  printf( "compressor also able to decompress legacy lzma-alone (.lzma) files.\n"
          "\nLzma-alone is a very bad format; it is essentially a raw LZMA stream.\n"
          "If you keep any lzma-alone files, it is advisable to recompress them to\n"
          "lzip format. Lziprecover can convert lzma-alone files to lzip format\n"
          "without recompressing.\n"
          "\nUsage: %s [options] [files]\n", invocation_name );
  printf( "\nOptions:\n"
          "  -h, --help                     display this help and exit\n"
          "  -V, --version                  output version information and exit\n"
          "  -a, --trailing-error           exit with error status if trailing data\n"
          "  -c, --stdout                   write to standard output, keep input files\n"
          "  -d, --decompress               decompress\n"
          "  -f, --force                    overwrite existing output files\n"
          "  -F, --recompress               force re-compression of compressed files\n"
          "  -k, --keep                     keep (don't delete) input files\n"
          "  -m, --match-length=<bytes>     set match length limit in bytes [36]\n"
          "  -o, --output=<file>            if reading standard input, write to <file>\n"
          "  -q, --quiet                    suppress all messages\n"
          "  -s, --dictionary-size=<bytes>  set dictionary size limit in bytes [8 MiB]\n"
          "  -t, --test                     test compressed file integrity\n"
          "  -v, --verbose                  be verbose (a 2nd -v gives more)\n"
          "  -1 .. -9                       set compression level [default 6]\n"
          "      --fast                     alias for -1\n"
          "      --best                     alias for -9\n"
          "If no file names are given, or if a file is '-', pdlzip compresses or\n"
          "decompresses from standard input to standard output.\n"
          "Numbers may be followed by a multiplier: k = kB = 10^3 = 1000,\n"
          "Ki = KiB = 2^10 = 1024, M = 10^6, Mi = 2^20, G = 10^9, Gi = 2^30, etc...\n"
          "Dictionary sizes 12 to 27 are interpreted as powers of two, meaning 2^12\n"
          "to 2^27 bytes.\n"
          "\nThe bidimensional parameter space of LZMA can't be mapped to a linear\n"
          "scale optimal for all files. If your files are large, very repetitive,\n"
          "etc, you may need to use the --dictionary-size and --match-length\n"
          "options directly to achieve optimal performance. For example, -9m64\n"
          "usually compresses executables more (and faster) than -9.\n"
          "\nExit status: 0 for a normal exit, 1 for environmental problems (file\n"
          "not found, invalid flags, I/O errors, etc), 2 to indicate a corrupt or\n"
          "invalid input file, 3 for an internal consistency error (eg, bug) which\n"
          "caused pdlzip to panic.\n"
          "\nReport bugs to lzip-bug@nongnu.org\n"
          "Pdlzip home page: http://www.nongnu.org/lzip/pdlzip.html\n" );
  }


static void show_version( void )
  {
  printf( "%s %s\n", program_name, PROGVERSION );
  printf( "Copyright (C) %s Antonio Diaz Diaz.\n", program_year );
  printf( "Public Domain 2009 Igor Pavlov.\n"
          "License 2-clause BSD.\n"
          "This is free software: you are free to change and redistribute it.\n"
          "There is NO WARRANTY, to the extent permitted by law.\n" );
  }


static void show_header( const unsigned dictionary_size )
  {
  if( verbosity >= 3 )
    {
    const char * const prefix[8] =
      { "Ki", "Mi", "Gi", "Ti", "Pi", "Ei", "Zi", "Yi" };
    enum { factor = 1024 };
    const char * p = "";
    const char * np = "  ";
    unsigned num = dictionary_size, i;
    bool exact = ( num % factor == 0 );

    for( i = 0; i < 8 && ( num > 9999 || ( exact && num >= factor ) ); ++i )
      { num /= factor; if( num % factor != 0 ) exact = false;
        p = prefix[i]; np = ""; }
    fprintf( stderr, "dictionary size %s%4u %sB.  ", np, num, p );
    }
  }


static unsigned long getnum( const char * const ptr,
                             const unsigned long llimit,
                             const unsigned long ulimit )
  {
  unsigned long result;
  char * tail;
  errno = 0;
  result = strtoul( ptr, &tail, 0 );
  if( tail == ptr )
    {
    show_error( "Bad or missing numerical argument.", 0, true );
    exit( 1 );
    }

  if( !errno && tail[0] )
    {
    const int factor = ( tail[1] == 'i' ) ? 1024 : 1000;
    int exponent = 0;				/* 0 = bad multiplier */
    int i;
    switch( tail[0] )
      {
      case 'Y': exponent = 8; break;
      case 'Z': exponent = 7; break;
      case 'E': exponent = 6; break;
      case 'P': exponent = 5; break;
      case 'T': exponent = 4; break;
      case 'G': exponent = 3; break;
      case 'M': exponent = 2; break;
      case 'K': if( factor == 1024 ) exponent = 1; break;
      case 'k': if( factor == 1000 ) exponent = 1; break;
      }
    if( exponent <= 0 )
      {
      show_error( "Bad multiplier in numerical argument.", 0, true );
      exit( 1 );
      }
    for( i = 0; i < exponent; ++i )
      {
      if( ulimit / factor >= result ) result *= factor;
      else { errno = ERANGE; break; }
      }
    }
  if( !errno && ( result < llimit || result > ulimit ) ) errno = ERANGE;
  if( errno )
    {
    show_error( "Numerical argument out of limits.", 0, false );
    exit( 1 );
    }
  return result;
  }


static int get_dict_size( const char * const arg )
  {
  char * tail;
  const int bits = strtol( arg, &tail, 0 );
  if( bits >= min_dictionary_bits &&
      bits <= max_dictionary_bits && *tail == 0 )
    return ( 1 << bits );
  return getnum( arg, min_dictionary_size, max_dictionary_size );
  }


static int extension_index( const char * const name )
  {
  int i;
  for( i = 0; known_extensions[i].from; ++i )
    {
    const char * const ext = known_extensions[i].from;
    const unsigned name_len = strlen( name );
    const unsigned ext_len = strlen( ext );
    if( name_len > ext_len &&
        strncmp( name + name_len - ext_len, ext, ext_len ) == 0 )
      return i;
    }
  return -1;
  }


static int open_instream( const char * const name, struct stat * const in_statsp,
                          const enum Mode program_mode, const int eindex,
                          const bool recompress, const bool to_stdout )
  {
  int infd = -1;
  if( program_mode == m_compress && !recompress && eindex >= 0 )
    {
    if( verbosity >= 0 )
      fprintf( stderr, "%s: Input file '%s' already has '%s' suffix.\n",
               program_name, name, known_extensions[eindex].from );
    }
  else
    {
    infd = open( name, O_RDONLY | O_BINARY );
    if( infd < 0 )
      {
      if( verbosity >= 0 )
        fprintf( stderr, "%s: Can't open input file '%s': %s\n",
                 program_name, name, strerror( errno ) );
      }
    else
      {
      const int i = fstat( infd, in_statsp );
      const mode_t mode = in_statsp->st_mode;
      const bool can_read = ( i == 0 &&
                              ( S_ISBLK( mode ) || S_ISCHR( mode ) ||
                                S_ISFIFO( mode ) || S_ISSOCK( mode ) ) );
      const bool no_ofile = ( to_stdout || program_mode == m_test );
      if( i != 0 || ( !S_ISREG( mode ) && ( !can_read || !no_ofile ) ) )
        {
        if( verbosity >= 0 )
          fprintf( stderr, "%s: Input file '%s' is not a regular file%s.\n",
                   program_name, name,
                   ( can_read && !no_ofile ) ?
                   ",\n        and '--stdout' was not specified" : "" );
        close( infd );
        infd = -1;
        }
      }
    }
  return infd;
  }


/* assure at least a minimum size for buffer 'buf' */
static void * resize_buffer( void * buf, const int min_size )
  {
  if( buf ) buf = realloc( buf, min_size );
  else buf = malloc( min_size );
  if( !buf )
    {
    show_error( "Not enough memory.", 0, false );
    cleanup_and_fail( 1 );
    }
  return buf;
  }


static void set_c_outname( const char * const name )
  {
  output_filename = resize_buffer( output_filename, strlen( name ) +
                                   strlen( known_extensions[0].from ) + 1 );
  strcpy( output_filename, name );
  strcat( output_filename, known_extensions[0].from );
  }


static void set_d_outname( const char * const name, const int i )
  {
  const unsigned name_len = strlen( name );
  if( i >= 0 )
    {
    const char * const from = known_extensions[i].from;
    const unsigned from_len = strlen( from );
    if( name_len > from_len )
      {
      output_filename = resize_buffer( output_filename, name_len +
                                       strlen( known_extensions[0].to ) + 1 );
      strcpy( output_filename, name );
      strcpy( output_filename + name_len - from_len, known_extensions[i].to );
      return;
      }
    }
  output_filename = resize_buffer( output_filename, name_len + 4 + 1 );
  strcpy( output_filename, name );
  strcat( output_filename, ".out" );
  if( verbosity >= 1 )
    fprintf( stderr, "%s: Can't guess original name for '%s' -- using '%s'\n",
             program_name, name, output_filename );
  }


static bool open_outstream( const bool force, const bool from_stdin )
  {
  const mode_t usr_rw = S_IRUSR | S_IWUSR;
  const mode_t all_rw = usr_rw | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
  const mode_t outfd_mode = from_stdin ? all_rw : usr_rw;
  int flags = O_CREAT | O_WRONLY | O_BINARY;
  if( force ) flags |= O_TRUNC; else flags |= O_EXCL;

  outfd = open( output_filename, flags, outfd_mode );
  if( outfd >= 0 ) delete_output_on_interrupt = true;
  else if( verbosity >= 0 )
    {
    if( errno == EEXIST )
      fprintf( stderr, "%s: Output file '%s' already exists, skipping.\n",
               program_name, output_filename );
    else
      fprintf( stderr, "%s: Can't create output file '%s': %s\n",
               program_name, output_filename, strerror( errno ) );
    }
  return ( outfd >= 0 );
  }


static bool check_tty( const int infd, const enum Mode program_mode )
  {
  if( program_mode == m_compress && isatty( outfd ) )
    {
    show_error( "I won't write compressed data to a terminal.", 0, true );
    return false;
    }
  if( ( program_mode == m_decompress || program_mode == m_test ) &&
      isatty( infd ) )
    {
    show_error( "I won't read compressed data from a terminal.", 0, true );
    return false;
    }
  return true;
  }


void cleanup_and_fail( const int retval )
  {
  if( delete_output_on_interrupt )
    {
    delete_output_on_interrupt = false;
    if( verbosity >= 0 )
      fprintf( stderr, "%s: Deleting output file '%s', if it exists.\n",
               program_name, output_filename );
    if( outfd >= 0 ) { close( outfd ); outfd = -1; }
    if( remove( output_filename ) != 0 && errno != ENOENT )
      show_error( "WARNING: deletion of output file (apparently) failed.", 0, false );
    }
  exit( retval );
  }


     /* Set permissions, owner and times. */
static void close_and_set_permissions( const struct stat * const in_statsp )
  {
  bool warning = false;
  if( in_statsp )
    {
    const mode_t mode = in_statsp->st_mode;
    /* fchown will in many cases return with EPERM, which can be safely ignored. */
    if( fchown( outfd, in_statsp->st_uid, in_statsp->st_gid ) == 0 )
      { if( fchmod( outfd, mode ) != 0 ) warning = true; }
    else
      if( errno != EPERM ||
          fchmod( outfd, mode & ~( S_ISUID | S_ISGID | S_ISVTX ) ) != 0 )
        warning = true;
    }
  if( close( outfd ) != 0 )
    {
    show_error( "Error closing output file", errno, false );
    cleanup_and_fail( 1 );
    }
  outfd = -1;
  delete_output_on_interrupt = false;
  if( in_statsp )
    {
    struct utimbuf t;
    t.actime = in_statsp->st_atime;
    t.modtime = in_statsp->st_mtime;
    if( utime( output_filename, &t ) != 0 ) warning = true;
    }
  if( warning && verbosity >= 1 )
    show_error( "Can't change output file attributes.", 0, false );
  }


static int compress( const struct Lzma_options * const encoder_options,
                     const int infd, struct Pretty_print * const pp )
  {
  int retval = 0;
  CLzmaEncHandle encoder = 0;
  File_header header;
  Fh_set_magic( header );

  if( verbosity >= 1 ) Pp_show_msg( pp, 0 );
  if( Fh_set_dictionary_size( header, encoder_options->dictionary_size ) &&
      encoder_options->match_len_limit >= min_match_len_limit &&
      encoder_options->match_len_limit <= max_match_len )
    encoder = LzmaEnc_Init( Fh_get_dictionary_size( header ),
                            encoder_options->match_len_limit, infd, outfd );
  else internal_error( "invalid argument to encoder." );

  if( !encoder )
    {
    Pp_show_msg( pp, "Not enough memory. Try a smaller dictionary size." );
    return 1;
    }

  if( writeblock( outfd, header, Fh_size ) != Fh_size )
    { show_error( "Can't write output file", errno, false ); retval = 1; }
  else
    if( LzmaEnc_Encode( encoder ) != 0 )
      { Pp_show_msg( pp, "Encoder error." ); retval = 1; }
  LzmaEnc_Free( encoder );
  return retval;
  }


#define IN_BUF_SIZE (1 << 16)
#define OUT_BUF_SIZE (1 << 16)

static bool read_inbuf( const int infd, uint8_t inBuf[],
                        int * const inPos, int * const inSize )
  {
  int rest;
  if( *inPos >= *inSize ) *inSize = 0;
  else if( *inPos > 0 )
    {
    memmove( inBuf, inBuf + *inPos, *inSize - *inPos );
    *inSize -= *inPos;
    }
  *inPos = 0;
  rest = IN_BUF_SIZE - *inSize;
  if( rest > 0 )
    {
    const int rd = readblock( infd, inBuf + *inSize, rest );
    if( rd < rest && errno )
      { show_error( "Read error", errno, false ); return false; }
    *inSize += rd;
    }
  return true;
  }


/* 5 bytes of LZMA properties and 8 bytes of uncompressed size */
enum { lzma_header_size = LZMA_PROPS_SIZE + 8 };

static int lzma_decode( uint64_t unpackSize, CLzmaDec *decoder, const int infd,
                        uint8_t inBuf[], int * const inPos,
                        int * const inSize, const bool testing )
  {
  unsigned long long total_in = lzma_header_size, total_out = 0;
  uint8_t outBuf[OUT_BUF_SIZE];
  int outPos = 0;
  const bool thereIsSize = (unpackSize != (uint64_t)-1);

  for (;;)
    {
    uint32_t inProcessed;
    uint32_t outProcessed = OUT_BUF_SIZE - outPos;
    ELzmaFinishMode finishMode = LZMA_FINISH_ANY;
    ELzmaStatus status;

    if( *inPos >= *inSize && !read_inbuf( infd, inBuf, inPos, inSize ) )
      return 1;
    inProcessed = *inSize - *inPos;
    if (thereIsSize && outProcessed > unpackSize)
      {
      outProcessed = unpackSize;
      finishMode = LZMA_FINISH_END;
      }

    if( !LzmaDec_DecodeToBuf( decoder, outBuf + outPos, &outProcessed,
        inBuf + *inPos, &inProcessed, finishMode, &status ) )
      { show_error( "Data error.", 0, false ); return 2; }
    *inPos += inProcessed;
    total_in += inProcessed;
    outPos += outProcessed;
    unpackSize -= outProcessed;

    if( outfd >= 0 && writeblock( outfd, outBuf, outPos ) != outPos )
      { show_error( "Can't write output file", errno, false ); return 1; }

    total_out += outPos;
    outPos = 0;

    if( ( inProcessed == 0 && outProcessed == 0 ) ||
        ( thereIsSize && unpackSize == 0 ) )
      {
      if( ( thereIsSize && unpackSize != 0 ) ||
          ( !thereIsSize && status != LZMA_STATUS_FINISHED_WITH_MARK ) )
        { show_error( "Data error.", 0, false ); return 2; }
      if( verbosity >= 2 && total_out > 0 && total_in > 0 )
        fprintf( stderr, "%6.3f:1, %6.3f bits/byte, %5.2f%% saved.  ",
                 (double)total_out / total_in,
                 ( 8.0 * total_in ) / total_out,
                 100.0 * ( 1.0 - ( (double)total_in / total_out ) ) );
      if( verbosity >= 4 )
        fprintf( stderr, "uncompressed size %9llu, compressed size %8llu.  ",
                 total_out, total_in );
      if( verbosity >= 2 )
        fputs( "lzma-alone, ", stderr );
      if( verbosity >= 1 )
        fputs( testing ? "(apparently) ok\n" : "(apparently) done\n", stderr );
      return 0;
      }
    }
  }


static int lzip_decode( CLzmaDec *decoder, const int infd, uint8_t inBuf[],
                        int * const inPos, int * const inSize )
  {
  unsigned long long total_in = Fh_size, total_out = 0;
  uint8_t outBuf[OUT_BUF_SIZE];
  int outPos = 0;
  uint32_t crc = 0xFFFFFFFFU;

  for (;;)
    {
    uint32_t inProcessed;
    uint32_t outProcessed = OUT_BUF_SIZE - outPos;
    ELzmaFinishMode finishMode = LZMA_FINISH_ANY;
    ELzmaStatus status;

    if( *inPos >= *inSize && !read_inbuf( infd, inBuf, inPos, inSize ) )
      return 1;
    if( *inPos >= *inSize )
      { show_error( "Unexpected EOF.", 0, false ); return 2; }
    inProcessed = *inSize - *inPos;

    if( !LzmaDec_DecodeToBuf( decoder, outBuf + outPos, &outProcessed,
        inBuf + *inPos, &inProcessed, finishMode, &status ) )
      { show_error( "Data error.", 0, false ); return 2; }
    *inPos += inProcessed;
    total_in += inProcessed;
    outPos += outProcessed;

    if( outfd >= 0 && writeblock( outfd, outBuf, outPos ) != outPos )
      { show_error( "Can't write output file", errno, false ); return 1; }

    CRC32_update_buf( &crc, outBuf, outPos );
    total_out += outPos;
    outPos = 0;

    if (inProcessed == 0 && outProcessed == 0)
      {
      File_trailer trailer;
      int i;
      bool error = false;

      if( status != LZMA_STATUS_FINISHED_WITH_MARK )
        { show_error( "Data error.", 0, false ); return 2; }
      if( *inSize - *inPos < Ft_size &&
          !read_inbuf( infd, inBuf, inPos, inSize ) ) return 1;
      if( *inSize - *inPos < Ft_size )
        {
        error = true;
        if( verbosity >= 0 )
          fprintf( stderr, "Trailer truncated at trailer position %u;"
                           " some checks may fail.\n",
                           (unsigned)(*inSize - *inPos) );
        for( i = *inSize - *inPos; i < Ft_size; ++i )
          inBuf[*inPos+i] = 0;
        }
      for( i = 0; i < Ft_size; ++i )
        trailer[i] = inBuf[(*inPos)++];
      total_in += Ft_size;
      crc ^= 0xFFFFFFFFU;
      if( Ft_get_data_crc( trailer ) != crc )
        {
        error = true;
        if( verbosity >= 0 )
          fprintf( stderr, "CRC mismatch; trailer says %08X, data crc is %08X\n",
                   Ft_get_data_crc( trailer ), crc );
        }
      if( Ft_get_data_size( trailer ) != total_out )
        {
        error = true;
        if( verbosity >= 0 )
          fprintf( stderr, "Data size mismatch; trailer says %llu, data size is %llu (0x%llX)\n",
                   Ft_get_data_size( trailer ), total_out, total_out );
        }
      if( Ft_get_member_size( trailer ) != total_in )
        {
        error = true;
        if( verbosity >= 0 )
          fprintf( stderr, "Member size mismatch; trailer says %llu, member size is %llu (0x%llX)\n",
                   Ft_get_member_size( trailer ), total_in, total_in );
        }
      if( !error && verbosity >= 2 && total_out > 0 && total_in > 0 )
        fprintf( stderr, "%6.3f:1, %6.3f bits/byte, %5.2f%% saved.  ",
                 (double)total_out / total_in,
                 ( 8.0 * total_in ) / total_out,
                 100.0 * ( 1.0 - ( (double)total_in / total_out ) ) );
      if( !error && verbosity >= 4 )
        fprintf( stderr, "data CRC %08X, data size %9llu, member size %8llu.  ",
                 crc, total_out, total_in );
      if( error ) return 2;
      return 0;
      }
    }
  }


static int decompress( const int infd, struct Pretty_print * const pp,
                       const bool ignore_trailing, const bool testing )
  {
  uint64_t unpackSize = 0;
  CLzmaDec decoder;
  uint8_t inBuf[IN_BUF_SIZE];
  int inPos = 0, inSize = 0;
  int retval = 0;
  bool lzip_mode = true;
  bool first_member;
  uint8_t raw_props[lzma_header_size];

  for( first_member = true; ; first_member = false )
    {
    int i, size;
    unsigned dictionary_size;
    File_header header;
    if( inSize - inPos < lzma_header_size &&
        !read_inbuf( infd, inBuf, &inPos, &inSize ) ) return 1;
    size = inSize - inPos;
    for( i = 0; i < size && i < Fh_size; ++i )
      raw_props[i] = header[i] = inBuf[inPos++];
    if( size <= Fh_size )			/* End Of File */
      {
      if( first_member || Fh_verify_prefix( header, size ) )
        { Pp_show_msg( pp, "File ends unexpectedly at member header." );
          retval = 2; }
      break;
      }
    if( !Fh_verify_magic( header ) )
      {
      if( !first_member )
        {
        if( !ignore_trailing )
          { Pp_show_msg( pp, "Trailing data not allowed." ); retval = 2; }
        break;
        }
      if( inSize - inPos >= lzma_header_size - Fh_size )  /* try lzma-alone */
        {
        for( i = Fh_size; i < lzma_header_size; ++i )
          raw_props[i] = inBuf[inPos++];
        if( ( raw_props[12] == 0 || raw_props[12] == 0xFF ) &&
            raw_props[12] == raw_props[11] && raw_props[0] < (9 * 5 * 5) )
          {
          lzip_mode = false;
          dictionary_size = 0;
          for( i = 4; i >= 1; --i )
            { dictionary_size <<= 8; dictionary_size += raw_props[i]; }
          for( i = 7; i >= 0; --i )
            { unpackSize <<= 8; unpackSize += raw_props[LZMA_PROPS_SIZE+i]; }
          }
        }
      if( lzip_mode )
        {
        Pp_show_msg( pp, "Bad magic number (file not in lzip format)." );
        retval = 2; break;
        }
      }
    if( lzip_mode )
      {
      int ds, i;
      if( !Fh_verify_version( header ) )
        {
        if( verbosity >= 0 )
          { Pp_show_msg( pp, 0 );
            fprintf( stderr, "Version %d member format not supported.\n",
                     Fh_version( header ) ); }
        retval = 2; break;
        }
      dictionary_size = Fh_get_dictionary_size( header );
      if( !isvalid_ds( dictionary_size ) )
        { Pp_show_msg( pp, "Invalid dictionary size in member header." );
          retval = 2; break; }

      raw_props[0] = 93;		/* (45 * 2) + (9 * 0) + 3 */
      ds = dictionary_size;
      for( i = 1; i <= 4; ++i ) { raw_props[i] = ds & 0xFF; ds >>= 8; }
      }

    if( verbosity >= 2 || ( verbosity == 1 && first_member ) )
      { Pp_show_msg( pp, 0 ); show_header( dictionary_size ); }

    if( !LzmaDec_Init( &decoder, raw_props ) )
      { Pp_show_msg( pp, "Not enough memory." ); return 1; }
    if( lzip_mode )
      retval = lzip_decode( &decoder, infd, inBuf, &inPos, &inSize );
    else
      retval = lzma_decode( unpackSize, &decoder, infd,
                            inBuf, &inPos, &inSize, testing );
    LzmaDec_Free(&decoder);
    if( retval != 0 || !lzip_mode ) break;
    if( verbosity >= 2 )
      { fputs( testing ? "ok\n" : "done\n", stderr ); Pp_reset( pp ); }
    }
  if( lzip_mode && verbosity == 1 && retval == 0 )
    fputs( testing ? "ok\n" : "done\n", stderr );
  return retval;
  }


void signal_handler( int sig )
  {
  if( sig ) {}				/* keep compiler happy */
  show_error( "Control-C or similar caught, quitting.", 0, false );
  cleanup_and_fail( 1 );
  }


static void set_signals( void )
  {
  signal( SIGHUP, signal_handler );
  signal( SIGINT, signal_handler );
  signal( SIGTERM, signal_handler );
  }


CRC32 crc32;


void Pp_show_msg( struct Pretty_print * const pp, const char * const msg )
  {
  if( verbosity >= 0 )
    {
    if( pp->first_post )
      {
      unsigned i;
      pp->first_post = false;
      fprintf( stderr, "  %s: ", pp->name );
      for( i = strlen( pp->name ); i < pp->longest_name; ++i )
        fputc( ' ', stderr );
      if( !msg ) fflush( stderr );
      }
    if( msg ) fprintf( stderr, "%s\n", msg );
    }
  }


/* Returns the number of bytes really read.
   If (returned value < size) and (errno == 0), means EOF was reached.
*/
int readblock( const int fd, uint8_t * const buf, const int size )
  {
  int sz = 0;
  errno = 0;
  while( sz < size )
    {
    const int n = read( fd, buf + sz, size - sz );
    if( n > 0 ) sz += n;
    else if( n == 0 ) break;				/* EOF */
    else if( errno != EINTR ) break;
    errno = 0;
    }
  return sz;
  }


/* Returns the number of bytes really written.
   If (returned value < size), it is always an error.
*/
int writeblock( const int fd, const uint8_t * const buf, const int size )
  {
  int sz = 0;
  errno = 0;
  while( sz < size )
    {
    const int n = write( fd, buf + sz, size - sz );
    if( n > 0 ) sz += n;
    else if( n < 0 && errno != EINTR ) break;
    errno = 0;
    }
  return sz;
  }


void show_error( const char * const msg, const int errcode, const bool help )
  {
  if( verbosity < 0 ) return;
  if( msg && msg[0] )
    {
    fprintf( stderr, "%s: %s", program_name, msg );
    if( errcode > 0 ) fprintf( stderr, ": %s", strerror( errcode ) );
    fputc( '\n', stderr );
    }
  if( help )
    fprintf( stderr, "Try '%s --help' for more information.\n",
             invocation_name );
  }


void internal_error( const char * const msg )
  {
  if( verbosity >= 0 )
    fprintf( stderr, "%s: internal error: %s\n", program_name, msg );
  exit( 3 );
  }


int main( const int argc, const char * const argv[] )
  {
  /* Mapping from gzip/bzip2 style 1..9 compression modes
     to the corresponding LZMA compression modes. */
  const struct Lzma_options option_mapping[] =
    {
    { 1 << 20,   5 },		/* -0 */
    { 1 << 20,   5 },		/* -1 */
    { 3 << 19,   6 },		/* -2 */
    { 1 << 21,   8 },		/* -3 */
    { 3 << 20,  12 },		/* -4 */
    { 1 << 22,  20 },		/* -5 */
    { 1 << 23,  36 },		/* -6 */
    { 1 << 24,  68 },		/* -7 */
    { 3 << 23, 132 },		/* -8 */
    { 1 << 25, 273 } };		/* -9 */
  struct Lzma_options encoder_options = option_mapping[6];  /* default = "-6" */
  const char * input_filename = "";
  const char * default_output_filename = "";
  const char ** filenames = 0;
  int num_filenames = 0;
  int infd = -1;
  enum Mode program_mode = m_compress;
  int argind = 0;
  int retval = 0;
  int i;
  bool filenames_given = false;
  bool force = false;
  bool ignore_trailing = true;
  bool keep_input_files = false;
  bool stdin_used = false;
  bool recompress = false;
  bool to_stdout = false;
  struct Pretty_print pp;

  const struct ap_Option options[] =
    {
    { '0', "fast",            ap_no  },
    { '1',  0,                ap_no  },
    { '2',  0,                ap_no  },
    { '3',  0,                ap_no  },
    { '4',  0,                ap_no  },
    { '5',  0,                ap_no  },
    { '6',  0,                ap_no  },
    { '7',  0,                ap_no  },
    { '8',  0,                ap_no  },
    { '9', "best",            ap_no  },
    { 'a', "trailing-error",  ap_no  },
    { 'b', "member-size",     ap_yes },
    { 'c', "stdout",          ap_no  },
    { 'd', "decompress",      ap_no  },
    { 'f', "force",           ap_no  },
    { 'F', "recompress",      ap_no  },
    { 'h', "help",            ap_no  },
    { 'k', "keep",            ap_no  },
    { 'm', "match-length",    ap_yes },
    { 'n', "threads",         ap_yes },
    { 'o', "output",          ap_yes },
    { 'q', "quiet",           ap_no  },
    { 's', "dictionary-size", ap_yes },
    { 'S', "volume-size",     ap_yes },
    { 't', "test",            ap_no  },
    { 'v', "verbose",         ap_no  },
    { 'V', "version",         ap_no  },
    {  0 ,  0,                ap_no  } };

  struct Arg_parser parser;

  invocation_name = argv[0];
  CRC32_init();

  if( !ap_init( &parser, argc, argv, options, 0 ) )
    { show_error( "Not enough memory.", 0, false ); return 1; }
  if( ap_error( &parser ) )				/* bad option */
    { show_error( ap_error( &parser ), 0, true ); return 1; }

  for( ; argind < ap_arguments( &parser ); ++argind )
    {
    const int code = ap_code( &parser, argind );
    const char * const arg = ap_argument( &parser, argind );
    if( !code ) break;					/* no more options */
    switch( code )
      {
      case '0': case '1': case '2': case '3': case '4':
      case '5': case '6': case '7': case '8': case '9':
                encoder_options = option_mapping[code-'0']; break;
      case 'a': ignore_trailing = false; break;
      case 'b': break;
      case 'c': to_stdout = true; break;
      case 'd': program_mode = m_decompress; break;
      case 'f': force = true; break;
      case 'F': recompress = true; break;
      case 'h': show_help(); return 0;
      case 'k': keep_input_files = true; break;
      case 'm': encoder_options.match_len_limit =
                  getnum( arg, min_match_len_limit, max_match_len ); break;
      case 'n': break;
      case 'o': default_output_filename = arg; break;
      case 'q': verbosity = -1; break;
      case 's': encoder_options.dictionary_size = get_dict_size( arg );
                break;
      case 'S': break;
      case 't': program_mode = m_test; break;
      case 'v': if( verbosity < 4 ) ++verbosity; break;
      case 'V': show_version(); return 0;
      default : internal_error( "uncaught option." );
      }
    } /* end process options */

#if defined(__MSVCRT__) || defined(__OS2__)
  setmode( STDIN_FILENO, O_BINARY );
  setmode( STDOUT_FILENO, O_BINARY );
#endif

  if( program_mode == m_test )
    outfd = -1;

  num_filenames = max( 1, ap_arguments( &parser ) - argind );
  filenames = resize_buffer( filenames, num_filenames * sizeof filenames[0] );
  filenames[0] = "-";

  for( i = 0; argind + i < ap_arguments( &parser ); ++i )
    {
    filenames[i] = ap_argument( &parser, argind + i );
    if( strcmp( filenames[i], "-" ) != 0 ) filenames_given = true;
    }

  if( !to_stdout && program_mode != m_test &&
      ( filenames_given || default_output_filename[0] ) )
    set_signals();

  Pp_init( &pp, filenames, num_filenames, verbosity );

  output_filename = resize_buffer( output_filename, 1 );
  for( i = 0; i < num_filenames; ++i )
    {
    int tmp;
    struct stat in_stats;
    const struct stat * in_statsp;
    output_filename[0] = 0;

    if( !filenames[i][0] || strcmp( filenames[i], "-" ) == 0 )
      {
      if( stdin_used ) continue; else stdin_used = true;
      input_filename = "";
      infd = STDIN_FILENO;
      if( program_mode != m_test )
        {
        if( to_stdout || !default_output_filename[0] )
          outfd = STDOUT_FILENO;
        else
          {
          if( program_mode == m_compress )
            set_c_outname( default_output_filename );
          else
            {
            output_filename = resize_buffer( output_filename,
                                             strlen( default_output_filename ) + 1 );
            strcpy( output_filename, default_output_filename );
            }
          if( !open_outstream( force, true ) )
            {
            if( retval < 1 ) retval = 1;
            close( infd ); infd = -1;
            continue;
            }
          }
        }
      }
    else
      {
      const int eindex = extension_index( filenames[i] );
      input_filename = filenames[i];
      infd = open_instream( input_filename, &in_stats, program_mode,
                            eindex, recompress, to_stdout );
      if( infd < 0 ) { if( retval < 1 ) retval = 1; continue; }
      if( program_mode != m_test )
        {
        if( to_stdout ) outfd = STDOUT_FILENO;
        else
          {
          if( program_mode == m_compress )
            set_c_outname( input_filename );
          else set_d_outname( input_filename, eindex );
          if( !open_outstream( force, false ) )
            {
            if( retval < 1 ) retval = 1;
            close( infd ); infd = -1;
            continue;
            }
          }
        }
      }

    if( !check_tty( infd, program_mode ) )
      {
      if( retval < 1 ) retval = 1;
      cleanup_and_fail( retval );
      }

    in_statsp = input_filename[0] ? &in_stats : 0;
    Pp_set_name( &pp, input_filename );
    if( program_mode == m_compress )
      tmp = compress( &encoder_options, infd, &pp );
    else
      tmp = decompress( infd, &pp, ignore_trailing, program_mode == m_test );
    if( tmp > retval ) retval = tmp;
    if( tmp && program_mode != m_test ) cleanup_and_fail( retval );

    if( delete_output_on_interrupt )
      close_and_set_permissions( in_statsp );
    if( input_filename[0] )
      {
      close( infd ); infd = -1;
      if( !keep_input_files && !to_stdout && program_mode != m_test )
        remove( input_filename );
      }
    }
  if( outfd >= 0 && close( outfd ) != 0 )
    {
    show_error( "Can't close stdout", errno, false );
    if( retval < 1 ) retval = 1;
    }
  free( output_filename );
  free( filenames );
  ap_free( &parser );
  return retval;
  }
