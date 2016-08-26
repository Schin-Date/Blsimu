#include <stdio.h>  /* for printf,fgets NULL */
#include <time.h> /* for ctime */
#include <stdlib.h>  /* for exit, malloc */
#include <math.h>   /* to use sqrt */
#include <string.h> /* for strcpy */
#include <const.h> /* for PAI, R4OVER, EMASS and so on */
/*                     
        Fitting data points on 2 dimensional mesh by Q

061207 coded on miho
051025  coded  iBook:~/note/clib/fit
/********************************************/
#ifdef CHK
#define NMESHX 25
#define NMESHY 380
void main(){
double x,y,f;
int i,j,status;
double y0;
/* data */
double xval[NMESHX], yval[NMESHY], fval[NMESHX*NMESHY];
double dmeshx, dmeshy; /* mesh accuracy */
/* eat_table*/
int ncolumn=5, nline = NMESHX*NMESHY, iskip = 1;
int ndata;
double *Rdata;
/* IO */
FILE *instr, *outstr, *logstr;
char infile[80], outfile[80];
time_t time_pointer;

/* defaults */
      logstr = stdout;
      strcpy(infile,"/home/3/schin/Public/SP8SR_bm_field");
      dmeshx = 1.e-9; dmeshy = 1.1e-2; /* mesh accuracy */
      strcpy(outfile,"chk.out");
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
         fprintf(logstr,"N...main< eat_table_real: ndata= %d\n",ndata);
                  if(status != 0){  
                    fprintf(logstr,"E...main< eat_table_real"
                                    "status = %d\n",status);
                          exit(1); 
                                 }
     j = 0; nline=0;
     while(j<NMESHY){
         if(nline == ndata -1){ break;}
       
      for(i=0;i<NMESHX;i++){
         nline = j*NMESHX + i;

         if(i==0){   yval[j] = *(Rdata+ncolumn*nline+2); }
             else{  y= *(Rdata+ncolumn*nline+2) - yval[j]; 
                 if( fabs(y) > dmeshy ){
                    fprintf(logstr,"E...main: (i,j)=(%d,%d) data=%f yval=%f\n", 
                                    i,j,*(Rdata+ncolumn*nline+2),yval[j] );
                          exit(3);                      } 
                 }


         if(j==0){    xval[i] = *(Rdata+ncolumn*nline+0); }
             else{  x = (*(Rdata+ncolumn*nline+0) - xval[i]);
                   if(  fabs(x) > dmeshx ){
                    fprintf(logstr,"E...main: (i,j)=(%d,%d) data=%f xval=%f\n", 
                                    i,j,*(Rdata+ncolumn*nline+0),xval[i] );
                          exit(2);} 
                  }

          fval[nline] = *(Rdata+ncolumn*nline+4); 

       }/* i loop */
              j++;

     }/* Y mesh while j loop */
     fprintf(logstr,"N...main: mesh(x,y) = (%d, %d) nline=%d\n",NMESHX,j,nline+1);


      free(Rdata);
      fclose(instr);

#ifdef CHKsingle
    x = -12.53;
    x = -10.23;
    x = 21.23;
    y = 3277.56; 
    y = -503.56; 
    y = 234.56; 
    y = 3.; 
    x = 23.;
    fprintf(logstr,"CHK...Estimating f(%f,%f) ...\n",x, y);
 qfit2d(logstr, NMESHX, NMESHY, xval, yval, fval, 
                      x, y, &f, "dummy");
     fprintf(logstr,"CHK...Result: f = %e\n\n", f);
#endif
#ifdef PrintTable
      dmeshy = (3283.27+502.795)/499.;
      dmeshx = 120. / 40.;
     fprintf(outstr,"y: 500pt in [-502.795, 3283.27], x: 41 pt in [-60, 60]\n");
      for(j=0;j<500;j++){
         y = -502.795+(double)j * dmeshy;
      for(i=0;i<41;i++){
         x = -60. + (double)i * dmeshx; 
     qfit2d(logstr, NMESHX, NMESHY, xval, yval, fval, 
                         x, y, &f, "dummy");
     /*fprintf(logstr,"%d %d %f %f %e\n", i, j,  x, y, f);*/
     fprintf(outstr,"%f %f %e\n", x, y, f);
#endif


      }
      }

      fclose(outstr);
}
#endif
/******************************************************************************* 061208**
    +++ x and y are not symmetric:
    series of data fd is ordered so that first proceed in the x direction at a fixed y
  then step to the next y point and proceed in x, and the same for the rest.
*/
int qfit2d(FILE *logstr, int nx, int ny, double *xd, double *yd, double *fd, 
                                double x, double y, double *estimate, char *cntl){

static int idx=0 ,idy=0;

/* local */
int idf;
int i, istep;
double d0, d1, d2;
int iy0, iy1, iy2;
double y0,y1,y2;
double f0,f1,f2;
double *fval;
double c0, c1, c2;
/* find the nearest data y point */
     d0 = y - yd[idy];
     istep = 1;
     if(d0 < 0.){ istep = -1;}
     d0 = fabs(d0);
            #ifdef DEBUG7
              fprintf(logstr,"D7...qfit2d: y start with d0 = %f at idy =%d, istep=%d\n",
                                     d0,idy, istep);
            #endif
     for( i = idy + istep; 0 <= i && i <= ny; i=i+istep){
        d1 = fabs(y - yd[i]);
              /*fprintf(logstr,"DD7...  loop: i= %d d1 = %f\n",i, d1);*/
        if( d1 < d0 ){ idy= i; d0 = d1; continue;}
        break;
      }     
     
     #ifdef DEBUG7
         fprintf(logstr,"D7... --->the nearest y mesh point at y[%d]=%f\n", idy,yd[idy]);
     #endif

/* find the nearest data x point */
     d0 = x - xd[idx];
     istep = 1;
     if(d0 < 0.){ istep = -1;}
     d0 = fabs(d0);
            #ifdef DEBUG7
              fprintf(logstr,"D7...qfit2d: x start with d0 = %f at idx =%d, istep=%d\n",
                                     d0,idx, istep);
            #endif
     for( i = idx + istep; 0 <= i && i <= nx; i=i+istep){
        d1 = fabs(x - xd[i]);
              /*fprintf(logstr,"DD7...  loop: i= %d d1 = %f\n",i, d1);*/
        if( d1 < d0 ){ idx= i; d0 = d1; continue;}
        break;
      }     

/* address of data for the nearest 2d mesh point */     
     idf = idy*nx + idx;
  /*----------------------*/ 
     #ifdef DEBUG7
         fprintf(logstr,"D7... --->the nearest x mesh point at x[%d]=%f\n", idx,xd[idx]);
         fprintf(logstr,"D7... data at the mesh point:  f(%d)=%e\n", idf, fd[idf]);
     #endif

/* choose a y triplet */
     iy1 = idy;
     if( idy == 0 ){ iy1 = 1; }
     if( idy == ny ){ iy1 = ny-1; }
     iy0 = iy1 - 1;
     iy2 = iy1 + 1;
     y0 = *(yd + iy0);
     y1 = *(yd + iy1);
     y2 = *(yd + iy2);

     fval = fd + nx*iy0; 
     qfit(logstr, nx, xd, fval, x, &f0, "dummy");
     fval = fd + nx*iy1; 
     qfit(logstr, nx, xd, fval, x, &f1, "dummy");
     fval = fd + nx*iy2; 
     qfit(logstr, nx, xd, fval, x, &f2, "dummy");

     #ifdef DEBUG8
         fprintf(logstr,"D8... : estimate for f0=%e, f1=%e, f2=%e\n", f0, f1, f2);
     #endif

/*** Evaluate local coefficients ***/
/* numerators of the local coefficients */
    c2 = (y2-y1)*f0 + (y1-y0)*f2 + (y0-y2)*f1;
    c1 = (y2*y2-y1*y1)*f0 + (y1*y1-y0*y0)*f2 + (y0*y0-y2*y2)*f1;
    c0 = (y2-y1)*f0*y2*y1 + (y1-y0)*f2*y1*y0 + (y0-y2)*f1*y0*y2;
    
/* determinant */
    d0 = (y2 - y1)*(y1 - y0)*(y0 - y2);
    
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

      *estimate = c2 * y*y + c1 * y + c0; 

      return(0);
 
}
