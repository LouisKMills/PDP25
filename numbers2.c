/*
    Name: Louis Parry-Mills
    ID: a1774044
        This MPI program uses a master-worker communication model. 
    Each of five processes holds a number from a fixed array. Worker processes 
    (ranks 1–4) determine if their number is out of order (read: less than the
    number held by their left-hand neighbour) and report this to the master 
    process (rank 0).
*/

#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size; // rank: current process, size: # of processes

    // Hardcoded array of numbers for each process, each process will
    // use its rank to pull its number from the array
    int numbers[5] = {10, 20, 5, 15, 25};
    int myNumber; // this will hold the number for current process

    // This will hold the value for left process
    int leftNumber; // NOTE: will be received using MPI_Send and MPI_Recv

    // Initialize the MPI environment
    MPI_Init(&argc, &argv);
    
    // Get current rank and environment size from MPI calls
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Determine the left and right neighbors in the ring using modulo
    int left = (rank - 1 + size) % size;
    int right = (rank + 1) % size;

    myNumber = numbers[rank]; // get the number for this process

    ////// COMMUNICATION TO MASTER PROCESS HERE //////
    /*
    1. Send my number to the right neighbour

    2. Receive the number from the left neighbour
        set flag true/false indiciation 'out of order'*/

    // MPI SEND AND RECEIVE
    // Send my number to the right neighbour & receive it from my left
    MPI_Sendrecv(
        &myNumber, 1, MPI_INT, right, 0,
        &leftNumber, 1, MPI_INT, left,  0,
        MPI_COMM_WORLD, MPI_STATUS_IGNORE
    );

    /*3.
    If worker:
        send to master:
            rank ID
            my number
            out-of-order flag

    
    If master:
        Loop over all worker ranks (in order) and for each:
            Receive that worker's report
            If its flag is true, print the message
    */
   if (rank != 0) {
        /* 2. worker: decide out‐of‐order, then report to master */
        int out_of_order = (myNumber < leftNumber) ? 1 : 0;
        int info[2] = { myNumber, out_of_order };
        MPI_Send(info, 2, MPI_INT, 0, 1, MPI_COMM_WORLD);
    } else {
        /* 3. master: collect reports in rank order, print results */
        int total = 0;
        for (int src = 1; src < size; ++src) {
            int info[2];
            MPI_Recv(info, 2, MPI_INT, src, 1, MPI_COMM_WORLD, &status);
            if (info[1]) {
                printf("Process %d has number %d out of order.\n", src, info[0]);
                ++total;
            }
        }
        printf("The number of processes holding an out-of-order number is %d.\n",
            total);
    }


    // This process is done.
    MPI_Finalize();
    return 0;
}