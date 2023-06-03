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
        printf("Esta aplicación fue creada para 2 procesos.\n");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
 
    enum role_ranks { SENDER, RECEIVER };
    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    switch(my_rank)
    {
        case SENDER:
        {
            int buffer[3] = {123, 456, 789};
            printf("Proceso %d: envia un mensaje que contiene 3 ints (%d, %d, %d), pero el receptor no sabe el largo del mensaje.\n", my_rank, buffer[0], buffer[1], buffer[2]);
            MPI_Send(buffer, 3, MPI_INT, RECEIVER, 0, MPI_COMM_WORLD);
            break;
        }
        case RECEIVER:
        {
            
            MPI_Status status;
            MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
            printf("Proces0 %d: obtiene el STATUS mediante 'Probe', el cual le entreega información referente al mensaje.\n", my_rank);
 
            //obtiene el largo del mensaje basado en la información del mensaje de STATUS rescatado con "Probe"
            int count;
            MPI_Get_count(&status, MPI_INT, &count);
 
            //crea el espacio de memoria de tamaño 'count' obtenido gracias a STATUS
            int* buffer = (int*)malloc(sizeof(int) * count);
 
            //recibe el mensaje
            MPI_Recv(buffer, count, MPI_INT, SENDER, 0, MPI_COMM_WORLD, &status);
            printf("Proceso %d: recibe el mensaje con los %d ints:", my_rank, count);
            for(int i = 0; i < count; i++)
            {
                printf(" %d", buffer[i]);
            }
            printf(".\n");
            break;
        }
    }
 
    MPI_Finalize();
 
    return EXIT_SUCCESS;
}