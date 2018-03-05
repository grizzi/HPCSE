//
// Created by giuseppe on 05/03/18.
//

#include <stdio.h>
#include <stdlib.h>
#include"mpi.h"
#include <stdbool.h>


/*
 * Example on the use of the blocking operations MPI_SEND and MPI_RECV
 * through ping pong message passing between 2 specific processes
 */
int main(int *argc, char **argv)
{
    int number_pings;
    int err_code, proc_num, proc_id;
    int sender, receiver;

    err_code = MPI_Init(&argc, &argv);
    if(argc > 1)
        number_pings = atoi(argv[1]);
    else
        number_pings = 1;

    err_code = MPI_Comm_rank(MPI_COMM_WORLD, &proc_id);
    err_code = MPI_Comm_size(MPI_COMM_WORLD, &proc_num);

    sender   = 0;
    receiver = 1;

    if((proc_id == sender) || (proc_id == receiver))
    {
        int send_buffer;
        int recv_buffer = 0;
        double start = 0.0, end = 0.0;
        int flag_1 = 0, flag_2 = 0;       // flag to check if both processes are entered

        if(proc_id==sender)
            send_buffer = 2;
        else if(proc_id==receiver)
            send_buffer = 1;

        for(int i=0; i<number_pings; i++)
        {
            start = MPI_Wtime();
            printf("Starting time of ping-pong: %f s\n", start);
            if(proc_id == sender)
            {
                /*
                 *  MPI allows senders and receivers to also specify message IDs with
                 *  the message (known as tags). When process B only
                 *  requests a message with a certain tag number, messages with
                 *  different tags will be buffered by the network until B is ready for them.
                 */
                printf("#%d : I will send something, send_buffer=%d, receive_buffer=%d\n", proc_id, send_buffer, recv_buffer);
                MPI_Send(&send_buffer, 1, MPI_INT, receiver, 0,MPI_COMM_WORLD);
                printf("#%d : I sent somothing, send_buffer=%d, receive_buffer=%d\n", proc_id, send_buffer, recv_buffer);
                MPI_Recv(&recv_buffer, 1, MPI_INT, receiver, 0,MPI_COMM_WORLD, MPI_STATUS_IGNORE);  // receiver works as a sender for sender
                printf("#%d : I received something, send_buffer=%d, receive_buffer=%d\n", proc_id, send_buffer, recv_buffer);
                flag_1 = true;

            }
            else if(proc_id == receiver)
            {
                printf("#%d : I will receive something, send_buffer=%d, receive_buffer=%d\n", proc_id, send_buffer, recv_buffer);
                MPI_Recv(&recv_buffer, 1, MPI_INT, sender, 0,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                printf("#%d : I received something, send_buffer=%d, receive_buffer=%d\n", proc_id, send_buffer, recv_buffer);

                // change the value of the send buffer to send back something different to first process
                send_buffer = 3;
                MPI_Send(&send_buffer, 1, MPI_INT, sender, 0,MPI_COMM_WORLD);
                printf("#%d : I sent something, send_buffer=%d, receive_buffer=%d\n", proc_id, send_buffer, recv_buffer);
                flag_2 = true;
            }

            // Ask for current time
            end = MPI_Wtime();

            // Print end time only when visited both processes
            if((flag_1 == true) && (flag_2 == true))
            {
                printf("Ending time of ping-pong: %f s\n\n",end);
                // Reset the flag to false
                flag_1 = false;
                flag_2 = false;
            }
        }
    }

    MPI_Finalize();
}