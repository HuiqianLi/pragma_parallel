#include <stdio.h>
#include "omp.h"
#define N 100

int main(){
   int X[200] ,Y[200] ,A[200][200] ,B[200] ,C[200][200];
   int X1[200] ,Y1[200] ,A1[200][200] ,B1[200] ,C1[200][200];
	for(int i=0;i<200;i++){
      for(int j=0;j<200;j++){
         A[i][j] = C[i][j] = 1;
         A1[i][j] = C1[i][j] = 1;
      }
      X[i] = Y[i] = B[i] = 1;
      X1[i] = Y1[i] = B1[i] = 1;
   }
   for(int i=1;i<=100;i++){
      X[i] = Y[i] + 10;
      for(int j=1;j<=100;j++){
         B[j] = A[j][N];
#pragma omp parallel for
         for(int k=1;k<=100;k++){
            A[j+1][k] = B[j] + C[j][k];
         }
#pragma omp barrier
         Y[i+j] = A[j+1][N];
      }
   }
   for(int i=1;i<=100;i++){
      X1[i] = Y1[i] + 10;
      for(int j=1;j<=100;j++){
         B1[j] = A1[j][N];
         for(int k=1;k<=100;k++){
            A1[j+1][k] = B1[j] + C1[j][k];
         }
         Y1[i+j] = A1[j+1][N];
      }
   }
   printf("using omp\n");
   for(int i=1;i<=100;i++){
      printf("%d ",Y[i]);
   }
   printf("\n");
   printf("not using omp\n");
   for(int i=1;i<=100;i++){
      printf("%d ",Y1[i]);
   }
   printf("\n");
   return 0;
}