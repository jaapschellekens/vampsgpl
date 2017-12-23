/* Header: /home/schj/src/vamps_0.96/src/environ/RCS/canopy.c,v 1.5 1995/12/17 21:50:17 schj Exp $ */

/*
 *  $RCSfile: canopy.c,v $
 *  $Author: schj $
 *  $Date: 1999/01/06 12:13:01 $
 */

static char RCSid[] =
"$Id: canopy.c,v 1.2 1999/01/06 12:13:01 schj Alpha $";

#include <math.h>
#include "vamps.h"
#include "deffile.h"
#include "canopy.h"
#include "swatsoil.h"
#include "met.h"

#define TOCM 60*60*24*0.1

static double storage;
double VPD,ea,es,rra;
int intmethod, transmethod;
/* if drytime  > 0 then this will be used to determine if the canopy is
 * dry or not. Handy if you don't use the interception stuff 
 */ 
double drytime = -1.0;
double wetevap = -1.0;
int calcrs =0;
canopytp *canop;
int canopy_layers = 1;
static double lai, laifrac;
static double *laiprof;
double kk;
int gashm = 0; /* default to old unadjusted gash method */
double Rnet_absorb;
static double p_tr, p_f, S;	/* proportion going to trunks, free troughfall, storage */
static int calcra = TRUE;
static double thefrac, Cgamma, delta;
static double Z, z_0, d, rs;	/* Needed for aerodynamic resistance calculation */



/*
   If one layer then a test for Rnet_absorb is done. For multiple layesr we _need_
   the CLATTER model (lai and laiprof)
 */

/*-
 * int precanop()
 * Gets the canopy stuff from the input file and checks if the needed datasets
 * are present.
 */
void
precanop (void)
{
	int pts,i;
	char s[1024];
	XY *t_netrad;
	XY *interception;
	XY *ptranspiration;
	XY *pevaporation;
	XY *t_lai;
	XY *canopS;
	XY *spep;

	canopy_layers = getdefint ("canopy", "layers", canopy_layers, infilename, TRUE);
	canop = (canopytp *) ts_memory ((void *) NULL, canopy_layers * sizeof (canopytp), progname);
	/* set some stuff to zero */
	for (i=0; i< canopy_layers; i++)
		canop[i].actstorage = 0.0;

	drytime = getdefdoub ("canopy", "drytime", drytime, infilename, FALSE);
	wetevap = getdefdoub ("canopy", "wetevap", wetevap, infilename, FALSE);
	showit ("canopy",MESG,"Using fixed canopy drying time",1,verbose);
	interception = (XY *) ts_memory (NULL, steps * sizeof (XY), progname);
	if (Dsoil){
		spep = (XY *) ts_memory (NULL, steps * sizeof (XY), progname);
		(void)add_set (spep, "spe", "spe", steps, 0, 0);
		id.spe = getsetbyname("spe");
	}
	ptranspiration = (XY *) ts_memory (NULL, steps * sizeof (XY), progname);
	pevaporation = (XY *) ts_memory (NULL, steps * sizeof (XY), progname);
	canopS = (XY *) ts_memory (NULL, steps * sizeof (XY), progname);
	(void)add_set (interception, "inr", "inr", steps, 0, 0);
	(void)add_set (canopS, "sca", "sca", steps, 0, 0);
	(void)add_set (ptranspiration, "ptr", "ptr", steps, 0, 0);
	(void)add_set (pevaporation, "pev", "pev", steps, 0, 0);
	id.pev = getsetbyname("pev");
	id.ptr = getsetbyname("ptr");
	id.inr = getsetbyname("inr");
	id.sca =getsetbyname("sca");

	if ((rra = getdefdoub ("canopy", "ra", 0.0, infilename, FALSE)) > 0.0)
		calcra = FALSE;


  /* How will interception be determined */
  intmethod = getdefint ("interception", "method", 0, infilename, TRUE);
  switch (intmethod){
	  case 0:{			/* use gash */
			 /* first check singe value lai , interception
			  * and canopy then, check for lai time serie */
			 if ((lai = getdefdoub ("interception", "lai", MISSVAL, infilename, FALSE)) == MISSVAL){
				 if ((lai = getdefdoub ("canopy", "lai", MISSVAL, infilename, FALSE)) == MISSVAL)
					 if (getsetbyname ("lai") < 0){
						 if(getdefstr ("ts", "lai", NULL, infilename, FALSE))
							 (void)get_data (getdefstr ("ts", "lai", "lai", infilename, TRUE), "lai",steps);
					 }
			 }
			 /* construct lai time serie from fixed value */
			 t_lai = (XY *) ts_memory (NULL, steps * sizeof (XY), progname);
			 for (i = 0; i < steps; i++){
				 t_lai[i].x = (double) i;
				 t_lai[i].y = lai;
			 }
			 (void)add_set (t_lai, "lai", "lai", steps, 0, 0);
			 id.lai = getsetbyname ("lai");	/* set the laiset */

			 thefrac = getdefdoub ("interception", "E_avg/R", 0.0, infilename, TRUE);
			 gashm = getdefint("interception","gashm",gashm,infilename,FALSE);
			 if (thiststep > 1.0 && gashm != 1)
				 showit ("canopy",WARN,"using unadapted gash with timestep < 1 day",1,verbose);
			 if (thefrac != 0.0)
				 showit ("canopy",MESG,"using predefined E_avg/R",1,verbose);

			 p_tr = getdefdoub ("interception", "p_tr", 0.0, infilename, TRUE);
			 p_f = getdefdoub ("interception", "p_f", 0.0, infilename, TRUE);
			 S = getdefdoub ("interception", "S", 0.1, infilename, TRUE);
			 break;
		 }
	  case 1:{			/* use rutter */
			 showit ("canopy",WARN,"Rutter not tested yet",0,0);
			 p_tr = getdefdoub ("interception", "p_tr", 0.0, infilename, TRUE);
			 p_f = getdefdoub ("interception", "p_f", 0.0, infilename, TRUE);
			 S = getdefdoub ("interception", "S", 0.1, infilename, TRUE);
			 break;
		 }
	  case 3:			/* lai fraction */
		 {
			 /* first check singe value lai , interception and canopy 
			    than, check for lai time serie */
			 if ((lai = getdefdoub ("interception", "lai", -1.0, infilename, FALSE)) < 0.0){
				 if ((lai = getdefdoub ("canopy", "lai", -1.0, infilename, FALSE)) < 0.0)
					 if (getsetbyname ("lai") < 0)
						 (void)get_data (getdefstr ("ts", "lai", "lai", infilename, TRUE), "lai",steps);
			 }
			 /* construct lai time serie from fixed value */
			 if (lai >= 0.0){
				 t_lai = (XY *) ts_memory (NULL, steps * sizeof (XY), progname);
				 for (i = 0; i < steps; i++){
					 t_lai[i].x = (double) i;
					 t_lai[i].y = lai;
				 }
				 (void)add_set (t_lai, "lai", "lai", steps, 0, 0);
			 }
			 id.lai = getsetbyname ("lai");	/* set the laiset */
			 laifrac = getdefdoub ("interception", "laifrac", laifrac, infilename, TRUE);
			 break;
		 }
	  case 2:			/*use calder */
		 {
			 Cgamma = getdefdoub ("interception", "gamma", Cgamma, infilename, TRUE);
			 delta = getdefdoub ("interception", "delta", delta, infilename, TRUE);
			 break;
		 }
	  default:
		 Perror (progname, 1,0, RCSid, "Interception method not known", "> 2");
		 break;
  }

  /* How will dry canopy transpiration be determined */
  transmethod = getdefint ("canopy", "transpiration", 0, infilename, TRUE);
  switch (transmethod){
	  case 2:
		  /* penman monteith */
		  Z = getdefdoub ("canopy", "z", 0.0, infilename, TRUE);
		  z_0 = getdefdoub ("canopy", "z_0", 0.0, infilename, TRUE);
		  d = getdefdoub ("canopy", "d", 0.0, infilename, TRUE);
		  rs = getdefdoub ("canopy", "rs", 0.0, infilename, FALSE);
		  if (rs == 0.0){
			  calcrs = TRUE;
			  showit("canopy",WARN,"no rs given, using regression equation estrs()",1,verbose);
		  }
		  /* Check the datasets we need */
		  if (getsetbyname ("nra") < 0){	/* We need radiation data */
			  strcpy(s,getdefstr ("ts", "nra", "NONE", infilename, FALSE));
			  if (strcmp(s,"NONE") != 0){
				  (void)get_data (s, "nra",steps);
			  }else{
				  (void)get_data (getdefstr ("ts", "ira", "NONE", infilename, TRUE), "ira",steps);
				  t_netrad = (XY *) ts_memory (NULL, steps * sizeof (XY), progname);
				  id.ira = getsetbyname("ira");
				  for (i = 0; i < steps; i++){
					  t_netrad[i].x = data[id.pre].xy[i].x;
					  t_netrad[i].y = Rs_to_Rn(data[id.ira].xy[i].y);
				  }
				  (void)add_set (t_netrad, "nra", "nra", steps, 0, 0);
			  }
		  }
		  id.nra = getsetbyname ("nra");
		  if (getsetbyname ("tem") < 0)	/* We need temperature data oC */
			  (void)get_data (getdefstr ("ts", "tem", "tem", infilename, TRUE), "tem",steps);
		  id.tem = getsetbyname ("tem");
		  if (getsetbyname ("rlh") < 0)	/* We need rel humid % */
			  (void)get_data (getdefstr ("ts", "rlh", "rlh", infilename, TRUE), "rlh",steps);
		  id.rlh = getsetbyname ("rlh");
		  if (getsetbyname ("pre") < 0){	/* We need precipitation % */
			  (void)get_data (getdefstr ("ts", "pre", "pre", infilename, TRUE), "pre",steps);
			  Perror(progname,1,0,RCSid,"Added second pset!!!","");
		  }
		  id.pre = getsetbyname ("pre");
		  if (getsetbyname ("win") < 0)	/* windspeed (m/s) */
			  (void)get_data (getdefstr ("ts", "win", "win", infilename, TRUE), "win",steps);
		  id.win = getsetbyname ("win");

		  break;
	  case 3: /* read from file */
		del_set(getsetbyname("ptr"));
		get_data(getdefstr ("ts", "ptr", "NONE", infilename, TRUE),"ptr",steps);
		id.ptr = getsetbyname("ptr");
		del_set(getsetbyname("pev"));
		get_data(getdefstr ("ts", "pev", "NONE", infilename, TRUE),"pev",steps);
		id.pev = getsetbyname("pev");
		break;
	  default:
			  Perror (progname, 1,0, RCSid, "transpiration method not known", "");
  }

  if (canopy_layers > 1){/*We need an LAI profile for this (see Roberts'93) */
	  lai = getdefdoub ("canopy", "lai", 0.0, infilename, TRUE);
	  laiprof = getdefar ("canopy", "laiprof", NULL, infilename, &pts, TRUE);
	  if (pts != canopy_layers)
		  Perror (progname, 1,0, RCSid, "Laiprof points not equal to canopy layers", "");
	  kk = getdefdoub ("canopy", "k", 0.0, infilename, TRUE);
  }else{
	  if ((Rnet_absorb =
			  getdefdoub ("canopy", "Rnet_absorb", 0.0, infilename, FALSE)) > 0.0){
		  ;	/*empty */
	  }else{			/* calculate via Roberts and lai */
		  Perror (progname, 1,0, RCSid, "Lai not finished", "Use Rnet_ansorb");
		  lai = getdefdoub ("canopy", "lai", 0.0, infilename, TRUE);
		  kk = getdefdoub ("canopy", "k", 0.0, infilename, TRUE);
	  }
  }
}

void
postcanop ()
{
  free (canop);
}

void
tstep_ocanop (int tstep, double *precipitation,
		double *interception, double *transpiration,
		double *soilevaporation)
{
	double Cp = 1005 , Mgamma , slope, L, rho, wetE, dryE;
	extern topout (int tstep);
	static int lastwet = 0;
	static int wetsteps = 0;

	Mgamma = slope= L= rho =0.0;
	/* Calculate air density */
	rho = 1.201 * (290.0 * (998.0 - 0.378 * ea)) / ((data[id.tem].xy[tstep].y + 273.15) * 1000.0);
	if (Rnet_absorb > 0.0){/* We have one layer ! Calculate rnet via simple eq */
		canop[0].rnet = data[id.nra].xy[tstep].y * Rnet_absorb;
		eaes(data[id.tem].xy[tstep].y, data[id.rlh].xy[tstep].y,&ea,&es);
		L = lambda(data[id.tem].xy[tstep].y);
		slope = vslope(data[id.tem].xy[tstep].y,es);
		Mgamma = mgamma(data[id.tem].xy[tstep].y, L);

      /* Calculate airodynamic resistance if not given */
      if (calcra)
	rra = ra (Z, z_0, d, data[id.win].xy[tstep].y);
      if (calcrs) /* we have to estimate it using S-Lang function */
      	rs = estrs();
      
      canop[0].ra = rra;
      canop[0].ea = ea;
      canop[0].es = es;
      canop[0].rs = rs;
      canop[0].rho = rho;
      canop[0].slope = slope;
      canop[0].gamma = Mgamma;
      canop[0].L = L;
      canop[0].Cp = Cp;

      /* Canopy wet or dry ? */

      if (data[id.pre].xy[tstep].y > 0.0){
	      lastwet = tstep;
	      wetsteps++;
      }

      if ((data[id.pre].xy[tstep].y == 0.0 /*&& storage == 0.0*/ && drytime < 0.0) || 
		      ((drytime > 0.0 && data[id.pre].xy[tstep].x -
		       data[id.pre].xy[lastwet].x > drytime) || !wetsteps)){
	  printint("wet",0);
	  canop[0].E_dry = penmon (canop[0].rnet*Rnet_absorb, slope, Mgamma, rho, Cp, es, ea, rra, rs)/L* TOCM; /* convert to cm */
	  canop[0].transpiration = canop[0].E_dry;
	  /* do interception as wel */
	  switch (intmethod){
	    case 0:
		/* for gash always use wetE */
		    if (wetevap >0.0)
			    wetE = wetevap;
		    else
			    wetE = canop[0].E_dry;
	      gash (gashm,p_f, p_tr, S,wetE , data[id.pre].xy[tstep].y,
		    thiststep,thefrac,&canop[0].throughfall,
		    &canop[0].stemflow, &canop[0].interception,&storage,data[id.lai].xy[tstep].y);
	      break;
	    case 1:
	      canop[0].actstorage = 
		rutter (p_f,p_tr,data[id.pre].xy[tstep].y,
			/*penmon (canop[0].rnet*Rnet_absorb, slope, Mgamma, 
			    rho, Cp, es, ea, rra, 0.0)/L* TOCM*/data[id.pev].xy[tstep].y,
			S,storage,&canop[0].throughfall,&canop[0].stemflow,
			&canop[0].E_wet,&canop[0].interception,thiststep);
	      storage = canop[0].actstorage;
	      break;
	    default:
	      break;
	    }	  /*canop[0].interception = canop[0].E_wet = 0.0;*/
	  canop[0].E_wet = canop[0].interception;
	  canop[0].throughfall = 0.0;
	  canop[0].actstorage = storage;
	}else{			
          if (gashm == 0)
		  storage = data[id.pre].xy[tstep].y < S ? data[id.pre].xy[tstep].y : S;
	  printint("wet",1);

	  dryE = penmon (canop[0].rnet*Rnet_absorb, slope, Mgamma, rho, Cp, es, ea, rra, rs)/L* TOCM; /* convert to cm */
	  if (wetevap > 0.0)
		  wetE = wetevap;
	  else
		  wetE = penmon (canop[0].rnet*Rnet_absorb, slope, Mgamma, rho, Cp, es, ea, rra, 0.0)/L* TOCM; /* convert to cm */
	/* reduce transpiration for canopy period */
	  canop[0].transpiration = canop[0].E_dry;
	  /* Interception via gash */
	  switch (intmethod){
	    case 0:		/* gash */
	      gash (gashm,p_f, p_tr, S, wetE, data[id.pre].xy[tstep].y,
		    thiststep, thefrac,
		    &canop[0].throughfall, &canop[0].stemflow, 
		    &canop[0].interception,&storage,data[id.lai].xy[tstep].y);
	      canop[0].actstorage = storage;
	      break;
	    case 1:
	      canop[0].actstorage = 
		rutter (p_f,p_tr,data[id.pre].xy[tstep].y,
		data[id.pev].xy[tstep].y/*wetE*/, S,storage,&canop[0].throughfall,&canop[0].stemflow,
		      &canop[0].E_wet,&canop[0].interception,thiststep);
	      storage = canop[0].actstorage;
	      break;
	    case 2:
	      Perror (progname, 1,0, RCSid, "Interception method not done", "calder");
	      break;
	    case 3:
	      lai_int (data[id.pre].xy[tstep].y, data[id.lai].xy[tstep].y, laifrac,
			 &canop[0].throughfall, &canop[0].stemflow, &canop[0].interception);
	      break;
	    default:
	      break;
	    }
	  /*dryE *= 1.0 - (storage/S) > 0.0 ?  1.0 - (storage/S) > 0.0: 0.0;*/
	  dryE = 0.0;
	  canop[0].E_wet = canop[0].interception;
	  canop[0].E_dry = canop[0].transpiration = dryE;
	}
    } else{
      Perror (progname, 1,0, RCSid, "Not yet done", "Multi layer canopy");
    }
  data[id.sca].xy[tstep].y = canop[0].actstorage;
  data[id.sca].xy[tstep].x = data[id.pre].xy[tstep].x;
  data[id.ptr].xy[tstep].y = canop[0].transpiration;
  data[id.ptr].xy[tstep].x = data[id.pre].xy[tstep].x;
  data[id.inr].xy[tstep].y= canop[0].interception;
  data[id.inr].xy[tstep].x=data[id.pre].xy[tstep].x;
  data[id.pev].xy[tstep].y= penmon (canop[0].rnet * (1 - Rnet_absorb), slope, Mgamma, rho, Cp, es, ea, rra, 0.0)/L* TOCM;
  data[id.pev].xy[tstep].x = data[id.pre].xy[tstep].x;
  if (Dsoil){
    data[id.spe].xy[tstep].y = penmon_soil(canop[0].rnet*(1-Rnet_absorb), slope, Mgamma)/L* TOCM;
    data[id.spe].xy[tstep].x = data[id.pre].xy[tstep].x;
  }
  *interception = data[id.inr].xy[tstep].y;
  *soilevaporation = data[id.spe].xy[tstep].y;
  *transpiration = data[id.ptr].xy[tstep].y;
  *precipitation = data[id.pre].xy[tstep].y;
  printfl("ra",canop[0].ra);
  printfl("rs",canop[0].rs);
  printfl("rho",canop[0].rho);
  printfl("ea",canop[0].ea);
  printfl("es",canop[0].es);
  printfl("gamma",canop[0].gamma);
  printfl("slope",canop[0].slope);
  printfl("VPD",VPD);
  printfl("L",canop[0].L);
  printfl("Cp",canop[0].Cp);
  printfl("interception",canop[0].E_wet);
  printfl("transpiration",canop[0].E_dry);
  printfl("Cstorage",canop[0].actstorage);
  printfl("stemflow",canop[0].stemflow);
  printfl("throughfall",canop[0].throughfall);
  topout (tstep);
}
