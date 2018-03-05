//
// Created by giuseppe on 04/03/18.
//
#include <stdio.h>
#include "mpi.h"

/*
 * Example of including and excluding SCAN
 */

int main(int *argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int ex_sum=0,in_sum=0, local_var;
    int proc_num, proc_id, err_code;

    err_code = MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    err_code = MPI_Comm_rank(MPI_COMM_WORLD, &proc_id);

    local_var = proc_id*2;
    err_code = MPI_Exscan(&local_var,&ex_sum,
                     1,MPI_INT,MPI_SUM, MPI_COMM_WORLD);
    err_code = MPI_Scan(&local_var,&in_sum,
                          1,MPI_INT,MPI_SUM, MPI_COMM_WORLD);
    printf("#%d - local_var=%d - ex_sum=%d - in_sum=%d\n", proc_id, local_var, ex_sum, in_sum);
    MPI_Finalize();
}
