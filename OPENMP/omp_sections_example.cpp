//
// Example of use of the sections directive
//

#include <omp.h>
#include <iostream>
#define N 1000

main(int argc, char *argv[]) {

    int i;
    float a[N], b[N], c[N], d[N];

    /* Some initializations */
    for (i=0; i < N; i++) {
        a[i] = i * 1.5;
        b[i] = i + 22.35;
    }

#pragma omp parallel shared(a,b,c,d) private(i)
    {

#pragma omp sections nowait
        {

    // One thread will be assigned this section
    #pragma omp section
            {
                int id = omp_get_thread_num();
                printf("Thread #%d initializing vector c[].\n", id);
                for (i=0; i < N; i++)
                    c[i] = a[i] + b[i];
            }

    // A second thread will be assigned this section
    // It could also theoretically be the previous one if it completed its task yet
    #pragma omp section
            {
                int id = omp_get_thread_num();
                printf("Thread #%d initializing vector d[].\n", id);
                for (i=0; i < N; i++)
                    d[i] = a[i] * b[i];
            }
        }  /* end of sections */
    }  /* end of parallel region */
}