// FigPtTrans Galeo version
//If you read this line, the file is copied from cpplib/asource/RootDraw_GUI
//#include <TApplication.h>
#ifndef __ELETRACER_H__
#define __ELETRACER_H__

//#include <TTree.h>
//#include <TH1D.h>
//#include <TH2.h>
//#include <TCanvas.h>
//#include "witgn.h"

//class GUI_ele0;

class EleTracer {
public:
    EleTracer(){};
	virtual ~EleTracer();
    void setTracer(const witgn::SetDoPrm & blsimudoprm, FILE *logstr);
    int track(double* X, double* P, int &nbmpcl, double slastelm_m, double &pel0, FILE* &logstr);
    int tagger(int ievent, int nbmpcl, int nrejec, double *XSelec, double *PSelec, FILE *logstr);

private:
    std::string cntl_event;

    double smin;
    double smax;
    
    //Tagging parameters
    double Tags1;
    double Tags2;
    double BRz1;
    double BRz2;
    double Tagx0;
    double Tagxd;
    
    //chamber physical apertures
    double CHMBDX;
    double CHMBDY;

	
};
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/
//tagger.c
    extern int read_tagger(FILE *logstr, char const *infile);
    extern int get_tagger(FILE *logstr, double *X4, double *P4, int npcl, int *iftag);
//lattice.c
    extern int read_lattice(FILE *logstr, char const *infile, double smin, double smax);
    extern int track_lattice(FILE *logstr, double *X4, double *P4, int Npcl,
                              double CHMBDX, double CHMBDY,
                              double slastelm, double pel0 );
#ifdef __cplusplus
}
#endif /*__cplusplus*/



#endif