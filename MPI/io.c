//
// Created by giuseppe on 06/03/18.
//


/*
 * Example of parsing a file dividing the load on multiple processes
 * It makes use of some MPI reading, parsing routines
 */
#include <stdio.h>   /* all IO stuff lives here */
#include <stdlib.h>  /* exit lives here */
#include <unistd.h>  /* getopt lives here */
#include <string.h>  /* strcpy lives here */
#include <limits.h>  /* INT_MAX lives here */
#include <mpi.h>     /* MPI and MPI-IO live here */

#define DEBUG
#define ROOT_RANK 0
#define TRUE 1
#define FALSE 0
#define BOOLEAN int
#define MBYTE 1048576
#define SYNOPSIS printf ("synopsis: %s -f <file>\n", argv[0])

int main(int *argc, char **argv)
{
    /* my variables */
    int proc_id, proc_num, last_proc, count;
    BOOLEAN root = FALSE, input_error = FALSE;
    char *filename = NULL, *read_buffer;
    int filename_length;
    int file_open_error, number_of_bytes;

    /* MPI_Offset is long long
     * MPI_Offset is an integer type of size sufficient to represent the size (in bytes)
     * of the largest file supported by MPI. Displacements and offsets are always specified
     * as values of type MPI_Offset.
     */
    MPI_Offset my_offset, total_number_of_bytes, number_of_bytes_ll, max_number_of_bytes_ll;

    // object handle for the file to open
    MPI_File fh;
    MPI_Status status;
    double start, finish, io_time, longest_io_time;

    /* getopt variables */
    extern char *optarg;
    int c;

    /* ACTION */

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_id);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    last_proc = proc_num - 1;

    if (proc_id == ROOT_RANK)
        root = TRUE;

    if (root) {

        /* read the command line */
        // All options requiring a argument are followed by a semicolon in getopt
        while ((c = getopt(argc, argv, "f:h")) != EOF)
            switch(c) {
                case 'f':
                    filename = optarg;
                    #ifdef DEBUG
                        printf("input file: %s\n", filename);
                    #endif
                    break;
                case 'h':
                    SYNOPSIS;
                    input_error = TRUE;
                    break;
                case '?':
                    SYNOPSIS;
                    input_error = TRUE;
                    break;
            } /* end of switch(c) */

        /* Check if the command line has initialized filename and
         * number_of_blocks.
         */
        if (filename == NULL)
        {
            SYNOPSIS;
            input_error = TRUE;
        }

        if (input_error)
            MPI_Abort(MPI_COMM_WORLD, 1);

        filename_length = strlen(filename) + 1;
    } /* end of "if (root)"; reading the command line */

    /* If we got this far, the data read from the command line
       should be OK. */

    MPI_Bcast(&filename_length, 1, MPI_INT, ROOT_RANK, MPI_COMM_WORLD);

    /* Allocate a buffer to containt the name of the file and send it
    /* with broadcast to any process
    */
    if (! root)
        filename = (char*) malloc(filename_length);
    #ifdef DEBUG
        printf("%3d: allocated space for filename\n", proc_id);
    #endif
    MPI_Bcast(filename, filename_length, MPI_CHAR, ROOT_RANK, MPI_COMM_WORLD);
    #ifdef DEBUG
        printf("%3d: received broadcast\n", proc_id);
        printf("%3d: filename = %s\n", proc_id, filename);
    #endif

    /* MPI_Barrier
     * Blocks until all processes in the communicator have reached this routine.
     */
    MPI_Barrier(MPI_COMM_WORLD);

    /* Default I/O error handling is MPI_ERRORS_RETURN */
    file_open_error = MPI_File_open(MPI_COMM_WORLD, filename,
                                    MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);

    if (file_open_error != MPI_SUCCESS)
    {
        char error_string[BUFSIZ];
        int length_of_error_string, error_class;

        MPI_Error_class(file_open_error, &error_class);
        MPI_Error_string(error_class, error_string, &length_of_error_string);
        printf("%3d: %s\n", proc_id, error_string);

        MPI_Error_string(file_open_error, error_string, &length_of_error_string);
        printf("%3d: %s\n", proc_id, error_string);

        MPI_Abort(MPI_COMM_WORLD, file_open_error);
    }

    MPI_File_get_size(fh, &total_number_of_bytes);
    #ifdef DEBUG
        printf("%3d: total_number_of_bytes = %lld\n", proc_id, total_number_of_bytes);
    #endif

    number_of_bytes_ll = total_number_of_bytes / proc_num;

    /* If proc_num does not divide total_number_of_bytes evenly,
       the last process will have to read more data, i.e., to the
       end of the file. */
    max_number_of_bytes_ll =
            number_of_bytes_ll + total_number_of_bytes % proc_num;

    if (max_number_of_bytes_ll < INT_MAX) {

        if (proc_id == last_proc)
            number_of_bytes = (int) max_number_of_bytes_ll;
        else
            number_of_bytes = (int) number_of_bytes_ll;

        read_buffer = (char*) malloc(number_of_bytes);
        #ifdef DEBUG
            printf("%3d: allocated %d bytes\n", proc_id, number_of_bytes);
        #endif

        /* Allocate a pointer for storing the global information of where any process
         * has to start (from which byte) to read the file
         */
        my_offset = (MPI_Offset) proc_id * number_of_bytes_ll;
        #ifdef DEBUG
            printf("%3d: my offset = %lld\n", proc_id, my_offset);
        #endif

        /*int MPI_File_seek(MPI_File mpi_fh,MPI_Offset offset,int whence);
         *
         * MPI_FILE_SEEK updates the individual file pointer according to whence, which has the following possible values:
         * MPI_SEEK_SET: the pointer is set to offset
         * MPI_SEEK_CUR: the pointer is set to the current pointer position plus offset
         * MPI_SEEK_END: the pointer is set to the end of file plus offset
         */
        MPI_File_seek(fh, my_offset, MPI_SEEK_SET);
        MPI_Barrier(MPI_COMM_WORLD);


        start = MPI_Wtime();
        MPI_File_read(fh, read_buffer, number_of_bytes, MPI_BYTE, &status);
        finish = MPI_Wtime();
        MPI_Get_count(&status, MPI_BYTE, &count);
        #ifdef DEBUG
            printf("%3d: read %d bytes\n", proc_id, count);
        #endif

        /* MPI_File_get_position:
         * return the offset of individual file pointer (nonnegative integer)
         */
        MPI_File_get_position(fh, &my_offset);
        #ifdef DEBUG
            printf("%3d: my offset = %lld\n", proc_id, my_offset);
        #endif

        io_time = finish - start;

        // Get the maximum processing time
        MPI_Allreduce(&io_time, &longest_io_time, 1, MPI_DOUBLE, MPI_MAX,
                      MPI_COMM_WORLD);
        if (root) {
            printf("longest_io_time       = %f seconds\n", longest_io_time);
            printf("total_number_of_bytes = %lld\n", total_number_of_bytes);
            printf("transfer rate         = %f MB/s\n",
                   total_number_of_bytes / longest_io_time / MBYTE);
        }
    }
    else {
        if (root) {
            printf("Not enough memory to read the file.\n");
            printf("Consider running on more nodes.\n");
        }
    } /* of if(max_number_of_bytes_ll < INT_MAX) */

    MPI_File_close(&fh);
    MPI_Finalize();

    exit(0);
}

