#include <stdio.h>
#include <math.h>
#include "omp.h"

int main(){
	int A[501][501] ,C[501][501] ,D[501][501];
	int A1[501][501] ,C1[501][501] ,D1[501][501];
	for(int i=0;i<500;i++){
      for(int j=0;j<500;j++){
         A[i][j] = C[i][j] = D[i][j] = 1;
         A1[i][j] = C1[i][j] = D1[i][j] = 1;
      }
   }

   for(int i=1;i<=100;i++){
#pragma omp parallel for
      for(int j=1;j<=100;j++){
         A[3*i+2*j][2*j] = C[i][j] * 2;
         if(i-j+6 >= 0){
            D[i][j] = A[i-j+6][i+j];
         }
      }
   }

   for(int i=1;i<=100;i++){
      for(int j=1;j<=100;j++){
         A1[3*i+2*j][2*j] = C1[i][j] * 2;
         if(i-j+6 >= 0){
            D1[i][j] = A1[i-j+6][i+j];
         }
      }
   }

   for(int i=0;i<500;i++){
      for(int j=0;j<500;j++){
			if(A[i][j] != A1[i][j] || C[i][j] != C1[i][j] || D[i][j] != D1[i][j]){
            printf("Can't be paralleled.\n");
         }
      }
   }
   return 0;
}