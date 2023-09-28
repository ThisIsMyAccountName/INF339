// efficient matrix vector multiplication on one core

#include <iostream>
#include <vector>
#include <mpi.h>

using namespace std;

int main(int argc, char* argv[]) {
	const int scale = 1 << 16;

	// Initialize the matrix and vector
	std::vector<std::vector<double>> matrix(scale, vector<double>(scale));
	std::vector<double> vector(scale);
	std::vector<double> result(scale, 0.0);

	// Initialize the matrix with its rank relevant data
	for (int i = 0; i < scale; i++) {
		for (int j = 0; j < scale; j++) {
			matrix[i][j] = (i == j) ? 1.0 : 0.0;
		}
	}

	// Initialize the vector in the root process
	for (int i = 0; i < scale; i++) {
		vector[i] = i + 1.0;
	}

	// Start timing the matrix-vector multiplication
	double start_time = MPI_Wtime();

	// Calculate the local portion of the result
	for (int i = 0; i < scale; i++) {
		for (int j = 0; j < scale; j++) {
			result[i] += matrix[i][j] * vector[j];
		}
	}

	// Stop timing the matrix-vector multiplication
	double end_time = MPI_Wtime();

	cout << "Matrix-Vector Multiplication Time: " << end_time - start_time << " seconds" << endl;

	return 0;
}