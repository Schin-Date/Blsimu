#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
//#include        <const.h>
#include "main.h"

/*  Compton smasher       based on tracking_f/compt.for      cre 050304
060721   wrong signature of GC[3] is corrected
===================================================================================================*/ 
int compton(FILE *logstr, int Npcl, double *Pel4, double *Pho4,
                                   double *PSel4, double *PSph4, char const *cntl){

//unused int i,j,k,status,ipcl;
int i,k,ipcl;
//unused double x,y,z,s,t,u,v,w,PP, gzi, elab;
double x,y,s,t,u,v,w,PP, gzi, elab;
double emas2, tmin, tmax, F1, F;
double PHAI,EGCM, COSCM, SINCM, EPCM, GC[4], PR[4],PTOT[4];
int loop, loopMX;        
double  getrandom(); 
void    setrandom(int);


     emas2 = EMASS*EMASS;
     tmin = 0.; tmax = 0.;
     loopMX = 0;
     
/*** Particle loop ipcl ***/
/**************************<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
     for( ipcl = 0; ipcl < Npcl; ipcl++ ){
        PP = 0.;
        for(i=1;i<4;i++){
        PP = PP + *(Pel4 + 4*ipcl + i) * *(Pho4 + 4*ipcl + i); }
        s = emas2 + 2.*( *(Pel4 + 4*ipcl + 0) * *(Pho4 + 4*ipcl + 0) - PP );
        x = s - emas2;
        x = x / emas2;
/* tmin and tmax */    
        v = -emas2*emas2 * x*x /s;
        if(v < tmin){ tmin = v; }

/* final state momenta in the cms */
/**********************************/
        F1 = x*x /4. / (1.+x) + 0.5; /* this is the upper limit of the distribution */

     F = 0.; gzi = 1.; loop = 0;
     while( F < gzi ){ ++loop;              /* reject-accept method */
/*...A uniform random number in [0,Fmax=F1] */
      w = getrandom();
      gzi = F1 * w;
/*...A uniform random representing COS(ThetCM) */
      w = getrandom();
      COSCM = -1. + 2. * w;

      t = 1. - COSCM;
      t = t * x*x / (1. + x );
      t = - t * emas2 /2.;

      y = x + t / emas2;
      F = pow((1./x - 1./y ),2.);
      F = F + ( 1./x - 1./y );
      F = F + ( x/y + y/x ) / 4.;

       } /* while loop */

      if( loop > loopMX ){ loopMX = loop; }

/* T is generated  -> GC in cms */
/*.----------------------       */
/*...Generate phai */
      w = getrandom();
      //PHAI = 2. * PAI * w;
      PHAI = 2. * M_PI * w;

      EGCM = ( s - emas2 )/ 2. / sqrt(s);
      SINCM = sqrt( 1. - COSCM*COSCM);
      
      GC[0] = EGCM;
      GC[1] = EGCM * SINCM * cos( PHAI );
      GC[2] = EGCM * SINCM * sin( PHAI );
      GC[3] = -EGCM * COSCM; /*<<<<<<<< the minus sign had been missing up to 060721 */
                             /*<<<<<<<<************************************************/
/*... PR in the cms
  ...-------------- */
      EPCM = ( s + emas2 ) / 2. / sqrt(s);
      PR[0] = EPCM;
      PR[1] = - GC[1];
      PR[2] = - GC[2];
      PR[3] = - GC[3];

/**** Boosting to the lab frame ****/
      for(k=0;k<4;k++){
         PTOT[k] = *(Pel4 + 4*ipcl + k) + *(Pho4 + 4*ipcl + k); }
/* final state electron */
      PP = 0.;
      for(k=1;k<4;k++){ PP = PP + PTOT[k]*PR[k]; }
      elab = (PTOT[0]*PR[0] + PP) / sqrt(s);
      *(PSel4 + 4*ipcl + 0) = elab;
      u = (PR[0] + elab) / (sqrt(s) + PTOT[0]);
      for(k=1;k<4;k++){
      *(PSel4 + 4*ipcl + k) = PR[k] + u*PTOT[k]; }
      
/* final state photon */
      PP = 0.;
      for(k=1;k<4;k++){ PP = PP + PTOT[k]*GC[k]; }
      elab = (PTOT[0]*GC[0] + PP) / sqrt(s);
      *(PSph4 + 4*ipcl + 0) = elab;
      u = (GC[0] + elab) / (sqrt(s) + PTOT[0]);
      for(k=1;k<4;k++){
      *(PSph4 + 4*ipcl + k) = GC[k] + u*PTOT[k]; }
      
        #ifdef DEBUG40
/*        printf("ipcl=%d: %e %e %e %e\n",ipcl,GC[0],GC[1],GC[2],GC[3]);*/
        printf("ipcl=%d s = %e PP = %e elab= %e\n",ipcl,s,PP,elab);
        #endif
        
    } /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> loop ipcl for particles */

    #ifdef DEBUG41
    fprintf(logstr,"D41...compton: chao\n");
    #endif
    
    return 0;
}
