#include <stdio.h>
#include "mpi.h"
#include <time.h>
#include <stdlib.h>
#include "quickSort.h"
#define n 10000000

double whi_sta,whi_end;

//并行排序
void para_quickSort(int *data,int sta,int end,int whi_m,int id,int now_id){
    //printf("1\n");
    int whi_r,whi_j,whi_i;
    int MyLength = -1;
    int *tmp;
    MPI_Status status;
    //可剩处理器为0，进行串行快速排序排序
    if(whi_m == 0){
        whi_sta = MPI_Wtime();
        if(now_id == id){
            quickSort(data,sta,end);  
        }
        return ;
    }
    //由当前处理器进行分块
    if(now_id == id){
        whi_r = partition(data,sta,end);
        MyLength = end - whi_r;
        MPI_Send(&MyLength,1,MPI_INT,id+exp2(whi_m-1),now_id,MPI_COMM_WORLD);
        if(MyLength != 0){
	MPI_Send(data+whi_r+1,MyLength,MPI_INT,id+exp2(whi_m-1),now_id,MPI_COMM_WORLD);
        }
    }
    if(now_id == id+exp2(whi_m-1)){
        MPI_Recv(&MyLength,1,MPI_INT,id,id,MPI_COMM_WORLD,&status);
 
        if(MyLength != 0){
            tmp = (int*)malloc(sizeof(int)*MyLength);

            MPI_Recv(tmp,MyLength,MPI_INT,id,id,MPI_COMM_WORLD,&status);
        }
    }
    whi_j = whi_r-1-sta;
    MPI_Bcast(&whi_j,1,MPI_INT,id,MPI_COMM_WORLD);
    if(whi_j > 0){
        para_quickSort(data,sta,whi_r-1,whi_m-1,id,now_id);
    }
    whi_j = MyLength;
    MPI_Bcast(&whi_j,1,MPI_INT,id,MPI_COMM_WORLD);
    if(whi_j > 0){
        para_quickSort(tmp,0,MyLength-1,whi_m-1,id+exp2(whi_m-1),now_id);
    }
    if(now_id == id+exp2(whi_m-1)&&MyLength != 0){
        MPI_Send(tmp,MyLength,MPI_INT,id,id+exp2(whi_m-1),MPI_COMM_WORLD);
    }
    if((now_id == id) && (MyLength != 0))
        MPI_Recv(data+whi_r+1,MyLength,MPI_INT,id+exp2(whi_m-1),id+exp2(whi_m-1),MPI_COMM_WORLD,&status);
}

int main(int argc,char *argv[])
{
    int *data1,*data2;
    int now_id,sum_id;
    int whi_m,whi_r;
    int whi_i,whi_j;

    MPI_Status status;
    //启动mpi
    MPI_Init(&argc,&argv);
    //确定自己的进程标志符now_id
    MPI_Comm_rank(MPI_COMM_WORLD,&now_id);
    //组内进程数是sum_id
    MPI_Comm_size(MPI_COMM_WORLD,&sum_id);
    double whi_n;
    if(now_id == 0){
        //初始化串行和并行所需排序的随机数组
        whi_n = n;
        data1 = (int*)malloc(sizeof(int)*whi_n);
        data2 = (int*)malloc(sizeof(int)*whi_n);
        rands(data1,n);
        rands(data2,n);
        //打印初始数组
        // printf("---------\n");
        // printf("The original array data1: \n");
        // print(data1,n);
        // printf("The original array data2: \n");
        // print(data2,n);
        // printf("---------\n");
    }
    whi_m = log2(sum_id);
MPI_Bcast(&whi_n,1,MPI_INT,0,MPI_COMM_WORLD);

    //并行排序
    para_quickSort(data1,0,whi_n-1,whi_m,0,now_id);
    whi_end = MPI_Wtime();
    if(now_id == 0){
        double sta_time2 = MPI_Wtime();
        quickSort(data2,0,n-1);
        double end_time2 = MPI_Wtime();
        // printf("----------\n");
        // printf("The final array data1 : \n");
        // print(data1,n);
        // printf("The final array data2 : \n");
        // print(data2,n);
        // printf("----------\n");
        printf("串行时间 = %f s\n",end_time2-sta_time2);
        printf("并行时间 = %f s\n",whi_end-whi_sta);
    }
    MPI_Finalize();
    return 0;
}
