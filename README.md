dependencias:

mpi:    sudo apt-get install libopenmpi-dev

cd mainMPI

// Compilar
mpicxx main.cpp -o mpi_main

// Ejecutar
mpiexec -n 4 ./mpi_main