#include <stdio.h>
#include <math.h>
#include "omp.h"

int main(){
	double A[1000] ,B[1000] ,C[1000] ,D[1000];
	for(int i=0;i<1000;i++){
      A[i] = B[i] = C[i] = D[i] = 1;
   }

   for(int i=1;i<=999;i++){
      A[i] = B[i] + C[i];
      D[i] = A[i] + A[999 - i + 1];
   }

   printf("not using omp A\n");
   for(int i=1;i<=50;i++){
      printf("%.2lf ",A[i]);
   }
   printf("\n");
   printf("not using omp D\n");
   for(int i=1;i<=50;i++){
      printf("%.2lf ",D[i]);
   }
   printf("\n");
   return 0;
}