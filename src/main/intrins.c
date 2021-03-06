
#ifdef HAVE_CONFIG_H
#include "vconfig.h"
#endif

#ifdef HAVE_LIBSLANG
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <slang.h>
#include "nrutil.h"

#ifndef FLOAT_TYPE
#define FLOAT_TYPE 3
#endif

#define SLASH_FORMAT	"%6g"

static char RCSid[] =
 "$Id: intrins.c,v 1.4 1999/01/06 12:13:01 schj Alpha $";

char	*sl_format;		/* default floating point format */
char	sl_path[2048];		/* library pathlist */

/* in slash.c */
extern char	sl_arg0[];
extern int	version;
extern int	sl_tty;

/* in misc.c */
extern char	**sl_make_strv(char *, char *, int *);
extern char	*sl_fgetl(FILE *);
extern char	*sl_statfile(char *, char *);
extern void	sl_free_strv(char **);
extern void	sl_memerr(void);
extern void	sl_exit(int);

/* in help.c */
extern void	sl_setidx(char *idxname);

extern SLuser_Object_Type	*SLcreate_array(); /* not in slang.h */

typedef union {
	int	i;
	double	f;
	char	*s;
} SL_ARG;

static int	d1, d2;

/*-
 * Recursive argument parsing based on spec in fmt.
 * Returns 0 on success, -1 on error.
-*/
int
sl_pop_args(char *fmt, SL_ARG *argv)
{
	if(!*fmt)
		return(0);
	if(!sl_pop_args(fmt + 1, argv + 1)) {
		switch(*fmt) {
		case	'i':
			return(SLang_pop_integer(&argv->i));
		case	'f':
			return(SLang_pop_float(&argv->f, &d1, &d2));
		case	's':
			return(SLpop_string(&argv->s));
		}
	}
	return(-1);
}

/*I:exit
 *@ exit
 * Terminate execution.
*/
void
slash_exit(void)
{
	sl_exit(0);
	/* NOTREACHED */
}

/*I:error
 *@ Void error(String str)
 * Prints the text held in @str@ followed by a newline on standard
 * error output and raises internal error status.
*/
void
slash_error(char *s)
{
	(void)fprintf(stderr, "%s\n", s);
	SLang_Error = INTRINSIC_ERROR;
}

/*I:getarg
 *@ String getarg()
 * Returns a string holding the next command line argument or
 * ``@--@'' if there are no options left. Increases the internal
 * index to the command line arguments each time it is called.
 * For example:
 *
 *@	variable arg;
 *@	while(arg = getarg(), strcmp("--", arg))
 *@	{
 *@		...
 *@	}
*/
char *
slash_getarg(void)
{
	extern char	**sl_argv;
	extern int	sl_argc, sl_optind;

	if(sl_optind < sl_argc)
		return(sl_argv[sl_optind++]);

	return("--");
}

/*I:format
 *@ Void format(String fmt)
 * Sets the floating point output format to @fmt@. If given
 * as the nullstring (""), the default format is restored.
 * The default can be set by calling @format@ from @.vampssl@,
 * else it is set to ``@%6g@''. (Using the S-Lang intrinsic
 * @set_float_format@ is deprecated.)
*/
void
slash_format(char *s)
{
	if(sl_format == (char *)NULL) {
	/* called from slashrc or main(), set default */
		if(s == (char *)NULL || *s == '\0')
			s = SLASH_FORMAT;
		sl_format = SLmake_string(s);
	}
	if(s == (char *)NULL || *s == '\0')	/* restore */
		s = sl_format;

	/* set_float_format is declared static in libslang !! */
	SLang_run_hooks("set_float_format", s, (char *)NULL);
}

/*I:prompt
 *@ String prompt(String str1, String str2)
 * Sets the prompts used in interactive mode. Default prompts
 * are ``@Slash> @'' and ``@_Slash> @''. If either argument
 * is the nullstring (@""@), that prompt is not changed. The
 * sequences @%#@ and @%$@ are translated to the current input
 * line number and stack depth respectively.
*/
void
slash_prompt(char *s1, char *s2)
{
	extern char	*sl_prompt[2];

	if(s1 != (char *)NULL && *s1) {
		SLFREE(sl_prompt[0]);
		sl_prompt[0] = SLmake_string(s1);
	}
	if(s2 != (char *)NULL && *s2) {
		SLFREE(sl_prompt[1]);
		sl_prompt[1] = SLmake_string(s2);
	}
	return;
}

/*I:strv
 *@ Array strv(String str, String sep)
 * Splits the string @str@ into fields separated by one or
 * more characters from the string @sep@. A terminating
 * newline is always considered a field separator. Returns a
 * vector (1D array) of strings set to the individual fields.
 * For example:
 *
 *@	variable fid, i, s, v;
 *@	fid = fopen("my_file.dat", "r");
 *@	while(fgets(fid) > 0)
 *@	{
 *@		s = ();
 *@		v = strv(s, " \t");
 *@		for(i = 0; i < asize(v); i++)
 *@			printf("%16s\n", v[i], 1);
 *@	}
 *
 * reads an entire data file of fields separated by runs of
 * whitespace characters and prints the fields on standard
 * output.
*/
void
slash_strv(char *str, char *sep)
{
	int	nf;
	char	**strv;
	SLuser_Object_Type	*arr;

	strv = sl_make_strv(str, sep, &nf);
	arr = SLcreate_array((char *)strv, 1, nf, 0, 0, 's', 0);
	SLang_push_user_object(arr);
	return;
}

/* array types */
typedef struct SLArray_Type
{
   unsigned char type;		       /* int, float, etc... */
   int dim;			       /* # of dims (max 3) */
   int x, y, z;			       /* actual dims */
   union
     {
	unsigned char *c_ptr;
	unsigned char **s_ptr;
	int *i_ptr;
#ifdef FLOAT_TYPE
	float64 *f_ptr;
#endif
	SLuser_Object_Type **u_ptr;
     }
   buf;
   unsigned char flags;		       /* readonly, etc...  If this is non-zero,
					* the buf pointer will NOT be freed.
					* See SLarray_free_array.
					*/
} SLArray_Type;


/*I:mwrite
 *@ Void mwrite(Array m, String fname)
 *
 * Writes the double matrix @m@ to an ascii file
 * Returns nothing;
 */ 
void
sl_mwrite()
{
	SLuser_Object_Type *uob;
	SLArray_Type *at;
	int i1,i = 0,d = 0;
	static char *name;
	float64 *jj;
	FILE *ofile;

	if (SLang_pop_string(&name, &d)){
		Perror("mwrite",0,0,RCSid,"Could not pop argument 2","");
		return;
	}
	if (NULL == (uob = (SLuser_Object_Type *)SLang_pop_array(&i1))){
		Perror("mwrite",0,0,RCSid,"Could not pop argument 1","");
		return;
	}

	at = (SLArray_Type *) uob->obj;

	if (at->type != FLOAT_TYPE){
		Perror("mwrite",0,0,RCSid,"Can only write FLOAT matrix","");
		return;
	}
	jj = at->buf.f_ptr;

	if (nr_genw(name,(void *)jj) == -1){
		if ((ofile = fopen(name,"w")) == NULL){
			Perror("mwrite",0,0,RCSid,"Could not open:",name);
			return;
		}

		for (i=0;i<at->x * at->y;i++){
			(void)fprintf(ofile,"%f\t",jj[i]);
			if (i % at->y == at->y -1)
				(void)fprintf(ofile,"\n");
		}

		(void) fclose(ofile);
	}

	return;
}

/*I:mread
 *@ Array mread(String file, Integer nc)
 * Reads a matrix (2D floating point array) from the named
 * ascii @file@, which is supposed to consist of lines with
 * numerical fields separated by whitespace (spaces, TABS
 * and/or newlines). Lines starting with `@#@' or `@%@' are
 * considered comments; blank lines are skipped. If @nc@>0,
 * the number of matrix columns are set to @nc@. If @nc@ is
 * zero or less, the number of columns are set equal to the
 * number of fields in the input lines. The number of matrix
 * rows are calculated from dividing the number of values by
 * the number of lines in the input @file@.
*/
void
slash_mread(void)
{
	int n, m, d, nc;
	char	*name;
	double	**p;

	if(SLang_pop_integer(&nc) || SLang_pop_string(&name, &d)){
		Perror("mread",0,0,RCSid,"Could not pop arguments","(2)");
		return;
	}


	p = nr_dmread(name,&m,&nc);

	if (p){
		SLang_push_user_object(SLcreate_array((char *)&p[0][0], 2, m, nc, 0, 'f', 0));
	} else{
		Perror("mread",0,0,RCSid,"Could not read:",name);
	}
	return;
}

/*I:slpath
 *@ String slpath(String str)
 * Sets a colon (`@:@') separated pathlist of directories to
 * search when loading S-Lang command files (using @autoload@
 * and @evalfile@).  If @str@ is the nullstring (@""@), the
 * pathlist is not changed. The returned string holds the
 * previous pathlist.
 *
 * Within a pathlist specification, a single dot (`@.@') denotes
 * the current working directory. If the first character of
 * a path in the list is the tilde (`@~@'), it is expanded to
 * the home directory obtained from the @HOME@ environment
 * variable. For example, if the user maintains a library of
 * S-Lang command files in a directory @$HOME/lib/vamps@,
 * the command:
 *
 *@	() = slpath(strcat("~/lib/vamps:", slpath("")));
 *
 * can be placed in the @.vampssl@ file in the user's home
 * directory. Script files, however, should restore the
 * previous pathlist before terminating.
*/
void
slash_slpath(char *path)
{
	int	i, n, nf;
	char	*p, *pp;
	char	*home, **strv;

	SLang_push_malloced_string(SLmake_string(sl_path));
	while(*path == ':')
		path++;
	if(*path == '\0')
		return;

	/* copy path and expand '~' */
	home = getenv("HOME");
	p = sl_path;
	*p = ':';
	while(*p) {
		if(*p++ == ':' && *path == '~' && home != (char *)NULL) {
			for(pp = home; *pp; pp++)
				*p++ = *pp;
			path++;
		}
		*p = *path++;
	}

	/* rebuild and check redundancy */
	strv = sl_make_strv(sl_path, ":", &nf);
	strcpy(sl_path, strv[0]);
	for(n = 1; n < nf; n++) {
		for(i = 0; i < n; i++) {
			if(!strcmp(strv[i], strv[n]))
				break;
		}
		if(i == n) {
			strcat(sl_path, ":");
			strcat(sl_path, strv[n]);
		}
	}

	sl_free_strv(strv);
	return;
}

/* redefined autoload and evalfile to use sl_path
*/
void
slash_autoload(char *func, char *file)
{
	char	*path;

	if((path = sl_statfile(file, sl_path)) == (char *)NULL) {
		fprintf(stderr, "%s: file {%s}/%s not found\n",
			sl_arg0, sl_path, file);
		return;
	}
	SLang_autoload(func, path);
	SLFREE(path);

	return;
}

int
slash_evalfile(char *file)
{
	char	*path;

	if((path = sl_statfile(file, sl_path)) == (char *)NULL) {
		fprintf(stderr, "%s: file {%s}/%s not found\n",
			sl_arg0, sl_path, file);
		return(0);
	}
	SLang_load_file(path);
	SLFREE(path);

	return(1);
}

/*I:pause
 *@ Void pause(Integer n)
 * Suspends processing for @n@ seconds, if @n@ is positive. If
 * @n@ is less than or equals zero, processing waits until a key
 * is pressed on the keyboard for which the user is prompted on
 * standard error.
*/
void
slash_pause(void) {
	int     secs;

	if(SLang_pop_integer(&secs))
		return;
	if(secs > 0)
		sleep(secs);
	else if(sl_tty) {
		(void)fprintf(stderr, "Press any key ");
		if(sl_tty & 0x1) {
			SLang_getkey();
			SLang_flush_input();
			(void)putc('\n', stderr);
		}
		else {
			if(fgetc(stdin) != '\n')
				(void)putc('\n', stderr);
		/*	(void)fpurge(stdin); */
		}
	}

	return;
}

#ifdef GRAPHICS
/* prelim graphics */
int
slash_gopen(void)
{
	int	ret;
	char	*dev, *name;

	ret = -1;
	if(!SLpop_string(&name)) {
		if(!SLpop_string(&dev)) {
			ret = sl_gopen(dev, name);
			SLFREE(dev);
		}
		SLFREE(name);
	}
	return(ret);
}

int
slash_gclose(void)
{
	int	gid;

	if(SLang_pop_integer(&gid))
		return(-1);

	return(sl_gclose(gid));
}

int
slash_ginit(void)
{
	SL_ARG	a[5];

	if(sl_pop_args("iffff", a))
		return(-1);

	return(sl_ginit(a[0].i, a[1].f, a[2].f, a[3].f, a[4].f));
}

int
slash_gsize(void)
{
	SL_ARG	a[3];

	if(sl_pop_args("iff", a))
		return(-1);

	return(sl_gsize(a[0].i, a[1].f, a[2].f));
}

int
slash_gscale(void)
{
	SL_ARG	a[3];

	if(sl_pop_args("iff", a))
		return(-1);

	return(sl_gscale(a[0].i, a[1].f, a[2].f));
}

int
slash_grotate(void)
{
	SL_ARG	a[2];

	if(sl_pop_args("if", a))
		return(-1);

	return(sl_grotate(a[0].i, a[1].f));
}

int
slash_gtranslate(void)
{
	SL_ARG	a[3];

	if(sl_pop_args("iff", a))
		return(-1);

	return(sl_gtranslate(a[0].i, a[1].f, a[2].f));
}

int
slash_gerase(void)
{
	int	gid;

	if(SLang_pop_integer(&gid))
		return(-1);

	return(sl_gerase(gid));
}

int
slash_gshow(void)
{
	int	gid;

	if(SLang_pop_integer(&gid))
		return(-1);

	return(sl_gshow(gid));
}

int
slash_gmove(void)
{
	SL_ARG	a[3];

	if(sl_pop_args("iff", a))
		return(-1);

	return(sl_gmove(a[0].i, a[1].f, a[2].f));
}

int
slash_gline(void)
{
	SL_ARG	a[3];

	if(sl_pop_args("iff", a))
		return(-1);

	return(sl_gline(a[0].i, a[1].f, a[2].f));
}

int
slash_gplot(void)
{
	int	gid;

	if(SLang_pop_integer(&gid))
		return(-1);

	return(sl_gplot(gid));
}

int
slash_mapxy(void)
{
	SL_ARG	a[3];

	if(sl_pop_args("iff", a))
		return(-1);

	return(sl_mapxy(a[0].i, a[1].f, a[2].f));
}
#endif

/* in scanf.c */
extern int	slash_scanf(char *fmt);
extern int	slash_fscanf(int fid, char *fmt);
extern int	slash_sscanf(char *str, char *fmt);

/* the function/variable table */
SLang_Name_Type sl_intrinsics[] = {
/*I:arg0
 *@ String arg0
 * Readonly variable holding the name of the application
 * (``@slash@'' if running interactive) or the name of the
 * current script file being processed.
 */
	MAKE_VARIABLE(".arg0", sl_arg0, STRING_TYPE, 1),
/*I:version
 *@ Integer version
 * Readonly variable holding the current @slash@ version number
 * multiplied by 100.
 */
	MAKE_VARIABLE(".version", &version, INT_TYPE, 1),
/*I:scrcols
 *I:scrrows
 *@ Integer scrcols
 *@ Integer scrrows
 * Readonly variables set to the number of (text) columns and
 * rows of the screen. These values change when the window
 * size changes.
 */
	MAKE_VARIABLE(".scrcols", &SLtt_Screen_Cols, INT_TYPE, 1),
	MAKE_VARIABLE(".scrrows", &SLtt_Screen_Rows, INT_TYPE, 1),
/*I:tty
 *@ Integer tty
 * 	Readonly variable set to @1@ if standard output is a tty, @2@
 * 	if standard input is a tty, @3@ if both standard input and
 * 	output are connected to a tty, else set to @0@.
 */
	MAKE_VARIABLE(".tty", &sl_tty, INT_TYPE, 1),
/*I:scanf
 *I:fscanf
 *I:sscanf
 *@ ..., Integer scanf(String *fmt)
 *@ ..., Integer fscanf(Integer fid, String *fmt)
 *@ ..., integer sscanf(String *str, String *fmt)
 *
 * Formatted variable assignment like their C counterparts
 * (see @scanf(3)@), except that the assigned values, followed
 * by the number of assignments are placed on the stack. For
 * example:
 * 
 *@	variable month, day;
 *@	if(sscanf("December, 5\n", "%[A-Z a-z], %d\n") == 2)
 *@	{
 *@		(month, day) = ();
 *@		...
 *@	}
 *
 * pushes a string followed by an integer on the stack.  
*/
	MAKE_INTRINSIC(".scanf", slash_scanf, INT_TYPE, 1),
	MAKE_INTRINSIC(".fscanf", slash_fscanf, INT_TYPE, 0),
	MAKE_INTRINSIC(".sscanf", slash_sscanf, INT_TYPE, 2),
	MAKE_INTRINSIC(".error", slash_error, VOID_TYPE, 1),
	MAKE_INTRINSIC(".exit", slash_exit, VOID_TYPE, 0),
	MAKE_INTRINSIC(".prompt", slash_prompt, VOID_TYPE, 2),
	MAKE_INTRINSIC(".getarg", slash_getarg, STRING_TYPE, 0),
	MAKE_INTRINSIC(".slpath", slash_slpath, VOID_TYPE, 1),
	MAKE_INTRINSIC(".strv", slash_strv, VOID_TYPE, 2),
	MAKE_INTRINSIC(".mread", slash_mread, VOID_TYPE, 0),
	MAKE_INTRINSIC(".mwrite", sl_mwrite, VOID_TYPE, 0),
	MAKE_INTRINSIC(".format", slash_format, VOID_TYPE, 1),
	MAKE_INTRINSIC(".pause", slash_pause, VOID_TYPE, 0),
/* redefine these two to use slpath */
	MAKE_INTRINSIC(".autoload", slash_autoload, VOID_TYPE, 2),
	MAKE_INTRINSIC(".evalfile", slash_evalfile, INT_TYPE, 1),
	MAKE_INTRINSIC(".setidx", sl_setidx, VOID_TYPE, 1),
	/*I:setidx
	 *@void sl_setidx(char *idxname);
	 */
/* prelim graphics */
#ifdef GRAPHICS
	MAKE_INTRINSIC(".gopen", slash_gopen, INT_TYPE, 0),
	MAKE_INTRINSIC(".gclose", slash_gclose, INT_TYPE, 0),
	MAKE_INTRINSIC(".ginit", slash_ginit, INT_TYPE, 0),
	MAKE_INTRINSIC(".gsize", slash_gsize, INT_TYPE, 0),
	MAKE_INTRINSIC(".gscale", slash_gscale, INT_TYPE, 0),
	MAKE_INTRINSIC(".grotate", slash_grotate, INT_TYPE, 0),
	MAKE_INTRINSIC(".gtranslate", slash_gtranslate, INT_TYPE, 0),
	MAKE_INTRINSIC(".gerase", slash_gerase, INT_TYPE, 0),
	MAKE_INTRINSIC(".gshow", slash_gshow, INT_TYPE, 0),
	MAKE_INTRINSIC(".gmove", slash_gmove, INT_TYPE, 0),
	MAKE_INTRINSIC(".gline", slash_gline, INT_TYPE, 0),
	MAKE_INTRINSIC(".gplot", slash_gplot, INT_TYPE, 0),
	MAKE_INTRINSIC(".mapxy", slash_mapxy, VOID_TYPE, 0),
#endif
	SLANG_END_TABLE
};

/* Initialize
*/
int
sl_init()
{
	if(!SLang_add_table(sl_intrinsics, "Slash"))
		return(0);
	/*strncpy(sl_path, SLPATH, 2048);*/
	SLang_Traceback = 1;
	SLdefine_for_ifdef("SLASH");

	return(1);
}

/* sldoc entries for interactive-only commands */

/*I::
 *@ : command [options ...]
 * Execute a system @command@ in a subshell. Note that the
 * syntax does not use S-Lang conventions; everything after
 * the `@:@' is passed to the shell. Returns nothing, a message
 * is printed on standard error if @command@ returned a non-zero
 * exit status.
*/
/*I:?
 *@ ? [name ...]
 * Invoke the @vamps@ online help system. This command can
 * only be used when running interactive.
 *
 * If a single `@?@' is entered at the prompt, a list of
 * @names@, typically @vamps@ command or variable names, for
 * which online documentation is present will be printed on
 * the screen.
 *
 * If one or more @name@(s) are given after the `@?@', the
 * available documentation text for those @name@(s) is printed
 * on the screen.
 *
 * If there is more than one screenfull of text, the @cursor@
 * and @PgDn/PgUp@ keys will cause the text to scroll; pressing
 * `@q@' will stop text display and go back to the command
 * prompt.
*/

#endif
