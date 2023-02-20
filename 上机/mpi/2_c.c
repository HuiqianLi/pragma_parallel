#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<math.h>
#define N 8//设定N大点，为了让处理器小于N
#define INDEX(i, j, N) (((i)*(N))+(j))

// 打印矩阵
void print_mat(int *a, int num){
    for(int i = 0; i < num; i++){
        for(int j = 0; j < num; j++){
            printf("%d\t", a[INDEX(i, j, num)]);
            }
            printf("\n");
            }
}

int main(int argc,char *argv[]){
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);//分组
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    int A[N][N];
    int B[N][N];//因为可能会越界我们这里用n+2作为大小
    int C[N][N];
    int T[N][N];
    //我们这里把矩阵分给4个处理器，根号p也就是2，所以每块的大小是4*4，因为实验平台只能支持处理器的的数量只能为4，不能为9,16
    for(int i=0;i<N;i++){//初始化A，所有线程都生成相同的数据，不用进行广播数据
        for(int j=0;j <N;j++){
            A[i][j]=i+j+1;
            B[i][j]=2*N-i-j+1;
            C[i][j]=0;
        }
    }
    //    for(int i=0;i<N;i++){//初始化A，所有线程都生成相同的数据，不用进行广播数据
    // 		for(int j=0;j <N;j++){
    // 			for(int k=0;k<N;k++)
    //             C[i][j]+=A[i][k]*B[k][j];
    // 		}
    // 	} 
    int BN=sqrt(size);
    int TN=size;
    int row=rank/BN;
    int line=rank%BN;
    int times=0;
    MPI_Datatype block;
    MPI_Type_vector(N/2, N/2, N, MPI_INT, &block);//定义新的数据类型
    MPI_Type_commit(&block);
    for(int i=0;i<BN;i++){//两次循环
        for(int j=0;j<BN;j++){//广播A块
            if(row==j&&line==(j+times)%BN){//向同行元素广播A
                for(int k=0;k<BN;k++){
                    MPI_Send(&A[row*size][line*size],1,block,row*BN+k,0,MPI_COMM_WORLD);//发送A
                }
            }
            if(row==j)     //接受A,放到T中
                MPI_Recv(&T[row*size][line*size],1,block,row*BN+(j+times)%BN,MPI_ANY_TAG,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        }
        MPI_Barrier(MPI_COMM_WORLD);//等待传送完成
        for(int j=row*TN;j<row*TN+TN;j++){
            for(int k=line*TN;k<line*TN+TN;k++){
                for(int p=0;p<TN;p++)
                    C[j][k]+=T[j][line*TN+p]*B[row*TN+p][k];//结果放到C中
                //printf("%d:%d=%d*%d:%d+%d\t",rank,C[j][k],T[j][k],B[j][k],j,k);
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);//等待计算完成
        //B矩阵的块向上传播一块，这个容易造成死锁
        MPI_Send(&B[row*TN][line*TN],1,block,(row-1+BN)%BN*BN+line,0,MPI_COMM_WORLD);
        MPI_Recv(&T[row*TN][line*TN],1,block,(row+1)%BN*BN+line,MPI_ANY_TAG,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        // printf("走过三");
        // MPI_Barrier(MPI_COMM_WORLD);//等待传输结束
        for(int j=row*TN;j<row*TN+TN;j++)
            for(int k=line*TN;k<line*TN+TN;k++)
                B[j][k]=T[j][k];//更新B

        MPI_Barrier(MPI_COMM_WORLD);//等待更新结束
        times++;
    }
    MPI_Barrier(MPI_COMM_WORLD);//等待更新结束
    //开始汇总数据到0号进程
    for(int i=1;i<size;i++){
        if(rank==i)
            MPI_Send(&C[row*TN][line*TN],1,block,0,0,MPI_COMM_WORLD);
        if(rank==0)
            MPI_Recv(&C[i/BN*TN][i%BN*TN],1,block,i,MPI_ANY_TAG,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    }
    if(rank==0){
        printf("C:\n");
        for(int i=0;i<N;i++){//初始化A，所有线程都生成相同的数据，不用进行广播数据
            for(int j=0;j <N;j++){
                printf("%d \t",C[i][j]);
            }
            printf("\n");
        }
        printf("A:\n");
        print_mat(*A, N);
        printf("B:\n");
        print_mat(*B, N);
    }


    MPI_Finalize();
    return 0;
}