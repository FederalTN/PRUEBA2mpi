// archivo main.cpp
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include "mpi.h"

using namespace std;

std::vector<int> leerVector(const std::string& filename);   // funcion que lee el vector de un archivo .txt
std::vector<int> dividirVector(const std::vector<int>& vector, int rank);   //funcion que divide el vector y asigna los subvectores a cada proceso

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int rank;  // Rank del proceso
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // Obtener el rank de cada proceso

    // 1) Condicion de los 4 procesos
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size != 4)
    {
        printf("El parámetro de ejecución debe ser 4\n");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    std::vector<int> vectorAsignado;    // sub vector que se asignara a cada proceso en el paso 4

    // P0 es el encargado de leer el archivo y asignar los subvectores a cada proceso, incluido asi mismo.
    if (rank == 0)
    {
        // 2) Leer el vector del archivo
        std::string filename = "vector.txt";
        std::vector<int> vector = leerVector(filename); // 3) leerVector verifica que el vector.size sea multiplo y mayor a 4

        // 4) Dividir el vector y enviar las partes a los otros procesos
        vectorAsignado = dividirVector(vector, rank);
    }
    else
    {
        // 5) Recibir la parte correspondiente del vector desde el proceso 0
        int chunkSize;
        MPI_Bcast(&chunkSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
        std::vector<int> vectorLocal(chunkSize);
        MPI_Scatter(nullptr, chunkSize, MPI_INT, vectorLocal.data(), chunkSize, MPI_INT, 0, MPI_COMM_WORLD);

        // Imprimir los elementos del vector en cada proceso
        printf("P%d: ", rank);
        for (int i = 0; i < chunkSize; ++i)
        {
            printf("%d ", vectorLocal[i]);
        }
        printf("\n");
        vectorAsignado = vectorLocal;
    }

    // Barrera para asegurar que cada proceso tiene un subvector
    MPI_Barrier(MPI_COMM_WORLD);

    // 6) En cada uno de los 4 Procesos se determina el mayor de cada subvector

    int mayor = vectorAsignado[0];
    for (int i = 1; i < vectorAsignado.size(); ++i)
    {
        if (mayor < vectorAsignado[i])
        {
            mayor = vectorAsignado[i];
        }
    }
    printf("mayor en p%d = ", rank);
    printf("%d\n", mayor);

    // 7) Obtener los mayores de cada subvector y realizar la suma en P0
    
    // Barrera para asegurar que cada proceso tiene el mayor de su subvector
    MPI_Barrier(MPI_COMM_WORLD);

    // Crear un vector para almacenar los mayores de cada subvector
    std::vector<int> mayores(size);

    // Recopilar los mayores de cada subvector en el proceso 0
    MPI_Gather(&mayor, 1, MPI_INT, mayores.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        // Realizar la suma de los 4 mayores en el proceso 0
        int sumaMayores = 0;
        for (int i = 0; i < size; ++i)
        {
            sumaMayores += mayores[i];
        }
        printf("\nSuma total: %d\n", sumaMayores);
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
    // Lectura del vector que se cargo
    printf("P0\nlee Vector = %d", vector[0]);
    for (int i = 1; i < vector.size(); i++) 
    {
        printf(";%d", vector[i]);
    }
    printf("\n");
    return vector;
}

std::vector<int> dividirVector(const std::vector<int>& vector, int rank)
{
    printf("divide la informacion\n");
    int size = vector.size();
    int chunkSize = size / 4;  // Tamaño de cada sub vector

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
    std::vector<int> vectorLocal(chunkSize);
    MPI_Scatter(vector.data(), chunkSize, MPI_INT, vectorLocal.data(), chunkSize, MPI_INT, 0, MPI_COMM_WORLD);

    // Imprimir los elementos del vector en cada proceso
    printf("P%d: ", rank);
    for (int i = 0; i < chunkSize; ++i)
    {
        printf("%d ", vectorLocal[i]);
    }
    printf("\n");
    return vectorLocal;
}
