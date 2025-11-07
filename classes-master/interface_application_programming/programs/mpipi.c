/*
* mpipi.c
* Neil Gershenfeld  2/5/11
* use MPI to evaluate pi by summation
*/

#include <stdio.h>
#include <mpi.h>
#include <sys/time.h>

#define NPTS 100000000
#define NLOOP 10

void main(int argc, char** argv) {
   int rank,nproc,i,istart,iend,loop,N;
   unsigned long int start_time,end_time;
   struct timeval start,end;
   double sum,pi,mflops;

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &nproc);
   if (rank == 0) {
      gettimeofday(&start, NULL);
      istart = 1 + NPTS*((rank+0.0)/nproc);
      iend = NPTS*((rank+1.0)/nproc);
      sum = 0.0;
      for (loop = 0; loop < NLOOP; ++loop)
         for (i = istart; i <= iend; ++i)
            sum += 0.5/((i-0.75)*(i-0.25));
      MPI_Reduce(&sum,&pi,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
      gettimeofday(&end, NULL);
      start_time = start.tv_sec * 1e6 + start.tv_usec;
      end_time = end.tv_sec * 1e6 + end.tv_usec;
      mflops = NLOOP*(NPTS*(5.0/(end_time-start_time)));
      printf("processes = %d, NPTS = %d, NLOOP = %d, pi = %f\n",nproc,NPTS,NLOOP,pi/NLOOP);
      printf("time = %f, estimated MFlops = %f\n",(end_time-start_time)/1.0e6,mflops);
      }
   else {
      istart = 1 + NPTS*((rank+0.0)/nproc);
      iend = NPTS*((rank+1.0)/nproc);
      sum = 0.0;
      for (loop = 0; loop < NLOOP; ++loop)
         for (i = istart; i <= iend; ++i)
            sum += 0.5/((i-0.75)*(i-0.25));
      MPI_Reduce(&sum,&pi,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
      }
   MPI_Finalize();
   }
