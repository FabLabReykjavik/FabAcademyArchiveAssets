#
# numbapi.py
# Neil Gershenfeld 2/6/20
# calculation of pi by a Numba sum
# pi = 3.14159265358979323846 
#

import time
from numba import jit

NPTS = 100000000
@jit(nopython=True)
def calc():
   a = 0.5
   b = 0.75
   c = 0.25
   pi = 0
   for i in range(1,(NPTS+1)):
      pi += a/((i-b)*(i-c))
   return pi
pi = calc() # first call to compile the function
start_time = time.time()
pi = calc() # second call uses the cached compilation
end_time = time.time()
mflops = NPTS*5.0/(1.0e6*(end_time-start_time))
print("NPTS = %d, pi = %f"%(NPTS,pi))
print("time = %f, estimated MFlops = %f"%(end_time-start_time,mflops))
