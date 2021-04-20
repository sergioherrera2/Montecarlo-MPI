// required MPI include file  
#include "mpi.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[]) {
  int  numtasks, rank, len, rc; 
  int count, niter;
  double x, y, z;
  char hostname[MPI_MAX_PROCESSOR_NAME];

  // initialize MPI  
  MPI_Init(&argc,&argv);

  // get number of tasks 
  MPI_Comm_size(MPI_COMM_WORLD,&numtasks);

  // get my rank  
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);

  // this one is obvious  
  MPI_Get_processor_name(hostname, &len);
  printf ("Number of tasks= %d My rank= %d Running on %s\n", numtasks,rank,hostname);


  // do some work with message passing 
  int npoints = 10000;
  int circle_count = 0;
  int num = npoints/numtasks;
  int nPointsInCircle = 0;
  int received[numtasks];
  long recvniter[numtasks];
  int finalcount = 0;
  long finalniter = 0;
  double pi = 0.0;
  srand(time(NULL) ^ rank);

  if (rank != 0){
    for (int i = 0; i<numtasks; i++){
      x = (double)rand() / RAND_MAX;
      y = (double)rand() / RAND_MAX;
      if (pow(x,2) + pow(y,2) <= 1){
        nPointsInCircle++;
      } 
    }
    for (int i = 0; i < numtasks; i++){
      MPI_Send(&count, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
      MPI_Send(&niter, 1, MPI_LONG, 0, 2, MPI_COMM_WORLD);
    }
  } else if (rank == 0){
    for (int i = 0; i < numtasks; i++){
      MPI_Recv(&received[i], numtasks, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Recv(&recvniter[i], numtasks, MPI_LONG, MPI_ANY_SOURCE, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    for (int i = 0; i < numtasks; i++){
      finalcount += received[i];
      finalniter += recvniter[i];
    }
  }

  pi = ((double)finalcount/(double)finalniter)*4.0;
  printf("Pi: %f\n", pi);

  // done with MPI  
  MPI_Finalize();
}
