/* mudecode.c - MIME / UU decoder
**
** Copyright (C) 1995,1997 by Jef Poskanzer <jef@mail.acme.com>.  All rights
** reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
** 1. Redistributions of source code must retain the above copyright
**    notice, this list of conditions and the following disclaimer.
** 2. Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in the
**    documentation and/or other materials provided with the distribution.
** 
** THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
** ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
** FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
** DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
** OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
** HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
** LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
** OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
** SUCH DAMAGE.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/param.h>
#include <sys/stat.h>

static char* argv0;
static int verbose;
static int exit_val;
static char* infilename;
static FILE* infile;
static char outfiledir[MAXPATHLEN];
static char outfilename[MAXPATHLEN];
static char real_outfilename[MAXPATHLEN];
static FILE* outfile;
static int in_data_seen, out_data_written;
static int got_base64;

static int state;
#define ST_BETWEEN_FILES 0
#define ST_UU_READING_FILE 1
#define ST_UU_READING_END 2
#define ST_MM_READING_HEADERS 3
#define ST_MM_READING_RESTOFHEADERS 4
#define ST_MM_READING_FILE 5

static int uu_decode_table[256];
static int uu_backquote_seen, uu_blank_seen, uu_blank_line_ok, uu_long_warn;
static char uu_next_seqbyte;

static int mm_decode_table[256];


static void
usage( void )
    {
    (void) fprintf( stderr, "usage: %s [-v] [-o destdir] [file ...]\n", argv0 );
    exit( 1 );
    }


static void
outfile_done( void )
    {
    (void) fclose( outfile );
    if ( ! out_data_written )
	(void) fprintf(
	    stderr, "%s - %s: no valid data written, output file is empty\n",
	    argv0, outfilename );
    outfilename[0] = '\0';
    state = ST_BETWEEN_FILES;
    }


static void
infile_done( void )
    {
    if ( infile != stdin )
	(void) fclose( infile );
    if ( ! in_data_seen )
	(void) fprintf(
	    stderr, "%s - %s: no valid data seen, input file may be bogus\n",
	    argv0, infilename );
    }


static void
uu_decode_line( int elen, unsigned char* line )
    {
    unsigned char c;

    for ( ; elen > 0; line += 4, elen -= 3 )
	{
	if ( elen >= 1 )
	    {
	    c = uu_decode_table[line[0]] << 2 | uu_decode_table[line[1]] >> 4;
	    putc( c, outfile );
	    }
	if ( elen >= 2 )
	    {
	    c = uu_decode_table[line[1]] << 4 | uu_decode_table[line[2]] >> 2;
	    putc( c, outfile );
	    }
	if ( elen >= 3 )
	    {
	    c = uu_decode_table[line[2]] << 6 | uu_decode_table[line[3]];
	    putc( c, outfile );
	    }
	}
    in_data_seen = 1;
    out_data_written = 1;
    }


static int gotchars;

static void
mm_decode_init( void )
    {
    gotchars = 0;
    }


static void
mm_decode_line( unsigned char* line )
    {
    static unsigned char c1, c2, c3, c4;
    unsigned char* cp;
    unsigned char c;

    for ( cp = line; *cp != '\0'; ++cp )
	{
	switch ( gotchars )
	    {
	    case 0:
	    c1 = mm_decode_table[(int) *cp];
	    ++gotchars;
	    break;
	    case 1:
	    c2 = mm_decode_table[(int) *cp];
	    c = ( ( c1 << 2 ) | ( ( c2 & 0x30 ) >> 4 ) );
	    putc( c, outfile );
	    ++gotchars;
	    break;
	    case 2:
	    c3 = mm_decode_table[(int) *cp];
	    c = ( ( ( c2 & 0xf ) << 4 ) | ( ( c3 & 0x3c ) >> 2 ) );
	    putc( c, outfile );
	    ++gotchars;
	    break;
	    case 3:
	    c4 = mm_decode_table[(int) *cp];
	    c = ( ( ( c3 & 0x03 ) << 6 ) | c4 );
	    putc( c, outfile );
	    gotchars = 0;
	    break;
	    }
	}
    in_data_seen = 1;
    out_data_written = 1;
    }


static int
uu_legal_chars( char* s )
    {
    int bs, ss;

    /* A line consisting of a single blank is legal only sometimes.  This is
    ** regardless of the setting of uu_backquote_seen - some uuencoders still
    ** generate single-blank lines even though they use backquote for blank
    ** elsewhere.
    */
    if ( strcmp( s, " " ) == 0 )
	return uu_blank_line_ok;

    bs = ss = 0;
    for ( ; *s != '\0'; ++s )
	{
	if ( uu_decode_table[(int) *s] == -1 )
	    return 0;
	if ( *s == '`' )
	    bs = 1;
	else if ( *s == ' ' )
	    ss = 1;
	}
    /* Did we see backquote for the first time this line? */
    if ( bs && ! uu_backquote_seen )
	{
	uu_backquote_seen = 1;
	if ( ( ss || uu_blank_seen ) && verbose )
	    (void) fprintf( stderr,
		"%s - %s: both backquote and blank used, weird uuencoder\n",
		argv0, outfilename );
	if ( ! ( ss || uu_blank_seen ) )
	    /* Disable blank if we've seen backquote but not blank. */
	    uu_decode_table[' '] = -1;
	}
    /* Did we see blank for the first time this line? */
    if ( ss && ! uu_blank_seen )
	{
	uu_blank_seen = 1;
	if ( ( bs || uu_backquote_seen ) && verbose )
	    (void) fprintf( stderr,
		"%s - %s: both backquote and blank used, weird uuencoder\n",
		argv0, outfilename );
	else if ( verbose )
	    (void) fprintf(
		stderr, "%s - %s: blank used, old-fashioned uuencoder\n",
		argv0, outfilename );
	}
    return 1;
    }


static int
mm_legal_chars( char* s )
    {
    for ( ; *s != '\0'; ++s )
	if ( mm_decode_table[(int) *s] == -1 )
	    return 0;
    return 1;
    }


static void
init_uu_decode_table( void )
    {
    int c;

    for ( c = 0; c < 256; ++c )
	uu_decode_table[c] = -1;
    for ( c = ' '; c <= '_'; ++c )
	uu_decode_table[c] = c - ' ';
    uu_decode_table['`'] = uu_decode_table[' '];
    uu_decode_table['~'] = uu_decode_table['^'];
    }


static void
init_mm_decode_table( void )
    {
    int c;

    for ( c = 0; c < 256; ++c )
	mm_decode_table[c] = -1;
    for ( c = 'A'; c <= 'Z'; ++c )
	mm_decode_table[c] = c - 'A';
    for ( c = 'a'; c <= 'z'; ++c )
	mm_decode_table[c] = c - 'a' + 26;
    for ( c = '0'; c <= '9'; ++c )
	mm_decode_table[c] = c - '0' + 52;
    mm_decode_table['+'] = 62;
    mm_decode_table['/'] = 63;
    }


static int
start_outputfile( char* fn, char* code_type, int mode )
    {
    if ( strlen( fn ) >= sizeof(outfilename) )
	{
	(void) fprintf(
	    stderr, "%s - %s: output filename too long\n", argv0, infilename );
	exit_val = 1;
	return 0;
	}
    (void) strcpy( outfilename, fn );
    if ( outfilename[0] == '/' )
	(void) strcpy( real_outfilename, outfilename );
    else
	{
	(void) strcpy( real_outfilename, outfiledir );
	(void) strcat( real_outfilename, outfilename );
	}
    outfile = fopen( real_outfilename, "w" );
    if ( outfile == (FILE*) 0 )
	{
	(void) fprintf( stderr, "%s - %s: output file ", argv0, infilename );
	perror( real_outfilename );
	exit_val = 1;
	return 0;
	}
    if ( mode != -1 )
	fchmod( fileno(outfile), mode );
    if ( verbose )
	(void) fprintf(
	    stderr, "%s: %s-decoding to %s\n", argv0, code_type, outfilename );
    if ( outfilename[0] == '/' )
	(void) fprintf(
	    stderr, "%s: warning, output file %s is an absolute path name\n",
	    argv0, outfilename );
    out_data_written = 0;
    return 1;
    }


static int
is_mime_header( char* str )
    {
    if ( strncasecmp( str, "Content-Transfer-Encoding: base64", 33 ) == 0 ||
         strncasecmp( str, "Content-Type: ", 14 ) == 0 ||
         strncasecmp( str, "Content-Disposition: ", 21 ) == 0 )
	return 1;
    return 0;
    }


static void
handle_mime_header( char* str )
    {
    char junk1[5000], junk2[5000], attr[5000], fn[5000];

    if ( strncasecmp( str, "Content-Transfer-Encoding: base64", 33 ) == 0 )
	got_base64 = 1;
    else if ( (
	    sscanf( str, "Content-%[Tt]ype: %[^ ] %[a-zA-Z]=\"%[^\"]\"", junk1, junk2, attr, fn ) == 4 ||
	    sscanf( str, "Content-%[Tt]ype: %[^ ] %[a-zA-Z]=%s", junk1, junk2, attr, fn ) == 4 ||
	    sscanf( str, "Content-%[Dd]isposition: %[^ ] %[a-zA-Z]=\"%[^\"]\"", junk1, junk2, attr, fn ) == 4 ||
	    sscanf( str, "Content-%[Dd]isposition: %[^ ] %[a-zA-Z]=%s", junk1, junk2, attr, fn ) == 4
	) && (
	    strcasecmp( attr, "name" ) == 0 ||
	    strcasecmp( attr, "file" ) == 0 ||
	    strcasecmp( attr, "filename" ) == 0
	) )
	{
	if ( start_outputfile( fn, "MIME", -1 ) )
	    state = ST_MM_READING_RESTOFHEADERS;
	}
    }


static void
decode_file( void )
    {
    char buf[5000];
    char fn[5000];
    int len, mode;
    char seqbyte;

    if ( verbose )
	(void) fprintf( stderr, "%s: decoding from %s\n", argv0, infilename );

    while ( fgets( buf, sizeof(buf), infile ) != (char*) 0 )
	{
	/* Trim all CRs and LFs from the line. */
	len = strlen( buf );
	while ( buf[len - 1] == '\012' || buf[len - 1] == '\015' )
	    buf[--len] = '\0';

	switch ( state )
	    {
	    case ST_BETWEEN_FILES:
	    /* We are between files, looking for a uuencode begin line
	    ** or a MIME Content line.
	    */
	    if ( strncmp( buf, "begin ", 6 ) == 0 )
		{
		begin_found:
		if ( sscanf( buf, "begin %o %s", &mode, fn ) != 2 )
		    {
		    (void) fprintf(
			stderr, "%s - %s: malformed begin line\n", argv0,
			infilename );
		    exit_val = 1;
		    }
		else
		    {
		    /* Got a valid begin line. */
		    if ( ( mode & 0777 ) != mode )
			{
			(void) fprintf( stderr,
			    "%s - %s: attempt to set suid/sgid bits, ignored\n",
			    argv0, infilename );
			mode &= 0777;
			}
		    if ( ( mode & 0755 ) != mode )
			{
			(void) fprintf( stderr,
		 "%s - %s: attempt to set group/world writable bits, ignored\n",
			    argv0, infilename );
			mode &= 0755;
			}
		    if ( start_outputfile( fn, "UU", mode ) )
			{
			state = ST_UU_READING_FILE;
			init_uu_decode_table();
			uu_backquote_seen = uu_blank_seen = 0;
			uu_blank_line_ok = 1;
			uu_next_seqbyte = 'z';
			uu_long_warn = 0;
			}
		    }
		}
	    else if ( is_mime_header( buf ) )
		{
		state = ST_MM_READING_HEADERS;
		got_base64 = 0;
		handle_mime_header( buf );
		}
	    break;

	    case ST_MM_READING_HEADERS:
	    /* We are reading the MIME headers. */
	    if ( buf[0] == '\0' )
		{
		if ( got_base64 )
		    {
		    (void) fprintf(
			stderr, "%s - %s: no output filename found\n", argv0,
			infilename );
		    exit_val = 1;
		    }
		state = ST_BETWEEN_FILES;
		}
	    else
		handle_mime_header( buf );
	    break;

	    case ST_MM_READING_RESTOFHEADERS:
	    if ( buf[0] == '\0' )
		{
		mm_decode_init();
		state = ST_MM_READING_FILE;
		}
	    break;

	    case ST_UU_READING_FILE:
	    /* We are uudecoding a file. */
	    if ( strncmp( buf, "begin ", 6 ) == 0 )
		{
		(void) fprintf( stderr,
	   "%s - %s: begin line found in middle of file, %s may be truncated\n",
		    argv0, infilename, outfilename );
		exit_val = 1;
		outfile_done();
		goto begin_found;
		}
	    else if ( is_mime_header( buf ) )
		{
		(void) fprintf( stderr,
	    "%s - %s: MIME line found in middle of file, %s may be truncated\n",
		    argv0, infilename, outfilename );
		exit_val = 1;
		outfile_done();
		state = ST_MM_READING_HEADERS;
		got_base64 = 0;
		handle_mime_header( buf );
		}
	    else if ( strcmp( buf, "end" ) == 0 )
		{
		(void) fprintf( stderr,
		    "%s - %s: premature end line, %s may be truncated\n",
		    argv0, infilename, outfilename );
		exit_val = 1;
		outfile_done();
		}
	    else
		{
		/* It's not a begin or end line. */
		int elen;

		/* Look for possible sequence byte at EOL, save and remove. */
		if ( buf[len - 1] >= 'a' && buf[len - 1] <= 'z' )
		    {
		    seqbyte = buf[--len];
		    buf[len] = '\0';
		    }
		else
		    seqbyte = '\0';

		/* It is valid uuencoded line?  Check that the encoded
		** length and string length are legal and agree.  If so,
		** check that there are no characters that uuencode
		** doesn't generate.
		*/
		if ( buf[0] == ' ' )
		    elen = 0;		/* special case blank as length */
		else
		    elen = uu_decode_table[(int) buf[0]];
		if ( elen <= 45 && len <= 62 &&
		     elen * 4 / 3 >= len - 4 && elen * 4 / 3 <= len )
		    if ( uu_legal_chars( buf ) )
			{
			if ( len > 61 && ! uu_long_warn )
			    {
			    /* Some bogus uuencoders generate lines with
			    ** more than 60 data bytes.  Read them anyway,
			    ** even though the extra data is ignored.
			    */
			    (void) fprintf( stderr,
		 "%s - %s: over-long lines, non-standard, extra data ignored\n",
				argv0, outfilename );
			    uu_long_warn = 1;
			    }
			if ( elen == 0 )
			    state = ST_UU_READING_END;
			else
			    uu_decode_line( elen, (unsigned char*) ( buf+1 ) );
			/* Sequence check. */
			if ( seqbyte != '\0' )
			    {
			    if ( seqbyte != uu_next_seqbyte )
				(void) fprintf( stderr,
		         "%s - %s: line out of sequence, %s may be corrupted\n",
				    argv0, infilename, outfilename );
			    if ( uu_next_seqbyte == 'a' )
				uu_next_seqbyte = 'z';
			    else
				--uu_next_seqbyte;
			    }
			uu_blank_line_ok = 1;
			}
		    else
			uu_blank_line_ok = 0;
		else
		    uu_blank_line_ok = 0;
		/* We silently skip non-uuencoded lines.  There is a slight
		** possibility for error here, due to a random intervening
		** line accidentally beginning with the right character
		** and not having any illegal characters.
		**
		** However, note the uu_backquote_seen hack - modern uuencoders
		** use backquote instead of blank.  If we make blank an illegal
		** character then we're much less likely to mistake a random
		** line for valid uuencoding.  So, if we've seen backquotes
		** but no blanks, we disallow blanks for the rest of that
		** output file.
		**
		** Also, one particularly likely case for a false-positive is
		** the line right before an end line - some uuencoders, even
		** if they use backquote instead of blank elsewhere, still
		** generate a line consisting of a single blank there.
		** Such lines can easily occur by accident in a file header
		** or trailer.  We handle it by making such lines illegal
		** (and therefore ignored) unless they immediately follow
		** a valid uuencoded line.  The only downside of this
		** scheme is if a multi-part file happens to get split
		** such that the last part has only the single-blank line
		** and the end line.  In that case the single-blank line
		** will be ignored and the unexpected end line will generate
		** a warning - but the file will decode just fine.
		*/
		}
	    break;

	    case ST_UU_READING_END:
	    if ( strcmp( buf, "end" ) != 0 )
		{
		(void) fprintf( stderr,
		    "%s - %s: end line not found, %s may be truncated\n",
		    argv0, infilename, outfilename );
		exit_val = 1;
		}
	    else
		{
		if ( verbose )
		    (void) fprintf(
			stderr, "%s: finished writing %s\n", argv0,
			outfilename );
		}
	    /* Whether the end line was there or not, clean up as if it was. */
	    outfile_done();
	    break;

	    case ST_MM_READING_FILE:
	    /* We are MIME-decoding a file. */
	    if ( strncmp( buf, "begin ", 6 ) == 0 )
		{
		(void) fprintf( stderr,
	   "%s - %s: begin line found in middle of file, %s may be truncated\n",
		    argv0, infilename, outfilename );
		exit_val = 1;
		outfile_done();
		goto begin_found;
		}
	    else if ( is_mime_header( buf ) )
		{
		(void) fprintf( stderr,
	    "%s - %s: MIME line found in middle of file, %s may be truncated\n",
		    argv0, infilename, outfilename );
		exit_val = 1;
		outfile_done();
		state = ST_MM_READING_HEADERS;
		got_base64 = 0;
		handle_mime_header( buf );
		}
	    while ( buf[len - 1] == '=' )
		buf[--len] = '\0';
	    if ( mm_legal_chars( buf ) )
		{
		if ( len >= 5 && len <= 78 )
		    {
		    mm_decode_line( (unsigned char*) buf );
		    if ( len < 60 )
			outfile_done();
		    }
		}
	    break;

	    default:
	    (void) fprintf( stderr, "%s: internal error\n", argv0 );
	    exit( 1 );
	    }
	}

    /* If there was an error reading the input file, report it. */
    if ( ferror( infile ) )
	{
	(void) fprintf( stderr, "%s - ", argv0 );
	perror( infilename );
	exit_val = 1;
	}
    }


int
main( int argc, char** argv )
    {
    int argn, stdin_used;

    argv0 = argv[0];

    argn = 1;
    verbose = 0;
    (void) strcpy( outfiledir, "" );
    while ( argn < argc && argv[argn][0] == '-' && argv[argn][1] != '\0' )
	{
	if ( strcmp( argv[argn], "-v" ) == 0 )
	    verbose = 1;
	else if ( strcmp( argv[argn], "-o" ) == 0 )
	    {
	    ++argn;
	    if ( argn >= argc )
		usage();
	    (void) strcpy( outfiledir, argv[argn] );
	    if ( outfiledir[strlen(outfiledir)-1] != '/' )
		(void) strcat( outfiledir, "/" );
	    }
	else
	    usage();
	++argn;
	}

    exit_val = 0;
    init_mm_decode_table();
    state = ST_BETWEEN_FILES;
    outfilename[0] = '\0';
    if ( argn == argc )
	{
	infilename = "(stdin)";
	infile = stdin;
	in_data_seen = 0;
	decode_file();
	infile_done();
	}
    else
	{
	stdin_used = 0;
	while ( argn < argc )
	    {
	    infilename = argv[argn];
	    if ( strcmp( infilename, "-" ) == 0 )
		{
		if ( stdin_used )
		    {
		    (void) fprintf(
			stderr, "%s: can't use stdin twice\n", argv0 );
		    exit_val = 1;
		    }
		else
		    {
		    infilename = "(stdin)";
		    infile = stdin;
		    stdin_used = 1;
		    in_data_seen = 0;
		    decode_file();
		    infile_done();
		    }
		}
	    else
		{
		infile = fopen( infilename, "r" );
		if ( infile == (FILE*) 0 )
		    {
		    (void) fprintf( stderr, "%s - ", argv0 );
		    perror( infilename );
		    exit_val = 1;
		    }
		else
		    {
		    in_data_seen = 0;
		    decode_file();
		    infile_done();
		    }
		}
	    ++argn;
	    }
	}
    
    if ( state != ST_BETWEEN_FILES )
	{
	(void) fprintf(
	    stderr, "%s: ran out of files, %s may be truncated\n",
	    argv0, outfilename );
	outfile_done();
	exit_val = 1;
	}

    exit( exit_val );
    }
