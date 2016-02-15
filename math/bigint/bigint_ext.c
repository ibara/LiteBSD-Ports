/* bigint_ext - external portion of large integer package
**
** Copyright © 2000 by Jef Poskanzer <jef@mail.acme.com>.
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

#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "bigint.h"
#include "low_primes.h"


#define FERMAT
#define FERMAT2
#define FERMAT3
#define SOLOVAY_STRASSEN
#define MILLER_RABIN

/* Forwards. */
static int char_to_digit( char c );
static char digit_to_char( int d );
static void print_pos( FILE* f, bigint bi, int base );
static int null_test( bigint bi, bigint bim1 );
#ifdef FERMAT
static int fermat_test( bigint bi, bigint bim1 );
#endif
#ifdef FERMAT2
static int fermat2_test( bigint bi, bigint bim1 );
#endif
#ifdef FERMAT3
static int fermat3_test( bigint bi, bigint bim1 );
#endif
#ifdef SOLOVAY_STRASSEN
static int solovay_strassen_test( bigint bi, bigint bim1, bigint bim1o2 );
#endif
#ifdef MILLER_RABIN
static int miller_rabin_test( bigint bi, bigint bim1, bigint s, int r );
#endif


bigint
str_to_bi( char* str )
    {
    return str_to_bi_base( str, 10 );
    }


bigint
str_to_bi_base( char* str, int base )
    {
    int sign;
    bigint biR;
    int d;

    sign = 1;
    if ( *str == '-' )
	{
	sign = -1;
	++str;
	}
    for ( biR = bi_0; ; ++str )
	{
	d = char_to_digit( *str );
	if ( d == -1 || d >= base )
	    break;
	biR = bi_int_add( bi_int_multiply( biR, base ), d );
	}
    if ( sign == -1 )
	biR = bi_negate( biR );
    return biR;
    }


bigint
bi_scan( FILE* f )
    {
    return bi_scan_base( f, 10 );
    }


bigint
bi_scan_base( FILE* f, int base )
    {
    int sign;
    bigint biR;
    int c;
    int d;

    sign = 1;
    c = getc( f );
    if ( c == '-' )
	sign = -1;
    else
	ungetc( c, f );

    biR = bi_0;
    for (;;)
	{
	c = getc( f );
	d = char_to_digit( c );
	if ( d == -1 || d >= base )
	    break;
	biR = bi_int_add( bi_int_multiply( biR, base ), d );
	}

    if ( sign == -1 )
	biR = bi_negate( biR );
    return biR;
    }


static int
char_to_digit( char c )
    {
    if ( c >= '0' && c <= '9' )
	return c - '0';
    else if ( c >= 'a' && c <= 'z' )
	return c - 'a' + 10;
    else if ( c >= 'A' && c <= 'A' )
	return c - 'A' + 10;
    else
	return -1;
    }


static char
digit_to_char( int d )
    {
    if ( d >= 0 && d <= 9 )
	return d + '0';
    else
	return d - 10 + 'a';
    }


void
bi_print( FILE* f, bigint bi )
    {
    bi_print_base( f, bi, 10 );
    }


void
bi_print_base( FILE* f, bigint bi, int base )
    {
    if ( bi_is_negative( bi_copy( bi ) ) )
	{
	putc( '-', f );
	bi = bi_negate( bi );
	}
    print_pos( f, bi, base );
    }


static void
print_pos( FILE* f, bigint bi, int base )
    {
    if ( bi_compare( bi_copy( bi ), int_to_bi( base ) ) >= 0 )
	print_pos( f, bi_int_divide( bi_copy( bi ), base ), base );
    putc( digit_to_char( bi_int_mod( bi, base ) ), f );
    }


int
bi_int_mod( bigint bi, int m )
    {
    int r;

    if ( m <= 0 )
	{
	(void) fprintf( stderr, "bi_int_mod: zero or negative modulus\n" );
	(void) kill( getpid(), SIGFPE );
	}
    r = bi_int_rem( bi, m );
    if ( r < 0 )
	r += m;
    return r;
    }


bigint
bi_rem( bigint bia, bigint bim )
    {
    return bi_subtract(
	bia, bi_multiply( bi_divide( bi_copy( bia ), bi_copy( bim ) ), bim ) );
    }


bigint
bi_mod( bigint bia, bigint bim )
    {
    bigint biR;

    if ( bi_compare( bi_copy( bim ), bi_0 ) <= 0 )
	{
	(void) fprintf( stderr, "bi_mod: zero or negative modulus\n" );
	(void) kill( getpid(), SIGFPE );
	}
    biR = bi_rem( bia, bi_copy( bim ) );
    if ( bi_is_negative( bi_copy( biR ) ) )
	biR = bi_add( biR, bim );
    else
	bi_free( bim );
    return biR;
    }


bigint
bi_power( bigint bi, bigint biexp )
    {
    bigint biR;
    int bits, bitnum;

    if ( bi_is_negative( bi_copy( biexp ) ) )
	{
	(void) fprintf( stderr, "bi_power: negative exponent\n" );
	(void) kill( getpid(), SIGFPE );
	}
    bits = bi_bits( bi_copy( biexp ) );
    if ( bi_bit( bi_copy( biexp ), 0 ) )
	biR = bi_copy( bi );
    else
	biR = bi_1;
    bitnum = 0;
    for ( bitnum = 1; bitnum < bits; ++bitnum )
	{
	bi = bi_square( bi );
	if ( bi_bit( bi_copy( biexp ), bitnum ) )
	    biR = bi_multiply( biR, bi_copy( bi ) );
	}
    bi_free( bi );
    bi_free( biexp );
    return biR;
    }


bigint
bi_factorial( bigint bi )
    {
    bigint biR;

    biR = bi_1;
    while ( bi_compare( bi_copy( bi ), bi_1 ) > 0 )
	{
	biR = bi_multiply( biR, bi_copy( bi ) );
	bi = bi_int_subtract( bi, 1 );
	}
    bi_free( bi );
    return biR;
    }


int
bi_is_even( bigint bi )
    {
    return ! bi_is_odd( bi );
    }


int
bi_digits( bigint bi )
    {
    long long b;

    b = bi_bits( bi );
    return ( b * 1000001 + 3321928 ) / 3321929;
    }


bigint
bi_mod_add( bigint bia, bigint bib, bigint bim )
    {
    bigint biR;

    if ( bi_compare( bi_copy( bim ), bi_0 ) <= 0 )
	{
	(void) fprintf( stderr, "bi_mod: zero or negative modulus\n" );
	(void) kill( getpid(), SIGFPE );
	}
    /* If either operand is outside of [0..bim), do a regular add & mod. */
    if ( bi_is_negative( bi_copy( bia ) ) ||
         bi_is_negative( bi_copy( bib ) ) ||
	 bi_compare( bi_copy( bia ), bi_copy( bim ) ) >= 0 ||
         bi_compare( bi_copy( bib ), bi_copy( bim ) ) >= 0 )
	return bi_mod( bi_add( bia, bib ), bim );

    /* Both operands are in [0..bim), therefore the sum will be outside
    ** of that range by at most one factor of bim.
    */
    biR = bi_add( bia, bib );
    if ( bi_compare( bi_copy( biR ), bi_copy( bim ) ) >= 0 )
	return bi_subtract( biR, bim );
    bi_free( bim );
    return biR;
    }


bigint
bi_mod_subtract( bigint bia, bigint bib, bigint bim )
    {
    bigint biR;

    if ( bi_compare( bi_copy( bim ), bi_0 ) <= 0 )
	{
	(void) fprintf( stderr, "bi_mod: zero or negative modulus\n" );
	(void) kill( getpid(), SIGFPE );
	}
    /* If either operand is outside of [0..bim), do a regular subtract & mod. */
    if ( bi_is_negative( bi_copy( bia ) ) ||
         bi_is_negative( bi_copy( bib ) ) ||
	 bi_compare( bi_copy( bia ), bi_copy( bim ) ) >= 0 ||
         bi_compare( bi_copy( bib ), bi_copy( bim ) ) >= 0 )
	return bi_mod( bi_subtract( bia, bib ), bim );

    /* Both operands are in [0..bim), therefore the difference will be outside
    ** of that range by at most one factor of bim.
    */
    biR = bi_subtract( bia, bib );
    if ( bi_is_negative( bi_copy( biR ) ) )
	return bi_add( biR, bim );
    bi_free( bim );
    return biR;
    }


bigint
bi_mod_power( bigint bi, bigint biexp, bigint bim )
    {
    int invert;
    bigint biR;
    int bits, bitnum;

    invert = 0;
    if ( bi_is_negative( bi_copy( biexp ) ) )
	{
	biexp = bi_negate( biexp );
	invert = 1;
	}

    if ( bi_bit( bi_copy( biexp ), 0 ) )
	biR = bi_mod( bi_copy( bi ), bi_copy( bim ) );
    else
	biR = bi_1;
    bits = bi_bits( bi_copy( biexp ) );
    for ( bitnum = 1; bitnum < bits; ++bitnum )
	{
	bi = bi_mod_square( bi, bi_copy( bim ) );
	if ( bi_bit( bi_copy( biexp ), bitnum ) )
	    biR = bi_mod_multiply( biR, bi_copy( bi ), bi_copy( bim ) );
	}
    bi_free( bi );
    bi_free( biexp );

    if ( invert )
	biR = bi_mod_inverse( biR, bim );
    else
	bi_free( bim );
    return biR;
    }


bigint
bi_mod_inverse( bigint bi, bigint bim )
    {
    bigint gcd, mul0, mul1;

    gcd = bi_egcd( bi_copy( bim ), bi, &mul0, &mul1 );

    /* Did we get gcd == 1? */
    if ( ! bi_is_one( gcd ) )
	{
	(void) fprintf( stderr, "bi_mod_inverse: not relatively prime\n" );
	(void) kill( getpid(), SIGFPE );
	}

    bi_free( mul0 );
    return bi_mod( mul1, bim );
    }


#ifdef notdef
/* Euclid's GCD algorithm. */
bigint
bi_gcd( bigint bix, bigint biy )
    {
    bigint bit;

    bix = bi_abs( bix );
    biy = bi_abs( biy );
    if ( bi_compare( bi_copy( bix ), bi_copy( biy ) ) < 0 )
	{ bit = bix; bix = biy; biy = bit; }
    while ( ! bi_is_zero( bi_copy( biy ) ) )
	{
	bit = bi_mod( bix, bi_copy( biy ) );
	bix = biy;
	biy = bit;
	}
    bi_free( biy );
    return bix;
    }
#endif


/* Binary GCD algorithm.  Faster than Euclid's by about a factor of two. */
bigint
bi_gcd( bigint bix, bigint biy )
    {
    bigint big, bit;

    bix = bi_abs( bix );
    biy = bi_abs( biy );
    if ( bi_compare( bi_copy( bix ), bi_copy( biy ) ) < 0 )
	{ bit = bix; bix = biy; biy = bit; }
    big = bi_1;
    while ( bi_is_even( bi_copy( bix ) ) && bi_is_even( bi_copy( biy ) ) )
	{
	bix = bi_half( bix );
	biy = bi_half( biy );
	big = bi_double( big );
	}
    while ( ! bi_is_zero( bi_copy( bix ) ) )
	{
	while ( bi_is_even( bi_copy( bix ) ) )
	    bix = bi_half( bix );
	while ( bi_is_even( bi_copy( biy ) ) )
	    biy = bi_half( biy );
	bit = bi_half(
	    bi_abs( bi_subtract( bi_copy( bix ), bi_copy( biy ) ) ) );
	if ( bi_compare( bi_copy( bix ), bi_copy( biy ) ) >= 0 )
	    { bi_free( bix ); bix = bit; }
	else
	    { bi_free( biy ); biy = bit; }
	}
    bi_free( bix );
    return bi_multiply( big, biy );
    }


/* Euclidean extended GCD algorithm. */
bigint
bi_egcd( bigint bix, bigint biy, bigint* bix_mul, bigint* biy_mul )
    {
    bigint a0, b0, c0, a1, b1, c1, q, t;

    if ( bi_is_negative( bi_copy( bix ) ) )
	{
	bigint biR = bi_egcd( bi_negate( bix ), biy, &t, biy_mul );
	*bix_mul = bi_negate( t );
	return biR;
	}
    if ( bi_is_negative( bi_copy( biy ) ) )
	{
	bigint biR = bi_egcd( bix, bi_negate( biy ), bix_mul, &t );
	*biy_mul = bi_negate( t );
	return biR;
	}
    if ( bi_compare( bi_copy( bix ), bi_copy( biy ) ) < 0 )
	return bi_egcd( biy, bix, biy_mul, bix_mul );

    a0 = bi_1;  b0 = bi_0;  c0 = bix;
    a1 = bi_0;  b1 = bi_1;  c1 = biy;

    while ( ! bi_is_zero( bi_copy( c1 ) ) )
	{
	q = bi_divide( bi_copy( c0 ), bi_copy( c1 ) );
	t = a0;
	a0 = bi_copy( a1 );
	a1 = bi_subtract( t, bi_multiply( bi_copy( q ), a1 ) );
	t = b0;
	b0 = bi_copy( b1 );
	b1 = bi_subtract( t, bi_multiply( bi_copy( q ), b1 ) );
	t = c0;
	c0 = bi_copy( c1 );
	c1 = bi_subtract( t, bi_multiply( bi_copy( q ), c1 ) );
	bi_free( q );
	}

    bi_free( a1 );
    bi_free( b1 );
    bi_free( c1 );
    *bix_mul = a0;
    *biy_mul = b0;
    return c0;
    }


#ifdef notdef
/* Binary extended GCD algorithm.  Slightly slower that Euclid's. */
bigint
bi_egcd( bigint bix, bigint biy, bigint* bix_mul, bigint* biy_mul )
    {
    bigint big, biu, biv, bia, bib, bic, bid;

    if ( bi_is_negative( bi_copy( bix ) ) )
	{
	bigint biR = bi_egcd( bi_negate( bix ), biy, bix_mul, biy_mul );
	*bix_mul = bi_negate( *bix_mul );
	return biR;
	}
    if ( bi_is_negative( bi_copy( biy ) ) )
	{
	bigint biR = bi_egcd( bix, bi_negate( biy ), bix_mul, biy_mul );
	*biy_mul = bi_negate( *biy_mul );
	return biR;
	}
    if ( bi_compare( bi_copy( bix ), bi_copy( biy ) ) < 0 )
	return bi_egcd( biy, bix, biy_mul, bix_mul );

    big = bi_1;
    while ( bi_is_even( bi_copy( bix ) ) && bi_is_even( bi_copy( biy ) ) )
	{
	bix = bi_half( bix );
	biy = bi_half( biy );
	big = bi_double( big );
	}

    biu = bi_copy( bix );
    biv = bi_copy( biy );
    bia = bi_1;
    bib = bi_0;
    bic = bi_0;
    bid = bi_1;

    while ( ! bi_is_zero( bi_copy( biu ) ) )
	{
	while ( bi_is_even( bi_copy( biu ) ) )
	    {
	    biu = bi_half( biu );
	    if ( bi_is_even( bi_copy( bia ) ) && bi_is_even( bi_copy( bib ) ) )
		{
		bia = bi_half( bia );
		bib = bi_half( bib );
		}
	    else
		{
		bia = bi_half( bi_add( bia, bi_copy( biy ) ) );
		bib = bi_half( bi_subtract( bib, bi_copy( bix ) ) );
		}
	    }
	while ( bi_is_even( bi_copy( biv ) ) )
	    {
	    biv = bi_half( biv );
	    if ( bi_is_even( bi_copy( bic ) ) && bi_is_even( bi_copy( bid ) ) )
		{
		bic = bi_half( bic );
		bid = bi_half( bid );
		}
	    else
		{
		bic = bi_half( bi_add( bic, bi_copy( biy ) ) );
		bid = bi_half( bi_subtract( bid, bi_copy( bix ) ) );
		}
	    }
	if ( bi_compare( bi_copy( biu ), bi_copy( biv ) ) >= 0 )
	    {
	    biu = bi_subtract( biu, bi_copy( biv ) );
	    bia = bi_subtract( bia, bi_copy( bic ) );
	    bib = bi_subtract( bib, bi_copy( bid ) );
	    }
	else
	    {
	    biv = bi_subtract( biv, bi_copy( biu ) );
	    bic = bi_subtract( bic, bi_copy( bia ) );
	    bid = bi_subtract( bid, bi_copy( bib ) );
	    }
	}

    bi_free( bix );
    bi_free( biy );
    bi_free( biu );
    bi_free( bia );
    bi_free( bib );
    *bix_mul = bic;
    *biy_mul = bid;
    return bi_multiply( big, biv );
    }
#endif


bigint
bi_lcm( bigint bia, bigint bib )
    {
    bigint biR;

    biR = bi_divide(
	bi_multiply( bi_copy( bia ), bi_copy( bib ) ),
	bi_gcd( bi_copy( bia ), bi_copy( bib ) ) );
    bi_free( bia );
    bi_free( bib );
    return biR;
    }


/* The Jacobi symbol. */
bigint
bi_jacobi( bigint bia, bigint bib )
    {
    if ( bi_is_even( bi_copy( bib ) ) )
	{
	(void) fprintf( stderr, "bi_jacobi: don't know how to compute Jacobi(n, even)\n" );
	(void) kill( getpid(), SIGFPE );
	}

    if ( bi_compare( bi_copy( bia ), bi_copy( bib ) ) >= 0 )
	return bi_jacobi( bi_mod( bia, bi_copy( bib ) ), bib );

    if ( bi_is_zero( bi_copy( bia ) ) || bi_is_one( bi_copy( bia ) ) )
	{
	bi_free( bib );
	return bia;
	}

    if ( bi_compare( bi_copy( bia ), bi_2 ) == 0 )
	{
	bi_free( bia );
	switch ( bi_int_and( bib, 7 ) )
	    {
	    case 1: case 7:
	    return bi_1;
	    case 3: case 5:
	    return bi_m1;
	    }
	}

    if ( bi_is_even( bi_copy( bia ) ) )
	{
	/* Peel off as many factors of 2 as we can. */
	int j2, c;

	j2 = bi_to_int( bi_jacobi( bi_2, bi_copy( bib ) ) );	/* 1 or -1 */
	bia = bi_half( bia );
	c = j2;
	while ( bi_is_even( bi_copy( bia ) ) )
	    {
	    bia = bi_half( bia );
	    c *= j2;
	    }
	if ( c == -1 )
	    return bi_negate( bi_jacobi( bia, bib ) );
	else
	    return bi_jacobi( bia, bib );
	}

    if ( bi_int_and( bi_copy( bia ), 3 ) == 3 &&
         bi_int_and( bi_copy( bib ), 3 ) == 3 )
	return bi_negate( bi_jacobi( bib, bia ) );
    else
	return bi_jacobi( bib, bia );
    }


/* Probabalistic prime checking. */
int
bi_is_probable_prime( bigint bi, int certainty )
    {
    int i, p;
    bigint bim1;
    bigint s;
    int r;

    /* First do trial division by a list of small primes.  This eliminates
    ** many candidates and runs very quickly.
    */
    for ( i = 0; i < sizeof(low_primes)/sizeof(*low_primes); ++i )
	{
	p = low_primes[i];
	switch ( bi_compare( int_to_bi( p ), bi_copy( bi ) ) )
	    {
	    case 0:
	    bi_free( bi );
	    return 1;
	    case 1:
	    bi_free( bi );
	    return 0;
	    }
	if ( bi_int_mod( bi_copy( bi ), p ) == 0 )
	    {
	    bi_free( bi );
	    return 0;
	    }
	}

    /* We need bi-1, and bi-1 with the powers of 2 removed. */
    bim1 = bi_int_subtract( bi_copy( bi ), 1 );
    s = bi_copy( bim1 );
    r = 0;
    while ( bi_is_even( bi_copy( s ) ) )
	{
	s = bi_half( s );
	++r;
	}

    /* Now do the probabilistic tests. */
    for ( i = 0; i < certainty; ++i )
	{
	if ( ! miller_rabin_test( bi_copy( bi ), bi_copy( bim1 ), bi_copy( s ), r ) )
	    {
	    bi_free( bi );
	    bi_free( bim1 );
	    bi_free( s );
	    return 0;
	    }
	}

    bi_free( bi );
    bi_free( bim1 );
    bi_free( s );
    return 1;
    }


int
bi_is_probable_prime_mr( bigint bi, int certainty )
    {
    int i;
    bigint bim1;
    bigint s;
    int r;

    /* We need bi-1, and bi-1 with the powers of 2 removed. */
    bim1 = bi_int_subtract( bi_copy( bi ), 1 );
    s = bi_copy( bim1 );
    r = 0;
    while ( bi_is_even( bi_copy( s ) ) )
	{
	s = bi_half( s );
	++r;
	}

    /* Now do the probabilistic tests. */
    for ( i = 0; i < certainty; ++i )
	{
	if ( ! miller_rabin_test( bi_copy( bi ), bi_copy( bim1 ), bi_copy( s ), r ) )
	    {
	    bi_free( bi );
	    bi_free( bim1 );
	    bi_free( s );
	    return 0;
	    }
	}

    bi_free( bi );
    bi_free( bim1 );
    bi_free( s );
    return 1;
    }


int
bi_is_probable_prime_ss( bigint bi, int certainty )
    {
    int i;
    bigint bim1, bim1o2;

    /* We need bi-1 and (bi-1)/2 for the tests. */
    bim1 = bi_int_subtract( bi_copy( bi ), 1 );
    bim1o2 = bi_half( bi_copy( bim1 ) );

    /* Now do the probabilistic tests. */
    for ( i = 0; i < certainty; ++i )
	{
	if ( ! solovay_strassen_test( bi_copy( bi ), bi_copy( bim1 ), bi_copy( bim1o2 ) ) )
	    {
	    bi_free( bi );
	    bi_free( bim1 );
	    bi_free( bim1o2 );
	    return 0;
	    }
	}

    bi_free( bi );
    bi_free( bim1 );
    bi_free( bim1o2 );
    return 1;
    }


int
bi_is_probable_prime_f( bigint bi, int certainty )
    {
    int i;
    bigint bim1;

    /* We need bi-1 for the tests. */
    bim1 = bi_int_subtract( bi_copy( bi ), 1 );

    /* Now do the probabilistic tests. */
    for ( i = 0; i < certainty; ++i )
	{
	if ( ! fermat_test( bi_copy( bi ), bi_copy( bim1 ) ) )
	    {
	    bi_free( bi );
	    bi_free( bim1 );
	    return 0;
	    }
	}

    bi_free( bi );
    bi_free( bim1 );
    return 1;
    }


int
bi_is_probable_prime_f2( bigint bi, int certainty )
    {
    int i;
    bigint bim1;

    /* We need bi-1 for the tests. */
    bim1 = bi_int_subtract( bi_copy( bi ), 1 );

    /* Now do the probabilistic tests. */
    for ( i = 0; i < certainty; ++i )
	{
	if ( ! fermat2_test( bi_copy( bi ), bi_copy( bim1 ) ) )
	    {
	    bi_free( bi );
	    bi_free( bim1 );
	    return 0;
	    }
	}

    bi_free( bi );
    bi_free( bim1 );
    return 1;
    }


int
bi_is_probable_prime_f3( bigint bi, int certainty )
    {
    int i;
    bigint bim1;

    /* We need bi-1 for the tests. */
    bim1 = bi_int_subtract( bi_copy( bi ), 1 );

    /* Now do the probabilistic tests. */
    for ( i = 0; i < certainty; ++i )
	{
	if ( ! fermat3_test( bi_copy( bi ), bi_copy( bim1 ) ) )
	    {
	    bi_free( bi );
	    bi_free( bim1 );
	    return 0;
	    }
	}

    bi_free( bi );
    bi_free( bim1 );
    return 1;
    }


int
bi_is_probable_prime_n( bigint bi, int certainty )
    {
    int i;
    bigint bim1;

    /* We need bi-1 for the tests. */
    bim1 = bi_int_subtract( bi_copy( bi ), 1 );

    /* Now do the probabilistic tests. */
    for ( i = 0; i < certainty; ++i )
	{
	if ( ! null_test( bi_copy( bi ), bi_copy( bim1 ) ) )
	    {
	    bi_free( bi );
	    bi_free( bim1 );
	    return 0;
	    }
	}

    bi_free( bi );
    bi_free( bim1 );
    return 1;
    }


int
bi_is_probable_prime_td( bigint bi )
    {
    int i, p;

    for ( i = 0; i < sizeof(low_primes)/sizeof(*low_primes); ++i )
	{
	p = low_primes[i];
	switch ( bi_compare( int_to_bi( p ), bi_copy( bi ) ) )
	    {
	    case 0:
	    bi_free( bi );
	    return 1;
	    case 1:
	    bi_free( bi );
	    return 0;
	    }
	if ( bi_int_mod( bi_copy( bi ), p ) == 0 )
	    {
	    bi_free( bi );
	    return 0;
	    }
	}

    bi_free( bi );
    return 1;
    }


static int
null_test( bigint bi, bigint bim1 )
    {
    bigint a;

    /* Pick random test number. */
    a = bi_int_add( bi_random( bi_copy( bim1 ) ), 1 );

    bi_free( a );
    bi_free( bim1 );
    bi_free( bi );

    return 1;
    }


#ifdef FERMAT
/* Fermat test.  Note that this is not state of the art.  There's a
** class of numbers called Carmichael numbers which are composite
** but look prime to this test - it lets them slip through no
** matter how many reps you run.
*/
static int
fermat_test( bigint bi, bigint bim1 )
    {
    bigint a;

    /* Pick random test number. */
    a = bi_int_add( bi_random( bi_copy( bim1 ) ), 1 );

    return bi_is_one( bi_mod_power( a, bim1, bi ) );
    }
#endif /* FERMAT */


#ifdef FERMAT2
/* Fermat test with base 2.  Runs slightly faster than the general Fermat
** test.  Has the same problem with Carmichael numbers.
*/
static int
fermat2_test( bigint bi, bigint bim1 )
    {
    return bi_is_one( bi_mod_power( bi_2, bim1, bi ) );
    }
#endif /* FERMAT2 */


#ifdef FERMAT3
/* Fermat test with base 3.  Runs slightly faster than the general Fermat
** test.  Has the same problem with Carmichael numbers.
*/
static int
fermat3_test( bigint bi, bigint bim1 )
    {
    return bi_is_one( bi_mod_power( bi_3, bim1, bi ) );
    }
#endif /* FERMAT3 */


#ifdef SOLOVAY_STRASSEN
/* Solovay-Strassen test.  This doesn't have exceptions like the Fermat
** test, but it runs slower.
*/
static int
solovay_strassen_test( bigint bi, bigint bim1, bigint bim1o2 )
    {
    bigint a, j, jac;

    /* Pick random test number. */
    a = bi_int_add( bi_random( bi_copy( bim1 ) ), 1 );

#ifdef notdef
    /* GCD test.  This rarely hits, but we need it. */
    if ( ! bi_is_one( bi_gcd( bi_copy( bi ), bi_copy( a ) ) ) )
	{
	bi_free( bi );
	bi_free( bim1 );
	bi_free( bim1o2 );
	bi_free( a );
	return 0;
	}
#endif

    /* Compute the pseudo Jacobi. */
    j = bi_mod_power( bi_copy( a ), bim1o2, bi_copy( bi ) );
    if ( bi_compare( bi_copy( j ), bim1 ) == 0 )
	{
	bi_free( j );
	j = bi_m1;
	}

    /* Now compute the real Jacobi. */
    jac = bi_jacobi( a, bi );

    /* If they're not equal, the number is definitely composite. */
    return ( bi_compare( j, jac ) == 0 );
    }
#endif /* SOLOVAY_STRASSEN */


#ifdef MILLER_RABIN
/* Miller-Rabin test.  This also doesn't have exceptions like the Fermat
** test, yet it still runs almost as fast.
*/
static int
miller_rabin_test( bigint bi, bigint bim1, bigint s, int r )
    {
    bigint a, p;
    int j;

    /* Pick random test number. */
    a = bi_int_add( bi_random( bi_copy( bim1 ) ), 1 );

    p = bi_mod_power( a, s, bi_copy( bi ) );
    if ( bi_is_one( bi_copy( p ) ) )
	{
	bi_free( p );
	bi_free( bi );
	bi_free( bim1 );
	return 1;
	}
    if ( bi_compare( bi_copy( p ), bi_copy( bim1 ) ) == 0 )
	{
	bi_free( p );
	bi_free( bi );
	bi_free( bim1 );
	return 1;
	}

    for ( j = 1; j < r; ++j )
	{
	p = bi_mod_square( p, bi_copy( bi ) );
	if ( bi_compare( bi_copy( p ), bi_copy( bim1 ) ) == 0 )
	    {
	    bi_free( p );
	    bi_free( bi );
	    bi_free( bim1 );
	    return 1;
	    }
	}

    bi_free( p );
    bi_free( bi );
    bi_free( bim1 );
    return 0;
    }
#endif /* MILLER_RABIN */


bigint
bi_generate_prime( int bits, int certainty )
    {
    bigint mo2, mo4, p;

    mo4 = bi_power( bi_2, int_to_bi( bits - 2 ) );
    mo2 = bi_double( bi_copy( mo4 ) );
    for (;;)
	{
	/* Pick a random prime candidate.  It order for it to
	** have the requested number of bits, it should be in:
	**     2^(bits-1)  <=  p  <  2^bits
	** However, most likely we are being called to generate the two
	** parts of an RSA key, which will be multiplied together.
	** The product is supposed to be 2*bits long, but for some
	** primes in the above range it will be one or two bits shorter.
	** To prevent this, we generate primes only in the upper half of
	** the range:
	**     2^(bits-1) + 2^(bits-2)  <=  p  <  2^bits
	** Another way of putting this is that the top two bits are on,
	** instead of just the top bit.  This is actually a slightly
	** narrower range than we really need, but there are still plenty
	** of primes up there.
	*/
	p = bi_add( bi_add(
		bi_random( bi_copy( mo4 ) ), bi_copy( mo2 ) ), bi_copy( mo4 ) );

	/* Check it. */
	if ( bi_is_probable_prime( bi_copy( p ), certainty ) )
	    {
	    /* Yes. */
	    bi_free( mo2 );
	    bi_free( mo4 );
	    return p;
	    }

	/* No.  Try again. */
	bi_free( p );
	}
    }
