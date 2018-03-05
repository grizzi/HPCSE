//
// Created by giuseppe on 04/03/18.
//

#include <stdio.h>
#include <stdbool.h>
#include "mpi.h"

/*
 * Example Compute the product of an array of complex numbers, in C
 * through the definition of a custom mpi operator
 */


typedef struct {
    double real, imag;
} Complex;

/* the user-defined function
 * Syntax:
 * typedef void MPI_User_function( void *invec, void *inoutvec, int *len, MPI_Datatype *datatype)
 * where.
 * - *invec : address of the input vector
 * - *inoutvec : address of the secon input vector - overwritten with corresponig output vector
 * - *len : address of the element where the length of the operands is stored (basically the length of invec)
 * - *datatype : MPI datatype, not used in custom operator
 */
void myProd( Complex *in, Complex *inout, int *len, MPI_Datatype *dptr )
{
    int i;
    Complex c;

    for (i=0; i< *len; ++i) {
        c.real = inout->real*in->real -
                 inout->imag*in->imag;
        c.imag = inout->real*in->imag +
                 inout->imag*in->real;
        *inout = c;
        in++; inout++;
    }
}

int main(int *argc, char **argv) {

    int err_code, proc_num, proc_id;

    MPI_Init(&argc, &argv);
    err_code = MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    err_code = MPI_Comm_rank(MPI_COMM_WORLD, &proc_id);

    int root = proc_num-1;

    /* each process has a complex (it could be a array)
     */
    Complex a, answer;
    MPI_Op myOp;
    MPI_Datatype ctype;

    a.imag = (float)(proc_id-1);
    a.real = 0.5*(float)proc_id;

    /* explain to MPI how type Complex is defined
     *
     * int MPI_Type_contiguous(int count, MPI_Datatype oldtype, MPI_Datatype *newtype)
     *
     * The simplest datatype constructor is MPI_Type_contiguous,
     * which allows replication of a datatype into contiguous locations.
     * newtype is the datatype obtained by concatenating count copies of oldtype.
     * Concatenation is defined using the extent of oldtype as the size of the concatenated copies.
     */
    MPI_Type_contiguous(2, MPI_DOUBLE, &ctype);

    /*
     * The commit operation commits the datatype, that is, the formal description of
     * a communication buffer, not the content of that buffer.
     * Thus, after a datatype has been committed, it can be repeatedly
     * reused to communicate the changing content of a buffer or, indeed,
     * the content of different buffers, with different starting addresses.
     */
    MPI_Type_commit(&ctype);

    /* create the complex-product user-op
     * second parameter state if the commutative property holds (true or false)
     */
    MPI_Op_create(myProd, true, &myOp);
    MPI_Reduce(&a, &answer, 1, ctype, myOp, root, MPI_COMM_WORLD);

    printf("#%d - (%f) +j(%f)\n", proc_id, a.real, a.imag);
    if(proc_id==root)
        printf("#%d - The product of the complex numbers is: (%f) +j(%f)\n", proc_id, answer.real, answer.imag);

    MPI_Finalize();

}