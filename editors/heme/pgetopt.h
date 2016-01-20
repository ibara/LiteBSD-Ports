#ifndef PGETOPT_H
#define PGETOPT_H

extern const char *poptarg;
extern int poptindex;

int pgetopt(int argc, char **argv, const char *opt_spec);

#endif	/* PGETOPT_H */
