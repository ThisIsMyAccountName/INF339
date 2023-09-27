#include <iostream>
#include <vector>
#include <mpi.h>

using namespace std;

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);


    const int scale = 1 << 10;
    const int modSpace = 1 << 16;

    // Initialize the matrix and vector
    std::vector<std::vector<double> > matrix(scale/size, vector<double>(scale));
    std::vector<double> vector(scale);
    std::vector<double> result(scale, 0.0);

    // Calculate the local portion of the result
    int local_rows = scale / size;
    int start_row = rank * local_rows;
    int end_row = (rank == size - 1) ? scale : start_row + local_rows;

    // initialize the matrix with its rank relevant data
    for (int i = 0; i < end_row - start_row; i++) {
        for (int j = 0; j < scale; j++) {
            matrix[i][j] = (i + start_row == j) ? 1.0 : 0.0;
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

    // // print the broadcasted vector
    // cout << "Rank " << rank << " received the vector: ";
    // for (int i = 0; i < scale; i++) {
    //     cout << vector[i] << " ";
    // }
    // cout << endl;
    // print the generated matrix
    // cout << "Rank " << rank << " generated the matrix: " << endl;
    // for (int i = 0; i < end_row - start_row; i++) {
    //     for (int j = 0; j < scale; j++) {
    //         cout << matrix[i][j] << " ";
    //     }
    //     cout << "N";
    // }
    // cout << endl;

    // Calculate the local portion of the result
    for (int i = 0; i < end_row - start_row; i++) {
        for (int j = 0; j < scale; j++) {
            result[i+(scale/size)*rank] += matrix[i][j] * vector[j];
            // result[i+(scale/size)*rank] = fmod(result[i], modSpace);
        }
    }

    // Gather the results from all processes to the root process
    MPI_Gather(result.data() + start_row, local_rows, MPI_DOUBLE, result.data(), local_rows, MPI_DOUBLE, 0, MPI_COMM_WORLD);
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