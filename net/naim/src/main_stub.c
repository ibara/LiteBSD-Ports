/*  _ __   __ _ ___ __  __
** | '_ \ / _` |_ _|  \/  | naim
** | | | | (_| || || |\/| | Copyright 2003 Daniel Reed <n@ml.org>
** |_| |_|\__,_|___|_|  |_| ncurses-based chat client
*/
#include <naim/naim.h>

#include "naim-int.h"

#ifdef FAKE_MAIN_STUB
int	main_stub(int argc, char **args);

int	main(int argc, char **args) {
	return(main_stub(argc, args));
}
#endif
