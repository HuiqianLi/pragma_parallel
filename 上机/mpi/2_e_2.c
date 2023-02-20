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
    int BN=sqrt(size);
    int h=N/sqrt(size);
    int w=N/sqrt(size);//定义高宽，我们假设可以整除
    int row=rank/BN;
    int line=rank/BN;

    MPI_Datatype block;
    MPI_Type_vector(h, w, N+2, MPI_DOUBLE, &block);//定义新的数据类型
    MPI_Type_commit(&block);
    for(int i=row*h;i<row*h+h;i++){//计算数据
        for(int j=line*w;j<line*w+w;j++){
            if(i>0&&j>0)
                B[i][j]=(A[i-1][j]+A[i][j+1]+A[i+1][j]+A[i][j-1])/4.0;
            else if(i==0&&j==0)
                B[i][j]=(A[i][j+1]+A[i+1][j])/2.0;
            else if(i==0)
                B[i][j]=(A[i][j+1]+A[i+1][j]+A[i][j-1])/3.0;
            else if(j==0)
                B[i][j]=(A[i-1][j]+A[i][j+1]+A[i+1][j])/3.0;
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);//等待计算结束
    for(int i=1;i<size;i++){
        if(i==rank)//如果不是0向0发送数据
        MPI_Send(&B[row*h][line*w],1,block,0,0,MPI_COMM_WORLD);
        if(rank==0)//如果是0就接受数据
        MPI_Recv(&B[i/BN*h][i%BN*w],1,block,i,MPI_ANY_TAG,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    }
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