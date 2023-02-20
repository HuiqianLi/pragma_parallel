#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<math.h>
#include<time.h>
#define N 3

int main(int argc,char *argv[]){
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);//分组
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    MPI_Comm severworld;
    MPI_Group tempgroup,severgroup;
    MPI_Comm_group(MPI_COMM_WORLD,&tempgroup);//创建无通讯能力组
    int ranks[N]={0,1,2};
    MPI_Group_incl(tempgroup,3,ranks,&severgroup);//选取前三个进程为服务器
    MPI_Comm_create(MPI_COMM_WORLD,severgroup,&severworld);//赋予通讯能力
    for(int i=0;i<10;i++){//进行10次消息传递
        if(rank<N){//服务器进程
            int left=size-3;//算出还有多少进程
            int local_sum=0;//当前保存数据
            int count=0;
            for(int j=rank+N;j<size;j+=N){//获取数据
                int tempnum;
                MPI_Recv(&tempnum,1,MPI_INT,j,MPI_ANY_TAG,MPI_COMM_WORLD,MPI_STATUS_IGNORE);//从工作进程获取随机数
                local_sum+=tempnum;
                count++;
            }
            MPI_Barrier(severworld); //等待拿到所有数字 
            int ave=local_sum/count;//获得当前服务器的平均数
            int res=ave;//存放最终平均数
            for(int j=0;j<N;j++){//发给其他人并且拿到平均数
                int tempnum;
                if(j!=rank){//不是自己就发送再接受
                    MPI_Send(&ave,1,MPI_INT,j,0,MPI_COMM_WORLD);
                    MPI_Recv(&tempnum,1,MPI_INT,j,MPI_ANY_TAG,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
                    res+=tempnum;
                }
            }
            MPI_Barrier(severworld);//等待收集完成
            res=res/(size-3);
            printf("rank%dgetres%d\n",rank,res);
            //向工作进程发送平均数
            for(int j=rank+N;j<size;j+=N){
                MPI_Send(&res,1,MPI_INT,j,0,MPI_COMM_WORLD);
            }
        }
        else{//工作进程
            srand((unsigned)time(NULL));
            int random = rand()%1000+1;//生成100以内随机数
            int pos=rank-N;//得到当前工作进程序号
            int rp=pos%N;//拿到接收的服务器进程序号
            MPI_Send(&random,1,MPI_INT,rp,0,MPI_COMM_WORLD);//发送
            int res;
            MPI_Recv(&res,1,MPI_INT,rp,MPI_ANY_TAG,MPI_COMM_WORLD,MPI_STATUS_IGNORE);//拿到结果
            printf("rank%dgetres%d\n",rank,res);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}