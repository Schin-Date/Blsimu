/*============================================================================*/
#include <stdio.h>  /* for printf,fgets NULL */
#include <stdlib.h>  /* for exit, malloc */
#include <string.h> /* for strcpy */
/*---------------------------------------------------------------------------*/
int strltrim(char *s){
char buff[256];
char *a, *b;
int slen, i,j;

slen = strlen(s);
if(slen >= 256){
    printf("E...strltrim: string length exceeds the limit: strlen=%d\n",slen);
    printf("s:'%s'\n",s);	
	exit(1);
}
#ifdef DEBUG2
  printf("D1...strltrim:input s:'%s'\n",s);
  printf("D1...strltrim: slen=%d\n",slen);
  printf("D1...strltrim: s[slen-3]='%c',s[slen-2]='%c', s[slen-1]='%c'\n",
                    s[slen-3],s[slen-2],s[slen-1]);
#endif

    i=0; j=0;
    while(i>=0){
#ifdef DEBUG1
     printf("D2...strltrim:%d: '%c'\n",i, *(s+i));
#endif
      if(*(s+i)!=' ' ){ i=-1;}
      else{ i++; j++; }
    }
   
#ifdef DEBUG1
  printf("D3...strltrim: i, j = %d, %d  slen=%d\n",i,j,slen);
#endif
 
      strcpy(buff, s+j);  

    for(i=0; i<slen-j+1;i++){
         *(s+i)=*(buff+i);  
    }

        return(0);
}
/*---------------------------------------------------------------------------*/
int strrtrim(char *s){
char buff[256];
char *a, *b, c1, c2;
int slen, i,j;

slen = strlen(s);
if(slen >= 256){
    printf("E...strrtrim: string length exceeds the limit: strlen=%d\n",slen);
    printf("s:'%s'\n",s);	
	exit(1);
}

#ifdef DEBUG1
  printf("D1...strrtrim:input s:'%s'\n",s);
#endif

#ifdef DEBUG2
  i= strlen(s); 
  printf("D2...strrtrim: slen=%d after transformation\n",i);
  printf("D2...strrtrim: s[slen-3]='%c',s[slen-2]='%c', s[slen-1]='%c'\n",
                    s[i-3],s[i-2],s[i-1]);
  printf("D2...strrtrim pointer: s[slen-3]='%c',s[slen-2]='%c'\n",
                    *(s+slen-3),*(s+slen-2));
/*
   *(s+slen-j+2) = '\0';  
   *(s+slen-j-8) = '8';  
*/
#endif

   i=0;
   while(i>-1){
     i++;
     c1 = s[slen-i];
     if(c1=='\0'){continue;} 
     if(isspace(c1)!=0){continue;} 
     break;
   }

#ifdef DEBUG1
  printf("D...strrtrim: i=%d\n",i);
#endif

       *(s+slen-i+1) = '\0';

       return(0);
}
