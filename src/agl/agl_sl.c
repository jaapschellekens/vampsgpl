/* $Header: /home/schj/src/vamps_0.99g/src/agl/agl_sl.c,v 1.2 1999/01/06 12:13:01 schj Alpha $ 
 */
/* 
 *  $RCSfile: agl_sl.c,v $
 *  $Author: schj $
 *  $Date: 1999/01/06 12:13:01 $
 */

#include "vconfig.h"

#ifdef HAVE_LIBAGL
#ifdef HAVE_LIBSLANG

#define ANSI
static char RCSid[] =
"$Id: agl_sl.c,v 1.2 1999/01/06 12:13:01 schj Alpha $";

#define FLOAT_TYPE 3

#include <stdio.h>
#include "slang.h"
#include "agl.h"


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

void SL_AG_GPLL();
void SL_AG_GPLM();
void SL_AG_FILL();
void SL_AG_HIST();
int SL_AG_VDEF();
void SL_AG_AXES();
void SL_AG_SSET();
void SL_AG_VSEL();
void SL_AG_GTXT();
void SL_AG_WDEF();
/* This table provides some 'general' utilities */

SLang_Name_Type AGL[] =
{
	MAKE_INTRINSIC(".AG_VDEF", SL_AG_VDEF, INT_TYPE, 0),
	MAKE_INTRINSIC(".AG_AXES", SL_AG_AXES, VOID_TYPE, 0),
	MAKE_INTRINSIC(".AG_GPLL", SL_AG_GPLL, VOID_TYPE, 0),
	MAKE_INTRINSIC(".AG_GPLM", SL_AG_GPLM, VOID_TYPE, 0),
	/*C:AG_GPLL
	 *@ void AG_GPL(Matrix,Int xcol,Int ycol);
	 */ 
	MAKE_INTRINSIC(".AG_FILL", SL_AG_FILL, VOID_TYPE, 0),
	/*C:AG_FILL
	 *@ AG_FILL(Matrix,double space, double angle, String sset)
	 */ 
	MAKE_INTRINSIC(".AG_HIST", SL_AG_HIST, VOID_TYPE, 0),
	/*C:AG_HIST
	 *@ AG_HIST(Matrix)
	 */ 
	MAKE_INTRINSIC(".AG_SSET", SL_AG_SSET, VOID_TYPE, 0),
	MAKE_INTRINSIC(".AG_CLS", AG_CLS, VOID_TYPE, 0),
	MAKE_INTRINSIC(".AG_VKIL", AG_VKIL, VOID_TYPE, 0),
	MAKE_INTRINSIC(".AG_VSEL", AG_VSEL, VOID_TYPE, 0),
	MAKE_INTRINSIC(".AG_VERS", AG_VERS, VOID_TYPE, 0),
	MAKE_INTRINSIC(".AG_VUPD", AG_VUPD, VOID_TYPE, 0),
	MAKE_INTRINSIC(".AG_MCLS", AG_MCLS, VOID_TYPE, 0),
	MAKE_INTRINSIC(".AG_MRES", AG_MRES, VOID_TYPE, 0),
	MAKE_INTRINSIC(".AG_MSUS", AG_MSUS, VOID_TYPE, 0),
	MAKE_INTRINSIC(".AG_WDEF", SL_AG_WDEF, VOID_TYPE, 0),
	MAKE_INTRINSIC(".AG_DRIV", AG_DRIV, VOID_TYPE, 0),
	MAKE_INTRINSIC(".AG_GTXT", SL_AG_GTXT, VOID_TYPE, 0),
	MAKE_INTRINSIC(".AG_IDN", AG_IDN, STRING_TYPE, 0),
	SLANG_END_TABLE
};

void   
SL_AG_WDEF()
{
	double x1,x2,y1,y2;
	int i1,i2;

	if (SLang_pop_float(&y2,&i1,&i2)) return;
	if (SLang_pop_float(&y1,&i1,&i2)) return;
	if (SLang_pop_float(&x2,&i1,&i2)) return;
	if (SLang_pop_float(&x1,&i1,&i2)) return;
	AG_WDEF(x1,x2,y1,y2);
}

void SL_AG_GPLL()
{
	SLuser_Object_Type *uob;
	SLArray_Type *at;
	int i1,i = 0,j = 0,ycol,xcol;
	float64 *jj;
	float *x,*y;

	if (SLang_pop_integer(&ycol)) return;
	if (SLang_pop_integer(&xcol)) return;
	if (NULL == (uob = (SLuser_Object_Type *)SLang_pop_array(&i1))) return;
	at = (SLArray_Type *) uob->obj;
	jj = at->buf.f_ptr;

	x = (float *)SLMALLOC(at->x * sizeof(float));
	y = (float *)SLMALLOC(at->x * sizeof(float));
	for (i=0;i<at->x * at->y;i++){
		if (i % at->y == xcol){
			x[j] = jj[i];
		}else if (i % at->y == ycol){
			y[j] = jj[i];
		}
		if (i % at->y == at->y -1)
			j++;
	}
	AG_GPLL(x,y,at->x);
	SLang_free_user_object(uob);
	SLFREE(x);
	SLFREE(y);
}

void SL_AG_HIST()
{
	SLuser_Object_Type *uob;
	SLArray_Type *at;
	int i1,i = 0,j = 0,mode,join;
	float64 *jj;
	float *x,*y;
	int xcol = 0, ycol = 1;

	if (SLang_pop_integer(&join)) return;
	if (SLang_pop_integer(&mode)) return;
	if (NULL == (uob = (SLuser_Object_Type *)SLang_pop_array(&i1))) return;
	at = (SLArray_Type *) uob->obj;
	jj = at->buf.f_ptr;

	x = (float *)SLMALLOC(at->x * sizeof(float));
	y = (float *)SLMALLOC(at->x * sizeof(float));
	for (i=0;i<at->x * at->y;i++){
		if (i % at->y == xcol){
			x[j] = jj[i];
		}else if (i % at->y == ycol){
			y[j] = jj[i];
		}
		if (i % at->y == at->y -1)
			j++;
	}
	AG_HIST(x,y,at->x,mode,join);
	SLang_free_user_object(uob);
	SLFREE(x);
	SLFREE(y);
}

void SL_AG_FILL()
{
	SLuser_Object_Type *uob;
	SLArray_Type *at;
	int i1,i2,i = 0,j = 0;
	double space,angle;
	float64 *jj;
	float *x,*y;
	int xcol = 0, ycol = 1;
	char *sset;
	
	if (SLpop_string(&sset)) return;
	if (SLang_pop_float(&angle,&i1,&i2)) return;
	if (SLang_pop_float(&space,&i1,&i2)) return;
	i1 = 0;
	if (NULL == (uob = (SLuser_Object_Type *)SLang_pop_array(&i1))) return;
	at = (SLArray_Type *) uob->obj;
	jj = at->buf.f_ptr;

	x = (float *)SLMALLOC(at->x * sizeof(float));
	y = (float *)SLMALLOC(at->x * sizeof(float));
	for (i=0;i<at->x * at->y;i++){
		if (i % at->y == xcol){
			x[j] = jj[i];
		}else if (i % at->y == ycol){
			y[j] = jj[i];
		}
		if (i % at->y == at->y -1)
			j++;
	}
	AG_FILL(x,y,at->x,space,angle,sset);
	SLang_free_user_object(uob);
	SLFREE(x);
	SLFREE(y);
}


void SL_AG_GTXT ()
{
	char *txt;
	double xc,yc;
	int i1,i2;
	int centre;

	if (SLang_pop_integer(&centre)) return;
	if (SLpop_string(&txt)) return;
	if (SLang_pop_float(&yc,&i1,&i2)) return;
	if (SLang_pop_float(&xc,&i1,&i2)) return;

	AG_GTXT(xc,yc,txt,centre);
}

void SL_AG_SSET ()
{
	char *command;

	if (SLpop_string(&command)) return;

	AG_SSET(command);
}

void SL_AG_VSEL ()
{
	int i1;

	if (SLang_pop_integer(&i1)) return;

	AG_VSEL(i1);
}

int SL_AG_VDEF ()
{
	char *device;
	double xa,xb,ya,yb,xlim,ylim;
	int i1,i2;

	if (SLang_pop_float(&ylim,&i1,&i2)) return -1;
	if (SLang_pop_float(&xlim,&i1,&i2)) return -1;
	if (SLang_pop_float(&yb,&i1,&i2)) return -1;
	if (SLang_pop_float(&ya,&i1,&i2)) return -1;
	if (SLang_pop_float(&xb,&i1,&i2)) return -1;
	if (SLang_pop_float(&xa,&i1,&i2)) return -1;
	if (SLpop_string(&device)) return -1;

	return AG_VDEF(device,xa,xb,ya,yb,xlim,ylim);
}

void SL_AG_AXES()
{
	double xmin,xmax,ymin,ymax;
	char *command;
	int i1,i2;

	if (SLpop_string(&command)) return;
	if (SLang_pop_float(&ymax,&i1,&i2)) return;
	if (SLang_pop_float(&ymin,&i1,&i2)) return;
	if (SLang_pop_float(&xmax,&i1,&i2)) return;
	if (SLang_pop_float(&xmin,&i1,&i2)) return;

	AG_AXES(xmin,xmax,ymin,ymax,command);
}

/*C:sl_AGL_init
 * @void sl_AGL_init (void)
 *
 * Initializes the AGL S-Lang intrinsics
 */
void
sl_AGL_init (void)
{
	if(!SLang_add_table(AGL, "AGL")) {
		fprintf(stderr, "Unable to initialize AGL S-Lang table.\n");
		exit(-1);
	}
	/* Turn on debugging */
	SLang_Traceback = 1;
	SLdefine_for_ifdef("AGL");
}

void SL_AG_GPLM()
{
	SLuser_Object_Type *uob;
	SLArray_Type *at;
	int mark,i1,i = 0,j = 0,ycol,xcol;
	float64 *jj;
	float *x,*y;

	if (SLang_pop_integer(&mark)) return;
	if (SLang_pop_integer(&ycol)) return;
	if (SLang_pop_integer(&xcol)) return;
	if (NULL == (uob = (SLuser_Object_Type *)SLang_pop_array(&i1))) return;
	at = (SLArray_Type *) uob->obj;
	jj = at->buf.f_ptr;

	x = (float *)SLMALLOC(at->x * sizeof(float));
	y = (float *)SLMALLOC(at->x * sizeof(float));
	for (i=0;i<at->x * at->y;i++){
		if (i % at->y == xcol){
			x[j] = jj[i];
		}else if (i % at->y == ycol){
			y[j] = jj[i];
		}
		if (i % at->y == at->y -1)
			j++;
	}
	AG_GPLM(x,y,at->x,mark);
	SLang_free_user_object(uob);
	SLFREE(x);
	SLFREE(y);
}

#endif
#endif
