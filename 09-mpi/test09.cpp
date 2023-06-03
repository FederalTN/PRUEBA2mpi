#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
 
/**
genera grupos de ejecución de menor tamaño, en este caso si genero 1 grupo de tamaño 3 
este grupo lo fija sobre los procesos (1,3,4) y los vuelve a renombrar como (0,1,2) del subgrupo

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
    int small_group_ranks[3] = {1, 3, 4};
    MPI_Group small_group;
    MPI_Group_incl(world_group, 3, small_group_ranks, &small_group);
 
    // Get my rank in the small group
    int my_small_group_rank;
    MPI_Group_rank(small_group, &my_small_group_rank);
 
    // Continue only if we are part of the small group
    if(my_small_group_rank != MPI_UNDEFINED)
    {
        printf("I am process %d in world group and %d in small group.\n", my_world_group_rank, my_small_group_rank);
    }
    else
    {
        printf("I am process %d in world group but I am not part of the small group.\n", my_world_group_rank);
    }
 
    MPI_Finalize();
 
    return EXIT_SUCCESS;
}