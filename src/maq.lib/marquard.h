#ifndef _MAQ_H_
#define _MAQ_H_

#include <stdlib.h>

#ifdef HAVE_CONFIG_H
#include "vconfig.h"
#endif
#ifdef HAVE_LIBSLANG
#include "slang.h"
#endif

#define MINM 2
#define true 1
#define false 0
#define PROGNAME "Marquardt"

extern  double diffac;
extern  int func_error;
extern  int maqverb;
extern	double *inp;
extern	double *out;
extern	double **jjinv;
extern	double *par;
extern  int *fixed;
extern	double *rv;
extern int  (*allfunc) (double *par, int mmm, int nnn);

extern	int Marquardt  (int m_, int n_,double *par_, double *g, double **v, double *inp, double *out);
extern int Funct  (int m, int n, double *par, double *rv);
extern int maqinit  (int *m, int *n);
extern void maqend  (int *m);
extern void prhead  (void);
extern void prtail  (void);

/* Stuff from memory.c */
extern double **m_matr  (int rows, int cols);
extern void m_free_matr  (double **m, int rows);
extern void *m_memory  (void *ptr, size_t size,const char *progname);

extern int maq_n;
extern int maq_m;
extern int maqrun();

#ifdef HAVE_LIBSLANG
extern SLang_Name_Type Maq[];
#endif
#endif /* _MAQ_H_ */
