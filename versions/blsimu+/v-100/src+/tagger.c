/*
070301
- abondone X4-P4 scheme in this routine. Assume each particle is inquired. 
  This routine is called when the particle coordinates satisfy conditions 
given in the main.c. Evaluate positions on the tagger anyway. Set iftag = 0
when there is no crossing of the particle.
  Particle coordinates should be given in the B-frame where the position of
tagger is defined.


*/
#include <stdio.h>  /* for printf,fgets NULL */
#include <stdlib.h>  /* for exit, malloc */
#include <ctype.h> /* for tolower */
#include <string.h> /* for strcpy */
#include <math.h>   /* to use sqrt */
#include <time.h>
//#include <const.h>
#include "main.h"

/* Valules of npoints, ztagger[] and xtagger[] are read in read_tagger()
from a file specified in a character string "tagger." */
static int npoints;
static double ztagger[3000], xtagger[3000];

/*===========================================================================*/
int get_tagger(FILE *logstr, double *X4, double *P4, int npcl, int *iftag){

double x0, xp0, y0, yp0, x1, xp1, y1, yp1, x2, xp2, y2, yp2, x, xp, y, yp;
double el, pel, px,py,pz,pt2,pp2;
double z0,  z1,  z2;
double zstep, xtag, xtag1;
int ipcl, ielm, i,j,k;
static double xU, zU, xL, zL;
static int ncall=0;
  ncall++; 

    *iftag = 0;

/*    *iftag = 1;
    if( ncall > 0 ) return(0);
*/

#ifdef DBGtagger1
  if(ncall==1) fprintf(logstr,"D...get_tagger:npoints=%d\n",npoints);
#endif


      if(npcl > 1){
          fprintf(logstr,"E...get_tagger:accept-reject scheme of particles is NR\n");
          exit(99);
          }

    if(ncall==1){
      xU = xtagger[0];
      zU = ztagger[0];
      if(xtagger[npoints-1] > xU){
            xL = xU;
            zL = zU;
            xU = xtagger[npoints-1];
            zU = ztagger[npoints-1];
      }
      else{
            xL = xtagger[npoints-1];
            zL = ztagger[npoints-1];
      }


#ifdef DBGtagger1
   fprintf(logstr,"D...get_tagger:xL=%e zL=%e xU=%e zU=%e\n",xL,zL,xU,zU);
#endif

     }
      for(ipcl = 0; ipcl<npcl; ipcl++){   /*<<<<<<<<<<< particle loop */
         el = *(P4 + 4*ipcl + 0);
         pz = *(P4 + 4*ipcl + 3);
         px = *(P4 + 4*ipcl + 1);
         py = *(P4 + 4*ipcl + 2);

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
         z0 = *(X4 + 4*ipcl + 3);

/** particles are assumed given in the B frame **/

        x1 = x0 + xp0* (zL - z0);
        if( pz > 0. && x1 < xL ){ *iftag=0; return(0);} 
        if( pz < 0. && x1 > xL ){ *iftag=0; return(0);} 
        x1 = x0 + xp0* (zU - z0);
        if( pz > 0. && x1 > xU ){ *iftag=0; return(0);} 
        z1 = zU;
        xtag = xU;
        xtag1 = xU;
#ifdef DBGtagger1
   if(ncall<2){
   fprintf(logstr,"D...get_tagger <while:x1=%e z1=%e xtag =%e \n",x1, z1, xtag, zstep );}
#endif

   /* the following scheme is prepared only for pz > 0. */   
        if( pz < 0. ){ 
          fprintf(logstr,"E...tagger: not prepared for negative pz = %e\n",pz);
         }

        zstep = -0.2;
     /*while(fabs(zstep) > 1.e-4){*/
     while(fabs(xtag-x1) > 1.e-4){

   /* x1 is particle x coordinate at z = zU */
   /* It is assumed that the tagger lays above the point (zU, x1) */
        while( x1 < xtag ){
         z2 = z1 + zstep;
         x2 = x1 + xp0 * zstep;
         x1=x2; z1=z2;
         xtag1 = xtag;

            for(i=0; i<npoints; i++){
                if( z1 < ztagger[i]) continue;
                break;
               }
        if(i==0){ fprintf(logstr,"E...get_tagger: <z1=%e\n",z1); exit(5);}

       xtag= xtagger[i-1] 
        +  (xtagger[i]-xtagger[i-1])*(z1 - ztagger[i-1])/(ztagger[i]-ztagger[i-i]);

#ifdef DBGtagger1
   if(ncall<2){
   fprintf(logstr,"D...get_tagger in while:x1=%e z1=%e xtag =%e zstep =%e\n",x1, z1, xtag, zstep );}
#endif


       } /* x1 < xtag  */

         z2 = z1 - zstep;
         x2 = x1 - xp0 * zstep;
         x1=x2; z1=z2;
         xtag=xtag1;        
        zstep = zstep /10.; 


     } /* while step> */

#ifdef DBGtagger2
    if(ncall < 10){
   fprintf(logstr,"D...get_tagger:x1=%e z1=%e xtag =%e zstep=%e\n",x1, z1, xtag, zstep );}
#endif
      

         *(X4 + 4*ipcl + 3) = z1;
         *(X4 + 4*ipcl + 1) = x1;
        y1 = y0 + yp0* (z1 - z0);
         *(X4 + 4*ipcl + 2) = y1;
          *iftag = 1;

      }/* loop of particles */
    return(0);
}
/*===========================================================================*/
int read_tagger(FILE *logstr, char const *infile){
FILE *instr;
int nline , iskip, rcolumn, scolumn, icolumn;
int ndata;
int *Idata;
double *Rdata;
char **Sdata;

int status, i, j, l;
char buffer[240]="";

      instr=fopen(infile,"r");
#ifdef DEBUG_FILE
      fprintf(logstr,"D_FILE...read_tagger : about to read a tagger file =%s\n",infile);
#endif
    printf("...read_tagger: R4OVER/UNDR = %e/%e\n",R4OVER,R4UNDR);//<<<<<<<<<<<<<<<<<<<<
      iskip = 0;
      nline = 100; scolumn = 0; icolumn = 0; rcolumn = 2;
           Rdata = calloc(nline*rcolumn,sizeof(double));

      status =  eat_table_tagger(logstr, instr, iskip, nline,
                          rcolumn,
                          &npoints, Rdata );

      fclose(instr);
            if(status != 0){
             fprintf(logstr,"E...read_tagger< eat_table_tagger"
                 "status = %d\n",status);
             exit;
            }

      fprintf(logstr,"N...read_tagger < eat_table_tagger: ndata = %d\n",npoints);

     j = 0;
     for(i=0;i<npoints;i++){
             ztagger[j] =  *(Rdata+ rcolumn*i + 0);  /* z[m] */
             xtagger[j] =  *(Rdata+ rcolumn*i + 1);  /* x[m] */
             j++;
     }
       npoints = j;

        free(Rdata);


}
/*===========================================================================*/
int eat_table_tagger(FILE *logstr, FILE *instr, int iskip, int nline,
             int rcolumn,
            int *ndata, double *Rdata){

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
           fprintf(logstr,"E...eat_table_tagger:"
                 "idata = %d > nline = %d\n",idata,nline);
                     exit(1);
                               }

        i = strlen(buffer);
        if( i <= 1 ) continue;  /* skip blank lines */
        strcpy(buffer2, buffer);  /* to preserve buffer */
     for(rcol =0; rcol<rcolumn; rcol++){
        if(rcol==0)   a = strtok(buffer2," ");
        else a = strtok(NULL," ");
        sscanf(a,"%lf",&x1);
        *(Rdata + rcolumn*idata + rcol) = x1;
      }
/*printf("%d %e\n",idata, *(Rdata + rcolumn*idata + 0) );*/

        idata++;

      } /* data line loop */
    *ndata = idata;
    return(0);

}













