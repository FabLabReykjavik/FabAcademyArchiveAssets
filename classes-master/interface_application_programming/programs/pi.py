#
# pi.py
# Neil Gershenfeld 1/23/17
# calculation of pi by a scalar sum
# pi = 3.14159265358979323846 
#

import time

NPTS = 10000000
a = 0.5
b = 0.75
c = 0.25
pi = 0
start_time = time.time()
for i in range(1,(NPTS+1)):
   pi += a/((i-b)*(i-c))
end_time = time.time()
mflops = NPTS*5.0/(1.0e6*(end_time-start_time))
print("NPTS = %d, pi = %f"%(NPTS,pi))
print("time = %f, estimated MFlops = %f"%(end_time-start_time,mflops))

