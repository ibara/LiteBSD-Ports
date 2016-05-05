struct {
	int	code;
	const char	*name;
} conio_bind_names[] = {
	{ '	',	"TAB" },
	{ ' ',	"SPACE" },
	{ 27,	"ESCAPE" },
#ifdef KEY_MAX
# ifdef SIGHUP
#  define KEY_SIGHUP (2*KEY_MAX+SIGHUP)
	{ KEY_SIGHUP,	"SIGHUP" },
# endif
# ifdef SIGUSR1
#  define KEY_SIGUSR1 (2*KEY_MAX+SIGUSR1)
	{ KEY_SIGUSR1,	"SIGUSR1" },
# endif
# ifdef SIGUSR2
#  define KEY_SIGUSR2 (2*KEY_MAX+SIGUSR2)
	{ KEY_SIGUSR2,	"SIGUSR2" },
# endif
#endif
#ifdef KEY_BREAK
	{ KEY_BREAK,	"BREAK" },
#endif
#ifdef KEY_SRESET
	{ KEY_SRESET,	"SRESET" },
#endif
#ifdef KEY_RESET
	{ KEY_RESET,	"RESET" },
#endif
#ifdef KEY_DOWN
	{ KEY_DOWN,	"DOWN" },
#endif
#ifdef KEY_UP
	{ KEY_UP,	"UP" },
#endif
#ifdef KEY_LEFT
	{ KEY_LEFT,	"LEFT" },
#endif
#ifdef KEY_RIGHT
	{ KEY_RIGHT,	"RIGHT" },
#endif
#ifdef KEY_HOME
	{ KEY_HOME,	"HOME" },
#endif
#ifdef KEY_BACKSPACE
	{ KEY_BACKSPACE,	"BACKSPACE" },
#endif
#ifdef KEY_F0
	{ KEY_F0,	"F0" },
#endif
#ifdef KEY_DL
	{ KEY_DL,	"DL" },
#endif
#ifdef KEY_IL
	{ KEY_IL,	"IL" },
#endif
#ifdef KEY_DC
	{ KEY_DC,	"DC" },
#endif
#ifdef KEY_IC
	{ KEY_IC,	"IC" },
#endif
#ifdef KEY_EIC
	{ KEY_EIC,	"EIC" },
#endif
#ifdef KEY_CLEAR
	{ KEY_CLEAR,	"CLEAR" },
#endif
#ifdef KEY_EOS
	{ KEY_EOS,	"EOS" },
#endif
#ifdef KEY_EOL
	{ KEY_EOL,	"EOL" },
#endif
#ifdef KEY_SF
	{ KEY_SF,	"SF" },
#endif
#ifdef KEY_SR
	{ KEY_SR,	"SR" },
#endif
#ifdef KEY_NPAGE
	{ KEY_NPAGE,	"NPAGE" },
#endif
#ifdef KEY_PPAGE
	{ KEY_PPAGE,	"PPAGE" },
#endif
#ifdef KEY_STAB
	{ KEY_STAB,	"STAB" },
#endif
#ifdef KEY_CTAB
	{ KEY_CTAB,	"CTAB" },
#endif
#ifdef KEY_CATAB
	{ KEY_CATAB,	"CATAB" },
#endif
#ifdef KEY_ENTER
	{ KEY_ENTER,	"ENTER" },
#endif
#ifdef KEY_PRINT
	{ KEY_PRINT,	"PRINT" },
#endif
#ifdef KEY_LL
	{ KEY_LL,	"LL" },
#endif
#ifdef KEY_A1
	{ KEY_A1,	"A1" },
#endif
#ifdef KEY_A3
	{ KEY_A3,	"A3" },
#endif
#ifdef KEY_B2
	{ KEY_B2,	"B2" },
#endif
#ifdef KEY_C1
	{ KEY_C1,	"C1" },
#endif
#ifdef KEY_C3
	{ KEY_C3,	"C3" },
#endif
#ifdef KEY_BTAB
	{ KEY_BTAB,	"BTAB" },
#endif
#ifdef KEY_BEG
	{ KEY_BEG,	"BEG" },
#endif
#ifdef KEY_CANCEL
	{ KEY_CANCEL,	"CANCEL" },
#endif
#ifdef KEY_CLOSE
	{ KEY_CLOSE,	"CLOSE" },
#endif
#ifdef KEY_COMMAND
	{ KEY_COMMAND,	"COMMAND" },
#endif
#ifdef KEY_COPY
	{ KEY_COPY,	"COPY" },
#endif
#ifdef KEY_CREATE
	{ KEY_CREATE,	"CREATE" },
#endif
#ifdef KEY_END
	{ KEY_END,	"END" },
#endif
#ifdef KEY_EXIT
	{ KEY_EXIT,	"EXIT" },
#endif
#ifdef KEY_FIND
	{ KEY_FIND,	"FIND" },
#endif
#ifdef KEY_HELP
	{ KEY_HELP,	"HELP" },
#endif
#ifdef KEY_MARK
	{ KEY_MARK,	"MARK" },
#endif
#ifdef KEY_MESSAGE
	{ KEY_MESSAGE,	"MESSAGE" },
#endif
#ifdef KEY_MOVE
	{ KEY_MOVE,	"MOVE" },
#endif
#ifdef KEY_NEXT
	{ KEY_NEXT,	"NEXT" },
#endif
#ifdef KEY_OPEN
	{ KEY_OPEN,	"OPEN" },
#endif
#ifdef KEY_OPTIONS
	{ KEY_OPTIONS,	"OPTIONS" },
#endif
#ifdef KEY_PREVIOUS
	{ KEY_PREVIOUS,	"PREVIOUS" },
#endif
#ifdef KEY_REDO
	{ KEY_REDO,	"REDO" },
#endif
#ifdef KEY_REFERENCE
	{ KEY_REFERENCE,	"REFERENCE" },
#endif
#ifdef KEY_REFRESH
	{ KEY_REFRESH,	"REFRESH" },
#endif
#ifdef KEY_REPLACE
	{ KEY_REPLACE,	"REPLACE" },
#endif
#ifdef KEY_RESTART
	{ KEY_RESTART,	"RESTART" },
#endif
#ifdef KEY_RESUME
	{ KEY_RESUME,	"RESUME" },
#endif
#ifdef KEY_SAVE
	{ KEY_SAVE,	"SAVE" },
#endif
#ifdef KEY_SBEG
	{ KEY_SBEG,	"SBEG" },
#endif
#ifdef KEY_SCANCEL
	{ KEY_SCANCEL,	"SCANCEL" },
#endif
#ifdef KEY_SCOMMAND
	{ KEY_SCOMMAND,	"SCOMMAND" },
#endif
#ifdef KEY_SCOPY
	{ KEY_SCOPY,	"SCOPY" },
#endif
#ifdef KEY_SCREATE
	{ KEY_SCREATE,	"SCREATE" },
#endif
#ifdef KEY_SDC
	{ KEY_SDC,	"SDC" },
#endif
#ifdef KEY_SDL
	{ KEY_SDL,	"SDL" },
#endif
#ifdef KEY_SELECT
	{ KEY_SELECT,	"SELECT" },
#endif
#ifdef KEY_SEND
	{ KEY_SEND,	"SEND" },
#endif
#ifdef KEY_SEOL
	{ KEY_SEOL,	"SEOL" },
#endif
#ifdef KEY_SEXIT
	{ KEY_SEXIT,	"SEXIT" },
#endif
#ifdef KEY_SFIND
	{ KEY_SFIND,	"SFIND" },
#endif
#ifdef KEY_SHELP
	{ KEY_SHELP,	"SHELP" },
#endif
#ifdef KEY_SHOME
	{ KEY_SHOME,	"SHOME" },
#endif
#ifdef KEY_SIC
	{ KEY_SIC,	"SIC" },
#endif
#ifdef KEY_SLEFT
	{ KEY_SLEFT,	"SLEFT" },
#endif
#ifdef KEY_SMESSAGE
	{ KEY_SMESSAGE,	"SMESSAGE" },
#endif
#ifdef KEY_SMOVE
	{ KEY_SMOVE,	"SMOVE" },
#endif
#ifdef KEY_SNEXT
	{ KEY_SNEXT,	"SNEXT" },
#endif
#ifdef KEY_SOPTIONS
	{ KEY_SOPTIONS,	"SOPTIONS" },
#endif
#ifdef KEY_SPREVIOUS
	{ KEY_SPREVIOUS,	"SPREVIOUS" },
#endif
#ifdef KEY_SPRINT
	{ KEY_SPRINT,	"SPRINT" },
#endif
#ifdef KEY_SREDO
	{ KEY_SREDO,	"SREDO" },
#endif
#ifdef KEY_SREPLACE
	{ KEY_SREPLACE,	"SREPLACE" },
#endif
#ifdef KEY_SRIGHT
	{ KEY_SRIGHT,	"SRIGHT" },
#endif
#ifdef KEY_SRSUME
	{ KEY_SRSUME,	"SRSUME" },
#endif
#ifdef KEY_SSAVE
	{ KEY_SSAVE,	"SSAVE" },
#endif
#ifdef KEY_SSUSPEND
	{ KEY_SSUSPEND,	"SSUSPEND" },
#endif
#ifdef KEY_SUNDO
	{ KEY_SUNDO,	"SUNDO" },
#endif
#ifdef KEY_SUSPEND
	{ KEY_SUSPEND,	"SUSPEND" },
#endif
#ifdef KEY_UNDO
	{ KEY_UNDO,	"UNDO" },
#endif
#ifdef KEY_MOUSE
	{ KEY_MOUSE,	"MOUSE" },
#endif
#ifdef KEY_RESIZE
	{ KEY_RESIZE,	"RESIZE" },
#endif
#ifdef KEY_EVENT
	{ KEY_EVENT,	"EVENT" },
#endif
#ifdef KEY_MAX
	{ KEY_MAX,	"MAX" },
#endif
};

#define CONIO_KEY_BUDDY_NEXT 0
#define CONIO_KEY_BUDDY_PREV 1
#define CONIO_KEY_BUDDY_SCROLL_BACK 2
#define CONIO_KEY_BUDDY_SCROLL_FORWARD 3
#define CONIO_KEY_CONN_NEXT 4
#define CONIO_KEY_CONN_PREV 5
#define CONIO_KEY_INPUT_BACKSPACE 6
#define CONIO_KEY_INPUT_CURSOR_END 7
#define CONIO_KEY_INPUT_CURSOR_HOME 8
#define CONIO_KEY_INPUT_CURSOR_HOME_END 9
#define CONIO_KEY_INPUT_CURSOR_LEFT 10
#define CONIO_KEY_INPUT_CURSOR_RIGHT 11
#define CONIO_KEY_INPUT_DELETE 12
#define CONIO_KEY_INPUT_ENT_BOLD 13
#define CONIO_KEY_INPUT_ENTER 14
#define CONIO_KEY_INPUT_ENT_ITALIC 15
#define CONIO_KEY_INPUT_KILL 16
#define CONIO_KEY_INPUT_KILL_EOL 17
#define CONIO_KEY_INPUT_KILL_WORD 18
#define CONIO_KEY_INPUT_PASTE 19
#define CONIO_KEY_INPUT_SCROLL_BACK 20
#define CONIO_KEY_INPUT_SCROLL_FORWARD 21
#define CONIO_KEY_INPUT_SYM_AMP 22
#define CONIO_KEY_INPUT_SYM_GT 23
#define CONIO_KEY_INPUT_SYM_LT 24
#define CONIO_KEY_REDRAW 25
#define CONIO_KEY_SPACE_OR_NBSP 26
#define CONIO_KEY_STATUS_DISPLAY 27
#define CONIO_KEY_STATUS_POKE 28
#define CONIO_KEY_TAB 29
#define CONIO_KEY_TAB_BUDDY_NEXT 30
#define CONIO_KEY_WINLIST_HIDE 31

struct {
	const char	*name,
			*desc;
} conio_key_names[] = {
	{ ":BUDDY_NEXT",	"Advance to the next window" },
	{ ":BUDDY_PREV",	"Go to the previous window" },
	{ ":BUDDY_SCROLL_BACK",	"Scroll the current window backwards (up)" },
	{ ":BUDDY_SCROLL_FORWARD",	"Scroll the current window forwards in time" },
	{ ":CONN_NEXT",	"Go to the next connection window (AIM, EFnet, etc.)" },
	{ ":CONN_PREV",	"Go to the previous connection window" },
	{ ":INPUT_BACKSPACE",	"Delete the previous character in the input line" },
	{ ":INPUT_CURSOR_END",	"Jump to the end of the input line" },
	{ ":INPUT_CURSOR_HOME_END",	"Jump between the beginning and end of the input line" },
	{ ":INPUT_CURSOR_HOME",	"Jump to the beginning of the input line" },
	{ ":INPUT_CURSOR_LEFT",	"Move left in the input line" },
	{ ":INPUT_CURSOR_RIGHT",	"Move right in the input line" },
	{ ":INPUT_DELETE",	"Delete the current character in the input line" },
	{ ":INPUT_ENT_BOLD",	"Toggle HTML bold mode" },
	{ ":INPUT_ENTER",	"In paste mode, an HTML newline, otherwise sends the current IM or executes the current command" },
	{ ":INPUT_ENT_ITALIC",	"Toggle HTML italic (inverse) mode" },
	{ ":INPUT_KILL",	"Delete the entire input line" },
	{ ":INPUT_KILL_EOL",	"Delete the input line from the current character to the end of the line" },
	{ ":INPUT_KILL_WORD",	"Delete the input line from the current character to the beginning of the previous word" },
	{ ":INPUT_PASTE",	"Alter the input handler to handle pasted text better" },
	{ ":INPUT_SCROLL_BACK",	"Search back through your input history" },
	{ ":INPUT_SCROLL_FORWARD",	"Search forward through your input history" },
	{ ":INPUT_SYM_AMP",	"In paste mode, an HTML ampersand, otherwise a literal ampersand" },
	{ ":INPUT_SYM_GT",	"In paste mode, an HTML greater-than, otherwise a literal greater-than" },
	{ ":INPUT_SYM_LT",	"In paste mode, an HTML less-than, otherwise a literal less-than" },
	{ ":REDRAW",	"Redraw the screen" },
	{ ":SPACE_OR_NBSP",	"In paste mode, an HTML hard space, otherwise a literal space" },
	{ ":STATUS_DISPLAY",	"Display or hide the status console" },
	{ ":STATUS_POKE",	"Bring the status console down for $autohide seconds" },
	{ ":TAB",	"An HTML tab (8 hard spaces)" },
	{ ":TAB_BUDDY_NEXT",	"In paste mode, an HTML tab (8 hard spaces), otherwise perform command completion or advance to the next window" },
	{ ":WINLIST_HIDE",	"Cycle between always visible, always hidden, or auto-hidden" },
};
