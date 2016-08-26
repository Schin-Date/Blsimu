#include <iostream>
#include <iomanip>
#include <cstdlib> //exit()
#include <ctime>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include <ctime>
#include "witgn.h"
#include "plnck.h"
#include "main.h"

//#include	<stdio.h>
//#include	<stdlib.h>
//#include	<string.h>
//#include	<math.h>
//#include	<time.h>
//#include    <const.h>

/*
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

/*                 main for blsimu+
 =========================================================
 141021                                                       */
//double R4OVER = 1.6e38;
//double R4UNDR = .3E-38;

main(int argn,char* argv[]) {
    using namespace std;
    
//    string doprmfilename = "./main.doprm";//the default doprmfile
    std::string  thisDir = getenv("blsimu_SRCDIR");
    std::string doprmfilename = thisDir+"main.doprm";
//NG    std::string doprmfilename = getenv("blsimu_SRCDIR")+"main.doprm";
    //+"./main.doprm";//the default doprmfile
    
    if (1<argn) {
        std::string buff = *(argv+1);
        if (6<buff.length()) {
            doprmfilename = buff;
        }
    }
    
    witgn::SetDoPrm mainprm(doprmfilename.c_str());
    
//    double R4OVER = mainprm.getValue<double>("R4OVER");
//    double R4UNDR = mainprm.getValue<double>("R4UNDR");
//    METER = plnck::meter;
    EMASS = plnck::EMASS;
    R4OVER = mainprm.getValue<double>("R4OVER");
    R4UNDR = mainprm.getValue<double>("R4UNDR");
    int MAXNPR = mainprm.getValue<int>("MAXNPR");
    int LPNAM = mainprm.getValue<int>("LPNAM");
    std::cout<<" mainprm: "<<MAXNPR<<", "<<LPNAM<<std::endl;
    
    time_t time_pointer;
    time(&time_pointer);
    std::cout<<"...main::time = "<<ctime(&time_pointer);

    witgn::SetDoPrm blsimu("blsimu.doprm");
//    witgn::SetDoPrm blsimu("blsimu.doprm");
//    int PrtLvl = blsimu.GetPrtLvl();
//    blsimu.SetPrtLvl(3);
//    blsimu.SetPrtLvl(PrtLvl);

    /* prepare for an output file */
//    std::ofstream ofs("main.out");
    std::ofstream ofs(blsimu.getValue<std::string>("outfile").c_str());
    ofs<<"time = "<<ctime(&time_pointer);
    //    ofs.setf(std::ios_base::fixed,std::ios_base::floatfield);
    ofs.setf(std::ios_base::scientific,std::ios_base::floatfield);
    //    ofs.precision(8);
    ofs.precision(6);
    ofs.width(12);
    
    
    
    /* induced parameters */
    double Llambda = blsimu.getValue<double>("Llambda")*plnck::nm;
//    double WL2E = plnck::hc/plnck::eV/plnck::nm;
    //#define WL2E   1239.84544 /* 2PAI*hbar*c [eV*nm] */
//    double Elaser = (WL2E / Llambda);  /* Laser energy */
    double Elaser = plnck::hc / Llambda;  /* Laser energy */
    std::cout<<"...main::Elaser = "<<Elaser/plnck::eV<<" eV"<<std::endl;

    //optfile
//    string optfile = thisDir+blsimu.getValue<string>("optfile");
    string optfile = blsimu.getValue<string>("optfile");
    double smin = blsimu.getValue<double>("smin")*plnck::meter;
    double smax = blsimu.getValue<double>("smax")*plnck::meter;
//    double R4OVER = mainprm.getValue<double>("R4OVER");
    std::cout<<"...main::going to read "<<optfile
    <<"\n                  << (smin, smax) = ("
    <<smin/plnck::meter<<", "<<smax/plnck::meter<<") [m]"<<std::endl;
    //******** calling a c function optics.c::read_opt()*******/
    FILE *logstr = fopen(blsimu.getValue<std::string>("logfile").c_str(),"w");
        int status = read_opt(logstr,optfile.c_str(),smin/plnck::meter,smax/plnck::meter);
        //======== this is a c function =========================
    //latfile
//    string latfile = thisDir+blsimu.getValue<string>("latfile");
    string latfile = blsimu.getValue<string>("latfile");
    std::cout<<"...main::going to read "<<latfile
    <<"\n                  << (smin, smax) = ("
    <<smin/plnck::meter<<", "<<smax/plnck::meter<<") [m]"<<std::endl;
        status = read_lattice(logstr,latfile.c_str(),smin/plnck::meter,smax/plnck::meter);
    //======== this is a c function =========================
    //Laser profile
#ifdef DEBUG
    /*061115 */
    fprintf(logstr,"D...Llambda = %6.1f nm (Elaser = %7.2e eV) Lwaist = %4.2f mm\n",
            Llambda,Elaser/eV,Lwaist);
    fprintf(logstr,"D...Laser profile: %12s around s=%6.1f [m]\n",Lprof, Lfocus);
    fprintf(logstr,"D...electron beam options: %s\n",cntl_ebeam);
    fprintf(logstr,"D...outfile: %s\n",outfile);
#endif
#if(0)
    061115   set_Lprof(logstr, Llambda, Lwaist, Lfocus, 857.2329, 903.6875); /* 29B2 exit to 31B1 entr */
    set_Lprof(logstr, Llambda/plnck::nm, Lwaist, Lfocus, 966.3227, 974.0913); /* 33B1 exit to B2 entr */
#endif
    double Lwaist = blsimu.getValue<double>("Lwaist")*plnck::mm;
    double Lfocus = blsimu.getValue<double>("Lfocus")*plnck::meter;
    double Lsstart = blsimu.getValue<double>("Lsstart")*plnck::meter;
    double Lsend = blsimu.getValue<double>("Lsend")*plnck::meter;
    set_Lprof(logstr, Llambda/plnck::nm, Lwaist/plnck::mm, Lfocus/plnck::meter,
              Lsstart/plnck::meter, Lsend/plnck::meter);

    /** detector **/
#ifdef DEBUG_FILE
    printf("D_FILE...main:tagger='%s'\n",tagger);
    printf("D_FILE...main:going to read_tagger\n");
#endif
    std::string tagger = thisDir+blsimu.getValue<string>("tagger");
    read_tagger(logstr,tagger.c_str());

    /* Data accumulator initialization */
    /* Read cell components */
    /* Read optical parameters at points all along the region */
    /** prepare for the optics parameters at arbitral point **/
    
    /* Allocating beam particle variables */
    int nbmpcl = blsimu.getValue<int>("nbmpcl");
    /* electron momenta and positions at interaction points */
//    double *Pbmpcl, *Xbmpcl;
//    Xbmpcl = calloc(nbmpcl*4,sizeof(double));
//    Pbmpcl = calloc(nbmpcl*4,sizeof(double));
    double Pbmpcl[nbmpcl*4];
    double Xbmpcl[nbmpcl*4];

    /* produced gamma momenta */
//    Photon = calloc(nbmpcl*4,sizeof(double));
    double Photon[nbmpcl*4];

    /* secondary electrons and photons at positions on detector */
//    XSelec = calloc(nbmpcl*4,sizeof(double));
//    PSelec = calloc(nbmpcl*4,sizeof(double));
//    PSphot = calloc(nbmpcl*4,sizeof(double));
    double XSelec[nbmpcl*4];
    double PSelec[nbmpcl*4];
    double PSphot[nbmpcl*4];

    /* Random number generator: initialization */
    int randseed = blsimu.getValue<int>("randseed");
    setrandom(randseed);
    std::cout<<"...main:: randseed = "<<randseed<<std::endl;
    ofs<<"     randseed = "<<randseed<<";"<<std::endl;
#ifdef DEBUG12
    x = getrandom();
    fprintf(logstr,"D12...main: 1st called random = %f seed = %d\n",x,randseed);
#ifdef DEBUG13
    for(i=0;i<100;i++)
    {r = getrandom(); printf("D13...i = %d,  r = %f\n",i,r);
    }
#endif
#endif

    //settings for event generation
    //Laser profile
    std::string Lprof = blsimu.getValue<string>("Lprof");
    //electron beam phase space distribution
    std::string cntl_ebeam = blsimu.getValue<string>("cntl_ebeam");
    //event control
    std::string cntl_event = blsimu.getValue<string>("cntl_event");
    //position of the last element of lattice
    double slastelm = blsimu.getValue<double>("slastelm")*plnck::meter;
    //Tagging parameters
    double Tags1 = blsimu.getValue<double>("Tags1")*plnck::meter;
    double Tags2 = blsimu.getValue<double>("Tags2")*plnck::meter;
    double BRz1 = blsimu.getValue<double>("BRz1")*plnck::meter;
    double BRz2 = blsimu.getValue<double>("BRz2")*plnck::meter;
    double Tagx0 = blsimu.getValue<double>("Tagx0")*plnck::meter;
    double Tagxd = blsimu.getValue<double>("Tagxd")*plnck::meter;
    //e-beam parameters
    double pel0 = blsimu.getValue<double>("pel0")*plnck::MeV;
    double e0spread = blsimu.getValue<double>("e0spread");
    double emittance_x = blsimu.getValue<double>("emittance_x")
                                *plnck::meter*plnck::radian;
    double emittance_y = blsimu.getValue<double>("emittance_y")
                                *plnck::meter*plnck::radian;
    //RF parameter
    double OVRVLR = blsimu.getValue<double>("OVRVLR");
    //chamber physical apertures
    double CHMBDX = blsimu.getValue<double>("CHMBDX")*plnck::meter;
    double CHMBDY = blsimu.getValue<double>("CHMBDY")*plnck::meter;
    //
    
    //<<<<<<<<<<<<<<<<<<<<<< Event Loop <<<<<<<<<<<<<<<<<<<<<<
    int Nevent = blsimu.getValue<double>("Nevent");
    int ievent = 0;
    int nrejec = 0;
    while(ievent<Nevent){  ++ievent;
        /* generate a number of electrons */
        /** interaction points according to laser profile along the line **/
        /** choose the way to distribute interaction points **/
    
        //set nbmpcl scattering points
        scat_points(logstr, Xbmpcl, nbmpcl, Lprof.c_str());
#ifdef DEBUG14
        if( ievent ==1 ){
            for(i=0;i<2;i++){
                fprintf(logstr,"D14...%d %f\n",i,*(Xbmpcl + 4*i +3)); }
            fprintf(logstr,"D14........\n");
            for(i=nbmpcl-2;i<nbmpcl;i++){
                fprintf(logstr,"D14...%d %f\n",i,*(Xbmpcl + 4*i +3)); }
        }
#endif

        /** generate the transverse coordinates and momenta of electrons according to its s-position **/
        status= bmgen(logstr,Xbmpcl,Pbmpcl, nbmpcl,
                      emittance_x/plnck::meter/plnck::radian,
                      emittance_y/plnck::meter/plnck::radian,
                      CHMBDX/plnck::meter,CHMBDY/plnck::meter,
                      pel0/plnck::MeV, e0spread, OVRVLR, cntl_ebeam.c_str());  /* 061002 */
        /*    pel0, e0spread, OVRVLR, "incline,dispersion");*/
        /*    pel0, e0spread, OVRVLR, "incline,nodispersion");*/
//        std::cout<<"<<<<<<<<<<<<<<<<<<<here <<<<<<<<<<<<<<<"<<std::endl;
        
        /** generate incident photons **/
        /** make a room to introduce energy and angular spread of the lasar beam **/
        for( int i = 0; i< nbmpcl; i++){
            *(Photon + 4*i + 0) = Elaser/plnck::MeV;
            *(Photon + 4*i + 1) = 0.;
            *(Photon + 4*i + 2) = 0.;
            *(Photon + 4*i + 3) = -Elaser/plnck::MeV;
        }
        
        /* Compton scsttering to generate final state  photons and electrons. */
        status = compton(logstr, nbmpcl, Pbmpcl, Photon, PSelec, PSphot," ");

        /** accept only events with Egamma in a region **<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
        //if(strstr(cntl_event,"CutEg-on")!=0){
        int ioffset = cntl_event.find("CutEg-on");
        if(ioffset!=std::string::npos){
            /*         -------------------------------    */
            /*#ifdef CutEg */
            if(*(PSphot) > 2400. || *(PSphot) < 2200. ){
                ++nrejec;
                --ievent;
                continue;
            }
            /* #endif */
        }

        /** set starting position for the final state electron **/
        for(int i=0;i<nbmpcl;i++){
            for(int k=0;k<4;k++){ *(XSelec + 4*i + k) = *(Xbmpcl + 4*i + k); } }

        /* For each scattering point, find the transfer matrix */ /*>>>>>>>>>>>> review notes *//*>>> track.c */
        track_lattice(logstr, XSelec, PSelec, nbmpcl, CHMBDX, CHMBDY, slastelm/plnck::meter, pel0 );
        /*      ------------------------------------------------------------------------------------  */

        /*070417
        The coordinate system for XSelec and PSelec are defined in the function
        track_lattice. Old difinitions with rotation of the coordinates followed
        in this section was documented in Museum/main.c070417.
        =======================================================================*/
        // Tagging and triggering
        //******************************
        //if(strstr(cntl_event,"Tagger-on")!=0){
            ioffset = cntl_event.find("Tagger-on");
            if(ioffset!=std::string::npos){
            /*  ----------------------------------------    */
            if(ievent == 1 && nrejec == 0){
                fprintf(logstr,"N...main: Tags1 = %f, Tags2 = %f\n",
                        Tags1/plnck::meter, Tags2/plnck::meter);
                fprintf(logstr,"N...main: BRz1  = %f, BRz2  = %f\n", BRz1, BRz2);
            }
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
                
                
            } /* s in [Tags1, Tags2] */
            
            /*    Trigger events for electrons on the tagger with xtag1 < xetag <  xtag2 [m]   */
            if( iftag == 1 ){
                double xetag = *(XSelec+1);
                if( xetag < (Tagx0-Tagxd/2.)/plnck::meter ||
                   xetag > (Tagx0+Tagxd/2.)/plnck::meter ){
                    iftag = 0;
                } 
            }
            
            //if( strstr(cntl_event,"Tagtrig-on")!=0 && iftag!=1 ){
            int ioffset2 = cntl_event.find("Tagtrig-on");
            if(ioffset!=std::string::npos && iftag!=1 ){
                ++nrejec;
                --ievent;
                continue;   /* jump to the next event loop */
            } /* Tagtrig-on */
            
        }/* Tagger-on */
        
        //********** End of tagging and triggering *********

        /* Evaluate relevant quantities here if you want*/
        /* End of the event looop */
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

    std::cout<<"N...main: Finished(Nevent="<<Nevent
    <<", nrejec="<<nrejec
    <<", nbmpcl="<<nbmpcl
    <<")"<<std::endl;
    
    /* Call for the accumulator */
    /* Store events into a ntuple */
    /* A routine for Data accumulatoin */
    /* Take statistics for the accumulated data */
    /** static pointers to accumulation **/
    /** statistcs on demand thru a string control **/
    
    /* A function to pass generated data to fortran function to store in a ntuple */
    
    /* Develop a routine to read event.hbook and take ststistics */
    
}/* end of main*/
