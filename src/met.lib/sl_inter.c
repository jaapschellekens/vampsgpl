/* $Header: /home/schj/src/vamps_0.99g/src/met.lib/RCS/sl_inter.c,v 1.2 1999/01/06 12:13:01 schj Alpha $ 
 */
/* 
 *  $RCSfile: sl_inter.c,v $
 *  $Author: schj $
 *  $Date: 1999/01/06 12:13:01 $
 */

static char RCSid[] =
"$Id: sl_inter.c,v 1.2 1999/01/06 12:13:01 schj Alpha $";

#ifdef HAVE_CONFIG_H
#include "vconfig.h"
#endif

#ifdef HAVE_LIBSLANG
#define FLOAT_TYPE 3

#include <stdio.h>
#include "slang.h"
#include "met.h"

double sl_vslope(void);
void sl_gash(void);
double sl_e0(void);
void sl_eaes(void);
double sl_earo(void);
double sl_ra(void);
double sl_gamma(void);
double sl_lambda(void);
double sl_rnet_open(void);
double sl_rnet_open_nN(void);

/* This table provides some 'general' utilities */
SLang_Name_Type Met_Lib[] =
{
	MAKE_INTRINSIC(".rnet_open_nN", sl_rnet_open_nN, FLOAT_TYPE, 0),
	/*I:rnet_open_nN
	 *@ Float rnet_open_nN(Float Rs, Float nN, Float td,
	 *@		Float ea, Float Lambda)
	 *
	 * Calculated net radiation over open water from measured
	 * incoming solar radiadion, td [oC], ea[mbar] and nN. Albedo
	 * of open water = 0.06
	 * Rs should be W/m2
	 * Returns net radiation above open water in mm/day.
	 * This is the original function derived by penman.
	 */
	MAKE_INTRINSIC(".gash", sl_gash, VOID_TYPE, 0),
	/*I:gash
	 *@Interception, Stemflow, Throughfall gash (double ER,
	 *@		Float p, p_tr, S, P, LAI)
	 *
	 * If LAI <= 0 it is not used
	 */
	MAKE_INTRINSIC(".rnet_open", sl_rnet_open, FLOAT_TYPE, 0),
	/*I:rnet_open
	 *@ Float rnet_open(Float Rs, Float Rsout Float Rnet, Float Lambda)
	 *
	 * Calculated net radiation over open water from measured
	 * incoming and reflected solar radiadion and net long wave radiation
	 * At the site.
	 * Albedo of open water = 0.05. Rs, Rsout and Rnet should me W/m2
	 * Returns net radiation above open water in mm/day
	 */
	MAKE_INTRINSIC(".lambda", sl_lambda, FLOAT_TYPE, 0),
	/*I:lambda
	 *@Float lambda(Float Td)
	 *
	 * Calculates lambda [J/kg] from dry bulb temperature using:
	 *@ 	lambda = 4185.5 * (751.0 - (0.5655 * Td))
	 * Returns lambda @[J/kg]@ */
	MAKE_INTRINSIC(".mgamma", sl_gamma, FLOAT_TYPE, 0),
	/*I:mgamma
	 *@Float mgamma(Float td, Float lambda)
	 *
	 * Calculates the psychometric constant (@[mbarC]@) It assumes
	 * an air pressure of 998mb. Cp is estimated at 1005.0 J/kgK.
	 * Returns: psychrometric constant. See Waterloo 1994 for details.
	 */
	MAKE_INTRINSIC(".earo", sl_earo, FLOAT_TYPE, 0),
	/*I:earo
	 *@Float earo(Float ea, Float es, Float u)
	 *
	 * Determines Eearo (Aerodynamic evaporation or drying power of air).
	 * See calder1990284 for details.
	 * @ea@ and @es@ should be in mbar, u  mean daily windspeed at 2m in m/s
	 *
	 * Returns earodynamic term in mm/day */
	MAKE_INTRINSIC(".ra", sl_ra, FLOAT_TYPE, 0),
	/*I:ra
	 *@Float ra (Float z, Float z0, Float d, Float u)
	 *
	 * Calculates ra (aerodynamic resistance) according to:
	 *@	ra = [ln((z-d)/z0)]^2 /(k^2u)
	 *@	Where:
	 *@		d  = displacement height
	 *@		z  = height of the wind speed measurements
	 *@		u  = windspeed
	 *@		z0 = roughness length
	 *@		k  = von Karman constant
	 *
	 * Returns: ra  */
	MAKE_INTRINSIC(".eaes", sl_eaes, VOID_TYPE, 0),
	/*I:eaes
	 *@Float ea, Float, es  eaes(Float td,Float rh)
	 *
	 * Determines saturation vapour pressure (@ea@) and actual vapour
	 * presssure (@ea@) from relative humidity and dry bulb temperature.
	 * @es@ is calculated according to Bringfeld 1986. Relative 
	 * humidity should be in % and dry bulb temp in degrees Celcius.
	 * @ea@ and @es@ are in mbar
	 */
	MAKE_INTRINSIC(".vslope", sl_vslope, FLOAT_TYPE, 0),
	/*I:vslope
	 *@Float vslope(Float Td,Float es);
	 *
	 * Calculates the slope of the saturation vapour pressure curve from es
	 * (vapour pressure at saturation) and dry bulb temp for use in Penman
	 * Eo and Penman-Montheith.  Required input: Td (dry bulb temp [oC]), es
	 * (vapour pressure at saturation) [mbar]
	 *
	 * Returns: slope of the vapour pressure curve */
	MAKE_INTRINSIC(".e0", sl_e0, FLOAT_TYPE, 0),
	/*I:e0
	 *@Float e0(Float Rnetopen, Float Slope, Float Gamma, Float earo)
	 *
	 * Calculates penman open water evaporation using net radiation.
	 * Use other functions to get Gamma, Slope,  and earo and net radiation
	 * above open water
	 *
	 * Returns: e0 in mm/day 
	 */
	SLANG_END_TABLE
};

/*C:sl_met_init
 * @void sl_met_init (void)
 *
 * Initializes the metlib S-Lang intrinsics
 */
void
sl_met_init (void)
{
	if (!SLang_add_table(Met_Lib, "Met_Lib")) {
		(void)fprintf(stderr, "Unable to initialize Met_Lib S-Lang intrinsics.\n");
		exit(-1);
	}
	/* Turn on debugging */
	SLang_Traceback = 1;
}


void sl_eaes(void)
{
	double td,rh,ea,es;
	int d2;

	if (SLang_pop_float (&rh,&d2,&d2)){
		(void)fprintf(stderr,"Argument 1 (rh) of eaes not given\n");
		return;
	}
	if (SLang_pop_float (&td,&d2,&d2)){
		(void)fprintf(stderr,"Argument 0 (td) of eaes not given\n");
		return;
	}

	eaes(td,rh,&ea,&es);

	SLang_push_float (ea);
	SLang_push_float (es);
}

double sl_vslope(void)
{
	double td,es;
	int d2;

	if (SLang_pop_float (&es,&d2,&d2)){
		(void)fprintf(stderr,"Argument 1 (es) of vslope not given\n");
		return 0.0;
	}
	if (SLang_pop_float (&td,&d2,&d2)){
		(void)fprintf(stderr,"Argument 0 (td) of vslope not given\n");
		return 0.0;
	}

	return vslope(td,es);
}

double sl_rnet_open(void)
{
	double Rs, Rsout, Rnet,lambda;
	int d2;

	if (SLang_pop_float (&lambda,&d2,&d2)){
		(void)fprintf(stderr,"Argument 3 (lambda) of rnet_open not given\n");
		return 0.0;
	}
	if (SLang_pop_float (&Rnet,&d2,&d2)){
		(void)fprintf(stderr,"Argument 2 (Rnet) of rnet_open not given\n");
		return 0.0;
	}
	if (SLang_pop_float (&Rsout,&d2,&d2)){
		(void)fprintf(stderr,"Argument 1 (Rsout) of rnet_open not given\n");
		return 0.0;
	}
	if (SLang_pop_float (&Rs,&d2,&d2)){
		(void)fprintf(stderr,"Argument 0 (Rs) of rnet_open not given\n");
		return 0.0;
	}
	return rnet_open(Rs, Rsout, Rnet,lambda);
}

double sl_e0(void)
{
	double rnet,slope,gamma,earo;
	int d2;


	if (SLang_pop_float (&earo,&d2,&d2)){
		(void)fprintf(stderr,"Argument 3 (earo) of e0b not given\n");
		return 0.0;
	}
	if (SLang_pop_float (&gamma,&d2,&d2)){
		(void)fprintf(stderr,"Argument 2 (gamma) of e0b not given\n");
		return 0.0;
	}
	if (SLang_pop_float (&slope,&d2,&d2)){
		(void)fprintf(stderr,"Argument 1 (slope) of e0b not given\n");
		return 0.0;
	}
	if (SLang_pop_float (&rnet,&d2,&d2)){
		(void)fprintf(stderr,"Argument 0 (rnetopen) of e0b not given\n");
		return 0.0;
	}

	return e0(rnet, slope,gamma,earo);
}


double
sl_earo(void)
{
	double ea,es,u;
	int d2;

	if (SLang_pop_float (&u,&d2,&d2)){
		(void)fprintf(stderr,"Argument 2 (u) of earo not given\n");
		return 0.0;
	}
	if (SLang_pop_float (&es,&d2,&d2)){
		(void)fprintf(stderr,"Argument 1 (es) of earo not given\n");
		return 0.0;
	}
	if (SLang_pop_float (&ea,&d2,&d2)){
		(void)fprintf(stderr,"Argument 0 (ea) of earo not given\n");
		return 0.0;
	}

	return earo(ea,es, u);
}

double 
sl_ra (void)
{
	double z,z0,d,u;
	int d2;

	if (SLang_pop_float (&u,&d2,&d2)){
		(void)fprintf(stderr,"Argument 3 (u) of ra not given\n");
		return 0.0;
	}
	if (SLang_pop_float (&d,&d2,&d2)){
		(void)fprintf(stderr,"Argument 2 (d) of ra not given\n");
		return 0.0;
	}
	if (SLang_pop_float (&z0,&d2,&d2)){
		(void)fprintf(stderr,"Argument 1 (z0) of ra not given\n");
		return 0.0;
	}
	if (SLang_pop_float (&z,&d2,&d2)){
		(void)fprintf(stderr,"Argument 0 (z) of ra not given\n");
		return 0.0;
	}

	return ra(z,z0,d, u);
}

double 
sl_gamma(void)
{
	int d2;
	double lambda,td;

	if (SLang_pop_float (&lambda,&d2,&d2)){
		(void)fprintf(stderr,"Argument 1 (lambda) of gamma not given\n");
		return 0.0;
	}
	if (SLang_pop_float (&td,&d2,&d2)){
		(void)fprintf(stderr,"Argument 0 (td) of gamma not given\n");
		return 0.0;
	}

	return mgamma(td, lambda);
}

double
sl_lambda (void)
{
	int d2;
	double td;

	if (SLang_pop_float (&td,&d2,&d2)){
		(void)fprintf(stderr,"Argument 0 (td) of lambda not given\n");
		return 0.0;
	}

	return  lambda(td);
}


double sl_rnet_open_nN(void)
{
	double Rs, nN, td,ea, lambda;
	int d2;

	if (SLang_pop_float (&lambda,&d2,&d2)){
		(void)fprintf(stderr,"Argument 4 (lambda) of rnet_open not given\n");
		return 0.0;
	}
	if (SLang_pop_float (&ea,&d2,&d2)){
		(void)fprintf(stderr,"Argument 3 (ea) of rnet_open not given\n");
		return 0.0;
	}
	if (SLang_pop_float (&td,&d2,&d2)){
		(void)fprintf(stderr,"Argument 2 (td) of rnet_open not given\n");
		return 0.0;
	}
	if (SLang_pop_float (&nN,&d2,&d2)){
		(void)fprintf(stderr,"Argument 1 (nN) of rnet_open not given\n");
		return 0.0;
	}
	if (SLang_pop_float (&Rs,&d2,&d2)){
		(void)fprintf(stderr,"Argument 0 (Rs) of rnet_open not given\n");
		return 0.0;
	}

	return rnet_open_nN(Rs, nN, td, ea,lambda);
}

/* gash (double ER, double p, p_tr, S, P, LAI)
 */
void
sl_gash()
{
	double p,p_tr,S,E,P,T,ER,throughfall,stemflow,interception,SS,LAI;
	int d2;


	if (SLang_pop_float (&LAI,&d2,&d2)){
		(void)fprintf(stderr,"gash: error in first argument\n");
		return ;
	}
	if (SLang_pop_float (&P,&d2,&d2)){
		(void)fprintf(stderr,"gash: error in argument 2\n");
		return ;
	}
	if (SLang_pop_float (&S,&d2,&d2)){
		(void)fprintf(stderr,"gash: error in argument 3\n");
		return ;
	}
	if (SLang_pop_float (&p_tr,&d2,&d2)){
		(void)fprintf(stderr,"gash: error in argument 4\n");
		return ;
	}
	if (SLang_pop_float (&p,&d2,&d2)){
		(void)fprintf(stderr,"gash: error in argument 5\n");
		return ;
	}
	if (SLang_pop_float (&ER,&d2,&d2)){
		(void)fprintf(stderr,"gash: error in argument 6\n");
		return ;
	}
	gash (0, p, p_tr,S,E,P,1, ER,&throughfall,&stemflow,
		&interception,&SS,LAI);
	SLang_push_float (interception);
	SLang_push_float (stemflow);
	SLang_push_float (throughfall);
}

#endif /*HAVE_LIBSLANG*/
