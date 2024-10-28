#include <mpi.h>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

int main(int argc, char** argv) {
    int rank, size;
    long long int number_in_circle = 0, total_in_circle = 0;
    long long int total_tosses;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        std::cout << "Numero de dardos: ";
        std::cin >> total_tosses;
    }

    // Transmitiendo el número total de lanzamientos a todos los procesos
    MPI_Bcast(&total_tosses, 1, MPI_LONG_LONG_INT, 0, MPI_COMM_WORLD);

    // Calcula los dardo por proceso
    long long int tosses_per_process = total_tosses / size;
    long long int remaining_tosses = total_tosses % size;

    // Añade los lanzamientos restantes al proceso 0
    if (rank == 0) {
        tosses_per_process += remaining_tosses; // Entregar el resto al proceso 0
    }

    // Initializa random seed
    std::srand(static_cast<unsigned int>(time(0)) + rank);

    for (long long int toss = 0; toss < tosses_per_process; toss++) {
        // Genera (x, y) entre -1 y 1
        double x = (2.0 * rand() / RAND_MAX) - 1.0;
        double y = (2.0 * rand() / RAND_MAX) - 1.0;
        double distance_squared = x * x + y * y;

        printf("Proceso %d: Dardo %lld en (%f, %f)\n", rank, toss + 1, x, y);

        // verifica si es dardo esta dentro fuera del circulo
        if (distance_squared <= 1) {
            number_in_circle++;
        }
    }

    // Sumar todos los recuentos locales de dardos en el círculo para procesar 0
    MPI_Reduce(&number_in_circle, &total_in_circle, 1, MPI_LONG_LONG_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // El proceso 0 calcula la estimación de pi
    if (rank == 0) {
        double pi_estimate = (4.0 * total_in_circle) / total_tosses;
        std::cout << "Pi: " << pi_estimate << std::endl;
    }

    MPI_Finalize();
    return 0;
}
