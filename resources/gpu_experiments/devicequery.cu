#include <stdio.h>
 
// Print device properties
void printDevProp(cudaDeviceProp devProp)
{
    printf("Major revision number:         %d\n",  devProp.major);
    printf("Minor revision number:         %d\n",  devProp.minor);
    printf("Name:                          %s\n",  devProp.name);
    printf("Total global memory:           %zu\n",  devProp.totalGlobalMem);
    printf("Warp size:                     %d\n",  devProp.warpSize);
    printf("Maximum shared memory per block: %zu\n",  devProp.sharedMemPerBlock);
    printf("Maximum registers per block:   %d\n",  devProp.regsPerBlock);
    printf("Maximum threads per block:     %d\n",  devProp.maxThreadsPerBlock);
    for (int i = 0; i < 3; ++i)
    printf("Maximum dimension %d of block  %d\n", i, devProp.maxThreadsDim[i]);
    for (int i = 0; i < 3; ++i)
    printf("Maximum dimension %d of grid:  %d\n", i, devProp.maxGridSize[i]);
    printf("Maximum shared memory per SM:  %zu\n",  devProp.sharedMemPerMultiprocessor );
    printf("Maximum registers per SM:      %d\n", devProp.regsPerMultiprocessor);
    printf("Maximum threads per SM:        %d\n", devProp.maxThreadsPerMultiProcessor);
    printf("Maximum thread blocks per SM:  %d\n", devProp.maxBlocksPerMultiProcessor);
    printf("Number of SMs:                 %d\n",  devProp.multiProcessorCount);
    printf("Clock rate:                    %d\n",  devProp.clockRate);
    return;
}
 
int main()
{
    // Number of CUDA devices
    int devCount;
    cudaGetDeviceCount(&devCount);
    printf("CUDA Device Query...\n");
    printf("There are %d CUDA devices.\n", devCount);
 
    // Iterate through devices
    for (int i = 0; i < devCount; ++i)
    {
        // Get device properties
        printf("\nCUDA Device #%d\n", i);
        cudaDeviceProp devProp;
        cudaGetDeviceProperties(&devProp, i);
        printDevProp(devProp);
    }
 
/*
    printf("\nPress any key to exit...");
    char c;
    scanf("%c", &c);
*/
 
    return 0;
}
