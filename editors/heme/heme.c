#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <pwd.h>
#include <curses.h>

#ifdef HAVE_MMAP
# include <sys/mman.h>
#endif

#include "xmalloc.h"
#include "pconfig.h"
#include "pgetopt.h"

/* this determines maximum memory that will be
 * available for blocks (default: 2 MB) */
#define MAX_BLOCKMEM	(2 * (1 << 20))	

static const char heme_version[] = "0.4.2";

typedef unsigned char byte;
static WINDOW *w_main, *w_offset, *w_ascii;
static int max_lines;
static off_t cur_offset, beg_offset, end_offset, file_size;
static int mode_ascii;
static int fd;
static int file_saved = 1;
static int cur_line;
static char *file_name;
static char *prog_name;
static const char *config_file;
static int current_nibble;

static int last_search = -1; /* 0 = byte search, 1 = string search */
static int last_byte_search = 0;
static char *last_string_search = NULL;

static off_t off_len;

static int bpl;	/* bytes per line */

/* undo struct used by put_byte */
typedef struct {
	off_t offset1, offset2;
	byte b;
	byte *fill_bytes;
}
undo_t;

static undo_t *undo_list;
static int undo_count;

typedef struct {
	byte *buf;
	off_t off_start, off_len;
	int modified;
}
blist_t;

static blist_t *blist;
static int blist_count;
static int blist_current;	/* index of current block */

static int use_colors = -1;
static void save_undo_info(off_t offset, byte b);

static int make_backup_files = -1;
static int advance_after_edit = -1;

static void advance_right();

/* reads file into buf, max n bytes, returns 0 on EOF, < 0 on error */
static ssize_t read_file(int fds, byte *buf, size_t n)
{
	ssize_t i;
	size_t sum = 0;

	while(sum < n)
	{
		i = read(fds, buf, n);
		if(i < 0)
			return i;
		else if(i == 0)
			return sum;
		sum += i;
	}

	return sum;
}

static void write_block(int i, int fds)
{
	off_t len;

	/* error checking ??? */
	lseek(fds, blist[i].off_start, SEEK_SET);

	/* check for eof */
	len = blist[i].off_len;
	if(blist[i].off_start + len > file_size)
		len = file_size - blist[i].off_start;

	write(fds, blist[i].buf, len);
}

static void save_file(int do_munmap)
{
	int i;

	if(make_backup_files)
	{
		int backup_fd;
		ssize_t r;
		byte *tmp;
		char *backup_name;

		backup_name = xmalloc(strlen(file_name) + 2);
		sprintf(backup_name, "%s~", file_name);
		backup_fd = open(backup_name, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR |
						 S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
		tmp = xmalloc(off_len);
		lseek(fd, 0, SEEK_SET);
		while((r = read_file(fd, tmp, off_len)) > 0)
			write(backup_fd, tmp, r);
		free(tmp);
		close(backup_fd);
		free(backup_name);
	}

	/* save all modified blocks */
	for(i = 0; i < blist_count; i++)
	{
		if(blist[i].modified)
		{
			write_block(i, fd);

#ifdef HAVE_MMAP
			if(do_munmap)
				munmap(blist[i].buf, blist[i].off_len);
#endif
			blist[i].modified = 0;
		}
	}
	file_saved = 1;
}

static int find_block(off_t offset)
{
	int i;

	for(i = 0; i < blist_count; i++)
	{
		if(blist[i].off_start <= offset && offset < blist[i].off_start + blist[i].off_len)
			return i;
	}
	return -1;
}

static void check_max_blocks()
{
	int i;

	/* check if we reached max. number of blocks */
	if((blist_count + 1) * off_len >= MAX_BLOCKMEM)
	{
		/* first attempt to free unmodified blocks */
		for(i = 0; i < blist_count; i++)
		{
			if(blist[i].modified)
				continue;

			if(blist[i].off_start <= cur_offset && cur_offset < blist[i].off_start + blist[i].off_len)
				continue;	/* ckip current block */

#ifdef HAVE_MMAP
			munmap(blist[i].buf, blist[i].off_len);
#else
			free(blist[i].buf);
#endif
			/* hack: move end block to this one's position in the list */
			if(i != blist_count - 1)
				blist[i] = blist[blist_count - 1];
			blist_count--;
			blist = xrealloc(blist, blist_count * sizeof(blist_t));
			return;
		}

		/* if no blocks are unmodified (very unlikely for such large files, btw)
		 * remove the first one */
		if((blist_count + 1) * off_len >= MAX_BLOCKMEM)
		{
			write_block(0, fd);

#ifdef HAVE_MMAP
			munmap(blist[0].buf, blist[0].off_len);
#else
			free(blist[0].buf);
#endif
			/* hack: move end block to this one's position in the list */
			if(blist_count != 1)
				blist[0] = blist[blist_count - 1];
			blist_count--;
			blist = xrealloc(blist, blist_count * sizeof(blist_t));
		}
	}
}

static int load_block(off_t offset)
{
	byte *buf;
	off_t off_start;

	/* we must allocate another block, check if we can */
	check_max_blocks();

	/* align on the block size */
	for(off_start = offset; off_start % off_len != 0; off_start--)
		;

#ifndef HAVE_MMAP
	lseek(fd, off_start, SEEK_SET);

	buf = xmalloc(off_len);	/* with mmap this isn't needed */

	if(read_file(fd, buf, off_len) < 0)
	{
		fprintf(stderr, "%s: read_file() failed, something's gone wrong", prog_name);
		exit(1);
	}
#else
	buf = mmap(0, off_len, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, off_start);
	if(buf == MAP_FAILED)
	{
		fprintf(stderr, "%s: mmap() failed", prog_name);
		exit(1);
	}
#endif

	/* add this block to the list */
	blist = xrealloc(blist, sizeof(blist_t) * (blist_count + 1));
	blist[blist_count].buf = buf;
	blist[blist_count].off_start = off_start;
	blist[blist_count].off_len = off_len;
	blist[blist_count].modified = 0;
	return blist_count++;
}

static void open_file(const char *name)
{
	struct stat st;

	fd = open(name, O_RDWR);
	if(fd < 0)
	{
		fprintf(stderr, "%s: open: %s\n", prog_name, strerror(errno));
		exit(1);
	}

	if(fstat(fd, &st) < 0)
	{
		fprintf(stderr, "%s: fstat: %s\n", prog_name, strerror(errno));
		close(fd);
		exit(1);
	}

	file_name = xstrdup(name);
	file_size = st.st_size;
	if(file_size == 0)
	{
		close(fd);
		fprintf(stderr, "%s: file size of `%s' is 0\n", prog_name, name);
		exit(0);
	}

#ifndef HAVE_MMAP
	off_len = st.st_blksize;
#endif

	load_block(0);
}

static void unhilite()
{
	mvwchgat(w_main, cur_line, (3 * (cur_offset % bpl)), 3,
			A_NORMAL, use_colors, NULL);
	mvwchgat(w_ascii, cur_line, cur_offset % bpl, 3,
			A_NORMAL, use_colors, NULL);
	touchwin(w_main);
	touchwin(w_ascii);
}

static void hilite()
{
	if(use_colors)
	{
		mvwchgat(w_main, cur_line, (3 * (cur_offset % bpl)) + current_nibble, 1,
				A_NORMAL, 2, NULL);
		mvwchgat(w_ascii, cur_line, cur_offset % bpl, 1,
				A_NORMAL, 2, NULL);
	}
	else
	{
		mvwchgat(w_main, cur_line, 3 * (cur_offset % bpl), 2,
				A_STANDOUT, 0, NULL);
		mvwchgat(w_ascii, cur_line, cur_offset % bpl, 1, 
				A_BLINK, 0, NULL);
	}
	touchwin(w_main);
	touchwin(w_ascii);
}

static byte get_byte(off_t offset)
{
	int i;

	i = blist_current;
	if(blist[i].off_start > offset || offset >= blist[i].off_start + blist[i].off_len)
	{
		/* if it isn't in the current block, search already mapped area */
		if((i = find_block(offset)) < 0)
			i = load_block(offset);
		blist_current = i;
	}

	return blist[i].buf[offset - blist[i].off_start];
}

static void put_byte(off_t offset, byte b)
{
	int i;

	/* search already mapped area */
	if((i = find_block(offset)) < 0)
		i = load_block(offset);

	blist[i].buf[offset - blist[i].off_start] = b;
	blist[i].modified = 1;
	file_saved = 0;
}

static int printable(byte *c)
{
	if(*c > 127 || !isprint(*c))
	{
		*c = '.';
		return 0;
	}
	return 1;
}

/* on line number 'line' print 'bpl' bytes starting from 'offset' */
static void put_line(int line, off_t offset)
{
	int i;
	byte b;

	/* print offset */
	mvwprintw(w_offset, line, 0, "%08X:", offset);
	mvwchgat(w_offset, line, 9, 4, A_NORMAL, 1, NULL);

	wmove(w_main, line, 0);
	wclrtoeol(w_main);

	wmove(w_ascii, line, 0);
	wclrtoeol(w_ascii);

	for(i = 0; i < bpl && offset < file_size; i++, offset++)
	{
		b = get_byte(offset);
		mvwprintw(w_main, line, i * 3, "%02X ", b);

		printable(&b);
		mvwprintw(w_ascii, line, i, "%c", b);
	}

	/* this is ugly */
	{
		int y,x;
		getmaxyx(w_main,y,x);
		mvwchgat(w_main, line, i, x - i, A_NORMAL, 1, NULL);
	}
	while(i <= bpl)
	{
		mvwchgat(w_ascii, line, i, 1, A_NORMAL, 1, NULL);
		i++;
	}
}

static void put_status()
{
	byte b;

	b = get_byte(cur_offset);

	mvprintw(0, 0, " File: %s%c  size = %lu bytes  %3d%%  [%c]", file_name, file_saved ? ' ' : '*',
			 file_size, beg_offset * 100 / ( end_offset + (end_offset == 0) ),
			 mode_ascii ? 'A' : 'H' );
	mvprintw(0, COLS - 19, "Press 'h' for help");

	mvprintw(LINES - 1, 0, " offset: 0x%-8X (%10d)  "
			 "char: %c\t hex = 0x%-02X dec = %-3d oct = %-8o",
			 cur_offset, cur_offset, printable(&b) ? b : '?', b, b, b
			 );
	clrtoeol();
	if(use_colors)
	{
		mvchgat(0, 0, -1, A_NORMAL, 3, NULL);
		mvchgat(LINES - 1, 0, -1, A_NORMAL, 3, NULL);
	}
	else
	{
		mvchgat(0, 0, -1, A_STANDOUT, 0, NULL);
		mvchgat(LINES - 1, 0, -1, A_STANDOUT, 0, NULL);
	}
}

/* called when in hex mode */
static void set_byte_part(byte b)
{
	byte old_byte;
	int shift;

	if(current_nibble == 1)
	{
		shift = 0;
	}
	else
	{
		shift = 4;
	}
	if(isdigit(b))
		b -= '0';
	else
		b = 10 + b - 'a';
	
	old_byte = get_byte(cur_offset);

	b = (b << shift) | (old_byte & (0xF << (4 - shift)));
	if(b == old_byte)
		return;

	save_undo_info(cur_offset, old_byte);
	put_byte(cur_offset, b);

	mvwprintw(w_main, cur_line, 3 * (cur_offset % bpl), "%02X", b);

	printable(&b);
	mvwprintw(w_ascii, cur_line, cur_offset % bpl, "%c", b);
	hilite();
}

/* called when in ascii mode */
static void set_byte(byte b)
{
	byte old_byte;
	
	old_byte = get_byte(cur_offset);
	if(b == old_byte)
		return;

	save_undo_info(cur_offset, old_byte);
	put_byte(cur_offset, b);

	mvwprintw(w_main, cur_line, 3 * (cur_offset % bpl), "%02X", b);

	printable(&b);
	mvwprintw(w_ascii, cur_line, cur_offset % bpl, "%c", b);
	hilite();
}

static void save_undo_info(off_t offset, byte b)
{
	int i;
	
	i = undo_count++;
	
	undo_list = xrealloc(undo_list, sizeof(undo_t) * undo_count);
	undo_list[i].offset1 = offset;
	undo_list[i].offset2 = 0;
	undo_list[i].b = b;

	file_saved = 0;
}

static void save_undo_fill_info(off_t offset1, off_t offset2)
{
	int i;
	off_t t;

	i = undo_count++;
	
	undo_list = xrealloc(undo_list, sizeof(undo_t) * undo_count);
	undo_list[i].offset1 = offset1;
	undo_list[i].offset2 = offset2;
	undo_list[i].fill_bytes = xmalloc(offset2 - offset1 + 1);

	for(t = 0; t <= offset2 - offset1; t++)
		undo_list[i].fill_bytes[t] = get_byte(offset1 + t);

	file_saved = 0;
}

static void do_undo()
{
	byte b;
	off_t offset;

	if(undo_count == 0)
		return;

	/* the difference between "fill" undo and normal undo is
	 * that the "fill" undo struct has offset2 != 0 */
	undo_count--;

	if(undo_list[undo_count].offset2 == 0)
	{
		offset = undo_list[undo_count].offset1;
		b = undo_list[undo_count].b;
		put_byte(offset, b);

		/* if this is in current view ... */
		if(offset >= beg_offset && offset < beg_offset + bpl * max_lines)
		{
			unhilite();
			cur_offset = offset;
			cur_line = (cur_offset - beg_offset) / bpl;

			mvwprintw(w_main, cur_line, 3 * (cur_offset % bpl), "%02X", b);
			printable(&b);
			mvwprintw(w_ascii, cur_line, cur_offset % bpl, "%c", b);
			hilite();
		}
	}
	else
	{
		undo_t fu;
		off_t t;

		fu = undo_list[undo_count];
		for(t = 0; t <= fu.offset2 - fu.offset1; t++)
			put_byte(fu.offset1 + t, fu.fill_bytes[t]);
		offset = undo_list[undo_count].offset1;

		/* update the view */
		if(offset >= beg_offset && offset < beg_offset + bpl * max_lines)
		{
			unhilite();
			t = 0;
			while(offset < beg_offset + bpl * max_lines && offset <= fu.offset2)
			{
				b = fu.fill_bytes[t];
				cur_offset = offset;
				cur_line = (cur_offset - beg_offset) / bpl;

				mvwprintw(w_main, cur_line, 3 * (cur_offset % bpl), "%02X", b);
				printable(&b);
				mvwprintw(w_ascii, cur_line, cur_offset % bpl, "%c", b);
				offset++;
				t++;
			}
			/* back to the last position */
			cur_offset -= t - 1;
			cur_line = (cur_offset - beg_offset) / bpl;

			hilite();
		}
		free(fu.fill_bytes);
	}
	
	undo_list = xrealloc(undo_list, sizeof(undo_t) * undo_count);
}

static int get_user_input(const char *prompt)
{
	mvprintw(LINES - 1, 1, "%s", prompt);
	clrtoeol();
	if(use_colors)
		mvchgat(LINES - 1, 0, -1, A_NORMAL, 3, NULL);
	else
		mvchgat(LINES - 1, 0, -1, A_STANDOUT, 0, NULL);
	wmove(stdscr, LINES - 1, strlen(prompt) + 1);
	wrefresh(stdscr);

	return tolower(getch());
}

static int ask_user_offset(const char *prompt, off_t *uoff)
{
	char tmp[30], *p;
	int base, t;
	off_t ret;

	mvprintw(LINES - 1, 1, "%s", prompt);
	clrtoeol();
	if(use_colors)
		mvchgat(LINES - 1, 0, -1, A_NORMAL, 3, NULL);
	else
	{
		mvchgat(LINES - 1, 0, -1, A_STANDOUT, 0, NULL);
		attron(A_STANDOUT);
	}
	wmove(stdscr, LINES - 1, strlen(prompt) + 1);
	wrefresh(stdscr);

	echo();
	getnstr(tmp, sizeof(tmp));
	noecho();
	if(use_colors == 0)
		attroff(A_STANDOUT);

	p = tmp;
	while(isspace(*p))
		p++;

	base = 10;
	if(*p == '0')
	{
		if(*(p + 1) == 'x' || *(p + 1) == 'X')
		{
			base = 16;
			p += 2;
		}
		else
			base = 8;
	}

	ret = 0;
	/* calc value */
	while(*p)
	{
		if(isdigit(*p))
			t = *p - '0';
		else if(isxdigit(*p) && base == 16)
			t = 10 + tolower(*p) - 'a';
		else
			return -1;

		ret *= base;
		ret += t;
		p++;
	}

	*uoff = ret;
	return 0;
}

static int ask_user_byte(const char *prompt, byte *bv)
{
	off_t ret;

	if(ask_user_offset(prompt, &ret) < 0 || ret > 255)
		return -1;

	*bv = (byte) ret;
	return 0;
}

static const char *ask_user_string(const char *prompt)
{
	static char buf[1024];

	mvprintw(LINES - 1, 1, "%s", prompt);
	clrtoeol();
	if(use_colors)
		mvchgat(LINES - 1, 0, -1, A_NORMAL, 3, NULL);
	else
	{
		mvchgat(LINES - 1, 0, -1, A_STANDOUT, 0, NULL);
		attron(A_STANDOUT);
	}
	wmove(stdscr, LINES - 1, strlen(prompt) + 1);
	wrefresh(stdscr);

	echo();
	getnstr(buf, sizeof(buf));
	noecho();
	if(use_colors == 0)
		attroff(A_STANDOUT);

	return buf;
}

static void print_message(const char *msg)
{
	mvprintw(LINES - 1, 1, "%s", msg);
	clrtoeol();
	if(use_colors)
		mvchgat(LINES - 1, 0, -1, A_NORMAL, 3, NULL);
	else
		mvchgat(LINES - 1, 0, -1, A_STANDOUT, 0, NULL);
	wmove(stdscr, LINES - 1, strlen(msg) + 1);
	wrefresh(stdscr);
}

static void string_search(const char *p)
{
	static off_t last_string_search_pos = -1;
	off_t tmp_offset;
	int i, len;
	char *s;
	
	len = strlen(p);
	s = xmalloc(len);
	
	tmp_offset = cur_offset;
	if(tmp_offset == last_string_search_pos)
		tmp_offset++;

	for(i = 0; i < len; i++)
		s[i] = get_byte(tmp_offset + i);
	
	while(tmp_offset + len < file_size)
	{
		if(memcmp(p, s, len) == 0)
		{
			unhilite();
			last_string_search_pos = cur_offset = tmp_offset;
			for(beg_offset = cur_offset; beg_offset % bpl != 0; beg_offset--)
				;
			if(beg_offset > end_offset)
				beg_offset = end_offset;
			cur_line = (cur_offset - beg_offset) / bpl;
			for(i = 0; i < max_lines; i++)
				put_line(i, beg_offset + i * bpl);
			hilite();
			break;
		}
		
		for(i = 0; i < len - 1; i++)
			s[i] = s[i + 1];
		s[i] = get_byte(tmp_offset + len);
		tmp_offset++;
	}
	free(s);
}

static void byte_search(byte b)
{
	off_t tmp_offset;
	int i;
	
	unhilite();
	
	tmp_offset = cur_offset;
	while(++cur_offset < file_size && get_byte(cur_offset) != b)
		;
	if(cur_offset == file_size)
	{

		/* not found */
		cur_offset = tmp_offset;
		hilite();
		return;
	}
	
	if(cur_offset >= beg_offset && cur_offset < beg_offset + bpl * max_lines)
	{
		/* byte is in the current view */
		cur_line = (cur_offset - beg_offset) / bpl;
	}
	else
	{
		/* refresh all lines */
		for(beg_offset = cur_offset; beg_offset % bpl != 0; beg_offset--)
			;
		cur_line = (cur_offset - beg_offset) / bpl;
		for(i = 0; i < max_lines; i++)
			put_line(i, beg_offset + i * bpl);
	}
	hilite();
}

static void show_helpwin()
{
	WINDOW *w_help;
	char format[] = "%-14s %s";

	w_help = newwin(LINES - 2, COLS, 1, 0);
	werase(w_help);

	mvwprintw(w_help, 1, COLS / 2 - 2, "Help");
	mvwprintw(w_help, 3, 1, format, "left (right)", "move to previous (next) byte");
	mvwprintw(w_help, 4, 1, format, "up (down)", "move to previous (next) line");
	mvwprintw(w_help, 5, 1, format, "<", "move to beginning of line");
	mvwprintw(w_help, 6, 1, format, ">", "move to end of line");
	mvwprintw(w_help, 7, 1, format, "page up", "move 1 page up");
	mvwprintw(w_help, 8, 1, format, "page down", "move 1 page down");
	mvwprintw(w_help, 9, 1, format, "home", "move to beginning of file");
	mvwprintw(w_help, 10, 1, format, "end", "move to end of file");
	mvwprintw(w_help, 11, 1, format, "h", "display this help screen");
	mvwprintw(w_help, 12, 1, format, "j", "jump to offset");
	mvwprintw(w_help, 13, 1, format, "n (w)", "search for byte (string)");
	mvwprintw(w_help, 14, 1, format, "r", "repeat last search");

	mvwprintw(w_help, 15, 1, format, "l", "fill range with byte");
	mvwprintw(w_help, 16, 1, format, "tab", "toggle between hex and ascii editing modes");
	mvwprintw(w_help, 17, 1, format, "u", "undo");
	mvwprintw(w_help, 18, 1, format, "s", "save file");
	mvwprintw(w_help, 19, 1, format, "q", "quit");

	mvwprintw(w_help, 22, 1,"Press any key to exit this help screen.");
	wrefresh(w_help);
	getch();

	delwin(w_help);
	redrawwin(w_offset);
	redrawwin(w_main);
	redrawwin(w_ascii);
}

static void main_loop()
{
	int running = 1;
	int ch, lastch = EOF;
	int i, ret = 0;
	off_t tmp_offset;

	/* determine number of bytes per line (always an even number) */
	bpl = (COLS - 14) / 4;
	if(bpl % 2 != 0)
		bpl--;

	/* lines 0 and (LINES - 1) are reserved for status info */
	max_lines = LINES - 2;

	w_offset = newwin(max_lines, 13, 1, 0);
	w_main = newwin(max_lines, COLS - (bpl + 1) - 13, 1, 13);
	w_ascii = newwin(max_lines, bpl + 1, 1, COLS - bpl - 1);

	scrollok(w_offset, TRUE);
	scrollok(w_main, TRUE);
	scrollok(w_ascii, TRUE);

	current_nibble = cur_offset = cur_line = beg_offset = 0;
	for(end_offset = file_size; end_offset % bpl != 0; end_offset--)
		;
	end_offset -= bpl * (max_lines - 1);
	if(end_offset < 0)
		end_offset = 0;

	if(use_colors)
	{
		wattrset(w_offset, COLOR_PAIR(1));
		wattrset(w_main, COLOR_PAIR(1));
		wattrset(w_ascii, COLOR_PAIR(1));
		wattrset(stdscr, COLOR_PAIR(3));

		for(i = 0; i < LINES; i++)
			mvchgat(i, COLS - bpl - 2, 1, A_NORMAL, 1, NULL);
	}

	for(i = 0; i < max_lines; i++)
		put_line(i, i * bpl);
	hilite();

	do
	{
		put_status();
		wnoutrefresh(stdscr);
		wnoutrefresh(w_offset);
		wnoutrefresh(w_main);
		wnoutrefresh(w_ascii);
		if(!mode_ascii)
		{
			wmove(w_main, cur_line, (3 * (cur_offset % bpl)) + current_nibble);
			wnoutrefresh(w_main);
		}
		if(mode_ascii)
		{
			wmove(w_ascii, cur_line, cur_offset % bpl);
			wnoutrefresh(w_ascii);
		}
		doupdate();

		ch = getch();
		if(ch == '\t')
		{
			mode_ascii = !mode_ascii;
			continue;
		}

		if(mode_ascii && isprint(ch) && (0xFF00 & ch) == 0)
		{
			set_byte(ch);
			ch = KEY_RIGHT; /* nice hack for incrementing current_offset ... */
		}

		switch(ch)
		{
		case KEY_LEFT:
			if(cur_line == 0 && beg_offset == 0 && cur_offset == 0 && (current_nibble == 0 || mode_ascii))
				break;

			unhilite();
			if(cur_line == 0 && beg_offset > 0 && cur_offset % bpl == 0)
			{
				/* move one line up */
				wscrl(w_offset, -1);
				wscrl(w_main, -1);
				wscrl(w_ascii, -1);
				beg_offset -= bpl;
				cur_offset--;
				put_line(0, beg_offset);
			}
			else
			{
				if(mode_ascii || current_nibble == 0)
				{
					cur_offset--;
					if(mode_ascii) {
						current_nibble=0;
					} else {
						current_nibble=1;
					}
				}
				else
				{
					current_nibble--;
				}
				if((cur_offset + 1) % bpl == 0 && (current_nibble == 1 || mode_ascii))
					cur_line--;
			}
			hilite();
			break;

		case KEY_RIGHT:
			advance_right();
			break;

		case KEY_UP:
			if(cur_line == 0 && beg_offset == 0)
				break;

			unhilite();
			if(cur_line == 0)
			{
				/* move one line up */
				wscrl(w_offset, -1);
				wscrl(w_main, -1);
				wscrl(w_ascii, -1);
				beg_offset -= bpl;
				cur_offset -= bpl;
				put_line(0, beg_offset);
			}
			else
			{
				cur_offset -= bpl;
				cur_line--;
			}
			hilite();
			break;

		case KEY_DOWN:
			/* move one line down, but only if allowed */
			if(cur_offset + bpl >= file_size && beg_offset == end_offset)
				break;

			unhilite();
			if(cur_line == max_lines - 1)
			{
				scroll(w_offset);
				scroll(w_main);
				scroll(w_ascii);
				beg_offset += bpl;

				if(cur_offset + bpl >= file_size && beg_offset <= end_offset)
					cur_line--;
				else
					cur_offset += bpl;
				put_line(max_lines - 1, beg_offset + bpl * (max_lines - 1));
			}
			else
			{
				cur_offset += bpl;
				cur_line++;
			}
			hilite();
			break;

		case KEY_HOME:
			if(cur_offset == 0)
				break;

			unhilite();
			if(beg_offset == 0)
			{
				/* we just need to move to the beginning of the line */
				cur_offset = 0;
				cur_line = 0;
				hilite();
				break;
			}

			for(i = 0; i < max_lines; i++)
				put_line(i, i * bpl);

			cur_offset = 0;
			cur_line = 0;
			beg_offset = 0;

			hilite();
			break;

		case KEY_END:
			if(cur_offset == file_size - 1)
				break;

			unhilite();
			if(beg_offset == end_offset)
			{
				/* we just need to move to EOF */
				cur_offset = file_size - 1;
				cur_line = (cur_offset - beg_offset) / bpl;
				hilite();
				break;
			}

			beg_offset = end_offset;
			for(i = 0; i < max_lines; i++)
				put_line(i, beg_offset + i * bpl);

			cur_offset = file_size - 1;
			cur_line = (cur_offset - beg_offset) / bpl;

			hilite();
			break;

		case KEY_NPAGE:
			if(beg_offset == end_offset && cur_offset == file_size - 1)
				break;

			beg_offset += bpl * max_lines;
			if(beg_offset >= end_offset)
			{
				beg_offset = end_offset;
				cur_offset = file_size - 1;
				cur_line = (cur_offset - beg_offset) / bpl;
			}
			else
				cur_offset += bpl * max_lines;

			for(i = 0; i < max_lines; i++)
				put_line(i, beg_offset + i * bpl);

			hilite();
			break;

		case KEY_PPAGE:
			if(beg_offset == 0 && cur_offset == 0)
				break;

			beg_offset -= bpl * max_lines;
			if(beg_offset < 0)
			{
				beg_offset = 0;
				cur_offset = 0;
				cur_line = 0;
			}
			else
				cur_offset -= bpl * max_lines;

			for(i = 0; i < max_lines; i++)
				put_line(i, beg_offset + i * bpl);

			hilite();
			break;

		case 'w':
		case 'W':
			/* search for string
			 * FIXME: needs testing; improve speed(use memcmp() on blocks directly?)
			 */
			{
				const char *p;

				p = ask_user_string("Enter string to search for: ");
				if(*p == '\0')
					break;

				if(last_string_search)
					free(last_string_search);
				last_string_search = xstrdup(p);
				last_search = 1;

				string_search(p);
			}
			break;

		case 'n':
		case 'N':
			/* search for byte */
			{
				byte b;

				ret = ask_user_byte("Enter byte to search for: ", &b);
				if(ret == -1)
				{
					print_message("Wrong byte value (must be in [0, 255]).");
					sleep(1);
					break;
				}

				last_search = 0;
				last_byte_search = b;

				byte_search(b);
			}
			break;

		case 'r':
		case 'R':
			/* repeat last search */
			if(last_search == 0)
				byte_search(last_byte_search);
			else if(last_search == 1)
				string_search(last_string_search);
			break;

		case 'l':
		case 'L':
			/* fill range with byte */
			{
				off_t of1, of2, of;
				byte fill_byte;

				ret = ask_user_offset("Enter start offset: ", &of1);
				if(ret == -1)
				{
					print_message("Invalid starting offset!");
					sleep(1);
					break;
				}
				else if(of1 >= file_size)
				{
					print_message("Starting offset is beyond the end of file!");
					sleep(1);
					break;
				}

				ret = ask_user_offset("Enter end offset: ", &of2);
				if(ret == -1)
				{
					print_message("Invalid end offset!");
					sleep(1);
					break;
				}
				else if(of2 >= file_size)
				{
					print_message("End offset is beyond the end of file!");
					sleep(1);
					break;
				}
				else if(of2 <= of1)
				{
					print_message("End offset is less than or equal to starting offset!");
					sleep(1);
					break;
				}

				ret = ask_user_byte("Fill with byte: ", &fill_byte);
				if(ret == -1)
				{
					print_message("Wrong byte value (must be in [0, 255]).");
					sleep(1);
					break;
				}

				save_undo_fill_info(of1, of2);
				for(of = of1; of <= of2; of++)
					put_byte(of, fill_byte);

				/* refresh lines in view that have been changed */
				of = of1;
				unhilite();
				while(of >= beg_offset && of < beg_offset + bpl * max_lines)
				{
					put_line((of - beg_offset) / bpl, beg_offset + bpl * ((of - beg_offset) / bpl));
					of += bpl;
				}
				hilite();
			}
			break;

		case 'j':
		case 'J':
			/* jump to offset */
			/* the offset can be given in hex(0xab, 0xAB), dec(171) or octal(0...) */
			ret = ask_user_offset("Enter offset (e.g. 0x1a, 27, 033): ", &tmp_offset);
			if(ret == -1)
			{
				print_message("Invalid offset!");
				sleep(1);
				break;
			}

			if(tmp_offset >= file_size)
			{
				print_message("Offset is beyond the end of file!");
				sleep(1);
				break;
			}

			unhilite();
			cur_offset = tmp_offset;

			if(cur_offset > end_offset)
				beg_offset = end_offset;
			else
			{
				for(beg_offset = cur_offset; beg_offset % (bpl * max_lines) != 0; beg_offset--)
					;
			}

			for(i = 0; i < max_lines; i++)
				put_line(i, beg_offset + i * bpl);

			cur_line = (cur_offset - beg_offset) / bpl;
			hilite();
			break;

		case '<':
			/* move to the begging of the current line */
			if(cur_offset % bpl != 0)
			{
				unhilite();
				while(--cur_offset % bpl)
					;
				hilite();
			}
			break;

		case '>':
			/* move to the end of the current line */
			if((cur_offset + 1) % bpl != 0)
			{
				unhilite();
				while(++cur_offset % bpl != 0 && cur_offset < file_size)
					;
				cur_offset--;
				hilite();
			}
			break;

		case 's':
		case 'S':
			/* save file */
			/* if(uando_count > 0) */
			if(!file_saved)
			{
				do
					ret = get_user_input("Are you sure you want to save (y/n)?");
				while(ret != 'y' && ret != 'n');

				if(ret == 'y')
					save_file(0);
				break;
			}
			break;

		case 'q':
		case 'Q':
			/* quit */
			if(!file_saved)
			{
				/* ask the user to save file */
				do
					ret = get_user_input("File was modified. Save (y/n/c)?");
				while(ret != 'y' && ret != 'n' && ret != 'c');

				if(ret == 'y')
					save_file(1);
				if(ret != 'c')
					running = 0;
			}
			else
				running = 0;
			break;

		case 'u':
		case 'U':
			do_undo();
			break;

		case 'h':
		case 'H':
			show_helpwin();
			break;

		default:
			ch = tolower(ch);
			if(isxdigit(ch))
			{
				set_byte_part(ch);
				if(advance_after_edit)
				{
					advance_right();
				}
			}
			break;
		}
		if(isxdigit(ch))
			lastch = ch;
		else
			lastch = EOF;
	}
	while(running);

	scrollok(stdscr,FALSE);
	refresh();
	delwin(w_main);
	delwin(w_offset);
	delwin(w_ascii);
	putchar('\n');
}

static void advance_right()
{
	if(cur_offset == file_size - 1 && (current_nibble == 1 || mode_ascii))
		return;

	unhilite();
	if(mode_ascii || current_nibble == 1)
	{
		cur_offset++;
		current_nibble=0;
	}
	else
	{
		current_nibble++;
	}
	if(cur_line == max_lines - 1 && (cur_offset) % bpl == 0 && current_nibble == 0)
	{
		/* move down one line */
		scroll(w_offset);
		scroll(w_main);
		scroll(w_ascii);
		beg_offset += bpl;
		put_line(max_lines - 1, cur_offset);
	}

	if(cur_offset % bpl == 0 && current_nibble == 0 && cur_line < max_lines - 1)
		cur_line++;
	hilite();
}

static void init_blocksize()
{

#ifdef HAVE_MMAP

# if defined(PAGESIZE)
	off_len = PAGESIZE;
# elif defined(_PAGESIZE)
	off_len = _PAGESIZE;
# else
	if((off_len = sysconf(_SC_PAGESIZE)) <= 1 &&
	   (off_len = sysconf(_SC_PAGE_SIZE)) <= 1)
		off_len = 4096;
# endif
	off_len <<= 1;

#else
	/*
	 * we don't have mmap() so off_len is set in open_file() to
	 * st.st_blksize
	 */
#endif
}

static void my_signal_handler(int sig)
{
	/* don't save anything */
	scrollok(stdscr, FALSE);
	refresh();
	endwin();
	pconfig_quit();
	fprintf(stderr, "\n%s: received signal %d, exiting.\n", prog_name, sig);
	exit(sig);
}

static int str_to_color(const char *str)
{
	if(strcmp(str, "black") == 0)
		return COLOR_BLACK;
	else if(strcmp(str, "red") == 0)
		return COLOR_RED;
	else if(strcmp(str, "green") == 0)
		return COLOR_GREEN;
	else if(strcmp(str, "yellow") == 0)
		return COLOR_YELLOW;
	else if(strcmp(str, "blue") == 0)
		return COLOR_BLUE;
	else if(strcmp(str, "magenta") == 0)
		return COLOR_MAGENTA;
	else if(strcmp(str, "cyan") == 0)
		return COLOR_CYAN;
	else if(strcmp(str, "white") == 0)
		return COLOR_WHITE;

	/* this should never happen */
	return COLOR_BLACK;
}

/* init curses, set up colors ... */
static void setup_curses()
{
	int fg_color, bg_color;

	initscr();
	keypad(stdscr, TRUE);
	noecho();
	cbreak();

	if(use_colors)
	{
		use_colors = 1;	/* because of unhilite() */
		if(start_color() == ERR)
		{
			/* disable colors */
			use_colors = 0;
			return;
		}

		bg_color = str_to_color(pconfig_get_string("bg_color"));
		fg_color = str_to_color(pconfig_get_string("fg_color"));
		init_pair(1, fg_color, bg_color);	/* default */

		bg_color = str_to_color(pconfig_get_string("bg_color_selected"));
		fg_color = str_to_color(pconfig_get_string("fg_color_selected"));
		init_pair(2, fg_color, bg_color);	/* selected byte */

		bg_color = str_to_color(pconfig_get_string("bg_color_status"));
		fg_color = str_to_color(pconfig_get_string("fg_color_status"));
		init_pair(3, fg_color, bg_color);	/* status colors */
	}
}

/*
 * supported options and their defaults
 * note: if the config file isn't present,
 * defaults will be used
 */
static pconfig_option_t opts[] = {
	{ PCONFIG_INT, "advance_after_edit", "1" },
	{ PCONFIG_INT, "make_backup_files", "1" },
	{ PCONFIG_INT, "use_colors", "1" },
	{ PCONFIG_STRING, "bg_color", "blue" },
	{ PCONFIG_STRING, "fg_color", "white" },
	{ PCONFIG_STRING, "bg_color_selected", "black" },
	{ PCONFIG_STRING, "fg_color_selected", "green" },
	{ PCONFIG_STRING, "bg_color_status", "cyan" },
	{ PCONFIG_STRING, "fg_color_status", "black" },
	{ 0, NULL, NULL }
};

static void read_config()
{
	struct passwd *pw;
	char cfg_file[1024];

	if(config_file == NULL)
	{
		/* use default config file */
		pw = getpwuid(getuid());
		if(pw == NULL)
			return;
		sprintf(cfg_file, "%s/.hemerc", pw->pw_dir);
		config_file = cfg_file;
	}

	if(pconfig_init(0, opts) < 0)
	{
		fprintf(stderr, "%s: pconfig_init() failed\n", prog_name);
		exit(1);
	}

	pconfig_parse(config_file);	/* error return? */

	if(use_colors == -1)
		use_colors = pconfig_get_int("use_colors");

	if(make_backup_files == -1)
		make_backup_files = pconfig_get_int("make_backup_files");

	if(advance_after_edit == -1)
		advance_after_edit = pconfig_get_int("advance_after_edit");
}

static void print_usage()
{
	printf("Usage: %s [options] FILE\nAvailable options:\n", prog_name);
	puts("  -n, --no-backup-files \tdon't make backup files");
	puts("  -c, --config-file CONFIGFILE \tread configuration from CONFIGFILE");
	puts("  -d, --disable-colors \t\tdon't use colors");
	puts("  -h, --help       \t\thelp");
	puts("  -v, --version    \t\tversion");
	puts("\nReport bugs to <pokemon@fly.srk.fer.hr>.");
}

static void save_prog_name(const char *str)
{
	const char *p, *r;

	r = str + strlen(str);
	p = r;
	while(p >= str && *p != '/')
		p--;

	if(p < r)
	{
		prog_name = xmalloc(r - p);
		strcpy(prog_name, p + 1);
	}
	else
		prog_name = xstrdup("heme");
}

int main(int argc, char *argv[])
{
	int c;

	save_prog_name(argv[0]);

	/* set up the signal handler */
	signal(SIGINT, my_signal_handler);
	signal(SIGTERM, my_signal_handler);

	while((c = pgetopt(argc, argv, " -n --no-backup-files -c: --config-file: "
					"-d --disable-colors -h --help -v --version")) != -1)
	{
		switch(c)
		{
		case 0:
		case 1:
			make_backup_files = 0;
			break;

		case 2:
		case 3:
			config_file = poptarg;
			break;

		case 4:
		case 5:
			use_colors = 0;
			break;

		case 6:
		case 7:
			print_usage();
			return 0;
			break;

		case 8:
		case 9:
			printf("heme %s\n", heme_version);
			return 0;
			break;
		}
	}

	if(argv[poptindex] == 0)
	{
		printf("%s: missing file argument\n", prog_name);
		return 0;
	}

	read_config();

	init_blocksize();
	open_file(argv[poptindex]);

	setup_curses();

	main_loop();

	close(fd);
	pconfig_quit();
	endwin();
	return 0;
}
