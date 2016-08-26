#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<math.h>
#include	<time.h>
#include    <const.h>

/*
---------------------------------------------below is v1.2
070108: section of back rotation of axis just after Bending
061115: Lsstart and Lsend introduced
061016: read Useinp.txt in for input parameters
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
#define MAXNPR 128 
#define LPNAM 32
/*============================================================================*/
void main(){
/* parameters to controll simulation */
char cmntline[256];
int nbmpcl, Nevent;
double smin, smax, slastelm; /* defining the maximum region of the tracking and the last element*/
/* event control integers and strings */
char cntl_event[128];
/* pointers to the basic beam paramaters such as emittance, energy spread and overvoltage ratio.
/** electron beam parameters **/
char cntl_ebeam[128];
double pel0, e0spread, OVRVLR, emittance_x, emittance_y;
double CHMBDX, CHMBDY;
/** laser parameters **/
double Llambda, Lwaist, Lfocus, Elaser;
double Lsstart, Lsend;
char Lprof[32];
/** detector **/
double dtoscr; /* distance from B2 to tagger screen in meters */
/* pointers to lattice components */
/* pointers to electron momenta and positions */ /* interaction points */
double *Pbmpcl, *Xbmpcl;
/* pointers to produced gamma momenta */
double *Photon;
/* pointers to the secondary electrons and photons */ /* positions on detector */
double *PSelec, *XSelec, *PSphot;

/* pointers to optical parameters at points along the region */

/* random number generator */
int randseed;
/* timesptamp */
time_t time_pointer;
/* files */
FILE *logstr,*instr,*outstr;
char optfile[80], latfile[80], outfile[80], tagger[80];
/* variables */
int status, i,j,k,l,m,n,ievent, nrejec;
double x,y,z,r,a,c,s,t,u;
/* double xetag, xtag1, xtag2; */   /* electron position on the tagger */
double xetag, Tagx0, Tagxd;
double Tags1, Tags2, BRz1, BRz2;
int iftag;
double  getrandom(); 
void    setrandom(int);
/*
    define integer, real and string parameters ipar, rpar and spar, respectively,
  with dimension MAXNPR for the maximum number of the parameters.
 *********************************************************************/
/*----- parameter reading --------*/
char pname[MAXNPR][LPNAM];
int ipar[MAXNPR];
double rpar[MAXNPR];
char **spar;
int nipar, nrpar, nspar; 
/* files */
char inpfile[80];
FILE *paraminstr;



/* defaults */
     logstr=stdout;
/* timesptamp */
      time(&time_pointer);
      fprintf(logstr,"present time = %.24s\n",ctime(&time_pointer));



/*** set a file for input parameters ***/
         strcpy(inpfile,"Useinp.txt");
#ifdef DEBUG_FILE
	printf("D_FILE...main:inpfile:'%s'\n",inpfile);
#endif
/* define string variables */
      spar = calloc(MAXNPR,sizeof(char*));
      for(i=0;i<MAXNPR;i++){
         *(spar+i) = calloc(256,sizeof(char));
      }
/* reset all types of variables */
      for(i=0;i<MAXNPR;i++){
         strcpy(pname[i],""); ipar[i]=0; rpar[i]=0.; }

 
/*--------  Define a series of variable names  --------*/
      i = 0;
      strcpy(pname[i],"int nbmpcl");  i++;
      strcpy(pname[i],"int Nevent");  i++;
      strcpy(pname[i],"double smin");  i++;
      strcpy(pname[i],"double smax");  i++;
      strcpy(pname[i],"double slastelm");  i++;
      strcpy(pname[i],"double pel0");  i++;
      strcpy(pname[i],"double e0spread");  i++;
      strcpy(pname[i],"double OVRVLR");  i++;
      strcpy(pname[i],"double CHMBDX");  i++;
      strcpy(pname[i],"double CHMBDY");  i++;
      strcpy(pname[i],"double emittance_x");  i++;
      strcpy(pname[i],"double emittance_y");  i++;
      strcpy(pname[i],"char optfile");  i++;
      strcpy(pname[i],"char latfile");  i++;
      strcpy(pname[i],"double Llambda");  i++;
      strcpy(pname[i],"double Lwaist");  i++;
      strcpy(pname[i],"double Lsstart");  i++;
      strcpy(pname[i],"double Lsend");  i++;
      strcpy(pname[i],"double Lfocus");  i++;
      strcpy(pname[i],"char Lprof");  i++;
      strcpy(pname[i],"char cntl_ebeam");  i++;
      strcpy(pname[i],"char outfile");  i++;
      strcpy(pname[i],"double Tags1");  i++;
      strcpy(pname[i],"double Tags2");  i++;
      strcpy(pname[i],"double BRz1");  i++;
      strcpy(pname[i],"double BRz2");  i++;
/*      strcpy(pname[i],"double dtoscr");  i++; */
      strcpy(pname[i],"char tagger");  i++;
      strcpy(pname[i],"double Tagx0");  i++;
      strcpy(pname[i],"double Tagxd");  i++;  
      strcpy(pname[i],"int randseed");  i++;
      strcpy(pname[i],"char cntl_event");  i++;
      strcpy(pname[i],"char cmntline");  i++;




#ifdef DEBUG_FILE
	printf("D_FILE...main:going to readinp\n");
#endif
      readinp(logstr, inpfile, pname, ipar, rpar, spar); 
/*         ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~   */

/*    printf("........here<<<<<<<<<<\n");*/
    /*    exit(0);*/
    /**************************************/

      for(i=0;i<MAXNPR;i++ ){

#if(0)
       the following is a hint for the advanced way 
        char *v, *a, vrbl[28], vtyp[8], buffer[240]; 
            if( strlen(pname[i])==0 ){continue;}
            strcpy(buffer,pname[i]);
            a = strtok(buffer," ");
            strcpy(vtyp, a);
            a = strtok(NULL," ");
            strcpy(vrbl,a);
       printf("D...main: %d= '%s', '%s', '%s'\n",
                 i,pname[i],vtyp,vrbl);    
        if(strcmp(vtyp,"int")=0 && strcmp(vrbl,"nbmpcl")==0){ 
                nbmpcl = ipar[i];       }

#endif

      if(strcmp(pname[i],"int nbmpcl")==0){ nbmpcl=ipar[i]; }
      if(strcmp(pname[i],"int Nevent")==0){ Nevent=ipar[i];  }
      if(strcmp(pname[i],"double smin")==0){ smin=rpar[i]; }
      if(strcmp(pname[i],"double smax")==0){ smax=rpar[i];  }
      if(strcmp(pname[i],"double slastelm")==0){ slastelm=rpar[i];  }
      if(strcmp(pname[i],"double pel0")==0){ pel0=rpar[i];  }         
      if(strcmp(pname[i],"double e0spread")==0){ e0spread=rpar[i];  }
      if(strcmp(pname[i],"double OVRVLR")==0){ OVRVLR=rpar[i];  }
      if(strcmp(pname[i],"double CHMBDX")==0){ CHMBDX=rpar[i];  }
      if(strcmp(pname[i],"double CHMBDY")==0){ CHMBDY=rpar[i];  }
      if(strcmp(pname[i],"double emittance_x")==0){ emittance_x=rpar[i];  }
      if(strcmp(pname[i],"double emittance_y")==0){ emittance_y=rpar[i];  }
      if(strcmp(pname[i],"char optfile")==0){ strcpy(optfile,spar[i]);  }
      if(strcmp(pname[i],"char latfile")==0){ strcpy(latfile,spar[i]);  }
      if(strcmp(pname[i],"double Llambda")==0){ Llambda=rpar[i];  }
      if(strcmp(pname[i],"double Lwaist")==0){ Lwaist=rpar[i];  }
      if(strcmp(pname[i],"double Lsstart")==0){ Lsstart=rpar[i];  }
      if(strcmp(pname[i],"double Lsend")==0){ Lsend=rpar[i];  }
      if(strcmp(pname[i],"double Lfocus")==0){ Lfocus=rpar[i];  }
      if(strcmp(pname[i],"char Lprof")==0){ strcpy(Lprof,spar[i]);  }
      if(strcmp(pname[i],"char cntl_ebeam")==0){ strcpy(cntl_ebeam,spar[i]);  }
      if(strcmp(pname[i],"char outfile")==0){ strcpy(outfile,spar[i]);  }
      if(strcmp(pname[i],"double Tags1")==0){ Tags1=rpar[i];  }
      if(strcmp(pname[i],"double Tags2")==0){ Tags2=rpar[i];  }
      if(strcmp(pname[i],"double BRz1")==0){ BRz1=rpar[i];  }
      if(strcmp(pname[i],"double BRz2")==0){ BRz2=rpar[i];  }
/*      if(strcmp(pname[i],"double dtoscr")==0){ dtoscr=rpar[i];  } */
      if(strcmp(pname[i],"char tagger")==0){ strcpy(tagger,spar[i]);  }
      if(strcmp(pname[i],"double Tagx0")==0){ Tagx0=rpar[i];  }
      if(strcmp(pname[i],"double Tagxd")==0){ Tagxd=rpar[i];  } 
      if(strcmp(pname[i],"int randseed")==0){ randseed=ipar[i];  }                
      if(strcmp(pname[i],"char cntl_event")==0){ strcpy(cntl_event,spar[i]);  }
      if(strcmp(pname[i],"char cmntline")==0){ strcpy(cmntline,spar[i]);  }

     }
         fprintf(logstr,"N...main: %s\n",cmntline);

/* induced parameters */
         Elaser = (WL2E / Llambda) *eV;  /* Laser energy */
         fprintf(logstr,"N...Elaser = %7.2e eV\n",Elaser/eV);

#ifdef DEBUG_FILE
	printf("D_FILE...main:optfile='%s'\n",optfile);
	printf("D_FILE...main:going to read_opt\n");
#endif
         read_opt(logstr,optfile,smin,smax);              
#ifdef DEBUG_FILE
	printf("D_FILE...main:latfile='%s'\n",latfile);
	printf("D_FILE...main:going to read_opt\n");
#endif
         status = read_lattice(logstr,latfile,smin,smax);              
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
         set_Lprof(logstr, Llambda, Lwaist, Lfocus, 966.3227, 974.0913); /* 33B1 exit to B2 entr */
#endif
         set_Lprof(logstr, Llambda, Lwaist, Lfocus, Lsstart, Lsend); 

/** detector **/ 
#ifdef DEBUG_FILE
	printf("D_FILE...main:tagger='%s'\n",tagger);
	printf("D_FILE...main:going to read_tagger\n");
#endif
         read_tagger(logstr,tagger);              
/* Data accumulator initialization */
/* Read cell components */
/* Read optical parameters at points all along the region */
/** prepare for the optics parameters at arbitral point **/

/* Allocating beam particle variables */
           Xbmpcl = calloc(nbmpcl*4,sizeof(double));              
           Pbmpcl = calloc(nbmpcl*4,sizeof(double));       
              #ifdef DEBUG1 
              fprintf(logstr,"D1...main: passed X and Pbmpcl allocation\n");
              #endif       
/* Allocating photon momenta */
           Photon = calloc(nbmpcl*4,sizeof(double));       
/* Allocating secondaries */
           XSelec = calloc(nbmpcl*4,sizeof(double));              
           PSelec = calloc(nbmpcl*4,sizeof(double));       
           PSphot = calloc(nbmpcl*4,sizeof(double));       

/* Random number generator: initialization */
           randseed = 566387; setrandom(randseed);
              #ifdef DEBUG12
              x = getrandom();
              fprintf(logstr,"D12...main: 1st called random = %f seed = %d\n",x,randseed);
                     #ifdef DEBUG13
                         for(i=0;i<100;i++)
                            {r = getrandom(); printf("D13...i = %d,  r = %f\n",i,r);
                         }
                     #endif    
              #endif

/* Event loop <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
ievent = 0; nrejec = 0;
  while(ievent<Nevent){  ++ievent;
/* generate a number of electrons */
/** interaction points according to laser profile along the line **/
/** choose the way to distribute interaction points **/
            scat_points(logstr, Xbmpcl, nbmpcl, Lprof); 
/*            scat_points(logstr, Xbmpcl, nbmpcl, "distributed");*/ 
/*            scat_points(logstr, Xbmpcl, nbmpcl, "focalpoint");*/
/*            if(ievent == 1 && nrejec == 0)
            fprintf(logstr,"N...int. pt. = dist. around %f out:%s\n",Lfocus,outfile);*/
/*            fprintf(logstr,"N...int. pt. = focus at %f out:%s\n",Lfocus,outfile);*/

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
                           emittance_x, emittance_y,CHMBDX,CHMBDY,
                           pel0, e0spread, OVRVLR, cntl_ebeam);  /* 061002 */
                       /*    pel0, e0spread, OVRVLR, "incline,dispersion");*/
                       /*    pel0, e0spread, OVRVLR, "incline,nodispersion");*/
/** generate incident photons **/
/** make a room to introduce energy and angular spread of the lasar beam **/
            for( i = 0; i< nbmpcl; i++){
                   *(Photon + 4*i + 0) = Elaser;
                   *(Photon + 4*i + 1) = 0.;
                   *(Photon + 4*i + 2) = 0.;
                   *(Photon + 4*i + 3) = -Elaser;
               }
/* make Compton scsttering to generate final state  photons and electrons. */
            status = compton(logstr, nbmpcl, Pbmpcl, Photon, PSelec, PSphot," ");
/** accept only events with Egamma in a region **<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/ 
     if(strstr(cntl_event,"CutEg-on")!=0){
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
            for(i=0;i<nbmpcl;i++){
             for(k=0;k<4;k++){ *(XSelec + 4*i + k) = *(Xbmpcl + 4*i + k); } }
/* For each scattering point, find the transfer matrix */ /*>>>>>>>>>>>> review notes *//*>>> track.c */
            track_lattice(logstr, XSelec, PSelec, nbmpcl, CHMBDX, CHMBDY, slastelm, pel0 );
/*      ------------------------------------------------------------------------------------  */
#if(0)
070417
The coordinate system for XSelec and PSelec are defined in the function
track_lattice. Old difinitions with rotation of the coordinates followed
in this section was documented in Museum/main.c070417.
=======================================================================
#endif
/*     if(strstr(cntl_event,"Tagele-on")!=0){  */
     if(strstr(cntl_event,"Tagger-on")!=0){
/*  ----------------------------------------    */
                 if(ievent == 1 && nrejec == 0){
                fprintf(logstr,"N...main: Tags1 = %f, Tags2 = %f\n", Tags1, Tags2);
                fprintf(logstr,"N...main: BRz1  = %f, BRz2  = %f\n", BRz1, BRz2);
                    }
      iftag = 0;
      s = *(XSelec+0);
      if( Tags1 <= s && s <= Tags2 ){

        z = *(XSelec+3);
   /* differences between s and z of particles may affect */
        if( z < BRz1  ){
      /* rotate back to the B frame */
        fprintf(logstr,"W...main: Tag| z < BRz1: z=%e, s=%e\n",z,s);

        } /* z < BRz1 */
        if( BRz2 < z ){
      /* rotate back to the B frame */
        fprintf(logstr,"W...main: Tag| z > BRz2: z=%e, s=%e\n",z,s);


        } /* BRz2 < z */


      get_tagger(logstr, XSelec,PSelec,nbmpcl,&iftag );


      } /* s in [Tags1, Tags2] */ 

/*    Trigger events for electrons on the tagger with xtag1 < xetag <  xtag2 [m]   */
     if( iftag == 1 ){
         xetag = *(XSelec+1);
         if( xetag < Tagx0-Tagxd/2. || xetag > Tagx0+Tagxd/2. ){
              iftag = 0;
         } 
     }

      if( strstr(cntl_event,"Tagtrig-on")!=0 && iftag!=1 ){
                    ++nrejec;
                    --ievent;
                    continue;   /* jump to the next event loop */
        } /* Tagtrig-on */

       }/* Tagger-on */
 
                        
/* Evaluate the final results */
/* End of the event looop */ 
             #ifdef DEBUG3
              fprintf(logstr,"D3...main: finished ievent=%d\n",ievent);
            #endif
/* Print out the results onto a output file */

           outstr = fopen(outfile,"a"); 
     for(i=0;i<nbmpcl;i++){
           fprintf(outstr,"%e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e\n",
                  *(Xbmpcl+4*i+0),*(Xbmpcl+4*i+1),*(Xbmpcl+4*i+2),*(Xbmpcl+4*i+3),
                  *(Pbmpcl+4*i+0),*(Pbmpcl+4*i+1),*(Pbmpcl+4*i+2),*(Pbmpcl+4*i+3),
                  *(XSelec+4*i+0),*(XSelec+4*i+1),*(XSelec+4*i+2),*(XSelec+4*i+3),
                  *(PSelec+4*i+0),*(PSelec+4*i+1),*(PSelec+4*i+2),*(PSelec+4*i+3),
                  *(PSphot+4*i+0),*(PSphot+4*i+1),*(PSphot+4*i+2),*(PSphot+4*i+3)
                  );
                          }
            fclose(outstr);
      /*      nrejec = 0;  suppressed on 060728 */
} /*<<<<<< event loop */
       fprintf(logstr,"N...main: Nevent=%d nrejec=%d nbmpcl=%d\n",Nevent, nrejec, nbmpcl);

}

/* Call for the accumulator */
/* Store events into a ntuple */
/* A routine for Data accumulatoin */
/* Take statistics for the accumulated data */       
/** static pointers to accumulation **/
/** statistcs on demand thru a string control **/

/* A function to pass generated data to fortran function to store in a ntuple */  

/* Develop a routine to read event.hbook and take ststistics */

