#include "mpi.h"
#include <stdio.h>

typedef struct 
{
    char a;
    int b;
    short c;
} my_struct;


int main (int argc, char *argv[])
{
    int  numtasks, taskid;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);


    if (taskid == 0)     MPI_Comm_size(MPI_COMM_WORLD, &numtasks);


    if (taskid == 0) 
    {
        my_struct m;
        m.a = '!';
        m.b = 1234;
        m.c = 5678;

        MPI_Send(&m, sizeof(my_struct), MPI_CHAR, 1, 0, MPI_COMM_WORLD);
    }
    else 
    {
        my_struct m;
        MPI_Recv(&m, sizeof(my_struct), MPI_CHAR, 0, 0, MPI_COMM_WORLD, 
                 MPI_STATUS_IGNORE);
        printf("%c %d %d\n", m.a, m.b, m.c); 
    }

    MPI_Finalize();
}