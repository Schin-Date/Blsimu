#include <iostream>
#include <iomanip>
#include <cstdlib> //exit()
#include <ctime>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include <ctime>
//#include "witpy.h"
#include "witpy.h"
#include "plnck.h"
#include "Blsimu.h"
#include "EleTracer.h"
#include "main.h"


witpy::SetDoPrm blsimuDoPrm("useBlsimu/blsimu.doprm"); //doprm for the simulator
//---------------------------------------
double EMASS = plnck::EMASS;
double R4OVER = blsimuDoPrm.getValue<double>("R4OVER");
double R4UNDR = blsimuDoPrm.getValue<double>("R4UNDR");



Blsimu::Blsimu(std::string mainDoPrm_s){

    
    std::cout << "D"
    <<"...Blsimu("
    <<"): instantiated."<<std::endl;
    
    if (3<witpy::SetDoPrm::GetPrtLvl()) std::cout << "N4"
        <<"...Blsimu("
        <<"): instantiated."<<std::endl;

    
    //    witpy::SetDoPrm mainprm(doprmfilename.c_str());
    //-----------------------------------------------
    //    R4OVER = mainprm.getValue<double>("R4OVER");
    //    R4UNDR = mainprm.getValue<double>("R4UNDR");
    //    int MAXNPR = mainprm.getValue<int>("MAXNPR");
    //    int LPNAM = mainprm.getValue<int>("LPNAM");
//    witpy::SetDoPrm blsimuDoPrm("useBlsimu/blsimu.doprm"); //doprm for the simulator
    //---------------------------------------
//    EMASS = plnck::EMASS;
//    R4OVER = blsimuDoPrm.getValue<double>("R4OVER");
//    R4UNDR = blsimuDoPrm.getValue<double>("R4UNDR");
    //unused int MAXNPR = blsimuDoPrm.getValue<int>("MAXNPR");
    //unused int LPNAM = blsimuDoPrm.getValue<int>("LPNAM");
    
    time_t time_pointer;
    time(&time_pointer);
    std::cout<<"N...Blsimu/main::time = "<<ctime(&time_pointer);

    /* prepare for output files */
    //-----------------------------
    std::ofstream ofs(blsimuDoPrm.getValue<std::string>("outfile").c_str());
    ofs<<"time = "<<ctime(&time_pointer);
    ofs.setf(std::ios_base::scientific,std::ios_base::floatfield);
    ofs.precision(6);
    ofs.width(12);
    FILE *logstr = fopen(blsimuDoPrm.getValue<std::string>("logfile").c_str(),"w");
            //log file used in C functions
    
    //global event controller
    //--------------------------
    std::string cntl_event = blsimuDoPrm.getValue<std::string>("cntl_event");

    /* Laser light parameters */
    //--------------------------
    double Llambda = blsimuDoPrm.getValue<double>("Llambda")*plnck::nm;
    double Elaser = plnck::hc / Llambda;  /* Laser energy */
    std::cout<<"N...Blsimu/main::Elaser = "<<Elaser/plnck::eV<<" eV"<<std::endl;
    //Laser profile parameters
    double Lwaist = blsimuDoPrm.getValue<double>("Lwaist")*plnck::mm;
    double Lfocus = blsimuDoPrm.getValue<double>("Lfocus")*plnck::meter;
    double Lsstart = blsimuDoPrm.getValue<double>("Lsstart")*plnck::meter;
    double Lsend = blsimuDoPrm.getValue<double>("Lsend")*plnck::meter;
    int status = set_Lprof(logstr, Llambda/plnck::nm, Lwaist/plnck::mm, Lfocus/plnck::meter,
                       Lsstart/plnck::meter, Lsend/plnck::meter);
        //======== this is a c function =========================
    
    //parameters of electron beam
    //--------------------------------
    //e-beam parameters
    double pel0 = blsimuDoPrm.getValue<double>("pel0")*plnck::MeV;
    double e0spread = blsimuDoPrm.getValue<double>("e0spread");
    double emittance_x = blsimuDoPrm.getValue<double>("emittance_x")
    *plnck::meter*plnck::radian;
    double emittance_y = blsimuDoPrm.getValue<double>("emittance_y")
    *plnck::meter*plnck::radian;
    //RF parameter
    double OVRVLR = blsimuDoPrm.getValue<double>("OVRVLR");
    //chamber physical apertures
    double CHMBDX = blsimuDoPrm.getValue<double>("CHMBDX")*plnck::meter;
    double CHMBDY = blsimuDoPrm.getValue<double>("CHMBDY")*plnck::meter;
    
    //set optical parameters for electron beam
    //------------------------------------------
    std::string optfile = blsimuDoPrm.getValue<std::string>("optfile");
    double smin = blsimuDoPrm.getValue<double>("smin")*plnck::meter;
    double smax = blsimuDoPrm.getValue<double>("smax")*plnck::meter;
    std::cout<<"N...Blsimu/main::going to read "<<optfile
    <<" for a region (smin, smax) = ("
    <<smin/plnck::meter<<", "<<smax/plnck::meter<<") [m]"<<std::endl;
    status = read_opt(logstr,optfile.c_str(),smin/plnck::meter,smax/plnck::meter);
        //======== this is a c function =========================

    //prepare for simulation
    //--------------------------
    //--------------------------
    /* Random number generator: initialization */
    int randseed = blsimuDoPrm.getValue<int>("randseed");
    setrandom(randseed);
        //======== this is a c function =========================
    std::cout<<"N...Blsimu/main:: randseed = "<<randseed<<std::endl;
    ofs<<"randseed = "<<randseed<<";"<<std::endl;
    
    //way to generate laser profile
    std::string Lprof = blsimuDoPrm.getValue<std::string>("Lprof");
    //way to generate electron beam in the phase space
    std::string cntl_ebeam = blsimuDoPrm.getValue<std::string>("cntl_ebeam");
    //electron tracker
    EleTracer eleTracer;
    if(cntl_event.find("Tagging_ON")!=std::string::npos)
        eleTracer.setTracer(blsimuDoPrm,logstr);
    //---------------------------------------------------
    //position of the last element of lattice (to specify endpoint of tracking)
    double slastelm = blsimuDoPrm.getValue<double>("slastelm")*plnck::meter;
    
    /* Allocating variables for particles */
    //---------------------------------------
    int nbmpcl = blsimuDoPrm.getValue<int>("nbmpcl");//number of beam particle in a bunch
    /* incident electron momenta and positions at interaction points */
    double Pbmpcl[nbmpcl*4];
    double Xbmpcl[nbmpcl*4];
    // incident photons (laser)
    double Photon[nbmpcl*4];
    /* secondary electrons and photons at positions on detector */
    double XSelec[nbmpcl*4];
    double PSelec[nbmpcl*4];
    /* momenta of secondary photons (produced gamma)*/
    double PSphot[nbmpcl*4];

    
    //<<<<<<<<<<<<<<<<<<<<<< Event Loop <<<<<<<<<<<<<<<<<<<<<<
    int Nevent = blsimuDoPrm.getValue<double>("Nevent");
    int ievent = 0;
    int nrejec = 0;
    while(ievent<Nevent){  ++ievent;
        /* (generate a number of electrons, nbmpcl, in this bunch) */
        //set nbmpcl scattering points
        scat_points(logstr, Xbmpcl, nbmpcl, Lprof.c_str());

        /** generate incident electrons
            in the transverse phase space according to its s-position **/
        status= bmgen(logstr,Xbmpcl,Pbmpcl, nbmpcl,
                      emittance_x/plnck::meter/plnck::radian,
                      emittance_y/plnck::meter/plnck::radian,
                      CHMBDX/plnck::meter,CHMBDY/plnck::meter,
                      pel0/plnck::MeV, e0spread, OVRVLR, cntl_ebeam.c_str());
        
        /** generate incident photons **/
        /** angular spread of the lasar beam is currently set to zero **/
        for( int i = 0; i< nbmpcl; i++){
            *(Photon + 4*i + 0) = Elaser/plnck::MeV;
            *(Photon + 4*i + 1) = 0.;
            *(Photon + 4*i + 2) = 0.;
            *(Photon + 4*i + 3) = -Elaser/plnck::MeV;
        }
        
        /* Compton scsttering between nbmpcl incident electrons and photons
                            resulting in their final states.*/
        status = compton(logstr, nbmpcl, Pbmpcl, Photon, PSelec, PSphot," ");

        if(std::string::npos != cntl_event.find("EgCut_ON")){
            /** accept only events with Egamma in a region
             ----------------------------------------------*/
//            if(*(PSphot) > 2400. || *(PSphot) < 2200. ){
            if(*(PSphot) > blsimuDoPrm.getValue<double>("EgCutLow") ||
               *(PSphot) < blsimuDoPrm.getValue<double>("EgCutHi")  ){
                ++nrejec;
                --ievent;
                continue;
            }
        }//"EgCut_ON"

        //position of secondary electrons before tracking//
        for(int i=0;i<nbmpcl;i++){
            for(int k=0;k<4;k++){ *(XSelec + 4*i + k) = *(Xbmpcl + 4*i + k); } }

        //tracking of secondary electrons for tagging
        if(cntl_event.find("Tagging_ON")!=std::string::npos)
            eleTracer.track(&(XSelec[0]), &(PSelec[0]),nbmpcl,slastelm/plnck::meter,pel0,logstr);

        //detection of secondary electrons on the tagger
        if(cntl_event.find("Tagging_ON")!=std::string::npos){
            status = eleTracer.tagger(ievent, nbmpcl, nrejec, &(XSelec[0]), &(PSelec[0]), logstr);
            if (-1==status) continue;
        }
        
        /* Print out the results onto a output file */
        for(int i=0;i<nbmpcl;i++){
        ofs<<
           *(Xbmpcl+4*i+0)<<" "<<*(Xbmpcl+4*i+1)<<" "<<*(Xbmpcl+4*i+2)<<" "<<*(Xbmpcl+4*i+3)
    <<" "<<*(Pbmpcl+4*i+0)<<" "<<*(Pbmpcl+4*i+1)<<" "<<*(Pbmpcl+4*i+2)<<" "<<*(Pbmpcl+4*i+3)
    <<" "<<*(XSelec+4*i+0)<<" "<<*(XSelec+4*i+1)<<" "<<*(XSelec+4*i+2)<<" "<<*(XSelec+4*i+3)
    <<" "<<*(PSelec+4*i+0)<<" "<<*(PSelec+4*i+1)<<" "<<*(PSelec+4*i+2)<<" "<<*(PSelec+4*i+3)
    <<" "<<*(PSphot+4*i+0)<<" "<<*(PSphot+4*i+1)<<" "<<*(PSphot+4*i+2)<<" "<<*(PSphot+4*i+3)
    <<std::endl;
        }/*i loop*/
    } //>>>>>>>>>>>>>>>>>>> End of event loop >>>>>>>>>>>>>>>>>

    std::cout<<"N...Blsimu/main: Finished(Nevent="<<Nevent
    <<", nrejec="<<nrejec
    <<", nbmpcl="<<nbmpcl
    <<")"<<std::endl;
    
}/* end of main*/
