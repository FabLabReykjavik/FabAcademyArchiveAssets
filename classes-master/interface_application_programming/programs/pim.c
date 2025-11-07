/*
* pim.c
* Neil Gershenfeld 2/10/08
* calculation of pi by an array sum
* pi = 3.14159265358979323846 
*/

#include <stdio.h>
#include <sys/time.h>

#define NPTS 1000000
#define NLOOP 100

void main() {
   int i,loop;
   double a,b,c,pi[NPTS],mflops;
   unsigned long int start_time,end_time;
   struct timeval start,end;
   a = 0.5;
   b = 0.75;
   c = 0.25;
   gettimeofday(&start, NULL);
   for (loop = 0; loop < NLOOP; ++loop) {
      pi[0] = 0;
      for (i = 1; i <= NPTS; ++i)
         pi[i] = pi[i-1] + a/((i-b)*(i-c));
      }
   gettimeofday(&end, NULL);
   start_time = start.tv_sec * 1e6 + start.tv_usec;
   end_time = end.tv_sec * 1e6 + end.tv_usec;
   mflops = NPTS*NLOOP*5.0/(end_time-start_time);
   printf("NPTS = %d, NLOOP = %d, pi = %f\n",NPTS,NLOOP,pi[NPTS-1]);
   printf("time = %f, estimated MFlops = %f\n",(end_time-start_time)/1.0e6,mflops);
   }
