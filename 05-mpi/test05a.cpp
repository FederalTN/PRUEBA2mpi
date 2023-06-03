#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv){

	int rank, value;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 

    if (rank == 0) {

        printf("Enter a number to broadcast:\n");
	    scanf("%d", &value);
    } else {
    	printf("process %d: Before MPI_Bcast, value is %d\n", rank, value); 
	}

    MPI_Bcast(&value, 1, MPI_INT, 0, MPI_COMM_WORLD);

    printf("process %d: After MPI_Bcast, value is %d\n", rank, value);

    MPI_Finalize();

	return 0;
}