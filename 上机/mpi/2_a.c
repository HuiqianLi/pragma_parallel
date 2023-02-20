#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#define NODES_NUM 2

int main(int argc, char* argv[]){
    int rank, size;
    char processor_name[MPI_MAX_PROCESSOR_NAME];//MPI_MAX_PROCESSOR_NAME：似乎是一个整数，值为128.
    int nameLen;
    MPI_Status status;

    //MPI_COMM_WORLD：通讯子，“一组可以互发消息的进程集合”
    MPI_Init(&argc, &argv);//初始化
    MPI_Comm_size(MPI_COMM_WORLD, &size); //通信子中总进程数
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);//进程号
    MPI_Get_processor_name(processor_name, &nameLen);//得到处理器的名字
    MPI_Comm splitWorld;//进程分通讯域
    int rank2, size2, color;
    color = processor_name[nameLen-1] - '0';
    MPI_Comm_split(MPI_COMM_WORLD, color, rank, &splitWorld);
    MPI_Barrier(MPI_COMM_WORLD);//跨组的所有成员启动屏障同步
    MPI_Comm_size(splitWorld, &size2);
    MPI_Comm_rank(splitWorld, &rank2);
    printf("processor_name: %s,rank1:%d,rank2:%d\n", processor_name, rank, rank2);
    int sendData, recvData, tag;
    recvData = 0;
    if (rank == 0) {
        sendData = 9999;
        for (tag = 1; tag <= NODES_NUM; tag++) {
            //发送函数
            MPI_Send(&sendData, 1, MPI_INT, (tag-1)*(size/NODES_NUM), tag, MPI_COMM_WORLD);
        }
    }
    if (rank2 == 0) {
        //接收函数
        MPI_Recv(&recvData, 1, MPI_INT, 0, color, MPI_COMM_WORLD, &status);
    }
    
    MPI_Bcast(&recvData, 1, MPI_INT, 0, splitWorld);//将数据从组的一个成员广播到组的所有成员
    printf("processor_name: %s,rank: %d, RecvData: %d\n", processor_name, rank2, recvData);

    MPI_Comm_free(&splitWorld);
    MPI_Finalize();//MPI程序结束
    return 0;
}