#include<device_launch_parameters.h>
#include<device_functions.h>
#include "utils.h"



__global__ void kernel_red_op(float * dop, const float * const dip, bool maxbool)
{
  
  extern __shared__ float dats[];

  int AID = threadIdx.x + blockDim.x * blockIdx.x;
  int thread_id = threadIdx.x;


  dats[thread_id] = dip[AID];
  __syncthreads();           
  for (unsigned int vars = blockDim.x / 2; vars > 0; vars >>= 1)
  {
    if (thread_id < vars)
    {
      if (maxbool)
        dats[thread_id] = max(dats[thread_id], dats[thread_id + vars]);
      else
        dats[thread_id] = min(dats[thread_id], dats[thread_id + vars]);
    }
    __syncthreads();        
  }


  if (thread_id == 0)
  {
    dop[blockIdx.x] = dats[0];
  }
}

__global__ void kern_hist(unsigned int * dop, const float * const dip,
  const size_t noofbins, float LLR, float minLL)
{
  int AID = threadIdx.x + blockDim.x * blockIdx.x;
  int b = (dip[AID] - minLL) / LLR * noofbins;
  if (b == noofbins)  b--;
  atomicAdd(&dop[b], 1);
}

__global__ void scan_kernel(unsigned int * dop, const float * const dip,
  const size_t noofbins, float LLR, float minLL)
{
  int AID = threadIdx.x + blockDim.x * blockIdx.x;
  int b = (dip[AID] - minLL) / LLR * noofbins;
  if (b == noofbins)  b--;
  atomicAdd(&dop[b], 1);
}


__global__ void cdfk(unsigned int * dip, const size_t noofbins)
{
  int AID = threadIdx.x;
  for (int d = 1; d < noofbins; d *= 2) {
    if ((AID + 1) % (d * 2) == 0) {
      dip[AID] += dip[AID - d];
    }
    __syncthreads();
  }
  if (AID == noofbins - 1) dip[AID] = 0;
  for (int d = noofbins / 2; d >= 1; d /= 2) {
    if ((AID + 1) % (d * 2) == 0) {
      unsigned int tmp = dip[AID - d];
      dip[AID - d] = dip[AID];
      dip[AID] += tmp;
    }
    __syncthreads();
  }
}


__global__ void cdfk_2(unsigned int * dip, const size_t noofbins)
{ 
  int idx = threadIdx.x;
  extern __shared__ int temp[];
  int outp = 0, inp = 1;

  temp[idx] = (idx > 0) ? dip[idx - 1] : 0;
  __syncthreads();

  for (int offs = 1; offs < n; offs *= 2) {
  
    outp = 1 - outp;
    inp = 1 - outp;
    if (idx >= offs) {
      temp[outp*n+idx] = temp[inp*n+idx - offs] + temp[inp*n+idx];  
    } else {
      temp[outp*n+idx] = temp[inp*n+idx];
    }
    __syncthreads();
  }
  dip[idx] = temp[outp*n+idx];
}

void myfuncs(const float* const d_LL,
  unsigned int* const d_cdf,
  float &minLL,
  float &max_LL,
  const size_t nr,
  const size_t nc,
  const size_t noofbins)
{

  const int varm = 1 << 10;
  int bl_grp = ceil((float)nc * nr / varm);

  float *middle_i;
  checkCudaErrors(cudaMalloc(&middle_i, sizeof(float)* bl_grp)); 
  float *min_d, *max_d;
  checkCudaErrors(cudaMalloc((void **)&min_d, sizeof(float)));
  checkCudaErrors(cudaMalloc((void **)&max_d, sizeof(float)));

  kernel_red_op << <bl_grp, varm, varm * sizeof(float) >> >(middle_i, d_LL, true);
  kernel_red_op << <1, bl_grp, bl_grp * sizeof(float) >> >(max_d, middle_i, true);
  kernel_red_op << <bl_grp, varm, varm * sizeof(float) >> >(middle_i, d_LL, false);
  kernel_red_op << <1, bl_grp, bl_grp * sizeof(float) >> >(min_d, middle_i, false);
  checkCudaErrors(cudaMemcpy(&minLL, min_d, sizeof(float), cudaMemcpyDeviceToHost));
  checkCudaErrors(cudaMemcpy(&max_LL, max_d, sizeof(float), cudaMemcpyDeviceToHost));

  checkCudaErrors(cudaFree(middle_i));
  checkCudaErrors(cudaFree(min_d));
  checkCudaErrors(cudaFree(max_d));


  float LLR = max_LL - minLL;
  printf("max_LL: %f  minLL: %f  LLR: %f\n", max_LL, minLL, LLR);

  checkCudaErrors(cudaMemset(d_cdf, 0, sizeof(unsigned int)* noofbins));
  kern_hist << <bl_grp, varm >> >(d_cdf, d_LL, noofbins, LLR, minLL);


  cdfk_2 << <1, noofbins, sizeof(unsigned int) * noofbins * 2 >> >(d_cdf, noofbins);
}