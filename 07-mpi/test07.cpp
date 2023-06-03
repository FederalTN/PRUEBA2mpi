

#include <stdlib.h>
#include <mpi.h>
using namespace std;

 int main(int argc, char* argv[])

{
    int world_size;
    int world_rank;
    
	MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);


    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);
    printf("Hello world from processor %s, rank %d out of %d processors\n",
           processor_name, world_rank, world_size);

    if(world_size != 3)
    {
        printf("Esta aplicación esta creada para correr con 3 procesos.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }


    int num=world_rank+100;
    printf("data in process[%d] num = (%d) \n", world_rank, num);

    int rec[3];
    MPI_Allgather(&num,1,MPI_INT,rec, 1, MPI_INT,  MPI_COMM_WORLD);

    printf("data in process[%d] = (%d,%d,%d) \n", world_rank, rec[0],rec[1],rec[2]);

    MPI_Finalize();
    return 0;
}
