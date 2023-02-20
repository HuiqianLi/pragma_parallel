#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<math.h>

int main(int argc, char* argv[]){
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int data = rank+1;
    int recvdata;
    MPI_Status status;
    printf("process id %d data = %d\n",rank, data);
    int logN = (int)log2(size);
    for(int i = 0; i < logN; i++) {
        int tag = i+1;
        int step = (int)pow(2,i);
        int dest = rank ^ step;
        MPI_Send(&data, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
        MPI_Recv(&recvdata, 1, MPI_INT, dest, tag, MPI_COMM_WORLD, &status);
        data += recvdata;
    }


    printf("process id %d sum is = %d\n",rank, data);

    MPI_Finalize();
    return 0;
}