#ifndef CANOPY_H
#define CANOPY_H
extern int nr_c_horizons;  	/* Number of horizons defined */
typedef struct {
	double storage_initial;
	double thickness;
	double rnet;
	double E_dry;
	double E_wet;
	double ra;
	double rs;
	double es;
	double ea;
	double slope;
	double gamma;
	double L;
	double Cp;
	double rho;
	double throughfall;
	double interception;
	double transpiration;
	double stemflow;
        double actstorage;
} canopytp;

extern	canopytp	*canop;
/*extern double storage;*/ /* canopy storage */
extern	double	Rnet_absorb;
extern double ea,es,VPD; /* in potevap.h*/
extern int intmethod,transmethod;
extern double Rs_to_Rn (double Rs);
extern void CalcDeltaGamma (double Td, double RH, double *Ea, double *Es,
			    double *Gamma, double *Slope, double *L, 
			    double *Cp);
extern double CalcPenmanEoA (double Td, double Rs, double sunratio,
			     double u, double Ea, double Es, double Slope,
			     double Gamma, double L);
extern double CalcPenmanEoB (double Td, double Rs, double Rs_out,
			     double Rnet, double u, double Ea, double Es, 
			     double Slope, double Gamma, double L);
extern double ET (double A, double delta, double mgamma, double rho,
		  double c_p, double e_sat, double e_act, double r_a, 
		  double r_s);
extern int gash (int methos, double p, double p_tr, double S, double E, 
		 double P, double T, double thefrac, double *throughfall, 
		 double *stemflow, double *interception,double *SS,double LAI);
extern double R_A (double z, double z_0, double d, double u);
extern void lai_int (double P, double lai, double coeff, double *throughfall,
		    double *stemflow, double *interception);
extern double SET (double A,double delta,double gamma);
extern double
rutter (double p,double p_tr,double P,double E,double S,double Cinit,
	double *TF,double *SF,double *Ei, double *I,double tstep);
#endif /* SOILS_H */

