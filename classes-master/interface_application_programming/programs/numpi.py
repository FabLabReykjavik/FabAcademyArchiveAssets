#
# numpi.py
# Neil Gershenfeld 1/23/17
# calculation of pi by a numpy sum
# pi = 3.14159265358979323846 
#

from numpy import *
import time

NPTS = 10000000
a = 0.5
b = 0.75
c = 0.25
start_time = time.time()
i = arange(1,(NPTS+1),dtype=float64)
pi = sum(0.5/((i-0.75)*(i-.25)))
end_time = time.time()
mflops = NPTS*5.0/(1.0e6*(end_time-start_time))
print("NPTS = %d, pi = %f"%(NPTS,pi))
print("time = %f, estimated MFlops = %f"%(end_time-start_time,mflops))
