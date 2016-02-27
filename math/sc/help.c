// SC is free software distributed under the MIT license

#include "sc.h"

static const char intro[] = {
" \0"
" Overview:\0"
" \0"
" A:   This overview\0"
" B:   Toggle Options\0"
" C:   Set Options\0"
" D:   Cursor movement commands\0"
" E:   Cell entry and editing commands\0"
" F:   Line Editing\0"
" G:   File commands\0"
" H:   Row and column commands\0"
" I:   Range commands\0"
" J:   Miscellaneous commands\0"
" K:   Variable names/Expressions\0"
" L:   Range functions\0"
" M:   Numeric functions\0"
" N:   String functions\0"
" O:   Financial functions\0"
" P:   Time and date functions\0"
" \0"
" Q:   Return to main spreadsheet\0"
};

static const char toggleoptions[] = {
" \0"
" B: Toggle Options\0"
" \0"
"     ^To  Toggle options. Toggle one option selected by o:\0"
"          a    Recalculate automatically or on \"@\" commands.\0"
"          o    Optimize expressions upon entry if enabled.\0"
"          c    Current cell highlighting enable/disable.\0"
"          e    External function execution enable/disable.\0"
"          l    Autolabeling defined cells enable/disable.\0"
"          n    If enabled, a digit starts a numeric value.\0"
"          t    Top line display enable/disable.\0"
"          $    Dollar prescale.  If enabled, all numeric constants\0"
"               (not expressions) entered are multipled by 0.01.\0"
"          r    Newline action.  Toggle between no action, move down\0"
"               after entry and move right after entry.\0"
"          z    Set the newline action limits to the current row and column\0"
"            (for r && z see also set rowlimit=n, collimit=n)\0"
};

static const char setoptions[] = {
" \0"
" C: Set Options\0"
" \0"
"     S  Set options.  Options include:\0"
"          autocalc      Automatic recalculation.\0"
"          byrows        Recalculate in row order. (default)\0"
"          bycols        Recalculate in column order.\0"
"          optimize      Optimize expressions upon entry. (default off)\0"
"          iterations=n  Set the number of iterations allowed. (10)\0"
"          tblstyle=xx   Set \"T\" output style to:\0"
"                        0 (none), tex, latex, slatex, or tbl.\0"
"          rndtoeven     Round *.5 to nearest even number instead of\0"
"                        always up.\0"
"          rowlimit=n    Set the remembered row limit for newline action.\0"
"          collimit=n    Set the remembered column limit for newline action.\0"
"                        (rowlimit and collimit can both be set by ^Tz)\0"
};

static const char cursor[] = {
" \0"
" D: Cell cursor movement (always OK):\0"
" \0"
"     ^N ^P      Down, up\0"
"     j k l h    Down, up, right, left\0"
"     Arrow keys (if the terminal and termcap support them.)\0"
"     ^Ed        Go to end of range.  Follow ^E by a direction indicator\0"
"                such as ^P or j.\0"
" \0"
"     J K L H    Down, up, right, left by 1/2 pages (or 'pagesize' rows)\0"
"     SPACE      Forward\0"
"     ^H         Back\0"
"     TAB        Forward, otherwise starts/ends a range\0"
"     ^          Up to row 0 of the current column.\0"
"     #          Down to the last valid row of the current column.\0"
"     0          Back to column A.  Preface with ^U if numeric mode.\0"
"     $          Forward to the last valid column of the current row.\0"
"     b          Back then up to the previous valid cell.\0"
"     w          Forward then down to the next valid cell.\0"
"     g          Go to a cell.  Cell name, range name, quoted string,\0"
"                a number, 'error', or 'invalid' to specify which cell.\0"
"     ` '        Go to a marked cell (see help screen e for more info.\0"
};


static const char cell[] = {
" \0"
" E: Cell entry and editing commands:\0"
" \0"
"     =    Enter a numeric constant or expression.\0"
"     <    Enter a left justified string or string expression.\0"
"     \\    Enter a centered label.\0"
"     >    Enter a right justified string or string expression.\0"
"     e    Edit the current cell's numeric value.\0"
"     E    Edit the current cell's string part.\0"
"     F    Assign a format to the current cell's numeric value.\0"
"     x    Clear the current cell.\0"
"     m    Followed by any lowercase letter, marks the current cell\0"
"          with that letter.\0"
"     c    Copy a marked cell to the current cell.\0"
"     +    Increment numeric part\0"
"     -    Decrement numeric part\0"
"  RETURN  Enter insert mode if the input line was empty (ESC to edit)\0"
" \0"
"     In numeric mode, a decimal digit, \"+\", \"-\", and \".\" all start\0"
"     a new numeric constant or expression.\0"
};


static const char vi[] = {
" \0"
" F: Line Editor\0"
" \0"
"     Hitting the ESC key while entering any command on the top line\0"
"     will start a one-line vi-style editor.  Supported commands:\0"
" \0"
"     ESC q        Abort command entry.\0"
"     h l          Move cursor forward, backward.\0"
"     0 $          Move cursor to the beginning, end of the line.\0"
"     b w          Move cursor forward/back one word.\0"
"     e            Move cursor to next end-of-word.\0"
"     fc           Move cursor to character c.\0"
"     tc           Move the cursor to the character before c.\0"
"     i a          Enter insert mode before/after the cursor.\0"
"     I A          Move to column 0/end of line and enter insert mode.\0"
"     x X          Delete the character under/before the cursor.\0"
"     rc           Replace the character under the cursor with c.\0"
"     cm dm        Change/Delete - m = b,e,f,h,l,t or w.\0"
"     R            Enter replace (overstrike) mode.\0"
"     s            Delete character under cursor and enter insert mode.\0"
"     + j - k / ?  Forward/backward/search the command history.\0"
"     n N          Repeat last history search (N = opposite direction).\0"
"     . u          Repeat/undo the last command.\0"
"     ^V           Enter navigate mode.  Another ^V enters current cell address.\0"
"     ^W           Type, in the command line, the current cell's expression.\0"
"     ^A           Type, in the command line, the current cell's numeric value.\0"
};

static const char file[] = {
" \0"
" G: File commands:\0"
" \0"
"     G    Get a new database from a file. \0"
"     M    Merge a new file into the current database.\0"
"     P    Put the current database into a file.\0"
"     W    Write a listing of the current database into a file in\0"
"          a form that matches its appearance on the screen.\0"
"     T    Write a listing of the current database to a file, but\0"
"          put delimiters between each pair of fields.\0"
"          Optionally brackets output with control lines for \"tbl\",\0"
"          \"LaTeX\", \"SLaTex\", or \"TeX\".\0"
" \0"
"     \"\"| program\"\" for a file name will pipe output to\0"
"     a program for Put, Write and Table.  If a cell name is used\0"
"     as the file name, the cell's string part will be used as the\0"
"     file name.\0"
};


static const char row[] = {
" \0"
" H: Row and column commands:\0"
" \0"
"     ir, ic      Insert a new, empty row (column)\0"
"     ar, ac      Append a new copy of the current row (column)\0"
"     dr, dc      Delete the current row (column)\0"
"     pr, pc, pm  Pull deleted cells back into the spreadsheet\0"
"                 Insert rows, columns or merge the cells.\0"
"     vr, vc      Remove expressions from the affected rows (columns),\0"
"                 leaving only the values.\0"
"     Zr, Zc      Hide (\"zap\") the current row (column)\0"
"     sr, sc      Show hidden rows (columns)\0"
"     f           Set the output format to be used with the values of\0"
"                 each cell in this column.  Enter field width and\0"
"                 number of fractional digits.  A preceding count can be\0"
"                 used to change more than one column.\0"
" \0"
"     Commands which move or copy cells also modify the row and column \0"
"     references in the new cell expressions.  Use \"fixed\" or the\0"
"     \"$\" style cell reference to supress the change.\0"
" \0"
"     @myrow, @mycol        return the row or column of the current cell\0"
"     @lastrow, @lastcol    return the row or column of the current cell\0"
};

static const char range[] = {
" \0"
" I: Range commands:\0"
"     rx   Clear a range. \0"
"     rv   Remove the expressions from a range of cells, leaving \0"
"          just the values.\0"
"     rc   Copy a source range to a destination range.\0"
"     rf   Fill a range with constant values starting with a given\0"
"          value and increasing by a given increment.\0"
"     rd   Assign a name to a cell or a range of cells.  Give the\0"
"          the name, surrounded by quotes, and either a cell name such\0"
"          as \"A10\" or a range such as \"a1:b20\".\0"
"     rl   Locks a cell or a range of cells, i.e makes it unchangeable.\0"
"     rU   Unlocks a locked cell, i.e makes it changeable.\0"
"     rs   Shows the currently defined range names.  Pipe output to\0"
"          sort, then to less.\0"
"     ru   Use this command to undefine a previously defined range name.\0"
"     rF   Assign a format string to a range of cells.\0"
" \0"
"     Range operations affect a rectangular region on the screen\0"
"     defined by the upper left and lower right cells in the region.\0"
"     A range is specified by giving the cell names separated by \":\",\0"
"     such as \"a20:k52\".  Another way to refer to a range is to use\0"
"     a name previously defined using \"rd\".\0"
};

static const char misc[] = {
" \0"
" J: Miscellaneous commands:\0"
" \0"
"     Q q ^C   Exit from the program.\0"
"     ^G ESC   Abort entry of the current command.\0"
"     ?        Help\0"
"     !        Shell escape.  Enter a command to run.  \"!!\" repeats\0"
"              the last command.  Just \"!\" starts an interactive shell.\0"
"     ^L       Redraw the screen.\0"
"     ^R       Redraw the screen.  Highlight cells with values but no\0"
"              expressions.\0"
"     ^X       Redraw the screen.  Show formulas, not values.\0"
"     C        Redraw the screen with the row containing the current cell\0"
"              centered.\0"
"     @        Recalculate the spreadsheet.\0"
"     TAB      When the character cursor is on the top line TAB can be used\0"
"              to start or stop the display of the default range.\0"
};

static const char var[] = {
" \0"
" K: Variable names:\0"
" \0"
"     K20    Row and column can vary on copies.\0"
"     $K$20  Row and column stay fixed on copies.\0"
"     $K20   Row can vary; column stays fixed on copies.\0"
"     K$20   Row stays fixed; column can vary on copies.\0"
"     fixed  holds following expession fixed on copies.\0"
"     Cells and ranges can be given a symbolic name via \"rd\".\0"
" \0"
" Expressions:\0"
"     -e      Negation                e<=e  Less than or equal\0"
"     e+e     Addition                e=e   Equal\0"
"     e-e     Subtraction             e!=e  Not Equal\0"
"     e*e     Multiplication          e>=e  Greater than or equal\0"
"     e/e     Division                e>e  Greater than\0"
"     e%e     Modulo                  e<e  Less than\0"
"     e^e     Exponentiation          e&e  Boolean operator AND.\0"
"     ~e      Boolean operator NOT    e|e  Boolean operator OR\0"
"     e?e1:e2  or @if(e,e1,e2)\0"
"             Conditional: If e is non zero then then e1, else e2.\0"
"     Terms may be constants, variables, and parenthesized expressions.\0"
};

static const char rangef[] = {
" \0"
" L: Range functions:\0"
" \0"
"     @sum(r)           Sum all valid cells in the range.\0"
"     @prod(r)          Multiply together all valid cells in the range.\0"
"     @avg(r)           Average all valid cells in the range.\0"
"     @count(r)         Count all valid cells in the range.\0"
"     @max(r)           Return the maximum value in the range.\0"
"     @min(r)           Return the minimum value in the range.\0"
"     @stddev(r)        Return the sample standard deviation of \0"
"                       the cells in the range.\0"
"     @index(e,r) @stindex(e,r)\0"
"                       Return the numeric (string) value of the cell at\0"
"                       index e into range r.\0"
"     @lookup(e,r) @hlookup(e,r,n) @vlookup(e,r,n)\0"
"                       Search through the range r for a value that\0"
"                       matches e.  If e is numeric, the last value <= e\0"
"                       matches; if string, an exact match is required.\0"
"                       @lookup searches a single row (column) and returns\0"
"                       the value from the next column (row); @hlookup\0"
"                       (@vlookup) searches the first row (column) in r and\0"
"                       returns the value n columns (rows) from the match.\0"
};

static const char numericf[] = {
" \0"
" M: Numeric functions:\0"
" \0"
"     @atan2(e1,e2)     Arc tangent of e1/e2.\0"
"     @ceil(e)          Smallest integer not less than e.\0"
"     @eqs(se1,se2)     1 if string expr se1 has the same value as se2.\0"
"     @exp(e)           Exponential function of e.\0"
"     @abs(e) @fabs(e)  Absolute value of e.\0"
"     @floor(e)         The largest integer not greater than e.\0"
"     @hypot(x,y)       Sqrt(x*x+y*y).\0"
"     @max(e1,e2,...)   The maximum of the values of the e's.\0"
"     @min(e1,e2,...)   The minimum of the values of the e's\0"
"     @nval(se,e)       The numeric value of a named cell.\0"
"     pi       @pi      A constant quite close to pi.\0"
"     @pow(e1,e2)       e1 raised to the power of e2.\0"
"     @rnd(e)           Round e to the nearest integer.\0"
"     @round(e,n)       Round e to n decimal places.\0"
"     @sqrt(e)          Square root of e.\0"
"     @ston(se)         Convert string expr se to a numeric\0"
"     @ln(e)   @log(e)           Natural/base 10 logarithm of e.\0"
"     @dtr(e)  @rtd(e)           Convert degrees to/from radians.\0"
"     @cos(e)  @sin(e)  @tan(e)  Trig functions of radian arguments.\0"
"     @asin(e) @acos(e) @atan(e) Inverse trig function.\0"
};

static const char stringf[] = {
" \0"
" N: String functions:\0"
"     #                 Concatenate strings.  For example, the\0"
"                       string expression \"A0 # \"zy dog\"\" yields\0"
"                       \"the lazy dog\" if A0 is \"the la\".\0"
"     @substr(se,e1,e2) Extract characters e1 through e2 from the\0"
"                       string expression se.  For example,\0"
"                       \"@substr(\"Nice jacket\" 4, 7)\" yields \0"
"                       \"e ja\".\0"
"     @fmt(se,e)        Convert a number to a string using sprintf(3).\0"
"                       For example,  \"@fmt(\"*%6.3f*\"10.5)\" yields\0"
"                       \"*10.500*\".  Use formats are e, E, f, g, and G.\0"
"     @sval(se,e)       Return the string value of a cell selected by name.\0"
"     @ext(se,e)        Call an external function (program or\0"
"                       script).  Convert e to a string and append it\0"
"                       to the command line as an argument.  @ext yields\0"
"                       a string: the first line printed to standard\0"
"                       output by the command.\0"
"     @coltoa(e)        Return the column letter(s) from the passed number\0"
"     @upper(e) @lower(e)   Return the string in upper/lower case\0"
"     @capital(e)       Return the string with words in upper case\0"
"     String expressions are made up of constant strings (characters\0"
"     surrounded by quotes), variables, and string functions.\0"
};

static const char finf[] = {
" \0"
" O: Financial functions:\0"
" \0"
"     @pmt(e1,e2,e3)    @pmt(60000,.01,360) computes the monthly\0"
"                       payments for a $60000 mortgage at 12%\0"
"                       annual interest (.01 per month) for 30\0"
"                       years (360 months).\0"
" \0"
"     @fv(e1,e2,e3)     @fv(100,.005,36) computes the future value\0"
"                       of 36 monthly payments of $100 at 6%\0"
"                       interest (.005 per month).  It answers the\0"
"                       question:  \"How much will I have in 36\0"
"                       months if I deposit $100 per month in a\0"
"                       savings account paying 6% interest com-\0"
"                       pounded monthly?\"\0"
" \0"
"     @pv(e1,e2,e3)     @pv(1000,.015,36) computes the present\0"
"                       value of an ordinary annuity of 36\0"
"                       monthly payments of $1000 at 18% annual\0"
"                       interest.  It answers the question: \"How\0"
"                       much can I borrow at 18% for 30 years if I\0"
"                       pay $1000 per month?\"\0"
};

static const char timef[] = {
" \0"
" P: Time and date functions:\0"
" \0"
"     @now              Return the time encoded in seconds since 1970.\0"
"     @dts(e1,e2,e3)    Return e1/e2/e3 encoded in seconds since 1970.\0"
"                       Either m/d/y or y/m/d may be used (year must\0"
"                       include the century), although y/m/d is preferred.\0"
"     @tts(h,m,s)       Return h:m:s encoded in seconds since midnight.\0"
" \0"
"     All of the following take an argument expressed in seconds:\0"
" \0"
"     @date(e)          Convert the time in seconds to a date\0"
"                       string 24 characters long in the following\0"
"                       form: \"Sun Sep 16 01:03:52 1973\".  Note\0"
"                       that you can extract pieces of this fixed format\0"
"                       string with @substr.\0"
"     @year(e)          Return the year.  Valid years begin with 1970.\0"
"     @month(e)         Return the month: 1 (Jan) to 12 (Dec).\0"
"     @day(e)           Return the day of the month: 1 to 31.\0"
"     @hour(e)          Return the number of hours since midnight: 0 to 23.\0"
"     @minute(e)        Return the number of minutes since the\0"
"                       last full hour: 0 to 59.\0"
"     @second(e)        Return the number of seconds since the\0"
"                       last full minute: 0 to 59.\0"
};

static int pscreen (const char* l)
{
    clear();
    for (short lineno = 1; *l; ++lineno, l += strlen(l)+1) {
	move (lineno, 4);
	addstr (l);
    }
    move (0,0);
    addstr ("Which Screen? [a-p, q]");
    refresh();
    return (nmgetch());
}

void help (void)
{
    int option;
    const char* ns = intro;
    while ((option = tolower(pscreen(ns))) != 'q') {
    	switch (option) {
	    default:
	    case 'a': ns = intro; break;
	    case 'b': ns = toggleoptions; break;
	    case 'c': ns = setoptions; break;
	    case 'd': ns = cursor; break;
	    case 'e': ns = cell; break;
	    case 'f': ns = vi; break;
	    case 'g': ns = file; break;
	    case 'h': ns = row; break;
	    case 'i': ns = range; break;
	    case 'j': ns = misc; break;
	    case 'k': ns = var; break;
	    case 'l': ns = rangef; break;
	    case 'm': ns = numericf; break;
	    case 'n': ns = stringf; break;
	    case 'o': ns = finf; break;
	    case 'p': ns = timef; break;
	}
    }
    FullUpdate++;
    move(1,0);
    clrtobot();
}
