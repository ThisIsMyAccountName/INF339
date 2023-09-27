#include <iostream>
#include <vector>
#include <mpi.h>

using namespace std;

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int scale = 1 << 4;

    // Initialize the matrix and vector
    std::vector<std::vector<double> > matrix(scale, vector<double>(scale / size));
    std::vector<double> vector(scale);
    std::vector<double> result(scale / size, 0.0);

    // Calculate the local portion of the result
    int local_columns = scale / size;
    int start_col = rank * local_columns;
    int end_col = (rank == size - 1) ? scale : start_col + local_columns;

    // Initialize the matrix with its rank-relevant data (columns)
    for (int j = 0; j < end_col - start_col; j++) {
        for (int i = 0; i < scale; i++) {
            matrix[i][j] = (i == j + start_col) ? 2.0 : 1.0;
        }
    }

    // Initialize the vector in the root process
    if (rank == 0) {
        for (int i = 0; i < scale; i++) {
            vector[i] = i + 1.0;
        }
    }

    // Broadcast the vector to all processes
    MPI_Bcast(vector.data(), scale, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    // Calculate the local portion of the result
    for (int j = 0; j < end_col - start_col; j++) {
        for (int i = 0; i < scale; i++) {
            result[j] += matrix[i][j] * vector[i];
        }
    }

    // Gather the results from all processes to the root process
    MPI_Gather(result.data(), local_columns, MPI_DOUBLE, result.data(), local_columns, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    // Print the result in the root process
    if (rank == 0) {
        cout << "Result vector:" << endl;
        for (int i = 0; i < scale; i++) {
            cout << result[i] << " ";
        }
        cout << endl;
    }
    MPI_Finalize();

    return 0;
}
