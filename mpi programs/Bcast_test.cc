#include <iostream>
#include <vector>
#include <mpi.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, numProcesses;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);

    std::vector<int> data;

    if (rank == 0) {
        // Only rank 0 initializes the vector
        for (int i = 0; i < 10; i++) {
            data.push_back(i+1);
        }
    }

    // Get the size of the vector
    int vectorSize = data.size();

    // Broadcast the vector size to all processes
    MPI_Bcast(&vectorSize, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Resize the data vector on all processes
    data.resize(vectorSize);

    // Broadcast the data from rank 0 to all processes
    MPI_Bcast(data.data(), vectorSize, MPI_INT, 0, MPI_COMM_WORLD);

    // Multiply each element by its rank
    for (int i = 0; i < vectorSize; i++) {
        data[i] *= rank;
    }

    // Gather the modified data from all processes to rank 0
    std::vector<int> gatheredData(vectorSize * numProcesses);
    MPI_Gather(data.data(), vectorSize, MPI_INT, gatheredData.data(), vectorSize, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        // Process the gathered data on rank 0
        std::cout << "Rank 0 received the modified data: ";
        for (int i = 0; i < gatheredData.size(); i++) {
            std::cout << gatheredData[i] << " ";
        }
        std::cout << std::endl;
    }

    MPI_Finalize();
    return 0;
}
