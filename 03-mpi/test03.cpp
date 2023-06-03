#include <mpi.h>
#include <iostream>

using namespace std;

int main( int argc, char* argv[] )
{
    // Init MPI
    MPI_Init( &argc, &argv);

    // Get the rank and size
    int rank, size;
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );

    // If Master
    if( rank == 0 ){

        char* message_r1 = "Hello Rank 1";
        char* message_r2 = "Hello Rank 2";

        // Send a message over tag 0
        MPI_Send( message_r1, 13, MPI_CHAR, 1, 0, MPI_COMM_WORLD );

        // Send a message over tag 1
        MPI_Send( message_r2, 13, MPI_CHAR, 2, 1, MPI_COMM_WORLD );


    }
    else{

        // Buffer
        char buffer[256];
        MPI_Status status;

        // Wait for your own message
        MPI_Recv( buffer, 13, MPI_CHAR, 0, rank-1, MPI_COMM_WORLD, &status );

        cout << "Rank: " << rank << ", Message: " << buffer << std::endl;

    }

    // Finalize MPI
    MPI_Finalize();

}