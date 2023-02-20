#include <stdio.h>
#include "omp.h"

int main(){
	int A[200] ,B[200] ,C[200] ,D[200][200];
	int A1[200] ,B1[200] ,C1[200] ,D1[200][200];
	for(int i=0;i<=200;i++){
      for(int j=0;j<=200;j++){
         D[i][j] = D1[i][j] = 1;
      }
		A[i] = B[i] = C[i] = i;
		A1[i] = B1[i] = C1[i] = i;
   }
   int y = 1;
   int x = y * 2;
   int z = 3;
   for(int i=1;i<=100;i++){
      C[i] = B[i] + x;
      A[i] = C[i-1] + z;
      C[i+1] = A[i] * B[i];
#pragma omp parallel for
      for(int j=1;j<=50;j++){
         D[i][j] = D[i][j-1] + x;
      }
      z = y + 4;
#pragma omp barrier
   }

   int y1 = 1;
   int x1 = y1 * 2;
   int z1 = 3;
   for(int i=1;i<=100;i++){
      C1[i] = B1[i] + x1;
      A1[i] = C1[i-1] + z1;
      C1[i+1] = A1[i] * B1[i];
      for(int j=1;j<=50;j++){
         D1[i][j] = D1[i][j-1] + x1;
      }
      z1 = y1 + 4;
   }

	for(int i=0;i<=200;i++){
      for(int j=0;j<=200;j++){
         if(D[i][j]!=D1[i][j]){
            printf("Can't be paralleled.\n");
            break;
         }
      }
   }
   printf("using omp\n");
	for(int i=0;i<=10;i++){
      for(int j=0;j<=10;j++){
         printf("%d ",D[i][j]);
      }
      printf("\n");
   }
   printf("not using omp\n");
	for(int i=0;i<=10;i++){
      for(int j=0;j<=10;j++){
         printf("%d ",D1[i][j]);
      }
      printf("\n");
   }
   return 0;
}