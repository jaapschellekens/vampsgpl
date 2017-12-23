#ifndef SWATSOIL_H
#define SWATSOIL_H
#include "soils.h"
#include "vamps.h"

/* Some macro's to replace expensive procedure calls in 
   vamps' innner loops */
#define MKKGEOM(A) (0.5 * (k[A] + k[A-1]))

#define MAXTBL 7

#define T2KTAB 0
#define H2DMCTAB 1
#define T2HTAB 2
#define H2TTAB 3
#define H2KTAB 4
#define H2UTAB 5
#define H2DKDPTAB 6

#define THETA2K 0
#define DMCNODE 1
#define PRHNODE 2
#define THENODE 3

/* Equation solve methods in headcalc.c, set in solvemet */
#define TRI 0
#define BAN 1
#define GEN 2

extern int headc_method; /* head calculation method. SWATR or TOPOG */
extern int th_ckcnv();
extern int mb_ckcnv();
extern int (* ckcnv)();
extern int mbck;
extern double mbalerr;
extern int noit;
extern int speed;
extern int mktab;
extern int outskip;
extern double smddepth;
extern int soilverb;
extern double avgtheta;
extern double SMD; /* soil moisture deficit in cm */
extern double fieldcap; /* head in cm at field capacity */
extern double rootts;
extern int *allowdrain;
extern double *rootfrac;
extern double tm_mult;
extern int layers;			/* number of soil layers */
extern int initprof;	/* initial profile: theta, head or calculated */
extern node_t *node;
extern soilparmt *sp;
extern int tablesize; /* see filltab.c */
extern double cofred;		/* soil evap reduce stuff */

/* These are set in headcalc.c */
extern int   *error;
extern int   *itter;
extern int solvemet;

extern double qdrtot;
extern double masbal,cumtop,cumbot;
extern double cumeva,cumtra,cumprec,cumintc,rootextract;
extern double _cumeva,_cumtra,_cumprec,_cumintc;
extern int swredu;

/* See src91/headcalc.f for source */
/* These are alle the new gloabal vars needed */
extern  int	fltsat;
extern  int	numbit;
extern	int	maxitrlo;
extern  long int nrcalls; /* number of calls to the timestep proc. */
extern  long int nr_itter; /* number of itterations in headcalc. */
extern  long int nr_tri;
extern  long int nr_band;
extern  long int nr_hitt;
extern  long int nr_sat;
extern	int	maxitrhi;
extern	double	volsat;
extern	double	volm1;
extern	double	volact;
extern	double	cqbot;
extern  double  cqbotts;
extern	double	cqdra;
extern  double  cumdra;
extern	int	ftoph;
extern	int	daynr;
/*extern	int	daysta;*/
/*extern	int	dayend;*/
/*extern	int	year;*/
/*extern	int	yearen;*/
/*extern	int	ndyear;*/
extern 	double	*gwl; /* length 2*/
extern 	double	*basegw; /* length 2*/
extern 	double	*gw;	/* length a year 366, must be same as input */
extern	double	*q; /* length layers +1 */
extern	double	*inq; /* length layers +1 */
extern	double	aqave,aqamp,aqomeg,aqtamx,rimlay,cofqha,cofqhb;
extern double dsoilp; /* total depth of soil profile */

extern double t, tm1;			/* Time and time one step back */
extern int fllast;
extern int maxtit, maxitr;		/* Number of iterations */
extern int lbc;				/* Lower bottom condition */
extern double dtm1;			/* dt one step back */
extern double prec;
extern double ptra;
extern double peva;
extern double intc;
extern double hatmd,hatm; 		/* hatm pressure head atmosphere */
extern double dhead;
extern double dtmax;
extern double dtmin;
extern double thetol;
extern double mdhead;
extern int flendd;
extern double *k;			/* Conductivity (length is layers +1!!) */
extern double *depth;			/* Depth of each layer (disnod in swatr) */
extern double *h;			/* head in each layer */
extern double *kgeom;			/* geo gemm van k */
extern double *diffmoist;		/* differential moisture cap. */
extern double *theta;			/* Theta at t */
extern double *howsat;			/* How saturated (scale of 0 to 1) */
extern double *thetm1;			/* Theta one step back */
extern double dt;			/* Timestep */
extern double *dz;			/* Thickness of layers size of compartement */
extern double *dzf;			/* delta z forward (z[i] - z[i-1]) */
extern double *dzc;			/* delta z central ((z[i+1]-z[i-1])/2)*/
extern double *z;			/* Depth */
extern double *hm1;			/* Head one timestep back */
extern double qtop;			/* Discharge at top */
extern double *qrot;			/* Root extraction rate, determined in rootex.f */
extern double pond;			/* Ponding amount at top */
extern double qsurf;			
extern double runots;	
extern double surface_runoff;
extern double pondmx;			/* Max Ponding amount at top */
extern double ksurf;
extern double **qdra;			/* 4x layers matrix */
extern double qbot;			/* bottom q?? */
extern double *layer;			/* soil layer, can this go ?? */
extern double *mqdra;
extern int *kvspsim;			/* Method to use for for determining k VS psi each layer */
/* stuff for balance and rootex */
/*extern	double	osmota,osmotb;
extern	double	*cml;*/

extern void	integral (int i);
extern void	watcon (void);

/* Rootex vars*/
extern int swsink;
extern int swhypr;
extern int swupfu;
extern double cofsza;
extern double cofszb;
extern double zronam;
extern double drootz;
extern double hlim1;
extern double hlim2u;
extern double hlim2l;
extern double hlim3h;
extern double hlim3l;
extern double hlim3;
extern double hlim4;
extern double reva;

/* Drainage stuff */
extern int dodrain;
extern void drainage (int method);
extern double slope;

/* stuff from misc_p.c*/
extern void dethowsat(void);
extern double detavgtheta(int layr);
extern double smd(double drz, double fieldh);

extern void smooth(int inter,int what);
extern void fillmat(double **mat,double a[],double b[],double c[]);
extern void bandec (double **a, int  n, double **a1, int indx[]);
extern void banks(double **a,int n, double **a1,int indx[], double b[]);

extern double bocotop (double *kgeom0, int *ftoph);
/* These at the soil functions for each method */
extern double  h2dmc_0 (int nr, double head);  
extern double  t2k_0 (int nr, double wcon);  
extern double  t2h_0 (int nr, double wcon, double depth);  
extern double  h2t_0 (int nr, double head);
extern double  h2u_0 (int nr, double head);
extern double  h2k_0 (int nr, double head);
extern double  h2dkdp_0 (int nr, double head);
extern double  h2dmc_1 (int nr, double head);  
extern double  t2k_1 (int nr, double wcon);  
extern double  t2h_1 (int nr, double wcon, double depth);  
extern double  h2t_1 (int nr, double head);
extern double  h2u_1 (int nr, double head);
extern double  h2k_1 (int nr, double head);
extern double  h2dkdp_1 (int nr, double head);
extern double  h2dmc_2 (int nr, double head);  
extern double  t2k_2 (int nr, double wcon);  
extern double  t2h_2 (int nr, double wcon, double depth);  
extern double  h2t_2 (int nr, double head);
extern double  h2u_2 (int nr, double head);
extern double  h2k_2 (int nr, double head);
extern double  h2dkdp_2 (int nr, double head);
extern double  h2dmc_3 (int nr, double head);  
extern double  t2k_3 (int nr, double wcon);  
extern double  t2h_3 (int nr, double wcon, double depth);  
extern double  h2t_3 (int nr, double head);
extern double  h2u_3 (int nr, double head);
extern double  h2k_3 (int nr, double head);
extern double  h2dkdp_3 (int nr, double head);
extern void allocall ();
extern void initnode (int layer);
#endif /* SWATSOIL_H*/
