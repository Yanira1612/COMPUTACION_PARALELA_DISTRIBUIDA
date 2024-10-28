#include <mpi.h>
#include <iostream>
#include <vector>
#include <cmath>

int find_bin(float value, const std::vector<float>& bin_maxes, float min_meas) {
    int bin = 0;
    while (bin < bin_maxes.size() && value >= bin_maxes[bin]) {
        bin++;
    }
    return bin;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const float min_meas = 0.0;
    const float max_meas = 5.0;
    const int bin_count = 5;
    const std::vector<float> data = {1.3, 2.9, 0.4, 0.3, 1.3, 4.4, 1.7, 0.4, 3.2, 0.3, 4.9, 2.4, 3.1, 4.4, 3.9, 0.4, 4.2, 4.5, 4.9, 0.9};
    int data_count = data.size();
    
    float bin_width = (max_meas - min_meas) / bin_count;
    std::vector<float> bin_maxes(bin_count);
    for (int i = 0; i < bin_count; i++) {
        bin_maxes[i] = min_meas + bin_width * (i + 1);
    }

    std::vector<int> bin_counts(bin_count, 0);
    std::vector<int> loc_bin_counts(bin_count, 0);

    int local_data_count = data_count / size;
    int remainder = data_count % size;
    
    int start_index = rank * local_data_count + std::min(rank, remainder);
    int end_index = start_index + local_data_count + (rank < remainder ? 1 : 0);

    for (int i = start_index; i < end_index; i++) {
        int bin = find_bin(data[i], bin_maxes, min_meas);
        loc_bin_counts[bin]++;
    }

    MPI_Reduce(loc_bin_counts.data(), bin_counts.data(), bin_count, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "Histograma:\n";
        for (int i = 0; i < bin_count; i++) {
            std::cout << "Bin " << i << " (" << (i == 0 ? min_meas : bin_maxes[i - 1]) << " - " << bin_maxes[i] << "): " << bin_counts[i] << std::endl;
        }
    }

    MPI_Finalize();
    return 0;
}
