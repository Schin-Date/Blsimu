#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>
#include        <const.h>

static double kappa, focus, s1, s2, thet1, thet2;


/*=======================================================================================*/
int set_Lprof(FILE *logstr, double Llambda, double laser_waist, double laser_focus,
                                double z1, double z2 ){

         kappa = Llambda / PAI / laser_waist / laser_waist / 1000.;  /* meter^-1 */
         focus = laser_focus;
         s1 = z1;  s2 = z2;
         thet1 = atan( (s1 - focus) * kappa );
         thet2 = atan( (s2 - focus) * kappa );
         
         return(0);
}
/*=======================================================================================*/
int scat_points(FILE *logstr, double *X4, int Npcl, char *cntl){
int i;
double gzi, theta;
double  getrandom(); 
void    setrandom(int);


       if( strstr(cntl,"midpoint") != NULL ){
         for(i=0;i<Npcl;i++){
         *(X4 + 4*i + 0) = 970.;  /* s[m] = 970. for all particles*/
         *(X4 + 4*i + 3) = 970.;  /* s[m] = 970. for all particles*/
         }
       }
       else if(strstr(cntl,"focal") != NULL){
         for(i=0;i<Npcl;i++){
         *(X4 + 4*i + 0) = focus; 
         *(X4 + 4*i + 3) = focus; 
         }
       }
       else if(strstr(cntl,"dist") != NULL){
         for(i=0;i<Npcl;i++){
           gzi = getrandom();
           theta = thet1 + (thet2 - thet1)*gzi;
         *(X4 + 4*i + 0) = focus + tan(theta)/kappa;  
         *(X4 + 4*i + 3) = *(X4 + 4*i + 0); 
/*printf("%f %e %e\n",gzi,kappa, theta);*/
         }
       }
       else{
        fprintf(logstr,"E...scat_points: No instruction for cntl = %s\n",cntl);
        exit(1);
        }
#ifdef DEBUG4
   fprintf(logstr,"D4...scat_points: chao\n");
#endif   

   return(0);
}
