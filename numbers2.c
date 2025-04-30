/*
    Name: Louis Parry-Mills
    ID: a1774044
        This MPI program uses a master-worker communication model. 
    Each of five processes holds a number from a fixed array. Worker processes 
    (ranks 1–4) determine if their number is out of order (read: less than the
    number held by their left-hand neighbour) and report this to the master 
    process (rank 0).
*/

#include <stdlib.h>    /* for atoi */
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

    /* 
      DIVERT EXECUTION OF MASTER AND WORKERS
    If worker:
        send to master:
            my number
            out-of-order flag
    
    If master:
        Loop over all worker ranks (in order) and for each:
            Receive that worker's report
            If its flag is true, print the message
    */
    if (rank != 0) {
        // This process is WORKER
        // Set a flag for this worker's value is out of order
        int isOutOfOrder = (myNumber < leftNumber) ? 1 : 0;

        // Pack an array with the number, and whether it is out of order
        int info[2] = { myNumber, isOutOfOrder};

        // Send to master
        MPI_Send(info, 2, MPI_INT, 0, 1, MPI_COMM_WORLD);
    } else {
        // This process is MASTER
        int outOfOrderCount = 0; //count used in final print statement

        // Loop through sources (in order)
        for (int src = 1; src < size; ++src) {
            int info[2]; // array to store the incoming data

            // Receive from process with rank = src
            MPI_Recv(info, 2, MPI_INT, src, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
            // Note src argument is the sending process

            //If the out of order flag is set, print that that number was out of order
            if (info[1]) {
                printf("Process %d has number %d out of order.\n", src, info[0]);
                ++outOfOrderCount;
            }
        }

        // Print the total number of out of order numbers, per assignment spec
        printf("The number of processes holding an out-of-order number is %d.\n",
            outOfOrderCount);
    }


    // This process is done.
    MPI_Finalize();
    return 0;
}