/*****************************************************************************
 *                                                                           *
 * document.c - documentation that can be viewed from within cpat            *
 *                                                                           *
 *            Copyright (C) 2006 Trevor Carey-Smith                          *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify it   *
 * under the terms of the GNU General Public License version 2 as published  *
 * by the Free Software Foundation. (See the COPYING file for more info.)    *
 *                                                                           *
 *****************************************************************************/

#include "cpat.h"
#include "document.h"

/* Prints the help text for the current game */
void 
rules(GameInfo* g)
{
    switch(g->game) {
        case FCELL:     
            pager("FreeCell Rules","\
To begin the pack is dealt face up into 8 columns. \
There are also 4 vacant spaces (free cells) and 4 foundation piles. \
The objective is to get all the cards onto the foundations.\n\
\n\
Single exposed cards may be moved:\n\
  - column to column: placing the card on a card of the next rank \
and opposite colour (or onto an empty column).\n\
  - FreeCell to column: as above.\n\
  - column to vacant FreeCell.\n\
  - column to foundation: next card in order (Ace to King), \
one pile per suit.\n\
  - FreeCell to foundation: as above.\n\
\n\
How to play FreeCell:\n\
\n\
FreeCell is laid out with the columns [a-h] in the centre, the 4 \
foundation [p]iles on the left and the 4 vacant FreeCells in the [o]ther \
board on the right.\n\
\n\
For general instructions on moving cards around see the help text (press \
the '?' key in the main game window). The following applies specifically \
to Freecell.\n\
\n\
To move a card to a vacant freecell, the second letter in the move sequence \
should be 'o'. You can move more than one card at once to the freecells e.g.\
'3bo' moves the bottom three cards from the 'b' column to freecells (if \
there are enough).\n\
\n\
To move a card from a freecell, you need to designate which freecell \
the card is in by including a number between 1 and 4. \
For example, '2ob' moves the card from the second freecell to column 'b'. \
If only one freecell \
is occupied, this number can be left out.\n\
\n\
A sequence of cards can be moved from column to column by including the \
number of cards to move in the command e.g. '3dc' moves three cards from \
column 'd' to column 'c' (if there are enough freecells available). \n\
\n\
That's it. Enjoy!!\
",0,NULL);
            break;
        case KLOND:
            pager("Klondike Rules","\
To begin 28 cards are dealt face down into 7 columns with 7 cards in \
the first column, \
6 cards in the next, 5 in the next and so on down to 1. The bottom card in \
each column is turned face up.\n\
\n\
The four foundation piles are built up in suit from Ace to King and the \
columns on the tableau are built down by alternating colour. A sequence \
of cards can be moved from column to column if they are built down by \
alternating colour. An empty column can only be filled with a King.\n\
\n\
The remaining 24 cards can be dealt three at a time onto the waste. Cards \
can then be moved from the waste into play. In this version there is no \
limit to the number of times you can pass through the deck.\n\
\n\
How to play Klondike:\n\
\n\
Klondike is laid out with the columns [a-g] in the centre, the 4 \
foundation [p]iles on the left and the deck and waste on the [o]ther \
board on the right.\n\
\n\
For general instructions on moving cards around see the help text (press \
the '?' key in the main game window). The following applies specifically \
to Freecell.\n\
\n\
To turn over cards from the deck, the 'oo' command should be used. To move \
cards from the waste to columns or foundations, 'o' should be used as the \
first letter in the move command e.g. 'op' moves the top card from the \
waste to the appropriate foundation.\n\
\n\
A sequence of cards can be moved from column to column by including the \
number of cards to move in the command e.g. 3dc moves three cards from \
column 'd' to column 'c'. Actually, the number can be left out as \
Cpat will figure out how many cards to move by checking what \
card is on the top of the destination pile.\n\
\n\
That's it. Enjoy!!\
",0,NULL);
            break;
        case SPIDER:
            pager("Spider Rules","\
Two decks of cards are used in this game. To begin 54 cards are dealt face \
down in 10 columns (6 cards in each of the first 4 columns and 5 for the \
rest). The bottom card in each column is turned face up. \n\
\n\
The object is to move all cards to the 8 foundations. Cards can only be \
moved to the foundations as a complete sequence of 13 cards in suit \
running from King to Ace.\n\
\n\
Cards can be played on the tableau columns running down in rank and \
regardless of suit e.g. a 3C can be played on a 4H, however, only sequences \
running down in rank and of the same suit may be moved together. \
Any card can be placed on a vacant column.\n\
\n\
The remaining 50 cards are dealt out 10 at a time (one on each column) \
whenever required. However, there must be at least one card in each column \
prior to the deal.\n\
\n\
How to play Spider:\n\
\n\
Spider is laid out with the columns [a-j] in the centre, the 8 \
foundation [p]iles on the left and the deck on the [o]ther \
board on the right.\n\
\n\
For general instructions on moving cards around see the help text (press \
the '?' key in the main game window). The following applies specifically \
to Spider.\n\
\n\
To turn over cards from the deck, the 'oo' command should be used. \
This will deal out one card on each column. \n\
\n\
A sequence of cards can be moved from column to column by including the \
number of cards to move in the command e.g. 3dc moves three cards from \
column 'd' to column 'c'. \n\
\n\
That's it. Enjoy!!\
",0,NULL);
            break;
        case FORTYTH:
            pager("40 Thieves Rules","\
Two decks of cards are used in this game. To begin 10 columns of 4 cards \
each are dealt face up (the 40 Thieves).\n\
\n\
The object is to move all cards to the 8 foundations, which are built \
up in suit from Ace to King.\n\
\n\
Only one card can be moved at a time and cards are played on the tableau \
columns running down in suit (King to Ace). \
Any card can be placed on a vacant column.\n\
\n\
The remaining 64 cards are dealt one at a time to the waste and the deck \
can only be passed through once.\n\
\n\
How to play 40 Thieves:\n\
\n\
40 Thieves is laid out with the columns [a-j] in the centre, the 8 \
foundation [p]iles on the left and the deck and waste on the [o]ther \
board on the right.\n\
\n\
For general instructions on moving cards around see the help text (press \
the '?' key in the main game window). The following applies specifically \
to 40 Thieves.\n\
\n\
To turn over cards from the deck, the 'oo' command should be used. To move \
cards from the waste to columns or foundations, 'o' should be used as the \
first letter in the move command e.g. 'op' moves the top card from the \
waste to the appropriate foundation.\n\
\n\
That's it. Enjoy!!\
",0,NULL);
            break;
        case BAKERSD:
            pager("Baker's Dozen Rules","\
To begin, a single deck of 52 cards is dealt face up into 13 columns. \
Each column will contain 4 cards. During the deal, Kings are moved to the \
bottom of the column they are dealt to.\n\
\n\
The object is to move all cards to the 4 foundations, which are built \
up in suit from Ace to King.\n\
\n\
Only one card can be moved at a time and cards are played on the tableau \
columns running down from King to Ace in any suit. \
Once a column becomes vacant, no card can be placed there.\n\
\n\
How to play Baker's Dozen:\n\
\n\
Baker's Dozen is laid out with the columns [a-m] on the right and the 4 \
foundation [p]iles on the left.\n\
\n\
For general instructions on moving cards around see the help text (press \
the '?' key in the main game window). The are no specific instructions for \
Baker's Dozen.\n\
\n\
That's it. Enjoy!!\
",0,NULL);
            break;
        case SCORP:
            pager("Scorpion Rules","\
To begin, using a single deck of 52 cards, 7 cards are dealt into 7 \
columns. All cards are face up except for the bottom three cards on \
each of the first three columns. The three remaining cards are left \
in the deck.\n\
\n\
The object is to form four columns of suit sequence cards (King to Ace) on \
the tableau. These cards are not moved to separate foundation piles.\n\
\n\
Cards in the tableau are built down by suit and every face up card is \
available for play, no matter where in the column it is. That means that \
any card can be placed on top of a card of the same suit that is a rank \
higher. However, \
once a card from the bottom or middle of a column is moved, all cards \
on top of it are moved as well as one unit. Also, nothing can be placed \
on an ace and gaps on the tableau can only be taken by Kings or groups \
of cards with Kings as their bottom cards.\n\
\n\
When no more moves are possible, the three leftover cards are dealt \
onto the first three columns and put into play.\n\
\n\
How to play Scorpion:\n\
\n\
Scorpion is laid out with the columns [a-g] on the left and the \
remaining three cards in a deck on the [o]ther board on the right.\n\
\n\
For general instructions on moving cards around see the help text (press \
the '?' key in the main game window). The following applies specifically \
to Scorpion.\n\
\n\
Because each card has only one card that it is allowed to be played on, \
it is not necessary to specify the number of cards you want moved from \
one column to another, it is adequate to specify just the source and \
destination columns. The only exception is when moving a king to a vacant \
column and in this case when there is more than one king in the souce column, \
the number of cards to move needs to be specified.\n\
\n\
To deal the remaining 3 cards, use the 'oo' command.\n\
\n\
That's it. Enjoy!!\
",0,NULL);
            break;
        case PENGUIN:
            pager("Penguin Rules","\
To begin, a single deck of 52 cards is dealt face up into 7 \
columns, each containing 7 cards. The first card that is dealt is known \
as the beak, and when the (3) other cards of the same rank turn up, they \
are immediately moved to the foundation piles.\n\
\n\
The object is to move all cards from the tableau to the foundations. \
The foundations are built up in suit from the beak to the card that is \
one rank lower than the beak. E.g. if the beak is a 5, the foundations \
would run: 5,6,7,...,K,A,...,4.\n\
\n\
Cards in the tableau are built down by suit and can wrap around from Ace \
to King. Cards are moved one at a time, \
unless a suit sequence of cards is formed, which can be moved as a unit. \
When a space occurs, only a card one rank lower than the beak or a suit \
sequence starting with a such a card can be placed on it.\n\
\n\
There are also 7 free cells called the flipper, which can each store a \
single card.\n\
\n\
How to play Penguin:\n\
\n\
Penguin is laid out with the columns [a-h] in the centre, the 4 \
foundation [p]iles on the left and 7 flipper cells in the [o]ther \
board on the right.\n\
\n\
For general instructions on moving cards around see the help text (press \
the '?' key in the main game window). The following applies specifically \
to Penguin.\n\
\n\
To move a card to a vacant flipper cell, the second letter in the move \
sequence should be 'o'. You can move more than one card at once to the \
flipper e.g. '3bo' moves the bottom three cards from the 'b' column to \
the flipper (if there are enough free cells).\n\
\n\
To move a card from the flipper, you need to designate which cell \
the card is in by including a number between 1 and 7. If only one cell \
is occupied, this number can be left out.\n\
\n\
That's it. Enjoy!!\
",0,NULL);
            break;
        case YUKON:
            pager("Yukon Rules","\
To begin, using a single deck of 52 cards, 28 cards are dealt face down \
into 7 columns with 7 cards in the first column, \
6 cards in the next, 5 in the next and so on down to 1. The bottom card in \
each column is turned face up. The remaining cards are dealt face up onto \
the first four columns (four cards on each).\n\
\n\
The object is to move all cards from the tableau to the foundations \
which are are built up in suit from Ace to King. \
Cards in the tableau are built down by rank in alternating colour and every \
face up card is \
available for play, no matter where in the column it is. That means that \
any card can be placed on top of a card of the opposite colour that is a \
rank higher. However, \
once a card from the bottom or middle of a column is moved, all cards \
on top of it are moved as well as one unit. Also, nothing can be placed \
on an ace and gaps on the tableau can only be taken by Kings or groups \
of cards with Kings as their bottom cards.\n\
\n\
How to play Yukon:\n\
\n\
Yukon is laid out with the columns [a-g] in the centre and the \
foundation [p]iles on the left.\n\
\n\
For general instructions on moving cards around see the help text (press \
the '?' key in the main game window). The are no specific instructions for \
Yukon.\n\
\n\
That's it. Enjoy!!\
",0,NULL);
            break;
        case STRATEGY:
            pager("Strategy Rules","\
To begin, the Aces are removed from a single deck of 52 cards and placed \
on the foundations. The game is played in two parts, first cards are dealt \
from the deck onto any of the four tableau columns, irrespective of suit or \
rank. \
Once all the cards are dealt, they are then moved one at a time to the \
foundations which are built up in suit from Ace to King.\n\
\n\
The strategic part of the game is during the initial deal where you try \
not to cover lower ranked cards with higher ranked cards, especially if \
they are of the same suit.\n\
\n\
How to play Strategy:\n\
\n\
Strategy is laid out with the columns [a-i] in the centre, the four \
foundation [p]iles on the left and the deck on the [o]ther board on the \
right.\n\
\n\
For general instructions on moving cards around see the help text (press \
the '?' key in the main game window). When playing Stategy, cards can be \
moved from the deck to the columns by simply typing the letter of the \
requested column twice or by preceding that letter with an 'o' \
(i.e. 'cc' and 'oc' perform the same action).\n\
\n\
That's it. Enjoy!!\
",0,NULL);
            break;
        case CANFIELD:
            pager("Canfield Rules","\
To begin one card is dealt to a foundation pile and the other 3 foundation \
piles must begin with a card of this rank. One card is then dealt onto each \
of the four tableau columns and 13 cards are moved aside into a reserve \
pile (the lower pile in the right-hand side column). \
The remaining 34 cards can be dealt three at a time onto the waste.\n\
\n\
The four foundation piles are built up in suit and can wrap from King to \
Ace. The columns on the tableau are built down by alternating colour and \
can wrap from Ace to King. Cards can be moved from column to column either \
singly or as an entire column, but not a partial column. When a tableau \
stack becomes empty, a card from the reserve is moved to that column \
automatically.\n\
\n\
How to play Canfield:\n\
\n\
Canfield is laid out with the columns [a-d] in the centre, the 4 \
foundation [p]iles on the left and the deck, waste and reserve piles on the \
[o]ther board on the right.\n\
\n\
For general instructions on moving cards around see the help text (press \
the '?' key in the main game window).\n\
\n\
That's it. Enjoy!!\
",0,NULL);
            break;
        case DUCHESS:
            pager("Duchess Rules","\
To begin one card is dealt onto each of the four tableau columns and \
3 cards are dealt face-up onto each of the four reserve piles. \
The first move of the game is move a card from one of the reserve piles to \
a foundation pile. The other 3 foundation piles must begin with a card of \
this rank. The remaining 36 cards can be dealt one at a time onto the waste \
and this pile can only be dealt through twice.\n\
\n\
The four foundation piles are built up in suit and can wrap from King to \
Ace. The columns on the tableau are built down by alternating colour and \
can wrap from Ace to King. Cards can be moved from column to column either \
singly or as a sequence. When a tableau stack becomes empty, it must be \
filled with a card from the reserve (not necessarily immediately). \
If the reserve is empty, an empty tableau \
stack can only be filled with a card or sequence of cards from another \
tableau stack (i.e. cards from the waste pile can only be moved directly to \
a foundation pile or onto an existing card on the tableau).\n\
\n\
How to play Duchess:\n\
\n\
Duchess is laid out with the tableau columns [a-d] in the centre-left panel, \
the reserve piles [f-i] in the centre-right panel, the 4 foundation [p]iles \
on the left and the deck and waste pile on the [o]ther board on the right.\n\
\n\
For general instructions on moving cards around see the help text (press \
the '?' key in the main game window).\n\
\n\
That's it. Enjoy!!\
",0,NULL);
            break;
    }
}

/* Prints the movement help text */
void 
help()
{
    pager("Help","\
CPat - a curses-based solitaire suite\n\
\n\
In Game Options\n\
\n\
When playing a solitaire game, a number of commands are available at \
all times:\n\
 '?'  Prints what you are currently reading.\n\
 'r'  Will print the rules of the current game.\n\
 'u'  Will undo the previous move if this is allowed. You can undo only \
those moves that do not reveal any new information (e.g. moving \
cards from one column to another).\n\
 't'  Will show credit where it is due.\n\
 'w'  Will print license and warranty information.\n\
 's'  Will redraw the screen from scratch, in case something goes \
wrong with the display.\n\
 'q'  Will quit the current game and give you some statistics about \
the game you played (you will actually get the option to resume the \
game again).\n\
\n\
How To Move Cards Around:\n\
\n\
CPat is laid out with the general playing board in the centre of the screen. \
Each column on the main board is labelled with a letter starting at 'a' \
going up to the number of columns in the particular game you are playing. \
Most of the games have foundation piles and these are placed in the board \
on the left side of the screen. These piles are labelled 'p' for foundation \
[p]iles. On the right hand side there is usually another board which is used \
for different purposes depending on the game. \
This is labelled 'o' for [o]ther.\n\
\n\
Most move commands consist of a 2 character sequence proceeded by an \
optional number. \
The first character denotes the source card and the second the \
destination pile. For example 'ad' moves the bottom card from column 'a' \
onto column 'd', 'bp' moves the bottom card from column 'b' onto the \
appropriate foundation pile (depending on its suit).\n\
\n\
In general, a number is included in the command to signify the number of \
cards to move when moving a stack or sequence of cards. \
For example, '3bf' will move a stack of 3 cards from column 'b' to 'f'. \
The type of stacks \
you are allowed to move depends on the game you are playing. \n\
\n\
The 'o' command is used to manipulate cards on the \"other\" board. This \
will depend on the game you are playing. In Klondike type games where \
there is a deck of cards to look through, two 'o' characters will turn \
cards over onto the waste pile and an 'o' followed by another letter will \
move a card off the waste pile. In Freecell type games, the \"other\" board \
contains the freecells and cards can be moved to and from these piles in the \
normal way, however when moving a card from the freecells a number is \
required to designate which card you want to move.\n\
\n\
Cards can also be moved \
to the foundation piles automatically. To do this you use only the \
letter 'p'. Two 'p' characters will move one card (or pile depending on \
the game being played) to the foundation. If the 'pp' is proceeded by \
a number, a maximum of that many cards (piles) will be moved at once. \
A capital 'P' will move as many \
cards (piles) as possible. There is no particular order \
to the way the cards are moved.\n\
\n\
There are a number of mechanisms to allow the length of a column to grow \
beyond the height  of your terminal. Normally, cards have a one line space \
between them, but this is removed (from the top down) as the number of cards \
exceed the terminal height. If this is not enough, then cards of the same \
suit in sequence are replaced by the end cards of the sequence separated \
by the letters 'SEQ'. This normally allows enough space for card columns \
(unless you have reduced your terminal size much below the standard 80x25), \
however if these steps are not adequate (e.g. in the 'Scorpion' game), \
then the column will do a 'U-turn' at the bottom and head back up the \
board using the two free spaces on the right of the column. There is \
enough room here to display all cards except the '10' which is depicted \
with a '0', e.g. the ten of clubs will be '0C'. If the column carries on \
beyond the top of the screen, then you're out of luck!!.\
",0,NULL);
}

/* Prints license info */
void 
//license(GameInfo* g)
license()
{
    pager("License","\
CPat - a curses-based solitaire game\n\
\n\
Copyright (C) 2006 Trevor Carey-Smith\n\
\n\
This program is free software; you can redistribute it and/or \
modify it under the terms of the GNU General Public License \
version 2 as published by the Free Software Foundation.\n\
\n\
This program is distributed in the hope that it will be useful, \
but WITHOUT ANY WARRANTY; without even the implied warranty of \
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the \
GNU General Public License for more details.\n\
\n\
You should have received a copy of the GNU General Public License \
along with this program (the COPYING file); if not, write to the \
Free Software Foundation, Inc., 59 Temple Place, Suite 330, \
Boston, MA  02111-1307  USA\
",0,NULL);
}

/* prints credits */
void 
credits()
{
    pager("Credits","\
CPat - a curses-based solitaire game\n\
\n\
The basic idea and bare bones for this code come from a card game \
called bluemoon, which was written by T. A. Lister and Eric S. Raymond. \
See: http://www.catb.org/~esr/bluemoon/\n\
\n\
This implementation was written by Trevor Carey-Smith <trev@highwater.co.nz>\n\
\n\
Some of the text for the game's rules was taken from \
wikipedia: http://wikipedia.org/wiki/List_of_solitaire_card_games",0,NULL);
}

/* document.c ends here */
