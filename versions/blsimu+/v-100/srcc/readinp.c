/* 110302: revised for a problem: 
        saho/gcc does not accept strcpy(name,a); for a=NULL.
  Todo:
  -- case vrbl appears more than onece in a line.
  -- case there exist multiple variables in a lhs. 
*/
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<time.h>
#include	<string.h>
#include        <const.h>
#define MAXNPR 128 
#define LPNAM 32
/*============================================================================*/
#ifdef DEBUGmain
void main(){
/*
    define integer, real and string parameters ipar, rpar and spar, respectively,
  with dimension MAXNPR for the maximum number of the parameters.
 *********************************************************************/
char pname[MAXNPR][LPNAM];
int ipar[MAXNPR];
double rpar[MAXNPR];
/*char spar[MAXNPR][240];*/
char **spar;
int i;
/* timesptamp */
time_t time_pointer;
/* files */
char outfile[80], inpfile[80];
FILE *logstr,*outstr;
#ifdef DEBUG
char *v, *a, vrbl[28], vtyp[8], buffer[240];
#endif

/** input file **/
         strcpy(inpfile,"Usetst.txt2");
         strcpy(inpfile,"Usetst.txt");
         strcpy(inpfile,"Usetst.txt1");
/** output file **/
         strcpy(outfile,"readinp-dbg.dat");
         /*outstr=fopen(outfile,"a");*/
         outstr=stdout;
/* defaults */
     logstr=stdout;
/* timesptamp */
      time(&time_pointer);
      fprintf(outstr,"present time = %.24s\n",ctime(&time_pointer));

      spar = calloc(MAXNPR,sizeof(char*));

     for(i=0;i<MAXNPR;i++){
         *(spar+i) = calloc(256,sizeof(char));
     }

     for(i=0;i<MAXNPR;i++){
         strcpy(pname[i],""); }

      i = 0;
      strcpy(pname[i],"int nbmpcl");  i++;
      strcpy(pname[i],"int Nevent");  i++;
      strcpy(pname[i],"double smin");  i++;
      strcpy(pname[i],"double smax");  i++;
      strcpy(pname[i],"double slastelm");  i++;
      strcpy(pname[i],"double pel0");  i++;
      strcpy(pname[i],"double e0spread");  i++;
      strcpy(pname[i],"double OVRVLR");  i++;
      strcpy(pname[i],"double CHMBDX");  i++;
      strcpy(pname[i],"double CHMBDY");  i++;
      strcpy(pname[i],"double emittance_x");  i++;
      strcpy(pname[i],"double emittance_y");  i++;
      strcpy(pname[i],"char optfile");  i++;
      strcpy(pname[i],"char latfile");  i++;
      strcpy(pname[i],"double Llambda");  i++;
      strcpy(pname[i],"double Elaser");  i++;
      strcpy(pname[i],"double Lwaist");  i++;
      strcpy(pname[i],"double Lfocus");  i++;
      strcpy(pname[i],"double Lsstart");  i++;
      strcpy(pname[i],"double Lsend");  i++;
      strcpy(pname[i],"char Lprof");  i++;
      strcpy(pname[i],"char cntl_ebeam");  i++;
      strcpy(pname[i],"char outfile");  i++;
      strcpy(pname[i],"int randseed");  i++;
      strcpy(pname[i],"char cntl_event");  i++;


#ifdef DEBUG
        for(i=0;i<MAXNPR ;i++){        
            if( strlen(pname[i])==0 ){continue;}
            strcpy(buffer,pname[i]);
            a = strtok(buffer," ");
            strcpy(vtyp, a);
            a = strtok(NULL," ");
            strcpy(vrbl,a);
    printf("D...main: %d= '%s', '%s', '%s'\n",
                 i,pname[i],vtyp,vrbl);
        }/*** variable i ***/
#endif
      readinp(logstr, inpfile, pname,
                 ipar, rpar, spar); 
/*         ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~   */



      fclose(outstr);
}
#endif
/*
             readinp
061016-23 coded by S,D. @SP8

============================================================================*/
int readinp(FILE *logstr, char *inpfile,  
               char pname[][LPNAM],
               int *ipar, double *rpar, char **spar
){
FILE *instr;
char buffer[240]="", buffer1[240]="", buffer2[240]="", buffer3[240]="", buffer4[240]="";
char bufferExt[240]="";
char *a, *b, *a1, *b1, *v, vrbl[28], vtyp[8];
int line, nline, nskip;
int ifcomment;
int kipar=0, krpar=0, kspar=0;
int i,j,k,l,m,n; 
double x, y;

#ifdef DEBUG
    printf("\nD**************** START Debugging readinp ***********************************\n");
#endif

     instr=fopen(inpfile,"r");
     nline=0; nskip=0;
/*************** Data Line Loop *****************/
/*---loop of reading/printing---*/
    line = 0;  ifcomment=0;
    
    while( ( fgets(buffer,sizeof(buffer),instr) )!=NULL ){
       line++;
                 i = strlen(buffer);
                 if( i <= 1 ){ nskip++; continue;}  /* skip blank lines */
 
#ifdef DEBUG1
    printf("\nD...readinp:------------------------------------"
                             " buffer:(line %d) \n",line);
    printf("\n'%s'\n",buffer);
#endif
/*---- comment line processing starts here ----
           buffer1 to pass to analysis
        ---------------------------------------                    */
        if( ifcomment!=0 ){ 
            if( strstr(buffer,"*/")==NULL){
#ifdef DEBUG1
    printf("                 a comment line -> skip\n");
#endif
                        nskip++; continue; }      /* skip comment lines */
            else{ buffer1[0]='\0'; 
                  a= buffer;
            }
        }
        else{                        /* ifcomment == 0 */             
#ifdef DEBUG101
    printf("D101...ifcomment==0, issue strcpy(buffer1, buffer)\n");
#endif
             strcpy(buffer1, buffer);
/* find a string segment and put it in buffer1 for analysis */
             a= strstr(buffer,"/*");
#ifdef DEBUG101
    printf("D101...a:'%s'\n",a);
#endif
             if(a!=NULL){
                 /* find a string before the comment part */
                 /* b=strtok(buffer2,"/*"); this is wrong */
                 k = strlen(buffer) - strlen(a);
                 if(k>0){ buffer1[k]='\0'; }
                 else{ buffer1[0] = '\0';}
                 ifcomment=1;
                 /* there is no statement in a line after a comment mark */
             } /* a was not NULL i.e. there is a slash* in this line */
         }  /* ifcomment was not 0 */
#ifdef DEBUG13
      printf("D13 ... existence of a comment section ifcomment=%d\n" 
             "        string to analyse buffer1='%s'\n"
             "        comment section a='%s'\n"
             "   <judge whether or not the comment region is closed in this line>\n"
                ,ifcomment,buffer1,a);
#endif
/*  judge whether or not the comment region is closed in this line */
        l=0;
        while( a != NULL ){    l++;        /* loop of comment segments */
                        if(l==1 && ifcomment != 1){
                           printf("E...xxxx l=%d ifcomment=%d\n",l,ifcomment);
                           printf(" ...a='%s'\n",a); 
                           exit(1);                }
                 ifcomment=1;
                 strcpy(buffer2,a);
#ifdef DEBUG11
    printf("D11    l=%d, a='%s'\n",l,buffer2);
#endif
                 b = strstr(buffer2,"*/");    /* find an closing */               
                 if(b==NULL){break;}       /* no closing, exit the loop */
                 
                 ifcomment = 0;    /* because a closing is found */
/* look into the next segment */
                 /*a=b+2;*/ 
                 strcpy(buffer3,b+2);
                 /*strcpy(buffer2,buffer3);*//* 141020*/
                 strcpy(bufferExt,buffer3);
                 /*a = strstr(buffer2,"/*");  /* looking for the next comment seg. */
                 a = strstr(bufferExt,"/*");  /* looking for the next comment seg. */
#ifdef DEBUG11
    printf("D11   next comment segment  b='%s'\n",buffer2);
#endif

        } /* loop of comment segments*/

#ifdef DEBUG12
    printf("D12   at the end of this line:   ifcomment=%d\n",ifcomment);
#endif
/*------------------------------------- finished comment line processing  */

             if( strlen(buffer1)==0){
#ifdef DEBUG1
    printf("D1....   null buffer1,   a comment line -> skip\n");
#endif
                                  continue;} /* nothing to analyse */
#ifdef DEBUG14
      printf("D14... analysis <  buffer1='%s'\n",buffer1);
#endif
                  a=0; b=0;strcpy(buffer2,"");strcpy(buffer3,"");


/*     analyse a string in buffer1 */
/*======================================<<<<<<<<<<<< loop l of variables */ 
       for(l=0;l<MAXNPR ;l++){        
            if( strlen(pname[l])==0 ){continue;}
#ifdef DEBUG103
         printf("D103...variable loop l=%d; buffer1:'%s'\n",l,buffer1);            
#endif
/*---   vtyp and vrbl for the l-th variable */
            strcpy(buffer3, pname[l]);
            a = strtok(buffer3," ");
            strcpy(vtyp, a);
            a = strtok(NULL," ");
            strcpy(vrbl,a);
            a=0; b=0; strcpy(buffer3,""); strcpy(buffer2,""); strcpy(buffer4,"");
#ifdef DEBUG2
    printf("D2...readinp: %d= '%s', '%s', '%s'\n",
                 l,pname[l],vtyp,vrbl);
#endif
            if(strlen(buffer1)<strlen(vrbl)){
#ifdef DEBUG1
              printf("D1...short buffer1:'%s' --> skip\n",buffer1);
                 /*l,pname[l],vtyp,vrbl);*/
#endif
            continue;}

               b=strstr(buffer1,vrbl);  /* <------ b = pointer to the l th variable */
            if(b==NULL ){ 
#ifdef DEBUG1
              printf("D1...b==NULL --> skip\n");
#endif
             continue; 
                        }   /* no vrbl in this line -> next vrbl */ 
#ifdef DEBUG103
    printf("D103... b:'%s', vrbl:'%s'; b is going to be sotred in buffer2\n",b,vrbl);
#endif
/*** vrbl found in buffer ***/
/*---- this line contains the l-th variable vrbl */
               strcpy(buffer2,b);
               strcpy(buffer4,b);
#ifdef DEBUG15
    printf("D15 ... found: variable %d, vtyp='%s', vrbl= '%s' in line %d \n",
                 l,vtyp,vrbl,line);
#endif
/*============================================= type integer ======*/
                 if(strstr(vtyp,"int")!=NULL){  
#ifdef DEBUG3
                   printf("D3... the variable in line = %d is 'int'\n",line);
#endif
#ifdef DEBUG103
             printf("D103...before strtok  buffer2:'%s'\n",buffer2);
#endif
                   a = strtok(buffer2,"=");     /* lhs. of the eq., strtok set */
                   k=strlen(a) + 1;
                   strcpy(buffer4,buffer2+k); /* buffer4 is the string after the '=' */
             if(strstr(a,vrbl)==NULL){ 
#ifdef DEBUG1
                     printf("D1... buffer2 has no substitution¥n");   
#endif
                                    continue;}
                  /* vrlb is involved in the lhs. */
#ifdef DEBUG103
             printf("D103...after strtok  buffer2:'%s', b:'%s'\n",buffer2,b);
             printf("D103...buffer4:'%s'\n",buffer4);
#endif
#if(0)                                   
                               ------------------part below is commented out
/*------ this line invovle an '=' */
                   strcpy(buffer2,b);
                   a = strtok(buffer2,"=");     /* lhs. of the eq., strtok set */
/*------- a is a string befor '=' */
                   while(a!=NULL){      /* loop of the lhs. that exists */
                               ------------------part above is commented out
#endif
/*printf("a='%s'\n", a);*/
                      strcpy(buffer3,a); strltrim(buffer3); strrtrim(buffer3);
#ifdef DEBUG4
                                  printf("D4... lhs. a = '%s'\n",buffer3);
#endif
                     v =  strtok(NULL,";"); /* the rhs. terminated by ; */
                     /*  strtok is directing on buffer2*/
/*-------- v is the right hand side of 'a = v;' in this line*/
                     k=strlen(v)+1;
                if(k<strlen(buffer4)){
                   /*strcpy(buffer4,buffer4+k);} /* buffer4 is the string after the ';' */
                    strcpy(bufferExt,buffer4+k);} /* buffer4 is the string after the ';' */
              /*else{ strcpy(buffer4,""); }*/
              else{ strcpy(bufferExt,""); }
            strcpy(buffer4, bufferExt); /* 141020 added */
               /*      strcpy(buffer4,buffer4+k);*/
#ifdef DEBUG103
                   printf("D103...k=%d next article buffer4:'%s'\n",k,buffer4);
#endif

                     if(strcmp(buffer3,vrbl)==0){    /* lhs. == vrbl */
#ifdef DEBUG5
                       printf("D5...lhs=rhs: variable %d, %s %s = '%s'\n",
                                  l,vtyp,vrbl,v);
#endif
                       strcpy(buffer3,v);    /* the rhs. */
/*!  Do not use other strtok in a loop of strtok 
                       _ x sscanf(strtok(buffer3,";"),"%d",&j);<< causes error!*/
                       sscanf(buffer3,"%d;",&j);  /* value of the rhs. */
                       ipar[l]=j;
                       printf("N...>>>>>>>>>> ipar[%d]:'%s' = %d <<<<<<<<<\n",
                                          l,vrbl,j);
                       kipar++;
                     }
#ifdef DEBUG16
                     else{ printf("D16...         but the lhs doesn't contain vrbl\n");}
#endif
  /* put the remaining string into buffer1 to be analysed */
                    strcpy(buffer1,buffer4);
                    strcpy(buffer2,buffer4);


/*        set buffer2 as the whole string after the last ";"     *
                    strcpy(buffer2,strtok(NULL,"\0"));*/
#ifdef DEBUG6
                   printf("D6...segment remains to analyse buffer2='%s'\n",buffer2);
                   printf("D6...strlen buffer2=%d going to strstr'\n",strlen(buffer2));
#endif
#if(0)                                   
                               ------------------part below is commented out
/*         point to vrbl in buffer2 */
                    b = strstr(buffer2,vrbl);
#ifdef DEBUG6
                  printf("D6...b=strstr(buffer2,); passed. b= '%s', going to ask if b==NULL\n",b);
#endif
                  if(b==NULL){ a = '\0'; continue;}
                   strcpy(buffer2,b);
                   a = strtok(buffer2,"=");
#ifdef DEBUG6
                  printf("D6...strcpy(buffer2,b) passed. buffer2='%s', a = '%s'\n",buffer2,a);
#endif


                   }  /* loop of lhs. that exists */
                               ------------------part above is commented out
#endif
                 } 
/*============================================= type double ======*/
                 else if(strstr(vtyp,"double")!=NULL){ 
#ifdef DEBUG23
                   printf("D23... the variable in line = %d is 'double'\n",line);
#endif
#ifdef DEBUG103
             printf("D103...before strtok  buffer2:'%s'\n",buffer2);
#endif
                   a = strtok(buffer2,"=");     /* lhs. of the eq., strtok set */
                     
             if(a==NULL){
#ifdef DEBUG1
                     printf("D1... there is no substitution in buffer2\n");   
#endif
                         continue;}
                   k=strlen(a) + 1;
    printf("  k = %d;  buff4='%s'\n",k, buffer4);
    printf("  len(buff4)=%d\n",strlen(buffer4));
                     
              if(k<strlen(buffer4)){
/*                   strcpy(buffer4,buffer4+k);}*/ /* buffer4 is the string after the '=' */
                    strcpy(bufferExt,buffer4+k);} /* buffer4 is the string after the '=' */
                   /*strcpy(buffer4,b+k); * buffer4 is the string after the '=' */
/*              else{ strcpy(buffer4,""); }*/
              else{ strcpy(bufferExt,""); }
                     
            strcpy(buffer4, bufferExt); /* 141020 added */
             if(strstr(a,vrbl)==NULL){
#ifdef DEBUG1
                     printf("D1... there is no '%s' in buffer2\n",vrbl);   
#endif
              continue;}
          /* vrlb is involved in the lhs. */
#ifdef DEBUG103
             printf("D103...after strtok  buffer2:'%s', b:'%s'\n",buffer2,b);
             printf("D103...buffer4:'%s'\n",buffer4);
#endif
#if(0)                                   
                               ------------------part below is commented out

                   if(strstr(buffer1,"=")==NULL){
#ifdef DEBUG
                     printf("D   but there is no substitution¥n");   
#endif
                                       break;} /*>> not a data line */
/*------ this line invovle an '=' */
                   strcpy(buffer2,b);
#ifdef DEBUG103
             printf("D103...before strtok  buffer2:'%s', b:'%s'\n",buffer2,b);
#endif
                   a = strtok(buffer2,"=");     /* lhs. of the eq., strtok set */
                   k=strlen(a) + 1;
                   strcpy(buffer4,b+k);
#ifdef DEBUG103
             printf("D103...after strtok  buffer2:'%s', b:'%s'\n",buffer2,b);
             printf("D103...buffer4:'%s'\n",buffer4);
#endif
#ifdef DEBUG23
             printf("D23... an '=' is found in buffer2:'%s', a:'%s'\n",buffer2,a);
#endif
/*------- a is a string befor '=' */
                   while(a!=NULL){      /* loop of the lhs. that exists */
                               ------------------part above is commented out
#endif
                      strcpy(buffer3,a); strltrim(buffer3); strrtrim(buffer3);
#ifdef DEBUG24
                     printf("D24... lhs. trimmed buffer3:'%s'\n",buffer3);
#endif
                     v =  strtok(NULL,";"); /* the rhs. terminated by ; */
                     /*  strtok is directing on buffer2*/
/*-------- v is the right hand side of 'a = v;' in this line*/
                     k=strlen(v)+1;
                     /*strcpy(buffer4,buffer4+k);*/
                if(k<strlen(buffer4)){
 /*                   strcpy(buffer4,buffer4+k);} /* buffer4 is the string after the ';' */
                    strcpy(bufferExt,buffer4+k);} /* buffer4 is the string after the ';' */
              else{ strcpy(bufferExt,""); }
            strcpy(buffer4, bufferExt); /* 141020 added */
          
#ifdef DEBUG103
                   printf("D103...k=%d next article buffer4:'%s'\n",k,buffer4);
#endif

                     if(strcmp(buffer3,vrbl)==0){    /* lhs. == vrbl */
#ifdef DEBUG25
                       printf("D25...lhs=rhs: variable %d, %s %s = '%s'\n",
                                  l,vtyp,vrbl,v);
#endif
                       strcpy(buffer3,v);    /* the rhs. */
/*!  Do not use other strtok in a loop of strtok 
                       _ x sscanf(strtok(buffer3,";"),"%d",&j);<< causes error!*/
                       sscanf(buffer3,"%lf;",&y);  /* value of the rhs. */
                       rpar[l]=y;
                       printf("N...>>>>>>>>>> rpar[%d]:'%s' = %e <<<<<<<<<\n",
                                          l,vrbl,y);
                       krpar++;
                      k = strlen(v)+1;
                      strcpy(buffer4,buffer4+k);
#ifdef DEBUG103
                   printf("D103...k=%d buffer4:'%s'\n",k,buffer4);
#endif
                      
                     }
#ifdef DEBUG16
                     else{ 
                      printf("D16...         but the lhs doesn't contain vrbl:'%s'\n",vrbl);
                      break;}
    
#endif
                   /*...It seems strtok(name,"\0") is not allowed.
                   v = strtok(NULL,"\0");
                   printf("D116...v:'%s' strtok passed. > strcpy\n",v);
                   strcpy(buffer2,v);  */                 

/*                   v = strchr(a,';');
                   k=0;
                   if( v != NULL){
                   k = strlen(v);}
                   if(k>1){ strcpy(buffer2,v+1); }
                   else{ buffer2='\0'; }*/

/*        set buffer2 as the whole string after the last ";"     */
      /* >>>>>>>strcpy(buffer2,strtok(NULL,"\0"));<<<<this line doesn't work on saho*/
      /*strcpy(buffer2,strtok(NULL,"\0"));*<<<<this line doesn't work on saho*/

  /* put the remaining string into buffer1 to be analysed */
                    strcpy(buffer1,buffer4);
                    strcpy(buffer2,buffer4);
#ifdef DEBUG26
                   printf("D26...segment remains to analyse buffer2='%s'\n",buffer2);
                   printf("D26...strlen buffer2=%d going to strstr'\n",strlen(buffer2));

#endif
#if(0)                                   
                               ------------------part below is commented out
/*         point to vrbl in buffer2 */
                    b = strstr(buffer2,vrbl);
#ifdef DEBUG26
                  printf("D26...b=strstr(buffer2,); passed. b= '%s', going to ask if b==NULL\n",b);
#endif
                      /* strcpy of a '\0' may cause a segmentation fault */
                  if(b==NULL){ a = '\0'; continue;}
                   strcpy(buffer2,b);
#ifdef DEBUG26
                  printf("D26...strcpy(buffer2,b) passed. buffer2:'%s'\n",buffer2);
#endif
                   a = strtok(buffer2,"=");
                   
#ifdef DEBUG26
                  printf("D26...after strtok buffer2:'%s', a:'%s'\n",buffer2,a);
#endif

                   }  /* loop of lhs. that exists */
                               ------------------part above is commented out
#endif
                 } 
/*============================================= type char* ======*/
                 else if(strstr(vtyp,"char")!=NULL){ 
                       /*~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#ifdef DEBUG33
                   printf("D33... the variable in line = %d is 'char *'\n",line);
#endif
#ifdef DEBUG103
             printf("D103...before strtok  buffer2:'%s'\n",buffer2);
#endif
                   /*if(strstr(buffer1,"strcpy")==NULL){*/

                   a = strtok(buffer2,",\"");     /* lhs. of the eq., strtok set */
             if(a==NULL){ 
#ifdef DEBUG1
                     printf("D1...  there is no str article in buffer2\n");   
#endif
                        continue;}
                   /*a = strtok(buffer2,"=");     * lhs. of the eq., strtok set */
                   k=strlen(a) + 2;
              if(k<strlen(buffer4)){
/*                   strcpy(buffer4,buffer4+k);} /* buffer4 is the string after the '=' */
                  strcpy(bufferExt,buffer4+k);} /* buffer4 is the string after the '=' */
                   /*strcpy(buffer4,b+k); * buffer4 is the string after the '=' */
              /*else{ strcpy(buffer4,""); }*/
              else{ strcpy(bufferExt,""); }
         /*          strcpy(buffer4,buffer2+k); * buffer4 is the string after the '=' */
            strcpy(buffer4, bufferExt); /* 141020 added */
             if(strstr(a,vrbl)==NULL){
#ifdef DEBUG1
                     printf("D1...  there is no vrbl in buffer2\n");   
#endif
                               continue;}
          /* vrlb is involved in the lhs. */
#ifdef DEBUG103
             printf("D103...after strtok  buffer2:'%s', b:'%s'\n",buffer2,b);
             printf("D103...buffer4:'%s'\n",buffer4);
#endif

#if(0)                                   
                               ------------------part below is commented out
/*------ this line involve an '=' */
#ifdef DEBUG33
                   printf("D33... a "strcpy" is found\n");
#endif
                   strcpy(buffer2,b);
                   a = strtok(buffer2,",\"");     /* lhs. of the eq., strtok set */
/*------- a is a string befor '=' */
                   while(a!=NULL){      /* loop of the lhs. that exists */
                               ------------------part above is commented out
#endif
                      strcpy(buffer3,a); strltrim(buffer3); strrtrim(buffer3);
#ifdef DEBUG34
                                  printf("D34... lhs. a = '%s'\n",buffer3);
#endif
                     v =  strtok(NULL,"\")"); /* the rhs. terminated by ") */
                     /*  strtok is directing on buffer2*/
/*-------- v is the right hand side of 'ai,"v")' in this line*/
                     k=strlen(v)+2;
                if(k<strlen(buffer4)){
                   strcpy(buffer4,buffer4+k);} /* buffer4 is the string after the ';' */
              else{ strcpy(buffer4,""); }

#ifdef DEBUG103
                   printf("D103...k=%d next article buffer4:'%s'\n",k,buffer4);
#endif

                     if(strcmp(buffer3,vrbl)==0){    /* lhs. == vrbl */
#ifdef DEBUG35
                       printf("D35...lhs=rhs: variable %d, %s %s = '%s'\n",
                                  l,vtyp,vrbl,v);
#endif
                       strcpy(buffer3,v);    /* the rhs. */
                        /* strcpy(spar[l],v);*/
                        strcpy(*(spar+l),v);
                       printf("N...>>>>>>>>>> spar[%d]:'%s' = '%s'<<<<<<<<<\n",
                                          l,vrbl,v);
                       krpar++;
                     }
#ifdef DEBUG16
                     else{ printf("D16...         but the lhs doesn't contain vrbl\n");}
#endif
/*        set buffer2 as the whole string after the last ";"     *
                    strcpy(buffer2,strtok(NULL,"\0"));*/
  /* put the remaining string into buffer1 to be analysed */
                    strcpy(buffer1,buffer4);
                    strcpy(buffer2,buffer4);
#ifdef DEBUG36
                   printf("D36...segment remains to analyse buffer2='%s'\n",buffer2);
                   printf("D36...strlen buffer2=%d going to strstr'\n",strlen(buffer2));
#endif
#if(0)                                   
                               ------------------part below is commented out
/*         point to vrbl in buffer2 */
                    b = strstr(buffer2,vrbl);

#ifdef DEBUG36
                  printf("D36...b=strstr(buffer2,); passed. b= '%s', going to ask if b==NULL\n",b);
#endif
                  if(b==NULL){ a = '\0'; continue;}
                   strcpy(buffer2,b);
                   a = strtok(buffer2,"=");
#ifdef DEBUG36
                  printf("D36...strcpy(buffer2,b) passed. buffer2='%s', a = '%s'\n",buffer2,a);
#endif


                   }  /* loop of lhs. that exists */

                               ------------------part above is commented out
#endif


                 }      /*********** end of case type = char **********/ 
                 else{
                    printf("W...readinp: A variable not on list:\n"
                           " ...pname = '%s', l = %d found in line %d\n",
                              pname[l], l, line); 
                    /*  exit(1); */   
                 } 


        }/*** variable l ***/

/*
          a=strstr(buffer2, vrbl); 
          if( a != NULL){
             i = strlen(vrbl);
             while(*(a+i)!='='){i++;
        printf("D...readinp: seach '=' i=%d\n",i);        
             }
           }
*/

      } /******** Data line loop */
      fclose(instr);
      nline = line;

      fprintf(logstr,"N...readinp: read %d lines from %s\n",
                nline, inpfile);
#ifdef DEBUG
      fprintf(logstr,"D...readinp: nskip=%d\n",
                nskip);
    printf("\nD**************** END Debugging readinp ************************************\n");
#endif
      return(0);
}
