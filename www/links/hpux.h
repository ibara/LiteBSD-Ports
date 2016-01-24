int hp_accept(int s, struct sockaddr *addr, socklen_t *len);
int hp_getpeername(int s, struct sockaddr *addr, socklen_t *len);
int hp_getsockname(int s, struct sockaddr *addr, socklen_t *len);
int hp_getsockopt(int s, int level, int opt, void *val, socklen_t *len);

void fatal_exit(char *, ...);
