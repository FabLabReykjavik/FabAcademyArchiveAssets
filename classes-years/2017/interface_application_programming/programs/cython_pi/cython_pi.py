# cython_pi.py
# Sam Calisch 1/24/2017
# calculation of pi by summing in cython
# pi = 3.14159265358979323846 
#
from numpy import *
import time
from cython_pi import compute_pi

NPTS = 1000000000
start_time = time.time()
pi_computed = compute_pi( NPTS )
end_time = time.time()

mflops = NPTS*5.0/(1.0e6*(end_time-start_time))
print "NPTS = %d, pi = %12f"%(NPTS,pi_computed)
print "time = %f, estimated MFlops = %f"%(end_time-start_time,mflops)