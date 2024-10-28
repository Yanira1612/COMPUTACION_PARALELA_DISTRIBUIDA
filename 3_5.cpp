#include <mpi.h>
#include <iostream>
#include <vector>

void print_vector(const std::vector<double>& vec) {
    for (double val : vec) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    const int n = 2; // Assume a square matrix of order n
    std::vector<double> matrix(n * n); // n x n matrix
    std::vector<double> vector(n);       // Input vector
    std::vector<double> local_result(n / size); // Local result for each process

    if (rank == 0) {
        // Initialize the matrix
        std::cout << "Enter the elements of the matrix (" << n << "x" << n << "):\n";
        for (int i = 0; i < n * n; ++i) {
            std::cin >> matrix[i];
        }

        // Initialize the vector
        std::cout << "Enter the elements of the vector (" << n << "):\n";
        for (int i = 0; i < n; ++i) {
            std::cin >> vector[i];
        }
    }

    // Broadcast the vector to all processes
    MPI_Bcast(vector.data(), n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Distribute the matrix to all processes
    int rows_per_process = n / size;
    std::vector<double> local_matrix(rows_per_process * n);

    // Process 0 sends its portion of the matrix to all other processes
    MPI_Scatter(matrix.data(), rows_per_process * n, MPI_DOUBLE, local_matrix.data(), rows_per_process * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Local matrix-vector multiplication
    for (int i = 0; i < rows_per_process; ++i) {
        local_result[i] = 0; // Initialize local result
        for (int j = 0; j < n; ++j) {
            local_result[i] += local_matrix[i * n + j] * vector[j];
        }
    }

    // Gather all local results to process 0
    std::vector<double> final_result(n); // Final result
    MPI_Gather(local_result.data(), rows_per_process, MPI_DOUBLE, final_result.data(), rows_per_process, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Process 0 prints the result
    if (rank == 0) {
        std::cout << "Result of matrix-vector multiplication:\n";
        print_vector(final_result);
    }

    MPI_Finalize();
    return 0;
}
