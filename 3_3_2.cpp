#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    int rank, size;
    int local_sum = 0;
    int global_sum = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Supongamos que cada proceso tiene un número local para sumar
    local_sum = rank + 1; // Por ejemplo, el rango del proceso + 1

    // Iniciar el árbol de suma
    for (int step = 1; step < size; step *= 2) {
        if (rank % (2 * step) == 0) { // Proceso padre
            int received_sum = 0;
            if (rank + step < size) { // Asegurarse de que el proceso hijo existe
                MPI_Recv(&received_sum, 1, MPI_INT, rank + step, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
            local_sum += received_sum;
        } else if (rank % (2 * step) == step) { // Proceso hijo
            MPI_Send(&local_sum, 1, MPI_INT, rank - step, 0, MPI_COMM_WORLD);
        }
    }

    // Solo el proceso 0 tendrá el resultado final
    if (rank == 0) {
        global_sum = local_sum;
        std::cout << "Suma global: " << global_sum << std::endl;
    }

    MPI_Finalize();
    return 0;
}
