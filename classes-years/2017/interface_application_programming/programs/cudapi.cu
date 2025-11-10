/*
* cudapi.cu
* Thras Karydis  2/4/2017
* use CUDA to evaluate pi by summation
*/

#include <stdio.h>
#include <cuda.h>
#include <sys/time.h>

#define NLOOP 1
#define NPTS 1000000000  // Number of points (one billion)
#define NUM_THREAD  256  // Number of threads per block (max is 1024)
#define NUM_BLOCK  (NPTS + NUM_THREAD - 1) / NUM_THREAD // Number of blocks per grid

int tid;
double pi = 0;

// Kernel that executes on the GPU
__global__ void compute_pi(float *sum) {

        unsigned int idx = blockIdx.x * blockDim.x + threadIdx.x;  // Sequential thread index across the blocks
        if (idx < NPTS)
        sum[idx] = 0.5/((idx-0.75)*(idx-0.25));
}

// Main routine that executes on the host
int main(void) {

        unsigned long int start_time,end_time;
        struct timeval start,end;
        double flops;

        int blocksPerGrid = (NPTS + NUM_THREAD - 1) / NUM_THREAD; // Compute number of blocks needed
        float *sumHost, *sumDev;  // Pointer to host & device arrays

        size_t size = blocksPerGrid*NUM_THREAD*sizeof(float);  //Array memory size
        sumHost = (float *)malloc(size);  //  Allocate array on host
        cudaMalloc((void **) &sumDev, size);  // Allocate array on device
        // Initialize array in device to 0
        cudaMemset(sumDev, 0, size);

        gettimeofday(&start, NULL);

        // Do calculation on device
        compute_pi <<<blocksPerGrid, NUM_THREAD>>> (sumDev); // call CUDA kernel
        // Retrieve result from device and store it in host array
        cudaMemcpy(sumHost, sumDev, size, cudaMemcpyDeviceToHost);
        // Reduce
        for(tid=1; tid<NPTS; tid++)
                pi += sumHost[tid];

        gettimeofday(&end, NULL);

        // Print results
        start_time = start.tv_sec * 1e6 + start.tv_usec;
        end_time = end.tv_sec * 1e6 + end.tv_usec;
        flops = NLOOP*(NPTS*(5.0/(end_time-start_time)));
        printf("processes = %d: %d blocks and %d threads per block\n", blocksPerGrid*NUM_THREAD, blocksPerGrid, NUM_THREAD);
        printf("NPTS = %d, NLOOP = %d, pi = %f\n", NPTS, NLOOP, pi/NLOOP);
        printf("time = %f, estimated GFlops = %f\n",(end_time-start_time)/1.0e9,flops);

        // Cleanup
        free(sumHost);
        cudaFree(sumDev);

        return 0;
}