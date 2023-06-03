// archivo main.cpp
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include "mpi.h"

std::vector<int> leerVector(const std::string& filename);

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    // Condicion de los 4 procesos
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size != 4)
    {
        printf("El parámetro de ejecución debe ser 4\n");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    // Read vector from file
    std::string filename = "vector.txt";  // Nombre del archivo
    std::vector<int> vector = leerVector(filename);


    printf("\nbien hecho\n");

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

    return vector;
}