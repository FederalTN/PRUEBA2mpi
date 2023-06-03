
#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) 
{
    MPI_Status status;
    int rank, size, msgR,msgS;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size); 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 

    if(rank == 0){

        msgS=rank*10;
        printf("processor ori=%d (total %d) msg send %d des=%d\n",rank,size,msgS, rank+1);
        MPI_Send(&msgS,1, MPI_INT ,rank+1 ,0 ,MPI_COMM_WORLD); 
        
    }else{
        MPI_Recv(&msgR, 1,MPI_INT,rank-1 ,0 ,MPI_COMM_WORLD ,&status); 


        printf("processor ori=%d (total %d) msg received %d des=%d\n",rank-1,size,msgR, rank);
        
        if(rank != size-1){
            msgS=rank*10;
            printf("processor ori=%d (total %d) msg send %d des=%d\n",rank,size,msgS, rank+1);
            MPI_Send(&msgS, 1 ,MPI_INT ,rank+1 , 0 ,MPI_COMM_WORLD);
        }


    }

    MPI_Finalize();
    return 0;
}

