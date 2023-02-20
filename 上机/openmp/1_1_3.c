#include <stdio.h>
#include <stdlib.h>
#include "omp.h"

int main(){
    int A[30], B[30], C[30];
    int A1[30], B1[30], C1[30];
    for(int i=0;i<30;i++){
        A[i] = A[i] = rand()%2;
        B[i] = C[i] = 1;
        B1[i] = C1[i] = 1;
    }
#pragma omp parallel for
    for(int i=2;i<=20;i++){
        if(A[i]>0){
            B[i] = C[i-1] + 1;
        }
        else{
            C[i] = B[i] - 1;
        }
    }
    for(int i=2;i<=20;i++){
        if(A1[i]>0){
            B1[i] = C1[i-1] + 1;
        }
        else{
            C1[i] = B1[i] - 1;
        }
    }
    for(int i=0;i<30;i++){
        if(B[i]!=B1[i] or C[i]!=C1[i]){
            printf("Can't be paralleled.\n");
        }
    }
}