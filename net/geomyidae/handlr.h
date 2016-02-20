/*
 * Copy me if you can.
 * by 20h
 */

#ifndef HANDLR_H
#define HANDLR_H

void handledir(int sock, char *path, char *port, char *base, char *args,
			char *sear, char *ohost);
void handlegph(int sock, char *file, char *port, char *base, char *args,
			char *sear, char *ohost);
void handlebin(int sock, char *file, char *port, char *base, char *args,
			char *sear, char *ohost);
void handlecgi(int sock, char *file, char *port, char *base, char *args,
			char *sear, char *ohost);
void handledcgi(int sock, char *file, char *port, char *base, char *args,
			char *sear, char *ohost);

#endif
