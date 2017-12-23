/* $Header: /home/schj/src/vamps_0.99g/src/main/RCS/scanf.c,v 1.6 1999/01/06 12:13:01 schj Alpha $ */

/* $RCSfile: scanf.c,v $
 *  $Author: schj $
 *  $Date: 1999/01/06 12:13:01 $ */

#ifdef HAVE_CONFIG_H
#include "vconfig.h"
#endif

#ifdef HAVE_LIBSLANG

#include <string.h>
#include <stdio.h>

static char RCSid[] =
 "$Id: scanf.c,v 1.6 1999/01/06 12:13:01 schj Alpha $";

#include "slang.h"
#include "slfile.h"

#define PERC 1
#define DBLPERC 2
#define VERBAT 3

#define SC_INT_TYPE 2
#define SC_FLOAT_TYPE 3
#define SC_DOUBLE_TYPE 6
#define SC_LINT_TYPE 7
#define SC_SINT_TYPE 8
#define SC_UINT_TYPE 9
#define SC_STRING_TYPE 4
#define SC_CSTRING_TYPE 5
#define SC_NOTHING_TYPE 1
#define SC_ERROR 10

/*F:scanf.c
 *@ [fs]scanf functionality for S-Lang 
 *
 * sl_fscanf breaks the fmt string in one-item pieces that can be
 * passed on to fscanf directly. It pushes all converted items on the
 * stack.
 *
 * core:     int sl_fscanf(FILE *sfile, char *fmt, char *s)
 * wrappers: int slash_scanf(char *fmt)
 *           int slash_fscanf(void)
 *	    int slash_sscanf(void)
 *
 * use in C:
 * MAKE_INTRINSIC(".scanf", slash_scanf,INT_TYPE,1)
 * MAKE_INTRINSIC(".fscanf", slash_fscanf,INT_TYPE,0)
 * MAKE_INTRINSIC(".fscanf", slash_sscanf,INT_TYPE,2)
 * use in S-Lang:
 * 	int scanf(char *fmt, int items)
 * 	int fscanf(int fp, char *fmt, int items)
 * 	int sscanf(char *str, char *fmt, int items)
 *
 * BUGS 
 *  - the %c conversion in sscanf adds a '\0' for practicality
 *  - it is a lot of code and probably not very fast
 *  - %p not implemented, ignored     
 *  - %a -> malloced string not implemented (non ansii anyway)
 *  - ll and L (long long) treated as long 
 *
 * Jaap Schellekens
 */


/* At the moment we need this stuff from slfiles.c */
/* returns pointer to file entry if it is open and consistent with 
   flags.  Returns NULL otherwise */
static SL_File_Table_Type *pop_fp (unsigned int flags)
{
	int n;
	SL_File_Table_Type *t;
	
	if (SLang_pop_integer (&n)) return NULL;
	
	if ((n < 0) || (n >= SL_MAX_FILES)) return(NULL);
  
	t = SL_File_Table + n;
	if (t->fd == -1) return (NULL);
	if (flags & t->flags) return (t);
	return NULL;
}


/*C:cktype
 *@int cktype (char *fmt)
 *
 *  Checks type that should be pushed on stack
 *  Returns one of the SC_* defines
 */
int 
cktype(char *fmt)
{
	int retval = 0;
	int longflag = 0;
	int shortflag = 1;

	while(*fmt){
		switch (*fmt){
			case '*': retval = SC_NOTHING_TYPE; break;
			case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8':
			case '9': retval = 0; break;
			case 'l': retval = 0; longflag = 1; break;    
			case 'L': retval = 0; longflag = 1; break;
			case 'q': retval = 0; longflag = 1; break;
			case 'a': retval = SC_ERROR; break; /* malloced string */
			case 'h': retval = 0; shortflag = 1;
			case 'd': retval = SC_INT_TYPE; break; /* int */
			case 'D': retval = SC_LINT_TYPE; break; /* int */
			case 'i': retval = SC_INT_TYPE; break; /* int */
			case 'o': retval = SC_UINT_TYPE; break; /* int */
			case 'u': retval = SC_UINT_TYPE; break; /* int */
			case 'x': retval = SC_UINT_TYPE; break; /* int */
			case 'X': retval = SC_UINT_TYPE; break; /* int */
			case 'f': retval = SC_FLOAT_TYPE; break; /* double */
			case 'e': retval = SC_FLOAT_TYPE; break; /* double */
			case 'g': retval = SC_FLOAT_TYPE; break; /* double */
			case 'E': retval = SC_FLOAT_TYPE; break; /* double */
			case 's': retval = SC_STRING_TYPE; break; /* string - non whitespace */
			case 'c': retval = SC_CSTRING_TYPE; break; /* string - count characters */
			case '[': retval = SC_STRING_TYPE; break; /* string - count characters */
			case 'p': retval = SC_ERROR; break; /* pointer - count characters */
			case 'n': retval = SC_INT_TYPE; break; /* int - num of characters read */
			default: retval = 0; break;
		}
		if (retval)
			break;
		fmt++;
	}

	if (longflag)
		switch (retval){
			case SC_FLOAT_TYPE: retval = SC_DOUBLE_TYPE; break;
			case SC_INT_TYPE: retval = SC_LINT_TYPE; break;
		}

	if (shortflag && retval == SC_INT_TYPE)
		retval = SC_SINT_TYPE;

	return retval;
}

/*C:pushit 
 *@int pushit(int type, FILE *sfile, char *fmt, int *fscanret);
 *
 * Pushit take a type, a stream, and an one-item format string.
 * pushit checks the S-Lang type to conversion-type relation. It then
 * uses fscanf to do the conversion and pushes the items on the
 * stack depending on conversion flags.  
 * fscanret holds fscanf's return value.
 */

int 
pushit(int type, FILE *sfile,char *fmt,int *fscanfret) 
{ 
	double v_dflt; 
	float v_flt; 
	int v_int; 
	unsigned int v_uint; 
	short int v_sint;
	long int v_lint; 
	char v_s[2048]; 
	int push = 0;

	switch (type){
		case SC_ERROR:
			break;
		case SC_INT_TYPE: 
			*fscanfret = fscanf(sfile,fmt,&v_int); 
			if (*fscanfret == EOF)
				break;
			SLang_push_integer(v_int);
			push++;
			break;
		case SC_SINT_TYPE: 
			*fscanfret = fscanf(sfile,fmt,&v_sint); 
			if (*fscanfret == EOF)
				break;
			SLang_push_integer((int)v_sint);
			push++;
			break;
		case SC_LINT_TYPE: 
			*fscanfret = fscanf(sfile,fmt,&v_lint); 
			if (*fscanfret == EOF)
				break;
			SLang_push_integer((int)v_lint);
			push++;
			break;
		case SC_UINT_TYPE:
			*fscanfret = fscanf(sfile,fmt,&v_uint); 
			if (*fscanfret == EOF)
				break;
			SLang_push_float((int)v_uint);
			push++;
			break;
		case SC_DOUBLE_TYPE: 
			*fscanfret = fscanf(sfile,fmt,&v_dflt); 
			if (*fscanfret == EOF)
				break;
			SLang_push_float(v_dflt);
			push++;
			break;
		case SC_FLOAT_TYPE: 
			*fscanfret = fscanf(sfile,fmt,&v_flt); 
			if (*fscanfret == EOF)
				break;
			SLang_push_float((double)v_flt);
			push++;
			break;
		case SC_CSTRING_TYPE: 
		case SC_STRING_TYPE: 
			*fscanfret = fscanf(sfile,fmt,v_s); 
			if (*fscanfret == EOF)
				break;
			SLang_push_string(v_s);
			push++;
			break;
		case SC_NOTHING_TYPE:
			*fscanfret = fscanf(sfile,fmt); 
			if (*fscanfret == EOF)
				break;
			break;
		default: break;
	}

	return push;
}


/*C:spushit 
 *@int spushit(int type, char *s, char *fmt, int *sscanret, int *startpos);
 *
 * spushit take a type, a string, an one-item format string and a start
 * position (needed because we uses multiple calls to sscanf);
 * pushit checks the S-Lang type to conversion-type relation. It then
 * uses sscanf to do the conversion and pushes the items on the
 * stack depending on conversion flags.  
 * sscanret holds sscanf's return value.
 */
int 
spushit(int type,char *s,char *fmt,int *sscanfret, int *startpos) 
{ 
	double v_dflt; 
	float v_flt; 
	int v_int; 
	unsigned int v_uint; 
	short int v_sint;
	long int v_lint; 
	char v_s[2048]; 
	int push = 0;
	int nrdone = 0;
	register int i;


	/* since we break the string up in pieces we need to know
	   where we stopped. To do this we append the %n thing to
	   the fmt string which will tell us where we have ended.*/
	for (i = 0; i < *startpos && *s; i++) 
		s++;
	strcat (fmt,"%n");

	switch (type){
		case SC_ERROR:
			break;
		case SC_INT_TYPE: 
			*sscanfret = sscanf(s,fmt,&v_int,&nrdone); 
			if (*sscanfret == EOF)
				break;
			SLang_push_integer(v_int);
			push++;
			break;
		case SC_SINT_TYPE: 
			*sscanfret = sscanf(s,fmt,&v_sint,&nrdone); 
			if (*sscanfret == EOF)
				break;
			SLang_push_integer((int)v_sint);
			push++;
			break;
		case SC_LINT_TYPE: 
			*sscanfret = sscanf(s,fmt,&v_lint,&nrdone); 
			if (*sscanfret == EOF)
				break;
			SLang_push_integer((int)v_lint);
			push++;
			break;
		case SC_UINT_TYPE:
			*sscanfret = sscanf(s,fmt,&v_uint,&nrdone); 
			if (*sscanfret == EOF)
				break;
			SLang_push_float((int)v_uint);
			push++;
			break;
		case SC_DOUBLE_TYPE: 
			*sscanfret = sscanf(s,fmt,&v_dflt,&nrdone); 
			if (*sscanfret == EOF)
				break;
			SLang_push_float(v_dflt);
			push++;
			break;
		case SC_FLOAT_TYPE: 
			*sscanfret = sscanf(s,fmt,&v_flt,&nrdone); 
			if (*sscanfret == EOF)
				break;
			SLang_push_float((double)v_flt);
			push++;
			break;
		case SC_CSTRING_TYPE: 
			*sscanfret = sscanf(s,fmt,v_s,&nrdone); 
			if (*sscanfret == EOF)
				break;
			v_s[nrdone] = '\0';
			SLang_push_string(v_s);
			push++;
			break;
		case SC_STRING_TYPE: 
			*sscanfret = sscanf(s,fmt,v_s,&nrdone); 
			if (*sscanfret == EOF)
				break;
			SLang_push_string(v_s);
			push++;
			break;
		case SC_NOTHING_TYPE:
			*sscanfret = sscanf(s,fmt,&nrdone); 
			if (*sscanfret == EOF)
				break;
			break;
		default: break;
	}

	*startpos += nrdone;
	return push;
}


/*C:sl_fscanf
 *@int sl_fscanf(FILE *sfile, char *fmt, char *s)
 *
 * Breaks the fmt string into single-item strings for pushit or
 * spushit to process. If FILE == NULL spushit is called, otherwise
 * pushit is called.
 *
 * return: the number of items pushed on the stack.
 */
int
sl_fscanf(FILE *sfile,char *fmt, char *str)
{
	int nrparts=0;
	char buf[256],s[256];
	int inpart=0;
	int flag = VERBAT;
	int nrpush = 0;
	int ret = 0;
	int strpos = 0;

	if (!*fmt)
		return 0;

	/* Get leading characters */
	while (*fmt && *fmt != '%'){
		buf[inpart] = *fmt;
		inpart++;
		fmt++;
	}

	/* if no % sign, got to wrap up at end */
	if (*fmt){
		fmt++;
		if (inpart){ /* All stuff leading the first % sign */
			buf[inpart] = '\0';
			nrpush += sfile == NULL ? 
				spushit(cktype(buf),str,s,&ret,&strpos) : 
				pushit(cktype(buf),sfile,s,&ret);
		}
		nrparts++;
		inpart = 0;

		while (*fmt){
			switch(*fmt){
				case '%': 
					fmt++;
					if (*fmt == '%'){
						buf[inpart] = *fmt;
						inpart++;
						buf[inpart] = '%';
						inpart++;
						flag = DBLPERC;
					}else{
						flag = PERC;
						fmt--;
						nrparts++;
						buf[inpart] = '\0';		
						strcpy(s,"%");strcat(s,buf);
						nrpush += sfile == NULL ? 
							spushit(cktype(buf),str,s,&ret,&strpos) : 
							pushit(cktype(buf),sfile,s,&ret);
						inpart = 0;
					}
					break;
				default: 
					flag = VERBAT;
					buf[inpart] = *fmt;
					inpart++;
					break;
			}
			fmt++;
		}
	}

	/* Wrap things up, last fmt str */
	if(flag == VERBAT && nrparts > 0){
		buf[inpart] = '\0';		
		strcpy(s,"%");strcat(s,buf);
		nrpush += sfile == NULL ? 
			spushit(cktype(buf),str,s,&ret,&strpos) : 
			pushit(cktype(buf),sfile,s,&ret);
	}else if (nrparts == 0){
		buf[inpart] = '\0';		
		nrpush += sfile == NULL ? 
			spushit(cktype(buf),str,s,&ret,&strpos) : 
			pushit(cktype(buf),sfile,s,&ret);
	}

	if (ret == EOF)
		return EOF;
	else
		return nrpush;
}


/* Wrappers for SL_fscanf */

int
slash_scanf(char *fmt)
{
	return sl_fscanf(stdin,fmt, NULL);
}

int
slash_fscanf(void)
{
	SL_File_Table_Type *t;
	char *fmt;
	FILE *fp;

	if (SLpop_string(&fmt)) return 0;
	if (NULL == (t = pop_fp(SL_READ))) return 0;
	fp = t->fp;
	return sl_fscanf(fp,fmt, NULL);
}

int 
slash_sscanf (char *s, char *fmt)
{
	return sl_fscanf(NULL, fmt, s);
}
#endif
