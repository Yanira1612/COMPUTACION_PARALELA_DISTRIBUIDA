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

    // Broadcasting the total number of tosses to all processes
    MPI_Bcast(&total_tosses, 1, MPI_LONG_LONG_INT, 0, MPI_COMM_WORLD);

    // Calculate tosses per process
    long long int tosses_per_process = total_tosses / size;
    long long int remaining_tosses = total_tosses % size;

    // Add the remaining tosses to the process 0
    if (rank == 0) {
        tosses_per_process += remaining_tosses; // Give the remainder to process 0
    }

    // Initialize random seed
    std::srand(static_cast<unsigned int>(time(0)) + rank);

    for (long long int toss = 0; toss < tosses_per_process; toss++) {
        // Generate random (x, y) between -1 and 1
        double x = (2.0 * rand() / RAND_MAX) - 1.0;
        double y = (2.0 * rand() / RAND_MAX) - 1.0;
        double distance_squared = x * x + y * y;

        printf("Proceso %d: Dardo %lld en (%f, %f)\n", rank, toss + 1, x, y);

        // Check if the dart is inside the circle
        if (distance_squared <= 1) {
            number_in_circle++;
        }
    }

    // Sum up all local counts of darts in the circle to process 0
    MPI_Reduce(&number_in_circle, &total_in_circle, 1, MPI_LONG_LONG_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Process 0 computes the estimate of pi
    if (rank == 0) {
        double pi_estimate = (4.0 * total_in_circle) / total_tosses;
        std::cout << "Pi: " << pi_estimate << std::endl;
    }

    MPI_Finalize();
    return 0;
}
