/*
Copyright (c) 2009 Mats Rauhala <mats.rauhala@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#ifndef LMACROS_H
#define LMACROS_H

# define LMAX (lifelines-1) /* Maximum lines */
# define CMAX (lifecols-1) /* Maximum columns */

# define SLEEP 200000000 /* Fifth of a second */

# define ALIVE(y, x) (CELL((y), (x))==1)?1:0 /* Check for whether a cell is
                                                alive */

# define COORD(y, x) ((y)*lifecols+(x)) /* Convert two-dimensional data to
                                           one-dimensional data. y*xmax+x */

# define ICOLS 30 /* Info-window width */
# define ILINES LINES /* Info-window height */

# define ASIZE (lifecols*lifelines) /* Amount of cells */
# define SIZE (ASIZE*sizeof(int)) /* And the real size of it */

# define CPR(y, x) (cells[COORD((y),(x))] = 1) /* Give CPR to the defined
                                                  coordinate. (Make it alive)
                                                  */
# define BCPR(y, x) (buffer[COORD((y),(x))] = 1) /* The same but for buffer */
# define ABCPR(y, x) (buffer[COORD((y),(x))] = 0) /* Kill the cell. Anticpr */

# define COPYB memcpy(cells, buffer, SIZE) /* Sync the cells with the data from
                                              the buffer */
# define COPYC memcpy(buffer, cells, SIZE) /* Sync the buffer with the data
                                              from the cells */

# define CELL(y, x) (cells[ COORD( (y), (x) ) ] ) /* Data from the cell. */
/* Debug macro */
# define debugc(fmt, x, s) fprintf(stdout, "%s:%d %s=" fmt ": %s\n", __FILE__, __LINE__, #x, (x), (s))
/* Helper macro for debugging */
# define debug(fmt, x) debugc(fmt, x, "")

/* Curses defines TRUE and FALSE, but vim colours true and false (lowercase) */
#define true 1
#define false 0

#endif 
