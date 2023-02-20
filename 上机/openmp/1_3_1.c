#include <stdio.h>
#include "omp.h"
#define N 100

int main(){
   int A[301][301];
   int A1[301][301];
	for(int i=0;i<=300;i++){
      for(int j=0;j<=300;j++){
         A[i][j] = A1[i][j] = 1;
      }
   }
   for(int i=1;i<=100;i++){
#pragma omp parallel for
      for(int j=1;j<=50;j++){
			A[3*i+2][2*j-1] = A[5*j][i+3] + 2;
      }
#pragma omp barrier
   }
   for(int i=1;i<=100;i++){
      for(int j=1;j<=50;j++){
			A1[3*i+2][2*j-1] = A1[5*j][i+3] + 2;
      }
   }
	for(int i=0;i<=300;i++){
      for(int j=0;j<=300;j++){
         if(A[i][j]!=A1[i][j]){
            printf("Can't be paralleled.\n");
            break;
         }
      }
   }
   printf("using omp\n");
	for(int i=0;i<=10;i++){
      for(int j=0;j<=10;j++){
         printf("%d ",A[i][j]);
      }
      printf("\n");
   }
   printf("not using omp\n");
	for(int i=0;i<=10;i++){
      for(int j=0;j<=10;j++){
         printf("%d ",A1[i][j]);
      }
      printf("\n");
   }
   return 0;
}