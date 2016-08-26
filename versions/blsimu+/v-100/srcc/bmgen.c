#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>
#include <const.h>
/*  Beam generator       based on tracking_f/bmgen.for      cre 050304
060710  local dispersion included 
===================================================================================================*/ 
int bmgen(FILE *logstr, double *X4, double *P4, int Npcl, 
                 double emittance_x, double emittance_y,
                 double CHMBDX, double CHMBDY,     
                 double pel0, double e0spread, double OVRVLR, char *cntl){
double beta_x, alpha_x, gamma_x, nu_x, eta_x, etap_x;
double beta_y, alpha_y, gamma_y, nu_y, eta_y, etap_y;
int ISGALX, ISGGBX, ITHETX, ISGALY, ISGGBY, ITHETY;
double CREGX, PHIX, CREGY, PHIY;
double CPHX, SPHX, CPHX2, SPHX2, AMAX, BMAX, AMM2, BMM2;
double CPHY, SPHY, CPHY2, SPHY2, AMAY, BMAY;
double POSX, DRVX, POSY, DRVY, E, pp, pz, Edev;

int i,j,k,status,ipcl; 
double x,y,z,s,t,u;
static int icall=0;

             ++icall;

             #ifdef DEBUG31
                for(i=0;i<2;i++){
                   get_opt(logstr,*(X4 + 4*i +3),
                  &beta_x, &alpha_x, &gamma_x, &nu_x, &eta_x, &etap_x,
                  &beta_y, &alpha_y, &gamma_y, &nu_y, &eta_y, &etap_y );
                   fprintf(logstr,"D31...bmgen: i = %d,   s = %f\n"
                                  "       %e %e %e %e %e %e\n"
                                  "       %e %e %e %e %e %e\n", i, *(X4 + 4*i +3),
                  beta_x, alpha_x, gamma_x, nu_x, eta_x, etap_x,
                  beta_y, alpha_y, gamma_y, nu_y, eta_y, etap_y );
                                 }
              
                for(i=Npcl-2;i<Npcl;i++){
                   get_opt(logstr,*(X4 + 4*i +3),
                  &beta_x, &alpha_x, &gamma_x, &nu_x, &eta_x, &etap_x,
                  &beta_y, &alpha_y, &gamma_y, &nu_y, &eta_y, &etap_y );
                   fprintf(logstr,"D31...bmgen: i = %d,   s = %f\n"
                                  "       %e %e %e %e %e %e\n"
                                  "       %e %e %e %e %e %e\n", i, *(X4 + 4*i +3),
                  beta_x, alpha_x, gamma_x, nu_x, eta_x, etap_x,
                  beta_y, alpha_y, gamma_y, nu_y, eta_y, etap_y );
                                 }
              #endif

       ipcl = 0;
/**------------------------------------<<<<<<<<<<<<<<<<<< particle loop **/
       while(ipcl < Npcl){ 
                   get_opt(logstr,*(X4 + 4*ipcl +0),
                  &beta_x, &alpha_x, &gamma_x, &nu_x, &eta_x, &etap_x,
                  &beta_y, &alpha_y, &gamma_y, &nu_y, &eta_y, &etap_y );

   if(strstr(cntl,"noinc") == NULL ){    

/*...Rotation angle measured from the X-axis */
/*...---------------------------------------- */
/*...Signs of (alpha) and (gamma - beta) */
      ISGALX = 0;
      if( alpha_x > 0. ) ISGALX = 1;
      ISGGBX = 0;
      if( gamma_x > beta_x ) ISGGBX = 1;
/*...Region assignment */
      ITHETX = ISGALX*ISGGBX;
      if( ISGALX == 0 && ISGGBX == 0 ) ITHETX = 1;
      CREGX = (double)( 1 - ITHETX + ISGALX ) * PAI / 2.; 
/*...PhiX */
      if( fabs(gamma_x - beta_x) < R4UNDR ){
         if( fabs(alpha_x) < R4UNDR ){ 
                PHIX = 0.;           }
         else if( ITHETX == 1 ){ 
                PHIX = CREGX + PAI /4.; }
         else{
                PHIX = CREGX - PAI /4.; }
         }
      else{
                PHIX = CREGX + atan( 2.*alpha_x / (gamma_x - beta_x) ) /2.; 
      }
/*........ alpha  gamma-beta    ITHET CREG   atan/2       PHI
             neg       neg        1    0.     0, pi/4     0, pi/4
             neg       pos        0    pi/2   -pi/4, 0    pi/4, pi/2
             pos       pos        1    pi/2   0, pi/4     pi/2, 3pi/4
             pos       neg        0    pi     -pi/4, 0    3pi/4, pi
                             See for Ref. basic/accel/leaves/beam_generator.pdf  */
/*...The same for Y */
/*.................. */
/*...Signs of (alpha) and (gamma - beta) */
      ISGALY = 0;
      if( alpha_y > 0. ) ISGALY = 1;
      ISGGBY = 0;
      if( gamma_y > beta_y ) ISGGBY = 1;
/*...Region assignment */
      ITHETY = ISGALY*ISGGBY;
      if( ISGALY == 0 && ISGGBY == 0 ) ITHETY = 1;
      CREGY = (double)( 1 - ITHETY + ISGALY ) * PAI / 2.; 
/*...PhiY */
      if( fabs(gamma_y - beta_y) < R4UNDR ){
         if( fabs(alpha_y) < R4UNDR ){ 
                PHIY = 0.;           }
         else if( ITHETY == 1 ){ 
                PHIY = CREGY + PAI /4.; }
         else{
                PHIY = CREGY - PAI /4.; }
         }
      else{
                PHIY = CREGY + atan( 2.*alpha_y / (gamma_y - beta_y) ) /2.; 
      }

/***** Determination of AMAX, BMAX; AMAY, BMAY *****/
/*****-----------------------------------------*****/
/*...Evaluate amax and bmax */
      CPHX = cos( PHIX );
      SPHX = sin( PHIX );
      CPHX2 = pow(CPHX, 2.);
      SPHX2 = pow(SPHX, 2.);
      AMM2 = gamma_x*CPHX2 + beta_x*SPHX2 + 2.* alpha_x * SPHX * CPHX;
      AMM2 = AMM2 / emittance_x;
      BMM2 = gamma_x*SPHX2 + beta_x*CPHX2 - 2.* alpha_x * SPHX * CPHX;
      BMM2 = BMM2 / emittance_x;
      if( AMM2 < R4UNDR || BMM2 < R4UNDR ){
         fprintf(logstr,"E...bmgen: CS Parameters inconsistent"
                                    " (amax)**-2 or (bmax)**-2 =< 0\n"
                        "         beta_x = %f, gamma_x = %f, alpha_x = %f, PHIX = %f\n",
                  beta_x,gamma_x,alpha_x,PHIX );
        exit(1);
                                          }
      AMAX = 1./sqrt( AMM2 );
      BMAX = 1./sqrt( BMM2 );


/*...Evaluate amay and bmay */
      CPHY = cos( PHIY );
      SPHY = sin( PHIY );
      CPHY2 = pow(CPHY, 2.);
      SPHY2 = pow(SPHY, 2.);
      AMM2 = gamma_y*CPHY2 + beta_y*SPHY2 + 2.* alpha_y * SPHY * CPHY;
      AMM2 = AMM2 / emittance_y;
      BMM2 = gamma_y*SPHY2 + beta_y*CPHY2 - 2.* alpha_y * SPHY * CPHY;
      BMM2 = BMM2 / emittance_y;
      if( AMM2 < R4UNDR || BMM2 < R4UNDR ){
         fprintf(logstr,"E...bmgen: CS Parameters inconsistent"
                                    " (amay)**-2 or (bmay)**-2 =< 0\n"
                        "         bata_y = %f, gamma_y = %f, alpha_y = %f, PHIY = %f\n",
                  beta_y,gamma_y,alpha_y,PHIY );
        exit(1);
                                          }
      AMAY = 1./sqrt( AMM2 );
      BMAY = 1./sqrt( BMM2 );

  } /* Case that cntl does not include "noinc" -- up to here */

  else if(strstr(cntl,"noinc") != NULL ){
        if(ipcl==0) fprintf(logstr,"N...bmgen: noincline\n");
        PHIX = 0.;
        CPHX = 1.;
        SPHX = 0.;
        AMAX = sqrt( emittance_x * beta_x );
        BMAX = sqrt( emittance_x * gamma_x );
        PHIY = 0.;
        CPHY = 1.;
        SPHY = 0.;
        AMAY = sqrt( emittance_y * beta_y );
        BMAY = sqrt( emittance_y * gamma_y );
  } /* cntl includes "noinc" --up to here */
         else{
                fprintf(logstr,"E...bmgen: no such case: cntl = %s\n",cntl);
                   exit(1);
            }

         POSX = 1.; POSY = 1.;
/*  while(POSX > CHMBDX || POSY > CHMBDY){          <----  mod. 050606 */
  while(fabs(POSX) > CHMBDX || fabs(POSY) > CHMBDY){
     status = getrandom_gauss(AMAX,0.,R4OVER,&x,&z);
     status = getrandom_gauss(BMAX,0.,R4OVER,&y,&u);
     status = getrandom_gauss(AMAY,0.,R4OVER,&s,&z);
     status = getrandom_gauss(BMAY,0.,R4OVER,&t,&u);

/*...Rotation to X-XP frame */
      POSX = CPHX * x - SPHX * y;
      DRVX = SPHX * x + CPHX * y;
      POSY = CPHY * s - SPHY * t;
      DRVY = SPHY * s + CPHY * t;
                                      } /* particle generated in apperture */
                                      
/* Generate momenta of the particle */
     status = getrandom_gauss(e0spread*pel0,pel0,OVRVLR,&E,&y);
     pp  = sqrt(E*E - EMASS*EMASS);
     
/* Effects of dispersion 060710*/
  if(strstr(cntl,"nodisp") == NULL ){
        if(icall==1&&ipcl==0) fprintf(logstr,"N...bmgen: dispersion on\n");
         Edev = (E - pel0)/pel0;
         POSX = POSX + eta_x * Edev;
         POSY = POSY + eta_y * Edev;
         DRVX = DRVX + etap_x * Edev;
         DRVY = DRVY + etap_y * Edev;
  }
  else { 
#ifdef DEBUG
/*061115*/
        if(icall==1&&ipcl==0) fprintf(logstr,"D...bmgen: dispersion off\n");
#endif
        Edev = 0.; } 

     pz = pp / sqrt(1. + DRVX*DRVX + DRVY*DRVY);

     *(X4 + 4*ipcl + 1) = POSX;
     *(X4 + 4*ipcl + 2) = POSY;
     *(P4 + 4*ipcl + 1) = pz * DRVX;
     *(P4 + 4*ipcl + 2) = pz * DRVY;
     *(P4 + 4*ipcl + 3) = pz;
     *(P4 + 4*ipcl + 0) = E;


            ipcl++;
      } /* particle loop */

              #ifdef DEBUG4
              fprintf(logstr,"D4...bmgen: chao\n"); 
              #endif
       return(0);
}
