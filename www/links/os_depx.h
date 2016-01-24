/* os_depx.h
 * (c) 2002 Mikulas Patocka
 * This file is a part of the Links program, released under GPL.
 */

#ifdef HAVE_VALUES_H
#include <values.h>
#endif

#ifndef MAXINT
#ifdef INT_MAX
#define MAXINT INT_MAX
#else
#define MAXINT ((int)((unsigned int)-1 >> 1))
#endif
#endif

#ifndef MAXLONG
#ifdef LONG_MAX
#define MAXLONG LONG_MAX
#else
#define MAXLONG ((long)((unsigned long)-1L >> 1))
#endif
#endif

#if defined(HAVE_STRTOLL) && !defined(MAXLLONG)
#ifdef LLONG_MAX
#define MAXLLONG LLONG_MAX
#else
#define MAXLLONG ((long long)((unsigned long long)-1L >> 1))
#endif
#endif

#if !defined(HAVE_ERRNO) && !defined(errno)
extern int errno;
#endif

#ifndef SEEK_SET
#ifdef L_SET
#define SEEK_SET	L_SET
#else
#define SEEK_SET	0
#endif
#endif
#ifndef SEEK_CUR
#ifdef L_INCR
#define SEEK_CUR	L_INCR
#else
#define SEEK_CUR	1
#endif
#endif
#ifndef SEEK_END
#ifdef L_XTND
#define SEEK_END	L_XTND
#else
#define SEEK_END	2
#endif
#endif

#ifndef S_ISUID
#define S_ISUID		04000
#endif
#ifndef S_ISGID
#define S_ISGID		02000
#endif
#ifndef S_ISVTX
#define S_ISVTX		01000
#endif
#ifndef S_IRUSR
#define S_IRUSR		00400
#endif
#ifndef S_IWUSR
#define S_IWUSR		00200
#endif
#ifndef S_IXUSR
#define S_IXUSR		00100
#endif
#ifndef S_IRGRP
#define S_IRGRP		00040
#endif
#ifndef S_IWGRP
#define S_IWGRP		00020
#endif
#ifndef S_IXGRP
#define S_IXGRP		00010
#endif
#ifndef S_IROTH
#define S_IROTH		00004
#endif
#ifndef S_IWOTH
#define S_IWOTH		00002
#endif
#ifndef S_IXOTH
#define S_IXOTH		00001
#endif

#if !defined(S_IFMT) && defined(_S_IFMT)
#define S_IFMT		_S_IFMT
#endif

#if !defined(S_ISREG) && defined(S_IFMT) && defined(S_IFREG)
#define S_ISREG(mode)	(((mode) & S_IFMT) == S_IFREG)
#endif

#if !defined(S_ISDIR) && defined(S_IFMT) && defined(S_IFDIR)
#define S_ISDIR(mode)	(((mode) & S_IFMT) == S_IFDIR)
#endif

#if !defined(S_ISBLK) && defined(S_IFMT) && defined(S_IFBLK)
#define S_ISBLK(mode)	(((mode) & S_IFMT) == S_IFBLK)
#endif

#if !defined(S_ISCHR) && defined(S_IFMT) && defined(S_IFCHR)
#define S_ISCHR(mode)	(((mode) & S_IFMT) == S_IFCHR)
#endif

#if !defined(S_ISLNK) && defined(S_IFMT) && defined(S_IFLNK)
#define S_ISLNK(mode)	(((mode) & S_IFMT) == S_IFLNK)
#endif

#if !defined(S_ISSOCK) && defined(S_IFMT) && defined(S_IFSOCK)
#define S_ISSOCK(mode)	(((mode) & S_IFMT) == S_IFSOCK)
#endif

#ifndef O_NOCTTY
#define O_NOCTTY	0
#endif

#ifndef SIG_ERR
#define SIG_ERR		((int (*)())-1)
#endif

#if defined(OPENVMS) || defined(DOS)
#define NO_SIGNAL_HANDLERS
#endif

#if !defined(HAVE_STRTOIMAX) && defined(strtoimax) && defined(HAVE___STRTOLL)
#define HAVE_STRTOIMAX	1
#endif

#ifndef SA_RESTART
#define SA_RESTART	0
#endif

#ifdef OS2
int bounced_read(int fd, void *buf, size_t size);
int bounced_write(int fd, const void *buf, size_t size);
#define read bounced_read
#define write bounced_write
#endif

#ifdef __EMX__
#ifndef HAVE_GETCWD
#define HAVE_GETCWD	1
#endif
#ifndef HAVE_STRCASECMP
#define HAVE_STRCASECMP	1
#endif
#ifndef HAVE_STRNCASECMP
#define HAVE_STRNCASECMP 1
#endif
#define strcasecmp stricmp
#define strncasecmp strnicmp
#define getcwd _getcwd2
#define chdir _chdir2
#endif

#ifdef BEOS
#define socket be_socket
#define connect be_connect
#define getpeername be_getpeername
#define getsockname be_getsockname
#define listen be_listen
#define accept be_accept
#define bind be_bind
#define pipe be_pipe
#define read be_read
#define write be_write
#define close be_close
#define select be_select
#define getsockopt be_getsockopt
#ifndef SO_ERROR
#define SO_ERROR	10001
#endif
#endif

#ifdef OPENVMS
#if defined(__INITIAL_POINTER_SIZE)
#if __INITIAL_POINTER_SIZE == 64
#define OPENVMS_64BIT
#endif
#endif
#ifndef HAVE_GETCWD
#define HAVE_GETCWD	1
#endif
#define getcwd(x, y)	getcwd(x, y, 0)
#define sleep(x)	portable_sleep((x) * 1000)	/* sleep is buggy */
#define OS_SETRAW
int vms_read(int fd, void *buf, size_t size);
int vms_write(int fd, const void *buf, size_t size);
#define read vms_read
#define write vms_write
int vms_close(int fd);
int vms_select(int n, fd_set *rs, fd_set *ws, fd_set *es, struct timeval *t);
#define close vms_close
#define select vms_select
#endif

#ifdef DOS
#define DOS_EXTRA_KEYBOARD
#ifdef DOS_EXTRA_KEYBOARD
#define OS_SETRAW
#endif
int dos_read(int fd, void *buf, size_t size);
int dos_write(int fd, const void *buf, size_t size);
int dos_close(int fd);
int dos_select(int n, fd_set *rs, fd_set *ws, fd_set *es, struct timeval *t, int from_main_loop);
#define read dos_read
#define write dos_write
#define close dos_close
#define select(a, b, c, d, e) dos_select(a, b, c, d, e, 0)
#endif

#if defined(O_SIZE) && defined(__EMX__)
#define HAVE_OPEN_PREALLOC
#elif (defined(HAVE_FALLOCATE) || defined(HAVE_POSIX_FALLOCATE)) && !defined(OPENVMS)
#define HAVE_OPEN_PREALLOC
#endif

#if defined(__WATCOMC__) && defined(_WCRTLINK)
#define LIBC_CALLBACK	_WCRTLINK
#else
#define LIBC_CALLBACK
#endif

#if defined(__WATCOMC__) && defined(__LINUX__)
#define SIGNAL_HANDLER	__declspec(__cdecl)
#else
#define SIGNAL_HANDLER
#endif

#if defined(HAVE_UTIME) && defined(HAVE_UTIMES) && defined(INTERIX)
#undef HAVE_UTIMES
#endif

#if defined(HAVE_HERROR) && defined(__GNUC__) && defined(__hpux)
#undef HAVE_HERROR
#endif

#ifdef HAVE_MAXINT_CONVERSION_BUG
#undef MAXINT
#define MAXINT 0x7FFFF000
#endif

#ifndef HAVE_SOCKLEN_T
#ifdef OPENVMS
#define socklen_t unsigned
#else
#define socklen_t int
#endif
#endif

#ifndef PF_INET
#define PF_INET AF_INET
#endif
#ifndef PF_UNIX
#define PF_UNIX AF_UNIX
#endif

#if defined(__hpux)
#include "hpux.h"
#define accept hp_accept
#define getpeername hp_getpeername
#define getsockname hp_getsockname
#define getsockopt hp_getsockopt
#endif

#if defined(OPENVMS_64BIT)
#define my_intptr_t long long
#define my_uintptr_t unsigned long long
#else
#define my_intptr_t long
#define my_uintptr_t unsigned long
#endif

#if defined(__GNUC__)
#if __GNUC__ >= 2
#define PRINTF_FORMAT(a, b)	__attribute__((__format__(__printf__, a, b)))
#endif
#endif
#ifndef PRINTF_FORMAT
#define PRINTF_FORMAT(a, b)
#endif

#if defined(__GNUC__)
#if __GNUC__ >= 2
#define ATTR_PACKED		__attribute__((__packed__))
#define HAVE_ATTR_PACKED
#endif
#endif
#ifndef ATTR_PACKED
#define ATTR_PACKED
#endif

#if defined(HAVE_GETADDRINFO) && defined(HAVE_FREEADDRINFO)
#define USE_GETADDRINFO
#endif

#ifdef SUPPORT_IPV6
#ifndef PF_INET6
#define PF_INET6 AF_INET6
#endif
#endif

#if !defined(INET6_ADDRSTRLEN)
#define INET6_ADDRSTRLEN	46
#endif

#ifdef G
#ifndef HAVE_POWF
#define float_double double
#define fd_pow pow
#else
#define float_double float
#define fd_pow powf
#endif
#endif

#if defined(BEOS) || (defined(HAVE_BEGINTHREAD) && defined(OS2)) || defined(HAVE_PTHREADS) || (defined(HAVE_ATHEOS_THREADS_H) && defined(HAVE_SPAWN_THREAD) && defined(HAVE_RESUME_THREAD))
#define EXEC_IN_THREADS
#endif

#if !defined(EXEC_IN_THREADS) || !defined(HAVE_GETHOSTBYNAME) || defined(USE_GETADDRINFO)
#define THREAD_SAFE_LOOKUP
#endif

#if defined(DOS)
#define loop_select(a, b, c, d, e) dos_select(a, b, c, d, e, 1)
#elif defined(GRDRV_SVGALIB)
#define loop_select vga_select
int vga_select(int n, fd_set *r, fd_set *w, fd_set *e, struct timeval *t);
#elif defined(GRDRV_ATHEOS)
#define loop_select ath_select
int ath_select(int n, fd_set *r, fd_set *w, fd_set *e, struct timeval *t);
#else
#define loop_select select
#endif

