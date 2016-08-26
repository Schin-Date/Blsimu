/*   ORIGINAL
                                              cre 050306 by S.D. in Londrina, Brazil
              LATTICE and transfer of paricles 

 The transfer matrix for each lattice element is expressed as a 2*2 real
  matrix of the form
                      U = ( U11 U12
                            U21 U22 )
  When a particle is given with its coordinate in a transverse plane
                      (X0, Y0)
  and its momenta (PX, PY, PZ) at the entrance of the element,
  angles of the trajectory are given as 
              XP0 = PX/PZ,  YP0 = PY/PZ
  respectively. 


*/
#include <stdio.h>  /* for printf,fgets NULL */
#include <stdlib.h>  /* for exit, malloc */
#include <ctype.h> /* for tolower */
#include <string.h> /* for strcpy */
#include <math.h>   /* to use sqrt */
#include <time.h>
//#include <const.h>
#include "main.h"


/* 070106
       bending magnet coordinate system
    --------------------------------------      
 
    z : longitudinal axis in the direction of the beam
        with the origin at the center of the bending magnet pole

    x : horizontal transverse axis directing to outside the ring
        with the origin at the entrace point of fiducial electrons. 

    give an injection trajectory (x1 [m], xp1; z1 [m]) and a linear 
    momentum pel [MeV/c] as the input.
    output is a trajectory at an ejection point, (x2[m], xp2, z2[m])
           direct = +1  forward trajectory
           direct = -1  backward trajectory
 
===========================================================================*/
int bendmag(FILE *logstr, double x1, double xp1, double y1, double yp1, 
                                     double z1, double pel,
                         double *x2, double *xp2, double *y2, double *yp2, 
                                     double *z2, int *direct,
                    double lenB, double rho8,
                      char *model
){

double zU, zD, xL, xR;
double thetb0, tanb0, cosb0, rho;
double thet1, thet2;
double pel0=8000.;
double xC, zC;
double C1, S1, C2, S2;
double s,t,u,arclen;
static int ncall=0;
    ncall++;

#ifdef DBGBend2
     if( ncall > 311288 ){ 
    fprintf(logstr,"D2...bendmag: ncall = %d\n", ncall);}
    printf("...bendmag: R4OVER/UNDR = %e/%e\n",R4OVER,R4UNDR);//<<<<<<<<<<<<<<<<<<<<
#endif

/* tmporal setting */
         zU = z1;  /* z1 is the entry point given in the main */  
         zD = z1 + lenB;   
         xL = -80./1000.; /* left border in m */
         xR = 80./1000.;  /* right border in m */


       if(x1<xL || x1>xR){
            fprintf(logstr,"E...bendmag:in x1 = %e ncall=%d\n", x1, ncall);
            exit(1);  }


         //thetb0 = PAI/88;
         thetb0 = M_PI/88;
         tanb0 = tan(thetb0);
         cosb0 = cos(thetb0);

         thet1 = atan(xp1);

/** the dipole **/
          rho = (pel / pel0) * rho8;
#ifdef DBGBend4
     if( ncall < 10  ){ 
    fprintf(logstr,"D4...bendmag: rho8 = %f rho = %f pel = %f\n", rho8, rho, pel);}
#endif

/*** center of the cyclotron motion in hor. plane ***/
          C1 = 1. / sqrt( 1. + xp1*xp1 );
          S1 = xp1 * C1; 
          xC = x1 - rho*C1;
          zC = z1 + rho*S1;

/**** judge for the left border ****/
          S2 = S1 - lenB/rho;

   if( S2 > -1.){          /*<<<< (1)  may touch to z=zD */
      *direct = 1;
      if(S2 > 1){
            fprintf(logstr,"E...bendmag:(1) S2=%e\n", S2);
            exit(1);  }
      C2 = sqrt(1. - S2*S2);  
      u = rho*rho -(zD-zC)*(zD-zC);
      if(u < 0.){
            fprintf(logstr,"E...bendmag:(1) x2  u=%e\n", u);
            exit(1);  }
      *x2 = xC + sqrt(u);
      if( *x2 > xL){  /* (a) ejection from  down border >>>>>>*/ 
         *z2 = zD;
         *xp2= S2 / C2;
         *y2 = y1 + lenB * yp1;    /* <--replace lenB by orbit length */
         *yp2 = yp1;
      if(fabs(*xp2) > R4OVER){
           fprintf(logstr,"W...bendmag:(1)(a) C2 = %e\n", C2);}
#ifdef DBGBend3
     if( ncall < 10 ){
    fprintf(logstr,"D3...bendmag:(1a)  x1=%e xp1=%e x2=%e\n", x1,xp1,*x2);
          thet2 = atan(*xp2);
           arclen = rho * (thet1 - thet2);
    fprintf(logstr,"D3...bendmag: lenB - arclen = %e / lenB=%f\n",lenB-arclen,lenB);      } 
#endif

         return(0);
                    }  /* (a) ejection from  down border <<<<<<<<*/ 
      else{        /*<<< N(a) ejection from the Left border */ 
        *x2 = xL;
      u = rho*rho - (xL-xC)*(xL-xC);
      if(u < 0.){
            fprintf(logstr,"E...bendmag:(1) N(a) u=%e\n", u);
            fprintf(logstr,"    x2 = %e  xL = %e xC = %e\n", *x2, xL, xC);
            fprintf(logstr,"    xL -  xC = %e, rho = %e\n", xL- xC, rho);
            fprintf(logstr,"    x1 = %e,  xp1 = %e\n", x1, xp1);
            exit(1);  }
        *z2 = zC + sqrt(u);
       if(fabs(xL-xC) < R4UNDR){
            fprintf(logstr,"W...bendmag:(1) N(a) xL-xC=%e\n", xL-xC);
            if(zC > *z2){ *xp2 = R4OVER; }
            else{ *xp2 = -R4OVER; }
         }
         else{
       *xp2 = (zC-*z2)/(xL-xC); 
           }
       thet2 = atan(*xp2);
        arclen = rho * (thet1 - thet2);
        *y2 = y1 + arclen * yp1;    /* <-- */
        *yp2 = yp1;
         return(0);
                    }  /*<<< N(a) ejection from the Left border */
                          }/*  (1) may touch to z=zD >>>>>*/
 /* N(1) may NOT touch to z=zD >>>>>*/ 
     else{   
          if(xC > xL ){  /*<<< Not(b) */
#ifdef DBGBend5
    fprintf(logstr,"D...bendmag:N(1) Not(b) ncall= %d\n", ncall);
#endif
          *direct = -1; 
              if(xC - xL < rho ){ /* (i) */  
               *x2 = xL;
                *z2 = zC + sqrt(rho*rho - (xL-xC)*(xL-xC));
                 *xp2 = (zC-*z2)/(xL-xC); 
                  thet2 = atan(*xp2);
//                  arclen = rho * (PAI +thet1 - thet2);
                  arclen = rho * (M_PI +thet1 - thet2);
                 *y2 = y1 + arclen * yp1;    /* <-- */
                *yp2 = yp1;
                return(0);
               }  /* (i) */
              else{  /* Not(i) */ 
               *x2 = xC - sqrt(rho*rho - (zC-zU)*(zC-zU));
                *z2 = zU; 
                 *xp2 = (zU-zC)/(xC-*x2); 
                  thet2 = atan(*xp2);
//                  arclen = rho * (PAI + thet1 - thet2);
                  arclen = rho * (M_PI + thet1 - thet2);
                 *y2 = y1 + arclen * yp1;    /* <-- */
                *yp2 = yp1;
               return(0);
              } /* Not(i)*/
          } /* Not(b) */
         else{  /* (b) */
#ifdef DBGBend5
    fprintf(logstr,"D...bendmag:N(1) & (b) ncall= %d\n", ncall);
#endif
           *direct = 1; 
           *x2 = xL;
           *z2 = zC + sqrt(rho*rho - (xL-xC)*(xL-xC));
           *xp2 = (zC-*z2)/(xL-xC); 
           thet2 = atan(*xp2);
           arclen = rho * (thet1 - thet2);
           *y2 = y1 + arclen * yp1;    /* <-- */
           *yp2 = yp1;
            return(0);
             } /* (b) */


         fprintf(logstr,"E...bendmag: S2=%e, xC = %e, rho=%e\n",S2, xC, rho);
         exit(1);

         }  /* N(1) may NOT touch to z=zD >>>>>*/ 




#if(0)
-------------------------------------
            if( fabs(S2) > 1. ){       /* the particle is trapped in B */
               fprintf(logstr,"E...track_lattice: particle trapped SB2 = %f  ipcl = %d\n",S2,ipcl);
               fprintf(logstr,"X4: %e %e %e %e\n"
                    ,*(X4+4*ipcl+0),*(X4+4*ipcl+1),*(X4+4*ipcl+2),*(X4+4*ipcl+3));
               fprintf(logstr,"P4: %e %e %e %e\n"
                    ,*(P4+4*ipcl+0),*(P4+4*ipcl+1),*(P4+4*ipcl+2),*(P4+4*ipcl+3));
               fprintf(logstr,"(x1,xp1) = (%e, %e)  (y1, yp1) = (%e, %e)\n"
                    ,x1,xp1,y1,yp1);
               iftrap = 1; ifin = 1; ++ntrap;
               break; }
   alternative evaluation
            C2 = sqrt( 1. - S2*S2 );
            *xp2 = S2 / C2;
            *x2 = x1 + (C2 - C1) * rho; 
           /*** z-direction ***/
            *z2 = z1 + lenB;    
-------------------------------------------
#endif



#ifdef DBGBend3
       if(ncall==1){
       fprintf(logstr,"D...bendmag: y2=%e yp2=%e",*y2, *yp2);
       }
#endif      
            } /* elem = B */ 
       
#if(0)
    if( ifin != 1 || s1 <= slastelm ){
       fprintf(logstr,"E...track_lattice: the last exit = %f < slastelm = %f",s1,slastelm);
       exit(1);         }
       
    if( iftrap == 1){  *(X4 + 4*ipcl + 1) = -R4OVER;  continue; }
    


         *(X4 + 4*ipcl + 0) = s1;
         *(X4 + 4*ipcl + 1) = x2;
         *(X4 + 4*ipcl + 2) = y2;
         *(X4 + 4*ipcl + 3) = s1;
       
         pz = pel / sqrt(1. + xp2*xp2 + yp2*yp2);
       
         *(P4 + 4*ipcl + 3) = pz;
         *(P4 + 4*ipcl + 1) = pz * xp2;
         *(P4 + 4*ipcl + 2) = pz * yp2;
       
     } /* >>>>>>>>>>>>>>>>>>> particle loop */

     #ifdef DEBUG3
       fprintf(logstr,"D3...track_lattice: nfin = %d, ntrap = %d  chao\n",nfin,ntrap);
     #endif

    return(0);          
}                                               
#endif
/*===========================================================================*/
