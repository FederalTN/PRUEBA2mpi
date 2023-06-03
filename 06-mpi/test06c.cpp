
#include <iostream>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
 

int main(int argc, char* argv[])

{
    int myid, numprocs;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	int *InputArr = new int[numprocs];

	if(myid == 0)
	{
		srand(time(NULL));
		for(int i = 0; i < numprocs*2; i++){
			InputArr[i] = rand() % 100;
            printf("%d) dato creado en proceso[%d] = %d \n",i, myid, InputArr[i]);
        }
	}

	int *rec = new int[2];
	MPI_Scatter(InputArr, 2, MPI_INT, rec, 2, MPI_INT, 0, MPI_COMM_WORLD);

	printf("data in process[%d] = (%d,%d) \n", myid, rec[0],rec[1]);
	MPI_Finalize();

return 0;
}