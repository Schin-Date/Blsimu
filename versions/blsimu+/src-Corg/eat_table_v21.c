/* If you are seeing this line, this file has been copied from clib/asource/eat_table_v21.c
      
                       The eat_table program for data files VERSION 21
			==================================================================
			                      ver 21 created on 100911 by S.D.@SP8
			                      ver 20 created on 100827 by S.D.@SP8

To do: accept arbitrary deliminators through dstruct[] (need to decompose without strtok)

Update:
   100911 Data in a line may be separated by an arbitrary deliminator.
   100827 The main body of eat_table() is revised to correspond to the arbitrary order 
        of appearance of each data type in the input. This module will be a member of
		the libschin.a. The old version is kept as clib/Old/eat_table_str.c
		
   051030 For the moment, it is necessary to modify the body of program according to 
        the order of appearance of each data type in the input. Therefore I will NOT
        include this and the eat_table_num programs in libschin.a until the code is
        generalized to arbitral orders.

    The first version is created on Mar. 05, 2004 by S.D.@SP8         


Usage:
		The data table to be read is composed of string, integer and real number columns 
	in an arbitrary order.

    1    2   ...ncolumn
1   str int str real real ....     
2   data line 
3   data line 
.   ...
nline  ....

example:
          fring effect taken into account as Q 
          nu_x = 40.146101, nu_y = 18.348601 
 elm magid cellOLD monOLD cell mon name            flag         s      betax      alphx        nux       etax      etapx      betay      alphy        nuy       etay      etapy
 MON    0  1  1  1  1 #######                        t     1.8200    26.9542    -2.7508    0.01157     0.0013     0.0001     7.7809     0.1222    0.04001     0.0000     0.0000
 SEX 2001  0  0  1  0 sr_mag_magnet_sx_1_1           t     2.1300    28.6902    -2.8493    0.01335     0.0014     0.0001     7.7176     0.0818    0.04638     0.0000     0.0000
....

        (1) Define char dstruct[] as shown in the following example. 
		(2) specify data separator
        (3) specify nline, iskip and number of columns for each data type.

Running example routine:
    make -f eat_table_example.make
    exe
	diff eat_table.out eat_table_example_input.txt
	       or
	diff eat_table.out eat_table_example2_output.txt 
    make -f eat_table_example.make clean
	
*/
#include <stdio.h>  /* for printf,fgets NULL */
#include <stdlib.h>  /* for exit, malloc */
#include <ctype.h> /* for tolower */
#include <string.h> /* for strcpy */
#include <math.h>   /* to use sqrt */
#include <time.h>
/*===========================================================================*/
#ifdef CHK
void main(){
FILE *instr, *outstr, *logstr;
char infile[80], outfile[80];
double tunex, tuney;
#ifdef EXPLE2
/*"SEX 2001  0  0  1  0 sr_mag_magnet_sx_1_1           t     2.1300    28.6902    -2.8493    0.01335     0.0014     0.0001     7.7176     0.0818    0.04638     0.0000     0.0000";*/
char dstruct[]="a4 i4 i1 i1 i1 i1 a28 a1 11r10.4";
#else
/* BMP  0  1 0.79500000 0.00000000 0.00000000 0.00000000 6003 22.8729 -0.1263 0.0444 0.0056 0.1067 0.0000  7.0572 -0.5066 0.1781 0.0189 0.0000 0.0000
    %-3.3s %2d %2d %5.8f %2.8f %5.8f %5.8f %4d %3.4f %3.4f %3.4f %3.4f %3.4f %3.4f  %3.4f %3.4f %3.4f %3.4f %3.4f %3.4f  */
char dstruct[]="a4 2i2 4r5.8 1i4 12r3.4";       /*<<<<<<<<<<< specify data types of a row (1)  */
#endif
int nline , iskip, rcolumn, scolumn, icolumn;
int ndata;
int *Idata;
double *Rdata;
char **Sdata;
int status, i;
char buffer[240]="";
char delim[]=" ";     /*<<<<<<<<<<< specify data separator (2)  */
/* defaults */
      outstr=fopen("eat_table.out","w"); 
      logstr = stdout;

/*** User settings ***/
/*********************/
#ifdef EXPLE2
      strcpy(infile,"eat_table_example2_input.txt");
#else
	  strcpy(infile,"eat_table_example_input.txt");
#endif
      instr=fopen(infile,"r");    

      fprintf(logstr,"N...main: Read the input file %s...\n",infile);
#ifdef EXPLE2
      nline = 2000; iskip = 3; scolumn = 3; icolumn = 5; rcolumn = 11;
#else	  
      nline = 5000; iskip = 1; scolumn = 1; icolumn = 3; rcolumn = 16;  /*<<<<<<<< specify nline, iskip and number of columns (3) */
#endif
           Idata = calloc(nline*icolumn,sizeof(int));
           Rdata = calloc(nline*rcolumn,sizeof(double));
           Sdata = calloc(nline*scolumn,sizeof(char*));  
           for(i=0;i<nline*scolumn;i++){
              *(Sdata+i) = calloc(64,sizeof(char));}
 
  
      status =  eat_table(logstr, instr, dstruct, iskip, nline, 
                          icolumn, rcolumn, scolumn, delim,
                          &tunex, &tuney,
                          &ndata, Idata, Rdata, Sdata );
            if(status != 0){  
             fprintf(logstr,"E...main< eat_table_str"
                 "status = %d\n",status);
             exit; 
            }
      fprintf(logstr,"N...main: eat_table: ndata = %d\n",ndata);
#ifdef READ_TUNES	  
      fprintf(logstr,"N...main: tunex=%f, tuney=%f\n", tunex, tuney); 
#endif	  
#ifdef EXPLE2
/*"SEX 2001  0  0  1  0 sr_mag_magnet_sx_1_1           t     2.1300    28.6902    -2.8493    0.01335     0.0014     0.0001     7.7176     0.0818    0.04638     0.0000     0.0000";*/
for(i=0;i<ndata;i++){
    fprintf(outstr,"%s %s %s ",
             *(Sdata + scolumn*i + 0),
             *(Sdata + scolumn*i + 1),
             *(Sdata + scolumn*i + 2)
             );
fprintf(outstr,"(%d %d %d %d %d) ",
             *(Idata+ icolumn*i + 0),
             *(Idata+ icolumn*i + 1),
             *(Idata+ icolumn*i + 2),
             *(Idata+ icolumn*i + 3),
             *(Idata+ icolumn*i + 4)
             );
fprintf(outstr,"(%f %f %f %f %f %f)\n",
             *(Rdata+ rcolumn*i + 0),
             *(Rdata+ rcolumn*i + 1),
             *(Rdata+ rcolumn*i + 3),
             *(Rdata+ rcolumn*i + 6),
             *(Rdata+ rcolumn*i + 8),
             *(Rdata+ rcolumn*i + 10)
             );
}
#else
/*"%-3.3s %2d %2d %5.8f %2.8f %5.8f %5.8f %4d %3.4f %3.4f %3.4f %3.4f %3.4f %3.4f  %3.4f %3.4f %3.4f %3.4f %3.4f %3.4f*/
for(i=0;i<ndata;i++){
    fprintf(outstr,"%-3.3s ",
             *(Sdata + scolumn*i + 0)             );
fprintf(outstr,"%2d %2d ",
             *(Idata+ icolumn*i + 0),
             *(Idata+ icolumn*i + 1)             );
fprintf(outstr,"%5.8f %2.8f %5.8f %5.8f ",
             *(Rdata+ rcolumn*i + 0),
             *(Rdata+ rcolumn*i + 1),
             *(Rdata+ rcolumn*i + 2),
             *(Rdata+ rcolumn*i + 3)
             );
fprintf(outstr,"%4d ",
             *(Idata+ icolumn*i + 2)             );
			 
fprintf(outstr,"%3.4f %3.4f %3.4f %3.4f %3.4f %3.4f  %3.4f %3.4f %3.4f %3.4f %3.4f %3.4f\n",
             *(Rdata+ rcolumn*i + 4),
             *(Rdata+ rcolumn*i + 5),
             *(Rdata+ rcolumn*i + 6),
             *(Rdata+ rcolumn*i + 7),
             *(Rdata+ rcolumn*i + 8),
             *(Rdata+ rcolumn*i + 9),
             *(Rdata+ rcolumn*i + 10),
             *(Rdata+ rcolumn*i + 11),
             *(Rdata+ rcolumn*i + 12),
             *(Rdata+ rcolumn*i + 13),
             *(Rdata+ rcolumn*i + 14),
             *(Rdata+ rcolumn*i + 15)
             );
}
#endif
      free(Idata);
      free(Rdata);
      for(i=0;i<nline*scolumn;i++){ free(*(Sdata+i));}
      free(Sdata);

}
#endif
/*===========================================================================*/
int eat_table(FILE *logstr, FILE *instr, char *dstruct, int iskip, int nline, 
             int icolumn,  int rcolumn, int scolumn, char *delim,
             double *tunex, double *tuney,
            int *ndata, int *Idata, double *Rdata, char **Sdata){

int i, j, k,  icol, rcol, scol;
int idata, itag, i1;
int *kfmt, *mlp;
char buffer[240]="", buffer2[240]="", *a, *b;  
char s[64];
char **dfmt;


/*---skip header lines ---*/
    i = 0;
    while( i < iskip ){
      fgets(buffer,sizeof(buffer),instr);
#ifdef SHOW_SKIP
	  if(i==0){
	  fprintf(logstr,"N...eat_table: Skipped lines...\n");
	  fprintf(logstr,"%d:'%s'\n",i,buffer); }
#endif
      i++;
#ifdef READ_TUNES
      if(i==2){ 
      sscanf(buffer,"          nu_x = %9lf, nu_y = %9lf",tunex,tuney); }
#endif
    }
      fprintf(logstr,"D...eat_table: %d lines skipped\n", iskip);
/******************************************/
/****** Raw analysis (data structure) *****/
/******************************************/
	      itag = icolumn+rcolumn+scolumn;
/*-(0) count itag -*/
          strcpy(buffer,dstruct);		  
#ifdef DBG_eat_table_full
printf("D...eat_table:(b0) itag=%d buffer:'%s'\n",itag,buffer);
#endif		  		   
		  a = strtok(buffer," ");
           for(i=0;i<itag;i++){
			  if((a = strtok(NULL," "))==NULL) break; 
           }
		  itag = i+1;
#ifdef DBG_eat_table_full
printf("D...eat_table:(a0) itag=%d buffer:'%s'\n",itag,buffer);
#endif		  		   
/*-(1) store element words in dstruct to dfmt -*/
		  mlp= calloc(itag,sizeof(int));
		  kfmt= calloc(itag,sizeof(int));
          dfmt = calloc(itag,sizeof(char*));  
          strcpy(buffer,dstruct);		  
		  a = strtok(buffer," ");
           for(i=0;i<itag;i++){
			  i1 = strlen(a);
#ifdef DBG_eat_table_full
printf("%d:a(%d):'%s' dfmt about to be defined",i,i1,a);
#endif		  
              *(dfmt+i) = calloc(i1,sizeof(char));
			  strcpy(*(dfmt+i),a);
#ifdef DBG_eat_table_full
printf("...done\n");
#endif		  
#ifdef DBG_eat_table_full
printf("D...eat_table: i=%d a(%d):%s dfmt:%s \n",i, strlen(a), a, *(dfmt+i));
#endif		  
			  if((a = strtok(NULL," "))==NULL&&i<itag-1){
              fprintf(logstr,"E...eat_table: a==NULL at i=%d,*(dfmt+i):'%s',dstruct:'%s'"
			                     ,i,*(dfmt+i),dstruct);
			          exit(1);
			  }; 
           } /*<********i< itag loop (1)*****/
#ifdef DBG_eat_table
printf("D...eat_table: *****Raw analysis***** loop(1) passed: i=%d \n",i);
#endif		  		   
/*-(2) analyse each element word -*/
           for(i=0;i<itag;i++){
/*	          if(*(dfmt+i)==NULL) break;*/
              strcpy(s,*(dfmt+i));
			  *(kfmt+i) = 0;
			  if(strstr(s, "a") != NULL) *(kfmt+i) = 1;
		      else if(strstr(s, "r") != NULL) *(kfmt+i) = 2;
			  else if(strstr(s, "i") != NULL) *(kfmt+i) = 3;
			   switch(*(kfmt+i)){
			    case 1:
				  a = strtok(s,"a");
 			      break;
			    case 2:
				  a = strtok(s,"r");
			      break;
			    case 3:
				  a = strtok(s,"i");
			      break;
			    default:
				  fprintf(logstr,"E...eat_table: a:%s at i=%d of dstruct:'%s'\n",a,i,dstruct);
				  exit(1);
	          }/** switch **/			  
		/** find multiplicity **/	  
			   *(mlp+i)=1;
			   if(a!=NULL){
			   if(strlen(a)+1!=strlen(s)){
                     sscanf(a,"%d",(mlp+i));
			   }}
#ifdef DBG_eat_table_full
printf("D...eat_table: i=%d dfmt:%s case%d a:%s mlp=%d\n",i, *(dfmt+i),*(kfmt+i),a, *(mlp+i));
#endif		  
           } /*<********i< itag loop (2)*****/			  
#ifdef DBG_eat_table
printf("D...eat_table: *****Raw analysis***** loop(2) passed:\n");
           for(i=0;i<itag;i++){
printf("D...eat_table: i=%d dfmt:%s kfmt=%d mlp=%d\n",i, *(dfmt+i),*(kfmt+i),*(mlp+i));
           }
#endif		  

/************************************************/
/*************** Data Line Loop *****************/
/************************************************/
/*---loop of reading/printing---*/
    idata = 0;
    while( ( fgets(buffer,sizeof(buffer),instr) )!=NULL ){

        if(idata > nline-1){
           fprintf(logstr,"E...eat_table:"
                 "idata = %d > nline = %d\n",idata,nline);
                     exit(1); 
                               }

        i = strlen(buffer);
        if( i <= 1 ) continue;  /* skip blank lines */
        strcpy(buffer2, buffer);  /* to preserve buffer */

        icol = 0; rcol = 0; scol = 0;

/*  The first item should be taken from buffer2 separately placing the first argument of strtok */
/*100911        a = strtok(buffer2," "); */
        a = strtok(buffer2,delim);

		for(j=0; j<itag; j++){
			for(k=0; k<*(mlp+j); k++){
			   strcpy(s,a);
			   switch(*(kfmt+j)){
			    case 1: /* a */
                  strcpy(*(Sdata + scolumn*idata + scol),s); 
                  scol++;
 			      break;
			    case 2: /* r */
				  sscanf(s,"%lf",(Rdata + rcolumn*idata + rcol));  
                  rcol++;
			      break;
			    case 3: /* i */
                  sscanf(s,"%d",(Idata + icolumn*idata + icol));  
				  icol++;
			      break;
			    default:
				  fprintf(logstr,"E...eat_table (data loop, switch): idata=%d, j=%d kfmt:%d a:'%s'\n",
				            idata, j, *(kfmt+j),a);
				  exit(1);
	           }/** switch **/			  
/*  The following items should be taken with NULL as the first argument of strtok */
/*100911			   if((a = strtok(NULL," "))==NULL&&(k<*(mlp+j)&&j<itag-1)){ */
			   if((a = strtok(NULL,delim))==NULL&&(k<*(mlp+j)&&j<itag-1)){ 
					   fprintf(logstr,"E...eat_table: a==NULL at idata=%d j=%d,k=%d s:'%s'"
						,idata,j,k,s);
						exit(1);
						} /* a==NULL Error */				  
			} /* multiplicity loop */
		} /*-- loop of column j --*/
#ifdef DBG_eat_table
if(idata==0)
printf("D...eat_table: *****Data read***** idata==0: loop(column) passed:\n");
#endif		  		
        idata++;
        
      } /* data line loop */
    *ndata = idata;
    return(0);
}
