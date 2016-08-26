/*If you read this line, the file is copied from cpplib/asource/RootDraw_GUI
130116
Modified the original copy of prodev/root/05GUI/02DrawFigure/02bDrawMyFigNobtn/FigOptics.cpp
*/
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <sstream>
//#include <memory>
//#include "witpy.h"
#include "witpy.h"
#include "plnck.h"
#include "EleTracer.h"
#include "main.h"

void EleTracer::setTracer(const witpy::SetDoPrm & blsimudoprm, FILE *logstr)
{
        if (3<witpy::SetDoPrm::GetPrtLvl()) std::cout << "N4"
            <<"...EleTracer(figdoprm"
            <<"): instantiated."<<std::endl;

    
    std::cout<<"DDD...globla R4OVER = "<<R4OVER<<std::endl;
    
    
    witpy::SetDoPrm::SetPrtLvl(2);
    
    cntl_event = blsimudoprm.getValue<std::string>("cntl_event");

        smin = blsimudoprm.getValue<double>("smin")*plnck::meter;
        smax = blsimudoprm.getValue<double>("smax")*plnck::meter;
        //latfile
        std::string latfile = blsimudoprm.getValue<std::string>("latfile");
        std::cout<<"...EleTracer::going to read "<<latfile
        <<"\n                  << (smin, smax) = ("
        <<smin/plnck::meter<<", "<<smax/plnck::meter<<") [m]"<<std::endl;
//        int status = read_lattice(logstr,latfile.c_str(),smin/plnck::meter,smax/plnck::meter);
//unused    int status = read_lattice(logstr,latfile.c_str(),smin/plnck::meter,smax/plnck::meter);
        read_lattice(logstr,latfile.c_str(),smin/plnck::meter,smax/plnck::meter);
        //======== this is a c function =========================
    
    /** detector **/
#ifdef DEBUG_FILE
    printf("D_FILE...main:tagger='%s'\n",tagger);
    printf("D_FILE...main:going to read_tagger\n");
#endif
//    std::string tagger = thisDir+blsimu.getValue<string>("tagger");
    std::string tagger = blsimudoprm.getValue<std::string>("tagger");
//    read_tagger(logstr,tagger.c_str());
    read_tagger(logstr,tagger.c_str());
    //======== this is a c function =========================
    
    /* Data accumulator initialization */

        //Tagging parameters
        Tags1 = blsimudoprm.getValue<double>("Tags1")*plnck::meter;
        Tags2 = blsimudoprm.getValue<double>("Tags2")*plnck::meter;
        BRz1 = blsimudoprm.getValue<double>("BRz1")*plnck::meter;
        BRz2 = blsimudoprm.getValue<double>("BRz2")*plnck::meter;
        Tagx0 = blsimudoprm.getValue<double>("Tagx0")*plnck::meter;
        Tagxd = blsimudoprm.getValue<double>("Tagxd")*plnck::meter;
        
        //chamber physical apertures
        CHMBDX = blsimudoprm.getValue<double>("CHMBDX")*plnck::meter;
        CHMBDY = blsimudoprm.getValue<double>("CHMBDY")*plnck::meter;
    
    
    
}
EleTracer::~EleTracer()
{
	if (3<witpy::SetDoPrm::GetPrtLvl()) std::cout<<"N4..."
        <<"~EleTracer()"<<std::endl;
    
}
int EleTracer::track(double* X, double* P, int &nbmpcl, double slastelm_m, double &pel0, FILE* &logstr){
    return track_lattice(logstr, X, P, nbmpcl, CHMBDX, CHMBDY, slastelm_m, pel0 );
    //======== this is a c function =========================
    /*070417
     The coordinate system for XSelec and PSelec are defined in the function
     track_lattice. Old difinitions with rotation of the coordinates followed
     in this section was documented in Museum/main.c070417.
     =======================================================================*/
    /* For each scattering point, find the transfer matrix */ /*>>>>>>>>>>>> review notes *//*>>> track.c */
    
    /*
     141113 tracking part separated to EleTracer
     ---------------------------------------------below is blsimu+ < c v1.3
     141021 this main is modified from a copy of blsimu/src/main.c
     
     ---------------------------------------------below is v1.2
     070108: section of back rotation of axis just after Bending
     061115: Lsstart and Lsend introduced
     061016: read blsimu.txt in for input parameters
     061001: cntl_ebeam added
     ---------------------------------------------below is v1.0
     060801: Lprof
     060728: nrejec counts throughout event loop
     060723-28: bl33 redefine
     060722: distributed
     060721: fixedpt
     060720: recompiled on miho
     060525: A new branch for BL31LSS
     060213: updated to use for low emittance optics
     
     
     The main routine for the electron beam tracking for LEPS
     original created in Feb. 2005 by S.D. in Londrina, Brazil
     Ref. bl33_construction/tracking_f/
     
     output filename:
     defined at line ~ 140 is composed of the following informations;
     The Laser focus Lfocus is set at l ~ 130.
     Number of events Nevent is defined at l ~ 85.
     Wavelength of the laser Llambda at l ~ 120.
     
     Pointers for all variables are declared in the main.
     Optics; Aside from the positions for the lattice components,
     optical parameters at points all along the interaction region
     are needed for electrons may interact at any point.
     Beam specification; energy0, energy spread, overvoltage_ratio
     */
}
int EleTracer::tagger(int ievent, int nbmpcl, int nrejec, double *XSelec, double *PSelec, FILE *logstr){
    // Tagging and triggering
    //******************************
    //if(strstr(cntl_event,"Tagging_ON")!=0){
    //            ioffset = cntl_event.find("Tagging_ON");
    //            if(ioffset!=std::string::npos){
        /*  ----------------------------------------    */
        if(ievent == 1 && nrejec == 0){
            fprintf(logstr,"N...main: Tags1 = %f, Tags2 = %f\n",
                    Tags1/plnck::meter, Tags2/plnck::meter);
            fprintf(logstr,"N...main: BRz1  = %f, BRz2  = %f\n", BRz1, BRz2);
        }//ievent == 1 && nrejec == 0
        int iftag = 0;
        double s = *(XSelec+0);
        if( Tags1/plnck::meter <= s && s <= Tags2/plnck::meter ){
            
            double z = *(XSelec+3);
            /* differences between s and z of particles may affect */
            if( z < BRz1/plnck::meter  ){
                /* rotate back to the B frame */
                fprintf(logstr,"W...main: Tag| z < BRz1: z=%e, s=%e\n",z,s);
                
            } /* z < BRz1 */
            if( BRz2/plnck::meter < z ){
                /* rotate back to the B frame */
                fprintf(logstr,"W...main: Tag| z > BRz2: z=%e, s=%e\n",z,s);
                
                
            } /* BRz2 < z */
            
            
            get_tagger(logstr, XSelec,PSelec,nbmpcl,&iftag );
            
            
        } /* s in [Tags1, Tags2] *///Tags1/plnck::meter <= s && s <= Tags2/plnck::meter
        
        /*    Trigger events for electrons on the tagger with xtag1 < xetag <  xtag2 [m]   */
        if( iftag == 1 ){
            double xetag = *(XSelec+1);
            if( xetag < (Tagx0-Tagxd/2.)/plnck::meter ||
               xetag > (Tagx0+Tagxd/2.)/plnck::meter ){
                iftag = 0;
            }
        }//iftag == 1
        
        //if( strstr(cntl_event,"Tagtrig-on")!=0 && iftag!=1 ){
        //            int ioffset2 = cntl_event.find("Tagtrig-on");
        //            if(ioffset!=std::string::npos && iftag!=1 ){
        if(cntl_event.find("Tagtrig-on")!=std::string::npos && iftag!=1 ){
            ++nrejec;
            --ievent;
            return -1;
//            continue;   /* jump to the next event loop */
        } /* Tagtrig-on */
        
    
    //********** End of tagging and triggering *********
    return 0;
}



















