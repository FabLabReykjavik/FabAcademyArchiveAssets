/*
* pi.c
* Neil Gershenfeld 2/6/11
* calculation of pi by a scalar sum
* pi = 3.14159265358979323846
*/

#include <stdio.h>
#include <sys/time.h>

#define NPTS 1000000000

void main() {
   int i;
   double a,b,c,pi,mflops;
   unsigned long int start_time,end_time;
   struct timeval start,end;
   a = 0.5;
   b = 0.75;
   c = 0.25;
   pi = 0;
   pi = 0;
   gettimeofday(&start, NULL);
   for (i = 1; i <= NPTS; ++i)
      pi += a/((i-b)*(i-c));
   gettimeofday(&end, NULL);
   start_time = start.tv_sec * 1e6 + start.tv_usec;
   end_time = end.tv_sec * 1e6 + end.tv_usec;
   mflops = NPTS*5.0/(end_time-start_time);
   printf("NPTS = %d, pi = %f\n",NPTS,pi);
   printf("time = %f, estimated MFlops = %f\n",(end_time-start_time)/1.0e6,mflops);
   }
