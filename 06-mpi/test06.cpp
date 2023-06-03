#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
 

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);
 
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if(size != 2)
    {
        printf("Esta aplicación esta creada para correr con 2 procesos.\n");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
 
    int MAX_SIZE = 8;
    int my_rank,buffer[MAX_SIZE];
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    
    if(my_rank==0){

        int bufferAux[MAX_SIZE]={12,1,3,5,4,3,2,2};
        for(int i=0; i<MAX_SIZE;i++) buffer[i] = bufferAux[i];
        printf("Proceso %d: envia en un mensaje broadcast el arreglo a todos los procesos.\n", my_rank);
        for(int i = 0; i < MAX_SIZE; i++){
                printf(" %d", buffer[i]);
        }
        printf("\n");

    } 
    MPI_Bcast(buffer, MAX_SIZE, MPI_INT, 0, MPI_COMM_WORLD);
    
    int sum=0;
    int ini=my_rank*4;
    int end=my_rank*4 + 4;
    for(int i=ini; i<end; i++){
        sum=sum+buffer[i];
    }
    printf("ini=%d -fin=%d sum=%d en el proceso=%d \n",ini, end, sum, my_rank);

    int results[2];
    MPI_Gather( &sum, 1, MPI_INT, results, 1, MPI_INT, 0, MPI_COMM_WORLD );

   // Process 0 prints what it collected
   int sumF=0;
    if ( my_rank == 0 ) {
        for ( int i = 0; i < size; i++ ) {
            printf( "Proceso 0 recibe %d desde proceso %d\n", results[i], i );
            sumF=sumF+results[i];
        }

        printf( "Proceso 0 suma Final %d\n", sumF );
 
    }

    MPI_Finalize();
 
    return EXIT_SUCCESS;
}
