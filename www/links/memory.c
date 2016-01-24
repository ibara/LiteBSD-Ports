/* memory.c
 * (c) 2002 Mikulas Patocka
 * This file is a part of the Links program, released under GPL.
 */

#include "links.h"

struct cache_upcall {
	struct cache_upcall *next;
	struct cache_upcall *prev;
	int (*upcall)(int);
	unsigned char flags;
	unsigned char name[1];
};

static struct list_head cache_upcalls = { &cache_upcalls, &cache_upcalls }; /* cache_upcall */

int shrink_memory(int type, int flags)
{
	struct cache_upcall *c;
	int a = 0;
	foreach(c, cache_upcalls) {
		if (flags && !(c->flags & flags)) continue;
		a |= c->upcall(type);
	}
#if defined(HAVE__HEAPMIN) || defined(HAVE_MALLOC_TRIM)
	{
		static uttime last_heapmin = 0;
		static uttime min_interval = 0;
		uttime now = get_time();
		/* malloc_trim degrades performance (unlike _heapmin), so we call it less often */
		if (type == SH_FREE_ALL || (now - last_heapmin >= min_interval &&
#if defined(HAVE_MALLOC_TRIM)
		    now - last_heapmin >= MALLOC_TRIM_INTERVAL
#else
		    (a & ST_SOMETHING_FREED || now - last_heapmin >= HEAPMIN_INTERVAL)
#endif
		   )) {
			uttime after;
#if defined(HAVE__HEAPMIN)
			_heapmin();
#endif
#if defined(HAVE_MALLOC_TRIM)
			malloc_trim(0);
#endif
			after = get_time();
			min_interval = HEAPMIN_FACTOR * (after - now);
			last_heapmin = after;
		}
	}
#endif
	return a;
}

void register_cache_upcall(int (*upcall)(int), int flags, unsigned char *name)
{
	struct cache_upcall *c;
	c = mem_alloc(sizeof(struct cache_upcall) + strlen(cast_const_char name));
	c->upcall = upcall;
	c->flags = (unsigned char)flags;
	strcpy(cast_char c->name, cast_const_char name);
	add_to_list(cache_upcalls, c);
}

void free_all_caches(void)
{
	struct cache_upcall *c;
	int a, b;
	do {
		a = 0;
		b = ~0;
		foreach(c, cache_upcalls) {
			int x = c->upcall(SH_FREE_ALL);
			a |= x;
			b &= x;
		}
	} while (a & ST_SOMETHING_FREED);
	if (!(b & ST_CACHE_EMPTY)) {
		unsigned char *m = init_str();
		int l = 0;
		foreach(c, cache_upcalls) if (!(c->upcall(SH_FREE_ALL) & ST_CACHE_EMPTY)) {
			if (l) add_to_str(&m, &l, cast_uchar ", ");
			add_to_str(&m, &l, c->name);
		}
		internal("could not release entries from caches: %s", m);
		mem_free(m);
	}
	free_list(cache_upcalls);
}

int malloc_try_hard = 0;

int out_of_memory_fl(int flags, unsigned char *msg, size_t size, unsigned char *file, int line)
{
	int sh;
retry:
	sh = shrink_memory(SH_FREE_SOMETHING, flags);
	/*fprintf(stderr, "out of memory: %d, %d (%s,%d)\n", flags, sh, msg, size);*/
	if (sh & ST_SOMETHING_FREED) return 1;
	if (flags) {
		flags = 0;
		goto retry;
	}
	if (!malloc_try_hard) {
		malloc_try_hard = 1;
		return 1;
	}
	if (!msg) return 0;

	fatal_tty_exit();

	fprintf(stderr, "\n");
#ifdef LEAK_DEBUG
	fprintf(stderr, "Allocated: %lu bytes, %lu blocks\n", (unsigned long)mem_amount, (unsigned long)mem_blocks);
#endif
	fprintf(stderr, "File cache: %lu bytes, %lu files, %lu locked, %lu loading\n", (unsigned long)cache_info(CI_BYTES), (unsigned long)cache_info(CI_FILES), (unsigned long)cache_info(CI_LOCKED), (unsigned long)cache_info(CI_LOADING));
#ifdef HAVE_ANY_COMPRESSION
	fprintf(stderr, "Decompressed cache: %lu bytes, %lu files, %lu locked\n", (unsigned long)decompress_info(CI_BYTES), (unsigned long)decompress_info(CI_FILES), (unsigned long)decompress_info(CI_LOCKED));
#endif
#ifdef G
	if (F) {
		fprintf(stderr, "Image cache: %lu bytes, %lu files, %lu locked\n", (unsigned long)imgcache_info(CI_BYTES), (unsigned long)imgcache_info(CI_FILES), (unsigned long)imgcache_info(CI_LOCKED));
		fprintf(stderr, "Font cache: %lu bytes, %lu letters\n", (unsigned long)fontcache_info(CI_BYTES), (unsigned long)fontcache_info(CI_FILES));
	}
#endif
	fprintf(stderr, "Formatted document cache: %lu documents, %lu locked\n", formatted_info(CI_FILES), formatted_info(CI_LOCKED));
	fprintf(stderr, "DNS cache: %lu servers\n", dns_info(CI_FILES));

	if (file) fatal_exit("ERROR: out of memory (%s(%lu) at %s:%d returned NULL)", msg, (unsigned long)size, file, line);
	else fatal_exit("ERROR: out of memory (%s(%lu) returned NULL)", msg, (unsigned long)size);
	return 0;
}

#ifdef DEBUG_TEST_FREE

struct debug_test_free_slot {
	struct debug_test_free_slot *next;
	struct debug_test_free_slot *prev;
	unsigned char *file;
	int line;
	unsigned long count;
};

static struct list_head debug_test_free_slots = {&debug_test_free_slots, &debug_test_free_slots};

#define DEBUG_TEST_FREE_DEFAULT_PROB	1024
#define DEBUG_TEST_FREE_INIT_COUNT	16

void debug_test_free(unsigned char *file, int line)
{
	struct debug_test_free_slot *sl = NULL;
	unsigned long prob;
	if (!file) {
		prob = DEBUG_TEST_FREE_DEFAULT_PROB;
		goto fixed_prob;
	}
	foreach(sl, debug_test_free_slots) {
		if (sl->line == line && (sl->file == file || !strcmp(cast_const_char sl->file, cast_const_char file))) {
			del_from_list(sl);
			goto have_it;
		}
	}
	retry:
	sl = malloc(sizeof(struct debug_test_free_slot));
	if (!sl) {
		if (out_of_memory(0, NULL, 0))
			goto retry;
		return;
	}
	sl->file = file;
	sl->line = line;
	sl->count = DEBUG_TEST_FREE_INIT_COUNT;
	have_it:
	add_to_list(debug_test_free_slots, sl);
	prob = sl->count;
	sl->count++;

	fixed_prob:
	if (!prob) prob = 1;
	if (!(random() % prob)) {
		if (shrink_memory(SH_FREE_SOMETHING) & ST_SOMETHING_FREED) {
			/*if (sl) sl->count++;*/
		}
	}
}

#endif
