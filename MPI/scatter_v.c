//
// Created by giuseppe on 04/03/18.
//

#include <stdlib.h>
#include "mpi.h"

int main(int *argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int proc_num,*sendbuf;
    int root, rbuf[10], i, *displs, *scounts;

    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    /* The buffer is bigger then the actual data sent to any process (10 elements)
     * so I can specify where to start selecting the chunk form the send buffer and the length of the chunk
     */
    int stride = 12; // > 10
    sendbuf = (int *)malloc(proc_num*stride*sizeof(int));
    displs = (int *)malloc(proc_num*sizeof(int));    // array of offsets (beginning of any chunk in sendbuf)
    scounts = (int *)malloc(proc_num*sizeof(int));   // length of any chunk (could be variable but it is constant in this example)
    for (i=0; i<proc_num; ++i)
    {
        displs[i] = i*stride;
        scounts[i] = 6;
    }
    MPI_Scatterv( sendbuf, scounts, displs, MPI_INT, rbuf, 10, MPI_INT,
    root, MPI_COMM_WORLD);
    MPI_Finalize();

}
