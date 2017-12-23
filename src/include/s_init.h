#ifndef S_INIT_H_
#define S_INIT_H_
extern char	*sl_statfile(char *, char *);
extern int at_start_f(void);
extern int readset(char *fname,char *name);
extern int at_end_f(void);
extern int each_step_f(void);
extern double getset_x (int *set,int *pos);
extern double getset_y (int *set,int *pos);
extern double getset_y_from_x (int *set, double *x);
extern int getset_points (int *set);
extern char *getset_name (int *set);
extern char *getset_fname (int *set);
extern double getset_xmax(int *set);
extern double getset_ymax(int *set);
extern double getset_ymin(int *set);
extern double getset_xmin(int *set);
extern double getset_xsum(int *set);
extern double getset_ysum(int *set);
extern void Sgetdefar (const char *section,const char *name, char *fname, int *exitonerror);
extern char *Sgetdefstr (const char *section,const char *name,char *def,char *fname, int *exitonerror);
extern double Sgetdefdoub (const char *section,const char *name,double *def,char *fname, int *exitonerror);
extern int Sgetdefint (const char *section,const char *name,int *def,char *fname, int *exitonerror);
extern void show_history(void);
extern void del_history(void);
extern int addtohist(char *item);
extern int save_history(char *histfname);
extern void s_printsum(void);
extern double estrs(void);
extern int s_writememini(char *fname,int *fmt);
extern void s_dumpset(int *setn,char *fname);
extern int SSLang_init_tty();
extern void Ssetvalue(char *section, char *var, char *name, int *check);
#endif 	/*S_INIT_H_*/

