/*
    Name: Louis Parry-Mills
    ID: a1774044
        This MPI program identifies out-of-order numbers in a distributed system 
    of five processes, where each process holds one number from a fixed array. 
*/

#include <stdlib.h>
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

    // Get the number for this process
    int myNumber = atoi(argv[rank + 1]); 

    // This will hold the value of left process' number
    int leftNumber;

    // MPI SEND AND RECEIVE
    // Send my number to the right neighbour & receive it from my left
    MPI_Sendrecv(
        &myNumber, 1, MPI_INT, right, 0,
        &leftNumber, 1, MPI_INT, left,  0,
        MPI_COMM_WORLD, MPI_STATUS_IGNORE
    );
    // Note: The use of MPI_Sendrecv guarantees no deadlocks.


    // Check if the number received from the left neighbour is less than my number
    // If it is, then this process has at least one number out of order
    // rank 0 is considered the "first" process in the ordering, so does not check.
    if (rank != 0) {
        if (myNumber < leftNumber) {
            printf("Process %d has at least one number out of order.\n", rank);
        }
    }

    // This process has no more execution to do.
    printf("Process %d has now finished.\n", rank); // Print statement as per assignment reqs.

    // End the process
    MPI_Finalize();
    return 0;
}