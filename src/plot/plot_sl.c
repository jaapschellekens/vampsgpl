/* $Header: /home/schj/src/vamps_0.99g/src/plot/plot_sl.c,v 1.2 1999/01/06 12:13:01 schj Alpha $ 
 */
/*  $RCSfile: plot_sl.c,v $
 *  $Author: schj $
 *  $Date: 1999/01/06 12:13:01 $
 */

static char RCSid[] =
"$Id: plot_sl.c,v 1.2 1999/01/06 12:13:01 schj Alpha $";

#ifdef HAVE_CONFIG_H
#include "vconfig.h"
#endif

#ifdef HAVE_LIBSLANG
#ifndef FLOAT_TYPE
#define FLOAT_TYPE 3
#endif

#include <stdio.h>
#include <string.h>
#include "slang.h"
#include "plot.h"

int plot_version = 1; /* devide by 10 to get version info */

FILE	*curoutf = NULL;

/* Output formats - if 1 is use the format depends on the library used
 * for linking */
#define GNU_PLOT 1
#define SL_COMMANDS 2

int pl_type = GNU_PLOT; /* GNU plotutils */

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




int sl_plflush();
int pl_init_f();
int pl_init_i();
int sl_fpoint();
int sl_fpointrel();
int sl_fmove();
int sl_fmoverel();
int sl_fcont();
int sl_fcontrel();
int sl_fspace();
int sl_fline();
int sl_flinerel();
int sl_flinewidth();
int sl_fbox();
int sl_fboxrel();
int sl_erase();
int sl_openpl();
int sl_closepl();
int sl_flushpl();
int sl_endpoly();
int sl_outfile();

/* Stuff from R. Vennekers graph 0.9 */
int sl_draw_frame();

SLang_Name_Type PLOT[] =
{
	MAKE_INTRINSIC(".pl_init_f", pl_init_f, INT_TYPE, 0),
	MAKE_INTRINSIC(".pl_init_i", pl_init_i, INT_TYPE, 0),
	MAKE_INTRINSIC(".pl_outfile", sl_outfile, INT_TYPE, 0),
	MAKE_VARIABLE(".pl_version", &plot_version, INT_TYPE, 0),
	MAKE_VARIABLE(".pl_type", &pl_type, INT_TYPE, 0),
	MAKE_VARIABLE(".pl_suffix",
			&_libplot_suffix, STRING_TYPE, 1),
	MAKE_VARIABLE(".pl_have_font_metrics",
			&_libplot_have_font_metrics, INT_TYPE, 1),
	MAKE_VARIABLE(".pl_have_vector_fonts",
			&_libplot_have_vector_fonts, INT_TYPE, 1),
	MAKE_VARIABLE(".pl_have_ps_fonts",
			&_libplot_have_ps_fonts, INT_TYPE, 1),
	MAKE_VARIABLE(".pl_have_wide_lines",
			&_libplot_have_wide_lines, INT_TYPE, 1),
	MAKE_VARIABLE(".pl_max_unflld_plln_lngth",
			&_libplot_max_unfilled_polyline_length, INT_TYPE, 0),
	MAKE_VARIABLE(".pl_output_is_ascii",
			&_libplot_output_is_ascii, INT_TYPE, 0),
	MAKE_VARIABLE(".pl_output_high_byte_first",
			&_libplot_output_high_byte_first, INT_TYPE, 0),
	MAKE_INTRINSIC(".pl_erase", sl_erase, INT_TYPE, 0),
	MAKE_INTRINSIC(".pl_flush", sl_plflush, INT_TYPE, 0),
	MAKE_INTRINSIC(".pl_openpl", sl_openpl, INT_TYPE, 0),
	MAKE_INTRINSIC(".pl_closepl", sl_closepl, INT_TYPE, 0),
	MAKE_INTRINSIC(".pl_flushpl", sl_flushpl, INT_TYPE, 0),
	MAKE_INTRINSIC(".pl_endpoly", sl_endpoly, INT_TYPE, 0),
	MAKE_INTRINSIC(".pl_draw_frame", sl_draw_frame, INT_TYPE, 0),
	SLANG_END_TABLE
};

SLang_Name_Type PLOT_F[] = {
	MAKE_INTRINSIC(".pl_fpoint", sl_fpoint, INT_TYPE, 0),
	MAKE_INTRINSIC(".pl_fpointrel", sl_fpointrel, INT_TYPE, 0),
	MAKE_INTRINSIC(".pl_fmove", sl_fmove, INT_TYPE, 0),
	MAKE_INTRINSIC(".pl_fmoverel", sl_fmoverel, INT_TYPE, 0),
	MAKE_INTRINSIC(".pl_fcont", sl_fcont, INT_TYPE, 0),
	MAKE_INTRINSIC(".pl_fcontrel", sl_fcontrel, INT_TYPE, 0),
	MAKE_INTRINSIC(".pl_fspace", sl_fspace, INT_TYPE, 0),
	MAKE_INTRINSIC(".pl_fline", sl_fline, INT_TYPE, 0),
	MAKE_INTRINSIC(".pl_flinerel", sl_flinerel, INT_TYPE, 0),
	MAKE_INTRINSIC(".pl_flinewidth", sl_flinewidth, INT_TYPE, 0),
	MAKE_INTRINSIC(".pl_fbox", sl_fbox, INT_TYPE, 0),
	MAKE_INTRINSIC(".pl_fboxrel", sl_fboxrel, INT_TYPE, 0),
	SLANG_END_TABLE
};
SLang_Name_Type PLOT_I[] = {
	SLANG_END_TABLE
};


int pl_init_f()
{
	if(!SLang_add_table(PLOT_F, "PLOT_F")) {
		fprintf(stderr, "Unable to initialize PLOT_F S-Lang table.\n");
		exit(-1);
	}
	SLdefine_for_ifdef("PLOT_F");

	return 0;
}

int pl_init_i()
{
	if(!SLang_add_table(PLOT_I, "PLOT_I")) {
		fprintf(stderr, "Unable to initialize PLOT_I S-Lang table.\n");
		exit(-1);
	}
	SLdefine_for_ifdef("PLOT_I");

	return 0;
}

/*C:sl_plot_init
 * @void sl_plot_init (void)
 *
 * Initializes the plot S-Lang basic intrinsics. Use pl_init_f
 * or pl_init_i to load floating point and integer functions.
 */
void
sl_plot_init (void)
{
	if(!SLang_add_table(PLOT, "PLOT")) {
		fprintf(stderr, "Unable to initialize PLOT S-Lang table.\n");
		exit(-1);
	}
	/* Turn on debugging */
	SLang_Traceback = 1;
	SLdefine_for_ifdef("PLOT");
}

/*--------------------------------------------------------------------------*/

int sl_erase()
{
	switch (pl_type) {
		case SL_COMMANDS:
			printf("() = pl_erase();\n");
			break;
		default:
			return erase();
			break;
	}

	return 0;
}

int sl_openpl()
{
	switch (pl_type) {
		case SL_COMMANDS:
			printf("() = pl_openpl();\n");
			break;
		default:
			return openpl();
			break;
	}

	return 0;
}

int sl_closepl()
{
	switch (pl_type) {
		case SL_COMMANDS:
			printf("() = pl_closepl();\n");
			break;
		default:
			return closepl();
			break;
	}

	return 0;
}

int sl_flushpl()
{
	switch (pl_type) {
		case SL_COMMANDS:
			printf("() = pl_flushpl();\n");
			break;
		default:
			return flushpl();
			break;
	}

	return 0;
}

int sl_endpoly()
{
	switch (pl_type) {
		case SL_COMMANDS:
			printf("() = pl_endpoly();\n");
			break;
		default:
			return endpoly();
			break;
	}

	return 0;
}

int sl_fpoint()
{
	double  x,y;
	int i1,i2;

	if (SLang_pop_float(&y,&i1,&i2)) return 0;
	if (SLang_pop_float(&x,&i1,&i2)) return 0;

	switch (pl_type) {
		case SL_COMMANDS:
			printf("() = pl_fpoint(%f, %f);\n",x,y);
			break;
		default:
			return fpoint(x, y);
			break;
	}

	return 0;
}

int sl_fpointrel()
{
	double  x,y;
	int i1,i2;

	if (SLang_pop_float(&y,&i1,&i2)) return 0;
	if (SLang_pop_float(&x,&i1,&i2)) return 0;

	switch (pl_type) {
		case SL_COMMANDS:
			printf("() = pl_fpointrel(%f, %f);\n",x,y);
			break;
		default:
			return fpointrel(x, y);
			break;
	}

	return 0;
}


int sl_fmove()
{
	double  x,y;
	int i1,i2;

	if (SLang_pop_float(&y,&i1,&i2)) return 0;
	if (SLang_pop_float(&x,&i1,&i2)) return 0;

	switch (pl_type) {
		case SL_COMMANDS:
			printf("() = pl_fmove(%f, %f);\n",x,y);
			break;
		default:
			return fmove(x, y);
			break;
	}
	return 0;
}

int sl_fmoverel()
{
	double  x,y;
	int i1,i2;

	if (SLang_pop_float(&y,&i1,&i2)) return 0;
	if (SLang_pop_float(&x,&i1,&i2)) return 0;

	switch (pl_type) {
		case SL_COMMANDS:
			printf("() = pl_fmoverel(%f, %f);\n",x,y);
			break;
		default:
			return fmoverel(x, y);
			break;
	}

	return 0;
}

int sl_fcont()
{
	double  x,y;
	int i1,i2;

	if (SLang_pop_float(&y,&i1,&i2)) return 0;
	if (SLang_pop_float(&x,&i1,&i2)) return 0;

	switch (pl_type) {
		case SL_COMMANDS:
			printf("() = pl_fcont(%f, %f);\n",x,y);
			break;
		default:
			return fcont(x, y);
			break;
	}

	return 0;
}

int sl_fcontrel()
{
	double  x,y;
	int i1,i2;

	if (SLang_pop_float(&y,&i1,&i2)) return 0;
	if (SLang_pop_float(&x,&i1,&i2)) return 0;

	switch (pl_type) {
		case SL_COMMANDS:
			printf("() = pl_fcontrel(%f, %f);\n",x,y);
			break;
		default:
			return fcontrel(x, y);
			break;
	}

	return 0;
}

int sl_fspace()
{
	double  x0,y1,y0,x1;
	int i1,i2;

	if (SLang_pop_float(&y1,&i1,&i2)) return 0;
	if (SLang_pop_float(&x1,&i1,&i2)) return 0;
	if (SLang_pop_float(&y0,&i1,&i2)) return 0;
	if (SLang_pop_float(&x0,&i1,&i2)) return 0;

	switch(pl_type){
		case SL_COMMANDS:
			printf("() = pl_fspace(%f,%f,%f,%f);\n",x0,y0,x1,y1);
			break;
		default:
			return fspace(x0, y0, x1, y1);
			break;
	}

	return 0;
}

int sl_fline()
{
	double  x0,y1,y0,x1;
	int i1,i2;

	if (SLang_pop_float(&y1,&i1,&i2)) return 0;
	if (SLang_pop_float(&x1,&i1,&i2)) return 0;
	if (SLang_pop_float(&y0,&i1,&i2)) return 0;
	if (SLang_pop_float(&x0,&i1,&i2)) return 0;

	switch(pl_type){
		case SL_COMMANDS:
			printf("() = pl_fline(%f,%f,%f,%f);\n",x0,y0,x1,y1);
			break;
		default:
			return fline(x0, y0, x1, y1);
			break;
	}

	return 0;
}

int sl_flinerel()
{
	double  x0,y1,y0,x1;
	int i1,i2;

	if (SLang_pop_float(&y1,&i1,&i2)) return 0;
	if (SLang_pop_float(&x1,&i1,&i2)) return 0;
	if (SLang_pop_float(&y0,&i1,&i2)) return 0;
	if (SLang_pop_float(&x0,&i1,&i2)) return 0;

	switch(pl_type){
		case SL_COMMANDS:
			printf("() = pl_flinerel(%f,%f,%f,%f);\n",x0,y0,x1,y1);
			break;
		default:
			return flinerel(x0, y0, x1, y1);
			break;
	}

	return 0;
}

int sl_flinewidth()
{
	double  size;
	int i1,i2;

	if (SLang_pop_float(&size,&i1,&i2)) return 0;

	switch(pl_type){
		case SL_COMMANDS:
			printf("() = pl_flinewodth(%f);\n",size);
			break;
		default:
			return flinewidth(size);
			break;
	}

	return 0;
}

int sl_fbox()
{
	double  x0,y1,y0,x1;
	int i1,i2;

	if (SLang_pop_float(&y1,&i1,&i2)) return 0;
	if (SLang_pop_float(&x1,&i1,&i2)) return 0;
	if (SLang_pop_float(&y0,&i1,&i2)) return 0;
	if (SLang_pop_float(&x0,&i1,&i2)) return 0;

	switch(pl_type){
		case SL_COMMANDS:
			printf("() = pl_fbox(%f,%f,%f,%f);\n",x0,y0,x1,y1);
			break;
		default:
			return fbox(x0, y0, x1, y1);
			break;
	}

	return fbox(x0, y0, x1, y1);
}

int sl_fboxrel()
{
	double  x0,y1,y0,x1;
	int i1,i2;

	if (SLang_pop_float(&y1,&i1,&i2)) return 0;
	if (SLang_pop_float(&x1,&i1,&i2)) return 0;
	if (SLang_pop_float(&y0,&i1,&i2)) return 0;
	if (SLang_pop_float(&x0,&i1,&i2)) return 0;

	switch(pl_type){
		case SL_COMMANDS:
			printf("() = pl_boxrel(%f,%f,%f,%f);\n",x0,y0,x1,y1);
			break;
		default:
			return fboxrel(x0, y0, x1, y1);
			break;
	}

	return 0;
}

int
sl_outfile()
{
	char *outfname;
	FILE *outstr;

	if (SLpop_string(&outfname)) return -1;

#ifdef HAVE_POPEN	
	if (strstr(outfname,"|") != NULL){
		outfname++;
		outstr = popen(outfname,"w");
		curoutf = outstr;
		outfile(outstr);
		return 0;
	}
#endif

	if ((outstr = fopen(outfname,"w")) == NULL)
		return;

	(void)outfile(outstr);
	curoutf = outstr;
	return 0;
}

int 
sl_plflush()
{
	return fflush(curoutf);
}
/*
int
draw_frame(grid, ticklen,
	    x_min, x_max, x_spc, y_min, y_max, y_spc,
	    x_title, y_title, p_title,
	    log_axis, nolabels)
int	grid, ticklen;
double	x_min, x_max, x_spc, y_min, y_max, y_spc;
char	*x_title, *y_title, *p_title;
int	log_axis, nolabels;
*/
int sl_draw_frame()
{
	int i1,i2;
	int	grid, ticklen;
	double	x_min, x_max, x_spc, y_min, y_max, y_spc;
	char	*x_title, *y_title, *p_title;
	int	log_axis, nolabels;

	if (SLang_pop_integer(&nolabels)) return -1;
	if (SLang_pop_integer(&log_axis)) return -1;
	if (SLpop_string(&p_title)) return -1;
	if (SLpop_string(&y_title)) return -1;
	if (SLpop_string(&x_title)) return -1;
	if (SLang_pop_float(&y_spc,&i1,&i2)) return -1;
	if (SLang_pop_float(&y_max,&i1,&i2)) return -1;
	if (SLang_pop_float(&y_min,&i1,&i2)) return -1;
	if (SLang_pop_float(&x_spc,&i1,&i2)) return -1;
	if (SLang_pop_float(&x_max,&i1,&i2)) return -1;
	if (SLang_pop_float(&x_min,&i1,&i2)) return -1;
	if (SLang_pop_integer(&ticklen)) return -1;
	if (SLang_pop_integer(&grid)) return -1;

	return draw_frame(grid, ticklen,
			x_min, x_max, x_spc, y_min, y_max, y_spc,
			x_title, y_title, p_title,
			log_axis, nolabels);
}
#endif
