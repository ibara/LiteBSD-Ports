/* cache.c
 * (c) 2002 Mikulas Patocka
 * This file is a part of the Links program, released under GPL.
 */

#include "links.h"

static struct list_head cache = {&cache, &cache};

static my_uintptr_t cache_size = 0;

static tcount cache_count = 1;

my_uintptr_t cache_info(int type)
{
	my_uintptr_t i = 0;
	struct cache_entry *ce;
	switch (type) {
		case CI_BYTES:
			return cache_size;
		case CI_FILES:
			foreach(ce, cache) i++;
			return i;
		case CI_LOCKED:
			foreach(ce, cache) i += !!ce->refcount;
			return i;
		case CI_LOADING:
			foreach(ce, cache) i += is_entry_used(ce);
			return i;
		default:
			internal("cache_info: bad request");
	}
	return 0;
}

my_uintptr_t decompress_info(int type)
{
	my_uintptr_t i = 0;
	struct cache_entry *ce;
	switch (type) {
		case CI_BYTES:
			return decompressed_cache_size;
		case CI_FILES:
			foreach(ce, cache) i += !!ce->decompressed;
			return i;
		case CI_LOCKED:
			foreach(ce, cache) i += ce->decompressed && ce->refcount;
			return i;
		default:
			internal("compress_info: bad request");
	}
	return 0;
}

static unsigned char *extract_proxy(unsigned char *url)
{
	unsigned char *a;
	if (strlen(cast_const_char url) < 8 || casecmp(url, cast_uchar "proxy://", 8)) return url;
	if (!(a = cast_uchar strchr(cast_const_char(url + 8), '/'))) return url;
	return a + 1;
}

int find_in_cache(unsigned char *url, struct cache_entry **f)
{
	struct cache_entry *e;
	url = extract_proxy(url);
	foreach(e, cache) if (!strcmp(cast_const_char e->url, cast_const_char url)) {
		e->refcount++;
		del_from_list(e);
		add_to_list(cache, e);
		*f = e;
		return 0;
	}
	return -1;
}

int get_cache_entry(unsigned char *url, struct cache_entry **f)
{
	if (!find_in_cache(url, f)) return 0;
	return new_cache_entry(url, f);
}

int new_cache_entry(unsigned char *url, struct cache_entry **f)
{
	struct cache_entry *e;
	shrink_memory(SH_CHECK_QUOTA, 0);
	url = extract_proxy(url);
	e = mem_calloc(sizeof(struct cache_entry));
	e->url = mem_alloc(strlen(cast_const_char url)+1);
	strcpy(cast_char e->url, cast_const_char url);
	e->length = 0;
	e->incomplete = 1;
	e->data_size = 0;
	e->http_code = -1;
	init_list(e->frag);
	e->count = cache_count++;
	e->count2 = cache_count++;
	e->refcount = 1;
	e->decompressed = NULL;
	e->decompressed_len = 0;
	add_to_list(cache, e);
	*f = e;
	return 0;
}

void detach_cache_entry(struct cache_entry *e)
{
	e->url[0] = 0;
}

static void mem_free_fragment(struct fragment *f)
{
	size_t s = (size_t)f->length;
	mem_free(f);
	s += sizeof(struct fragment);
	mem_freed_large(s);
}

#define sf(x) e->data_size += (x), cache_size += (my_uintptr_t)(x)

int page_size = 4096;

#define C_ALIGN(x) ((((x) + sizeof(struct fragment) + alloc_overhead) | (page_size - 1)) - sizeof(struct fragment) - alloc_overhead)

int add_fragment(struct cache_entry *e, off_t offset, unsigned char *data, off_t length)
{
	struct fragment *f;
	struct fragment *nf;
	int trunc = 0;
	volatile off_t ca;
	if (!length) return 0;
	free_decompressed_data(e);
	e->incomplete = 1;
	if ((off_t)(0UL + offset + length) < 0 || (off_t)(0UL + offset + length) < offset) return S_LARGE_FILE;
	if ((off_t)(0UL + offset + (off_t)C_ALIGN(length)) < 0 || (off_t)(0UL + offset + (off_t)C_ALIGN(length)) < offset) return S_LARGE_FILE;
	if (e->length < offset + length) e->length = offset + length;
	e->count = cache_count++;
	if (list_empty(e->frag)) e->count2 = cache_count++;
	else {
		f = e->frag.prev;
		if (f->offset + f->length != offset) e->count2 = cache_count++;
		else goto have_f;
	}
	foreach(f, e->frag) {
have_f:
		if (f->offset > offset) break;
		if (f->offset <= offset && f->offset+f->length >= offset) {
			if (offset+length > f->offset+f->length) {
				if (memcmp(f->data+offset-f->offset, data, (size_t)(f->offset+f->length-offset))) trunc = 1;
				if (offset-f->offset+length <= f->real_length) {
					sf((offset+length) - (f->offset+f->length));
					f->length = offset-f->offset+length;
				} else {
					sf(-(f->offset+f->length-offset));
					f->length = offset-f->offset;
					f = f->next;
					break;
				}
			} else {
				if (memcmp(f->data+offset-f->offset, data, (size_t)length)) trunc = 1;
			}
			memcpy(f->data+offset-f->offset, data, (size_t)length);
			goto ch_o;
		}
	}
/* Intel C 9 has a bug and miscompiles this statement (< 0 test is true) */
	/*if (C_ALIGN(length) > MAXINT - sizeof(struct fragment) || C_ALIGN(length) < 0) overalloc();*/
	ca = C_ALIGN(length);
	if (ca > MAXINT - (int)sizeof(struct fragment) || ca < 0) return S_LARGE_FILE;
	nf = mem_alloc_mayfail(sizeof(struct fragment) + (size_t)ca);
	if (!nf) return S_OUT_OF_MEM;
	sf(length);
	nf->offset = offset;
	nf->length = length;
	nf->real_length = C_ALIGN(length);
	memcpy(nf->data, data, (size_t)length);
	add_at_pos(f->prev, nf);
	f = nf;
	ch_o:
	while ((void *)f->next != &e->frag && f->offset+f->length > f->next->offset) {
		if (f->offset+f->length < f->next->offset+f->next->length) {
			nf = mem_realloc(f, sizeof(struct fragment)+(size_t)(f->next->offset-f->offset+f->next->length));
			nf->prev->next = nf;
			nf->next->prev = nf;
			f = nf;
			if (memcmp(f->data+f->next->offset-f->offset, f->next->data, (size_t)(f->offset+f->length-f->next->offset))) trunc = 1;
			memcpy(f->data+f->length, f->next->data+f->offset+f->length-f->next->offset, (size_t)(f->next->offset+f->next->length-f->offset-f->length));
			sf(f->next->offset+f->next->length-f->offset-f->length);
			f->length = f->real_length = f->next->offset+f->next->length-f->offset;
		} else {
			if (memcmp(f->data+f->next->offset-f->offset, f->next->data, (size_t)f->next->length)) trunc = 1;
		}
		nf = f->next;
		del_from_list(nf);
		sf(-nf->length);
		mem_free_fragment(nf);
	}
	if (trunc) truncate_entry(e, offset + length, 0);
	/*{
		foreach(f, e->frag) fprintf(stderr, "%d, %d, %d\n", f->offset, f->length, f->real_length);
		debug(cast_uchar "a-");
	}*/
	if (e->length > e->max_length) {
		e->max_length = e->length;
		return 1;
	}
	return 0;
}

int defrag_entry(struct cache_entry *e)
{
	struct fragment *f, *g, *h, *n, *x;
	off_t l;
	if (list_empty(e->frag)) {
		return 0;
	}
	f = e->frag.next;
	if (f->offset) {
		return 0;
	}
	for (g = f->next; g != (void *)&e->frag && g->offset <= g->prev->offset+g->prev->length; g = g->next) if (g->offset < g->prev->offset+g->prev->length) {
		internal("fragments overlay");
		return S_INTERNAL;
	}
	/*debug(cast_uchar "%p %p %d %d", g, f->next, f->length, f->real_length);*/
	if (g == f->next) {
		if (f->length != f->real_length) {
			f = mem_realloc_mayfail(f, sizeof(struct fragment) + (size_t)f->length);
			if (f) {
				f->real_length = f->length;
				f->next->prev = f;
				f->prev->next = f;
			}
		}
		return 0;
	}
	for (l = 0, h = f; h != g; h = h->next) {
		if ((off_t)(0UL + l + h->length) < 0 || (off_t)(0UL + l + h->length) < l) return S_LARGE_FILE;
		l += h->length;
	}
	if (l > MAXINT - (int)sizeof(struct fragment)) return S_LARGE_FILE;
	n = mem_alloc_mayfail(sizeof(struct fragment) + (size_t)l);
	if (!n) return S_OUT_OF_MEM;
	n->offset = 0;
	n->length = l;
	n->real_length = l;
	/*{
		struct fragment *f;
		foreach(f, e->frag) fprintf(stderr, cast_uchar "%d, %d, %d\n", f->offset, f->length, f->real_length);
		debug(cast_uchar "d1-");
	}*/
	for (l = 0, h = f; h != g; h = h->next) {
		memcpy(n->data + l, h->data, (size_t)h->length);
		l += h->length;
		x = h;
		h = h->prev;
		del_from_list(x);
		mem_free_fragment(x);
	}
	add_to_list(e->frag, n);
	/*{
		foreach(f, e->frag) fprintf(stderr, "%d, %d, %d\n", f->offset, f->length, f->real_length);
		debug(cast_uchar "d-");
	}*/
	return 0;
}

void truncate_entry(struct cache_entry *e, off_t off, int final)
{
	int modified = 0;
	struct fragment *f, *g;
	if (e->length > off) e->length = off, e->incomplete = 1;
	foreach(f, e->frag) {
		if (f->offset >= off) {
			del:
			while ((void *)f != &e->frag) {
				modified = 1;
				sf(-f->length);
				g = f->next;
				del_from_list(f);
				mem_free_fragment(f);
				f = g;
			}
			goto ret;
		}
		if (f->offset + f->length > off) {
			modified = 1;
			sf(-(f->offset + f->length - off));
			f->length = off - f->offset;
			if (final) {
				g = mem_realloc(f, sizeof(struct fragment) + (size_t)f->length);
				g->next->prev = g;
				g->prev->next = g;
				f = g;
				f->real_length = f->length;
			}
			f = f->next;
			goto del;
		}
	}
	ret:
	if (modified) {
		free_decompressed_data(e);
		e->count = cache_count++;
		e->count2 = cache_count++;
	}
}

void free_entry_to(struct cache_entry *e, off_t off)
{
	struct fragment *f, *g;
	e->incomplete = 1;
	free_decompressed_data(e);
	foreach(f, e->frag) {
		if (f->offset + f->length <= off) {
			sf(-f->length);
			g = f;
			f = f->prev;
			del_from_list(g);
			mem_free_fragment(g);
		} else if (f->offset < off) {
			sf(f->offset - off);
			memmove(f->data, f->data + (off - f->offset), (size_t)(f->length -= off - f->offset));
			f->offset = off;
		} else break;
	}
}

void delete_entry_content(struct cache_entry *e)
{
	e->count = cache_count++;
	e->count2 = cache_count++;
	free_list(e->frag);
	e->length = 0;
	e->incomplete = 1;
	if (cache_size < (my_uintptr_t)e->data_size) {
		internal("cache_size underflow: %lu, %lu", (unsigned long)cache_size, (unsigned long)e->data_size);
	}
	cache_size -= (my_uintptr_t)e->data_size;
	e->data_size = 0;
	if (e->last_modified) {
		mem_free(e->last_modified);
		e->last_modified = NULL;
	}
	free_decompressed_data(e);
}

void trim_cache_entry(struct cache_entry *e)
{
	struct fragment *f, *nf;
	foreach(f, e->frag) {
		if (f->length != f->real_length) {
			nf = mem_realloc_mayfail(f, sizeof(struct fragment) + (size_t)f->length);
			if (nf) {
				f = nf;
				f->real_length = f->length;
				f->next->prev = f;
				f->prev->next = f;
			}
		}
	}
}

void delete_cache_entry(struct cache_entry *e)
{
	if (e->refcount) internal("deleteing locked cache entry");
#ifdef DEBUG
	if (is_entry_used(e)) internal("deleting loading cache entry");
#endif
	delete_entry_content(e);
	del_from_list(e);
	mem_free(e->url);
	if (e->head) mem_free(e->head);
	if (e->last_modified) mem_free(e->last_modified);
	if (e->redirect) mem_free(e->redirect);
#ifdef HAVE_SSL
	if (e->ssl_info) mem_free(e->ssl_info);
#endif
	mem_free(e);
}

static int shrink_file_cache(int u)
{
	int r = 0;
	struct cache_entry *e, *f;
	my_uintptr_t ncs = cache_size;
	my_uintptr_t ccs = 0, ccs2 = 0;

	if (u == SH_CHECK_QUOTA && cache_size + decompressed_cache_size <= (my_uintptr_t)memory_cache_size) goto ret;
	foreachback(e, cache) {
		if (e->refcount || is_entry_used(e)) {
			if (ncs < (my_uintptr_t)e->data_size) {
				internal("cache_size underflow: %lu, %lu", (unsigned long)ncs, (unsigned long)e->data_size);
			}
			ncs -= (my_uintptr_t)e->data_size;
		} else if (u == SH_FREE_SOMETHING) {
			if (e->decompressed_len) free_decompressed_data(e);
			else delete_cache_entry(e);
			r |= ST_SOMETHING_FREED;
			goto ret;
		}
		if (!e->refcount && e->decompressed_len && cache_size + decompressed_cache_size > (my_uintptr_t)memory_cache_size) {
			free_decompressed_data(e);
			r |= ST_SOMETHING_FREED;
		}
		ccs += (my_uintptr_t)e->data_size;
		ccs2 += e->decompressed_len;
	}
	if (ccs != cache_size) internal("cache size badly computed: %lu != %lu", (unsigned long)cache_size, (unsigned long)ccs), cache_size = ccs;
	if (ccs2 != decompressed_cache_size) internal("decompressed cache size badly computed: %lu != %lu", (unsigned long)decompressed_cache_size, (unsigned long)ccs2), decompressed_cache_size = ccs2;
	if (u == SH_CHECK_QUOTA && ncs <= (my_uintptr_t)memory_cache_size) goto ret;
	foreachback(e, cache) {
		if (u == SH_CHECK_QUOTA && (longlong)ncs <= (longlong)memory_cache_size * MEMORY_CACHE_GC_PERCENT) goto g;
		if (e->refcount || is_entry_used(e)) {
			e->tgc = 0;
			continue;
		}
		e->tgc = 1;
		if (ncs < (my_uintptr_t)e->data_size) {
			internal("cache_size underflow: %lu, %lu", (unsigned long)ncs, (unsigned long)e->data_size);
		}
		ncs -= (my_uintptr_t)e->data_size;
	}
	if (ncs) internal("cache_size(%lu) is larger than size of all objects(%lu)", (unsigned long)cache_size, (unsigned long)(cache_size - ncs));
	g:
	if ((void *)(e = e->next) == &cache) goto ret;
	if (u == SH_CHECK_QUOTA) for (f = e; (void *)f != &cache; f = f->next) {
		if (f->data_size && (longlong)ncs + f->data_size <= (longlong)memory_cache_size * MEMORY_CACHE_GC_PERCENT) {
			ncs += (my_uintptr_t)f->data_size;
			f->tgc = 0;
		}
	}
	for (f = e; (void *)f != &cache;) {
		f = f->next;
		if (f->prev->tgc) {
			delete_cache_entry(f->prev);
			r |= ST_SOMETHING_FREED;
		}
	}
	ret:
	return r | (list_empty(cache) ? ST_CACHE_EMPTY : 0);
}

void init_cache(void)
{
#ifdef HAVE_GETPAGESIZE
	int getpg;
	EINTRLOOP(getpg, getpagesize());
	if (getpg > 0 && getpg < 0x10000 && !(getpg & (getpg - 1))) page_size = getpg;
#endif
	register_cache_upcall(shrink_file_cache, 0, cast_uchar "file");
}
