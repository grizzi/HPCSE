//
// Created by giuseppe on 04/03/18.
//

#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"

/*
 * Example for the GATHER collective
 */

int main(int *argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int err_code, proc_num, proc_id;
    int sendarray[10];   // any process will send 10 elements on this array
    int root, *rbuf;     // root process and array where the root process will gather data from processes
                         // we cannot know its size in advance, we first need to know the available processes

    MPI_Comm_rank(MPI_COMM_WORLD, &proc_id);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);

    /* Allocate the memory only if we are the root process
     * Since we are in the SPMD also the non root processes will execute the same command
     * which is MPI_Gather(......) but they will not use the param rbuf
     */
    root = proc_num -1;
    if ( proc_id == root)
        rbuf = (int *)malloc(proc_num*10*sizeof(int));   //allocate a buffer of enough memory

    /* INTERPRETATION:
     * the root will receive 10 int from any process and any process will send 10 int
     * MPI_Scatter has the same syntax but means any process will receive 10 int from the buffer
     * and the buffer will give 10 int to any process
     * MPI_Scatter( sendarray, 10, MPI_INT, rbuf, 10, MPI_INT, root, MPI_COMM_WORLD);
     */
    MPI_Gather( sendarray, 10, MPI_INT, rbuf, 10, MPI_INT, root, MPI_COMM_WORLD);
    MPI_Finalize();
}
