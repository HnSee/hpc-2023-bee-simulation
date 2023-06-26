#include <mpi.h>
#include <stdio.h>
#include <fstream>

int main(int argc, char* argv[]){
    

    MPI_Init(&argc, &argv);
    
    int world_rank, world_size;

    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int receive[world_size*2];

    int c[2] = {0, world_rank};
    
    std::cout << c[1] << "\n";

    MPI_Allgather(c, 2, MPI_INT, receive, 2 , MPI_INT, MPI_COMM_WORLD );

    int swap = 0;

    for( int k = 0; k < world_size*2; k++){
        swap += receive[k];
    }

    std::cout << "Swap: "<< swap << "\n";


    MPI_Finalize();
}