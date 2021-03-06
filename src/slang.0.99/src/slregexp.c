/* ed style regular expressions */
/* Copyright (c) 1992, 1995 John E. Davis
 * All rights reserved.
 * 
 * You may distribute under the terms of either the GNU General Public
 * License or the Perl Artistic License.
 */


#include "config.h"

#include <stdio.h>
#include <string.h>


#include "slang.h"

#define SET_BIT(b, n) b[(unsigned int) (n) >> 3] |= 1 << ((unsigned int) (n) % 8)
#define TEST_BIT(b, n) (b[(unsigned int)(n) >> 3] & (1 << ((unsigned int) (n) % 8)))
#define LITERAL 1
#define RANGE 2			       /* [...] */
#define ANY 3			       /* . */
#define BOL 4			       /* ^ */
#define EOL 5			       /* $ */
#define NTH_MATCH 6		       /* \1 \2 ... \9 */
#define OPAREN 7		       /* \( */
#define CPAREN 0x8		       /* \) */
#define ANY_DIGIT 0x9		       /* \d */
#define BOW	0xA		       /* \< */
#define EOW	0xB		       /* \> */
#if 0
#define NOT_LITERAL		0xC	       /* \~ */
#endif
#define STAR 0x80		       /* * */
#define LEAST_ONCE 0x40		       /* + */
#define MAYBE_ONCE 0x20		       /* ? */
#define MANY 0x10		       /* {n,m} */
/* The rest are additions */
#define YES_CASE (STAR | BOL)
#define NO_CASE  (STAR | EOL)

#define UPPERCASE(x)  (cs ? (x) : UPPER_CASE(x))
#define LOWERCASE(x)  (cs ? (x) : LOWER_CASE(x))

static unsigned char Word_Chars[256];
#define IS_WORD_CHAR(x) Word_Chars[(unsigned int) (x)]

static int Next_Pos;
static SLRegexp_Type *This_Reg;
static unsigned char *This_Str;

static unsigned char *do_nth_match(int n, unsigned char *str, unsigned char *estr)
{
   unsigned char *bpos;
   if (n > Next_Pos) return (NULL);
   
   bpos = This_Reg->beg_matches[n] + This_Str;
   n = This_Reg->end_matches[n];
   if (n == 0) return(str);
   if (n > (int) (estr - str)) return (NULL);
   
   /* This needs fixed for case sensitive match */
   if (0 != strncmp((char *) str, (char *) bpos, (unsigned int) n)) return (NULL);
   str += n;
   return (str);
}


/* returns pointer to the end of regexp or NULL */
static unsigned char *regexp_looking_at(register unsigned char *str, unsigned char *estr, unsigned char *buf, register int cs)
{
   register unsigned char p, p1;
   unsigned char *save_str, *tmpstr;
   int n, n0, n1;
   int save_next = 0;
   p = *buf++;
   
   
   while (p != 0)
     {
	/* p1 = UPPERCASE(*buf); */
	/* if (str < estr) c = UPPERCASE(*str); */

	switch((unsigned char) p)
	  {
	   case BOW:
	     if ((str != This_Str)
		 && ((str >= estr)
		     || IS_WORD_CHAR(*(str - 1))
		     || (0 == IS_WORD_CHAR(*str)))) return NULL;
	     break;
	     
	   case EOW:
	     if ((str < estr)
		 && IS_WORD_CHAR (*str)) return NULL;
	     break;
	     
	   case YES_CASE: cs = 1; break;
	   case NO_CASE: cs = 0; break;

	   case OPAREN:
	     This_Reg->beg_matches[Next_Pos] = (int) (str - This_Str);
	     This_Reg->beg_matches[Next_Pos + 1] = -1;
	     break;
	   case CPAREN: 
	     This_Reg->end_matches[Next_Pos] = (unsigned int) (str - This_Str) - This_Reg->beg_matches[Next_Pos];
	     Next_Pos++;
	     break;
#ifdef NOT_LITERAL
	   case NOT_LITERAL:
	     if ((str >= estr) || (*buf == UPPERCASE(*str))) return (NULL);
	     str++; buf++;
	     break;
	     
	   case MAYBE_ONCE | NOT_LITERAL:
	     save_str = str;
	     if ((str < estr) && (*buf != UPPERCASE(*str))) str++;
	     buf++;
	     goto match_rest;

	   case NOT_LITERAL | LEAST_ONCE:   /* match at least once */
	     if ((str >= estr) || (UPPERCASE(*str) == UPPERCASE(*buf))) return (NULL);
	     str++;
	     /* drop */
	   case STAR | NOT_LITERAL:
	     save_str = str;  p1 = *buf;
	     while ((str < estr) && (UPPERCASE(*str) != p1)) str++;
	     buf++;
	     goto match_rest;
	     
	     /* this type consists of the expression + two bytes that
	        determine number of matches to perform */
	   case MANY | NOT_LITERAL:
	     p1 = *buf; buf++;
	     n = n0 = (int) (unsigned char) *buf++;
	     /* minimum number to match--- could be 0 */
	     n1 = (int) (unsigned char) *buf++;
	     /* maximum number to match */

	     while (n && (str < estr) && (p1 != *str)) 
	       {
		  n--;
		  str++;
	       }
	     if (n) return (NULL);
	     
	     save_str = str;
	     n = n1 - n0;
	     while (n && (str < estr) && (p1 != *str)) 
	       {
		  n--;
		  str++;
	       }
	     goto match_rest;
#endif				       /* NOT_LITERAL */
	   case LITERAL: 
	     if ((str >= estr) || (*buf != UPPERCASE(*str))) return (NULL);
	     str++; buf++;
	     break;
	     
	   case MAYBE_ONCE | LITERAL:
	     save_str = str;
	     if ((str < estr) && (*buf == UPPERCASE(*str))) str++;
	     buf++;
	     goto match_rest;

	   case LITERAL | LEAST_ONCE:   /* match at least once */
	     if ((str >= estr) || (UPPERCASE(*str) != UPPERCASE(*buf))) return (NULL);
	     str++;
	     /* drop */
	   case STAR | LITERAL:
	     save_str = str;  p1 = *buf;
	     while ((str < estr) && (UPPERCASE(*str) == p1)) str++;
	     buf++;
	     goto match_rest;
	     
	     /* this type consists of the expression + two bytes that
	        determine number of matches to perform */
	   case MANY | LITERAL:
	     p1 = *buf; buf++;
	     n = n0 = (int) (unsigned char) *buf++;
	     /* minimum number to match--- could be 0 */
	     n1 = (int) (unsigned char) *buf++;
	     /* maximum number to match */

	     while (n && (str < estr) && (p1 == *str)) 
	       {
		  n--;
		  str++;
	       }
	     if (n) return (NULL);
	     
	     save_str = str;
	     n = n1 - n0;
	     while (n && (str < estr) && (p1 == *str)) 
	       {
		  n--;
		  str++;
	       }
	     goto match_rest;
	     
	   case NTH_MATCH: 
	     if ((str = do_nth_match((int) (unsigned char) *buf, str, estr)) == NULL) return(NULL);
	     buf++;
	     break;
	     
	   case MAYBE_ONCE | NTH_MATCH:
	     save_str = str;
	     tmpstr = do_nth_match((int) (unsigned char) *buf, str, estr);
	     buf++;
	     if (tmpstr != NULL)
	       {
		  str = tmpstr;
		  goto match_rest;
	       }
	     continue;

	   case LEAST_ONCE | NTH_MATCH:
	     if ((str = do_nth_match((int) (unsigned char) *buf, str, estr)) == NULL) return(NULL);
	     /* drop */
	   case STAR | NTH_MATCH: 
	     save_str = str;
	     while (NULL != (tmpstr = do_nth_match((int) (unsigned char) *buf, str, estr)))
	       {
		  str = tmpstr;
	       }
	     buf++;
	     goto match_rest;
	     
	   case MANY | NTH_MATCH: return(NULL);
	     /* needs done */
	     
	   case RANGE: 
	     if (str >= estr) return (NULL);
	     if (TEST_BIT(buf, UPPERCASE(*str)) == 0) return (NULL);
	     buf += 32; str++;
	     break;
	     
	   case MAYBE_ONCE | RANGE:
	     save_str = str;
	     if ((str < estr) && TEST_BIT(buf, UPPERCASE(*str))) str++;
	     buf += 32;
	     goto match_rest;

	   case LEAST_ONCE | RANGE: 
	     if ((str >= estr) || (0 == TEST_BIT(buf, UPPERCASE(*str)))) return NULL;
	     str++;
	     /* drop */
	   case STAR | RANGE:
	     save_str = str;
	     while ((str < estr) && TEST_BIT(buf, UPPERCASE(*str))) str++;
	     buf += 32;
	     goto match_rest;
	     
	     /* The first 32 bytes correspond to the range and the two 
	      * following bytes indicate the min and max number of matches.
	      */
	   case MANY | RANGE:  
	     /* minimum number to match--- could be 0 */
	     n = n0 = (int) (unsigned char) *(buf + 32);
	     /* maximum number to match */
	     n1 = (int) (unsigned char) *(buf + 33);

	     while (n && (str < estr) && (TEST_BIT(buf, UPPERCASE(*str))))
	       {
		  n--;
		  str++;
	       }
	     if (n) return (NULL);
	     save_str = str;
	     n = n1 - n0;
	     while (n && (str < estr) && (TEST_BIT(buf, UPPERCASE(*str))))
	       {
		  n--;
		  str++;
	       }
	     buf += 34;		       /* 32 + 2 */
	     goto match_rest;
	     
	   case ANY_DIGIT:
	     if ((str >= estr) || (*str > '9') || (*str < '0')) return (NULL);
	     str++;
	     break;

	   case MAYBE_ONCE | ANY_DIGIT:
	     save_str = str;
	     if ((str < estr) && ((*str > '9') || (*str < '0'))) str++;
	     goto match_rest;

	   case LEAST_ONCE | ANY_DIGIT:
	     if ((str < estr) && ((*str > '9') || (*str < '0'))) return NULL;
	     str++;
	     /* drop */
	   case STAR | ANY_DIGIT:
	     save_str = str;
	     while ((str < estr) && ((*str <= '9') && (*str >= '0'))) str++;
	     goto match_rest;
	     
	   case MANY | ANY_DIGIT: 
	     /* needs finished */
	     return (NULL);
	     
	   case ANY:
	     if ((str >= estr) || (*str == '\n')) return (NULL);
	     str++;
	     break;
	     
	   case MAYBE_ONCE | ANY:
	     save_str = str;
	     if ((str < estr) && (*str != '\n')) str++;
	     goto match_rest;

	   case LEAST_ONCE | ANY:
	     if ((str >= estr) || (*str == '\n')) return (NULL);
	     str++;
	     /* drop */
	   case STAR | ANY:
	     save_str = str;
	     while ((str < estr) && (*str != '\n')) str++;
	     goto match_rest;
	     
	   case MANY | ANY: 
	     return (NULL);
	     /* needs finished */

	   case EOL: 
	     if ((str >= estr) || (*str == '\n')) return (str);
	     return(NULL);

	   default: return (NULL);
	  }
	p = *buf++;
	continue;
	
	match_rest:
	if (save_str == str)
	  {
	     p = *buf++;
	     continue;
	  }
	
	/* if (p == EOL)
	 * {
	 * if (str < estr) return (NULL); else return (str);
	 * } 
	 */
	
	save_next = Next_Pos;
	while (str >= save_str)
	  {
	     tmpstr = regexp_looking_at(str, estr, buf, cs);
	     if (tmpstr != NULL) return(tmpstr);
	     Next_Pos = save_next;
	     str--;
	  }
	return (NULL);
     }
   if ((p != 0) && (p != EOL)) return (NULL); else return (str);
}


unsigned char *SLang_regexp_match(unsigned char *str, 
				  unsigned int len, SLRegexp_Type *reg)
{
   register unsigned char c = 0, *estr = str + len;
   int cs = reg->case_sensitive, lit = 0;
   unsigned char *buf = reg->buf, *epos;

   if (reg->min_length > len) return NULL;
   
   Next_Pos = 1;
   This_Reg = reg;
   This_Str = str;

   if (*buf == BOL) 
     {
	if (NULL == (epos = regexp_looking_at(str, estr, buf + 1, cs))) str = NULL;
	reg->beg_matches[0] = (int) (str - This_Str);
	reg->end_matches[0] = (unsigned int) (epos - str);
	return(str);
     }

   if (*buf == NO_CASE)
     {
	buf++;  cs = 0;
     }
   
   if (*buf == YES_CASE)
     {
	buf++;  cs = 1;
     }
   
   if (*buf == LITERAL) 
     {
	lit = 1;
	c = *(buf + 1);
     }
   else if ((*buf == OPAREN) && (*(buf + 1) == LITERAL))
     {
	lit = 1;
	c = *(buf + 2);
     }
   
   while (str < estr)
     {
	/* take care of leading chars */
	if (lit)
	  {
	     while ((str < estr) && (c != UPPERCASE(*str))) str++;
	     if (str >= estr) return(NULL);
	  }
	Next_Pos = 1;
	if (NULL != (epos = regexp_looking_at(str, estr, buf, cs)))
	  {
	     reg->beg_matches[0] = (int) (str - This_Str);
	     reg->end_matches[0] = (unsigned int) (epos - str);
	     return (str);
	  }
	str++;
     }
   return (NULL);
}

static unsigned char *convert_digit(unsigned char *pat, int *nn)
{
   int n = 0, m = 0;
   unsigned char c;
   while (c = (unsigned char) *pat, (c <= '9') && (c >= '0'))
     {
	pat++;
	n = 10 * n + (c - '0');
	m++;
     }
   if (m == 0) 
     {
	return (NULL);
     }
   *nn = n;
   return pat;
}

#define ERROR  return (int) (pat - reg->pat)

/* Returns 0 if successful or offset in pattern of error */
int SLang_regexp_compile (SLRegexp_Type *reg)
{
   register unsigned char *buf, *ebuf, *pat;
   unsigned char *last = NULL, *tmppat;
   register unsigned char c;
   int i, reverse = 0, n, cs;
   int oparen = 0, dparen = 0, nparen = 0;
   /* substring stuff */
   int count, last_count, this_max_mm = 0, max_mm = 0, ordinary_search, 
     no_osearch = 0, min_length = 0;
   unsigned char *mm_p = NULL, *this_mm_p = NULL;
   static int already_initialized;
   
   reg->beg_matches[0] = reg->end_matches[0] = 0;
   buf = reg->buf;
   ebuf = (reg->buf + reg->buf_len) - 2; /* make some room */
   pat = reg->pat;
   cs = reg->case_sensitive;

   if (already_initialized == 0)
     {
	SLang_init_case_tables ();
#ifdef pc_system
	SLmake_lut (Word_Chars, (unsigned char *) "_0-9a-zA-Z\200-\232\240-\245\341-\353", 0);
#else
	SLmake_lut (Word_Chars, (unsigned char *) "_0-9a-zA-Z\277-\326\330-\336\340-\366\370-\376", 0);
#endif
	already_initialized = 1;
     }
   
   i = 1; while (i < 10) 
     {
	reg->beg_matches[i] = -1;
	reg->end_matches[i] = 0;
	i++;
     }

   if (*pat == '^')
     {
	pat++;
	*buf++ = BOL;
	reg->must_match_bol = 1;
     }
   else reg->must_match_bol = 0;

   *buf = 0;
   
   last_count = count = 0;
   while ((c = *pat++) != 0)
     {
	if (buf >= ebuf - 3)
	  {
	     SLang_doerror ("Pattern too large to be compiled.");
	     ERROR;
	  }
	
	count++;
	switch (c)
	  {
	   case '$': 
	     if (*pat != 0) goto literal_char;
	     *buf++ = EOL;
	     break;
	     
	   case '\\': 
	     c = *pat++;
	     no_osearch = 1;
	     switch(c)
	       {
		case 'e': c = 033; goto literal_char;
		case 'n': c = '\n'; goto literal_char;
		case 't': c = '\t'; goto literal_char;
		case 'C': cs = 0; *buf++ = NO_CASE; break;
		case 'c': cs = 1; *buf++ = YES_CASE; break;
		case '1': case '2': case '3':  case '4':  case '5':  
		case '6': case '7': case '8':  case '9': 
		  c = c - '0';
		  if ((int) c > nparen / 2) ERROR;
		  last = buf;
		  *buf++ = NTH_MATCH; *buf++ = c;
		  break;
#ifdef NOT_LITERAL
		case '~':	       /* slang extension */
		  if ((c = *pat) == 0) ERROR;
		  pat++;
		  last = buf;
		  *buf++ = NOT_LITERAL;
		  *buf++ = c;
		  min_length++;
		  break;
#endif
		case 'd':	       /* slang extension */
		  last = buf;
		  *buf++ = ANY_DIGIT;
		  min_length++;
		  break;
		  
		case '<':
		  last = NULL;
		  *buf++ = BOW;
		  break;
		  
		case '>':
		  last = NULL;
		  *buf++ = EOW;
		  break;
		  
		case '{':
		  if (last == NULL) goto literal_char;
		  *last |= MANY;
		  tmppat = convert_digit(pat, &n);
		  if (tmppat == NULL) ERROR;
		  pat = tmppat;
		  *buf++ = n;

		  min_length += (n - 1);
		  
		  if (*pat == '\\') 
		    {
		       *buf++ = n;
		    }
		  else if (*pat == ',')
		    {
		       pat++;
		       if (*pat == '\\')
			 {
			    n = 255;
			 }
		       else 
			 {
			    tmppat = convert_digit(pat, &n);
			    if (tmppat == NULL) ERROR;
			    pat = tmppat;
			    if (*pat != '\\') ERROR;
			 }
		       *buf++ = n;
		    }
		  else ERROR;
		  last = NULL;
		  pat++;
		  if (*pat != '}') ERROR;
		  pat++;
		  break;   /* case '{' */
		  
		case '(': 
		  oparen++;
		  if ((oparen > '9') || (dparen == 1)) ERROR;
		  dparen = 1;
		  nparen++;
		  *buf++ = OPAREN;
		  break;
		case ')':
		  if (dparen != 1) ERROR;
		  dparen = 0;
		  nparen++;
		  *buf++ = CPAREN;
		  break;

		case 0: ERROR;
		default:
		  goto literal_char;
	       }
	     break;

	   case '[':

	     *buf = RANGE;
	     last = buf++;
	     
	     if (buf + 32 >= ebuf) ERROR;

	     for (i = 0; i < 32; i++) buf[i] = 0;
	     c = *pat++;
	     if (c == '^') 
	       {
		  reverse = 1;
		  SET_BIT(buf, '\n');
		  c = *pat++;
	       }
	     
	     if (c == ']')
	       {
		  SET_BIT(buf, c);
		  c = *pat++;
	       }
	     while (c && (c != ']'))
	       {
		  if (c == '\\')
		    {
		       c = *pat++;
		       switch(c)
			 {
			    case 'n': c = '\n'; break;
			    case 't': c = '\t'; break;
			    case 0: ERROR;
			 }
		    }

		  if (*pat == '-')
		    {
		       pat++;
		       while (c < *pat)
			 {
			    if (cs == 0)
			      {
				 SET_BIT(buf, UPPERCASE(c));
				 SET_BIT(buf, LOWERCASE(c));
			      }
			    else SET_BIT(buf, c);
			    c++;
			 }
		    }
		  if (cs == 0)
		    {
		       SET_BIT(buf, UPPERCASE(c));
		       SET_BIT(buf, LOWERCASE(c));
		    }
		  else SET_BIT(buf, c);
		  c = *pat++;
	       }
	     if (c != ']') ERROR;
	     if (reverse) for (i = 0; i < 32; i++) buf[i] = buf[i] ^ 0xFF;
	     reverse = 0;
	     buf += 32;
	     min_length++;
	     break;
	     
	   case '.':
	     last = buf;
	     *buf++ = ANY;
	     min_length++;
	     break;
	     
	   case '*': 
	     if (last == NULL) goto literal_char;
	     *last |= STAR;
	     min_length--;
	     last = NULL;
	     break;
	
	   case '+': 
	     if (last == NULL) goto literal_char;
	     *last |= LEAST_ONCE;
	     last = NULL;
	     break;

	   case '?':
	     if (last == NULL) goto literal_char;
	     *last |= MAYBE_ONCE;
	     last = NULL;
	     min_length--;
	     break;

	   literal_char:
	   default:
	     /* This is to keep track of longest substring */
	     min_length++;
	     this_max_mm++;
	     if (last_count + 1 == count)
	       {
		  if (this_max_mm == 1)
		    {
		       this_mm_p = buf;
		    }
		  else if (max_mm < this_max_mm)
		    {
		       mm_p = this_mm_p;
		       max_mm = this_max_mm;
		    }
	       }
	     else 
	       {
		  this_mm_p = buf;
		  this_max_mm = 1;
	       }
	     
	     last_count = count;
		       
	     last = buf;
	     *buf++ = LITERAL;
	     *buf++ = UPPERCASE(c);
	  }
     }
   *buf = 0;
   /* Check for ordinary search */
   ebuf = buf;
   buf = reg->buf;
   
   if (no_osearch) ordinary_search = 0;
   else
     {
	ordinary_search = 1;
	while (buf < ebuf)
	  {
	     if (*buf != LITERAL) 
	       {
		  ordinary_search = 0;
		  break;
	       }
	     buf += 2;
	  }
     }
   
   reg->osearch = ordinary_search;
   reg->must_match_str[15] = 0;
   reg->min_length = (min_length > 0) ? (unsigned int) min_length : 0;
   if (ordinary_search)
     {
	strncpy((char *) reg->must_match_str, (char *) reg->pat, 15);
	reg->must_match = 1;
	return(0);
     }
   /* check for longest substring of pattern */
   reg->must_match = 0;
   if ((mm_p == NULL) && (this_mm_p != NULL)) mm_p = this_mm_p;
   if (mm_p == NULL)
     {
	return (0);
     }
   n = 15;
   pat = reg->must_match_str;
   buf = mm_p;
   while (n--)
     {
	if (*buf++ != LITERAL) break;
	*pat++ = *buf++;
     }
   *pat = 0;
   if (pat != reg->must_match_str) reg->must_match = 1;
   return(0);
}

char *SLregexp_quote_string (char *re, char *buf, unsigned int buflen)
{
   char ch;
   char *b, *bmax;
   
   if (re == NULL) return NULL;
   
   b = buf;
   bmax = buf + buflen;
   
   while (b < bmax)
     {
	switch (ch = *re++)
	  {
	   case 0:
	     *b = 0;
	     return buf;
	     
	   case '$':
	   case '\\':
	   case '[':
	   case ']':
	   case '.':
	   case '^':
	   case '*':
	   case '+':
	   case '?':
	     *b++ = '\\';
	    if (b == bmax) break;
	     /* drop */
	     
	   default:
	     *b++ = ch;
	  }
     }
   return NULL;
}

/*
#define MAX_EXP 4096
int main(int argc, char **argv)
{
   FILE *fp;
   char *regexp, *file;
   char expbuf[MAX_EXP], buf[512];
   SLRegexp_Type reg;
   
   file = argv[2];
   regexp = argv[1];
   
   if (NULL == (fp = fopen(file, "r")))
     {
	fprintf(stderr, "File not open\n");
	return(1);
     }
   
   reg.buf = expbuf;
   reg.buf_len = MAX_EXP;
   reg.pat = regexp;
   reg.case_sensitive = 1;

   if (!regexp_compile(&reg)) while (NULL != fgets(buf, 511, fp))
     {
	if (reg.osearch)
	  {
	     if (NULL == strstr(buf, reg.pat)) continue;
	  }
	else 
	  {
	     if (reg.must_match && (NULL == strstr(buf, reg.must_match_str))) continue;
	     if (0 == regexp_match(buf, buf + strlen(buf), &reg)) continue;
	  }
	
	fputs(buf, stdout);
     }
   return (0);
}
*/
