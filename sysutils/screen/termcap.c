/* Copyright (c) 1993
 *      Juergen Weigert (jnweiger@immd4.informatik.uni-erlangen.de)
 *      Michael Schroeder (mlschroe@immd4.informatik.uni-erlangen.de)
 * Copyright (c) 1987 Oliver Laumann
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (see the file COPYING); if not, write to the
 * Free Software Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA
 *
 ****************************************************************
 */

#include "rcs.h"
RCS_ID("$Id: termcap.c,v 1.8 1994/05/31 12:33:13 mlschroe Exp $ FAU")

#include <sys/types.h>
#include "config.h"
#include "screen.h"
#include "extern.h"

extern struct display *display, *displays;

static void  AddCap __P((char *));
static void  MakeString __P((char *, char *, int, char *));
static char *findcap __P((char *, char **, int));
static int   copyarg __P((char **, char *));
static char *e_tgetstr __P((char *, char **));
static int   e_tgetflag __P((char *));
static int   e_tgetnum __P((char *));
#ifdef MAPKEYS
static int   addmapseq __P((char *, int));
static int   remmapseq __P((char *));
#ifdef DEBUG
static void  dumpmap __P((void));
#endif
#endif


extern struct term term[];	/* terminal capabilities */
extern struct NewWindow nwin_undef, nwin_default, nwin_options;
extern int force_vt, assume_LP;
extern int Z0width, Z1width;
#ifdef MAPKEYS
extern struct action umtab[];
extern struct action mmtab[];
extern struct action dmtab[];
extern char *kmap_extras[];
extern int kmap_extras_fl[];
extern int DefaultEsc;
#endif

char Termcap[TERMCAP_BUFSIZE + 8];	/* new termcap +8:"TERMCAP=" */
static int Termcaplen;
static int tcLineLen;
char Term[MAXSTR+5];		/* +5: "TERM=" */
char screenterm[20];		/* new $TERM, usually "screen" */

char *extra_incap, *extra_outcap;

static const char TermcapConst[] = "\\\n\
\t:DO=\\E[%dB:LE=\\E[%dD:RI=\\E[%dC:UP=\\E[%dA:bs:bt=\\E[Z:\\\n\
\t:cd=\\E[J:ce=\\E[K:cl=\\E[H\\E[J:cm=\\E[%i%d;%dH:ct=\\E[3g:\\\n\
\t:do=^J:nd=\\E[C:pt:rc=\\E8:rs=\\Ec:sc=\\E7:st=\\EH:up=\\EM:\\\n\
\t:le=^H:bl=^G:cr=^M:it#8:ho=\\E[H:nw=\\EE:ta=^I:is=\\E)0:";

char *
gettermcapstring(s)
char *s;
{
  int i;

  if (display == 0 || s == 0)
    return 0;
  for (i = 0; i < T_N; i++)
    {
      if (term[i].type != T_STR)
	continue;
      if (strcmp(term[i].tcname, s) == 0)
	return D_tcs[i].str;
    }
  return 0;
}

int
InitTermcap(wi, he)
int wi;
int he;
{
  register char *s;
  int i;
  char tbuf[TERMCAP_BUFSIZE], *tp;
  int t, xue, xse, xme;

  ASSERT(display);
  bzero(tbuf, sizeof(tbuf));
  debug1("InitTermcap: looking for tgetent('%s')\n", D_termname);
  if (*D_termname == 0 || tgetent(tbuf, D_termname) != 1)
    {
#ifdef TERMINFO
      Msg(0, "Cannot find terminfo entry for '%s'.", D_termname);
#else
      Msg(0, "Cannot find termcap entry for '%s'.", D_termname);
#endif
      return -1;
    }
  debug1("got it:\n%s\n",tbuf);
#ifdef DEBUG
  if (extra_incap)
    debug1("Extra incap: %s\n", extra_incap);
  if (extra_outcap)
    debug1("Extra outcap: %s\n", extra_outcap);
#endif

  if ((D_tentry = (char *)malloc(TERMCAP_BUFSIZE + (extra_incap ? strlen(extra_incap) + 1 : 0))) == 0)
    {
      Msg(0, strnomem);
      return -1;
    }

  tp = D_tentry;
  for (i = 0; i < T_N; i++)
    {
      switch(term[i].type)
	{
	case T_FLG:
	  D_tcs[i].flg = e_tgetflag(term[i].tcname);
	  break;
	case T_NUM:
	  D_tcs[i].num = e_tgetnum(term[i].tcname);
	  break;
	case T_STR:
	  D_tcs[i].str = e_tgetstr(term[i].tcname, &tp);
	  /* no empty strings, please */
	  if (D_tcs[i].str && *D_tcs[i].str == 0)
	    D_tcs[i].str = 0;
	  break;
	default:
	  Panic(0, "Illegal tc type in entry #%d", i);
	  /*NOTREACHED*/
	}
    }
  if (D_HC)
    {
      Msg(0, "You can't run screen on a hardcopy terminal.");
      return -1;
    }
  if (D_OS)
    {
      Msg(0, "You can't run screen on a terminal that overstrikes.");
      return -1;
    }
  if (!D_CL)
    {
      Msg(0, "Clear screen capability required.");
      return -1;
    }
  if (!D_CM)
    {
      Msg(0, "Addressable cursor capability required.");
      return -1;
    }
  if ((s = getenv("COLUMNS")) && (i = atoi(s)) > 0)
    D_CO = i;
  if ((s = getenv("LINES")) && (i = atoi(s)) > 0)
    D_LI = i;
  if (wi)
    D_CO = wi;
  if (he)
    D_LI = he;
  if (D_CO <= 0)
    D_CO = 80;
  if (D_LI <= 0)
    D_LI = 24;

  if (nwin_options.flowflag == nwin_undef.flowflag)
    nwin_default.flowflag = D_CNF ? FLOW_NOW * 0 : 
			    D_NX ? FLOW_NOW * 1 :
			    FLOW_AUTOFLAG;
  D_CLP |= (assume_LP || !D_AM || D_XV || D_XN ||
	 (!extra_incap && !strncmp(D_termname, "vt", 2)));
  if (!D_BL)
    D_BL = "\007";
  if (!D_BC)
    {
      if (D_BS)
	D_BC = "\b";
      else
	D_BC = D_LE;
    }
  if (!D_CR)
    D_CR = "\r";
  if (!D_NL)
    D_NL = "\n";

  /*
   *  Set up attribute handling.
   *  This is rather complicated because termcap has different
   *  attribute groups.
   */

  if (D_UG > 0)
    D_US = D_UE = 0;
  if (D_SG > 0)
    D_SO = D_SE = 0;
  /* Unfortunatelly there is no 'mg' capability.
   * For now we think that mg > 0 if sg and ug > 0.
   */
  if (D_UG > 0 && D_SG > 0)
    D_MH = D_MD = D_MR = D_MB = D_ME = 0;

  xue = ATYP_U;
  xse = ATYP_S;
  xme = ATYP_M;

  if (D_SO && D_SE == 0)
    {
      Msg(0, "Warning: 'so' but no 'se' capability.");
      if (D_ME)
	xse = xme;
      else
	D_SO = 0;
    }
  if (D_US && D_UE == 0)
    {
      Msg(0, "Warning: 'us' but no 'ue' capability.");
      if (D_ME)
	xue = xme;
      else
	D_US = 0;
    }
  if ((D_MH || D_MD || D_MR || D_MB) && D_ME == 0)
    {
      Msg(0, "Warning: 'm?' but no 'me' capability.");
      D_MH = D_MD = D_MR = D_MB = 0;
    }
  /*
   * Does ME also reverse the effect of SO and/or US?  This is not
   * clearly specified by the termcap manual. Anyway, we should at
   * least look whether ME and SE/UE are equal:
   */
  if (D_UE && D_SE && strcmp(D_SE, D_UE) == 0)
    xse = xue;
  if (D_SE && D_ME && strcmp(D_ME, D_SE) == 0)
    xse = xme;
  if (D_UE && D_ME && strcmp(D_ME, D_UE) == 0)
    xue = xme;

  for (i = 0; i < NATTR; i++)
    {
      D_attrtab[i] = D_tcs[T_ATTR + i].str;
      D_attrtyp[i] = i == ATTR_SO ? xse : (i == ATTR_US ? xue : xme);
    }
  
  /* Set up missing entries (attributes are priority ordered) */
  s = 0;
  t = 0;
  for (i = 0; i < NATTR; i++)
    if ((s = D_attrtab[i]))
      {
	t = D_attrtyp[i];
	break;
      }
  for (i = 0; i < NATTR; i++)
    {
      if (D_attrtab[i] == 0)
	{
	  D_attrtab[i] = s;
	  D_attrtyp[i] = t;
	}
      else
        {
	  s = D_attrtab[i];
	  t = D_attrtyp[i];
        }
    }
  if (D_CAF == 0 && D_CAB == 0)
    {
      /* hmm, where's the difference? */
      D_CAF = D_CSF;
      D_CAB = D_CSB;
    }

  if (!D_DO)
    D_DO = D_NL;
  if (!D_SF)
    D_SF = D_NL;
  if (D_IN)
    D_IC = D_IM = 0;
  if (D_EI == 0)
    D_IM = 0;
  /* some strange termcap entries have IC == IM */
  if (D_IC && D_IM && strcmp(D_IC, D_IM) == 0)
    D_IC = 0;
  if (D_KE == 0)
    D_KS = 0;
  if (D_CVN == 0)
    D_CVR = 0;
  if (D_VE == 0)
    D_VI = D_VS = 0;
  if (D_CCE == 0)
    D_CCS = 0;
  if (D_CG0)
    {
      if (D_CS0 == 0)
#ifdef TERMINFO
        D_CS0 = "\033(%p1%c";
#else
        D_CS0 = "\033(%.";
#endif
      if (D_CE0 == 0)
        D_CE0 = "\033(B";
      D_AC = 0;
    }
  else if (D_AC || (D_AS && D_AE))	/* some kind of graphics */
    {
      D_CS0 = (D_AS && D_AE) ? D_AS : "";
      D_CE0 = (D_AS && D_AE) ? D_AE : "";
      D_CC0 = D_AC;
    }
  else
    {
      D_CS0 = D_CE0 = "";
      D_CC0 = 0;
      D_AC = "";	/* enable default string */
    }

  for (i = 0; i < 256; i++)
    D_c0_tab[i] = i;
  if (D_AC)
    {
      /* init with default string first */
      s = "l+m+k+j+u+t+v+w+q-x|n+o~s_p\"r#`+a:f'g#~o.v-^+<,>h#I#0#y<z>";
      for (i = strlen(s) & ~1; i >= 0; i -= 2)
	D_c0_tab[(int)(unsigned char)s[i]] = s[i + 1];
    }
  if (D_CC0)
    for (i = strlen(D_CC0) & ~1; i >= 0; i -= 2)
      D_c0_tab[(int)(unsigned char)D_CC0[i]] = D_CC0[i + 1];
  debug1("ISO2022 = %d\n", D_CG0);
  if (D_PF == 0)
    D_PO = 0;
  debug2("terminal size is %d, %d (says TERMCAP)\n", D_CO, D_LI);

  if (D_CXC)
    if (CreateTransTable(D_CXC))
      return -1;

  /* Termcap fields Z0 & Z1 contain width-changing sequences. */
  if (D_CZ1 == 0)
    D_CZ0 = 0;
  Z0width = 132;
  Z1width = 80;

  CheckScreenSize(0);

  if (D_TS == 0 || D_FS == 0 || D_DS == 0)
    D_HS = 0;
  if (D_HS)
    {
      debug("oy! we have a hardware status line, says termcap\n");
      if (D_WS <= 0)
        D_WS = D_width;
    }
#ifdef KANJI
  D_kanji = 0;
  if (D_CKJ)
    {
      if (strcmp(D_CKJ, "euc") == 0)
	D_kanji = EUC;
      else if (strcmp(D_CKJ, "sjis") == 0)
	D_kanji = SJIS;
    }
#endif

  D_UPcost = CalcCost(D_UP);
  D_DOcost = CalcCost(D_DO);
  D_NLcost = CalcCost(D_NL);
  D_LEcost = CalcCost(D_BC);
  D_NDcost = CalcCost(D_ND);
  D_CRcost = CalcCost(D_CR);
  D_IMcost = CalcCost(D_IM);
  D_EIcost = CalcCost(D_EI);

#ifdef AUTO_NUKE
  if (D_CAN)
    {
      debug("termcap has AN, setting autonuke\n");
      D_auto_nuke = 1;
    }
#endif
  if (D_COL > 0)
    {
      debug1("termcap has OL (%d), setting limit\n", D_COL);
      D_obufmax = D_COL;
    }
#ifdef MAPKEYS
  D_nseqs = 0;
  for (i = 0; i < T_OCAPS - T_CAPS; i++)
    remap(i, 1);
  for (i = 0; i < KMAP_EXT; i++)
    remap(i + (KMAP_KEYS+KMAP_AKEYS), 1);
  D_seqp = D_kmaps[0].seq;
#endif

  D_tcinited = 1;
  MakeTermcap(0);
#ifdef MAPKEYS
  CheckEscape();
#endif
  return 0;
}

#ifdef MAPKEYS

int
remap(n, map)
int n;
int map;
{
  char *s;
  int fl = 0, domap = 0;
  struct action *a1, *a2, *tab;

  tab = umtab;
  for (;;)
    {
      a1 = &tab[n];
      a2 = 0;
      if (n < KMAP_KEYS+KMAP_AKEYS)
	{
	  if (n >= KMAP_KEYS)
	    n -= T_OCAPS-T_CURSOR;
	  s = D_tcs[n + T_CAPS].str;
	  if (n >= T_CURSOR-T_CAPS)
	    a2 = &tab[n + (T_OCAPS-T_CURSOR)];
	}
      else
	{
	  s = kmap_extras[n - (KMAP_KEYS+KMAP_AKEYS)];
	  fl |= kmap_extras_fl[n - (KMAP_KEYS+KMAP_AKEYS)];
	}
      if (s == 0)
	return 0;
      if (a1 && a1->nr == RC_ILLEGAL)
	a1 = 0;
      if (a2 && a2->nr == RC_ILLEGAL)
	a2 = 0;
      if (a1 && a1->nr == RC_STUFF && strcmp(a1->args[0], s) == 0)
	a1 = 0;
      if (a2 && a2->nr == RC_STUFF && strcmp(a2->args[0], s) == 0)
	a2 = 0;
      domap |= (a1 || a2);
      if (tab == umtab)
	tab = dmtab;
      else if (tab == dmtab)
	tab = mmtab;
      else
	break;
    }

  if (map == 0 && domap)
    return 0;
  if (map && !domap)
    return 0;
  debug3("%smapping %s %#x\n", map? "" :"un",s,n);
  if (map)
    return addmapseq(s, n | fl);
  else
    return remmapseq(s);
}

void
CheckEscape()
{
  struct display *odisplay;
  int i, nr;

  if (DefaultEsc >= 0)
    return;

  odisplay = display;
  for (display = displays; display; display = display->d_next)
    {
      for (i = 0; i < D_nseqs; i++)
	{
	  nr = D_kmaps[i].nr & ~KMAP_NOTIMEOUT;
	  if (umtab[nr].nr == RC_COMMAND)
	    break;
	  if (umtab[nr].nr == RC_ILLEGAL && dmtab[nr].nr == RC_COMMAND)
	    break;
	}
      if (i >= D_nseqs)
        break;
    }
  if (display == 0)
    {
      display = odisplay;
      return;
    }
  ParseEscape((struct user *)0, "^aa");
  if (odisplay->d_user->u_Esc <= 0)
    odisplay->d_user->u_Esc = DefaultEsc;
  display = 0;
  Msg(0, "Warning: escape char set back to ^A");
  display = odisplay;
}

static int
addmapseq(seq, nr)
char *seq;
int nr;
{
  int i, j = 0, k, mo, m;
  char *p, *o;

  k = strlen(seq);
  if (k > sizeof(D_kmaps[0].seq) - 1)
    return -1;
  for (i = 0; i < D_nseqs; i++)
    if ((j = strcmp(D_kmaps[i].seq, seq)) >= 0)
      break;
  if (i < D_nseqs && j == 0)
    {
      D_kmaps[i].nr = nr;
      return 0;
    }
  if (D_nseqs >= sizeof(D_kmaps)/sizeof(*D_kmaps)) /* just in case... */
    return -1;
  for (j = D_nseqs - 1; j >= i; j--)
    D_kmaps[j + 1] = D_kmaps[j];
  p = D_kmaps[i].seq;
  o = D_kmaps[i].off;
  strcpy(p, seq);
  bzero(o, k + 1);
  D_kmaps[i].nr = nr;
  D_nseqs++;

  if (i + 1 < D_nseqs)
    for (j = 0; *p; p++, o++, j++)
      {
        if (D_kmaps[i + 1].seq[j] != *p)
          {
            if (D_kmaps[i + 1].seq[j])
	      *o = 1;
	    break;
	  }
        *o = D_kmaps[i + 1].off[j] ? D_kmaps[i + 1].off[j] + 1 : 0;
      }

  for (k = 0; k < i; k++)
    for (m = j = 0, p = D_kmaps[k].seq, o = D_kmaps[k].off; *p; p++, o++, j++)
      {
	mo = m;
	if (!m && *p != D_kmaps[i].seq[j])
	  m = 1;
	if (*o == 0 && mo == 0 && m)
	  *o = i - k;
	if (*o < i - k || (*o == i - k && m))
	  continue;
	(*o)++;
      }
#ifdef DEBUG
  dumpmap();
#endif
  return 0;
}

static int
remmapseq(seq)
char *seq;
{
  int i, j = 0, k;
  char *p, *o;

  for (i = 0; i < D_nseqs; i++)
    if ((j = strcmp(D_kmaps[i].seq, seq)) >= 0)
      break;
  if (i == D_nseqs || j)
    return -1;
  for (k = 0; k < i; k++)
    for (j = 0, p = D_kmaps[k].seq, o = D_kmaps[k].off; *p; p++, o++, j++)
      {
	if (k + *o == i)
          *o = D_kmaps[i].off[j] ? D_kmaps[i].off[j] + *o - 1 : 0;
	else if (k + *o > i)
          (*o)--;
      }
  for (j = i + 1; j < D_nseqs; j++)
    D_kmaps[j - 1] = D_kmaps[j];
  D_nseqs--;
#ifdef DEBUG
  dumpmap();
#endif
  return 0;
}

#ifdef DEBUG
static void
dumpmap()
{
  char *p, *o;
  int i,j,n;
  debug("Mappings:\n");
  for (i = 0; i < D_nseqs; i++)
    {
      for (j = 0, p = D_kmaps[i].seq, o = D_kmaps[i].off; *p; p++, o++, j++)
	{
	  if (*p > ' ' && (unsigned char)*p < 0177)
	    {
              debug2("%c[%d] ", *p, *o);
	    }
          else
            debug2("\\%03o[%d] ", (unsigned char)*p, *o);
	}
      n = D_kmaps[i].nr;
      debug2(" ==> %d%s\n", n & ~KMAP_NOTIMEOUT, (n & KMAP_NOTIMEOUT) ? " (no timeout)" : "");
    }
}
#endif

#endif

static void
AddCap(s)
char *s;
{
  register int n;

  if (tcLineLen + (n = strlen(s)) > 55 && Termcaplen < TERMCAP_BUFSIZE + 8 - 4)
    {
      strcpy(Termcap + Termcaplen, "\\\n\t:");
      Termcaplen += 4;
      tcLineLen = 0;
    }
  if (Termcaplen + n < TERMCAP_BUFSIZE + 8)
    {
      strcpy(Termcap + Termcaplen, s);
      Termcaplen += n;
      tcLineLen += n;
    }
  else
    Panic(0, "TERMCAP overflow - sorry.");
}

char *
MakeTermcap(aflag)
int aflag;
{
  char buf[TERMCAP_BUFSIZE];
  register char *p, *cp, *s, ch, *tname;
  int i, wi, he;

  if (display)
    {
      wi = D_width;
      he = D_height;
      tname = D_termname;
    }
  else
    {
      wi = 80;
      he = 24;
      tname = "vt100";
    }
  debug1("MakeTermcap(%d)\n", aflag);
  if ((s = getenv("SCREENCAP")) && strlen(s) < TERMCAP_BUFSIZE)
    {
      sprintf(Termcap, "TERMCAP=%s", s);
      sprintf(Term, "TERM=screen");
      debug("getenvSCREENCAP o.k.\n");
      return Termcap;
    }
  Termcaplen = 0;
  debug1("MakeTermcap screenterm='%s'\n", screenterm);
  debug1("MakeTermcap termname='%s'\n", tname);
  if (*screenterm == '\0')
    {
      debug("MakeTermcap sets screenterm=screen\n");
      strcpy(screenterm, "screen");
    }
  do
    {
      sprintf(Term, "TERM=");
      p = Term + 5;
      if (!aflag && strlen(screenterm) + strlen(tname) < MAXSTR-1)
	{
	  sprintf(p, "%s.%s", screenterm, tname);
	  if (tgetent(buf, p) == 1)
	    break;
	}
      if (wi >= 132)
	{
	  sprintf(p, "%s-w", screenterm);
          if (tgetent(buf, p) == 1)
	    break;
	}
      sprintf(p, "%s", screenterm);
      if (tgetent(buf, p) == 1)
	break;
      sprintf(p, "vt100");
    }
  while (0);		/* Goto free programming... */

  /* check for compatibility problems, displays == 0 after fork */
  {
    char *tgetstr(), xbuf[TERMCAP_BUFSIZE], *xbp = xbuf;
    if (tgetstr("im", &xbp) && tgetstr("ic", &xbp) && displays)
      {
#ifdef TERMINFO
	Msg(0, "Warning: smir and ich1 set in %s terminfo entry", p);
#else
	Msg(0, "Warning: im and ic set in %s termcap entry", p);
#endif
      }
  }
  
  tcLineLen = 100;	/* Force NL */
  sprintf(Termcap,
	  "TERMCAP=SC|%s|VT 100/ANSI X3.64 virtual terminal", Term + 5);
  Termcaplen = strlen(Termcap);
  debug1("MakeTermcap decided '%s'\n", p);
  if (extra_outcap && *extra_outcap)
    {
      for (cp = extra_outcap; (p = index(cp, ':')); cp = p)
	{
	  ch = *++p;
	  *p = '\0';
	  AddCap(cp);
	  *p = ch;
	}
      tcLineLen = 100;	/* Force NL */
    }
  debug1("MakeTermcap after outcap '%s'\n", (char *)TermcapConst);
  if (Termcaplen + strlen(TermcapConst) < TERMCAP_BUFSIZE)
    {
      strcpy(Termcap + Termcaplen, (char *)TermcapConst);
      Termcaplen += strlen(TermcapConst);
    }
  sprintf(buf, "li#%d:co#%d:", he, wi);
  AddCap(buf);
  AddCap("am:");
  if (aflag || (force_vt && !D_COP) || D_CLP || !D_AM)
    {
      AddCap("xn:");
      AddCap("xv:");
      AddCap("LP:");
    }
  if (aflag || (D_CS && D_SR) || D_AL || D_CAL)
    {
      AddCap("sr=\\EM:");
      AddCap("al=\\E[L:");
      AddCap("AL=\\E[%dL:");
    }
  else if (D_SR)
    AddCap("sr=\\EM:");
  if (aflag || D_CS)
    AddCap("cs=\\E[%i%d;%dr:");
  if (aflag || D_CS || D_DL || D_CDL)
    {
      AddCap("dl=\\E[M:");
      AddCap("DL=\\E[%dM:");
    }
  if (aflag || D_DC || D_CDC)
    {
      AddCap("dc=\\E[P:");
      AddCap("DC=\\E[%dP:");
    }
  if (aflag || D_CIC || D_IC || D_IM)
    {
      AddCap("im=\\E[4h:");
      AddCap("ei=\\E[4l:");
      AddCap("mi:");
      AddCap("IC=\\E[%d@:");
    }
#ifdef MAPKEYS
  AddCap("ks=\\E[?1h\\E=:");
  AddCap("ke=\\E[?1l\\E>:");
#endif
  AddCap("vi=\\E[?25l:");
  AddCap("ve=\\E[34h\\E[?25h:");
  AddCap("vs=\\E[34l:");
  if (display)
    {
      if (D_US)
	{
	  AddCap("us=\\E[4m:");
	  AddCap("ue=\\E[24m:");
	}
      if (D_SO)
	{
	  AddCap("so=\\E[3m:");
	  AddCap("se=\\E[23m:");
	}
      if (D_MB)
	AddCap("mb=\\E[5m:");
      if (D_MD)
	AddCap("md=\\E[1m:");
      if (D_MH)
	AddCap("mh=\\E[2m:");
      if (D_MR)
	AddCap("mr=\\E[7m:");
      if (D_MB || D_MD || D_MH || D_MR)
	AddCap("me=\\E[m:ms:");
      if (D_CAF || D_CAB)
	AddCap("Co#8:pa#64:AF=\\E[3%dm:AB=\\E[4%dm:op=\\E[39;49m:AX:");
      if (D_VB)
	AddCap("vb=\\Eg:");
#ifndef MAPKEYS
      if (D_KS)
	{
	  AddCap("ks=\\E=:");
	  AddCap("ke=\\E>:");
	}
      if (D_CCS)
	{
	  AddCap("CS=\\E[?1h:");
	  AddCap("CE=\\E[?1l:");
	}
#endif
      if (D_CG0)
        AddCap("G0:");
      if (D_CC0 || (D_CS0 && *D_CS0))
	{
	  AddCap("as=\\E(0:");
	  AddCap("ae=\\E(B:");
	  /* avoid `` because some shells dump core... */
	  AddCap("ac=\\140\\140aaffggjjkkllmmnnooppqqrrssttuuvvwwxxyyzz{{||}}~~..--++,,hhII00:");
	}
      if (D_PO)
	{
	  AddCap("po=\\E[5i:");
	  AddCap("pf=\\E[4i:");
	}
      if (D_CZ0)
	{
	  AddCap("Z0=\\E[?3h:");
	  AddCap("Z1=\\E[?3l:");
	}
      if (D_CWS)
	AddCap("WS=\\E[8;%d;%dt:");
    }
  for (i = T_CAPS; i < T_ECAPS; i++)
    {
#ifdef MAPKEYS
      struct action *act;
      if (i < T_OCAPS)
	{
	  if (i >= T_KEYPAD)	/* don't put keypad codes in TERMCAP */
	    continue;		/* - makes it too big */
	  if (i >= T_CURSOR && i < T_OCAPS)
	    {
	      act = &umtab[i - (T_CURSOR - T_OCAPS + T_CAPS)];
	      if (act->nr == RC_ILLEGAL)
		act = &dmtab[i - (T_CURSOR - T_OCAPS + T_CAPS)];
	    }
	  else
	    {
	      act = &umtab[i - T_CAPS];
	      if (act->nr == RC_ILLEGAL)
		act = &dmtab[i - T_CAPS];
	    }
	  if (act->nr != RC_ILLEGAL)
	    {
	      if (act->nr == RC_STUFF)
		{
		  MakeString(term[i].tcname, buf, sizeof(buf), act->args[0]);
		  AddCap(buf);
		}
	      continue;
	    }
	}
#endif
      if (display == 0)
	continue;
      switch(term[i].type)
	{
	case T_STR:
	  if (D_tcs[i].str == 0)
	    break;
	  MakeString(term[i].tcname, buf, sizeof(buf), D_tcs[i].str);
	  AddCap(buf);
	  break;
	case T_FLG:
	  if (D_tcs[i].flg == 0)
	    break;
	  sprintf(buf, "%s:", term[i].tcname);
	  AddCap(buf);
	  break;
	default:
	  break;
	}
    }
  debug("MakeTermcap: end\n");
  return Termcap;
}

static void
MakeString(cap, buf, buflen, s)
char *cap, *buf;
int buflen;
char *s;
{
  register char *p, *pmax;
  register unsigned int c;

  p = buf;
  pmax = p + buflen - (3+4+2);
  *p++ = *cap++;
  *p++ = *cap;
  *p++ = '=';
  while ((c = *s++) && (p < pmax))
    {
      switch (c)
	{
	case '\033':
	  *p++ = '\\';
	  *p++ = 'E';
	  break;
	case ':':
	  sprintf(p, "\\072");
	  p += 4;
	  break;
	case '^':
	case '\\':
	  *p++ = '\\';
	  *p++ = c;
	  break;
	default:
	  if (c >= 200)
	    {
	      sprintf(p, "\\%03o", c & 0377);
	      p += 4;
	    }
	  else if (c < ' ')
	    {
	      *p++ = '^';
	      *p++ = c + '@';
	    }
	  else
	    *p++ = c;
	}
    }
  *p++ = ':';
  *p = '\0';
}


#undef QUOTES
#define QUOTES(p) \
  (*p == '\\' && (p[1] == '\\' || p[1] == ',' || p[1] == '%'))

int
CreateTransTable(s)
char *s;
{
  int curchar;
  char *templ, *arg;
  int templlen;
  int templnsub;
  char *p, *sx;
  char **ctable;
  int l, c;

  if ((D_xtable = (char ***)malloc(256 * sizeof(char **))) == 0)
    {
      Msg(0, strnomem);
      return -1;
    }
  bzero((char *)D_xtable, 256 * sizeof(char **));

  while (*s)
    {
      if (QUOTES(s))
	s++;
      curchar = (unsigned char)*s++;
      if (curchar == 'B')
	curchar = 0;	/* ASCII */
      templ = s;
      templlen = 0;
      templnsub = 0;
      if (D_xtable[curchar] == 0)
        {
          if ((D_xtable[curchar] = (char **)malloc(257 * sizeof(char *))) == 0)
	    {
	      Msg(0, strnomem);
	      FreeTransTable();
	      return -1;
	    }
	  bzero((char *)D_xtable[curchar], 257 * sizeof(char *));
        }
      ctable = D_xtable[curchar];
      for(; *s && *s != ','; s++)
	{
	  if (QUOTES(s))
	      s++;
	  else if (*s == '%')
	    {
	      templnsub++;
	      continue;
	    }
	  templlen++;
	}
      if (*s++ == 0)
	break;
      while (*s && *s != ',')
	{    
	  c = (unsigned char)*s++;
	  if (QUOTES((s - 1)))
	    c = (unsigned char)*s++;
	  else if (c == '%')
	    c = 256;
	  if (ctable[c])
	    free(ctable[c]);
	  arg = s;
	  l = copyarg(&s, (char *)0);
	  if (c != 256)
	    l = l * templnsub + templlen;
	  if ((ctable[c] = (char *)malloc(l + 1)) == 0)
	    {
	      Msg(0, strnomem);
	      FreeTransTable();
	      return -1;
	    }
	  sx = ctable[c];
	  for (p = ((c == 256) ? "%" : templ); *p && *p != ','; p++)
	    {
	      if (QUOTES(p))
		p++;
	      else if (*p == '%')
		{
		  s = arg;
		  sx += copyarg(&s, sx);
		  continue;
		}
	      *sx++ = *p;
	    }
	  *sx = 0;
	  ASSERT(ctable[c] + l == sx);
	  debug3("XC: %c %c->%s\n", curchar, c, ctable[c]);
	}
      if (*s == ',')
	s++;
    }
  return 0;
}

void
FreeTransTable()
{
  char ***p, **q;
  int i, j;

  if ((p = D_xtable) == 0)
    return;
  for (i = 0; i < 256; i++, p++)
    {
      if (*p == 0)
	continue;
      q = *p;
      for (j = 0; j < 257; j++, q++)
	if (*q)
	  free(*q);
      free((char *)*p);
    }
  free((char *)D_xtable);
}

static int
copyarg(pp, s)
char **pp, *s;
{
  int l;
  char *p;

  for (l = 0, p = *pp; *p && *p != ','; p++)
    {
      if (QUOTES(p))
	p++;
      if (s)
        *s++ = *p;
      l++;
    }
  if (*p == ',')
    p++;
  *pp = p;
  return l;
}


/*
**
**  Termcap routines that use our extra_incap
**
*/


/* findcap:
 *   cap = capability we are looking for
 *   tepp = pointer to bufferpointer
 *   n = size of buffer (0 = infinity)
 */

static char *
findcap(cap, tepp, n)
char *cap;
char **tepp;
int n;
{
  char *tep;
  char c, *p, *cp;
  int mode;	/* mode: 0=LIT  1=^  2=\x  3,4,5=\nnn */
  int num = 0, capl;

  if (!extra_incap)
    return 0;
  tep = *tepp;
  capl = strlen(cap);
  cp = 0;
  mode = 0;
  for (p = extra_incap; *p; )
    {
      if (strncmp(p, cap, capl) == 0)
	{
	  p += capl;
	  c = *p;
	  if (c && c != ':' && c != '@')
	    p++;
	  if (c == 0 || c == '@' || c == '=' || c == ':' || c == '#')
	    cp = tep;
	}
      while ((c = *p))
	{
	  p++;
	  if (mode == 0)
	    {
	      if (c == ':')
	        break;
	      if (c == '^')
		mode = 1;
	      if (c == '\\')
		mode = 2;
	    }
	  else if (mode == 1)
	    {
	      mode = 0;
	      c = c & 0x1f;
	    }
	  else if (mode == 2)
	    {
	      mode = 0;
	      switch(c)
		{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		  mode = 3;
		  num = 0;
		  break;
		case 'E':
		  c = 27;
		  break;
		case 'n':
		  c = '\n';
		  break;
		case 'r':
		  c = '\r';
		  break;
		case 't':
		  c = '\t';
		  break;
		case 'b':
		  c = '\b';
		  break;
		case 'f':
		  c = '\f';
		  break;
		}
	    }
	  if (mode > 2)
	    {
	      num = num * 8 + (c - '0');
	      if (mode++ == 5 || (*p < '0' || *p > '9'))
		{
		  c = num;
		  mode = 0;
		}
	    }
	  if (mode)
	    continue;

	  if (cp && n != 1)
	    {
	      *cp++ = c;
	      n--;
	    }
	}
      if (cp)
	{
	  *cp++ = 0;
	  *tepp = cp;
	  debug2("'%s' found in extra_incap -> %s\n", cap, tep);
	  return tep;
	}
    }
  return 0;
}

static char *
e_tgetstr(cap, tepp)
char *cap;
char **tepp;
{
  char *tep, *tgetstr();
  if ((tep = findcap(cap, tepp, 0)))
    return (*tep == '@') ? 0 : tep;
  return tgetstr(cap, tepp);
}

static int
e_tgetflag(cap)
char *cap;
{
  char buf[2], *bufp;
  char *tep;
  bufp = buf;
  if ((tep = findcap(cap, &bufp, 2)))
    return (*tep == '@') ? 0 : 1;
  return tgetflag(cap);
}

static int
e_tgetnum(cap)
char *cap;
{
  char buf[20], *bufp;
  char *tep, c;
  int res, base = 10;

  bufp = buf;
  if ((tep = findcap(cap, &bufp, 20)))
    {
      c = *tep;
      if (c == '@')
	return -1;
      if (c == '0')
	base = 8;
      res = 0;
      while ((c = *tep++) >= '0' && c <= '9')
	res = res * base + (c - '0');
      return res;
    }
  return tgetnum(cap);
}

