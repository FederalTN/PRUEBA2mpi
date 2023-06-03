// archivo main.cpp
#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
 
int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);
 
    // Check that the application is run with 4 processes.
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if(size != 4)
    {
        printf("El parametro de ejecucion debe de ser 4\n");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
 
    printf("bien hecho\n");
    MPI_Finalize();
 
    return EXIT_SUCCESS;
}