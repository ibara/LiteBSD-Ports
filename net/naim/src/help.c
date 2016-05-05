#include <stdio.h>
#include "help.h"


char *topics[] = {
 "In the top-right corner of your screen is a \"window list\" window, which will display one window for each online buddy once you have signed on. The window highlighted at the top is the current window. Use the Tab key to cycle through the listed windows, or <font color=\"#00FF00\">/jump buddyname</font> to jump directly to buddyname's window.",
" ",
"The window list window will hide itself automatically to free up space for conversation, but it will come back if someone other than your current buddy messages you. You can press Ctrl-N to go to the <b>n</b>ext waiting (yellow) buddy when this happens.",
NULL
};


char *about[] = {
 "naim is a console-mode chat client.",
"naim supports AIM, IRC, ICQ, and Lily.",
" ",
"Copyright 1998-2004 Daniel Reed <n@ml.org>",
"http://naim.n.ml.org/",
NULL
};


char *keys[] = {
 NULL
};


char *settings[] = {
 NULL
};


char *commands[] = {
 NULL
};

struct _help_texts h_texts[] = {
{topics,"topics"},
{about,"about"},
{keys,"keys"},
{settings,"settings"},
{commands,"commands"},
{NO_HELP, NULL}
};

