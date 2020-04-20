#include "reference_calc.cpp"
#include "utils.h"

__global__
void blur_g(const unsigned char* const ipc,
                   unsigned char* const opc,
                   int nr, int nc,
                   const float* const filt, const int fw)
{



  const int2 t2dp = make_int2( blockIdx.x * blockDim.x + threadIdx.x,
                                        blockIdx.y * blockDim.y + threadIdx.y);

  const int t1dp = t2dp.y * nc + t2dp.x;

  if (t2dp.x >= nc || t2dp.y >= nr)
    return;

  float result = 0.f;
  
  for (int fr = -fw/2; fr <= fw/2; ++fr) {
    for (int fc = -fw/2; fc <= fw/2; ++fc) {

      int imr = min(max(t2dp.y + fr, 0), static_cast<int>(nr - 1));
      int imc = min(max(t2dp.x + fc, 0), static_cast<int>(nc - 1));

      float imval = static_cast<float>(ipc[imr * nc + imc]);
      float filval = filt[(fr + fw/2) * fw + fc + fw/2];

      result += imval * filval;
    }
  }

  opc[t1dp] = result;
}


__global__
void septc(const uchar4* const rgbaip,
                      int nr,
                      int nc,
                      unsigned char* const redc,
                      unsigned char* const greenc,
                      unsigned char* const bluec)
{

  const int2 t2dp = make_int2( blockIdx.x * blockDim.x + threadIdx.x,
                                        blockIdx.y * blockDim.y + threadIdx.y);

  const int t1dp = t2dp.y * nc + t2dp.x;


  if (t2dp.x >= nc || t2dp.y >= nr)
    return;

  redc[t1dp] = rgbaip[t1dp].x;
  greenc[t1dp] = rgbaip[t1dp].y;
  bluec[t1dp] = rgbaip[t1dp].z;

}


__global__
void chanrecomb(const unsigned char* const redc,
                       const unsigned char* const greenc,
                       const unsigned char* const bluec,
                       uchar4* const rgbaop,
                       int nr,
                       int nc)
{
  const int2 t2dp = make_int2( blockIdx.x * blockDim.x + threadIdx.x,
                                        blockIdx.y * blockDim.y + threadIdx.y);

  const int t1dp = t2dp.y * nc + t2dp.x;

  if (t2dp.x >= nc || t2dp.y >= nr)
    return;

  unsigned char red   = redc[t1dp];
  unsigned char green = greenc[t1dp];
  unsigned char blue  = bluec[t1dp];


  uchar4 oppix = make_uchar4(red, green, blue, 255);

  rgbaop[t1dp] = oppix;
}

unsigned char *dr, *dg, *db;
float         *dfilter;

void allocateMemoryAndCopyToGPU(const size_t nri, const size_t nci,
                                const float* const h_filter, const size_t fw)
{


  checkCudaErrors(cudaMalloc(&dr,   sizeof(unsigned char) * nri * nci));
  checkCudaErrors(cudaMalloc(&dg, sizeof(unsigned char) * nri * nci));
  checkCudaErrors(cudaMalloc(&db,  sizeof(unsigned char) * nri * nci));


  int n_f_b = sizeof(float) * fw * fw;
  checkCudaErrors(cudaMalloc(&dfilter, n_f_b));
umBytes, cudaMemcpyHostToDevice);

  checkCudaErrors(cudaMemcpy(dfilter, h_filter, n_f_b, cudaMemcpyHostToDevice));

}

void your_gaussian_blur(const uchar4 * const hiprgb, uchar4 * const diprgb,
                        uchar4* const doprgb, const size_t nr, const size_t nc,
                        unsigned char *d_r_b, 
                        unsigned char *d_g_b, 
                        unsigned char *d_b_b,
                        const int fw)
{

  septc<<<gridSize, blockSize>>>(diprgb,nr,nc,dr,dg, db);

  cudaDeviceSynchronize(); checkCudaErrors(cudaGetLastError());


  blur_g<<<gridSize, blockSize>>>(dr,d_r_b,nr,nc, dfilter,fw);
  blur_g<<<gridSize, blockSize>>>(dg,d_g_b,nr,nc,dfilter,fw);
  blur_g<<<gridSize, blockSize>>>(db,d_b_b,nr,nc,dfilter,fw);

  cudaDeviceSynchronize(); checkCudaErrors(cudaGetLastError());


  chanrecomb<<<gridSize, blockSize>>>(d_r_b,d_g_b,d_b_b,doprgb,nr,nc);
  cudaDeviceSynchronize(); checkCudaErrors(cudaGetLastError());

}



void cleanup() {
  checkCudaErrors(cudaFree(dr));
  checkCudaErrors(cudaFree(dg));
  checkCudaErrors(cudaFree(db));
  checkCudaErrors(cudaFree(dfilter));
}