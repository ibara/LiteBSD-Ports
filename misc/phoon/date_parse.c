/* date_parse - parse string dates into internal form
**
** Copyright © 1995 by Jef Poskanzer <jef@mail.acme.com>.
** All rights reserved.
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

/* For a while now I've been somewhat unhappy with the various
** date-parsing routines available - yacc-based, lex-based, ad-hoc.  Large
** code size and not very portable are the main complaints.  So I wrote a
** new one that just does a bunch of sscanf's until one matches.  Slow,
** but small and portable.  To figure out what formats to support I did a
** survey of Date: lines in a bunch of Usenet articles.  The following
** two formats accounted for more than 99% of all articles:
**     DD mth YY HH:MM:SS ampm zone
**     wdy, DD mth YY HH:MM:SS ampm zone
** I added Unix ctime() format and a few others:
**     wdy, mth DD HH:MM:SS ampm zone YY
**     HH:MM:SS ampm zone DD mth YY
**     DD mth YY
**     HH:MM:SS ampm
**     DD/mth/YYYY:HH:MM:SS zone  (httpd common log format date)
** No-zone, no-seconds, and no-am/pm versions of each are also supported.
** Note that dd/mm/yy and mm/dd/yy are NOT supported - those formats are
** dumb.
*/

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "date_parse.h"

#ifdef OS_SYSV
extern long timezone;
#endif /* OS_SYSV */

struct strlong {
    char* s;
    long l;
    };

#define AMPM_NONE 0
#define AMPM_AM 1
#define AMPM_PM 2

static void pound_case( char* str );
static int strlong_compare( const void* v1, const void* v2 );
static int strlong_search( char* str, struct strlong* tab, int n, long* lP );
static int ampm_fix( int hour, int ampm );
static int scan_ampm( char* str_ampm, long* ampmP );
static int scan_wday( char* str_wday, long* tm_wdayP );
static int scan_mon( char* str_mon, long* tm_monP );
static int scan_gmtoff( char* str_gmtoff, long* gmtoffP );
static int is_leap( int year );
static time_t tm_to_time( struct tm* tmP );


time_t
date_parse( char* str )
    {
    time_t now;
    struct tm* now_tmP;
    struct tm tm;
    char* cp;
    char str_mon[500], str_wday[500], str_gmtoff[500], str_ampm[500];
    int tm_sec, tm_min, tm_hour, tm_mday, tm_year;
    long tm_mon, tm_wday, ampm, gmtoff;
    int got_zone;
    time_t t;
    int i;

    /* If it's all digits, treat it as a raw Unix time and we're done. */
    t = strtol( str, &cp, 10 );
    if ( *cp == '\0' )
	return t;

    /* Initialize tm with relevant parts of current local time. */
    now = time( (time_t*) 0 );
    now_tmP = localtime( &now );

    (void) memset( (char*) &tm, 0, sizeof(struct tm) );
    tm.tm_sec = now_tmP->tm_sec;
    tm.tm_min = now_tmP->tm_min;
    tm.tm_hour = now_tmP->tm_hour;
    tm.tm_mday = now_tmP->tm_mday;
    tm.tm_mon = now_tmP->tm_mon;
    tm.tm_year = now_tmP->tm_year;
    tm.tm_isdst = now_tmP->tm_isdst;
    ampm = AMPM_NONE;
    got_zone = 0;

    /* Find local zone offset.  This is the only real area of
    ** non-portability, and it's only used for local times that don't
    ** specify a zone - those don't occur in email and netnews.
    */
#ifdef OS_SYSV
    tzset();
    gmtoff = -timezone;
#else /* OS_SYSV */
    gmtoff = now_tmP->tm_gmtoff;
#endif /* OS_SYSV */

    /* Skip initial whitespace ourselves - sscanf is clumsy at this. */
    for ( cp = str; *cp == ' ' || *cp == '\t'; ++cp )
	continue;

    /* And do the sscanfs.  WARNING: you can add more formats here,
    ** but be careful!  You can easily screw up the parsing of existing
    ** formats when you add new ones.  The order is important.
    */

    /* DD/mth/YY:HH:MM:SS zone */
    if ( sscanf( cp, "%d/%400[a-zA-Z]/%d:%d:%d:%d %400[^: 	\n]",
	   &tm_mday, str_mon, &tm_year, &tm_hour, &tm_min, &tm_sec,
	   str_gmtoff ) == 7 &&
	 scan_mon( str_mon, &tm_mon ) &&
	 scan_gmtoff( str_gmtoff, &gmtoff ) )
	{
	tm.tm_mday = tm_mday;
	tm.tm_mon = tm_mon;
	tm.tm_year = tm_year;
	tm.tm_hour = ampm_fix( tm_hour, ampm );
	tm.tm_min = tm_min;
	tm.tm_sec = tm_sec;
	got_zone = 1;
	}

    /* DD-mth-YY HH:MM:SS ampm zone */
    else if ( ( ( sscanf( cp, "%d-%400[a-zA-Z]-%d %d:%d:%d %400[apmAPM] %400[^: 	\n]",
		    &tm_mday, str_mon, &tm_year, &tm_hour, &tm_min, &tm_sec,
		    str_ampm, str_gmtoff ) == 8 &&
	          scan_ampm( str_ampm, &ampm ) ) ||
                sscanf( cp, "%d-%400[a-zA-Z]-%d %d:%d:%d %400[^: 	\n]",
		   &tm_mday, str_mon, &tm_year, &tm_hour, &tm_min, &tm_sec,
		   str_gmtoff ) == 7 ) &&
	      scan_mon( str_mon, &tm_mon ) &&
	      scan_gmtoff( str_gmtoff, &gmtoff ) )
	{
	tm.tm_mday = tm_mday;
	tm.tm_mon = tm_mon;
	tm.tm_year = tm_year;
	tm.tm_hour = ampm_fix( tm_hour, ampm );
	tm.tm_min = tm_min;
	tm.tm_sec = tm_sec;
	got_zone = 1;
	}
    /* DD-mth-YY HH:MM ampm zone */
    else if ( ( ( sscanf( cp, "%d-%400[a-zA-Z]-%d %d:%d %400[apmAPM] %400[^: 	\n]",
		    &tm_mday, str_mon, &tm_year, &tm_hour, &tm_min, str_ampm,
		    str_gmtoff ) == 7 &&
	          scan_ampm( str_ampm, &ampm ) ) ||
                sscanf( cp, "%d-%400[a-zA-Z]-%d %d:%d %400[^: 	\n]",
		    &tm_mday, str_mon, &tm_year, &tm_hour, &tm_min,
		    str_gmtoff ) == 6 ) &&
	      scan_mon( str_mon, &tm_mon ) &&
	      scan_gmtoff( str_gmtoff, &gmtoff ) )
	{
	tm.tm_mday = tm_mday;
	tm.tm_mon = tm_mon;
	tm.tm_year = tm_year;
	tm.tm_hour = ampm_fix( tm_hour, ampm );
	tm.tm_min = tm_min;
	tm.tm_sec = 0;
	got_zone = 1;
	}
    /* DD-mth-YY HH:MM:SS ampm */
    else if ( ( ( sscanf( cp, "%d-%400[a-zA-Z]-%d %d:%d:%d %400[apmAPM]",
		    &tm_mday, str_mon, &tm_year, &tm_hour, &tm_min, &tm_sec,
		    str_ampm ) == 7 &&
	          scan_ampm( str_ampm, &ampm ) ) ||
                sscanf( cp, "%d-%400[a-zA-Z]-%d %d:%d:%d",
		  &tm_mday, str_mon, &tm_year, &tm_hour, &tm_min,
		  &tm_sec ) == 6 ) &&
	      scan_mon( str_mon, &tm_mon ) )
	{
	tm.tm_mday = tm_mday;
	tm.tm_mon = tm_mon;
	tm.tm_year = tm_year;
	tm.tm_hour = ampm_fix( tm_hour, ampm );
	tm.tm_min = tm_min;
	tm.tm_sec = tm_sec;
	}
    /* DD-mth-YY HH:MM ampm */
    else if ( ( ( sscanf( cp, "%d-%400[a-zA-Z]-%d %d:%d %400[apmAPM]",
		    &tm_mday, str_mon, &tm_year, &tm_hour, &tm_min,
		    str_ampm ) == 6 &&
	          scan_ampm( str_ampm, &ampm ) ) ||
                sscanf( cp, "%d-%400[a-zA-Z]-%d %d:%d",
		  &tm_mday, str_mon, &tm_year, &tm_hour, &tm_min ) == 5 ) &&
	      scan_mon( str_mon, &tm_mon ) )
	{
	tm.tm_mday = tm_mday;
	tm.tm_mon = tm_mon;
	tm.tm_year = tm_year;
	tm.tm_hour = ampm_fix( tm_hour, ampm );
	tm.tm_min = tm_min;
	tm.tm_sec = 0;
	}

    /* DD mth YY HH:MM:SS ampm zone */
    else if ( ( ( sscanf( cp, "%d %400[a-zA-Z] %d %d:%d:%d %400[apmAPM] %400[^: 	\n]",
		    &tm_mday, str_mon, &tm_year, &tm_hour, &tm_min, &tm_sec,
		    str_ampm, str_gmtoff ) == 8 &&
	          scan_ampm( str_ampm, &ampm ) ) ||
                sscanf( cp, "%d %400[a-zA-Z] %d %d:%d:%d %400[^: 	\n]",
		  &tm_mday, str_mon, &tm_year, &tm_hour, &tm_min, &tm_sec,
		  str_gmtoff ) == 7 ) &&
	      scan_mon( str_mon, &tm_mon ) &&
	      scan_gmtoff( str_gmtoff, &gmtoff ) )
	{
	tm.tm_mday = tm_mday;
	tm.tm_mon = tm_mon;
	tm.tm_year = tm_year;
	tm.tm_hour = ampm_fix( tm_hour, ampm );
	tm.tm_min = tm_min;
	tm.tm_sec = tm_sec;
	got_zone = 1;
	}
    /* DD mth YY HH:MM ampm zone */
    else if ( ( ( sscanf( cp, "%d %400[a-zA-Z] %d %d:%d %400[apmAPM] %400[^: 	\n]",
		    &tm_mday, str_mon, &tm_year, &tm_hour, &tm_min, str_ampm,
		    str_gmtoff ) == 7 &&
	          scan_ampm( str_ampm, &ampm ) ) ||
                sscanf( cp, "%d %400[a-zA-Z] %d %d:%d %400[^: 	\n]",
		  &tm_mday, str_mon, &tm_year, &tm_hour, &tm_min,
		  str_gmtoff ) == 6 ) &&
	      scan_mon( str_mon, &tm_mon ) &&
	      scan_gmtoff( str_gmtoff, &gmtoff ) )
	{
	tm.tm_mday = tm_mday;
	tm.tm_mon = tm_mon;
	tm.tm_year = tm_year;
	tm.tm_hour = ampm_fix( tm_hour, ampm );
	tm.tm_min = tm_min;
	tm.tm_sec = 0;
	got_zone = 1;
	}
    /* DD mth YY HH:MM:SS ampm */
    else if ( ( ( sscanf( cp, "%d %400[a-zA-Z] %d %d:%d:%d %400[apmAPM]",
		    &tm_mday, str_mon, &tm_year, &tm_hour, &tm_min, &tm_sec,
		    str_ampm ) == 7 &&
	          scan_ampm( str_ampm, &ampm ) ) ||
                sscanf( cp, "%d %400[a-zA-Z] %d %d:%d:%d",
		    &tm_mday, str_mon, &tm_year, &tm_hour, &tm_min,
		    &tm_sec ) == 6 ) &&
	      scan_mon( str_mon, &tm_mon ) )
	{
	tm.tm_mday = tm_mday;
	tm.tm_mon = tm_mon;
	tm.tm_year = tm_year;
	tm.tm_hour = ampm_fix( tm_hour, ampm );
	tm.tm_min = tm_min;
	tm.tm_sec = tm_sec;
	}
    /* DD mth YY HH:MM ampm */
    else if ( ( ( sscanf( cp, "%d %400[a-zA-Z] %d %d:%d %400[apmAPM]",
		    &tm_mday, str_mon, &tm_year, &tm_hour, &tm_min,
		    str_ampm ) == 6 &&
	          scan_ampm( str_ampm, &ampm ) ) ||
                sscanf( cp, "%d %400[a-zA-Z] %d %d:%d",
		  &tm_mday, str_mon, &tm_year, &tm_hour, &tm_min ) == 5 ) &&
	      scan_mon( str_mon, &tm_mon ) )
	{
	tm.tm_mday = tm_mday;
	tm.tm_mon = tm_mon;
	tm.tm_year = tm_year;
	tm.tm_hour = ampm_fix( tm_hour, ampm );
	tm.tm_min = tm_min;
	tm.tm_sec = 0;
	}

    /* DD mth HH:MM:SS ampm */
    else if ( ( ( sscanf( cp, "%d %400[a-zA-Z] %d:%d:%d %400[apmAPM]",
		    &tm_mday, str_mon, &tm_hour, &tm_min, &tm_sec,
		    str_ampm ) == 6 &&
	          scan_ampm( str_ampm, &ampm ) ) ||
                sscanf( cp, "%d %400[a-zA-Z] %d:%d:%d",
		  &tm_mday, str_mon, &tm_hour, &tm_min,
		  &tm_sec ) == 5 ) &&
	      scan_mon( str_mon, &tm_mon ) )
	{
	tm.tm_mday = tm_mday;
	tm.tm_mon = tm_mon;
	tm.tm_hour = ampm_fix( tm_hour, ampm );
	tm.tm_min = tm_min;
	tm.tm_sec = tm_sec;
	}
    /* DD mth HH:MM ampm */
    else if ( ( ( sscanf( cp, "%d %400[a-zA-Z] %d:%d %400[apmAPM]",
		    &tm_mday, str_mon, &tm_hour, &tm_min,
		    str_ampm ) == 5 &&
	          scan_ampm( str_ampm, &ampm ) ) ||
                sscanf( cp, "%d %400[a-zA-Z] %d:%d",
		    &tm_mday, str_mon, &tm_hour, &tm_min ) == 4 ) &&
	      scan_mon( str_mon, &tm_mon ) )
	{
	tm.tm_mday = tm_mday;
	tm.tm_mon = tm_mon;
	tm.tm_hour = ampm_fix( tm_hour, ampm );
	tm.tm_min = tm_min;
	tm.tm_sec = 0;
	}

    /* HH:MM:SS ampm zone DD-mth-YY */
    else if ( ( ( sscanf( cp, "%d:%d:%d %400[apmAPM] %400[^: 	\n] %d-%400[a-zA-Z]-%d",
		    &tm_hour, &tm_min, &tm_sec, str_ampm, str_gmtoff, &tm_mday,
		    str_mon, &tm_year ) == 8 &&
	          scan_ampm( str_ampm, &ampm ) ) ||
	        sscanf( cp, "%d:%d:%d %400[^: 	\n] %d-%400[a-zA-Z]-%d",
		  &tm_hour, &tm_min, &tm_sec, str_gmtoff, &tm_mday, str_mon,
		  &tm_year ) == 7 ) &&
	      scan_gmtoff( str_gmtoff, &gmtoff ) &&
	      scan_mon( str_mon, &tm_mon ) )
	{
	tm.tm_hour = ampm_fix( tm_hour, ampm );
	tm.tm_min = tm_min;
	tm.tm_sec = tm_sec;
	tm.tm_mday = tm_mday;
	tm.tm_mon = tm_mon;
	tm.tm_year = tm_year;
	}
    /* HH:MM ampm zone DD-mth-YY */
    else if ( ( ( sscanf( cp, "%d:%d %400[apmAPM] %400[^: 	\n] %d-%400[a-zA-Z]-%d",
		    &tm_hour, &tm_min, str_ampm, str_gmtoff, &tm_mday, str_mon,
		    &tm_year ) == 7 &&
	          scan_ampm( str_ampm, &ampm ) ) ||
	        sscanf( cp, "%d:%d %400[^: 	\n] %d-%400[a-zA-Z]-%d",
		  &tm_hour, &tm_min, str_gmtoff, &tm_mday, str_mon,
		  &tm_year ) == 6 ) &&
	      scan_gmtoff( str_gmtoff, &gmtoff ) &&
	      scan_mon( str_mon, &tm_mon ) )
	{
	tm.tm_hour = ampm_fix( tm_hour, ampm );
	tm.tm_min = tm_min;
	tm.tm_sec = 0;
	tm.tm_mday = tm_mday;
	tm.tm_mon = tm_mon;
	tm.tm_year = tm_year;
	}
    /* HH:MM:SS ampm DD-mth-YY */
    else if ( ( ( sscanf( cp, "%d:%d:%d %400[apmAPM] %d-%400[a-zA-Z]-%d",
		    &tm_hour, &tm_min, &tm_sec, str_ampm, &tm_mday, str_mon,
		    &tm_year ) == 7 &&
	          scan_ampm( str_ampm, &ampm ) ) ||
	        sscanf( cp, "%d:%d:%d %d-%400[a-zA-Z]-%d",
		  &tm_hour, &tm_min, &tm_sec, &tm_mday, str_mon,
		  &tm_year ) == 6 ) &&
	      scan_mon( str_mon, &tm_mon ) )
	{
	tm.tm_hour = ampm_fix( tm_hour, ampm );
	tm.tm_min = tm_min;
	tm.tm_sec = tm_sec;
	tm.tm_mday = tm_mday;
	tm.tm_mon = tm_mon;
	tm.tm_year = tm_year;
	}
    /* HH:MM ampm DD-mth-YY */
    else if ( ( ( sscanf( cp, "%d:%d %400[apmAPM] %d-%400[a-zA-Z]-%d",
		    &tm_hour, &tm_min, str_ampm, &tm_mday, str_mon,
		    &tm_year ) == 6 &&
	          scan_ampm( str_ampm, &ampm ) ) ||
	        sscanf( cp, "%d:%d %d-%400[a-zA-Z]-%d",
		  &tm_hour, &tm_min, &tm_mday, str_mon, &tm_year ) == 5 ) &&
	      scan_mon( str_mon, &tm_mon ) )
	{
	tm.tm_hour = ampm_fix( tm_hour, ampm );
	tm.tm_min = tm_min;
	tm.tm_sec = 0;
	tm.tm_mday = tm_mday;
	tm.tm_mon = tm_mon;
	tm.tm_year = tm_year;
	}

    /* HH:MM:SS ampm zone DD mth YY */
    else if ( ( ( sscanf( cp, "%d:%d:%d %400[apmAPM] %400[^: 	\n] %d %400[a-zA-Z] %d",
		    &tm_hour, &tm_min, &tm_sec, str_ampm, str_gmtoff, &tm_mday,
		    str_mon, &tm_year ) == 8 &&
	          scan_ampm( str_ampm, &ampm ) ) ||
	        sscanf( cp, "%d:%d:%d %400[^: 	\n] %d %400[a-zA-Z] %d",
		  &tm_hour, &tm_min, &tm_sec, str_gmtoff, &tm_mday, str_mon,
		  &tm_year ) == 7 ) &&
	      scan_gmtoff( str_gmtoff, &gmtoff ) &&
	      scan_mon( str_mon, &tm_mon ) )
	{
	tm.tm_hour = ampm_fix( tm_hour, ampm );
	tm.tm_min = tm_min;
	tm.tm_sec = tm_sec;
	tm.tm_mday = tm_mday;
	tm.tm_mon = tm_mon;
	tm.tm_year = tm_year;
	}
    /* HH:MM ampm zone DD mth YY */
    else if ( ( ( sscanf( cp, "%d:%d %400[apmAPM] %400[^: 	\n] %d %400[a-zA-Z] %d",
		    &tm_hour, &tm_min, str_ampm, str_gmtoff, &tm_mday, str_mon,
		    &tm_year ) == 7 &&
	          scan_ampm( str_ampm, &ampm ) ) ||
	        sscanf( cp, "%d:%d %400[^: 	\n] %d %400[a-zA-Z] %d",
		  &tm_hour, &tm_min, str_gmtoff, &tm_mday, str_mon,
		  &tm_year ) == 6 ) &&
	      scan_gmtoff( str_gmtoff, &gmtoff ) &&
	      scan_mon( str_mon, &tm_mon ) )
	{
	tm.tm_hour = ampm_fix( tm_hour, ampm );
	tm.tm_min = tm_min;
	tm.tm_sec = 0;
	tm.tm_mday = tm_mday;
	tm.tm_mon = tm_mon;
	tm.tm_year = tm_year;
	}
    /* HH:MM:SS ampm DD mth YY */
    else if ( ( ( sscanf( cp, "%d:%d:%d %400[apmAPM] %d %400[a-zA-Z] %d",
		    &tm_hour, &tm_min, &tm_sec, str_ampm, &tm_mday, str_mon,
		    &tm_year ) == 7 &&
	          scan_ampm( str_ampm, &ampm ) ) ||
	        sscanf( cp, "%d:%d:%d %d %400[a-zA-Z] %d",
		  &tm_hour, &tm_min, &tm_sec, &tm_mday, str_mon,
		  &tm_year ) == 6 ) &&
	      scan_mon( str_mon, &tm_mon ) )
	{
	tm.tm_hour = ampm_fix( tm_hour, ampm );
	tm.tm_min = tm_min;
	tm.tm_sec = tm_sec;
	tm.tm_mday = tm_mday;
	tm.tm_mon = tm_mon;
	tm.tm_year = tm_year;
	}
    /* HH:MM ampm DD mth YY */
    else if ( ( ( sscanf( cp, "%d:%d %400[apmAPM] %d %400[a-zA-Z] %d",
		    &tm_hour, &tm_min, str_ampm, &tm_mday, str_mon,
		    &tm_year ) == 6 &&
	          scan_ampm( str_ampm, &ampm ) ) ||
	        sscanf( cp, "%d:%d %d %400[a-zA-Z] %d",
		  &tm_hour, &tm_min, &tm_mday, str_mon, &tm_year ) == 5 ) &&
	      scan_mon( str_mon, &tm_mon ) )
	{
	tm.tm_hour = ampm_fix( tm_hour, ampm );
	tm.tm_min = tm_min;
	tm.tm_sec = 0;
	tm.tm_mday = tm_mday;
	tm.tm_mon = tm_mon;
	tm.tm_year = tm_year;
	}

    /* wdy, DD-mth-YY HH:MM:SS ampm zone */
    else if ( ( ( sscanf( cp, "%400[a-zA-Z,] %d-%400[a-zA-Z]-%d %d:%d:%d %400[apmAPM] %400[^: 	\n]",
		    str_wday, &tm_mday, str_mon, &tm_year, &tm_hour, &tm_min,
		    &tm_sec, str_ampm, str_gmtoff ) == 9 &&
	          scan_ampm( str_ampm, &ampm ) ) ||
	        sscanf( cp, "%400[a-zA-Z], %d-%400[a-zA-Z]-%d %d:%d:%d %400[^: 	\n]",
		  str_wday, &tm_mday, str_mon, &tm_year, &tm_hour, &tm_min,
		  &tm_sec, str_gmtoff ) == 8 ) &&
	      scan_wday( str_wday, &tm_wday ) &&
	      scan_mon( str_mon, &tm_mon ) &&
	      scan_gmtoff( str_gmtoff, &gmtoff ) )
	{
	tm.tm_wday = tm_wday;
	tm.tm_mday = tm_mday;
	tm.tm_mon = tm_mon;
	tm.tm_year = tm_year;
	tm.tm_hour = ampm_fix( tm_hour, ampm );
	tm.tm_min = tm_min;
	tm.tm_sec = tm_sec;
	got_zone = 1;
	}
    /* wdy, DD-mth-YY HH:MM ampm zone */
    else if ( ( ( sscanf( cp, "%400[a-zA-Z,] %d-%400[a-zA-Z]-%d %d:%d %400[apmAPM] %400[^: 	\n]",
		    str_wday, &tm_mday, str_mon, &tm_year, &tm_hour, &tm_min,
		    str_ampm, str_gmtoff ) == 8 &&
	          scan_ampm( str_ampm, &ampm ) ) ||
	        sscanf( cp, "%400[a-zA-Z], %d-%400[a-zA-Z]-%d %d:%d %400[^: 	\n]",
		  str_wday, &tm_mday, str_mon, &tm_year, &tm_hour, &tm_min,
		  str_gmtoff ) == 7 ) &&
	      scan_wday( str_wday, &tm_wday ) &&
	      scan_mon( str_mon, &tm_mon ) &&
	      scan_gmtoff( str_gmtoff, &gmtoff ) )
	{
	tm.tm_wday = tm_wday;
	tm.tm_mday = tm_mday;
	tm.tm_mon = tm_mon;
	tm.tm_year = tm_year;
	tm.tm_hour = ampm_fix( tm_hour, ampm );
	tm.tm_min = tm_min;
	tm.tm_sec = 0;
	got_zone = 1;
	}
    /* wdy, DD-mth-YY HH:MM:SS ampm */
    else if ( ( ( sscanf( cp, "%400[a-zA-Z,] %d-%400[a-zA-Z]-%d %d:%d:%d %400[apmAPM]",
		    str_wday, &tm_mday, str_mon, &tm_year, &tm_hour, &tm_min,
		    &tm_sec, str_ampm ) == 8 &&
	          scan_ampm( str_ampm, &ampm ) ) ||
	        sscanf( cp, "%400[a-zA-Z], %d-%400[a-zA-Z]-%d %d:%d:%d",
		  str_wday, &tm_mday, str_mon, &tm_year, &tm_hour, &tm_min,
		  &tm_sec ) == 7 ) &&
	      scan_wday( str_wday, &tm_wday ) &&
	      scan_mon( str_mon, &tm_mon ) )
	{
	tm.tm_wday = tm_wday;
	tm.tm_mday = tm_mday;
	tm.tm_mon = tm_mon;
	tm.tm_year = tm_year;
	tm.tm_hour = ampm_fix( tm_hour, ampm );
	tm.tm_min = tm_min;
	tm.tm_sec = tm_sec;
	}
    /* wdy, DD-mth-YY HH:MM ampm */
    else if ( ( ( sscanf( cp, "%400[a-zA-Z,] %d-%400[a-zA-Z]-%d %d:%d %400[apmAPM]",
		    str_wday, &tm_mday, str_mon, &tm_year, &tm_hour, &tm_min,
		    str_ampm ) == 7 &&
	          scan_ampm( str_ampm, &ampm ) ) ||
	        sscanf( cp, "%400[a-zA-Z], %d-%400[a-zA-Z]-%d %d:%d",
		  str_wday, &tm_mday, str_mon, &tm_year, &tm_hour,
		  &tm_min ) == 6 ) &&
	      scan_wday( str_wday, &tm_wday ) &&
	      scan_mon( str_mon, &tm_mon ) )
	{
	tm.tm_wday = tm_wday;
	tm.tm_mday = tm_mday;
	tm.tm_mon = tm_mon;
	tm.tm_year = tm_year;
	tm.tm_hour = ampm_fix( tm_hour, ampm );
	tm.tm_min = tm_min;
	tm.tm_sec = 0;
	}

    /* wdy, DD mth YY HH:MM:SS ampm zone */
    else if ( ( ( sscanf( cp, "%400[a-zA-Z,] %d %400[a-zA-Z] %d %d:%d:%d %400[apmAPM] %400[^: 	\n]",
		    str_wday, &tm_mday, str_mon, &tm_year, &tm_hour, &tm_min,
		    &tm_sec, str_ampm, str_gmtoff ) == 9 &&
	          scan_ampm( str_ampm, &ampm ) ) ||
	        sscanf( cp, "%400[a-zA-Z], %d %400[a-zA-Z] %d %d:%d:%d %400[^: 	\n]",
		  str_wday, &tm_mday, str_mon, &tm_year, &tm_hour, &tm_min,
		  &tm_sec, str_gmtoff ) == 8 ) &&
	      scan_wday( str_wday, &tm_wday ) &&
	      scan_mon( str_mon, &tm_mon ) &&
	      scan_gmtoff( str_gmtoff, &gmtoff ) )
	{
	tm.tm_wday = tm_wday;
	tm.tm_mday = tm_mday;
	tm.tm_mon = tm_mon;
	tm.tm_year = tm_year;
	tm.tm_hour = ampm_fix( tm_hour, ampm );
	tm.tm_min = tm_min;
	tm.tm_sec = tm_sec;
	got_zone = 1;
	}
    /* wdy, DD mth YY HH:MM ampm zone */
    else if ( ( ( sscanf( cp, "%400[a-zA-Z,] %d %400[a-zA-Z] %d %d:%d %400[apmAPM] %400[^: 	\n]",
		    str_wday, &tm_mday, str_mon, &tm_year, &tm_hour, &tm_min,
		    str_ampm, str_gmtoff ) == 8 &&
	          scan_ampm( str_ampm, &ampm ) ) ||
	        sscanf( cp, "%400[a-zA-Z], %d %400[a-zA-Z] %d %d:%d %400[^: 	\n]",
		  str_wday, &tm_mday, str_mon, &tm_year, &tm_hour, &tm_min,
		  str_gmtoff ) == 7 ) &&
	      scan_wday( str_wday, &tm_wday ) &&
	      scan_mon( str_mon, &tm_mon ) &&
	      scan_gmtoff( str_gmtoff, &gmtoff ) )
	{
	tm.tm_wday = tm_wday;
	tm.tm_mday = tm_mday;
	tm.tm_mon = tm_mon;
	tm.tm_year = tm_year;
	tm.tm_hour = ampm_fix( tm_hour, ampm );
	tm.tm_min = tm_min;
	tm.tm_sec = 0;
	got_zone = 1;
	}
    /* wdy, DD mth YY HH:MM:SS ampm */
    else if ( ( ( sscanf( cp, "%400[a-zA-Z,] %d %400[a-zA-Z] %d %d:%d:%d %400[apmAPM]",
		    str_wday, &tm_mday, str_mon, &tm_year, &tm_hour, &tm_min,
		    &tm_sec, str_ampm ) == 8 &&
	          scan_ampm( str_ampm, &ampm ) ) ||
	        sscanf( cp, "%400[a-zA-Z], %d %400[a-zA-Z] %d %d:%d:%d",
		  str_wday, &tm_mday, str_mon, &tm_year, &tm_hour, &tm_min,
		  &tm_sec ) == 7 ) &&
	      scan_wday( str_wday, &tm_wday ) &&
	      scan_mon( str_mon, &tm_mon ) )
	{
	tm.tm_wday = tm_wday;
	tm.tm_mday = tm_mday;
	tm.tm_mon = tm_mon;
	tm.tm_year = tm_year;
	tm.tm_hour = ampm_fix( tm_hour, ampm );
	tm.tm_min = tm_min;
	tm.tm_sec = tm_sec;
	}
    /* wdy, DD mth YY HH:MM ampm */
    else if ( ( ( sscanf( cp, "%400[a-zA-Z,] %d %400[a-zA-Z] %d %d:%d %400[apmAPM]",
		    str_wday, &tm_mday, str_mon, &tm_year, &tm_hour, &tm_min,
		    str_ampm ) == 7 &&
	          scan_ampm( str_ampm, &ampm ) ) ||
	        sscanf( cp, "%400[a-zA-Z], %d %400[a-zA-Z] %d %d:%d",
		  str_wday, &tm_mday, str_mon, &tm_year, &tm_hour,
		  &tm_min ) == 6 ) &&
	      scan_wday( str_wday, &tm_wday ) &&
	      scan_mon( str_mon, &tm_mon ) )
	{
	tm.tm_wday = tm_wday;
	tm.tm_mday = tm_mday;
	tm.tm_mon = tm_mon;
	tm.tm_year = tm_year;
	tm.tm_hour = ampm_fix( tm_hour, ampm );
	tm.tm_min = tm_min;
	tm.tm_sec = 0;
	}

    /* wdy, mth DD HH:MM:SS ampm zone YY */
    else if ( ( ( sscanf( cp, "%400[a-zA-Z,] %400[a-zA-Z] %d %d:%d:%d %400[apmAPM] %400[^: 	\n] %d",
		str_wday, str_mon, &tm_mday, &tm_hour, &tm_min, &tm_sec,
		str_ampm, str_gmtoff, &tm_year ) == 9 &&
	    scan_ampm( str_ampm, &ampm ) ) ||
	    sscanf( cp, "%400[a-zA-Z] %400[a-zA-Z] %d %d:%d:%d %400[^: 	\n] %d",
		str_wday, str_mon, &tm_mday, &tm_hour, &tm_min, &tm_sec,
		str_gmtoff, &tm_year ) == 8 ) &&
	    scan_wday( str_wday, &tm_wday ) &&
	    scan_mon( str_mon, &tm_mon ) &&
	    scan_gmtoff( str_gmtoff, &gmtoff ) )
	{
	tm.tm_wday = tm_wday;
	tm.tm_mon = tm_mon;
	tm.tm_mday = tm_mday;
	tm.tm_hour = ampm_fix( tm_hour, ampm );
	tm.tm_min = tm_min;
	tm.tm_sec = tm_sec;
	got_zone = 1;
	tm.tm_year = tm_year;
	}
    /* wdy, mth DD HH:MM ampm zone YY */
    else if ( ( ( sscanf( cp, "%400[a-zA-Z,] %400[a-zA-Z] %d %d:%d %400[apmAPM] %400[^: 	\n] %d",
	            str_wday, str_mon, &tm_mday, &tm_hour, &tm_min,
	            str_ampm, str_gmtoff, &tm_year ) == 8 &&
	          scan_ampm( str_ampm, &ampm ) ) ||
	        sscanf( cp, "%400[a-zA-Z] %400[a-zA-Z] %d %d:%d %400[^: 	\n] %d",
		  str_wday, str_mon, &tm_mday, &tm_hour, &tm_min,
		  str_gmtoff, &tm_year ) == 7 ) &&
	      scan_wday( str_wday, &tm_wday ) &&
	      scan_mon( str_mon, &tm_mon ) &&
	      scan_gmtoff( str_gmtoff, &gmtoff ) )
	{
	tm.tm_wday = tm_wday;
	tm.tm_mon = tm_mon;
	tm.tm_mday = tm_mday;
	tm.tm_hour = ampm_fix( tm_hour, ampm );
	tm.tm_min = tm_min;
	tm.tm_sec = 0;
	got_zone = 1;
	tm.tm_year = tm_year;
	}
    /* wdy, mth DD HH:MM:SS ampm YY */
    else if ( ( ( sscanf( cp, "%400[a-zA-Z,] %400[a-zA-Z] %d %d:%d:%d %400[apmAPM] %d",
		str_wday, str_mon, &tm_mday, &tm_hour, &tm_min, &tm_sec,
		str_ampm, &tm_year ) == 8 &&
	    scan_ampm( str_ampm, &ampm ) ) ||
	    sscanf( cp, "%400[a-zA-Z] %400[a-zA-Z] %d %d:%d:%d %d",
		str_wday, str_mon, &tm_mday, &tm_hour, &tm_min, &tm_sec,
		&tm_year ) == 7 ) &&
	    scan_wday( str_wday, &tm_wday ) &&
	    scan_mon( str_mon, &tm_mon ) )
	{
	tm.tm_wday = tm_wday;
	tm.tm_mon = tm_mon;
	tm.tm_mday = tm_mday;
	tm.tm_hour = ampm_fix( tm_hour, ampm );
	tm.tm_min = tm_min;
	tm.tm_sec = tm_sec;
	tm.tm_year = tm_year;
	}
    /* wdy, mth DD HH:MM ampm YY */
    else if ( ( ( sscanf( cp, "%400[a-zA-Z,] %400[a-zA-Z] %d %d:%d %400[apmAPM] %d",
	            str_wday, str_mon, &tm_mday, &tm_hour, &tm_min,
	            str_ampm, &tm_year ) == 7 &&
	          scan_ampm( str_ampm, &ampm ) ) ||
	        sscanf( cp, "%400[a-zA-Z] %400[a-zA-Z] %d %d:%d %d",
		  str_wday, str_mon, &tm_mday, &tm_hour, &tm_min,
		  &tm_year ) == 6 ) &&
	      scan_wday( str_wday, &tm_wday ) &&
	      scan_mon( str_mon, &tm_mon ) )
	{
	tm.tm_wday = tm_wday;
	tm.tm_mon = tm_mon;
	tm.tm_mday = tm_mday;
	tm.tm_hour = ampm_fix( tm_hour, ampm );
	tm.tm_min = tm_min;
	tm.tm_sec = 0;
	tm.tm_year = tm_year;
	}

    /* mth DD HH:MM:SS ampm */
    else if ( ( ( sscanf( cp, "%400[a-zA-Z] %d %d:%d:%d %400[apmAPM]",
		    str_mon, &tm_mday, &tm_hour, &tm_min, &tm_sec,
		    str_ampm ) == 6 &&
	          scan_ampm( str_ampm, &ampm ) ) ||
                sscanf( cp, "%400[a-zA-Z] %d %d:%d:%d",
		  str_mon, &tm_mday, &tm_hour, &tm_min,
		  &tm_sec ) == 5 ) &&
	      scan_mon( str_mon, &tm_mon ) )
	{
	tm.tm_mon = tm_mon;
	tm.tm_mday = tm_mday;
	tm.tm_hour = ampm_fix( tm_hour, ampm );
	tm.tm_min = tm_min;
	tm.tm_sec = tm_sec;
	}
    /* mth DD HH:MM ampm */
    else if ( ( ( sscanf( cp, "%400[a-zA-Z] %d %d:%d %400[apmAPM]",
		    str_mon, &tm_mday, &tm_hour, &tm_min,
		  str_ampm ) == 5 &&
	          scan_ampm( str_ampm, &ampm ) ) ||
                sscanf( cp, "%400[a-zA-Z] %d %d:%d",
		  str_mon, &tm_mday, &tm_hour, &tm_min ) == 4 ) &&
	      scan_mon( str_mon, &tm_mon ) )
	{
	tm.tm_mon = tm_mon;
	tm.tm_mday = tm_mday;
	tm.tm_hour = ampm_fix( tm_hour, ampm );
	tm.tm_min = tm_min;
	tm.tm_sec = 0;
	}

    /* DD-mth-YY */
    else if ( sscanf( cp, "%d-%400[a-zA-Z]-%d",
	        &tm_mday, str_mon, &tm_year ) == 3 &&
	      scan_mon( str_mon, &tm_mon ) )
	{
	tm.tm_mday = tm_mday;
	tm.tm_mon = tm_mon;
	tm.tm_year = tm_year;
	tm.tm_hour = 0;
	tm.tm_min = 0;
	tm.tm_sec = 0;
	}
    /* DD mth YY */
    else if ( sscanf( cp, "%d %400[a-zA-Z] %d",
	        &tm_mday, str_mon, &tm_year ) == 3 &&
	      scan_mon( str_mon, &tm_mon ) )
	{
	tm.tm_mday = tm_mday;
	tm.tm_mon = tm_mon;
	tm.tm_year = tm_year;
	tm.tm_hour = 0;
	tm.tm_min = 0;
	tm.tm_sec = 0;
	}

    /* HH:MM:SS ampm */
    else if ( ( sscanf( cp, "%d:%d:%d %400[apmAPM]",
	          &tm_hour, &tm_min, &tm_sec, str_ampm ) == 4 &&
	        scan_ampm( str_ampm, &ampm ) ) ||
	      sscanf( cp, "%d:%d:%d", &tm_hour, &tm_min, &tm_sec ) == 3 )
	{
	tm.tm_hour = ampm_fix( tm_hour, ampm );
	tm.tm_min = tm_min;
	tm.tm_sec = tm_sec;
	}
    /* HH:MM ampm */
    else if ( ( sscanf( cp, "%d:%d %400[apmAPM]", &tm_hour, &tm_min,
		  str_ampm ) == 3 &&
	        scan_ampm( str_ampm, &ampm ) ) ||
	      sscanf( cp, "%d:%d", &tm_hour, &tm_min ) == 2 )
	{
	tm.tm_hour = ampm_fix( tm_hour, ampm );
	tm.tm_min = tm_min;
	tm.tm_sec = 0;
	}
    else if ( sscanf( cp, "%d", &i ) == 1 &&
	      i >= 300000000 && i <= 2000000000 )
	      /*     ~1980             ~2033   */
	{
	t = (time_t) i;
	return t;
	}
    else
	return (time_t) -1;

    if ( tm.tm_year > 1900 )
	tm.tm_year -= 1900;
    else if ( tm.tm_year < 70 )
	tm.tm_year += 100;

    t = tm_to_time( &tm );
    t -= gmtoff;
#ifdef notdef
    /* This doesn't seem to be necessary, at least on SunOS. */
    if ( tm.tm_isdst && ! got_zone )
	t -= 60*60;
#endif

    return t;
    }


static void
pound_case( char* str )
    {
    for ( ; *str != '\0'; ++str )
	{
	if ( isupper( (int) *str ) )
	    *str = tolower( (int) *str );
	}
    }


static int
strlong_compare( const void* v1, const void* v2 )
    {
    const struct strlong* s1 = (const struct strlong*) v1;
    const struct strlong* s2 = (const struct strlong*) v2;

    return strcmp( s1->s, s2->s );
    }


static int
strlong_search( char* str, struct strlong* tab, int n, long* lP )
    {
    int i, h, l, r;

    l = 0;
    h = n - 1;
    for (;;)
	{
	i = ( h + l ) / 2;
	r = strcmp( str, tab[i].s );
	if ( r < 0 )
	    h = i - 1;
	else if ( r > 0 )
	    l = i + 1;
	else
	    {
	    *lP = tab[i].l;
	    return 1;
	    }
	if ( h < l )
	    return 0;
	}
    }


static int
ampm_fix( int hour, int ampm )
    {
    switch ( ampm )
	{
	case AMPM_NONE:
	break;
	case AMPM_AM:
	if ( hour == 12 )
	    hour = 0;
	break;
	case AMPM_PM:
	if ( hour != 12 )
	    hour += 12;
	break;
	}
    return hour;
    }


static int
scan_ampm( char* str_ampm, long* ampmP )
    {
    static struct strlong ampm_tab[] = {
	{ "am", AMPM_AM }, { "pm", AMPM_PM },
	};
    static int sorted = 0;

    if ( ! sorted )
	{
	(void) qsort(
	    ampm_tab, sizeof(ampm_tab)/sizeof(struct strlong),
	    sizeof(struct strlong), strlong_compare );
	sorted = 1;
	}
    pound_case( str_ampm );
    return strlong_search(
	str_ampm, ampm_tab, sizeof(ampm_tab)/sizeof(struct strlong), ampmP );
    }


static int
scan_wday( char* str_wday, long* tm_wdayP )
    {
    static struct strlong wday_tab[] = {
	{ "sun", 0 }, { "sunday", 0 },
	{ "mon", 1 }, { "monday", 1 },
	{ "tue", 2 }, { "tuesday", 2 },
	{ "wed", 3 }, { "wednesday", 3 },
	{ "thu", 4 }, { "thursday", 4 },
	{ "fri", 5 }, { "friday", 5 },
	{ "sat", 6 }, { "saturday", 6 },
	};
    static int sorted = 0;

    if ( ! sorted )
	{
	(void) qsort(
	    wday_tab, sizeof(wday_tab)/sizeof(struct strlong),
	    sizeof(struct strlong), strlong_compare );
	sorted = 1;
	}
    pound_case( str_wday );
    return strlong_search(
	str_wday, wday_tab, sizeof(wday_tab)/sizeof(struct strlong), tm_wdayP );
    }


static int
scan_mon( char* str_mon, long* tm_monP )
    {
    static struct strlong mon_tab[] = {
	{ "jan", 0 }, { "january", 0 },
	{ "feb", 1 }, { "february", 1 },
	{ "mar", 2 }, { "march", 2 },
	{ "apr", 3 }, { "april", 3 },
	{ "may", 4 },
	{ "jun", 5 }, { "june", 5 },
	{ "jul", 6 }, { "july", 6 },
	{ "aug", 7 }, { "august", 7 },
	{ "sep", 8 }, { "september", 8 },
	{ "oct", 9 }, { "october", 9 },
	{ "nov", 10 }, { "november", 10 },
	{ "dec", 11 }, { "december", 11 },
	};
    static int sorted = 0;

    if ( ! sorted )
	{
	(void) qsort(
	    mon_tab, sizeof(mon_tab)/sizeof(struct strlong),
	    sizeof(struct strlong), strlong_compare );
	sorted = 1;
	}
    pound_case( str_mon );
    return strlong_search(
	str_mon, mon_tab, sizeof(mon_tab)/sizeof(struct strlong), tm_monP );
    }


static int
scan_gmtoff( char* str_gmtoff, long* gmtoffP )
    {
    static struct strlong gmtoff_tab[] = {
	{ "gmt", 0L }, { "utc", 0L }, { "ut", 0L },
	{ "0000", 0L }, { "+0000", 0L }, { "-0000", 0L },
	{ "0100", 3600L }, { "+0100", 3600L }, { "-0100", -3600L },
	{ "0200", 7200L }, { "+0200", 7200L }, { "-0200", -7200L },
	{ "0300", 10800L }, { "+0300", 10800L }, { "-0300", -10800L },
	{ "0400", 14400L }, { "+0400", 14400L }, { "-0400", -14400L },
	{ "0500", 18000L }, { "+0500", 18000L }, { "-0500", -18000L },
	{ "0600", 21600L }, { "+0600", 21600L }, { "-0600", -21600L },
	{ "0700", 25200L }, { "+0700", 25200L }, { "-0700", -25200L },
	{ "0800", 28800L }, { "+0800", 28800L }, { "-0800", -28800L },
	{ "0900", 32400L }, { "+0900", 32400L }, { "-0900", -32400L },
	{ "1000", 36000L }, { "+1000", 36000L }, { "-1000", -36000L },
	{ "1100", 39600L }, { "+1100", 39600L }, { "-1100", -39600L },
	{ "1200", 43200L }, { "+1200", 43200L }, { "-1200", -43200L },
	{ "cet", 3600L }, { "ced", 7200L },	/* Central European time */
	{ "mez", 3600L }, { "mesz", 7200L },	/* Mitteleuropdische Zeit */
	{ "jst", 32400L }, { "jdt", 36000L },	/* Japan time */
	{ "bst", -3600L },
	{ "nst", -12600L },
	{ "ast", -14400L }, { "adt", -10800L },
	{ "est", -18000L }, { "edt", -14400L },
	{ "cst", -21600L }, { "cdt", -18000L },
	{ "mst", -25200L }, { "mdt", -21600L },
	{ "pst", -28800L }, { "pdt", -25200L },
	{ "yst", -32400L }, { "ydt", -28800L },
	{ "hst", -36000L }, { "hdt", -32400L },
	{ "a", -3600L }, { "b", -7200L }, { "c", -10800L }, { "d", -14400L },
	{ "e", -18000L }, { "f", -21600L }, { "g", -25200L }, { "h", -28800L },
	{ "i", -32400L }, { "k", -36000L }, { "l", -39600L }, { "m", -43200L },
	{ "n", 3600L }, { "o", 7200L }, { "p", 10800L }, { "q", 14400L },
	{ "r", 18000L }, { "s", 21600L }, { "t", 25200L }, { "u", 28800L },
	{ "v", 32400L }, { "w", 36000L }, { "x", 39600L }, { "y", 43200L },
	{ "z", 0L },
	};
    static int sorted = 0;

    if ( ! sorted )
	{
	(void) qsort(
	    gmtoff_tab, sizeof(gmtoff_tab)/sizeof(struct strlong),
	    sizeof(struct strlong), strlong_compare );
	sorted = 1;
	}
    pound_case( str_gmtoff );
    return strlong_search(
	str_gmtoff, gmtoff_tab, sizeof(gmtoff_tab)/sizeof(struct strlong),
	gmtoffP );
    }


static int
is_leap( int year )
    {
    return year % 400? ( year % 100 ? ( year % 4 ? 0 : 1 ) : 0 ) : 1;
    }


/* Basically the same as mktime(). */
static time_t
tm_to_time( struct tm* tmP )
    {
    time_t t;
    static int monthtab[12] = {
	0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };

    /* Years since epoch, converted to days. */
    t = ( tmP->tm_year - 70 ) * 365;
    /* Leap days for previous years - this will break in 2100! */
    t += ( tmP->tm_year - 69 ) / 4;
    /* Days for the beginning of this month. */
    t += monthtab[tmP->tm_mon];
    /* Leap day for this year. */
    if ( tmP->tm_mon >= 2 && is_leap( tmP->tm_year + 1900 ) )
	++t;
    /* Days since the beginning of this month. */
    t += tmP->tm_mday - 1;	/* 1-based field */
    /* Hours, minutes, and seconds. */
    t = t * 24 + tmP->tm_hour;
    t = t * 60 + tmP->tm_min;
    t = t * 60 + tmP->tm_sec;

    return t;
    }
