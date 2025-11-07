#
# taichipi.py
# Neil Gershenfeld 2/22/23
# calculation of pi by Taichi
# pi = 3.14159265358979323846 
#
import time
import taichi as ti

NPTS = 10000000

def calc_pi(NPTS):
   pi = 0
   for i in range(1,(NPTS+1)):
      pi += 0.5/((i-0.75)*(i-0.25))
   return pi

pi = calc_pi(1000) # run first to compile
start_time = time.time()
pi = calc_pi(NPTS)
end_time = time.time()
mflops = NPTS*5.0/(1.0e6*(end_time-start_time))
print("\nNPTS = %d, pi = %f"%(NPTS,pi))
print("time = %f, estimated Python MFlops = %f\n"%(end_time-start_time,mflops))

ti.init(arch=ti.cpu,default_ip=ti.i64,default_fp=ti.f64,cpu_max_num_threads=1)

NPTS = int(1000000000)

@ti.kernel
def calc_pi_CPU(NPTS:int)->float:
   pi = float(0)
   for i in range(1,(NPTS+1)):
      pi += 0.5/((i-0.75)*(i-0.25))
   return pi

pi = calc_pi_CPU(1000) # run first to compile
start_time = time.time()
pi = calc_pi_CPU(NPTS)
end_time = time.time()
mflops = NPTS*5.0/(1.0e6*(end_time-start_time))
print("NPTS = %d, pi = %f"%(NPTS,pi))
print("time = %f, estimated Taichi CPU serial MFlops = %f\n"%(end_time-start_time,mflops))

ti.init(arch=ti.cpu,default_ip=ti.i64,default_fp=ti.f64)

@ti.kernel
def calc_pi_CPU(NPTS:int)->float:
   pi = float(0)
   for i in range(1,(NPTS+1)):
      pi += 0.5/((i-0.75)*(i-0.25))
   return pi

pi = calc_pi_CPU(1000) # run first to compile
start_time = time.time()
pi = calc_pi_CPU(NPTS)
end_time = time.time()
mflops = NPTS*5.0/(1.0e6*(end_time-start_time))
print("NPTS = %d, pi = %f"%(NPTS,pi))
print("time = %f, estimated Taichi CPU parallel MFlops = %f\n"%(end_time-start_time,mflops))

ti.init(arch=ti.gpu,default_ip=ti.i64,default_fp=ti.f64)

@ti.kernel
def calc_pi_GPU(NPTS:int)->float:
   pi = float(0)
   for i in range(1,(NPTS+1)):
      pi += 0.5/((i-0.75)*(i-0.25))
   return pi

pi = calc_pi_GPU(1000) # run first to compile
start_time = time.time()
pi = calc_pi_GPU(NPTS)
end_time = time.time()
mflops = NPTS*5.0/(1.0e6*(end_time-start_time))
print("NPTS = %d, pi = %f"%(NPTS,pi))
print("time = %f, estimated Taichi GPU MFlops = %f\n"%(end_time-start_time,mflops))
