/* dns.c
 * (c) 2002 Mikulas Patocka
 * This file is a part of the Links program, released under GPL
 */

#include "links.h"

#ifdef SUPPORT_IPV6
int support_ipv6;
#endif

#if !defined(USE_GETADDRINFO) && (defined(HAVE_GETHOSTBYNAME_BUG) || !defined(HAVE_GETHOSTBYNAME))
#define EXTERNAL_LOOKUP
#endif

#if defined(WIN) || defined(INTERIX)
#define EXTRA_IPV6_LOOKUP
#endif

#ifdef OPENVMS_64BIT
#define addrinfo        __addrinfo64
#endif

struct dnsentry {
	struct dnsentry *next;
	struct dnsentry *prev;
	ttime get_time;
	struct lookup_result addr;
	unsigned char name[1];
};

#ifndef THREAD_SAFE_LOOKUP
struct dnsquery *dns_queue = NULL;
#endif

struct dnsquery {
#ifndef THREAD_SAFE_LOOKUP
	struct dnsquery *next_in_queue;
#endif
	void (*fn)(void *, int);
	void *data;
	int h;
	struct dnsquery **s;
	struct lookup_result *addr;
	int addr_preference;
	unsigned char name[1];
};

static int dns_cache_addr_preference = -1;
static struct list_head dns_cache = {&dns_cache, &dns_cache};

static void end_dns_lookup(struct dnsquery *q, int a);
static int shrink_dns_cache(int u);

static int get_addr_byte(unsigned char **ptr, unsigned char *res, unsigned char stp)
{
	unsigned u = 0;
	if (!(**ptr >= '0' && **ptr <= '9')) return -1;
	while (**ptr >= '0' && **ptr <= '9') {
		u = u * 10 + **ptr - '0';
		if (u >= 256) return -1;
		(*ptr)++;
	}
	if (stp != 255 && **ptr != stp) return -1;
	(*ptr)++;
	*res = (unsigned char)u;
	return 0;
}

int numeric_ip_address(unsigned char *name, unsigned char address[4])
{
	unsigned char dummy[4];
	if (!address) address = dummy;
	if (get_addr_byte(&name, address + 0, '.')) return -1;
	if (get_addr_byte(&name, address + 1, '.')) return -1;
	if (get_addr_byte(&name, address + 2, '.')) return -1;
	if (get_addr_byte(&name, address + 3, 0)) return -1;
	return 0;
}

#ifdef SUPPORT_IPV6

static int extract_ipv6_address(struct addrinfo *p, unsigned char address[16], unsigned *scope_id)
{
	/*{
		int i;
		for (i = 0; i < p->ai_addrlen; i++)
			fprintf(stderr, "%02x%c", ((unsigned char *)p->ai_addr)[i], i != p->ai_addrlen - 1 ? ':' : '\n');
	}*/
	if (p->ai_family == AF_INET6 && (socklen_t)p->ai_addrlen >= (socklen_t)sizeof(struct sockaddr_in6) && p->ai_addr->sa_family == AF_INET6) {
		memcpy(address, &((struct sockaddr_in6 *)p->ai_addr)->sin6_addr, 16);
#ifdef SUPPORT_IPV6_SCOPE
		*scope_id = ((struct sockaddr_in6 *)p->ai_addr)->sin6_scope_id;
#else
		*scope_id = 0;
#endif
		return 0;
	}
	return -1;
}

int numeric_ipv6_address(unsigned char *name, unsigned char address[16], unsigned *scope_id)
{
	unsigned char dummy_a[16];
	unsigned dummy_s;
	int r;
#ifdef HAVE_INET_PTON
	struct in6_addr i6a;
#endif
	struct addrinfo hints, *res;
	if (!address) address = dummy_a;
	if (!scope_id) scope_id = &dummy_s;

#ifdef HAVE_INET_PTON
	if (inet_pton(AF_INET6, cast_const_char name, &i6a) == 1) {
		memcpy(address, &i6a, 16);
		*scope_id = 0;
		return 0;
	}
	if (!strchr(cast_const_char name, '%'))
		return -1;
#endif

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET6;
	hints.ai_flags = AI_NUMERICHOST;
	if (getaddrinfo(cast_const_char name, NULL, &hints, &res))
		return -1;
	r = extract_ipv6_address(res, address, scope_id);
	freeaddrinfo(res);
	return r;
}

#endif

#ifdef EXTERNAL_LOOKUP

static int do_external_lookup(unsigned char *name, unsigned char *host)
{
	unsigned char buffer[1024];
	unsigned char sink[16];
	int rd;
	int pi[2];
	pid_t f;
	unsigned char *n;
	int rs;
	if (c_pipe(pi) == -1)
		return -1;
	EINTRLOOP(f, fork());
	if (f == -1) {
		EINTRLOOP(rs, close(pi[0]));
		EINTRLOOP(rs, close(pi[1]));
		return -1;
	}
	if (!f) {
#ifdef HAVE_SETSID
		/* without setsid it gets stuck when on background */
		EINTRLOOP(rs, setsid());
#endif
		EINTRLOOP(rs, close(pi[0]));
		EINTRLOOP(rs, dup2(pi[1], 1));
		if (rs == -1) _exit(1);
		EINTRLOOP(rs, dup2(pi[1], 2));
		if (rs == -1) _exit(1);
		EINTRLOOP(rs, close(pi[1]));
		EINTRLOOP(rs, execlp("host", "host", name, NULL));
		EINTRLOOP(rs, execl("/usr/sbin/host", "host", name, NULL));
		_exit(1);
	}
	EINTRLOOP(rs, close(pi[1]));
	rd = hard_read(pi[0], buffer, sizeof buffer - 1);
	if (rd >= 0) buffer[rd] = 0;
	if (rd > 0) {
		while (hard_read(pi[0], sink, sizeof sink) > 0);
	}
	EINTRLOOP(rs, close(pi[0]));
	/* Don't wait for the process, we already have sigchld handler that
	 * does cleanup.
	 * waitpid(f, NULL, 0); */
	if (rd < 0) return -1;
	/*fprintf(stderr, "query: '%s', result: %s\n", name, buffer);*/
	while ((n = strstr(buffer, name))) {
		memset(n, '-', strlen(cast_const_char name));
	}
	for (n = buffer; n < buffer + rd; n++) {
		if (*n >= '0' && *n <= '9') {
			if (get_addr_byte(&n, host + 0, '.')) goto skip_addr;
			if (get_addr_byte(&n, host + 1, '.')) goto skip_addr;
			if (get_addr_byte(&n, host + 2, '.')) goto skip_addr;
			if (get_addr_byte(&n, host + 3, 255)) goto skip_addr;
			return 0;
skip_addr:
			if (n >= buffer + rd) break;
		}
	}
	return -1;
}

#endif

static void add_address(struct lookup_result *host, int af, unsigned char *address, unsigned scope_id, int preference)
{
	struct host_address neww;
	struct host_address *e, *n, *t;
	if (af != AF_INET && preference == ADDR_PREFERENCE_IPV4_ONLY)
		return;
#ifdef SUPPORT_IPV6
	if (af != AF_INET6 && preference == ADDR_PREFERENCE_IPV6_ONLY)
		return;
#endif
	if (host->n >= MAX_ADDRESSES)
		return;
	memset(&neww, 0, sizeof(struct host_address));
	neww.af = af;
	memcpy(neww.addr, address, af == AF_INET ? 4 : 16);
	neww.scope_id = scope_id;
	e = &host->a[host->n];
	t = e;
	for (n = host->a; n != e; n++) {
		if (!memcmp(n, &neww, sizeof(struct host_address)))
			return;
		if (preference == ADDR_PREFERENCE_IPV4 && af == AF_INET && n->af != AF_INET) {
			t = n;
			break;
		}
#ifdef SUPPORT_IPV6
		if (preference == ADDR_PREFERENCE_IPV6 && af == AF_INET6 && n->af != AF_INET6) {
			t = n;
			break;
		}
#endif
	}
	memmove(t + 1, t, (e - t) * sizeof(struct host_address));
	memcpy(t, &neww, sizeof(struct host_address));
	host->n++;
}

#ifdef USE_GETADDRINFO

static int use_getaddrinfo(unsigned char *name, struct addrinfo *hints, int preference, struct lookup_result *host)
{
	int gai_err;
	struct addrinfo *res, *p;
	gai_err = getaddrinfo(cast_const_char name, NULL, hints, &res);
	if (gai_err)
		return gai_err;
	for (p = res; p; p = p->ai_next) {
		if (p->ai_family == AF_INET && (socklen_t)p->ai_addrlen >= (socklen_t)sizeof(struct sockaddr_in) && p->ai_addr->sa_family == AF_INET) {
			add_address(host, AF_INET, (unsigned char *)&((struct sockaddr_in *)p->ai_addr)->sin_addr.s_addr, 0, preference);
			continue;
		}
#ifdef SUPPORT_IPV6
		{
			unsigned char address[16];
			unsigned scope_id;
			if (!extract_ipv6_address(p, address, &scope_id)) {
				add_address(host, AF_INET6, address, scope_id, preference);
				continue;
			}
		}
#endif
	}
	freeaddrinfo(res);
	return 0;
}

#endif

void do_real_lookup(unsigned char *name, int preference, struct lookup_result *host)
{
	unsigned char address[16];
#ifdef SUPPORT_IPV6
	size_t nl;
#endif

	if (!support_ipv6) preference = ADDR_PREFERENCE_IPV4_ONLY;

	memset(host, 0, sizeof(struct lookup_result));
	if (!numeric_ip_address(name, address)) {
		add_address(host, AF_INET, address, 0, preference);
		return;
	}
#ifdef SUPPORT_IPV6
	nl = strlen(cast_const_char name);
	if (name[0] == '[' && name[nl - 1] == ']') {
		unsigned char *n2 = cast_uchar strdup(cast_const_char(name + 1));
		if (n2) {
			unsigned scope_id;
			n2[nl - 2] = 0;
			if (!numeric_ipv6_address(n2, address, &scope_id)) {
				free(n2);
				add_address(host, AF_INET6, address, scope_id, preference);
				return;
			}
			free(n2);
		}
	} else {
		unsigned scope_id;
		if (!numeric_ipv6_address(name, address, &scope_id)) {
			add_address(host, AF_INET6, address, scope_id, preference);
			return;
		}
	}
#endif

#if defined(USE_GETADDRINFO)
	use_getaddrinfo(name, NULL, preference, host);
#if defined(SUPPORT_IPV6) && defined(EXTRA_IPV6_LOOKUP)
	if ((preference == ADDR_PREFERENCE_IPV4 && !host->n) ||
	    preference == ADDR_PREFERENCE_IPV6 ||
	    preference == ADDR_PREFERENCE_IPV6_ONLY) {
		struct addrinfo hints;
		int i;
		for (i = 0; i < host->n; i++)
			if (host->a[i].af == AF_INET6)
				goto already_have_inet6;
		memset(&hints, 0, sizeof hints);
		hints.ai_family = AF_INET6;
		hints.ai_flags = 0;
		use_getaddrinfo(name, &hints, preference, host);
	}
	already_have_inet6:;
#endif
#elif defined(HAVE_GETHOSTBYNAME)
	{
		int i;
		struct hostent *hst;
		if ((hst = gethostbyname(cast_const_char name))) {
			if (hst->h_addrtype != AF_INET || hst->h_length != 4 || !hst->h_addr) return;
#ifdef h_addr
			for (i = 0; hst->h_addr_list[i]; i++) {
				add_address(host, AF_INET, cast_uchar hst->h_addr_list[i], 0, preference);
			}
#else
			add_address(host, AF_INET, cast_uchar hst->h_addr, 0, preference);
#endif
			return;
		}
	}
#endif

#ifdef EXTERNAL_LOOKUP
	if (!do_external_lookup(name, address)) {
		add_address(host, AF_INET, address, 0, preference);
		return;
	}
#endif
	return;
}

#ifndef NO_ASYNC_LOOKUP
static void lookup_fn(struct dnsquery *q, int h)
{
	struct lookup_result host;
	do_real_lookup(q->name, q->addr_preference, &host);
	/*{
		int i;
		for (i = 0; i < sizeof(struct lookup_result); i++) {
			if (i == 1) sleep(1);
			hard_write(h, (unsigned char *)&host + i, 1);
		}
	}*/
	hard_write(h, (unsigned char *)&host, sizeof(struct lookup_result));
}

static void end_real_lookup(struct dnsquery *q)
{
	int r = 1;
	int rs;
	if (!q->addr || hard_read(q->h, (unsigned char *)q->addr, sizeof(struct lookup_result)) != sizeof(struct lookup_result) || !q->addr->n) goto end;
	r = 0;

	end:
	set_handlers(q->h, NULL, NULL, NULL);
	EINTRLOOP(rs, close(q->h));
	end_dns_lookup(q, r);
}
#endif

static int do_lookup(struct dnsquery *q, int force_async)
{
	/*debug("starting lookup for %s", q->name);*/
#ifndef NO_ASYNC_LOOKUP
	if (!async_lookup && !force_async) {
#endif
#ifndef NO_ASYNC_LOOKUP
		sync_lookup:
#endif
		do_real_lookup(q->name, q->addr_preference, q->addr);
		end_dns_lookup(q, !q->addr->n);
		return 0;
#ifndef NO_ASYNC_LOOKUP
	} else {
		q->h = start_thread((void (*)(void *, int))lookup_fn, q, (int)((unsigned char *)strchr(cast_const_char q->name, 0) + 1 - (unsigned char *)q), 1);
		if (q->h == -1) goto sync_lookup;
		set_handlers(q->h, (void (*)(void *))end_real_lookup, NULL, q);
		return 1;
	}
#endif
}

static int do_queued_lookup(struct dnsquery *q)
{
#ifndef THREAD_SAFE_LOOKUP
	q->next_in_queue = NULL;
	if (!dns_queue) {
		dns_queue = q;
		/*debug("direct lookup");*/
#endif
		return do_lookup(q, 0);
#ifndef THREAD_SAFE_LOOKUP
	} else {
		/*debug("queuing lookup for %s", q->name);*/
		if (dns_queue->next_in_queue) internal("DNS queue corrupted");
		dns_queue->next_in_queue = q;
		dns_queue = q;
		return 1;
	}
#endif
}

static void check_dns_cache_addr_preference(void)
{
	if (dns_cache_addr_preference != ipv6_options.addr_preference) {
		shrink_dns_cache(SH_FREE_ALL);
		dns_cache_addr_preference = ipv6_options.addr_preference;
	}
}

static int find_in_dns_cache(unsigned char *name, struct dnsentry **dnsentry)
{
	struct dnsentry *e;
	check_dns_cache_addr_preference();
	foreach(e, dns_cache)
		if (!strcasecmp(cast_const_char e->name, cast_const_char name)) {
			del_from_list(e);
			add_to_list(dns_cache, e);
			*dnsentry = e;
			return 0;
		}
	return -1;
}

static void end_dns_lookup(struct dnsquery *q, int a)
{
	struct dnsentry *dnsentry;
	void (*fn)(void *, int);
	void *data;
	/*debug("end lookup %s", q->name);*/
#ifndef THREAD_SAFE_LOOKUP
	if (q->next_in_queue) {
		/*debug("processing next in queue: %s", q->next_in_queue->name);*/
		do_lookup(q->next_in_queue, 1);
	} else dns_queue = NULL;
#endif
	if (!q->fn || !q->addr) {
		free(q);
		return;
	}
	if (!find_in_dns_cache(q->name, &dnsentry)) {
		if (a) {
			memcpy(q->addr, &dnsentry->addr, sizeof(struct lookup_result));
			a = 0;
			goto e;
		}
		del_from_list(dnsentry);
		mem_free(dnsentry);
	}
	if (a) goto e;
	if (q->addr_preference != ipv6_options.addr_preference) goto e;
	check_dns_cache_addr_preference();
	dnsentry = mem_alloc(sizeof(struct dnsentry) + strlen(cast_const_char q->name) + 1);
	strcpy(cast_char dnsentry->name, cast_const_char q->name);
	memcpy(&dnsentry->addr, q->addr, sizeof(struct lookup_result));
	dnsentry->get_time = get_time();
	add_to_list(dns_cache, dnsentry);
	e:
	if (q->s) *q->s = NULL;
	fn = q->fn;
	data = q->data;
	free(q);
	fn(data, a);
}

int find_host_no_cache(unsigned char *name, struct lookup_result *addr, void **qp, void (*fn)(void *, int), void *data)
{
	struct dnsquery *q;
	retry:
	q = (struct dnsquery *)malloc(sizeof(struct dnsquery) + strlen(cast_const_char name));
	if (!q) {
		if (out_of_memory(0, NULL, 0))
			goto retry;
		fn(data, 1);
		return 0;
	}
	q->fn = fn;
	q->data = data;
	q->s = (struct dnsquery **)qp;
	q->addr = addr;
	q->addr_preference = ipv6_options.addr_preference;
	strcpy(cast_char q->name, cast_const_char name);
	if (qp) *(struct dnsquery **) qp = q;
	return do_queued_lookup(q);
}

int find_host(unsigned char *name, struct lookup_result *addr, void **qp, void (*fn)(void *, int), void *data)
{
	struct dnsentry *dnsentry;
	if (qp) *qp = NULL;
	if (!find_in_dns_cache(name, &dnsentry)) {
		if ((uttime)get_time() - (uttime)dnsentry->get_time > DNS_TIMEOUT) goto timeout;
		memcpy(addr, &dnsentry->addr, sizeof(struct lookup_result));
		fn(data, 0);
		return 0;
	}
	timeout:
	return find_host_no_cache(name, addr, qp, fn, data);
}

void kill_dns_request(void **qp)
{
	struct dnsquery *q = *qp;
	q->fn = NULL;
	q->addr = NULL;
	*qp = NULL;
}

void dns_set_priority(unsigned char *name, struct host_address *address, int prefer)
{
	int i;
	struct dnsentry *dnsentry;
	if (find_in_dns_cache(name, &dnsentry))
		return;
	for (i = 0; i < dnsentry->addr.n; i++)
		if (!memcmp(&dnsentry->addr.a[i], address, sizeof(struct host_address))) goto found_it;
	return;
	found_it:
	if (prefer) {
		memmove(&dnsentry->addr.a[1], &dnsentry->addr.a[0], i * sizeof(struct host_address));
		memcpy(&dnsentry->addr.a[0], address, sizeof(struct host_address));
	} else {
		memmove(&dnsentry->addr.a[i], &dnsentry->addr.a[i + 1], (dnsentry->addr.n - i - 1) * sizeof(struct host_address));
		memcpy(&dnsentry->addr.a[dnsentry->addr.n - 1], address, sizeof(struct host_address));
	}
}

unsigned long dns_info(int type)
{
	switch (type) {
		case CI_FILES: {
			unsigned long n = 0;
			struct dnsentry *e;
			foreach(e, dns_cache) n++;
			return n;
		}
		default:
			internal("dns_info: bad request");
	}
	return 0;
}

static int shrink_dns_cache(int u)
{
	struct dnsentry *d, *e;
	int f = 0;
	if (u == SH_FREE_SOMETHING && !list_empty(dns_cache)) {
		d = dns_cache.prev;
		goto free_e;
	}
	foreach(d, dns_cache) if (u == SH_FREE_ALL || (uttime)get_time() - (uttime)d->get_time > DNS_TIMEOUT) {
		free_e:
		e = d;
		d = d->prev;
		del_from_list(e);
		mem_free(e);
		f = ST_SOMETHING_FREED;
	}
	return f | (list_empty(dns_cache) ? ST_CACHE_EMPTY : 0);
}

int ipv6_full_access(void)
{
#ifdef SUPPORT_IPV6
	/*
	 * Test if we can access global IPv6 address space.
	 * This doesn't send anything anywhere, it just creates an UDP socket,
	 * connects it and closes it.
	 */
	struct sockaddr_in6 sin6;
	int h, c, rs;
	if (!support_ipv6) return 0;
	h = c_socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
	if (h == -1) return 0;
	memset(&sin6, 0, sizeof sin6);
	sin6.sin6_family = AF_INET6;
	sin6.sin6_port = htons(1024);
	memcpy(&sin6.sin6_addr.s6_addr, "\052\001\004\060\000\015\000\000\002\314\236\377\376\044\176\032", 16);
	EINTRLOOP(c, connect(h, (struct sockaddr *)(void *)&sin6, sizeof sin6));
	EINTRLOOP(rs, close(h));
	if (!c) return 1;
#endif
	return 0;
}

#ifdef FLOOD_MEMORY

void flood_memory(void)
{
	struct list_head list;
	size_t s = 32768 * 32;
	struct dnsentry *de;
	dns_cache_addr_preference = ipv6_options.addr_preference;
#if defined(HAVE__HEAPMIN)
	_heapmin();
#endif
	init_list(list);
	while (!list_empty(dns_cache)) {
		de = (struct dnsentry *)dns_cache.prev;
		del_from_list(de);
		add_to_list(list, de);
	}
	while (1) {
		while ((de = mem_alloc_mayfail(s))) {
			de->get_time = get_time();
			memset(&de->addr, 0, sizeof de->addr);
			de->name[0] = 0;
			add_to_list(list, de);
		}
		if (s == sizeof(struct dnsentry)) break;
		s = s / 2;
		if (s < sizeof(struct dnsentry)) s = sizeof(struct dnsentry);
	}
	while (!list_empty(list)) {
		de = (struct dnsentry *)list.prev;
		del_from_list(de);
		add_to_list(dns_cache, de);
	}
}

#endif

void init_dns(void)
{
	register_cache_upcall(shrink_dns_cache, 0, cast_uchar "dns");
#ifdef FLOOD_MEMORY
	flood_memory();
#endif
#ifdef SUPPORT_IPV6
	{
		int h, rs;
		h = c_socket(AF_INET6, SOCK_STREAM, 0);
		if (h == -1) {
			support_ipv6 = 0;
		} else {
			EINTRLOOP(rs, close(h));
			support_ipv6 = 1;
		}
	}
#endif
}
