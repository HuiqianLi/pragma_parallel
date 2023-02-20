#include <stdio.h>
#include <math.h>
#include "omp.h"

int main(){
	double A[200] ,B[200] ,C[200] ,D[200];
	double A1[200] ,B1[200] ,C1[200] ,D1[200];
	for(int i=0;i<200;i++){
      A[i] = B[i] = C[i] = D[i] = 1;
      A1[i] = B1[i] = C1[i] = D1[i] = 1;
   }
   for(int i=1;i<=100;i++){
      B[i] = C[i-1] * 2;
      C[i] = 1.0 / B[i];
   }
#pragma omp parallel for
   for(int i=1;i<=100;i++){
      A[i] = A[i] + B[i-1];
   }
#pragma omp parallel for
   for(int i=1;i<=100;i++){
      D[i] = C[i] * C[i];
   }

   for(int i=1;i<=100;i++){
      B1[i] = C1[i-1] * 2;
      C1[i] = 1.0 / B1[i];
      A1[i] = A1[i] + B1[i-1];
      D1[i] = C1[i] * C1[i];
   }

   printf("using omp\n");
	for(int i=1;i<=100;i++){
      printf("%.2lf ",A[i]);
   }
   printf("\n");
   printf("not using omp\n");
   for(int i=1;i<=100;i++){
      printf("%.2lf ",A1[i]);
   }
   printf("\n");
   return 0;
}