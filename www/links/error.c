/* error.c
 * (c) 2002 Mikulas Patocka
 * This file is a part of the Links program, released under GPL.
 */

#include "links.h"

#if DEBUGLEVEL >= 2
#define RED_ZONE	'R'
#endif
#if DEBUGLEVEL >= 3
#define FREE_FILL	0xfe
#define REALLOC_FILL	0xfd
#define ALLOC_FILL	0xfc
#endif

#if DEBUGLEVEL < 0
#define FREE_FILL	0xfe
#endif

#if DEBUGLEVEL < 0
#define NO_IE
#endif

#ifdef RED_ZONE
#define RED_ZONE_INC	1
#else
#define RED_ZONE_INC	0
#endif

volatile char dummy_val;
volatile char * volatile dummy_ptr = &dummy_val;
volatile char * volatile x_ptr;

void *do_not_optimize_here(void *p)
{
	/* break ANSI aliasing */
	x_ptr = p;
	*dummy_ptr = 0;
	return p;
}


#if defined(USE_WIN32_HEAP)

#include <windows.h>

/*#define NEW_HEAP*/

static HANDLE heap;

#define heap_malloc(s)		HeapAlloc(heap, 0, (s))
#define heap_calloc(s)		HeapAlloc(heap, HEAP_ZERO_MEMORY, (s))
#define heap_free(p)		HeapFree(heap, 0, (p))
#define heap_realloc(p, s)	HeapReAlloc(heap, 0, (p), (s))

void init_heap(void)
{
#ifndef NEW_HEAP
	heap = GetProcessHeap();
#else
	if (!(heap = HeapCreate(0, 0, 0)))
		fatal_exit("HeapCreate failed: %x", (unsigned)GetLastError());
#endif
	{
		ULONG heap_frag = 2;
		if (!HeapSetInformation(heap, HeapCompatibilityInformation, &heap_frag, sizeof(heap_frag))) {
			/*fatal_exit("HeapSetInformation failed: %x", (unsigned)GetLastError());*/
		}
	}
}

static void exit_heap(void)
{
#if DEBUGLEVEL >= 1
	if (!HeapValidate(heap, 0, NULL))
		internal("HeapValidate failed: %x", (unsigned)GetLastError());
#endif
#ifdef NEW_HEAP
	if (!HeapDestroy(heap))
		internal("HeapDestroy failed: %x", (unsigned)GetLastError());
#endif
}

#else

#define heap_malloc	malloc
#define heap_realloc	realloc
#define heap_free	free
#ifdef HAVE_CALLOC
#define heap_calloc(x) calloc(1, (x))
#else
static inline void *heap_calloc(size_t x)
{
	void *p;
	if ((p = heap_malloc(x))) memset(p, 0, x);
	return p;
}
#endif
void init_heap(void)
{
}
#define exit_heap()	do { } while (0)

#endif


#ifdef LEAK_DEBUG

my_uintptr_t mem_amount = 0;
my_uintptr_t mem_blocks = 0;

#define ALLOC_MAGIC		0xa110c
#define ALLOC_FREE_MAGIC	0xf4ee
#define ALLOC_REALLOC_MAGIC	0x4ea110c

#ifndef LEAK_DEBUG_LIST
struct alloc_header {
	int magic;
	size_t size;
};
#else
struct alloc_header {
	struct alloc_header *next;
	struct alloc_header *prev;
	size_t size;
	unsigned char *file;
	unsigned char *comment;
	int line;
	int magic;
};
static struct list_head memory_list = { &memory_list, &memory_list };
#endif

#define L_D_S ((sizeof(struct alloc_header) + 15) & ~15)

unsigned alloc_overhead = L_D_S + RED_ZONE_INC;

#endif

static inline void force_dump(void)
{
#if defined(DOS)
	fprintf(stderr, "\n"ANSI_SET_BOLD"Exiting"ANSI_CLEAR_BOLD"\n");
	fflush(stdout);
	fflush(stderr);
	exit(RET_INTERNAL);
#else
	int rs;
	fprintf(stderr, "\n"ANSI_SET_BOLD"Forcing core dump"ANSI_CLEAR_BOLD"\n");
	fflush(stdout);
	fflush(stderr);
	EINTRLOOP(rs, raise(SIGSEGV));
#endif
}

void check_memory_leaks(void)
{
#if defined(LEAK_DEBUG) && !defined(NO_IE)
	if (mem_amount || mem_blocks) {
		fatal_tty_exit();
		fprintf(stderr, "\n"ANSI_SET_BOLD"Memory leak by %lu bytes (%lu blocks)"ANSI_CLEAR_BOLD"\n", (unsigned long)mem_amount, (unsigned long)mem_blocks);
#ifdef LEAK_DEBUG_LIST
		fprintf(stderr, "\nList of blocks: ");
		{
			int r = 0;
			struct alloc_header *ah;
			foreach (ah, memory_list) {
				fprintf(stderr, "%s%p:%lu @ %s:%d", r ? ", ": "", (unsigned char *)ah + L_D_S, (unsigned long)ah->size, ah->file, ah->line), r = 1;
				if (ah->comment) fprintf(stderr, ":\"%s\"", ah->comment);
			}
			fprintf(stderr, "\n");
		}
#endif
		force_dump();
	}
#endif
	exit_heap();
}

static void er(int b, char *m, va_list l)
{
#ifdef HAVE_VPRINTF
	vfprintf(stderr, cast_const_char m, l);
#else
	fprintf(stderr, "%s", m);
#endif
	if (b) fprintf(stderr, ANSI_BELL);
	fprintf(stderr, "\n");
	fflush(stderr);
	sleep(1);
}

void error(char *m, ...)
{
	va_list l;
	va_start(l, m);
	fprintf(stderr, "\n");
	er(1, m, l);
	va_end(l);
}

void fatal_exit(char *m, ...)
{
	va_list l;
	fatal_tty_exit();
	va_start(l, m);
	fprintf(stderr, "\n");
	er(1, m, l);
	va_end(l);
	fflush(stdout);
	fflush(stderr);
	exit(RET_FATAL);
}

int errline;
unsigned char *errfile;

static unsigned char errbuf[4096];

void int_error(char *m, ...)
{
#ifdef NO_IE
	return;
#else
	va_list l;
	fatal_tty_exit();
	va_start(l, m);
	sprintf(cast_char errbuf, "\n"ANSI_SET_BOLD"INTERNAL ERROR"ANSI_CLEAR_BOLD" at %s:%d: ", errfile, errline);
	strcat(cast_char errbuf, cast_const_char m);
	er(1, cast_char errbuf, l);
	force_dump();
	va_end(l);
#endif
}

void debug_msg(char *m, ...)
{
	va_list l;
	va_start(l, m);
	sprintf(cast_char errbuf, "\nDEBUG MESSAGE at %s:%d: ", errfile, errline);
	strcat(cast_char errbuf, cast_const_char m);
	er(0, cast_char errbuf, l);
	va_end(l);
}

#ifdef LEAK_DEBUG

void *debug_mem_alloc(unsigned char *file, int line, size_t size, int mayfail)
{
	void *p;
#ifdef LEAK_DEBUG
	struct alloc_header *ah;
#endif
	dos_poll_break();
	debug_test_free(file, line);
	if (!size) return DUMMY;
	if (size > MAXINT) {
		if (mayfail) return NULL;
		overalloc_at(file, line);
	}
	size += L_D_S;
	retry:
#ifdef LEAK_DEBUG
	mem_amount += size - L_D_S;
	mem_blocks++;
#endif
	if (!(p = heap_malloc(size + RED_ZONE_INC))) {
#ifdef LEAK_DEBUG
		mem_amount -= size - L_D_S;
		mem_blocks--;
#endif
		if (out_of_memory_fl(0, !mayfail ? cast_uchar "malloc" : NULL, size + RED_ZONE_INC, file, line)) goto retry;
		return NULL;
	}
#ifdef RED_ZONE
	*((unsigned char *)p + size + RED_ZONE_INC - 1) = RED_ZONE;
#endif
#ifdef LEAK_DEBUG
	ah = p;
	p = (unsigned char *)p + L_D_S;
	ah->size = size - L_D_S;
	ah->magic = ALLOC_MAGIC;
#ifdef LEAK_DEBUG_LIST
	ah->file = file;
	ah->line = line;
	ah->comment = NULL;
	add_to_list(memory_list, ah);
#endif
#endif
#ifdef ALLOC_FILL
	memset(p, ALLOC_FILL, size - L_D_S);
#endif
	return p;
}

void *debug_mem_calloc(unsigned char *file, int line, size_t size, int mayfail)
{
	void *p;
#ifdef LEAK_DEBUG
	struct alloc_header *ah;
#endif
	dos_poll_break();
	debug_test_free(file, line);
	if (!size) return DUMMY;
	if (size > MAXINT) {
		if (mayfail) return NULL;
		overalloc_at(file, line);
	}
	size += L_D_S;
	retry:
#ifdef LEAK_DEBUG
	mem_amount += size - L_D_S;
	mem_blocks++;
#endif
	if (!(p = heap_calloc(size + RED_ZONE_INC))) {
#ifdef LEAK_DEBUG
		mem_amount -= size - L_D_S;
		mem_blocks--;
#endif
		if (out_of_memory_fl(0, !mayfail ? cast_uchar "calloc" : NULL, size + RED_ZONE_INC, file, line)) goto retry;
		return NULL;
	}
#ifdef RED_ZONE
	*((unsigned char *)p + size + RED_ZONE_INC - 1) = RED_ZONE;
#endif
#ifdef LEAK_DEBUG
	ah = p;
	p = (unsigned char *)p + L_D_S;
	ah->size = size - L_D_S;
	ah->magic = ALLOC_MAGIC;
#ifdef LEAK_DEBUG_LIST
	ah->file = file;
	ah->line = line;
	ah->comment = NULL;
	add_to_list(memory_list, ah);
#endif
#endif
	return p;
}

void debug_mem_free(unsigned char *file, int line, void *p)
{
#ifdef LEAK_DEBUG
	struct alloc_header *ah;
#endif
	dos_poll_break();
	if (p == DUMMY) return;
	if (!p) {
		errfile = file, errline = line, int_error("mem_free(NULL)");
		return;
	}
#ifdef LEAK_DEBUG
	p = (unsigned char *)p - L_D_S;
	ah = p;
	if (ah->magic != ALLOC_MAGIC) {
		errfile = file, errline = line, int_error("mem_free: magic doesn't match: %08x", ah->magic);
		return;
	}
#ifdef FREE_FILL
	memset((unsigned char *)p + L_D_S, FREE_FILL, ah->size);
#endif
	ah->magic = ALLOC_FREE_MAGIC;
#ifdef LEAK_DEBUG_LIST
	del_from_list(ah);
	if (ah->comment) heap_free(ah->comment);
#endif
	mem_amount -= ah->size;
	mem_blocks--;
#endif
#ifdef RED_ZONE
	if (*((unsigned char *)p + L_D_S + ah->size + RED_ZONE_INC - 1) != RED_ZONE) {
		errfile = file, errline = line, int_error("mem_free: red zone damaged: %02x (block allocated at %s:%d%s%s)", *((unsigned char *)p + L_D_S + ah->size + RED_ZONE_INC - 1),
#ifdef LEAK_DEBUG_LIST
		ah->file, ah->line, ah->comment ? ":" : "", ah->comment ? ah->comment : (unsigned char *)"");
#else
		"-", 0, "-");
#endif
		return;
	}
#endif
	heap_free(p);
}

void *debug_mem_realloc(unsigned char *file, int line, void *p, size_t size, int mayfail)
{
#ifdef LEAK_DEBUG
	struct alloc_header *ah;
#endif
	void *np;
	if (p == DUMMY) return debug_mem_alloc(file, line, size, mayfail);
	dos_poll_break();
	debug_test_free(file, line);
	if (!p) {
		errfile = file, errline = line, int_error("mem_realloc(NULL, %lu)", (unsigned long)size);
		return NULL;
	}
	if (!size) {
		debug_mem_free(file, line, p);
		return DUMMY;
	}
	if (size > MAXINT) {
		if (mayfail) return NULL;
		overalloc_at(file, line);
	}
#ifdef LEAK_DEBUG
	p = (unsigned char *)p - L_D_S;
	ah = p;
	if (ah->magic != ALLOC_MAGIC) {
		errfile = file, errline = line, int_error("mem_realloc: magic doesn't match: %08x", ah->magic);
		return NULL;
	}
	ah->magic = ALLOC_REALLOC_MAGIC;
#ifdef REALLOC_FILL
	if (!mayfail && size < (size_t)ah->size) memset((unsigned char *)p + L_D_S + size, REALLOC_FILL, ah->size - size);
#endif
#endif
#ifdef RED_ZONE
	if (*((unsigned char *)p + L_D_S + ah->size + RED_ZONE_INC - 1) != RED_ZONE) {
		errfile = file, errline = line, int_error("mem_realloc: red zone damaged: %02x (block allocated at %s:%d%s%s)", *((unsigned char *)p + L_D_S + ah->size + RED_ZONE_INC - 1),
#ifdef LEAK_DEBUG_LIST
		ah->file, ah->line, ah->comment ? ":" : "", ah->comment ? ah->comment : (unsigned char *)"");
#else
		"-", 0, "-");
#endif
		return (unsigned char *)p + L_D_S;
	}
#endif
	retry:
	if (!(np = heap_realloc(p, size + L_D_S + RED_ZONE_INC))) {
		if (out_of_memory_fl(0, !mayfail ? cast_uchar "realloc" : NULL, size + L_D_S + RED_ZONE_INC, file, line)) goto retry;
		ah->magic = ALLOC_MAGIC;
		return NULL;
	}
	p = np;
#ifdef RED_ZONE
	*((unsigned char *)p + size + L_D_S + RED_ZONE_INC - 1) = RED_ZONE;
#endif
#ifdef LEAK_DEBUG
	ah = p;
	/* OpenVMS Alpha C miscompiles this: mem_amount += size - ah->size; */
	mem_amount += size;
	mem_amount -= ah->size;
	ah->size = size;
	ah->magic = ALLOC_MAGIC;
#ifdef LEAK_DEBUG_LIST
	ah->prev->next = ah;
	ah->next->prev = ah;
#endif
#endif
	return (unsigned char *)p + L_D_S;
}

void set_mem_comment(void *p, unsigned char *c, int l)
{
#ifdef LEAK_DEBUG_LIST
	struct alloc_header *ah = (struct alloc_header *)((unsigned char *)p - L_D_S);
	if (ah->comment) heap_free(ah->comment);
	if ((ah->comment = heap_malloc(l + 1))) memcpy(ah->comment, c, l), ah->comment[l] = 0;
#endif
}

#ifdef JS
unsigned char *get_mem_comment(void *p)
{
#ifdef LEAK_DEBUG_LIST
	/* perm je prase: return ((struct alloc_header*)((unsigned char*)((void*)((unsigned char*)p-sizeof(int))) - L_D_S))->comment;*/
	struct alloc_header *ah = (struct alloc_header *)((unsigned char *)p - L_D_S);
	if (!ah->comment) return cast_uchar "";
	else return ah->comment;
#else
	return cast_uchar "";
#endif
}
#endif

#else

void *mem_alloc_(size_t size, int mayfail)
{
	void *p;
	dos_poll_break();
	debug_test_free(NULL, 0);
	if (!size) return DUMMY;
	if (size > MAXINT) {
		if (mayfail) return NULL;
		overalloc();
	}
	retry:
	if (!(p = heap_malloc(size))) {
		if (out_of_memory_fl(0, !mayfail ? cast_uchar "malloc" : NULL, size, NULL, 0)) goto retry;
		return NULL;
	}
	return p;
}

void *mem_calloc_(size_t size, int mayfail)
{
	void *p;
	dos_poll_break();
	debug_test_free(NULL, 0);
	if (!size) return DUMMY;
	if (size > MAXINT) {
		if (mayfail) return NULL;
		overalloc();
	}
	retry:
	if (!(p = heap_calloc(size))) {
		if (out_of_memory_fl(0, !mayfail ? cast_uchar "calloc" : NULL, size, NULL, 0)) goto retry;
		return NULL;
	}
	return p;
}

void mem_free(void *p)
{
	dos_poll_break();
	if (p == DUMMY) return;
	if (!p) {
		internal("mem_free(NULL)");
		return;
	}
	heap_free(p);
}

void *mem_realloc_(void *p, size_t size, int mayfail)
{
	void *np;
	if (p == DUMMY) return mem_alloc_(size, mayfail);
	dos_poll_break();
	debug_test_free(NULL, 0);
	if (!p) {
		internal("mem_realloc(NULL, %lu)", (unsigned long)size);
		return NULL;
	}
	if (!size) {
		mem_free(p);
		return DUMMY;
	}
	if (size > MAXINT) {
		if (mayfail) return NULL;
		overalloc();
	}
	retry:
	if (!(np = heap_realloc(p, size))) {
		if (out_of_memory_fl(0, !mayfail ? cast_uchar "realloc" : NULL, size, NULL, 0)) goto retry;
		return NULL;
	}
	return np;
}

#endif

#if !(defined(LEAK_DEBUG) && defined(LEAK_DEBUG_LIST))

unsigned char *memacpy(const unsigned char *src, size_t len)
{
	unsigned char *m;
	if (!(len + 1)) overalloc();
	m = (unsigned char *)mem_alloc(len + 1);
	memcpy(m, src, len);
	m[len] = 0;
	return m;
}

unsigned char *stracpy(const unsigned char *src)
{
	return src ? memacpy(src, src != DUMMY ? strlen(cast_const_char src) : 0) : NULL;
}

#else

unsigned char *debug_memacpy(unsigned char *f, int l, const unsigned char *src, size_t len)
{
	unsigned char *m;
	m = (unsigned char *)debug_mem_alloc(f, l, len + 1, 0);
	memcpy(m, src, len);
	m[len] = 0;
	return m;
}

unsigned char *debug_stracpy(unsigned char *f, int l, const unsigned char *src)
{
	return src ? (unsigned char *)debug_memacpy(f, l, src, src != DUMMY ? strlen(cast_const_char src) : 0L) : NULL;
}

#endif

#ifdef OOPS

struct prot {
	struct prot *next;
	struct prot *prev;
	sigjmp_buf buf;
};

static struct list_head prot = {&prot, &prot };

static void fault(void *dummy)
{
	struct prot *p;
	/*fprintf(stderr, "FAULT: %d !\n", (int)(unsigned long)dummy);*/
	if (list_empty(prot)) {
		fatal_tty_exit();
		exit(0);
	}
	p = prot.next;
	del_from_list(p);
	longjmp(p->buf, 1);
}

sigjmp_buf *new_stack_frame(void)
{
	static int handled = 0;
	struct prot *new;
	if (!handled) {
#ifdef SIGILL
		install_signal_handler(SIGILL, fault, (void *)SIGILL, 1);
#endif
#ifdef SIGABRT
		install_signal_handler(SIGABRT, fault, (void *)SIGABRT, 1);
#endif
#ifdef SIGFPE
		install_signal_handler(SIGFPE, fault, (void *)SIGFPE, 1);
#endif
#ifdef SIGSEGV
		install_signal_handler(SIGSEGV, fault, (void *)SIGSEGV, 1);
#endif
#ifdef SIGBUS
		install_signal_handler(SIGBUS, fault, (void *)SIGBUS, 1);
#endif
		handled = 1;
	}
	if (!(new = mem_alloc(sizeof(struct prot)))) return NULL;
	add_to_list(prot, new);
	return &new->buf;
}

void xpr(void)
{
	if (!list_empty(prot)) {
		struct prot *next = prot.next;
		del_from_list(next);
		mem_free(next);
	}
}

void nopr(void)
{
	free_list(prot);
}

#endif


