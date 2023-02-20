#include <stdio.h>
#include "omp.h"

int min(int a, int b){
	return a<b?a:b;
}
int main(){
	int A[20] ,B[20];
	int A1[20] ,B1[20];
	for(int i=0;i<=20;i++){
      A[i] = B[i] = 1;
      A1[i] = B1[i] = 1;
   }
   for(int k=1;k<=16;k+=3){
#pragma omp parallel for
      for(int i=k;i<=min(16,k+2);i++){
         A[i+3] = A[i] + B[i];
      }
#pragma omp barrier
   }
   for(int i=1;i<=16;i++){
      A1[i+3] = A1[i] + B1[i];
   }
   
   printf("using omp\n");
	for(int i=0;i<=20;i++){
      printf("%d ",A[i]);
   }
   printf("\n");
   printf("not using omp\n");
   for(int i=0;i<=20;i++){
      printf("%d ",A1[i]);
   }
   printf("\n");
   return 0;
}