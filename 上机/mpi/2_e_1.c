#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<math.h>
#include<time.h>
#define N 8

int main(int argc,char *argv[]){
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);//分组
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    double A[N+2][N+2];
    double B[N+2][N+2];//因为可能会越界我们这里用n+2作为大小
    for(int i=0;i<N+2;i++){//初始化A，所有线程都生成相同的数据，不用进行广播数据
        for(int j=0;j <N+2;j++){
            A[i][j]=i+j;
        }
    }
    int count=N/size;
    int left=N%size;
    if(rank<left)
        count++;//count表明了对应处理器要进行多少行的计算
    for(int i=0;i<count;i++){//计算数据
        int index=rank+i*size;//对应第几行
        for(int j=1;j<N-1;j++)
            B[index][j]=(A[index-1][j]+A[index][j+1]+A[index+1][j]+A[index][j-1])/4.0;
    }
    MPI_Barrier(MPI_COMM_WORLD);
    //将数据都传递给0号线程
    for(int i=0;i<N;i++){
        if(i%size==rank)//如果不是0向0发送数据
            MPI_Send(&B[i][1],N,MPI_DOUBLE,0,0,MPI_COMM_WORLD);
        if(rank==0)//如果是0就接受数据
            MPI_Recv(&B[i][1],N,MPI_DOUBLE,i%size,MPI_ANY_TAG,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    if(rank==0){
        for(int i=1;i<N-1;i++){
            for(int j=1;j <N-1;j++){
                printf("%.2lf \t",B[i][j]);
            }
            printf("\n");
        }
    }

    MPI_Finalize();
    return 0;
}