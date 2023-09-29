#include <iostream>
#include <vector>
#include <mpi.h>

using namespace std;

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int scale = 1 << 16;

    // Initialize the matrix and vector
    std::vector<std::vector<double>> matrix(scale / size, vector<double>(scale));
    std::vector<double> vector(scale);
    std::vector<double> result(scale, 0.0);

    // Calculate the local portion of the result
    int local_rows = scale / size;
    int start_row = rank * local_rows;
    int end_row = (rank == size - 1) ? scale : start_row + local_rows;

    // Initialize the matrix with its rank relevant data
    for (int i = 0; i < end_row - start_row; i++) {
        for (int j = 0; j < scale; j++) {
            matrix[i][j] = (i + start_row == j) ? 1.0 : 0.0;
        }
    }

    // Timing variables
    double end_time, start_time;
    
    // Initialize the vector in the root process
    if (rank == 0) {
        for (int i = 0; i < scale; i++) {
            vector[i] = i + 1.0;
        }
    }
    
    // Start timing the vector broadcast
    // a loop of 100 to get a more accurate time
    int total_time = 0;
    for (int i = 0; i < 100; i++) {
        MPI_Barrier(MPI_COMM_WORLD);
        start_time = MPI_Wtime();
        // Broadcast the vector to all processes
        MPI_Bcast(vector.data(), scale, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        // Stop timing the vector broadcast
        end_time = MPI_Wtime();
        total_time += end_time - start_time;
    }
    if (rank == 0) {
        cout << total_time / 100 << endl;
    }

    MPI_Finalize();

    return 0;
}
