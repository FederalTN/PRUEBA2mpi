

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
    printf("proceso[%d]  creo el número num = (%d) \n", world_rank, num);

    int sum;
    MPI_Reduce(&num,&sum,1,MPI_INT,MPI_SUM,0,  MPI_COMM_WORLD);


    if (world_rank == 0) {
        printf("proceso[%d] SUMA TOTAL = (%d) \n", world_rank, sum);
    }

    MPI_Finalize();
    return 0;
}
