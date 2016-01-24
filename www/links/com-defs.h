#ifndef COM_DEFS_H
#define COM_DEFS_H

#ifndef __EXTENSIONS__
#define __EXTENSIONS__
#endif

#ifndef _LARGEFILE_SOURCE
#define _LARGEFILE_SOURCE	1
#endif

#ifndef _LARGEFILE
#define _LARGEFILE		1
#endif

#ifndef _ALL_SOURCE
#define _ALL_SOURCE		1
#endif

#ifndef _GNU_SOURCE
#define _GNU_SOURCE		1
#endif

#ifndef _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS	64
#endif

#if defined(vax) && !defined(__vax)
#define __vax	vax
#endif

#ifdef __DJGPP
unsigned long __ntohl(unsigned long);
unsigned short __ntohs(unsigned short);
#endif

#endif
