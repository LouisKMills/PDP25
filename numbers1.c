#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size; // rank: current process, size: # of processes

    // hardcoded array of numbers for each process, each process will
    // use its rank to pull its number from the array
    int numbers[10] = {10, 20, 5, 15, 25};
    int myNumber; // this will hold the number for current process

    // this will hold the number for left process
    int leftNumber; // NOTE: left number will be received using MPI_Send and MPI_Recv

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

    myNumber = numbers[rank]; // get the number for this process
    printf("I am %d and my number is %d\n", rank, myNumber);

    // MPI SEND AND RECEIVE
    // Send my number to the right neighbour
    MPI_Send(myNumber, 1, MPI_INT, right, 0, MPI_COMM_WORLD);

    // Receive the number from the left neighbour
    MPI_Recv(&leftNumber, 1, MPI_INT, left, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    printf("I am %d and I received %d from my left neighbour %d\n", rank, leftNumber, left);

    // Finalize, i.e. clean up MPI env
    MPI_Finalize();
    return 0;
}