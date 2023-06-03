// archivo main.cpp
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include "mpi.h"

using namespace std;

std::vector<int> leerVector(const std::string& filename);
void dividirVector(const std::vector<int>& vector, int rank);

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int rank;  // Declare the rank variable
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // Get the rank of each process

    // Condicion de los 4 procesos
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size != 4)
    {
        printf("El parámetro de ejecución debe ser 4\n");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    if (rank == 0)
    {
        // Read vector from file
        std::string filename = "vector.txt";
        std::vector<int> vector = leerVector(filename);

        // Dividir el vector y enviar las partes a los otros procesos
        dividirVector(vector, rank);
    }
    else
    {
        // Recibir la parte correspondiente del vector desde el proceso 0
        int chunkSize;
        MPI_Bcast(&chunkSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
        std::vector<int> localVector(chunkSize);
        MPI_Scatter(nullptr, chunkSize, MPI_INT, localVector.data(), chunkSize, MPI_INT, 0, MPI_COMM_WORLD);

        // Imprimir los elementos del vector en cada proceso
        printf("P%d: ", rank);
        for (int i = 0; i < chunkSize; ++i)
        {
            printf("%d ", localVector[i]);
        }
        printf("\n");
    }

    MPI_Finalize();

    return EXIT_SUCCESS;
}

std::vector<int> leerVector(const std::string& filename)
{
    std::vector<int> vector;

    std::ifstream file(filename);
    if (file.is_open())
    {
        std::string line;
        if (std::getline(file, line))
        {
            std::stringstream ss(line);
            std::string number;
            while (std::getline(ss, number, ';'))
            {
                vector.push_back(std::stoi(number));
            }
        }
        file.close();
    }
    else
    {
        printf("No se pudo abrir el archivo '%s'\n", filename.c_str());
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    if (vector.size() % 4 != 0 || vector.size() <= 4)
    {
        printf("El largo del vector debe ser múltiplo de 4 y mayor que 4.\n");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    printf("P0\nlee Vector = %d", vector[0]);
    for (int i = 1; i < vector.size(); i++) {
        printf(";%d", vector[i]);
    }
    printf("\n");
    return vector;
}

void dividirVector(const std::vector<int>& vector, int rank)
{
    printf("divide la informacion\n");
    int size = vector.size();
    int chunkSize = size / 4;  // Tamaño de cada parte

    int divisor = 0;
    for (int i = 0; i < size; ++i)
    {   
        if (divisor == chunkSize)
        {
            printf("/ ");
            divisor=0;
        }
        printf("%d ", vector[i]);
        divisor++;
    }
    printf("\n\n");

    // Enviar el tamaño de la parte a todos los procesos
    MPI_Bcast(&chunkSize, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Enviar la parte correspondiente a cada proceso
    std::vector<int> localVector(chunkSize);
    MPI_Scatter(vector.data(), chunkSize, MPI_INT, localVector.data(), chunkSize, MPI_INT, 0, MPI_COMM_WORLD);

    // Imprimir los elementos del vector en cada proceso
    printf("P%d: ", rank);
    for (int i = 0; i < chunkSize; ++i)
    {
        printf("%d ", localVector[i]);
    }
    printf("\n");
}
