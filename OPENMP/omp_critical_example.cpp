//
// Created by giuseppe on 10/03/18.
//

/*
 * Example of critical region
 */

#include <iostream>
#include <omp.h>

int main(int argc, char *argv[]) {

    int x;
    x = 0;

#pragma omp parallel shared(x)
    {
/* Even if the pool of threads is in this region only one thread will execute
 * the following statement at a time. Once it completes the next thread which
 * was blocked by the execution of the current one can execute. */
#pragma omp critical
        {
            int id = omp_get_thread_num();
            printf("Thread #%d executing in the critical region\n", id);
            x = x + 1;
        }

        int id = omp_get_thread_num();
        printf("Thread #%d exiting critical region\n", id);
        printf("\n----> next thread can execute now\n\n");

    }  /* end of parallel region */

    return 0;
}
