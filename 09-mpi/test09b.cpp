#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
 
/**
genera grupos de ejecución de menor tamaño, en este caso si genero 1 grupo de tamaño 3 
este grupo lo fija sobre los procesos (0,1,2) y los vuelve a renombrar como (0,1,2) del subgrupo
luego crea el grupo para ejecutar procesos y le hace un broadcast específico a ese grupo

ejecutar 
mpirun -np 8 --oversubscribe ./test09 
 **/
int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);
 
    // Check that 4 processes are used
    int comm_size;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    if(comm_size <= 4)
    {
        printf("por lo menos deben ser 5 procesos.\n");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
 
    // Get the group or processes of the default communicator
    MPI_Group world_group;
    MPI_Comm_group(MPI_COMM_WORLD, &world_group);
 
    // Get my rank in the world group
    int my_world_group_rank;
    MPI_Group_rank(world_group, &my_world_group_rank);
 
    // Create the small group by including only processes 1 and 3 from the world group
    int small_group_ranks[3] = {0,1,2};
    MPI_Group small_group;
    MPI_Group_incl(world_group, 3, small_group_ranks, &small_group);
 
    // Create the new communicator from that group of processes.
    MPI_Comm new_communicator;
    MPI_Comm_create(MPI_COMM_WORLD, small_group, &new_communicator);
 
    // Do a broadcast between all processes
    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    int value;
    MPI_Bcast(&value, 1, MPI_INT, 0, MPI_COMM_WORLD);
    printf("Process %d took part to the global communicator broadcast.\n", my_rank);
 
    // Let's wait all processes before proceeding to the second phase.
    MPI_Barrier(MPI_COMM_WORLD);
 
    // Do a broadcast only between the processes of the new communicator.
    if(new_communicator == MPI_COMM_NULL)
    {
        // I am not part of the new communicator, I can't participate to that broadcast.
        printf("Process %d did not take part to the new communicator broadcast.\n", my_rank);
    }
    else
    {
        // I am part of the new communicator, I can participate to that broadcast.
        MPI_Bcast(&value, 1, MPI_INT, 0, new_communicator);
        printf("Process %d took part to the new communicator broadcast.\n", my_rank);
    }
 
    MPI_Finalize();
 
    return EXIT_SUCCESS;
}