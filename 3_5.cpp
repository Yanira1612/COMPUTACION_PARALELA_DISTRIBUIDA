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
    
    const int n = 2; // matriz cuadrada de orden n
    std::vector<double> matrix(n * n); // n x n matriz
    std::vector<double> vector(n);       // Input vector
    std::vector<double> local_result(n / size); // Resultado local para cada proceso

    if (rank == 0) {
        // Initializa la matriz
        std::cout << "ingresa los elementos de la matriz (" << n << "x" << n << "):\n";
        for (int i = 0; i < n * n; ++i) {
            std::cin >> matrix[i];
        }

        // Initialize el vector
        std::cout << "Ingresa elementos para el vector (" << n << "):\n";
        for (int i = 0; i < n; ++i) {
            std::cin >> vector[i];
        }
    }

    // Broadcast - transmitir el vector a todos los procesos
    MPI_Bcast(vector.data(), n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Distribuir la matriz a todos los procesos
    int rows_per_process = n / size;
    std::vector<double> local_matrix(rows_per_process * n);

    // El proceso 0 envía su parte de la matriz a todos los demás procesos.
    MPI_Scatter(matrix.data(), rows_per_process * n, MPI_DOUBLE, local_matrix.data(), rows_per_process * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Multiplicación de matriz-vector local
    for (int i = 0; i < rows_per_process; ++i) {
        local_result[i] = 0; // Initialize local result
        for (int j = 0; j < n; ++j) {
            local_result[i] += local_matrix[i * n + j] * vector[j];
        }
    }

    // Recopilar todos los resultados locales para procesar en 0
    std::vector<double> final_result(n); // Final result
    MPI_Gather(local_result.data(), rows_per_process, MPI_DOUBLE, final_result.data(), rows_per_process, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Process 0 imprime el resultado
    if (rank == 0) {
        std::cout << "Resultado:\n";
        print_vector(final_result);
    }

    MPI_Finalize();
    return 0;
}
