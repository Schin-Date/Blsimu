#include <stdio.h>  /* for printf,fgets NULL */
#include <time.h> /* for ctime */
#include <stdlib.h>  /* for exit, malloc */
#include <math.h>   /* to use sqrt */
#include <string.h> /* for strcpy */
/*#include "/Users/schin/Data/notes/clib/head/const.h" /* for PAI, R4OVER, EMASS and so on */
#include <const.h> /* for PAI, R4OVER, EMASS and so on */
/*                     
        Fitting data points by Q

051025  coded  iBook:~/note/clib/fit

/********************************************/
#ifdef CHK
void main(){
double x,y;
int i,status;
/* data */
double xval[1000], fval[1000];
/* eat_table*/
int ncolumn=2, nline = 1000, iskip = 0;
int ndata;
double *Rdata;
/* IO */
FILE *instr, *outstr, *logstr;
char infile[80], outfile[80];
time_t time_pointer;

/* defaults */
      logstr = stdout;
      strcpy(infile,"testin.dat");
      strcpy(outfile,"testout.dat");
      outstr = fopen(outfile,"w"); 
/*      outstr = fopen(outfile,"a"); */

      time(&time_pointer);
      fprintf(logstr,"present time = %.24s\n",ctime(&time_pointer));
      fprintf(outstr,"present time = %.24s\n",ctime(&time_pointer));

      
/***** read data *****/
      instr = fopen(infile,"r"); 
            Rdata = calloc(nline*ncolumn,sizeof(double));
            status =  eat_table_real(logstr, instr, iskip, nline, ncolumn, 
                                          &ndata, Rdata);
                  if(status != 0){  
                    fprintf(logstr,"E...main< eat_table_real"
                                    "status = %d\n",status);
                          exit(1); 
                                 }

      for(i=0;i<ndata;i++){
          xval[i] = *(Rdata+ncolumn*i+0);
          fval[i] = *(Rdata+ncolumn*i+1); 
       }

      free(Rdata);
      fclose(instr);

 
     for(i=0; i< 2280; i++){
         x = (double)(350*i)/2280.;
         qfit(logstr, ndata, xval, fval, x, &y, "hviope");
         fprintf(outstr,"%f %e\n",x,y);       
    }
/*    
     x = 50;
     qfit(logstr, ndata, xval, fval, x, &y, "hviope");
     fprintf(logstr,"CHK... y(%f) = %e\n\n",x, y);
*/ 
                   #ifdef DEBUG9
                   for(i=0;i<ndata;i++){
                   fprintf(outstr,"D9...%d %e %e\n",i,xval[i],fval[i] );
                   }
                   #endif

      fclose(outstr);
 

}
#endif
/******************************************************************************* 051031***/
/* 
*/
int qfit(FILE *logstr, int ndata, double *xd, double *fd, double x, double *estimate, char *cntl){
/* coefs */
static int id=0;
static double *coe0, *coe1, *coe2;
/* local */
double d0, d1, d2;
int i, istep;
int i0,i1,i2;
double x0,x1,x2;
double f0,f1,f2;
double c0, c1, c2;

      #ifdef DEBUG
      fprintf(logstr,"D...qfit: ndata=%d x = %f\n",ndata, x);
      #endif
      #ifdef DEBUG8
      for(i=0;i<3;i++){
          fprintf(logstr,"%d %e %e\n",i+1,*(xd+i),*(fd+i) );
      }
          fprintf(logstr," ......\n");
      for(i=ndata-3;i<ndata;i++){
          fprintf(logstr,"%d %e %e\n",i+1,*(xd+i),*(fd+i) );
      }
      #endif
      #ifdef DEBUG7
         fprintf(logstr,"D7...begin: id = %d  x[id]=%f\n",id,xd[id]);
      #endif


/* find the nearest data point */
     d0 = x - xd[id];
     istep = 1;
     if(d0 < 0.){ istep = -1;}
     d0 = fabs(d0);
            #ifdef DEBUG7
              fprintf(logstr,"D7...proc d0: d0 = %f, istep=%d\n",d0,istep);
            #endif
     for( i = id + istep; 0 <= i && i <= ndata; i=i+istep){
        d1 = fabs(x - xd[i]);
              /*fprintf(logstr,"DD7...  loop: i= %d d1 = %f\n",i, d1);*/
        if( d1 < d0 ){ id= i; d0 = d1; continue;}
        break;
      }     
     
     #ifdef DEBUG7
         fprintf(logstr,"D7...  end: The nearest x[%d]=%f y=%e\n", id,xd[id],fd[id]);
     #endif


/*  choose a triplet */
    i1 = id;
    if( id == 0 ){ i1 = 1; }
    if( id == ndata ){ i1 = ndata-1; }
    i0 = i1 - 1;
    i2 = i1 + 1;
    x0 = *(xd + i0);
    x1 = *(xd + i1);
    x2 = *(xd + i2);
    f0 = *(fd + i0);
    f1 = *(fd + i1);
    f2 = *(fd + i2);
     #ifdef DEBUG6
         fprintf(logstr,"D6...triplet: i0=%d i1=%d i2=%d\n", i0,i1,i2);
         fprintf(logstr,"D6...triplet: x0=%f x1=%f x2=%f\n", x0,x1,x2);
         fprintf(logstr,"D6...triplet: f0=%e f1=%e f2=%e\n", f0,f1,f2);
     #endif

/*** Evaluate local coefficients ***/
/* numerators of the local coefficients */
    c2 = (x2-x1)*f0 + (x1-x0)*f2 + (x0-x2)*f1;
    c1 = (x2*x2-x1*x1)*f0 + (x1*x1-x0*x0)*f2 + (x0*x0-x2*x2)*f1;
    c0 = (x2-x1)*f0*x2*x1 + (x1-x0)*f2*x1*x0 + (x0-x2)*f1*x0*x2;
    
/* determinant */
    d0 = (x2 - x1)*(x1 - x0)*(x0 - x2);
    
/* The local coefficients */
   if( fabs(d0) > R4UNDR ){
      c2 = - c2 / d0;
      c1 =  c1 / d0;
      c0 = - c0 / d0;
   }
   else{
     f2 = 1.; f1 = 1.; f0 = 1.;  
     if(-c2 < 0){f2 = -1.;}
     if(c1 < 0){f1 = -1.;}
     if(-c0 < 0){f0 = -1.;}
     if(d0 < 0.){ f2 = -1.*f2; f1 = -1.*f1; f0 = -1.*f0; }
     c2 = f2*R4OVER; c1 = f1*R4OVER; c0 = f0*R4OVER; 
   }

/* when the nearest data point is the same as previous one, use that coeffs.*/

/*** Return the estimate ***/

      *estimate = c2 * x*x + c1 * x + c0;

      return(0);
 

}
