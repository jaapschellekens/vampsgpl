#include "vamps.h"
#include "swatsoil.h"
#include <math.h>

double drootz;
double tronae, tronab, zronam;
int swupfu;					/* roo ex method */
extern double ptra; /* Potential tranporation */
double cofszb, cofsza;
int swsink;
double hlim3, hlim3h, hlim3l;
double hlim4;
int swhypr;
double cofsha, cofshb;
double hlim1, hlim2u, hlim2l;
double redosm;
double *rootfrac; /* Fraction of roots/total soil */

/*+Name: sminsert
 *
 *  Prototype: void sminsert(int pt, double depth)
 *
 *  Description:  Determines how much water to extract/add based on an external
 *  soil moisture time series. 
 *
 *  Returns:  nothing+*/
void sminsert(int pt, double depth)
{
	int i, top, bot, noddrz = 0;
	double t1;
	double smdepth;

	for (i = 0; i < layers; i++) /* Set to zero */
		smflow[i] = 0.0;

	if (depth > -1.0E-3)
	{
		/* nothing here, no roots, skip */
	}
	else
	{
		/* determine lowest compartment to use in inserting sm */
		smdepth = 0.0; /* depth in swap */
		for (i = 0; i < layers; i++)
		{
			noddrz = i;
			smdepth += dz[i];
			if (depth >= (smdepth - 1.0E-6))
				break;
		}

		/* Calculation of root extraction of the compartments */
		for (i = 0; i <= noddrz; i++)
		{ /* Check this !! */
			top = fabs(z[i] - 0.5 * dz[i]);
			bot = fabs(z[i] + 0.5 * dz[i]);
		
				qrot[i] = (a - (b * (bot + top) * 0.2)) * (bot - top);
		}
	}
}