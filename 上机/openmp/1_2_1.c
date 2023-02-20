#include <stdio.h>
#include "omp.h"
#define C 1
#define M 10
#define N 10

int main(){
   int a[20][20];
   int b[20][20];
	for(int i=0;i<20;i++){
      for(int j=0;j<20;j++){
         a[i][j] = b[i][j] = 1;
      }
   }
   for(int i=1;i<=M;i++){
#pragma omp parallel for
/*     Fork a team of threads      */
      for(int j=1;j<=N;j++){
         a[i][j] = a[i+1][j+1] + C;
      }
#pragma omp barrier
   }
   for(int i=1;i<=M;i++){
      for(int j=1;j<=N;j++){
         b[i][j] = b[i+1][j+1] + C;
      }
   }
   printf("using omp\n");
   for(int i=0;i<10;i++){
      for(int j=0;j<10;j++){
         printf("%d ",a[i][j]);
      }
      printf("\n");
   }
   printf("not using omp\n");
   for(int i=0;i<10;i++){
      for(int j=0;j<10;j++){
         printf("%d ",b[i][j]);
      }
      printf("\n");
   }
   return 0;
}