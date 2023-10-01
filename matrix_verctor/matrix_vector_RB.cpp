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
    double avg_time = 0.0;
    // Start timing the vector broadcast
    // a loop of 100 to get a more accurate time
    for (int i = 0; i < 100; i++) {
        MPI_Barrier(MPI_COMM_WORLD);
        start_time = MPI_Wtime();
        // Broadcast the vector to all processes
        MPI_Bcast(vector.data(), scale, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        // Stop timing the vector broadcast
        end_time = MPI_Wtime();
        avg_time += end_time - start_time;
    }

    if (rank == 0) {
        cout << avg_time / 100 << endl;
    }

    // Timing the matrix-vector multiplication
    start_time = MPI_Wtime();

    // Calculate the local portion of the result
    for (int i = 0; i < end_row - start_row; i++) {
        for (int j = 0; j < scale; j++) {
            result[i + (scale / size) * rank] += matrix[i][j] * vector[j];
        }
    }

    end_time = MPI_Wtime();

    if (rank == 0) {
        cout << end_time - start_time << endl;
    }
    int avg_time2 = 0.0;
    for (int i = 0; i < 100; i++) {
        MPI_Barrier(MPI_COMM_WORLD);
        start_time = MPI_Wtime();
        // Gather the results from all processes to the root process
        MPI_Gather(result.data() + start_row, local_rows, MPI_DOUBLE, result.data(), local_rows, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        end_time = MPI_Wtime();
        avg_time2 += end_time - start_time;
    }


    if (rank == 0) {
        cout << avg_time2 / 100 << endl;
    }

    MPI_Finalize();

    return 0;
}
