/*              random from  standard lib
/*----------------------------------------------------------*/
/*     In functions to call this generator, you need to declare

          double  getrandom(); 
          void    setrandom(int);

    Call setrandom(i) with i being an integer to set the seed for the random
    number generator before using the generator. If setrandom(i) is not
    called or called with i < 0, then the generator uses a default value
    which is set below. 
                                                       */    

#include   <math.h>
#include   <stdlib.h>

static int seed = 566387;  /* this is a file static variable.
                              Default value is set at compile time */

/*-------------------  Set seed by this function  ---------------------*/
void setrandom(int i){   if( i >= 0 )   seed = i;   srand48(seed);      
                /* srand(seed); */    }
/*---------------------------------------------------------------------*/

/*----------------  Get the current seed by this function  ------------*/
int getseed(){ return seed; }
/*---------------------------------------------------------------------*/

/*---------------------------------------------------------------------*/
/*  Random number is produced in this routine. Region is (0.0-1.0).    */
/*---------------------------------------------------------------------*/
double getrandom()
{
        /* return( (double)rand()/(double)RAND_MAX );*/
        return( drand48() );
}
