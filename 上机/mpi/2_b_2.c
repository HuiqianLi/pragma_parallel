#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<math.h>

int main(int argc, char* argv[]){
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int data, recvdata, logN;
    MPI_Status status;
    data = rank + 1;
    recvdata = 0;
    logN = (int)log2(size);
    printf("process id: %d,data:%d\n", rank, data);
    //sum
    for (int i = 1; i <= logN; i++)
    {
        int tag = i;
        int step = (int)pow(2, i);
        if (rank % step == 0)
        {
            MPI_Recv(&recvdata, 1, MPI_INT, rank+step/2, tag, MPI_COMM_WORLD, &status);
            data += recvdata;
        }
        else
         if(rank % step == step/2)
        {
            MPI_Send(&data, 1, MPI_INT, rank-step/2, tag, MPI_COMM_WORLD);
        }
    }
    //spread
    for (int i = logN; i >0; i--)
    {
        int tag = i;
        int step = (int)pow(2, i);
        if (rank % step == 0)
        {
            MPI_Send(&data, 1, MPI_INT, rank+step/2, tag, MPI_COMM_WORLD);
        }
        else
        if(rank % step == step/2)
        {
            MPI_Recv(&recvdata, 1, MPI_INT, rank-step/2, tag, MPI_COMM_WORLD, &status);
            data = recvdata;
        }
        
    }
    printf("%d sum is %d\n", rank, data);
    MPI_Finalize();
    return 0;
}