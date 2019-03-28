#ifndef VAMPS_H_
#define VAMPS_H_

#ifdef HAVE_CONFIG_H
#include "vconfig.h"
#endif

#ifdef HAVE_LIBSLANG
#include "slang.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include "ts.h"
#include "version.h"
#ifdef HAVE_LIBSLANG
#include "s_init.h"
#endif


/* The getdefname routine needs to know path sep char */
#if defined(__GO32__) || defined(__MSDOS__) || defined(__OS2__)
#ifndef DIRSEP
#define DIRSEP '\\'
#endif
#else
#ifndef DIRSEP
#define DIRSEP '/'
#endif
#endif

#define MAX_SEC_IN_TS 0 /* maximum seconds allowed per timestep before
			  a SIGALARM is set */
#define BUFJTE 4096

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif

#define INPUTERROR 2 /* used in exit() */
#define MISSVAL -99999.99  /* Missing value definition */

#ifdef HAVE_LIBSLANG
extern void init_s (char *vamps_sl); /* Initialize s-lang stuff */
int interpreter (int verb); /* interactive mode */
#endif

/* Some default file names */
#ifndef DEFNAME
#ifdef __FAT__  /* Name of the defaults file */
#define DEFNAME "vamps.rc"

#ifdef VAMPSLIBDIR
#ifndef VAMPSLIB
#define VAMPLIB VAMPSLIBDIR
#endif
#endif

#ifndef VAMPSLIB
#define VAMPSLIB "c:\\vamps\\lib"
#endif
#else
#define DEFNAME ".vampsrc"
#ifndef VAMPSLIB
#define VAMPSLIB "/usr/local/vamps/lib"
#endif
#endif
#endif

#ifdef __GO32__
#ifdef unix
#undef unix
#endif
#ifndef __FAT__
#define __FAT__
#endif
#endif

/* Types for logging */
#define MESG "mesg"
#define ERR "error"
#define WARN "warning"



typedef struct {
	int 	pre;	/* precipitation [cm] */
	int	rlh;	/* relative humidity [%] */
	int	hea;	/* head at bottom [cm] */
	int	rdp;	/* rooting depth [cm] */
	int	tem;	/* dry bulb temp [oC] */
	int	gwt;	/* ground-water table [cm] */
	int	inr;	/* interception	[cm] */
	int	trf;	/* troughfall	[cm] */
	int	stf;	/* stemflow	[cm] */
	int	pev;	/* potential evaporation [cm] */
	int	spe;	/* potential soil evaporation [cm] */
	int	ptr;	/* potential transpiration [cm] */
	int	qbo;	/* flow through bottom of profile [cm] */
	int	vol;	/* actual water content [cm] */
	int	avt;	/* average theta in profile */
	int	lai;	/* leaf area index */
	int	sca;	/* canopy storage [cm???] */
	int	ira;	/* incoming radiation [W/m2] */
	int	nra;	/* net radiation [W/m2] */
	int	ref;	/* reflected radiation [W/m2] */
	int	win;	/* windspeed [m/s] */
	int sur;	/* sunratio (n/N) */
	int sm0;    /* soil moisture input 0 */
	int sm1;    /* soil moisture input 1 */
	int sm2;    /* soil moisture input 2 */
} ID;

typedef struct {
	int id;
	char *name;
	char *description;
	int  defunit;
} datasettype;

typedef struct {
	XY *xy;
	datasettype *stype;
	char    name[1024]; /*name of the data set*/
	char    fname[1024]; /*name of file to dump data set*/
	double  ymax;
	double  xmax;
	double  xmin;
	double  ymin;
	double  ysum;
	double  xsum;
	int	points;
	int	tunit;
	int	vunit;
} dataset;

/* look up tables */
typedef struct {
	double	*x;
	double	*y;
	unsigned int	points;
	unsigned int	lasthit;
	double  accu;
	char	des[512];
} TBL;

extern int VampsPid;
extern char tmp_prefix[]; /* prefix for generating tmp files */
extern int version;
extern ID id;
extern double cpusec();
extern int exit_is_inter;
extern int iniinmem;
extern int Dcanop,Dsoil; /* use canopy or soil module */
extern int switchtoint; /* Switch to interactive mode */
extern  int dumptables;
extern  int tablesize;

/* some stuff from tmplist.c */
extern int addtotmplist(char *item);
extern void show_tmplist(void);
extern int save_tmplist(char *histfname);
extern void del_tmplist(int files);

extern  TBL *mktable (int points,char *des,int verbose);
extern double getval (TBL *tab,double xval);
extern double _getval (dataset *ds,double xval);
extern char xtraoutfn[];
extern char outfilename[];

/* date and time stuff */
extern	int steps; /* is days !! */
extern  double thiststep; /* Size of this timestep, see vamps.c */
extern  int thisstep;
extern char infilename[1024]; /*input file*/
extern dataset *data;	/* All input data*/
extern int	sets;	/*number of datasets*/
extern int	get_data (char *fname,char *name,int minpts);
extern int 	add_set (XY *xy,char *name,char *fname,int point, int vunit, int tunit);
extern void     mksetstats(int dset);
extern int 	del_set (int nr);
extern void 	del_all_sets (void);
extern int 	getsetbyname (char *s);
extern int	dumpset (int nr, FILE *stream);

/* Output functions */
extern void printar  (const char *des, double *ar, int pts);
extern void printfl  (const char *des, double fl);
extern void printint  (const char *des, int intje);
extern void printstr  (const char *des, const char *str);
extern void printcom  (const char *des);
extern void printsum  (FILE *thef);
extern void openout (void);
extern void xopenout (void);
extern void closeout  (void);
extern void xcloseout  (void);
extern void xtraout (int tstep);

extern	char outlist[64][128];
extern  char outfilename[1024];
extern  FILE *genoutfile;
extern	int outnr;
extern	int detnr;
extern	int header;

extern int startpos;
extern int verbose;
extern char *progname;
extern int getintercept  (int method);
extern int DoEvap  (int method);
extern int DoActEvap  (int method);
extern int to_outlist (char *s);
extern int inoutlist (char *s);
extern XY *resamp_a_to_b(XY *a, XY *b,int apoints,int bpoints);

/* log functions*/
extern void logit (char *tolog,char *from,char *TYPE);
extern int loggen;
extern void showit  (char *from,char *TYPE, char *toshow,int atverb, int whichverb);
extern void Perror  (char *Eprogname,int exitval,int prr,char *from,const char *descr,const char *xtr);
extern void startshow  (char *prname, char *name);
extern void endshow  (void);
extern void showprogress  (int step);
extern void initprogress  (void);
extern int showgraph;
extern char graphcommand[];
extern int noslang;
extern double starttime;
extern double endtime;
extern void presoil (void);
extern void postsoil (void);
extern void tstep_soil (int i, double e_t, double precipitation,
		double interception, double transpiration,
		double soilevaporation);
extern void precanop (void);
extern void postcanop (void);
extern void tstep_canop (int *i);
extern void utout (int *tstep);
extern void print (char *s);
extern void quit_vamps (void);
extern void prelim ();
extern void dorun ();
extern void dotail ();
extern char *cmline;
extern void disclaim (char * progname);
extern char vampslib[];
extern void inidump(void);
extern void outfiletrailer (void);
extern int  dumpini;
extern int Dsoil,Dcanop,doxtraout;
extern int setsig (void);
extern int unsetsig (void);
extern void onsig (int sig);
extern char logfilename[];
extern char *getdefname (char *defname);
extern int sreadline;
/* extern int fmttime; */
extern char deffname[];
#endif 	/*VAMPS_H_*/
