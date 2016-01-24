/* https.c
 * HTTPS protocol client implementation
 * (c) 2002 Mikulas Patocka
 * This file is a part of the Links program, released under GPL.

 * In addition, as a special exception, the copyright holders give
 * permission to link the code of portions of this program with the
 * OpenSSL library under certain conditions as described in each
 * individual source file, and distribute linked combinations
 * including the two.
 * You must obey the GNU General Public License in all respects
 * for all of the code used other than OpenSSL.  If you modify
 * file(s) with this exception, you may extend this exception to your
 * version of the file(s), but you are not obligated to do so.  If you
 * do not wish to do so, delete this exception statement from your
 * version.  If you delete this exception statement from all source
 * files in the program, then also delete it here.
 */

#include "links.h"

#ifndef PATH_MAX
#define PATH_MAX 255
#endif

#ifdef HAVE_SSL

#ifndef LINKS_CRT_FILE
#define LINKS_CRT_FILE		links.crt
#endif

static SSL_CTX *context = NULL;

#if defined(HAVE_SSL_CERTIFICATES) && (defined(DOS) || defined(OS2) || defined(WIN) || defined(OPENVMS))
static int ssl_set_private_paths(void)
{
	unsigned char *path, *c;
	int r;
#ifdef OPENVMS
	path = stracpy(g_argv[0]);
#else
	path = stracpy(path_to_exe);
#endif
	for (c = path + strlen(cast_const_char path); c > path; c--) {
		if (dir_sep(c[-1])
#ifdef OPENVMS
		    || c[-1] == ']' || c[-1] == ':'
#endif
		    )
			break;
	}
	c[0] = 0;
	add_to_strn(&path, cast_uchar stringify(LINKS_CRT_FILE));
	r = SSL_CTX_load_verify_locations(context, cast_const_char path, NULL);
	mem_free(path);
	if (r != 1)
		return -1;
	return 0;
}
#else
#define ssl_set_private_paths()		(-1)
#endif

int ssl_asked_for_password;

static int ssl_password_callback(char *buf, int size, int rwflag, void *userdata)
{
	ssl_asked_for_password = 1;
	if (size > (int)strlen(cast_const_char ssl_options.client_cert_password))
		size = (int)strlen(cast_const_char ssl_options.client_cert_password);
	memcpy(buf, ssl_options.client_cert_password, size);
	return size;
}

SSL *getSSL(void)
{
	if (!context) {
		const SSL_METHOD *m;
		unsigned char f_randfile[PATH_MAX];
		unsigned char *os_pool;
		int os_pool_size;

#if defined(HAVE_RAND_EGD) && defined(HAVE_RAND_FILE_NAME) && defined(HAVE_RAND_LOAD_FILE) && defined(HAVE_RAND_WRITE_FILE)
		const unsigned char *f = (const unsigned char *)RAND_file_name(cast_char f_randfile, sizeof(f_randfile));
		if (f && RAND_egd(cast_const_char f) < 0) {
			/* Not an EGD, so read and write to it */
			if (RAND_load_file(cast_const_char f_randfile, -1))
				RAND_write_file(cast_const_char f_randfile);
		}
#endif

#if defined(HAVE_RAND_ADD)
		os_seed_random(&os_pool, &os_pool_size);
		if (os_pool_size) RAND_add(os_pool, os_pool_size, os_pool_size);
		mem_free(os_pool);
#endif

		SSLeay_add_ssl_algorithms();
		m = SSLv23_client_method();
		if (!m) return NULL;
		context = SSL_CTX_new((void *)m);
		if (!context) return NULL;
		SSL_CTX_set_options(context, SSL_OP_ALL);
		if (ssl_set_private_paths())
			SSL_CTX_set_default_verify_paths(context);
		SSL_CTX_set_default_passwd_cb(context, ssl_password_callback);

	}
	return SSL_new(context);
}

void ssl_finish(void)
{
	if (context) {
		SSL_CTX_free(context);
		context = NULL;
	}
}

void https_func(struct connection *c)
{
	c->ssl = DUMMY;
	http_func(c);
}

#ifdef HAVE_SSL_CERTIFICATES

static int check_host_name(const unsigned char *templ, const unsigned char *host)
{
	int templ_len = (int)strlen(cast_const_char templ);
	int host_len = (int)strlen(cast_const_char host);
	unsigned char *wildcard;

	if (templ_len > 0 && templ[templ_len - 1] == '.') templ_len--;
	if (host_len > 0 && host[host_len - 1] == '.') host_len--;

	wildcard = memchr(templ, '*', templ_len);
	if (!wildcard) {
		if (templ_len == host_len && !casecmp(templ, host, templ_len))
			return 0;
		return -1;
	} else {
		int prefix_len, suffix_len;
		if (templ_len > host_len)
			return -1;
		prefix_len = (int)(wildcard - templ);
		suffix_len = (int)(templ + templ_len - (wildcard + 1));
		if (memchr(templ, '.', prefix_len))
			return -1;
		if (memchr(wildcard + 1, '*', suffix_len))
			return -1;
		if (casecmp(host, templ, prefix_len))
			return -1;
		if (memchr(host + prefix_len, '.', host_len - prefix_len - suffix_len))
			return -1;
		if (casecmp(host + host_len - suffix_len, wildcard + 1, suffix_len))
			return -1;
		return 0;
	}
}

/*
 * This function is based on verifyhost in libcurl - I hope that it is correct.
 */
static int verify_ssl_host_name(X509 *server_cert, unsigned char *host)
{
	unsigned char ipv4_address[4];
#ifdef SUPPORT_IPV6
	unsigned char ipv6_address[16];
#endif
	unsigned char *address = NULL;
	int address_len = 0;
	int type = GEN_DNS;

	STACK_OF(GENERAL_NAME) *altnames;

	if (!numeric_ip_address(host, ipv4_address)) {
		address = ipv4_address;
		address_len = 4;
		type = GEN_IPADD;
	}
#ifdef SUPPORT_IPV6
	if (!numeric_ipv6_address(host, ipv6_address, NULL)) {
		address = ipv6_address;
		address_len = 16;
		type = GEN_IPADD;
	}
#endif

#if 1
	altnames = X509_get_ext_d2i(server_cert, NID_subject_alt_name, NULL, NULL);
	if (altnames) {
		int retval = 1;
		int i;
		int n_altnames = sk_GENERAL_NAME_num(altnames);
		for (i = 0; i < n_altnames; i++) {
			const GENERAL_NAME *altname = sk_GENERAL_NAME_value(altnames, i);
			const unsigned char *altname_ptr;
			int altname_len;
			if (altname->type != type) {
				if (altname->type == GEN_IPADD || altname->type == GEN_DNS || altname->type == GEN_URI)
					retval = S_INVALID_CERTIFICATE;
				continue;
			}
			altname_ptr = ASN1_STRING_data(altname->d.ia5);
			altname_len = ASN1_STRING_length(altname->d.ia5);
			if (type == GEN_IPADD) {
				if (altname_len == address_len && !memcmp(altname_ptr, address, address_len)) {
					retval = 0;
					break;
				}
			} else {
				if (altname_len == (int)strlen(cast_const_char altname_ptr) && !check_host_name(altname_ptr, host)) {
					retval = 0;
					break;
				}
			}
			retval = S_INVALID_CERTIFICATE;
		}
		sk_GENERAL_NAME_free(altnames);
		if (retval != 1)
			return retval;
	}
#endif

	{
		unsigned char *nulstr = cast_uchar "";
		unsigned char *peer_CN = nulstr;
		X509_NAME *name;
		int j, i = -1;
	
		retval = 1;

		name = X509_get_subject_name(server_cert);
		if (name)
			while ((j = X509_NAME_get_index_by_NID(name, NID_commonName, i)) >= 0)
				i = j;
		if (i >= 0) {
			ASN1_STRING *tmp = X509_NAME_ENTRY_get_data(X509_NAME_get_entry(name, i));
			if (tmp) {
				if (ASN1_STRING_type(tmp) == V_ASN1_UTF8STRING) {
					j = ASN1_STRING_length(tmp);
					if (j >= 0) {
						peer_CN = OPENSSL_malloc(j + 1);
						if (peer_CN) {
							memcpy(peer_CN, ASN1_STRING_data(tmp), j);
							peer_CN[j] = '\0';
						}
					}
				} else {
					j = ASN1_STRING_to_UTF8(&peer_CN, tmp);
				}
				if (peer_CN && (int)strlen(cast_const_char peer_CN) != j) {
					retval = S_INVALID_CERTIFICATE;
				}
			}
		}
		if (peer_CN && peer_CN != nulstr) {
			if (retval == 1 && !check_host_name(peer_CN, host))
				retval = 0;
			OPENSSL_free(peer_CN);
		}
		if (retval != 1)
			return retval;
	}

	return S_INVALID_CERTIFICATE;
}

int verify_ssl_certificate(SSL *ssl, unsigned char *host)
{
	X509 *server_cert;
	int ret;

	if (SSL_get_verify_result(ssl) != X509_V_OK)
		return S_INVALID_CERTIFICATE;
	server_cert = SSL_get_peer_certificate(ssl);
	if (!server_cert)
		return S_INVALID_CERTIFICATE;
	ret = verify_ssl_host_name(server_cert, host);
	X509_free(server_cert);
	return ret;
}

int verify_ssl_cipher(SSL *ssl)
{
	const SSL_METHOD *meth = SSL_get_ssl_method(ssl);
	unsigned char *cipher;
	if (
#ifndef OPENSSL_NO_SSL2
	    meth == SSLv2_client_method() ||
#endif
#ifndef OPENSSL_NO_SSL3_METHOD
	    meth == SSLv3_client_method() ||
#endif

	    0) {
		return S_INSECURE_CIPHER;
	}
	if (SSL_get_cipher_bits(ssl, NULL) < 112)
		return S_INSECURE_CIPHER;
	cipher = cast_uchar SSL_get_cipher_name(ssl);
	if (strstr(cast_const_char cipher, "RC4-"))
		return S_INSECURE_CIPHER;
	return 0;
}

#endif

#else

void https_func(struct connection *c)
{
	setcstate(c, S_NO_SSL);
	abort_connection(c);
}

#endif
