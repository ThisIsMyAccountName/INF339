#include <stdio.h>
#include <mpi.h>

/* This program shows communication starting with processor 0
 * which sends to processor 1 and so on, until the message reaches
 * processor 0 again.
 */

int main(argc,argv) 
int argc;
char **argv;
{

  int myrank;
  int np;
  int x[2];
  MPI_Status status;
  MPI_Request req;

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  MPI_Comm_size(MPI_COMM_WORLD, &np);
  
  //printf("I am %d out of %d processors \n",myrank,np);

  if (myrank == 0) {
    x[0] = 11;
    x[1] = 12;
    MPI_Send(x,1,MPI_INT,(myrank-1+np)%np,1,MPI_COMM_WORLD);
    MPI_Recv(x,1,MPI_INT,(myrank+1)%np,1,MPI_COMM_WORLD,&status);
    printf("Processor %d got %d and %d from %d \n",myrank,x[0],x[1],(myrank+1)%np);
  }
  else {
    MPI_Recv(x,1,MPI_INT,(myrank+1)%np,1,MPI_COMM_WORLD,&status);
    printf("Processor %d got %d and %d from %d \n",myrank,x[0],x[1],(myrank+1)%np);
    MPI_Send(x,1,MPI_INT,(myrank-1+np)%np,1,MPI_COMM_WORLD);
  }

  MPI_Finalize();
  return(0);
}