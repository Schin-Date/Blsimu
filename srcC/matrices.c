/*                                      created on Mar. 06, 2005 by S.D. in Londrina, Brazil */
/*----------------------------------------------------------*/
/*               Basic matrix operations                       */
/*----------------------------------------------------------*/
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include "main.h"
/*============================================================================
             (N,M)    (N,L)    (L,M)
            C(i,j) = A(i,k) * B(k,j)
*/
int mtxprod(FILE *logstr, int N, double *A, int L, double *B, int M, double *C){

int i,j,k;

     for(i=0;i<N;i++){
      for(j=0;j<M;j++){
            *(C + M*i +j) = 0.;
        for(k=0;k<L;k++){
            *(C + M*i +j) = *(C + M*i +j) + *(A + L*i +k) * *(B + M*k + j); 
            }
            }
            }
            
    return(0);
}
/*============================================================================
             (N,M)    (N,M)
            B(i,j) = A(i,j) 
*/
int mtxcopy(FILE *logstr, int N, int M, double *A, double *B){
int i,j;
        for(i=0;i<N;i++){
          for(j=0;j<M;j++){
            *(B + M*i + j) = *(A + M*i + j); } }
            
      return(0);
}      
/*============================================================================
             (N,M)    
            A(i,j) = [1 0 0 ...
                      0 1 0 ...
                      .....
                      0 0 0 ...1 0 0] (N < M) or      0 0 0 ... 1
                                                    0 0 0 ... 0
                                                    .....     0] (N > M) 
*/
int mtxunit(FILE *logstr, int N, int M, double *A){
int i,j;
        for(i=0;i<N;i++){
          for(j=0;j<M;j++){
            *(A + M*i + j) = 0.; 
            if( j == i ) *(A + M*i + j) = 1.; } }
            
      return(0);
}      





