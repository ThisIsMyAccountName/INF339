#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2) {
        fprintf(stderr, "This program requires exactly 2 processes.\n");
        MPI_Finalize();
        return 1;
    }

    const int max_message_size = 64 * 1024 * 32;  // 64 MB
    int increment = 128;  // 1 MB increment

    if (rank == 0) {
        for (int msg_size = increment; msg_size <= max_message_size; msg_size += increment) {

            // Allocate memory for the message
            char *message = (char *)malloc(msg_size);
            if (message == NULL) {
                fprintf(stderr, "Memory allocation failed.\n");
                MPI_Abort(MPI_COMM_WORLD, 1);
            }
            double start_time = MPI_Wtime();

            // Send the message
            MPI_Send(message, msg_size, MPI_CHAR, 1, 0, MPI_COMM_WORLD);

            // Calculate and print the time taken
            double end_time = MPI_Wtime();
            printf("Sent a %d-byte message in %lf seconds\n", msg_size, end_time - start_time);

            free(message);
        }
    } else if (rank == 1) {
        for (int msg_size = increment; msg_size <= max_message_size; msg_size += increment) {
            // Receive the message
            char *message = (char *)malloc(msg_size);
            MPI_Recv(message, msg_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            free(message);
        }
    }

    MPI_Finalize();
    return 0;
}
