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

070115
   coordinates are computed at each element to check if they are inside CHMB
   this is necessaty for passages of very low electrons.
0702211
  exit point between elements are calculates as crossing points with
  the chamber acceptance. 

*/
#include <stdio.h>  /* for printf,fgets NULL */
#include <stdlib.h>  /* for exit, malloc */
#include <ctype.h> /* for tolower */
#include <string.h> /* for strcpy */
#include <math.h>   /* to use sqrt */
#include <time.h>
#include <const.h>

static int nelem;
static double s1, s2;
static char elmnam[4300][10];
static int flag[4300], cell[4300], magid[4300];
static double s[4300], length[4300], strg1[4300], strg2[4300];
static double beta_x[4300], alpha_x[4300], gamma_x[4300], nu_x[4300], eta_x[4300], etap_x[4300];
static double beta_y[4300], alpha_y[4300], gamma_y[4300], nu_y[4300], eta_y[4300], etap_y[4300];

/*===========================================================================*/
int track_lattice(FILE *logstr, double *X4, double *P4, int Npcl, 
                  double CHMBDX, double CHMBDY,     
                  double slastelm, double pel0 ){

double x0, xp0, y0, yp0, z0, a, b, c;
double x1, xp1, y1, yp1, x2, xp2, y2, yp2, x, xp, y, yp, z;
double el, pel, px,py,pz,pt2,pp2;
double s0, s1, z1, s2, z2, l, lenB, lenarc, rK;
double thetb0, tanb0, cosb0, rho0, rho;
double xB1, xpB1, CB1, SB1, xB2, xpB2, CB2, SB2;
double Uh[4], Uv[4];
double A[4],T[4],F[4],D[4];
int ipcl, ielm, i,j,k;
int ifin,  ntrap, nfin;
int idirect, ifBside, ifCHMBout;
char cntl_BFmodel[128];
static int ncall=0;
       ncall++;



         ntrap = 0; nfin=0;
      for(ipcl = 0; ipcl<Npcl; ipcl++){   /*<<<<<<<<<<< particle loop */
         el = *(P4 + 4*ipcl + 0);
         pz = *(P4 + 4*ipcl + 3);
         px = *(P4 + 4*ipcl + 1);
         py = *(P4 + 4*ipcl + 2);
         pt2 = px*px + py*py;
         pp2 = pt2 + pz*pz;
         pel = sqrt( pp2 );     
         x0 = *(X4 + 4*ipcl + 1);
         y0 = *(X4 + 4*ipcl + 2);
           xp0 = 0.;   yp0 = 0.;
           if( fabs(pz) < R4UNDR ){
              if( px > R4UNDR ) xp0 = R4OVER;
              if( px < -R4UNDR ) xp0 = -R4OVER;
              if( py > R4UNDR ) yp0 = R4OVER;
              if( py < -R4UNDR ) yp0 = -R4OVER;
            }
            else {
         xp0 = px / pz;
         yp0 = py / pz;
            }
         s0 = *(X4 + 4*ipcl + 0);     
         z0 = *(X4 + 4*ipcl + 3);     
         s1 = s0;
         x1 = x0; xp1 =xp0; y1 = y0; yp1 = yp0; z1 = z0;

         if( z0 != s0 ){
            fprintf(logstr,"E...lattice: z0 = %e, s0 = %e\n", z0, s0);
                                                     exit(1); }
         if( fabs(x1) > CHMBDX || fabs(y1) > CHMBDY ){
            fprintf(logstr,"E...lattice: starting point x1 = %e, y1 = %e, s1=%e\n",
                                          x1,y1,s1);
                                                     exit(1); }

         mtxunit(logstr,2,2,Uh);
         mtxunit(logstr,2,2,Uv);

         ifin = 0;  ifBside=0, ifCHMBout=0;
       for(ielm=0;ielm<nelem;ielm++){    /*<<<<<<<<<<<<<<<<<<<<<< loop of lattice elements */
           if(s[ielm] < s0) continue;           
           if(s1 > slastelm || ifBside==1 || ifCHMBout!=0){
/** coordinates at the exit of the last element **/         
              x2 = *(Uh + 2*0 + 0) * x1 +  *(Uh + 2*0 + 1) * xp1;
              xp2 = *(Uh + 2*1 + 0) * x1 +  *(Uh + 2*1 + 1) * xp1;
              y2 = *(Uv + 2*0 + 0) * y1 +  *(Uv + 2*0 + 1) * yp1;
              yp2 = *(Uv + 2*1 + 0) * y1 +  *(Uv + 2*1 + 1) * yp1;
              s2 = s1; z2 = z1;
              ifin = 1;  ++nfin;
             #ifdef DEBUG8
              fprintf(logstr,"D8...track_lattice: finished ipcl = %d\n", ipcl);
              #endif
            break;           }           /*>>>>>>>>>>>>>>>>>>>>>>>> exit from the loop */
/* In the following loop of elements, define variables at point 1 at the end 
  ------------------------------------------------------------------------------*/
         if( fabs(x1) > CHMBDX || fabs(y1) > CHMBDY ){
            fprintf(logstr,"E...lattice: starting point x1 = %e, y1 = %e, s1=%e, ielm=%d\n",
                                          x1,y1,s1,ielm);
                                                     exit(1); }
         idirect = 1;   
           if( strstr(elmnam[ielm],"Q") != NULL ){  /******  element = Quad *****/
/** drift space from s1 to the Q **/
              l = s[ielm] - s1; 

         if( ifCHMBout != 0 ){
            fprintf(logstr,"E...lattice(2): ifCHMBout = %d\n", ifCHMBout );
                                                     exit(1); }
          a = R4OVER; b=R4OVER;
         if( fabs(yp1) > R4UNDR ){
               if( fabs(y1) > CHMBDY ){
                   fprintf(logstr,"E...lattice(3y): s1 = %e, y1 = %e, yp1=%e\n",
                                         s1,y1,yp1);     exit(1); }
                c = 1.;
                if( yp1 < 0. ) c = -1.;
                a = (c*CHMBDY - y1)/yp1; 
         }
         if( fabs(xp1) > R4UNDR ){
               if( fabs(x1) > CHMBDX ){
                   fprintf(logstr,"E...lattice(3x): s1 = %e, x1 = %e, xp1=%e\n",
                                         s1,x1,xp1);     exit(1); }
                 c = 1.;
                 if( xp1 < 0. ) c = -1.;
                 b = (c*CHMBDX - x1)/xp1; 
         }     
          if( a < l || b < l ){
#ifdef Wlattice11
               fprintf(logstr,"W11...lattice(3yx): ielm= %d, s1= %e, l= %e, a=%e b=%e\n",
                                      ielm,s1,l,a,b);
#endif
                if( a > b ) a = b;
                 s2 = s1 + a; 
                 x2 = x1 + xp1*a; 
                 y2 = y1 + yp1*a;
                 z2 = s2;
                 xp2 = xp1; 
                 yp2 = yp1;
                 s1 = s2; x1 = x2;  y1 = y2; z1 = z2;
                 ifCHMBout = 1; 
                 break;  
              /*   continue; */ 

          }

         mtxunit(logstr,2,2,T);
         *(T + 2*0 + 1) = l;
         mtxcopy(logstr,2,2,Uh,A);              
         mtxprod(logstr,2,T,2,A,2,Uh);
         mtxcopy(logstr,2,2,Uv,A);
         mtxprod(logstr,2,T,2,A,2,Uv);

            s2 = s[ielm]; z2 = s2;
              x2 = *(Uh + 2*0 + 0) * x1 +  *(Uh + 2*0 + 1) * xp1;
              xp2 = *(Uh + 2*1 + 0) * x1 +  *(Uh + 2*1 + 1) * xp1;
              y2 = *(Uv + 2*0 + 0) * y1 +  *(Uv + 2*0 + 1) * yp1;
              yp2 = *(Uv + 2*1 + 0) * y1 +  *(Uv + 2*1 + 1) * yp1;
            x1 = x2; xp1 =xp2; y1 = y2; yp1 = yp2; s1 = s2; z1 = z2;
         mtxunit(logstr,2,2,Uh);
         mtxunit(logstr,2,2,Uv);

         if( fabs(x1) > CHMBDX || fabs(y1) > CHMBDY ){
            fprintf(logstr,"E...lattice: after straight to q(%d)\n"
                           "     ncall=%d x1 = %e, y1 = %e, s1=%e\n",
                                        magid[ielm], ncall, x1,y1,s1);
            fprintf(logstr,"             ielm = %d, cell = %d magid = %d selm = %e\n",
                                      ielm, cell[ielm],magid[ielm], s[ielm]);
            exit(1); 
                                                      }
/** the quad **/
            l = length[ielm];
            rK = sqrt( (pel0 / pel) * fabs(strg1[ielm]) );

         *(F + 2*0 + 0) = cos(rK*l);         *(F + 2*0 + 1) = sin(rK*l)/rK;
         *(F + 2*1 + 0) = -rK*sin(rK*l);     *(F + 2*1 + 1) = cos(rK*l);
         *(D + 2*0 + 0) = cosh(rK*l);         *(D + 2*0 + 1) = sinh(rK*l)/rK;
         *(D + 2*1 + 0) = rK*sinh(rK*l);     *(D + 2*1 + 1) = cosh(rK*l);

            if(strg1[ielm] > 0. ){   /* F-type */
         mtxcopy(logstr,2,2,Uh,A);              
         mtxprod(logstr,2,F,2,A,2,Uh);
         mtxcopy(logstr,2,2,Uv,A);
         mtxprod(logstr,2,D,2,A,2,Uv);
            }
            else {                   /* D-type */
         mtxcopy(logstr,2,2,Uh,A);              
         mtxprod(logstr,2,D,2,A,2,Uh);
         mtxcopy(logstr,2,2,Uv,A);
         mtxprod(logstr,2,F,2,A,2,Uv);
            }
            
         if( ifCHMBout != 0 ){
            fprintf(logstr,"E...lattice(4): ifCHMBout = %d\n", ifCHMBout );
                                                     exit(1); }
                                    
            s2 = s[ielm] + length[ielm];
              z2 = s2;
              x2 = *(Uh + 2*0 + 0) * x1 +  *(Uh + 2*0 + 1) * xp1;
              xp2 = *(Uh + 2*1 + 0) * x1 +  *(Uh + 2*1 + 1) * xp1;
              y2 = *(Uv + 2*0 + 0) * y1 +  *(Uv + 2*0 + 1) * yp1;
              yp2 = *(Uv + 2*1 + 0) * y1 +  *(Uv + 2*1 + 1) * yp1;
            s1 = s2; x1 = x2; xp1 =xp2; y1 = y2; yp1 = yp2; z1 = z2;
         mtxunit(logstr,2,2,Uh);
         mtxunit(logstr,2,2,Uv);

         if( fabs(x1) > CHMBDX || fabs(y1) > CHMBDY ){
           ifCHMBout=1;
#ifdef Wlattice5
            fprintf(logstr,"W5...lattice: q(%d) ncall=%d x1 = %e, y1 = %e, s1=%e\n",
                                        magid[ielm], ncall, x1,y1,s1);
  /*          fprintf(logstr,"             ielm = %d, cell = %d magid = %d selm = %e\n",
                                      ielm, cell[ielm],magid[ielm], s[ielm]); */
#endif
                 break;  
              /*   continue; */ 
                                                      }
            } /* elem = Q */ 


           else if(strstr(elmnam[ielm],"B") != NULL){   /***** element = Bend *****/ 
/** drift space from s1 to the B **/
              l = s[ielm] - s1; 
/*** splash before reaching to B ***/               

         if( ifCHMBout != 0 ){
            fprintf(logstr,"E...lattice(6): ifCHMBout = %d\n", ifCHMBout );
                                                     exit(1); }
          a = R4OVER; b=R4OVER;
         if( fabs(yp1) > R4UNDR ){
               if( fabs(y1) > CHMBDY ){
                   fprintf(logstr,"E...lattice(6y): s1 = %e, y1 = %e, yp1=%e\n",
                                         s1,y1,yp1);     exit(1); }
                c = 1.;
                if( yp1 < 0. ) c = -1.;
                a = (c*CHMBDY - y1)/yp1; 
         }
         if( fabs(xp1) > R4UNDR ){
               if( fabs(x1) > CHMBDX ){
                   fprintf(logstr,"E...lattice(6x): s1 = %e, x1 = %e, xp1=%e\n",
                                         s1,x1,xp1);     exit(1); }
                 c = 1.;
                 if( xp1 < 0. ) c = -1.;
                 b = (c*CHMBDX - x1)/xp1; 
         }     
          if( a < l || b < l ){
#ifdef Wlattice12
               fprintf(logstr,"W12...lattice(3yx): ielm= %d, s1= %e, l= %e, a=%e b=%e\n",
                                      ielm,s1,l,a,b);
#endif
                if( a > b ) a = b;
                 s2 = s1 + a; 
                 z2 = s2;  
                 x2 = x1 + xp1*a; 
                 y2 = y1 + yp1*a;
                 xp2 = xp1; 
                 yp2 = yp1;
                 s1 = s2; x1 = x2;  y1 = y2; z1 = z2; 
                 ifCHMBout = 1; 
#ifdef DBGlattice25
fprintf(logstr,"D25...lattice: splash from straight befor B, ncall=%d\n", ncall);
#endif
                 break;  

          }


         mtxunit(logstr,2,2,T);
         *(T + 2*0 + 1) = l;
         mtxcopy(logstr,2,2,Uh,A);              
         mtxprod(logstr,2,T,2,A,2,Uh);
         mtxcopy(logstr,2,2,Uv,A);
         mtxprod(logstr,2,T,2,A,2,Uv);

         s2 = s[ielm];
         z2 = s2;
/** coordinates at s = s1 **/         
              x2 = *(Uh + 2*0 + 0) * x1 +  *(Uh + 2*0 + 1) * xp1;
              xp2 = *(Uh + 2*1 + 0) * x1 +  *(Uh + 2*1 + 1) * xp1;
              y2 = *(Uv + 2*0 + 0) * y1 +  *(Uv + 2*0 + 1) * yp1;
              yp2 = *(Uv + 2*1 + 0) * y1 +  *(Uv + 2*1 + 1) * yp1;
            s1 = s2; x1 = x2; xp1 =xp2; y1 = y2; yp1 = yp2; z1 = z2;

         if( fabs(x1) > CHMBDX || fabs(y1) > CHMBDY ){
            fprintf(logstr,"E...lattice: after straight to B(%d)\n"
                           "     ncall=%d x1 = %e, y1 = %e, s1=%e\n",
                                        magid[ielm], ncall, x1,y1,s1);
            fprintf(logstr,"             ielm = %d, cell = %d magid = %d selm = %e\n",
                                      ielm, cell[ielm],magid[ielm], s[ielm]);
            exit(1); 
                                                      }

/**** check if particles are inside B field region ****/
    if(fabs(x1) > 80./1000.){
       fprintf(logstr,"Etr...lattice:x1 = %e x0 = %e, xp0 = %e\n"
                     "            Uh[0,0] = %e, Uh[0,1] = %e pel = %e\n",
                   x1,  x0, xp0, *(Uh + 2*0 + 0), *(Uh + 2*0 + 1), pel);
                                    }
            
/** reset transfer matrices **/
            mtxunit(logstr,2,2,Uh);
            mtxunit(logstr,2,2,Uv);
            

/** the dipole **/
            strcpy(cntl_BFmodel,"uniform");
            thetb0 = PAI/88;
            tanb0 = tan(thetb0);
            cosb0 = cos(thetb0);
            l = length[ielm]; /* this is trajectory length */
            lenarc = l;
            rho0 = strg1[ielm];
            lenB = 2.*rho0*tanb0*cosb0;
            rho = (pel / pel0) * rho0;

/*** (X1,XP1;S1) -> Z-XB frame (XB1,XPB1;Z1) ***/
            xpB1 = (xp1 + tanb0) / (1.- xp1 * tanb0);
            xB1 = x1 / (1.- xp1 * tanb0) / cosb0;
/*** (XB1,XPB1) -> Exit of B (XB2,XPB2;Z2) ***/
/* 070106 z1 is set as the same value as s1*/
    z1 = s1;
    if(fabs(xB1) > 80./1000.){
       fprintf(logstr,"E...lattice: xB1 = %e, x1 = %e, xp1 = %e, tanb0 = %e\n",
                       xB1, x1,xp1, tanb0);
                                   exit(1); }
    bendmag(logstr, xB1, xpB1, y1, yp1, z1, pel,
                    &xB2, &xpB2, &y2, &yp2, &z2, &idirect,
                   l, strg1[ielm], cntl_BFmodel);
#ifdef DBGBend0
       if(ncall==1){
  fprintf(logstr,"D0...lattice: bend l = %e lenB = %e\n",l,lenB);}
#endif
/** coordinates 2 of particles at their exit from the B field are given **/
/* side splashed particles end the tracking */
/** particles with z2 < z1 + lenB - 0.001 are side splashed **/
/** particles outside chamber apperture are side splashed **/
      a = z2 - z1 - lenB;
      if(  a <  -0.001 || fabs(xB2) > CHMBDX || fabs(y2) > CHMBDY ){ 
         ifBside = 1;
          x2 = xB2;
          s2 = z2;
          xp2 = xpB2;
          break;
       } 
/*########## Proceed partcles by more than 10 cm for further tracking.
           Reevaluate a after the process.
           Tracking of this particle finishes otherwise and
           its coordinates are given in the B frame. 
           The later case always happen in the current form
           since the process is not installed yet.              ############*/
      if(  a <  0.1 ){ 
          x2 = xB2;
          s2 = z2;
          xp2 = xpB2;
          break;
       } 
 
/*...(XB2,XPB2,Z2) -> X-S downstream frame (X2,XP2;S2) 
    ------------------------------------------------------
   will be applied for particles which arrive to points with
   z > z1 + lenB + 0.1 */
          x2 = cosb0*(xB2 + tanb0*(z2-z1-lenB));
          s2 = s1 + lenarc + cosb0*(-tanb0* xB2 + z2- z1-lenB);
          xp2 = (xpB2 + tanb0)/(1. - xpB2 * tanb0); 
/*070115<<<< yp should be changed according to the change of s direction */
#if(0)
    In the above, we have assumed particles are on a plane at z=z2.
This is not the case currently and neccessary to change this part.
This part is however never executed in recent considerations and
we leave this part until it get relevent.  -- 070302
Previously, a format had been employed where particle trajectories are
projected on a plane at s=s2. In a coordinate frame where s and z coinside
at the center on the B exit, we had a program piece as follows:
             s2 = z2;
/*...(XB2,XPB2) -> X-S downstream frame (X2,XP2;S2) */
/* in the following case, we extend straight trajectories to get values at s=s2
   for given data at z = z2 = s2. Adequate only for the canonical ejections. */
            xp2 = ( tanb0 + xpB2 ) / ( 1. - xpB2 * tanb0 );
            x2 = ( 1. + xp2 * tanb0 ) * cosb0 * xB2;      
            s1 = s2;
            x1 = x2; xp1 = xp2; y1 = y2; yp1 = yp2;
#endif            
/* direction inversion see #60 070107B */
/* this must be applied only for the case of coordinate rotation from the B frame */
         if( xpB2 > 1./tanb0 ){
              b = (xpB2 + tanb0)/(1. - xpB2 * tanb0); 
               if(b > 0.){
                 fprintf(logstr,"E...lattice: idirect=%d xpB2=%e tanb0=%e b=%e\n",
                              idirect, xpB2, tanb0, b);
                 exit(9); }
              idirect = -idirect;
           }

            s1 = s2; z1 = z2;
            x1 = x2; xp1 = xp2; y1 = y2; yp1 = yp2;

         if( fabs(x1) > CHMBDX || fabs(y1) > CHMBDY ){
                ifCHMBout=3;
#ifdef Wlattice2
            fprintf(logstr,"W2...lattice: B(3) ncall=%d x1 = %e, y1 = %e, s1=%e\n",
                                          ncall,x1,y1,s1);
            fprintf(logstr,"             B(3) xB1 = %e, xB2 = %e, pel=%e\n",
                              xB1, xB2, pel);
#endif
                break;
         }

            ifBside = 1;  /* 070228 */
            ifBside = 0;

#ifdef DBGBend3
       if(ncall==1){
        fprintf(logstr,"D3...lattice: x1=%e xp1=%e y1=%e yp1=%e\n",x1,xp1,y1,yp1);
                   }
#endif           
            } /* elem = B */ 
       } /*>>>>>>>>>>>>>>>>>>>>> element loop */
       

#ifdef DBGBend5
    /*if( ifin == 0 ||(ifin == 1 && s1 <= slastelm) ){ */
    if( ifin == 1 && s1 <= slastelm ){
       fprintf(logstr,"D5W...track_lattice: ifin=%d ifBside=%d ifCHMBout=%d exit = %f / slastelm = %f\n",
                             ifin,ifBside,ifCHMBout, s1,slastelm);
                       }
#endif           
    /* if( ifin != 1 || (s1 <= slastelm && ifBside!=1 && ifCHMBout==0)){ */
    /* if( ifin = 1 && ifBside!=1 && s1 <= slastelm ){ exit(1);         }*/
    
#ifdef DBGBend5
          if(ifCHMBout!=0&&ifCHMBout!=3){
           fprintf(logstr,"D5...lattice: ploop passed with ifCHMBout = %d in ncall = %d\n", 
                                 ifCHMBout,ncall);
          } 
#endif           
#ifdef Wlattice1
         if( fabs(x2) > CHMBDX || fabs(y2) > CHMBDY ){
            fprintf(logstr,"W1...lattice: end  ncall=%d x2 = %e, y2 = %e, Ee=%e\n",
                                         ncall, x2,y2,*(P4 + 4*ipcl + 0)); }
#endif
         *(X4 + 4*ipcl + 0) = s2;
         *(X4 + 4*ipcl + 1) = x2;
         *(X4 + 4*ipcl + 2) = y2;
         *(X4 + 4*ipcl + 3) = z2;
       
         pz = (double)idirect * pel / sqrt(1. + xp2*xp2 + yp2*yp2);
       
         *(P4 + 4*ipcl + 3) = pz;
         *(P4 + 4*ipcl + 1) = pz * xp2;
         *(P4 + 4*ipcl + 2) = fabs(pz) * yp2;
       
     } /* >>>>>>>>>>>>>>>>>>> particle loop */

     #ifdef DEBUG3
       fprintf(logstr,"D3...track_lattice: nfin = %d, ntrap = %d  chao\n",nfin,ntrap);
     #endif

    return(0);          
}                                               
/*===========================================================================*/
int read_lattice(FILE *logstr, char *infile, double smin, double smax){
FILE *instr;
int nline , iskip, rcolumn, scolumn, icolumn;
int ndata;
int *Idata;
double *Rdata;
char **Sdata;

int status, i, j, l;
double x;
char buffer[240]="";


/*** set region to store data ***/
/*070106     s1 = 946.980; s2 = 976.895; */   /* one normal cell around BL33*/
/*070106     s1 = 946.980; s2 = 980.;  */  /* one normal cell around BL33*/
     s1 = smin; s2 = smax;

/********************************/
/******* Read lattice file *******/
/*********************************/

              iskip = 1;

      instr=fopen(infile,"r");    
#ifdef DEBUG
/*061115     */ 
         fprintf(logstr,"D...about to read an lattice file =%s\n",infile);
#endif
      nline = 5000; scolumn = 1; icolumn = 3; rcolumn = 16;
           Idata = calloc(nline*icolumn,sizeof(int));
           Rdata = calloc(nline*rcolumn,sizeof(double));
           Sdata = calloc(nline*scolumn,sizeof(char*));  
           for(i=0;i<nline*scolumn;i++){
              *(Sdata+i) = calloc(64,sizeof(char));}
   
      status =  eat_table_lattice(logstr, instr, iskip, nline, 
                          icolumn, rcolumn, scolumn, 
                          &ndata, Idata, Rdata, Sdata );
            if(status != 0){  
             fprintf(logstr,"E...main< eat_table_optics"
                 "status = %d\n",status);
             exit; 
            }
      fclose(instr);
      fprintf(logstr,"N...read_lattice < eat_table_lattice: ndata = %d\n",ndata);
     j = 0;
     for(i=0;i<ndata;i++){
             x =  *(Rdata+ rcolumn*i + 0);  /* s[m] */
           if( x <= s2+.1 && x >= s1-.1 ){
             if( j > 4300-1 ){ fprintf(logstr,"E...read_lattice: j= %d\n",j); exit;}
             strcpy(&elmnam[j][0],*(Sdata + scolumn*i + 0)); 
             flag[j] = *(Idata+ icolumn*i + 0);
             cell[j] = *(Idata+ icolumn*i + 1);
             magid[j] = *(Idata+ icolumn*i + 2);
             s[j] =  x;  /* s[m] */
             length[j] =  *(Rdata+ rcolumn*i + 1);  /* length[m] */
             strg1[j] =  *(Rdata+ rcolumn*i + 2);  /* strength1 */
             strg2[j] =  *(Rdata+ rcolumn*i + 3);  /* strength2 */
             beta_x[j] =  *(Rdata+ rcolumn*i + 4);  /* betax[m] */
             alpha_x[j] =  *(Rdata+ rcolumn*i + 5);  /* alpha_x */
             gamma_x[j] =  *(Rdata+ rcolumn*i + 6);  /* gamma_x */
             nu_x[j] =  *(Rdata+ rcolumn*i + 7);  /* nux */
             eta_x[j] =  *(Rdata+ rcolumn*i + 8);  /* eta_x */
             etap_x[j] =  *(Rdata+ rcolumn*i + 9);  /* etap_x */
             beta_y[j] =  *(Rdata+ rcolumn*i + 10);  /* beta_y */
             alpha_y[j] =  *(Rdata+ rcolumn*i + 11);  /* alpha_y */
             gamma_y[j] =  *(Rdata+ rcolumn*i + 12);  /* gamma_y */
             nu_y[j] =  *(Rdata+ rcolumn*i + 13);  /* nuy */
             eta_y[j] =  *(Rdata+ rcolumn*i + 14);  /* eta_y */
             etap_y[j] =  *(Rdata+ rcolumn*i + 15);  /* etap_y */
             j++;
            }
     }
      nelem = j;
      fprintf(logstr,"N...read_lattice: nelem = %d elements stored, chao\n",nelem);

      free(Idata);
      free(Rdata);
      for(i=0;i<nline*scolumn;i++){ free(*(Sdata+i));}
      free(Sdata);

    
    
}
/*===========================================================================*/
int eat_table_lattice(FILE *logstr, FILE *instr, int iskip, int nline, 
             int icolumn,  int rcolumn, int scolumn,
            int *ndata, int *Idata, double *Rdata, char **Sdata){

char buffer[240]="", buffer2[240]="", *a;  
int i, idata, icol, rcol, scol;
int i1;
double  x1;
char s[64];

/*---skip header lines ---*/
    i = 0;
    while( i < iskip ){
      fgets(buffer,sizeof(buffer),instr);
      i++;
    }
/*     fprintf(logstr,"N...eat_table_optics: %d lines skipped\n", iskip);  */

/*************** Data Line Loop *****************/
/*---loop of reading/printing---*/
    idata = 0;
    while( ( fgets(buffer,sizeof(buffer),instr) )!=NULL ){

        if(idata > nline){
           fprintf(logstr,"E...eat_table_optics:"
                 "idata = %d > nline = %d\n",idata,nline);
                     exit(1); 
                               }

        i = strlen(buffer);
        if( i <= 1 ) continue;  /* skip blank lines */
        strcpy(buffer2, buffer);  /* to preserve buffer */

        icol = 0; rcol = 0; scol = 0;

/*  The first item should be taken separately with buffer2 as the first argument  */
        a = strtok(buffer2," ");
        strcpy(s,a);
        strcpy(*(Sdata + scolumn*idata + scol),s); 
        scol++;

/*  The following items should be taken with NULL as the first argument of strtok */
        a = strtok(NULL," ");
        sscanf(a,"%ld",&i1);  
        *(Idata + icolumn*idata + icol) = i1;
        icol++;

        a = strtok(NULL," ");
        sscanf(a,"%ld",&i1);  
        *(Idata + icolumn*idata + icol) = i1;
        icol++;

     for(rcol =0; rcol<4; rcol++){
        a = strtok(NULL," ");
        sscanf(a,"%lf",&x1);  
        *(Rdata + rcolumn*idata + rcol) = x1;
      }

        a = strtok(NULL," ");
        sscanf(a,"%ld",&i1);  
        *(Idata + icolumn*idata + icol) = i1;
        icol++;


     for(rcol =4; rcol<rcolumn; rcol++){
        a = strtok(NULL," ");
        sscanf(a,"%lf",&x1);  
        *(Rdata + rcolumn*idata + rcol) = x1;
      }

        idata++;
        
      } /* data line loop */
    *ndata = idata;
    return(0);

}
