/*  an v0.95 - Anagram generator
    Copyright (C) 1996, Free Software Foundation.
    Copyright (C) 1995,1996  Richard Jones
    Copyright (C) 2001,2002  Paul Martin <pm@debian.org>

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
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

    Mail suggestions and bug reports to:
    richard@deep-thought.org */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
/*#include <math.h>*/
#include <getopt.h>
#include <assert.h>

#ifdef __GLIBC__
typedef int (*gan_qsort_t) (const void *, const void *);
#define gan_qsort qsort
#else
#include "gan.h"
#endif

#ifdef UINT64_MAX
#define HAVE64
#endif

#ifdef FORCE_LONG
#undef HAVE64
#endif

#ifdef HAVE64
#define bitmask_t	uint64_t
#else
#define bitmask_t	unsigned long int
#endif

#if ENABLE_NLS
# include <libintl.h>
# define _(Text) gettext (Text)
#else
# undef bindtextdomain
# define bindtextdomain(Domain, Directory)	/* empty */
# undef textdomain
# define textdomain(Domain)	/* empty */
# define _(Text) Text
#endif


#define max(a,b) ((a) >= (b) ? (a) : (b))

#define VERSION "an v0.95 - May 6 2002"
#define TRUE 1
#define FALSE 0

#define DEFDICT "/usr/share/dict/words"

#define MAXLINE 512		/* Maximum line length when reading DICTIONARY */
#define MAXWORDS 512		/* Maximum number of words in anagram */
#define MAX_WORD_LEN MAXLINE	/* Maximum length of any word */
#define BLOCKSIZE 512
#define BIG_INT (2<<16)		/* Magic */

void lowercase (char *string);
void remove_punc (unsigned char *string);
int check_letter (unsigned char letter, unsigned char *phrase_word, int *avail);
int check_word (unsigned char *current_word, unsigned char *phrase_word);
void check_dict (int phrase_len, bitmask_t *phrase_mask,
		 int num_phrase_masks, bitmask_t **word_mask,
		 int level, int entry_point, int old_high_letter_num);
int remove_letters (unsigned char *remove_phrase, unsigned char *phrase_word);
int get_words (unsigned char *dict_file, unsigned char *phrase_word,
	       FILE * dict);
void make_word_masks (unsigned char **words, bitmask_t **word_mask,
		      int *num_word_masks, int dict_words);
int makefreq_table (int *freq_table, unsigned char *word);
int compare_len (char *string1[], char *string2[]);
int no_punc_len (unsigned char *string);
void *xmalloc (size_t size);
void *xcalloc (size_t num_objects, size_t size);
void *xrealloc (void *location, size_t newsize);
char *xstrdup (char *s);
void create_let_hash (bitmask_t **word_mask, int dict_words,
		      bitmask_t *letter_mask, int *let_hash);
void calc_letter_freq (unsigned char **words, int *letter_freq, int dict_words);
int compare_char (unsigned char *let1, unsigned char *let2);
int compare_word (unsigned char *word1[], unsigned char *word2[]);
int freq_strcmp (unsigned char *string1, unsigned char *string2);
void create_letter_index (int *letter_index);
int compare_let_index (int *let1, int *let2);
void create_word_len_hash (int **word_len_hash, int *word_len, int *let_hash,
			   int phrase_len);
void print (int *found, int level, int upto);


int contain_flag = FALSE;	/* flag set if -c option used  */
int words_in_contain_phrase = 1;	/* Number of words in CONTAIN_PHRASE */
int min_word_len = 0;		/* minimum word length to use in anagram */
int length_of_anagram = MAXWORDS;	/* Maximum number of words in anagram */
int max_num_anagrams = FALSE;	/*Maximum number of anagrams to output */
int num_anagrams = 0;		/* Number of anagrams found */
int output_words = FALSE;	/* output words option flag */
unsigned char **words;		/* array of words to make anagrams from */
int **word_len_hash;		/* length & letter index into words array */
int maxword_len = 0;		/* Maximum word length in words array */

char length_option = FALSE;
unsigned char *contain_phrase = 0;	/* pointer to contain phrase */

int *num_word_masks;		/* array of number of bit masks for each word */
int *let_hash;
int *word_len;			/* array of word lengths */
int found[MAXWORDS];		/* array of words found during recursion */
bitmask_t *letter_mask;		/* bit masks for letters */
int alphabet = 0;		/* number of letters in alphabet */
int letters[256];		/* list of letter positions used */
int letterpos[256];		/* list of letter positions */

int *letter_freq;		/* Frequency each letter appears in words array */
int *letter_index;
int *letter_index2;


int main (argc, argv)
     int argc;
     char *argv[];
{
   FILE *dict;			/* file pointer to dictionary file */
   int dict_words = 0;		/* number of words in new dictionary */
   int loop1;			/* loop counter */
   int loop2;
   unsigned char *phrase_word = 0;	/* array for users phrase */
   unsigned char *old_phrase_word;	/* PHRASE_WORD before processing */
   int level = 0;		/* level of current recursion */
   int phrase_len;		/* length of users phrase */
   int *freq_table;		/* array of letter frequencies for current word */
   bitmask_t **word_mask;	/* array of bit masks for letters in each word */

   int num_phrase_masks;	/* number of bit masks for users phrase */
   bitmask_t *phrase_mask = 0;	/* array of bit masks for letters in users phrase */

   int opt;			/* return value from getopt */

   int used_flag = FALSE;	/* used phrase option flag */
   int test_flag = FALSE;	/* test option flag */
   char *dict_file = NULL;	/* pointer to dictionary file name */
   unsigned char *test_phrase = 0;	/* pointer to test phrase */
   unsigned char *old_test_phrase;	/* TEST_PHRASE before processing */

   unsigned char *used_phrase = 0;	/* pointer to used phrase */
   int option_index;		/* index into LONG_OPTIONS structure */
   bitmask_t mask;



   struct option long_options[] =	/* long options structure for getopt */
   {
      {"contain", required_argument, 0, 'c'},
      {"dict", required_argument, 0, 'd'},
      {"length", required_argument, 0, 'l'},
      {"minimum", required_argument, 0, 'm'},
      {"number", required_argument, 0, 'n'},
      {"words", no_argument, &output_words, TRUE},
      {"help", no_argument, 0, 'h'},
      {"used", required_argument, 0, 'u'},
      {"test", required_argument, 0, 't'},
      {"version", no_argument, 0, 'v'},
      {0, 0, 0, 0}
   };

   /* Process short and long options */

   while ((opt = getopt_long (argc, argv, "c:d:l:m:n:t:u:w", long_options,
			      &option_index))
	  != -1) {
      switch (opt) {
      case 'c':
	 contain_flag = TRUE;
	 contain_phrase = optarg;
	 break;
      case 'd':
	 dict_file = optarg;
	 break;
      case 'l':
	 length_of_anagram = atoi (optarg);
	 length_option = TRUE;
	 break;
      case 'm':
	 min_word_len = atoi (optarg);
	 break;
      case 'n':
	 max_num_anagrams = atoi (optarg);
	 break;
      case 't':
	 test_flag = TRUE;
	 test_phrase = optarg;
	 break;
      case 'u':
	 used_flag = TRUE;
	 used_phrase = optarg;
	 break;
      case 'w':
	 output_words = TRUE;
	 break;
      case 'h':
	 printf (_("Usage: %s [OPTION] PHRASE\n\n"), argv[0]);
	 printf (_("  -c, --contain PHRASE\tprint anagrams containing PHRASE\n"));
	 printf (_("  -d, --dict DICTIONARY\tsearch DICTIONARY for words\n"));
	 printf	(_("  -l, --length WORDS\tfind anagrams with up to WORDS number of words\n"));
	 printf (_("  -m, --minimum WORDLEN\tonly use words at least WORDLEN long\n"));
	 printf (_("  -n, --number NUM\tprint maximum of NUM anagrams\n"));
	 printf (_("  -w, --words\t\tprint words that PHRASE letters make\n"));
	 printf (_("  -t, --test ANAG\ttest if ANAG can be made with PHRASE\n"));
	 printf (_("  -u, --used PHRASE\tFlag PHRASE letters allready used\n"));
	 printf (_("      --help\t\tdisplay this help and exit\n"));
	 printf (_("      --version\t\toutput version information and exit\n"));
#ifdef HAVE64
	 printf (_("       64 bit support\n"));
#endif
	 printf ("\n");
	 exit (0);
      case 'v':
	 printf ("%s\n", VERSION);
	 exit (0);
      case '?':
	 fprintf (stderr, _("Try '%s --help' for more information\n"), argv[0]);
	 exit (1);
	 break;
      default:
	 printf ("%c\n", optopt);
	 abort ();
      }

   }

   /*
    * If there is not one argument remaining after processing,
    * there is an incorrect number of arguments
    */

   if (optind != argc - 1) {
      fprintf (stderr, _("%s: incorrect number of arguments\n"), argv[0]);
      fprintf (stderr, _("Try '%s --help' for more information\n"), argv[0]);
      return 1;
   }

   /* initialise locale */
#ifdef HAVE64
   setlocale (LC_CTYPE, "");
#else
   setlocale (LC_CTYPE, "C");
#endif

   mask = 1;
   for (loop1 = 0; loop1 < 256; loop1++) {
     letterpos[loop1] = -1;
     if (islower(loop1)) {
      if (mask == 0) {
        fprintf(stderr,_("Not enough bits in bitmask type to work on your language.\n"));
	return 1;
      }
      letters[alphabet]=loop1;
      letterpos[loop1]=alphabet;
      alphabet++;
      mask <<= 1;
     }
   }

   letter_mask = xcalloc (alphabet, sizeof(bitmask_t));
   for (loop1 = 0; loop1 < alphabet; loop1++) {
     letter_mask[loop1] = (bitmask_t) 1 << (alphabet - loop1 - 1);
   }
   
   letter_freq = xcalloc (alphabet, sizeof (int));
   letter_index = xcalloc (alphabet, sizeof (int));
   letter_index2 = xcalloc (alphabet, sizeof (int));
   freq_table = xcalloc (alphabet, sizeof (int));
   let_hash = xcalloc (alphabet+2, sizeof (int));



   /* Copy phrase to make anagrams from into PHRASE_WORD and OLD_PHRASE_WORD */

   phrase_word = xstrdup (argv[optind]);
   old_phrase_word = xstrdup (phrase_word);

   /* Ensure PHRASE_WORD is lowercase and has no punctuation */

   lowercase (phrase_word);
   remove_punc (phrase_word);

   /* If test option selected check if TEST_PHRASE can be made with
      letters in PHRASE_WORD */

   if (test_flag) {
      old_test_phrase = xstrdup (test_phrase);
      lowercase (test_phrase);
      remove_punc (test_phrase);
      if (check_word (test_phrase, phrase_word) && (strlen (test_phrase) ==
						    strlen (phrase_word)))
	 printf (_("'%s' is an anagram of '%s'\n"), old_test_phrase,
		 old_phrase_word);
      else
	 printf (_("'%s' is not an anagram of '%s'\n"), old_test_phrase,
		 old_phrase_word);
      exit (0);
   }


   /* If contain option is set remove letters in CONTAIN_PHRASE from those
      int PHRASE_WORD, exit after printing error message if letters in
      PHRASE_WORD cannot make CONTAIN_PHRASE.  Also count the number of
      words in the CONTAIN_PHRASE and store in WORDS_IN_CONTAIN_PHRASE */

   if (contain_flag) {
      if (!remove_letters (contain_phrase, phrase_word)) {
	 printf (_("%s: '%s' cannot be made with letters available\n"),
		 argv[0], contain_phrase);
	 exit (1);
      }

      /* Count number of words in CONTAIN_PHRASE by checking for spaces */

      for (loop1 = 0; loop1 < strlen (contain_phrase); loop1++) {
	 if (isspace (contain_phrase[loop1]))
	    words_in_contain_phrase++;
      }
      if (length_option)
	 length_of_anagram -= words_in_contain_phrase;
   }



   /* If used option is set, remove letters in USED_PHRASE from those
      int PHRASE_WORD, exit after printing error message if letters in
      PHRASE_WORD cannot make USED_PHRASE */


   if (used_flag) {
      if (!remove_letters (used_phrase, phrase_word)) {
	 printf (_("%s: '%s' cannot be made with letters available\n"),
		 argv[0], used_phrase);
	 exit (1);
      }
   }

   /* If no dictionary chosen, use default dictionary */

   if (!dict_file) {
      dict_file = DEFDICT;
   }

   /* Open dictionary file and exit if error on open */

   dict = fopen (dict_file, "r");
   if (!dict) {
      fprintf (stderr, _("%s: error reading dictionary file '%s'\n"), argv[0],
	       dict_file);
      exit (1);
   }

   /* Read in words from file DICT_FILE which can be made from letters 
      in PHRASE_WORD */

   dict_words = get_words (dict_file, phrase_word, dict);

   /* If no words found output error message and exit */

   if (dict_words < 1) {
      printf (_("%s: no words in '%s' can be made\n"), argv[0], dict_file);
      exit (1);
   }

   /* If OUTPUT_WORDS option is turned on exit now */

   if (output_words)
      exit (0);




   /* Sort word by length, shortest to largest, if same length sort
      alphabetically */



   calc_letter_freq (words, letter_freq, dict_words);


   gan_qsort (words, dict_words, sizeof (char *), (gan_qsort_t) compare_word);

   /* Set up indexes indicating order of letters from least to most
      frequent */

   create_letter_index (letter_index);



   /* Store length of words without punctuation in WORD_LEN[], avoids
      evaluating word length in recursive loop where time is critical.  */


   word_len = xmalloc ((dict_words + 1) * sizeof (int));
   for (loop1 = 0; loop1 < dict_words; loop1++) {
      word_len[loop1] = no_punc_len (words[loop1]);
      if (word_len[loop1] > maxword_len)
	 maxword_len = word_len[loop1];
   }
   word_len[dict_words] = word_len[dict_words - 1];


   /* allocate memory for WORD_MASK[][], an array to hold the bit masks for 
      each word, the first bit mask is all the letters appearing at least 
      once in the word, the second bit mask is all the letters appearing
      at least twice etc. Also allocate memory for NUM_WORD_MASKS[], an 
      array holding the number of masks for each word */

   num_word_masks = xmalloc ((dict_words + 1) * sizeof (int));
   word_mask = xcalloc (dict_words, sizeof (bitmask_t *));

   /* Create bit masks for letters appearing in each word */

   make_word_masks (words, word_mask, num_word_masks, dict_words);
   num_word_masks[dict_words] = 100;


   /* Create index into WORDS array based on each letter */

   create_let_hash (word_mask, dict_words, letter_mask, let_hash);


   /* Initialise FREQ_TABLE[], a 26 element array containing the number of 
      times each letter (from 'a'-'z') appears in a word */

   for (loop1 = 0; loop1 < alphabet; loop1++)
      freq_table[loop1] = 0;

   /* Setup FREQ_TABLE for PHRASE_MASK.  Store number of PHRASE_MASKs in 
      NUM_PHRASE_MASKS */

   phrase_len = strlen (phrase_word);
   num_phrase_masks = makefreq_table (freq_table, phrase_word);

   /* Allocate memory for PHRASE_MASK[] */

   phrase_mask = xcalloc ((num_phrase_masks), sizeof (bitmask_t));

   /* Set bit masks for users phrase as for WORDS[] */

   for (loop1 = 0; loop1 < alphabet ; loop1++) {
      while (freq_table[loop1] > 0) {
	 phrase_mask[freq_table[loop1] - 1] |= letter_mask[loop1];
	 freq_table[loop1]--;
      }
   }
   free(freq_table);


   /* Allocate memory for and calculate the WORD_LEN_HASH array which
      is an index into the WORDS array based on word length and word
      starting letter. */

   word_len_hash = xcalloc (phrase_len + 1, sizeof (int *));

   for (loop1 = 0; loop1 < phrase_len + 1; loop1++)
      word_len_hash[loop1] = xcalloc (alphabet, sizeof (int));

   create_word_len_hash (word_len_hash, word_len, let_hash, phrase_len);




   for (loop1 = dict_words; loop1 > 0; loop1--) {
      int same = TRUE;		/* Boolean value set TRUE if two words are
				   anagrams of eachother */
      if (num_word_masks[loop1] == num_word_masks[loop1 - 1]) {
	 for (loop2 = 0; loop2 < num_word_masks[loop1]; loop2++) {

	    if (word_mask[loop1][loop2] != word_mask[loop1 - 1][loop2]) {
	       same = FALSE;
	       break;
	    }
	 }
      }
      else
	 same = FALSE;
      if (same)
	 num_word_masks[loop1] = BIG_INT;
   }


   /* Recursively check dictionary for anagrams */

   check_dict (phrase_len, phrase_mask, num_phrase_masks, word_mask,
	       level, 0, 0);


   /* Return control to caller */

   return (0);

}



/* Main work done here.  A recursive function which loops through
   dictionary searching for words which can be made from PHRASE_MASK
   bits, If a hit is found the set bits in the found word are turned off
   in PHRASE_MASK and CHECK_DICT calls itself, repeating the process.
   If at any time the number of letters left after a match is 0 an anagram
   has been found and is output to STDOUT */

void
check_dict (phrase_len, phrase_mask, num_phrase_masks,
	    word_mask, level, entry_point, old_high_letter_num)
     int phrase_len;
     bitmask_t *phrase_mask;
     int num_phrase_masks;
     bitmask_t **word_mask;
     int level;
     int entry_point;
     int old_high_letter_num;
{

   int high_letter_num = 0;	/* MSB of PHRASE_MASK */
   int new;			/* Counter used in NEW_PHRASE_MASK creation */
   int len_check_val = 0;	/* Value used in length short-circuiting */
#ifdef FORCE_ANSI
   bitmask_t *new_phrase_mask =
     xmalloc (num_phrase_masks * sizeof (bitmask_t));
                                /* New PHRASE_MASK to 
				   pass to itself */
#endif

   assert(phrase_len>=0);	 
   /* Compilation with wrong datatype for bitmask_t can trigger this */
   
   if (length_option)
      len_check_val = length_of_anagram - level - 1;

   /* Find MSB in PHRASE_MASK (which corresponds to least frequent letter
      in dictionary which still apears in PHRASE_MASK */

   {
      int n;
      for (n = old_high_letter_num; n<alphabet; n++) {
	 if (letter_mask[n] & *phrase_mask) {
	    high_letter_num = n;
	    break;
	 }
      }
   }


   {
      int n;			/* Start point in WORDS array */
      int end=0;		/* End point in WORDS array */
      int high;			/* Loop counter */
      /* WORDS[] is sorted by letter frequency and length, from longest to 
         shortest, therefore we can jump to the first word of the length of
         the current number of letters left unprocessed in the phrase via the 
         word length hashtable */

      n = word_len_hash[phrase_len][high_letter_num];

      if (entry_point > n)
	 n = entry_point;

      /* Recursive routine only uses words which have the currently least
         frequent letter in them at each level of recursion */

      for (high = high_letter_num + 1; high < alphabet ; high++) {
	 end = let_hash[high];
	 if (end > n) break;
      }

      /* Search through all words containing current least frequent letter
         for anagrams */

      for (; n < end; n++) {
	 /* If the number of masks for this word is greater than the
	    number of masks for the phrase we know the word has more
	    letters than can be made with the phrase letters, therefore
	    go to next loop */
	 if (num_word_masks[n] > num_phrase_masks)
	    continue;

	 /* If length option set then short-circuit this recursion if we
	    dont have enough letters left to make the phrase */

	 if (length_option) {
	    if (((len_check_val * maxword_len) + word_len[n])
		< phrase_len) {

#ifdef FORCE_ANSI
	       free (new_phrase_mask);
#endif

	       return;
	    }
	 }

	 /* Compare the first masks of the word and the phrase, which both
	    contain all the letters used in their masks, if the WORD_MASK
	    has bits set which the PHRASE_MASK doesn't we know we don't
	    have a match, therefore skip to next loop, otherwise
	    continue check with rest of WORD_MASKs */

	 if ((*word_mask[n] & *phrase_mask) == *word_mask[n]) {
	    /* If NUM_WORD_MASKS is greater than 1 we must check the rest
	       of the masks, otherwise we know we have a match as we have
	       allready checked the first mask and don't need to do
	       anything else here as a match is assumed */

	    if (num_word_masks[n] > 1) {
	       int n2;
	       int word_ok = TRUE;

	       /* Assume word can be made unless find otherwise */

	       for (n2 = 1; n2 < num_word_masks[n]; n2++) {
		  /* Check if there are any bits set in WORD_MASK which
		     are not set in PHRASE_MASK, if so can't make this
		     word therefore set WORD_OK to FALSE */

		  if ((word_mask[n][n2] & phrase_mask[n2]) !=
		      word_mask[n][n2]) {
		     word_ok = FALSE;
		     break;
		  }
	       }
	       if (!word_ok)
		  continue;
	    }
	 }
	 else
	    /* If first WORD_MASK has bits set that PHRASE_MASK doesn't, 
	       can't make word, therefore skip to next loop */

	    continue;



	 /* If word can be made store index to word in FOUND[LEVEL], LEVEL
	    being the level of recursion we are currently at.  Then check
	    to see if we have an anagram and if so print in fashion dictated
	    by output options. If we don't have an anagram then create
	    new PHRASE_MASKs and calculate new number of letters in PHRASE_WORD 
	    and pass to new level of recursion */

	 /* Store words number in FOUND[] */

	 found[level] = n;

	 /* If PHRASE_LEN - WORD_LEN == 0 then we have used all letters
	    and hence have an anagram */

	 /* If we have an anagram print it as specified by output options */
	 if (phrase_len - word_len[n] == 0) {

	    /* Call recursive routine which outputs anagrams, allowing
	       for anagrams that contain words which are themselves
	       anagrams for other words */

	    print (found, level, 0);
	 }


	 /* No anagram yet, if maximum word length for anagram not reached
	    then create NEW_PHRASE_MASK[] and go to next level of recursion
	  */

	 else {
#ifndef FORCE_ANSI
	    bitmask_t new_phrase_mask[num_phrase_masks]; /* New PHRASE_MASK to 
							   pass to itself */
#endif
	    /* New number of PHRASE_MASKs initially set to 
	       NUM_PHRASE_MASKS */

	    int new_num_phrase_masks = num_phrase_masks;
	    bitmask_t tempmask = *word_mask[n];	/* temp WORD_MASK for making new PHRASE_MASK */


	    /* Loop through WORD_MASKs and create NEW_PHRASE_MASK by
	       unsetting bits in PHRASE_MASK which are set in WORD_MASK,
	       start at last PHRASE_MASK and work backwards trying to
	       eliminate a level of PHRASE_MASK if possible */

	    new = new_num_phrase_masks - 1;

	    while (tempmask) {
	       new_phrase_mask[new] = (tempmask | phrase_mask[new]) -
		  tempmask;
	       if (!new_phrase_mask[new]) {
		  new_num_phrase_masks--;
	       }
	       tempmask = tempmask ^
		  (phrase_mask[new] ^ new_phrase_mask[new]);
	       new--;
	    }
	    while (new >= 0) {
	       new_phrase_mask[new] = phrase_mask[new];
	       new--;
	    }
	    {
	       int n2;
	       for (n2 = 1; n2 < num_word_masks[n]; n2++) {

		  new = new_num_phrase_masks - 1;
		  tempmask = word_mask[n][n2];


		  while (tempmask) {
		     bitmask_t tmp_new_mask = new_phrase_mask[new];
		     new_phrase_mask[new] = (tempmask | new_phrase_mask[new])
			- tempmask;
		     if (!new_phrase_mask[new]) {
			new_num_phrase_masks--;
		     }
		     tempmask = tempmask ^
			(tmp_new_mask ^ new_phrase_mask[new]);
		     new--;
		  }
	       }
	    }

	    /* Go to next level of recursion, adding one to the recursion
	       level, and passing new PHRASE_LEN by subtracting WORD_LEN
	       from current PHRASE_LEN */

	    check_dict (phrase_len - word_len[n], new_phrase_mask,
			new_num_phrase_masks, word_mask,
			level + 1, n, high_letter_num);


	 }

      }
   }


#ifdef FORCE_ANSI
   free (new_phrase_mask);
#endif

   return;

}


void print (found2, level, upto)
     int *found2;
     int level;
     int upto;
{
   int loop1;

#ifndef FORCE_ANSI
   int new_found[level + 1];
#else
   int *new_found;
   new_found = xmalloc (sizeof (int) * (level + 1));
#endif

   memcpy (new_found, found2, sizeof (int) * (level + 1));



   if (contain_flag) {
      fputs (contain_phrase, stdout);
      fputs (" ", stdout);
   }

   for (loop1 = 0; loop1 <= level; loop1++) {
      fputs (words[found2[loop1]], stdout);
      fputs (" ", stdout);
   }
   fputs ("\n", stdout);

   if (max_num_anagrams) {
      num_anagrams++;
      if (num_anagrams >= max_num_anagrams)
	 exit (1);
   }


   /* Check if any words are anagrams of other words, if so set word
      to the next word it is an anagram of and call self. */

   for (loop1 = upto; loop1 <= level; loop1++) {
      if (num_word_masks[found2[loop1] + 1] == BIG_INT) {
	 new_found[loop1]++;
	 print (new_found, level, loop1);
	 new_found[loop1]--;
      }
   }

#ifdef FORCE_ANSI
   free (new_found);
#endif

}


/* Check to see if CURRENT_WORD can be made with letters in PHRASE_WORD,
   returns TRUE if it can make it, FALSE otherwise */

int check_word (current_word, phrase_word)
     unsigned char *current_word;
     unsigned char *phrase_word;
{
   int loop1;			/* Loop counter */
   int phrase_len = strlen (phrase_word);	/* Length of word checking against */

#ifndef FORCE_ANSI
   int avail[phrase_len];
#else
   int *avail;			/* Letters available for use */
#endif

   int useit;			/* TRUE if we can use current letter */
   unsigned char letter;	/* Current letter being checked */



   /* Initialise AVAIL[], setting all letters available (TRUE) */

#ifdef FORCE_ANSI
   avail = xmalloc (phrase_len * sizeof (int));
#endif

   for (loop1 = 0; loop1 < phrase_len; loop1++)
      avail[loop1] = TRUE;

   /* Loop through letters in CURRENT_WORD checking if each letter is
      available for use, if not stop looping and reutrn FALSE */

   for (; *current_word != '\0'; current_word++) {
      letter = *current_word;

      /* If letter is punctuation go to next letter */

      if (letterpos[(int)letter] == -1)
	 continue;

      /* Check if letter is available for use (useit==TRUE) */

      useit = check_letter (*current_word, phrase_word, avail);

      /* If letter not available return FALSE */

      if (!useit) {

#ifdef FORCE_ANSI
	 free (avail);
#endif

	 return (FALSE);
      }
   }

   /* If all letters in CURRENT_WORD match letters available in 
      PHRASE_WORD return TRUE */

#ifdef FORCE_ANSI
   free (avail);
#endif

   return (TRUE);
}



/* Check if LETTER is contained in PHRASE_WORD and has not been used
   allready */

int check_letter (letter, phrase_word, avail)
     unsigned char letter;
     unsigned char *phrase_word;
     int *avail;
{

   /* Loop through letters in PHRASE_LEN searching for a match with LETTER */

   for (; *phrase_word != '\0'; phrase_word++) {
      /* If LETTER matches *PHRASE_WORD and letter is available return
         TRUE */

      if ((*phrase_word == letter) && (*avail == TRUE)) {
	 *avail = FALSE;
	 return (TRUE);
      }
      avail++;
   }

   /* Letter not found, return FALSE */

   return (FALSE);
}


/* Extract letters in REMOVE_PHRASE from PHRASE_WORD */

int remove_letters (remove_phrase, phrase_word)
  unsigned char *remove_phrase;
  unsigned char *phrase_word;
{

   int loop2;
   int phrase_len;
   int next;
   int word_ok;

   lowercase (remove_phrase);

   /* Check if REMOVE_PHRASE can be made with letters available
      and return FALSE if not */

   word_ok = check_word (remove_phrase, phrase_word);

   if (!word_ok) {
      return (FALSE);
   }

   /* Remove REMOVE_PHRASE letters from PHRASE_WORD,by looping
      through letters in REMOVE_PHRASE and finding that letter in
      PHRASE_WORD then removing that letter by moving the rest of
      PHRASE_WORD up one position */

   for (; *remove_phrase != '\0'; remove_phrase++) {
      next = 0;
      phrase_len = strlen (phrase_word);

      /* Loop through PHRASE_WORD looking for current letter in 
         REMOVE_PHRASE */

      while (next < phrase_len) {
	 /* If PHRASE_WORD letter == REMOVE_PHRASE letter move the
	    rest of PHRASE_WORD up one position */

	 if (phrase_word[next] == *remove_phrase) {
	    for (loop2 = next + 1; loop2 < phrase_len + 1; loop2++)
	       phrase_word[loop2 - 1] = phrase_word[loop2];
	    break;
	 }
	 next++;
      }
   }

   return (TRUE);
}



/* Get words which can be made with the given phrase from DICT_FILE and
   store in WORDS array */

int get_words (dict_file, phrase_word, dict)
     unsigned char *dict_file;
     unsigned char *phrase_word;
     FILE *dict;
{
   int num_words = 0;
   int size;
   int block;
   char line[MAXLINE + 1];
   int phrase_len;
   int word_ok;
   char *temp_string;
   int have_a_letter;
   int have_a_digit;

   /* Initial allocation of WORDS[] array, will realloc later if more
      than BLOCKSIZE words found */

   words = xmalloc (BLOCKSIZE * sizeof (char *));

   num_words = 0;
   block = 1;
   size = BLOCKSIZE;

   phrase_len = strlen (phrase_word);	/* get length of PHRASE_WORD */


   /* Read in words from dictionary, expecting one per line until end of file
      reached.  Check each word to see if it can be made from letters in 
      PHRASE_WORD, if it is at least MIN_WORD_LEN long copy it to 
      WORDS[] array */

   while (fgets (line, MAXLINE, dict)) {
      line[strlen (line) - 1] = 0;

      lowercase (line);

      /* If minimum option is turned on, check if word meets minimum
         length requirements */

      if (min_word_len) {
	 if (no_punc_len (line) < min_word_len)
	    continue;
      }

      /* Check if word has any letter at all and check that word has no
         numbers (as some dictionaries have words like "1st" which are
         not meaningful for anagram purpose, if someone disagrees, I
         may make this an option) */

      have_a_letter = FALSE;
      have_a_digit = FALSE;
      for (temp_string = line; *temp_string; temp_string++) {
	 if (letterpos[(int)*temp_string]!=-1)
	    have_a_letter = TRUE;
	 if (isdigit (*temp_string)) {
	    have_a_digit = TRUE;
	    break;
	 }
      }
      if (!have_a_letter || have_a_digit)
	 continue;




      /* Check if word can be made with letters in PHRASE_WORD */


      word_ok = check_word (line, phrase_word);

      /* If we can make this word and OUTPUT_WORDS option is turned on
         just print the word, otherwise store it in WORDS[] array, 
         then check to see if we need to increase size of WORDS[] array */

      if (word_ok) {

	 if (output_words) {
	    printf ("%s\n", line);
	    fflush (stdout);
	    num_words++;
	 }
	 else {
	    words[num_words] = xmalloc ((strlen (line) + 1) * sizeof (char));
	    strcpy (words[num_words], line);
	    num_words++;	/* increment index into WORDS[] */
	    block++;		/* increment number of elements in this block */


	    /* If number of elements in this block is greater than
	       BLOCK_SIZE then realloc WORDS[] */

	    if (block > BLOCKSIZE) {
	       words = xrealloc (words, (size + BLOCKSIZE) * sizeof (char *));
	       block = 1;	/* Reset block number */
	       size += BLOCKSIZE;	/* Keep track of WORDS[] size */
	    }
	 }
      }
   }

   fclose (dict);		/* Close dictionary file */


   return (num_words);

}

/* Create the bit masks for each word in WORDS array */

void make_word_masks (words, word_mask, num_word_masks, dict_words)
     unsigned char **words;
     bitmask_t **word_mask;
     int *num_word_masks;
     int dict_words;
{
   int loop1, loop2;
   int *freq_table;

   freq_table = xcalloc(alphabet,sizeof(int));

   /* Initialise FREQ_TABLE[], an element array containing the number of 
      times each letter (from least to most frequent) appears in a word */

   for (loop1 = 0; loop1 < alphabet; loop1++)
      freq_table[loop1] = 0;

   /* Loop through all words in the dictionary of words which can be made
      creating a bit mask array for each word.  Each bit mask takes up to
      26 bits, the highest bit being for the letter appearing least
      frequently in the WORDS array, the lowest bit being either the most
      frequenty or usually the alphabetically highest letter with 0
      frequency */

   for (loop1 = 0; loop1 < dict_words; loop1++) {
      /* Setup the FREQ_TABLE[] array and store the maximum number of
         times any letter appears in NUM_WORD_MASKS */

      num_word_masks[loop1] = makefreq_table (freq_table, words[loop1]);

      /* Allocate memory for number of word masks for this word */

      word_mask[loop1] = xcalloc ((num_word_masks[loop1]), sizeof (bitmask_t));

      /* Turn bits on in WORD_MASK by ORing with 2 to the power of the
         letter number */

      for (loop2 = 0; loop2<alphabet; loop2++) {
	 while (freq_table[loop2] > 0) {
	    word_mask[loop1][freq_table[loop2] - 1] |= letter_mask[loop2];
	    freq_table[loop2]--;
	 }
      }
   }

   free(freq_table);
   return;
}




/* Create index into each word in WORDS array based on first letter in
   each word */

void create_let_hash (bitmask_t **word_mask, int dict_words,
		      bitmask_t *letter_mask, int *let_hash)
{
   int loop1;
   int count = 0;
   bitmask_t mask = letter_mask[count];


   let_hash[alphabet+1] = dict_words;
   let_hash[0] = 0;

   for (loop1 = 1; loop1 <= alphabet; loop1++)
      let_hash[loop1] = -1;



   for (loop1 = 0; loop1 < dict_words; loop1++) {
      if (mask <= *word_mask[loop1])
	 continue;
      while (mask > *word_mask[loop1]) {
	 let_hash[++count] = loop1;
	 mask = letter_mask[count];
      }

   }

   for (loop1 = alphabet+1; loop1 >= 0; loop1--) {


      if (let_hash[loop1] == -1)
	 let_hash[loop1] = let_hash[loop1 + 1];

   }



   return;
}



/* Create a frequency table for the number of times the letters 'a'-'z' 
   appear in WORD, return the maximum number of times any letter appears 
   in WORD */

int makefreq_table (freq_table, word)
     int *freq_table;
     unsigned char *word;
{
   int max_freq = 1;		/* Maximum number of times any letter appears in WORD */
   int letfreq;			/* Number of times individual letter appears in FREQ_TABLE */



   for (; *word != '\0'; word++) {
      /* If letter is punctuation mark skip to next letter */
      int l = letterpos[(int)*word];

      if (l == -1)
	 continue;


      /* Add 1 to frequency for this letter in FREQ_TABLE */

      letfreq = ++freq_table[letter_index2[l]];

      /* If this letter appears more times than MAX_FREQ reset MAX_FREQ */

      if (letfreq > max_freq)
	 max_freq = letfreq;

   }

   /* Return the maximum number of times any letter appears in WORD */

   return (max_freq);
}






/* Calculate number of times each letter appears in WORDS array */

void calc_letter_freq (unsigned char **words, int *letter_freq, int dict_words)
{

   int loop1 = 0;
   unsigned char *tmpptr;
   loop1 = dict_words;

   while (loop1--) {
      tmpptr = *words++;
      for (; *tmpptr != '\0'; tmpptr++) {
	 if (letterpos[(int)*tmpptr]==-1)
	    continue;
	 else
	    letter_freq[letterpos[(int)*tmpptr]]++;
      }
   }

   for (loop1 = 0; loop1 < alphabet; loop1++) {
      if (letter_freq[loop1] == 0)
	 letter_freq[loop1] = 5000;
   }
}

/* Compares length of STRING1 and STRING2, if STRING1 length < STRING2
   length return 1.  If STRING1 length > STRING2 length return -1.
   If STRING1 length == STRING2 length return 0. */

int compare_len (string1, string2)
     char *string1[];
     char *string2[];
{

   int s1_len;
   int s2_len;

   s1_len = strlen (*string1);
   s2_len = strlen (*string2);
   if (s1_len < s2_len)
      return (-1);
   if (s1_len > s2_len)
      return (1);
   else
      return (0);
}

/* Compare function used by qsort to sort the words in WORDS array */

int compare_word (word1, word2)
     unsigned char *word1[];
     unsigned char *word2[];
{

#ifndef FORCE_ANSI
   char tmp_word1[strlen (*word1) + 1];
   char tmp_word2[strlen (*word2) + 1];
#else
   char *tmp_word1;
   char *tmp_word2;
   int result;

   tmp_word1 = xmalloc ((strlen (*word1) + 1) * sizeof (char));
   tmp_word2 = xmalloc ((strlen (*word2) + 1) * sizeof (char));
#endif

   strcpy (tmp_word1, *word1);
   strcpy (tmp_word2, *word2);


   gan_qsort (tmp_word1, strlen (tmp_word1), sizeof (char),
	      (gan_qsort_t) compare_char);
   gan_qsort (tmp_word2, strlen (tmp_word2), sizeof (char),
	      (gan_qsort_t) compare_char);

   /* First sort words by frequency of first letter */

   if (letter_freq[letterpos[(int)*tmp_word1]] <
        letter_freq[letterpos[(int)*tmp_word2]]) {

#ifdef FORCE_ANSI
      free (tmp_word1);
      free (tmp_word2);
#endif

      return (-1);
   }
   else {
      if (letter_freq[letterpos[(int)*tmp_word1]] >
           letter_freq[letterpos[(int)*tmp_word2]]) {

#ifdef FORCE_ANSI
	 free (tmp_word1);
	 free (tmp_word2);
#endif

	 return (1);
      }
      else {
	 /* If first letter frequency is equal then sort by alphabetical
	    value of first letter */

	 if (*tmp_word1 < *tmp_word2) {

#ifdef FORCE_ANSI
	    free (tmp_word1);
	    free (tmp_word2);
#endif

	    return -1;
	 }
	 else {
	    if (*tmp_word1 > *tmp_word2) {

#ifdef FORCE_ANSI
	       free (tmp_word1);
	       free (tmp_word2);
#endif

	       return 1;
	    }
	 }
	 /* If letters are the same then sort by word length */
	 if (no_punc_len (tmp_word1) > no_punc_len (tmp_word2)) {

#ifdef FORCE_ANSI
	    free (tmp_word1);
	    free (tmp_word2);
#endif

	    return (-1);
	 }
	 else {
	    if (no_punc_len (tmp_word1) < no_punc_len (tmp_word2)) {

#ifdef FORCE_ANSI
	       free (tmp_word1);
	       free (tmp_word2);
#endif

	       return (1);
	    }
	    else {

	       /* If All the above is equal then sort by frequency of
	          the rest of the letters in each word */
#ifdef FORCE_ANSI
	       result = freq_strcmp (tmp_word1, tmp_word2);
	       free (tmp_word1);
	       free (tmp_word2);
	       return result;
#else
	       return (freq_strcmp (tmp_word1, tmp_word2));
#endif
	    }
	 }
      }
   }
}


/* Used by qsort to sort letters in a word by their frequency in the WORDS
   array */

int compare_char (let1, let2)
     unsigned char *let1;
     unsigned char *let2;
{

   /* Check to make sure punctuation appears at the end of words */

   if (letterpos[(int)*let1]==-1) {
      if (letterpos[(int)*let2]==-1)
	 return 0;
      else
	 return 1;
   }

   if (letterpos[(int)*let2]==-1)
      return -1;


   if (letter_freq[letterpos[(int)*let1]] <
       letter_freq[letterpos[(int)*let2]])
      return (-1);
   else if (letter_freq[letterpos[(int)*let1]] >
            letter_freq[letterpos[(int)*let2]])
      return (1);

   if (*let1 < *let2)
      return (-1);
   else {
      if (*let1 > *let2)
	 return (1);
      else
	 return (0);
   }

}

/* Function used by compare_word which does a strcmp type comparison based
   on the frequency of the letters in the two words (A lower frequency
   letter goes before a higher frequency) */


int freq_strcmp (unsigned char *s1, unsigned char *s2)
{

   int c1;
   int c2;

   do {
      if (letterpos[(int)*s1]==-1) {
	 if (letterpos[(int)*s2]==-1)
	    return 0;
	 else
	    return 1;
      }
      if (letterpos[(int)*s2]==-1)
	 return 1;
      while (*(s1 + 1) == *s1)
	 s1++;
      if (*s1 == '\0')
	 return 1;
      while (*(s2 + 1) == *s2)
	 s2++;
      if (*s2 == '\0')
	 return -1;

      c1 = letter_freq[letterpos[(int)*s1]];
      c2 = letter_freq[letterpos[(int)*s2]];
   }
   while (*s1++ == *s2++);

   if (c1 < c2)
      return -1;
   else {
      if (c1 > c2)
	 return 1;
      else {
	 if (*(s1 - 1) < *(s2 - 1))
	    return -1;
	 if (*(s1 - 1) > *(s2 - 1))
	    return 1;
	 else
	    return 0;
      }
   }

}

/* Create LETTER_INDEX and LETTER_INDEX2 which show the order of the
   alphabetical letters to be used */

void create_letter_index (int *letter_index)
{
   int loop1;

   for (loop1 = 0; loop1 < alphabet; loop1++)
      letter_index[loop1] = loop1;

   gan_qsort (letter_index, alphabet, sizeof (int),
	      (gan_qsort_t) compare_let_index);
   for (loop1 = 0; loop1 < alphabet; loop1++)
      letter_index2[letter_index[loop1]] = loop1;
}


/* Used by qsort to sort letter frequency table into lowest to highest
   frequency */

int compare_let_index (int *let1, int *let2)
{


   if (letter_freq[*let1] < letter_freq[*let2])
      return (-1);
   else {
      if (letter_freq[*let1] > letter_freq[*let2])
	 return (1);
      else {
	 if (*let1 < *let2)
	    return -1;
	 else {
	    if (*let1 > *let2)
	       return 1;
	    else
	       return 0;
	 }
      }
   }
}


/* Create index into WORDS array based on word length and letter */

void create_word_len_hash (word_len_hash, word_len, let_hash, phrase_len)
     int **word_len_hash;
     int *word_len;
     int *let_hash;
     int phrase_len;
{
   int old_count = 0;
   int loop1, loop2;

   for (loop1 = 0; loop1 <= alphabet; loop1++) {
      for (loop2 = 0; loop2 <= phrase_len; loop2++) {
	 word_len_hash[loop2][loop1] = -1;
      }
   }

   for (loop1 = 0; loop1 < alphabet; loop1++) {
      old_count = let_hash[loop1];
      if (old_count < let_hash[alphabet+1])
	 word_len_hash[word_len[old_count]][loop1] = old_count;
      for (loop2 = let_hash[loop1]; loop2 < let_hash[loop1 + 1]; loop2++) {
	 if (word_len[loop2] < word_len[old_count]) {
	    word_len_hash[word_len[loop2]][loop1] = loop2;
	    old_count = loop2;

	 }
      }
   }

   for (loop1 = 0; loop1 < alphabet; loop1++) {
      word_len_hash[0][loop1] = let_hash[alphabet+1];
      for (loop2 = 1; loop2 < phrase_len; loop2++) {
	 if (word_len_hash[loop2][loop1] == -1) {
	    word_len_hash[loop2][loop1] = word_len_hash[loop2 - 1][loop1];
	 }
      }
   }

}

/* Change any uppercase letters in STRING to lowercase */

void lowercase (string)
     char *string;
{
   /* Loop through letters in STRING making each lowercase */

   for (; *string; string++)
      *string = tolower (*string);

   return;
}

/* Remove punctuation from STRING */

void remove_punc (string)
  unsigned char *string;
{
   unsigned char *tmp_string;

   /* Loop through letters in TMP_STRING copying non-punctuation letters 
      back into STRING */

   for (tmp_string = string; *tmp_string != '\0'; tmp_string++) {
      /* If current letter is punctuation mark skip to next letter */

      if (letterpos[(int)*tmp_string]==-1)
	 continue;

      /* Copy letter into NEW_STRING */

      else {
	 *(string++) = *tmp_string;
      }
   }

   /* Make sure string is truncated with '\0' */

   *string = '\0';

   return;
}


/* Get length of STRING without punctuation marks, assumes STRING is
   allready lowercase, returns STRING length */

int no_punc_len (string)
  unsigned char *string;
{
   int length = 0;		/* Length of STRING */

   /* Loop through letters in STRING counting only letters 'a'-'z' */

   for (; *string != '\0'; string++) {
      /* If letter punctuation mark skip to next letter */

      if (letterpos[(int)*string]==-1)
	 continue;

      /* Add 1 to LENGTH */

      else {
	 length++;
      }
   }

   /* Return LENGTH */

   return (length);

}

/* Calls MALLOC() checking if memory can be successfully allocated, returns
   pointer to newly allocated memory if successful, exits with error message
   otherwise */

void *xmalloc (size)
     size_t size;
{
   void *location;

   location = malloc (size);
   if (location == 0) {
      fprintf (stderr, _("Memory exhausted\n"));
      exit (1);
   }
   return (location);
}


/* Calls CALLOC() checking if memory can be successfully allocated, returns
   pointer to newly allocated memory if successful, exits with error message
   otherwise */

void *xcalloc (num_objects, size)
     size_t num_objects;
     size_t size;
{
   void *location;

   location = calloc (num_objects, size);
   if (location == 0) {
      fprintf (stderr, _("Memory exhausted\n"));
      exit (1);
   }
   return (location);
}


/* Calls REALLOC() checking if memory can be successfully allocated, returns
   pointer to newly allocated memory if successful, exits with error message
   otherwise */

void *xrealloc (location, newsize)
     void *location;
     size_t newsize;
{
   location = realloc (location, newsize);
   if (location == 0) {
      fprintf (stderr, _("Memory exhausted\n"));
      exit (1);
   }
   return (location);
}

/* like strdup() but exit if failure
 */

char *xstrdup (s)
     char *s;
{
   return strcpy (xmalloc (strlen (s) + 1), s);
}
