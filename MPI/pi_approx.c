//
// Created by giuseppe on 10/03/18.
//

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

#define MASTER 0

int sample_points(long num_points)
{
    int count = 0;

    for(int i=0; i<num_points; i++)
    {
        double x_coordinate = (double)rand()/RAND_MAX;
        double y_coordinate = (double)rand()/RAND_MAX;

        // Check if the sampled point is inside the circle
        double radius = (x_coordinate*x_coordinate) + (y_coordinate*y_coordinate);

    //    printf("x = %f , y = %f, Radius = %f\n", x_coordinate, y_coordinate, radius);
        if(radius <= 1)
            count +=1;
    }

    return count;
}

int main(int argc, char **argv)
{
    int proc_id, proc_num, err_code;


    err_code = MPI_Init(&argc, &argv);
    err_code = MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    err_code = MPI_Comm_rank(MPI_COMM_WORLD, &proc_id);

    // My vars
    int global_count = 0;
    int local_count = 0;
    int local_points, num_points = 0;

    // Parse the input
    if(argc==1 && proc_id==MASTER)
    {
        printf("Input the number of points\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    else{
        num_points = atoi(argv[1]);
        local_points = num_points/proc_num;

        // Add remaining points to the last process if do not evenly divisible
        if(proc_id == proc_num-1)
            local_points += num_points%proc_num;

    }

    printf("#%d sampling %d points.\n", proc_id, local_points);
    local_count = sample_points(local_points);
    printf("#%d sampled %d points.\n", proc_id, local_count);

    // Make sure any process completed its task
    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Reduce(&local_count, &global_count, 1, MPI_INT, MPI_SUM, MASTER, MPI_COMM_WORLD);
    if(proc_id == MASTER)
    {
        printf("Summing points from other processes");
        double pi_approximation = 4 * (double)global_count/num_points;
        printf("The computed approximation of pi is %d/%d = %f\n", global_count, num_points, pi_approximation);
        printf("Real pi= %f, approximated one = %f, error = %f\n", M_PI, pi_approximation, M_PI-pi_approximation);
    }

    MPI_Finalize();
}