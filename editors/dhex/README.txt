-----------------------------------------------------------------[ 0. Welcome ]-

Thank you for downloading dhex 0.67. It is a hex editor with a diff mode.
Since it is based on ncurses, it runs in any console.
It makes heavy use of colors, but it is themeable to run on monochrome-displays
as well.

If you have any questions, or found any bugs, please do not hesitate to contact
me under dettus@dettus.net. Be sure to include "DHEX" somewhere in the subject.


Thomas Dettbarn, 30-Dec-2010

0.60: Initial release
0.61: Some code cleanup, rudementary manual correlation is possible through 
      commandline parameters
0.62: Code cleanup, bug fixes, and as new features: bookmarks and ascii-search.
0.63: Man pages were written, searching is possible from the command line.
0.64: Naive correlation, a new default theme
0.65: dhex 0.64 crashed at startup on some systems.
0.66: searching for an ascii string with spaces works, some problems with the
      color themes were fixed
0.67: a "base address" functionality was added, which makes it easier to work
      with memory dumps.
0.68: slight bug fix as suggested by christoph: under some circumstances, the
      opening of a file could segfault. 

---------------------------------------------------------[ 1. Getting started ]-

Download the latest version from http://www.dettus.net/dhex/dhex_latest.tar.gz.
Unpack it,

% tar xvfz dhex_latest.tar.gz

then compile it

% make

This should produce an executable. It requires a file as a paramter to start

% ./dhex gpl.txt

When starting for the first time, dhex will attempt to create a .dhexrc file 
in your $HOME-directory. If you already have a .dhexrc from an older version of
dhex, it will convert it. 


The first screen you'll see will be the keyboard setup. (I was not satisfied 
with the way ncurses handled the function keys, so i created something new)
The keyboard setup will be written into the .dhexrc file.


You can edit the file, search for short strings in it, and scroll through it.
After pressing F10 (or whichever key you pressed instead of F10 during the 
keyboard setup), dhex will ask you whether or not to save the changes.


To invode the keyboard setup manually, just start dhex with the parameter -k

% ./dhex -k gpl.txt



---------------------------------------------------------------[ 2. Diff mode ]-


When you run dhex with two files, 

% ./dhex gpl.txt dhex

it will start in the diff mode. This is useful for file comparison. Any 
difference in the files will be highlighted. To jump from one difference to 
the next, press F3. To go to the previous one, press F4.


--------------------------------------------------------------------[ 3. Goto ]-

Pressing F1 will open up the "Goto" dialog. Here you have three possibilities:
Pressing = will set the cursor to the absolute address you're about to type in,
Pressing + will advance the cursors those many bytes,
Pressing - will go back those many bytes.

Press "Go" to set the cursor, or "Cancel" to quit the dialog.


Moreover, you can set up to ten bookmarks here. And save them for later use.



---------------------------------------------------------------[ 4. Searching ]-

When not in diff mode, pressing F2 will invoke the "Search" dialog. You can
search forward or backward. When selecting the "Writelog" option, you'll have
to provide a filename to write the search results to. 
Later, when you select the "Readlog" option, it'll read the cursor positions 
where to search from this file. Otherwise dhex will search the whole file.

When not selecting the "Writelog" option, the search dialog will immediately
close upon finding the first occuracnce, and set the cursor there.

Press F3 to go to the next occurace, or F4 to the previous one.

Searching is also possible from the command line: Simply apply the -sa, -sab, 
-sh, -shb parameters. To write into a searchlog, -w is the parameter to go. To 
read from it, -r can be used. For example

% ./dhex -oh affe -shb d00f dhex

sets the cursor to 0xaffe, and searches for the hex string "d0 0f" backwards 
from there.

% ./dhex -oh affe -shb d00f -w occurances.log dhex 

writes the results into a logfile called "occurances.log" and exits.

-----------------------------------------------------------------[ 5. Hexcalc ]-

Pressing F5 will open up the hexcalc dialog. This is a small 64-bit calculator.
Simply type in a value in one of the formats "decimal", "hexadecimal" or
"binary", chose an operation, press ENTER and see the results.
You can select any former result by going up a few lines with the cursor keys.



------------------------------------------------------------[ 6. config-files ]-

The default config file is $HOME/.dhexrc. Another config file can be chosen
with the -f option:

% ./dhex -f /etc/globaldhexrc gpl.txt

Basically, a config file is broken down into two sections: Colors and keyboard
conversion tab. The default config file on a MacBook looks like this:

DHEXCOLORSCHEME
#VERSION 0
#possible colors are: BLACK,GREEN,GREEN,YELLOW,BLUE,MAGENTA,CYAN,BLACK
#possible extra flags are: UNDERLINE,REVERSE,BLINK,DIM,BOLD

BRACKETS:       FG=BLACK,BG=BLACK,BOLD
HEXFIELD:       FG=WHITE,BG=BLACK
INPUT:          FG=BLACK,BG=WHITE
CURSOR:         FG=WHITE,BG=BLACK
TEXT:           FG=WHITE,BG=BLACK,BOLD
MENU_NORMAL:    FG=CYAN,BG=BLACK
MENU_HIGHLIGHT: FG=BLACK,BG=CYAN
MENU_HOTKEY:    FG=YELLOW,BG=BLACK,BOLD
MENU_HOTKEY_HI: FG=YELLOW,BG=CYAN,BOLD
FRAME:          FG=BLUE,BG=BLACK,BOLD
NORMAL_DIFF:    FG=YELLOW,BG=BLACK
CURSOR_DIFF:    FG=YELLOW,BG=WHITE,BOLD
HEADLINE:       FG=BLUE,BG=BLACK,BOLD
	
KEYF1:c2 a1 
KEYF2:e2 84 a2 
KEYF3:c2 a3 
KEYF4:c2 a2 
KEYF5:e2 88 9e 
KEYF6:c2 a7 
KEYF7:c2 b6 
KEYF8:e2 80 a2 
KEYF9:c2 aa 
KEYF10:c2 ba 
KEYESC:1b 
KEYBACKSPACE:08 
KEYDEL:1b 
KEYENTER:0a 
KEYTAB:09 
KEYUP:1b 5b 41 
KEYDOWN:1b 5b 42 
KEYRIGHT:1b 5b 43 
KEYLEFT:1b 5b 44 
KEYPGUP:02 
KEYPGDOWN:06 
KEYHOME:5e 
KEYEND:24 

(Actually, in this example ALT+1, ALT+2, ALT+3,... ALT+0 were used instead of 
F1..F10) You can edit it with any texteditor you like.



--------------------------------------[ 7. Correlating two files in diff mode ]-

Lets assume you would like to compare two files. One of those files starts with
garbage at the beginning:

  file1: GARBAGE0123456789ABCDEF
  file2:        0123456789ABCDEF

GARBAGE has 7 letters, so to skip over the first 7 bytes in the first file, just
apply the following paramters:

% dhex -o1d 7 file1 file2

-o sets the cursor offset. -o1 means that the first buffer is affected. -o1d 
gives the radix for the offset. it can either be -o1d for decimal, -o1b for
binary, -o1h for hexadecimal or -o1o for octal.

If the garbage is in the second file, just use

% dhex -o2d 7 file1 file2

Since version 0.64 there is also a way of doing this automatically: Pressing
F6 in the GUI will open up a dialog where you can select one of three
correlation algorithms. They are painfully slow, but they'll do the job.
Those three algorithms are:

- best match
  move the two files in such a way that as many bytes match as possible
- longest match
  move the two files in such a way that as many consecutive bytes match as
  possible
- minimum diff
  move the two files in such a way that as little bytes differ as possible. This
  algorithm requires an upper limit to improve its speed.

To invoke correlation from the command line, use one of these

% dhex -cb file1 file2  # best match
% dhex -cl file1 file2  # longest match
% dhex -cd 1000 file1 file2 # minimum difference, with the upper limit of 1000.
  


--------------------------------------------------------------[ 8. Disclaimer ]-

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


--------------------------------------------------------------[ 9. Known bugs ]-

- Searching has not been thourughly tested.
- Code is not clean (yet)
- Searching is only possible in normal mode, not diff mode
- Searching ignores unsaved changes.
- Some features are missing
- -k expects a file to be opened
- -f might overwrite ANY file.
- GOTO can jump way behind the end of the file
- some ways of exiting won't free all the allocated buffers
