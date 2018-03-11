/*
 * Example of usage of the omp for directive
 */

#include <omp.h>
#include <iostream> // printf leaves here
#define N 1000
#define CHUNKSIZE 100

main(int argc, char *argv[]) {

    int i, chunk;
    float a[N], b[N], c[N];

    /* Some initializations */
    for (i=0; i < N; i++)
        a[i] = b[i] = i * 1.0;
    chunk = CHUNKSIZE;

#pragma omp parallel shared(a,b,c,chunk) private(i)
    {
        /* Obtain and print thread id */
        int tid = omp_get_thread_num();
        printf("Starting thread # %d\n", tid);

        #pragma omp for schedule(dynamic,chunk) nowait
        for (i=0; i < N; i++)
            c[i] = a[i] + b[i];

    }   /* end of parallel region */

}
