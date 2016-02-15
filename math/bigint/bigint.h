/* bigint.h - include file for bigint package
**
** This library lets you do math on arbitrarily large integers.  It's
** pretty fast - compared with the multi-precision routines in the "bc"
** calculator program, these routines are between two and twelve times faster,
** except for division which is maybe half as fast.
**
** The calling convention is a little unusual.  There's a basic problem
** with writing a math library in a language that doesn't do automatic
** garbage collection - what do you do about intermediate results?
** You'd like to be able to write code like this:
**
**     d = bi_sqrt( bi_add( bi_multiply( x, x ), bi_multiply( y, y ) ) );
**
** That works fine when the numbers being passed back and forth are
** actual values - ints, floats, or even fixed-size structs.  However,
** when the numbers can be any size, as in this package, then you have
** to pass them around as pointers to dynamically-allocated objects.
** Those objects have to get de-allocated after you are done with them.
** But how do you de-allocate the intermediate results in a complicated
** multiple-call expression like the above?
**
** There are two common solutions to this problem.  One, switch all your
** code to a language that provides automatic garbage collection, for
** example Java.  This is a fine idea and I recommend you do it wherever
** it's feasible.  Two, change your routines to use a calling convention
** that prevents people from writing multiple-call expressions like that.
** The resulting code will be somewhat clumsy-looking, but it will work
** just fine.
**
** This package uses a third method, which I haven't seen used anywhere
** before.  It's simple: each number can be used precisely once, after
** which it is automatically de-allocated.  This handles the anonymous
** intermediate values perfectly.  Named values still need to be copied
** and freed explicitly.  Here's the above example using this convention:
**
**     d = bi_sqrt( bi_add(
**             bi_multiply( bi_copy( x ), bi_copy( x ) ),
**             bi_multiply( bi_copy( y ), bi_copy( y ) ) ) );
**     bi_free( x );
**     bi_free( y );
**
** Or, since the package contains a square routine, you could just write:
**
**     d = bi_sqrt( bi_add( bi_square( x ), bi_square( y ) ) );
**
** This time the named values are only being used once, so you don't
** have to copy and free them.
**
** This really works, however you do have to be very careful when writing
** your code.  If you leave out a bi_copy() and use a value more than once,
** you'll get a runtime error about "zero refs" and a SIGFPE.  Run your
** code in a debugger, get a backtrace to see where the call was, and then
** eyeball the code there to see where you need to add the bi_copy().
**
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


/* Type definition for bigints - it's an opaque type, the real definition
** is in bigint.c.
*/
typedef void* bigint;


/* Some convenient pre-initialized numbers.  These are all permanent,
** so you can use them as many times as you want without calling bi_copy().
*/
extern bigint bi_0, bi_1, bi_2, bi_3, bi_m1, bi_maxint, bi_minint;


/* Initialize the bigint package.  You must call this when your program
** starts up.
*/
void bi_initialize( void );

/* Shut down the bigint package.  You should call this when your program
** exits.  It's not actually required, but it does do some consistency
** checks which help keep your program bug-free, so you really ought
** to call it.
*/
void bi_terminate( void );

/* Run in unsafe mode, skipping most runtime checks.  Slightly faster.
** Once your code is debugged you can add this call after bi_initialize().
*/
void bi_no_check( void );

/* Make a copy of a bigint.  You must call this if you want to use a
** bigint more than once.  (Or you can make the bigint permanent.)
** Note that this routine is very cheap - all it actually does is
** increment a reference counter.
*/
bigint bi_copy( bigint bi );

/* Make a bigint permanent, so it doesn't get automatically freed when
** used as an operand.
*/
void bi_permanent( bigint bi );

/* Undo bi_permanent().  The next use will free the bigint. */
void bi_depermanent( bigint bi );

/* Explicitly free a bigint.  Normally bigints get freed automatically
** when they are used as an operand.  This routine lets you free one
** without using it.  If the bigint is permanent, this doesn't do
** anything, you have to depermanent it first.
*/
void bi_free( bigint bi );

/* Compare two bigints.  Returns -1, 0, or 1. */
int bi_compare( bigint bia, bigint bib );

/* Convert an int to a bigint. */
bigint int_to_bi( int i );

/* Convert a bigint to an int.  SIGFPE on overflow. */
int bi_to_int( bigint bi );


/* I/O operations. */

/* Convert a string to a bigint. */
bigint str_to_bi( char* str );
bigint str_to_bi_base( char* str, int base );

/* Read a bigint from a file. */
bigint bi_scan( FILE* f );
bigint bi_scan_base( FILE* f, int base );

/* Write a bigint to a file. */
void bi_print( FILE* f, bigint bi );
void bi_print_base( FILE* f, bigint bi, int base );


/* Operations on a bigint and a regular int. */

/* Add an int to a bigint. */
bigint bi_int_add( bigint bi, int i );

/* Subtract an int from a bigint. */
bigint bi_int_subtract( bigint bi, int i );

/* Multiply a bigint by an int. */
bigint bi_int_multiply( bigint bi, int i );

/* Divide a bigint by an int.  SIGFPE on divide-by-zero. */
bigint bi_int_divide( bigint binumer, int denom );

/* Take the remainder of a bigint by an int, with an int result.
** SIGFPE if m is zero.
*/
int bi_int_rem( bigint bi, int m );

/* Take the modulus of a bigint by an int, with an int result.
** Note that mod is not rem: mod is always within [0..m), while
** rem can be negative.  SIGFPE if m is zero or negative.
*/
int bi_int_mod( bigint bi, int m );


/* Basic operations on two bigints. */

/* Add two bigints. */
bigint bi_add( bigint bia, bigint bib );

/* Subtract bib from bia. */
bigint bi_subtract( bigint bia, bigint bib );

/* Multiply two bigints. */
bigint bi_multiply( bigint bia, bigint bib );

/* Divide one bigint by another.  SIGFPE on divide-by-zero. */
bigint bi_divide( bigint binumer, bigint bidenom );

/* Binary division of one bigint by another.  SIGFPE on divide-by-zero.
** This is here just for testing.  It's about five times slower than
** regular division.
*/
bigint bi_binary_divide( bigint binumer, bigint bidenom );

/* Take the remainder of one bigint by another.  SIGFPE if bim is zero. */
bigint bi_rem( bigint bia, bigint bim );

/* Take the modulus of one bigint by another.  Note that mod is not rem:
** mod is always within [0..bim), while rem can be negative.  SIGFPE if
** bim is zero or negative.
*/
bigint bi_mod( bigint bia, bigint bim );


/* Some less common operations. */

/* Negate a bigint. */
bigint bi_negate( bigint bi );

/* Absolute value of a bigint. */
bigint bi_abs( bigint bi );

/* Divide a bigint in half. */
bigint bi_half( bigint bi );

/* Multiply a bigint by two. */
bigint bi_double( bigint bi );

/* Square a bigint. */
bigint bi_square( bigint bi );

/* Raise bi to the power of biexp.  SIGFPE if biexp is negative. */
bigint bi_power( bigint bi, bigint biexp );

/* Integer square root. */
bigint bi_sqrt( bigint bi );

/* Factorial. */
bigint bi_factorial( bigint bi );


/* Some predicates. */

/* 1 if the bigint is odd, 0 if it's even. */
int bi_is_odd( bigint bi );

/* 1 if the bigint is even, 0 if it's odd. */
int bi_is_even( bigint bi );

/* 1 if the bigint equals zero, 0 if it's nonzero. */
int bi_is_zero( bigint bi );

/* 1 if the bigint equals one, 0 otherwise. */
int bi_is_one( bigint bi );

/* 1 if the bigint is less than zero, 0 if it's zero or greater. */
int bi_is_negative( bigint bi );


/* Now we get into the esoteric number-theory stuff used for cryptography. */

/* Modular addition.  May be faster than bi_mod(bi_add(bia,bib),bim). */
bigint bi_mod_add( bigint bia, bigint bib, bigint bim );

/* Modular subtraction.  May be faster than bi_mod(bi_subtract(bia,bib),bim). */
bigint bi_mod_subtract( bigint bia, bigint bib, bigint bim );

/* Modular multiplication.  Faster than bi_mod(bi_multiply(bia,bib),bim). */
bigint bi_mod_multiply( bigint bia, bigint bib, bigint bim );

/* Modular square.  Faster than bi_mod(bi_square(bi),bim). */
bigint bi_mod_square( bigint bi, bigint bim );

/* Modular exponentiation.  Much faster than bi_mod(bi_power(bi,biexp),bim).
** Also, biexp can be negative.
*/
bigint bi_mod_power( bigint bi, bigint biexp, bigint bim );

/* Modular inverse.  mod( bi * modinv(bi), bim ) == 1.  SIGFPE if bi is not
** relatively prime to bim.
*/
bigint bi_mod_inverse( bigint bi, bigint bim );

/* Produce a random number in the half-open interval [0..bi).  You need
** to have called srandom() before using this.
*/
bigint bi_random( bigint bi );

/* Greatest common divisor of two bigints. */
bigint bi_gcd( bigint bix, bigint biy );

/* Greatest common divisor of two bigints, plus the corresponding multipliers.*/
bigint bi_egcd( bigint bix, bigint biy, bigint* bim_mul, bigint* bin_mul );

/* Least common multiple of two bigints. */
bigint bi_lcm( bigint bia, bigint bib );

/* The Jacobi symbol.  SIGFPE if bib is even. */
bigint bi_jacobi( bigint bia, bigint bib );

/* Probabalistic prime checking.  A zero return means bi is definitely not
** prime.  A non-zero return means bi is probably prime, with an error
** probability of less than 1/4 ^ certainty.
**
** Note: in fact the error probability is much lower, that's just a
** theoretical bound.  Using a certainty of 1 gives a theoretical error
** probability of 0.25, but in fact a large number that passes such a test
** is almost certainly prime.  Using certainty more than, say, 5 is huge
** overkill.  See "Finding Four Million Large Random Primes", Ronald Rivest,
** Advances in Cryptology: Proceedings of Crypto '91 - he estimates the
** chance of a 512-bit number falsely passing a single test at 10^-44.
*/
int bi_is_probable_prime( bigint bi, int certainty );

int bi_is_probable_prime_mr( bigint bi, int certainty );
int bi_is_probable_prime_ss( bigint bi, int certainty );
int bi_is_probable_prime_f( bigint bi, int certainty );
int bi_is_probable_prime_f2( bigint bi, int certainty );
int bi_is_probable_prime_f3( bigint bi, int certainty );
int bi_is_probable_prime_n( bigint bi, int certainty );
int bi_is_probable_prime_td( bigint bi );

/* Random probabilistic prime with the specified number of bits.  The
** certainty has the same meaning as in bi_is_probable_prime().
*/
bigint bi_generate_prime( int bits, int certainty );

/* Number of bits in |bi|.  The integer log base 2. */
int bi_bits( bigint bi );

/* Number of digits in |bi|, approximately.  The integer log base 10. */
int bi_digits( bigint bi );

/* The nth bit in |bi|.  Bit zero is the low-order bit. */
int bi_bit( bigint bi, int bitnum );

/* Does a bitwise 'and' operation on |bi|.  Since the mask is an int, so
** is the return value.
*/
int bi_int_and( bigint bi, int mask );
