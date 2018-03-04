//
// Created by giuseppe on 04/03/18.
//
/*
 * REDUCE_TO_ALL: the difference is that any process receives also the result
 * of the reduction. The same as a combination of reduce and broadcasting
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"

int main(int *argc, char **argv)
{
    int err_code, proc_num, proc_id;
    time_t t;
    float rand_num[2], sum[2];

    sum[0] = 0.0; sum[1] = 0.0;
    rand_num[0] = 0.0; rand_num[1] = 0.0;

    err_code = MPI_Init(&argc, &argv);
    err_code = MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    err_code = MPI_Comm_rank(MPI_COMM_WORLD, &proc_id);

    int target_proc = proc_num-1;

    /* Every process (exept the root process) initializes a different random number */
    if(!(proc_id==target_proc))
    {
        /* Intializes random number generator */
        srand((unsigned) time(&t));
        rand_num[0] = (float) rand() /(float)RAND_MAX;
        rand_num[1] = rand_num[0]*2;
    }

    /* With MPI_Reduce the root process collect and sum rand_num from any process in sum
     * (aplplying the MPI_SUM operator), tested for the general case of a vector entry.
     * Notice that we have to specify the length of the buffer (e.g.3rd param = 2)
     *
     * Difference with reduce only:
     * - not necessary to give the root process since any process will than receive the sum
     * - the print of the sum in the else always give the final sum (received by all processes)
     */

    MPI_Allreduce(&rand_num, &sum, 2, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);

    if(proc_id==target_proc)
        printf("Root process #%d, sum_1=%f, sum_2=%f\n", proc_id, sum[0], sum[1]);
    else
    {
        printf("Proc #%d, rand_num1=%f rand_num2=%f\n", proc_id, rand_num[0], rand_num[1]);
        printf("sum_1=%f, sum_2=%f\n", sum[0], sum[1]);
    }

    err_code = MPI_Finalize();

}