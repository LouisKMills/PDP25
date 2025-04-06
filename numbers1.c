#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size; // rank: current process, size: # of processes

    // Initialize the MPI environment
    MPI_Init(&argc, &argv);
    
    // Get current rank and environment size from MPI calls
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Determine the left and right neighbors in the ring using modulo
    int left = (rank - 1 + size) % size;
    int right = (rank + 1) % size;

    // Prints a message specifying its rank and its neighbours
    printf("my left neighbour is %d, I am %d, and my right neighbour is %d\n", left, rank, right);

    // Finalize, i.e. clean up MPI env
    MPI_Finalize();
    return 0;
}
