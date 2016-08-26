/*                                       created on 22 Oct. 1996 by S.D.@SP8 */
/*                                      modified on 25 Nov. 1996 by S.D.@SP8 */
/*                                      modified on 26 Nov. 1996 by S.D.@SP8 */
/*                                      modified on 27 Nov. 1996 by S.D.@SP8 */
/*----------------------------------------------------------*/
/*                       Testing                            */
/*               Random Number Generators                   */
/*----------------------------------------------------------*/
/*  compile with 
              histog minmax  besides the generator.
*/
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
/*#include "/Users/schin/Data/notes/clib/head/const.h"*/
#include <const.h>
/*============================================================================*/
int getrandom_gauss(double sigma, double mean, double max, 
                                                   double *r1, double *r2){
double getrandom();
void setrandom(int);                                                   
double dmax, gzi, amp, phi;                                                   

      dmax = fabs(max - mean);
      gzi = getrandom();
      if(gzi < R4UNDR ){  amp = R4OVER; }
      else{ amp = sqrt( - 2. * log( gzi ) ) * sigma; }
      amp = dmax * amp / (amp + dmax);
      gzi = getrandom();
      phi = 6.283185307 * gzi;
                   #ifdef DEBUG99
                   printf("%f ",gzi);
                   #endif
      *r1 = amp * cos( phi ) + mean;
      *r2 = amp * sin( phi ) + mean;

    return(0);
}


#if(0)
   SUBROUTINE  GAUSS ( SIGMA, MEAN, MAX, R1, R2 )
      COMMON /MACNTL/ R4OVER,R4UNDR
      REAL MEAN, MAX
      DMAX = ABS( MAX - MEAN )
      GZI = RND(1)
      IF( GZI .LT. R4UNDR ) THEN
      AMP = R4OVER
      ELSE
      AMP = SQRT( - 2. * DLOG( GZI ) ) * SIGMA
      ENDIF
      AMP = DMAX * AMP / ( AMP + DMAX )
      PHI = 6.283185307 * RND ( 1 )
      R1 = AMP * COS( PHI )  + MEAN
      R2 = AMP * SIN( PHI )  + MEAN
      RETURN
      END
#endif


#ifdef DEBUG99
void main(){
double sig, aver, maximum;
double x,y;
int n,i,status;

        sig = 1.;  aver = 30; maximum = R4OVER;
              n= 100;
              for(i=0;i<n;i++){
              status = getrandom_gauss(sig,aver,maximum,&x,&y);
              printf("%f %f\n",x,y); }
           printf("sig=%f mean=%f max = %e\n",sig,aver,maximum);
}
#endif




