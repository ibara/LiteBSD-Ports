/*  _ __   __ _ ___ __  __
** | '_ \ / _` |_ _|  \/  | naim
** | | | | (_| || || |\/| | Copyright 1998-2003 Daniel Reed <n@ml.org>
** |_| |_|\__,_|___|_|  |_| ncurses-based chat client
*/
#include <ctype.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

int	aimcmp(const unsigned char *sn1, const unsigned char *sn2) {
	register const unsigned char
		*s1 = sn1, *s2 = sn2;

	if ((s1 == NULL) || (s2 == NULL))
		return(1);

	while ((*s1 != 0) && (*s2 != 0)) {
		while (isspace(*s1))
			s1++;
		while (isspace(*s2))
			s2++;
		if (toupper(*s1) > toupper(*s2))
			return(1);
		else if (toupper(*s1) < toupper(*s2))
			return(-1);
		s1++;
		s2++;
	}
	while (isspace(*s1))
		s1++;
	while (isspace(*s2))
		s2++;
	if (*s1 != 0)
		return(-1);
	else if (*s2 != 0)
		return(1);
	return(0);
}

int	aimncmp(const unsigned char *sn1, const unsigned char *sn2, int len) {
	register const unsigned char
		*s1 = sn1, *s2 = sn2;

	if ((s1 == NULL) || (s2 == NULL))
		return(1);

	while ((*s1 != 0) && (*s2 != 0) && (len > 0)) {
		len--;
		while (isspace(*s1))
			s1++;
		while (isspace(*s2))
			s2++;
		if (toupper(*s1) > toupper(*s2))
			return(1);
		else if (toupper(*s1) < toupper(*s2))
			return(-1);
		s1++;
		s2++;
	}
	if (len == 0)
		return(0);
	while (isspace(*s1))
		s1++;
	while (isspace(*s2))
		s2++;
	if (*s1 != 0)
		return(-1);
	else if (*s2 != 0)
		return(1);
	return(0);
}

const char
	*dtime(double _t) {
	static char
		buf[sizeof("365d 23:59:59")];
	unsigned long
		t = (unsigned long)_t;

	if (_t < 0) {
		snprintf(buf, sizeof(buf), "(error)");
		return(buf);
	}

	if (t < 2) {
		unsigned long
			mt = (unsigned long)(100*(_t - t));

		snprintf(buf, sizeof(buf), "%lu.%02lus", t, mt);
	} else if (t < 10) {
		unsigned long
			mt = (unsigned long)(10*(_t - t));

		snprintf(buf, sizeof(buf), "%lu.%01lus", t, mt);
	} else if (t < 90)
		snprintf(buf, sizeof(buf), "%lus", t);
	else if (t < 60*60)
		snprintf(buf, sizeof(buf), "%lum", t/60);
	else if (t < 24*60*60)
		snprintf(buf, sizeof(buf), "%lu:%02lum", t/(60*60), 
			(t/60)%60);
	else if (t < 365*24*60*60)
		snprintf(buf, sizeof(buf), "%lud %lu:%02lum", 
			t/(24*60*60), t/(60*60)%24, t/60%60);
	else
		snprintf(buf, sizeof(buf), "%luy %lud", t/(365*24*60*60),
			t/(24*60*60)%365);

	return(buf);
}

const char
	*dsize(double b) {
	static char
		buf[sizeof("9.99 &nbsp;B")];
	const char
		*suf;
	int	dig;
	unsigned long
		bint;

	if (b >= 1000.*1024.*1024.*1024.) {
		suf = "TB";
		dig = 100;
		b /= (1024.*1024.*1024.*1024.);
	} else if (b >= 100.*1024.*1024.*1024.) {
		suf = "GB";
		dig = 0;
		b /= (1024.*1024.*1024.);
	} else if (b >= 10.*1024.*1024.*1024.) {
		suf = "GB";
		dig = 10;
		b /= (1024.*1024.*1024.);
	} else if (b >= 1000.*1024.*1024.) {
		suf = "GB";
		dig = 100;
		b /= (1024.*1024.*1024.);
	} else if (b >= 100.*1024.*1024.) {
		suf = "MB";
		dig = 0;
		b /= (1024.*1024.);
	} else if (b >= 10.*1024.*1024.) {
		suf = "MB";
		dig = 10;
		b /= (1024.*1024.);
	} else if (b >= 1000.*1024.) {
		suf = "MB";
		dig = 100;
		b /= (1024.*1024.);
	} else if (b >= 100.*1024.) {
		suf = "kB";
		dig = 0;
		b /= 1024.;
	} else if (b >= 10.*1024.) {
		suf = "kB";
		dig = 10;
		b /= 1024.;
	} else if (b >= 1000.) {
		suf = "kB";
		dig = 100;
		b /= 1024.;
	} else {
		suf = "&nbsp;B";
		dig = 0;
	}

	bint = (unsigned long)b;
	if (dig == 0)
		snprintf(buf, sizeof(buf), "%lu %s", bint, suf);
	else
		snprintf(buf, sizeof(buf), "%lu.%lu %s", bint, (unsigned long)(dig*(b-bint)), suf);
	return(buf);
}

void	htmlstrip(char *bb) {
	char	*start, *end;

	while ((start = strchr(bb, '<')) != NULL)
		if ((end = strchr(start, '>')) != NULL)
			memmove(start, end+1, strlen(end));
		else
			break;
	end = bb;
	while ((start = strchr(end, '&')) != NULL)
		if ((end = strchr(start, ';')) != NULL) {
			if (strncasecmp(start+1, "nbsp;", 5) == 0)
				*start = ' ';
			else if (strncasecmp(start+1, "amp;", 4) == 0)
				*start = '&';
			else if (strncasecmp(start+1, "gt;", 3) == 0)
				*start = '>';
			else if (strncasecmp(start+1, "lt;", 3) == 0)
				*start = '<';
			else if (strncasecmp(start+1, "quot;", 5) == 0)
				*start = '"';
			else
				continue;
			memmove(start+1, end+1, strlen(end));
			end = start+1;
		} else
			break;
}

void	htmlreplace(char *bb, char what) {
	char	*start, *end;

	while ((start = strchr(bb, '<')) != NULL)
		if ((end = strchr(start, '>')) != NULL) {
			char	*tmp;

			for (tmp = start; tmp <= end; tmp++)
				*tmp = what;
		} else
			break;
	end = bb;
}
