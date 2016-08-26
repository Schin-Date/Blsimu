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


/******************************************************************************/
#if(0)
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

/* pointers to optical parameters at points along the region */

/* random number generator */
int randseed;
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

    
/* Event loop <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
    <<<<<<<<<<<<<<<

    
      
      
#if(0)
#endif
    
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

}/*========================<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/


#endif

