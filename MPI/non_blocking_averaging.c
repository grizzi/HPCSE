//
// Created by giuseppe on 05/03/18.
//

#include <stdio.h>
#include <stdlib.h>
#include"mpi.h"
#include <stdbool.h>


/*
 * Example on the use of the blocking operations MPI_SEND and MPI_RECV
 * through ping pong message passing between 2 specific processes
 */
int main(int *argc, char **argv) {

    int err_code, proc_num, proc_id;

    err_code = MPI_Init(&argc, &argv);
    err_code = MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    err_code = MPI_Comm_rank(MPI_COMM_WORLD, &proc_id);

    // any process has a different value x
    float x = proc_id*2 + 1.0;
    float x_prev, x_succ;
    printf("#%d Before message passing - (x=%f, x_prev=%f, x_succ=%f)\n", proc_id, x, x_prev, x_succ);

    // any process send its value to the predecessor and successor one as well as receive
    // the 2 values from the predecessor and successor
    int predecessor, successor;
    predecessor = proc_id - 1;
    successor   = proc_id + 1;

    if(proc_id==0)
        predecessor = MPI_PROC_NULL;    // send to null process aka do not send/receive
    if(proc_id == proc_num -1)
        successor = MPI_PROC_NULL;

    // Need to define a vector of requests (in this case 4 : 2 sent and 2 receives)
    MPI_Request requests_array[4];
    MPI_Status status_array[4];

    // Send to the predecessor and successor
    MPI_Isend(&x, 1, MPI_FLOAT, predecessor, 0, MPI_COMM_WORLD, &requests_array[0]);
    MPI_Isend(&x, 1, MPI_FLOAT, successor, 0, MPI_COMM_WORLD, &requests_array[1]);

    /* Notice that the receive function loses the status entry - at the completion
     * of the receiving the data has not been received (non-blocking function) and thus we cannot
     * store any information about the received object in a status variable
     */
    MPI_Irecv(&x_prev, 1, MPI_FLOAT, predecessor, 0, MPI_COMM_WORLD, &requests_array[2]);
    MPI_Irecv(&x_succ, 1, MPI_FLOAT, successor, 0, MPI_COMM_WORLD, &requests_array[3]);

    /* Before performing averaging on any process we need for all the send/receive
     * requests to complete, using the blocking operations Waitall
     */
    MPI_Waitall(4, requests_array, status_array);

    // Check that we received the correct values
    printf("#%d After message passing  - (x=%f, x_prev=%f, x_succ=%f)\n", proc_id, x, x_prev, x_succ);

    float average;
    average = (float)(x + x_prev + x_succ)/3.0;
    printf("#%d average=%f\n", proc_id, average);

    MPI_Finalize();
}
