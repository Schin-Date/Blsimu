// main.h to call c functions
#ifndef __main_H__
#define __main_H__
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
#ifdef __cplusplus
}
#endif /*__cplusplus*/

//Global variables in the following are introduced temporary
//to include old c-functions with a minimum efforts.
//They should be eliminated in the near future.
//extern double R4OVER;
//extern double R4UNDR;
double R4OVER;
double R4UNDR;
double EMASS;
//double METER;

#endif /*__main_H__*/




#if(0)
//namespace klein {
class Electron: public lortz::Particle {
public:
//        Electron(){}
        Electron();
        Electron(Particle const& arg);
//    ~Electron();
    ~Electron(){}
    //kinini here
    void supplKinVar();
    const double &gamma() const {return value.gamma;}
    const double &beta() const {return value.beta;}
    const double &x1beta() const {return value.x1beta;}
//    const double &E() const {return value.E;}
////    const double &E() const {return Particle::E();}
//    const double &E2() const {return value.E2;}
//    const double &P() const {return value.P;}
//    const double &P2() const {return value.P2;}
    
private:
    struct {
//    double P2;
//    double P;
//    double E2;
//    double E;
    double gamma;
    double beta;
    double x1beta;//1 - beta for large gamma
    } value;
    
};//class Electron
class Photon: public lortz::Particle {
public:
    Photon();
    Photon(Particle const& arg);
    ~Photon(){};
    void supplKinVar();
//    const double &E() const {return value.E;}
//    const double &E2() const {return value.E2;}
//    const double &P() const {return value.P;}
//    const double &P2() const {return value.P2;}
private:
    struct {
//        double E2;
//        double E;
//        double P; // keep this variable for offshell photons
//        double P2; // keep this variable for offshell photons
//        double lambda;
    } value;
    
};//class Photon
//===========================================================================
class KleinNishina {
    public:
        KleinNishina(Electron const& ele, Photon const& photon);
        virtual ~KleinNishina();
        //    static int Kinematics(double upper, double lower,
        //                         double xbInteg[], double wbInteg[]);
        /* For a given set of electron.value and photon.value, evaluate
         derived variables
         */
//    void print_all();
    double unpol_total_cross_section();
    const double Elab_photon_Max() const {return initial.Elab_photon_Max_;}
//    double Elab_photon_at_thet(double const& thet, double const& phi=0);
    double Elab_photon_at_Omega(lortz::SpatialVector const& ehat_g);
    //give components of ehat_g in the same coordinate frame where
    //ele and photon have been given.
    double thet0_at_Egamma(double const& Egamma);
    double dsdo(lortz::SpatialVector const& ehat_g);
    double dsdEg(double const& Egamma);
//Definitions of **Lab.** angles
// electorn = (0,0,1)
// initial_photon = (sin thet_eL, 0, cos thet_eL)  0 =< thet_eL =< pi
// final_photon = (sin thet_eg * cos phi_g, sin thet_eg * sin phi_g
//                                          cos thet_eg)
//                    0 =< thet_eg =< pi, 0 =< phi_g < 2*pi
    
    private:
    struct{
        Electron ele_;
        Photon photon_;
        double wphoton_; //incident photon energy scaled by m_e
        double coselepho_; // cosine of momenta fo incident ele and photon
        double x_; // scaled s variable
        double s_; // mandelstam s
        double rs_; // total cms energy
        double E_total_; // total lab energy
        double P_total_; // total lab momentum
        double gamma_total_; // gamma of total en-mom. in lab
        double gammabeta_total_; // gamma*beta of total en-mom. in lab
        double Ecms_photon_; // photon energy in cms
        double Elab_photon_Min_; // minimum final photon energy in lab
        double Elab_photon_Max_; // maximum final photon energy in lab
        double Ecms_electron_; // electron energy in cms
        double Pcms_electron_; // electron momentum in cms
        double gamma_ele_cms_; // gamma of electron in cms
        double gammabeta_ele_cms_; // gamma*beta of electron in cms
        double Elab_electron_Min_; //minimum final electron energy in lab
        double Elab_electron_Max_; //maximum final electron energy in lab
//    double ceoMin_;// cosine of final electron wrt total P in lab
        double thet_e0_Max_;//max deviation angle of final electron wrt total P in lab
    } initial;
    struct{
        Photon Gamma_;
        double y_;//scaled u
        double wgamma_;//final gamma energy scaled by m_e
        double a_;//see "Introduction to ... I"
        double b_;//see "Introduction to ... I"
        double ybar_;//see "Introduction to ... I"
    } final;
    KleinNishina();
    void   initStateVars();
    double kleinNishinaF(double const& x, double const& y);
};
//}//namespace
#endif