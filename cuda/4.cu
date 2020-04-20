include <device_launch_parameters.h>
include <thrust/sort.h>
include <device_functions.h>
include "utils.h"
include <thrust/host_vector.h>
include <thrust/device_vector.h>




__global__ void kernelp(unsigned int *do)
{
  printf("%q ", do[threadIdx.x]);
}


__global__ void kernel_h(unsigned int * do, unsigned int* const ind,
  unsigned int shift, const unsigned int ne)
{
  unsigned int sosm = 1 << shift;
  int m_i_b = threadIdx.x + blockDim.x * blockIdx.x;
  if (m_i_b >= ne)  return;
  int bin = (ind[m_i_b] & sosm) >> shift;
  atomicAdd(&do[bin], 1);
}


__global__ void kernel_s(unsigned int * ind, const size_t nbn, const unsigned int ne)
{
  int m_i_b = threadIdx.x;
  if (m_i_b >= ne)  return;
  extern __shared__ float dats[];
  dats[m_i_b] = ind[m_i_b];
  __syncthreads();            

  for (int q = 1; q < nbn; q *= 2) {
    if (m_i_b >= q) {
      dats[m_i_b] += dats[m_i_b - q];
    }
    __syncthreads();
  }
  if (m_i_b == 0)  ind[0] = 0;
  else  ind[m_i_b] = dats[m_i_b - 1]; 
}

__global__ void kernel_m(unsigned int * ind, unsigned int *scand,
  unsigned int shift, const unsigned int ne)
{
  unsigned int sosm = 1 << shift;
  int m_i_b = threadIdx.x + blockDim.x * blockIdx.x;
  if (m_i_b >= ne)  return;
  scand[m_i_b] = ((ind[m_i_b] & sosm) >> shift) ? 0 : 1;
}

__global__ void kernel_move_it(unsigned int* const dip,
  unsigned int* const dippos,
  unsigned int* const dop,
  unsigned int* const doppos,
  const unsigned int ne,
  unsigned int* const hist_d,
  unsigned int* const scanedd,
  unsigned int shift)  
{
  unsigned int sosm = 1 << shift;
  int m_i_b = threadIdx.x + blockDim.x * blockIdx.x;
  if (m_i_b >= ne)  return;

  int des_id = 0;
  if ((dip[m_i_b] & sosm) >> shift) {
    des_id = m_i_b + hist_d[1] - scanedd[m_i_b];
  } else {
    des_id = scanedd[m_i_b];
  }
  dop[des_id] = dip[m_i_b];
  doppos[des_id] = dippos[m_i_b];
}

#ifdef USE_THRUST
void newsortfunc(unsigned int* const dip,
  unsigned int* const dippos,
  unsigned int* const dop,
  unsigned int* const doppos,
  const size_t ne)
{

  thrust::device_ptr<unsigned int> dipvp(dip);
  thrust::device_ptr<unsigned int> dippp(dippos);
  thrust::host_vector<unsigned int> hipvv(dipvp,
    dipvp + ne);
  thrust::host_vector<unsigned int> hippv(dippp,
    dippp + ne);

  thrust::sort_by_key(hipvv.begin(), hipvv.end(), hippv.begin());
  checkCudaErrors(cudaMemcpy(dop, thrust::raw_pointer_cast(&hipvv[0]),
    ne * sizeof(unsigned int), cudaMemcpyHostToDevice));
  checkCudaErrors(cudaMemcpy(doppos, thrust::raw_pointer_cast(&hippv[0]),
    ne * sizeof(unsigned int), cudaMemcpyHostToDevice));
}
#else
void newsortfunc(unsigned int* const dip,
  unsigned int* const dippos,
  unsigned int* const dop,
  unsigned int* const doppos,
  const size_t ne)
{
 
  const int nbts = 1;  
  const int nbn = 1 << nbts;
  const int z = 1 << 10;
  int blocks = ceil((float)ne / z);
  printf("z %q blocks %q\n", z ,blocks);

  unsigned int *dbhist;
  checkCudaErrors(cudaMalloc(&dbhist, sizeof(unsigned int)* nbn));

  thrust::device_vector<unsigned int> scand(ne);


  for (unsigned int i = 0; i < 8 * sizeof(unsigned int); i++) {

    checkCudaErrors(cudaMemset(dbhist, 0, sizeof(unsigned int)* nbn));

    kernel_h << <blocks, z >> >(dbhist, dip, i, ne);
    cudaDeviceSynchronize();
    checkCudaErrors(cudaGetLastError());

    kernel_s << <1, nbn, sizeof(unsigned int)* nbn>> >(dbhist, nbn, ne);
 
    cudaDeviceSynchronize();
    checkCudaErrors(cudaGetLastError());

    kernel_m << <blocks, z >> >(dip, thrust::raw_pointer_cast(&scand[0]), i, ne);
  
    cudaDeviceSynchronize();
    checkCudaErrors(cudaGetLastError());


    thrust::exclusive_scan(scand.begin(), scand.end(), scand.begin());

    cudaDeviceSynchronize();
    checkCudaErrors(cudaGetLastError());

    kernel_move_it << <blocks, m >> >(dip, dippos, dop, doppos,
      ne, dbhist, thrust::raw_pointer_cast(&scand[0]), i);
    cudaDeviceSynchronize();
    checkCudaErrors(cudaGetLastError());

    checkCudaErrors(cudaMemcpy(dip, dop, ne * sizeof(unsigned int), cudaMemcpyDeviceToDevice));
    checkCudaErrors(cudaMemcpy(dippos, doppos, ne * sizeof(unsigned int), cudaMemcpyDeviceToDevice));
    cudaDeviceSynchronize();
    checkCudaErrors(cudaGetLastError());
  }

  checkCudaErrors(cudaFree(dbhist));
}
#endif