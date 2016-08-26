/*   


BASED ON   optics/beamshape/opt2beamshape.c
                                 cre. in Mar. 2004 by S.D.@SP8                

*/
#include <stdio.h>  /* for printf,fgets NULL */
#include <stdlib.h>  /* for exit, malloc */
#include <ctype.h> /* for tolower */
#include <string.h> /* for strcpy */
#include <math.h>   /* to use sqrt */
#include <time.h>
//#include <const.h>
#include "main.h"

static int npoints;
static double s1, s2;
static double s[3000], beta_x[3000], alpha_x[3000], gamma_x[3000], nu_x[3000], eta_x[3000], etap_x[3000];
static double beta_y[3000], alpha_y[3000], gamma_y[3000], nu_y[3000], eta_y[3000], etap_y[3000];

/*===========================================================================*/
int read_opt(FILE *logstr, char const *infile, double smin, double smax){
FILE *instr;
double emittance_x, emittance_y, x;
double tunex, tuney;
int nline , iskip, rcolumn, scolumn, icolumn;
int ndata;
int *Idata;
double *Rdata;
char **Sdata;

int status, i, j, l;
char buffer[240]="", tunefmt[]="*** tune(cal) = (%9lf, %9lf";

/*** set region to store data ***/
     s1 = 946.980; s2 = 976.895;    /* one normal cell around BL33*/
     s1 = 946.980; s2 = 980.;    /* one normal cell around BL33*/
     s1 = smin; s2 = smax;

/********************************/
/******* Read optics file *******/
/*********************************/

              emittance_x = 6.4e-9; emittance_y= 1.e-3 * emittance_x;
              strcpy(tunefmt,"*** tune(cal) = (%9lf, %9lf");
              iskip = 12;
              iskip = 40;  /* 060525 */
              iskip = 47;  /* 110310 */


      instr=fopen(infile,"r");    
//141021    printf("...read_opt: infile = '%s%\n", infile);
#ifdef DEBUG
      fprintf(logstr,"D...read_opt: about to read an optics file =%s\n",infile);
#endif
      nline = 15000; scolumn = 0; icolumn = 0; rcolumn = 13;
           Rdata = calloc(nline*rcolumn,sizeof(double));

      status =  eat_table_optics_all(logstr, instr, iskip, nline,
                          rcolumn, tunefmt,
                          &tunex, &tuney,
                          &npoints, Rdata );

      fclose(instr);
            if(status != 0){  
             fprintf(logstr,"E...read_opt< eat_table_optics_all"
                 "status = %d\n",status);
             exit; 
            }
      fprintf(logstr,"N...read_opt < eat_table_optics: ndata = %d\n",npoints);
      fprintf(logstr,"N...read_opt : tunex=%f, tuney=%f\n", tunex, tuney);  

     j = 0;
     for(i=0;i<npoints;i++){
             x =  *(Rdata+ rcolumn*i + 0);  /* s[m] */
           if( x <= s2+.1 && x >= s1-.1 ){
             if( j > 3000 ){ fprintf(logstr,"E...read_opt: j= %d\n",j); exit;}
             s[j] =  x;  /* s[m] */
             beta_x[j] =  *(Rdata+ rcolumn*i + 1);  /* betax[m] */
             alpha_x[j] =  *(Rdata+ rcolumn*i + 2);  /* alpha_x */
             gamma_x[j] =  *(Rdata+ rcolumn*i + 3);  /* gamma_x */
             nu_x[j] =  *(Rdata+ rcolumn*i + 4);  /* nux */
             eta_x[j] =  *(Rdata+ rcolumn*i + 5);  /* eta_x */
             etap_x[j] =  *(Rdata+ rcolumn*i + 6);  /* etap_x */
             beta_y[j] =  *(Rdata+ rcolumn*i + 7);  /* beta_y */
             alpha_y[j] =  *(Rdata+ rcolumn*i + 8);  /* alpha_y */
             gamma_y[j] =  *(Rdata+ rcolumn*i + 9);  /* gamma_y */
             nu_y[j] =  *(Rdata+ rcolumn*i + 10);  /* nuy */
             eta_y[j] =  *(Rdata+ rcolumn*i + 11);  /* eta_y */
             etap_y[j] =  *(Rdata+ rcolumn*i + 12);  /* etap_y */
             j++;
            }
     }

       npoints = j;
       
      #ifdef DEBUG21
         for(i=0;i<2;i++){
           fprintf(logstr,"D21...%d %f %f %f %f %f %f %f\n"
                          "           %f %f %f %f %f %f\n",i,
            s[i], beta_x[i], alpha_x[i], gamma_x[i], nu_x[i], eta_x[i], etap_x[i],
                  beta_y[i], alpha_y[i], gamma_y[i], nu_y[i], eta_y[i], etap_y[i]);
                  }
         for(i=npoints-2;i<npoints;i++){
           fprintf(logstr,"D21...%d %f %f %f %f %f %f %f\n"
                          "           %f %f %f %f %f %f\n",i,
            s[i], beta_x[i], alpha_x[i], gamma_x[i], nu_x[i], eta_x[i], etap_x[i],
                  beta_y[i], alpha_y[i], gamma_y[i], nu_y[i], eta_y[i], etap_y[i]);
                  }
       #endif

        free(Rdata);
    
    
}
//printf("...read_opt:-------------here<<<<<<<<<<<<<<<<<<<<\n");
//}
#if(0)
#endif
/*===========================================================================*/
int eat_table_optics_all(FILE *logstr, FILE *instr, int iskip, int nline,
                         int rcolumn, char *tunefmt,
                         double *tunex, double *tuney,
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
        if(i==6){
            sscanf(buffer,tunefmt,tunex, tuney);
        }
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
//printf("...eat_table_optics_all:-------------here<<<<<<<<<<<<<<<<<<<<\n");
//}
#if(0)
#endif
