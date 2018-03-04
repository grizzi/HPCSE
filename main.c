//
// Created by giuseppe on 04/03/18.
//make
#include<stdio.h>
#include<mpi.h>

int main(int argc, char** argv)
{
    int err_code, proc_id, num_processes;

    printf("Before MPI_Init\n");

    err_code = MPI_Init(&argc, &argv);

    /* The group of MPI_COMM_WORLD is the set of all MPI processes.
     * It is defined afer calling MPI_Init
     */

    err_code = MPI_Comm_rank(MPI_COMM_WORLD, &proc_id);
    err_code = MPI_Comm_size(MPI_COMM_WORLD, &num_processes);

    printf("Process: %d out of %d processes:  ", proc_id, num_processes);
    printf(" Hello world\n");

    err_code = MPI_Finalize();

    printf("After MPI_Finalize\n");
    return 0;
}

