#
# mpipi2.py
# Neil Gershenfeld 2/6/20
# calculation of pi by an MPI Numba sum
# pi = 3.14159265358979323846 
#

import time
from numba import jit
from mpi4py import MPI

NPTS = 1000000000
NLOOP = 10

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
nproc = comm.Get_size()

@jit(nopython=True)
def calc(istart,iend):
   sum = 0.0
   for i in range(istart,iend+1):
      sum += 0.5/((i-0.75)*(i-0.25))
   return sum

if (rank == 0):
   istart = 1+rank*NPTS;
   iend = (rank+1)*NPTS;
   max = 0
   for j in range(NLOOP):
      comm.Barrier()
      start_time = time.time()
      sum = 0.0;
      sum = calc(istart,iend)
      pi = comm.reduce(sum,op=MPI.SUM,root=0)
      end_time = time.time()
      mflops = nproc*NPTS*5.0/(1.0e6*(end_time-start_time))
      print("processes = %d, NPTS = %d, pi = %f"%(nproc,NPTS,pi))
      if (mflops > max):
         max = mflops
      print("time = %f, estimated MFlops = %f, max = %f"%(end_time-start_time,mflops,max))
else:
   istart = 1+rank*NPTS
   iend = (rank+1)*NPTS
   for j in range(NLOOP):
      comm.Barrier()
      sum = calc(istart,iend)
      comm.reduce(sum,op=MPI.SUM,root=0)
