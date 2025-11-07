cimport cython
# run python setup.py build_ext --inplace to compile

@cython.wraparound(False)
@cython.boundscheck(False)
@cython.nonecheck(False)
def compute_pi( NPTS):
	cdef int i = 0;
	cdef double result = 0;
	for i in range(NPTS):
		result += 0.5/((i+.25)*(i+.75))
	return result;
