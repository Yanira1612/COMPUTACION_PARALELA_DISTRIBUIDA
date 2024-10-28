#include <mpi.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

// Función para fusionar dos vectores ordenados
std::vector<int> combine(const std::vector<int>& first, const std::vector<int>& second) {
    std::vector<int> merged;
    int index1 = 0, index2 = 0;
    
    while (index1 < first.size() && index2 < second.size()) {
        if (first[index1] < second[index2]) {
            merged.push_back(first[index1]);
            ++index1;
        } else {
            merged.push_back(second[index2]);
            ++index2;
        }
    }

    // Agregar elementos restantes
    while (index1 < first.size()) {
        merged.push_back(first[index1]);
        ++index1;
    }
    while (index2 < second.size()) {
        merged.push_back(second[index2]);
        ++index2;
    }
    return merged;
}

int main(int argc, char* argv[]) {
    int rank, size;
    int total_elements;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        std::cout << "Tamaño de la lista (n): ";
        std::cin >> total_elements;
    }

    MPI_Bcast(&total_elements, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int elements_per_process = total_elements / size;
    std::vector<int> local_array(elements_per_process);

    srand(static_cast<unsigned>(time(0)) + rank);

    for (int i = 0; i < elements_per_process; ++i) {
        local_array[i] = rand() % 100; // Generar números aleatorios entre 0 y 99
    }

    // Ordenar el arreglo local
    std::sort(local_array.begin(), local_array.end());

    // Mostrar los arreglos locales ordenados en cada proceso
    if (rank == 0) {
        std::cout << "Resultados Locales:" << std::endl;
    }

    for (int p = 0; p < size; ++p) {
        if (rank == p) {
            std::cout << "Proceso " << rank << ": ";
            for (int value : local_array) {
                std::cout << value << " ";
            }
            std::cout << std::endl;
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }

    int stage = 1;
    while (stage < size) {
        if (rank % (2 * stage) == 0) {
            if (rank + stage < size) {
                int received_size;
                MPI_Recv(&received_size, 1, MPI_INT, rank + stage, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                std::vector<int> incoming_list(received_size);
                MPI_Recv(incoming_list.data(), received_size, MPI_INT, rank + stage, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                local_array = combine(local_array, incoming_list);
            }
        } else {
            int sending_size = local_array.size();
            MPI_Send(&sending_size, 1, MPI_INT, rank - stage, 0, MPI_COMM_WORLD);
            MPI_Send(local_array.data(), sending_size, MPI_INT, rank - stage, 0, MPI_COMM_WORLD);
            break;
        }
        stage *= 2;
    }

    // Proceso 0 imprime la lista global ordenada
    if (rank == 0) {
        std::cout << "\nLista ordenada final:" << std::endl;
        for (int value : local_array) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }

    MPI_Finalize();
    return 0;
}
