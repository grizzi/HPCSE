//
// Created by giuseppe on 04/03/18.
//

#include <stdlib.h>
#include "mpi.h"

/*
 * Example for the GATHER_V collective
 */

int main(int *argc, char **argv) {

    MPI_Init(&argc, &argv);
    int err_code, proc_num, proc_id;
    int root, *rbuf;     // root process and array where the root process will gather data from processes
    // we cannot know its size in advance, we first need to know the available processes

    MPI_Comm_rank(MPI_COMM_WORLD, &proc_id);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);

    root = proc_num -1;
    // Each process has an array "localdata of size "localsize"
    int localdata[proc_id*2];
    int localsize = proc_id*2;

    // the root process decides how much data will be coming:
    // allocate arrays to contain size and offset information
    int *localsizes;   // local sizes array
    int *offsets;      // offsets array
    int *alldata;      // total buffer array

    if (proc_id == root) {
        localsizes = (int *) malloc((proc_num + 1) * sizeof(int));
        offsets = (int *) malloc(proc_num * sizeof(int));            // n_proc offsets
    }

    // use a first gather to get the information about any local chunk
    // everyone contributes their info
    MPI_Gather(&localsize, 1, MPI_INT,
               localsizes, 1, MPI_INT, root, MPI_COMM_WORLD);

    // the root constructs the offsets array
    if (proc_id == root) {
        offsets[0] = 0;
        for (int i = 0; i < proc_num; i++)
            offsets[i + 1] = offsets[i] + localsizes[i];
        // Now we know what is the size of the buffer for collecting info from processes
        alldata = (int *) malloc(offsets[proc_num] * sizeof(int));
    }
    // everyone contributes with their data
    MPI_Gatherv(localdata, localsize, MPI_INT,
                alldata, localsizes /* length of any chunk */, offsets /* initial index of any chunk*/,
                MPI_INT, root, MPI_COMM_WORLD);

    MPI_Finalize();
}