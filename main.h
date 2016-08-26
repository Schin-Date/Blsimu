// main.h to call c functions
#ifndef __MAIN_H__
#define __MAIN_H__
#ifdef __cplusplus
//#include <lortz.h>
//class Electron;
//class Photon;
//class Particle;
extern "C" {
#endif /*__cplusplus*/
//optics.c
    extern int get_opt(FILE *logstr, double spoint,
                       double *beta_x_s, double *alpha_x_s, double *gamma_x_s,
                       double *nu_x_s, double *eta_x_s, double *etap_x_s,
                       double *beta_y_s, double *alpha_y_s, double *gamma_y_s,
                       double *nu_y_s, double *eta_y_s, double *etap_y_s
                       );
    extern int read_opt(FILE *logstr, char const *infile, double smin, double smax);
    extern int eat_table_optics_all(FILE *logstr, FILE *instr,
                                    int iskip, int nline,
                             int rcolumn, char *tunefmt,
                             double *tunex, double *tuney,
                                    int *ndata, double *Rdata);
//lattice.c
    extern int track_lattice(FILE *logstr, double *X4, double *P4, int Npcl,
                             double CHMBDX, double CHMBDY,
                             double slastelm, double pel0 );
    extern int read_lattice(FILE *logstr, char const *infile, double smin, double smax);
    extern int eat_table_lattice(FILE *logstr, FILE *instr, int iskip, int nline,
                                 int icolumn,  int rcolumn, int scolumn,
                                 int *ndata, int *Idata, double *Rdata, char **Sdata);
//bendmag.c
    extern int bendmag(FILE *logstr, double x1, double xp1, double y1, double yp1,
                       double z1, double pel,
                       double *x2, double *xp2, double *y2, double *yp2,
                       double *z2, int *direct,
                       double lenB, double rho8,
                       char *model
                       );
//matrices.c
    extern int mtxprod(FILE *logstr, int N, double *A, int L, double *B, int M, double *C);
    extern int mtxcopy(FILE *logstr, int N, int M, double *A, double *B);
    extern int  mtxunit(FILE *logstr, int N, int M, double *A);
//scat_points.c
    extern int set_Lprof(FILE *logstr, double Llambda, double laser_waist,
                         double laser_focus, double z1, double z2 );
    extern int scat_points(FILE *logstr, double *X4, int Npcl, char const *cntl);
//stdran48.c
    extern void setrandom(int i);
    extern int getseed();
    extern double getrandom();
//gauss.c
    extern int getrandom_gauss(double sigma, double mean, double max,
                               double *r1, double *r2);
//bmgen.c
    extern int bmgen(FILE *logstr, double *X4, double *P4, int Npcl,
                     double emittance_x, double emittance_y,
                     double CHMBDX, double CHMBDY,
                     double pel0, double e0spread, double OVRVLR, char const *cntl);
//compt.c
    extern int compton(FILE *logstr, int Npcl, double *Pel4, double *Pho4,
                       double *PSel4, double *PSph4, char const *cntl);
//tagger.c
    extern int get_tagger(FILE *logstr, double *X4, double *P4, int npcl, int *iftag);
    extern int read_tagger(FILE *logstr, char const *infile);
    extern int eat_table_tagger(FILE *logstr, FILE *instr, int iskip, int nline,
                                    int rcolumn,
                                    int *ndata, double *Rdata);
//    extern
//    extern double R4OVER;
//    extern double R4UNDR;
//    extern double EMASS;
    
    
#ifdef __cplusplus
}
#endif /*__cplusplus*/

//Global variables in the following are introduced temporary
//to include old c-functions with a minimum efforts.
//They should be eliminated in the near future.
//#ifdef __cplusplus
//extern "C" double R4OVER;
//extern "C" double R4UNDR;
//extern "C" double EMASS;
//#else
//#ifdef __cplusplus
//extern "C" {
//#endif
//global scope
extern double R4OVER;
extern double R4UNDR;
extern double EMASS;
//#ifdef __cplusplus
//}
//#endif
//static double R4OVER;
//static double R4UNDR;
//static double EMASS;
//double R4OVER;
//double R4UNDR;
//double EMASS;

#endif /*__MAIN_H__*/
