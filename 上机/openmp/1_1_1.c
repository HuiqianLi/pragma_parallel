#include <stdio.h>
#include "omp.h"

int main(){
   int a[12][12];
	int b[12][12];
	for(int i = 0; i < 12; i++){
		for(int j = 0; j < 12; j++){
			b[i][j] = a[i][j] = i * j;
      }
	}
	for(int i = 2; i <= 10; i++){
		for(int j = 2; j <= 10; j++){
			a[i][j] = (a[i - 1][j - 1] + a[i + 1][j + 1]) * 0.5;
      }
	}
   for(int i = 2; i <= 10; i++){
#pragma omp parallel for
/*     Fork a team of threads      */
      for(int j = 2; j <= 10; j++){
         b[i][j] = (b[i - 1][j - 1] + b[i + 1][j + 1]) * 0.5;
      }
#pragma omp barrier
   }

   printf("using omp\n");
   for(int i = 2; i <= 10; i++){
		for(int j = 2; j <= 10; j++){
			printf("%d ",a[i][j]);
      }
	}
   printf("\n");

   printf("not using omp\n");
   for(int i = 2; i <= 10; i++){
		for(int j = 2; j <= 10; j++){
			printf("%d ",b[i][j]);
      }
	}
   printf("\n");
   return 0;
}