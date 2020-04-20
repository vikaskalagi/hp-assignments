#include "reference_calc.cpp"
#include "utils.h"
#include <stdio.h>

__global__ void rgba_to_greyscale(const uchar4* const rgbimg,unsigned char* const greyscaleimg,int nr, int nc)
{
  
      
      int xindex = threadIdx.x;  
      int yindex = threadIdx.y;
      
      int bx = blockIdx.x;
      int by = blockIdx.y;

      int xdimg = gridDim.x;
      int ydimg = gridDim.y;

      int xdimb = blockDim.x;
      int ydimb = blockDim.y; 
      
      
      int posx = xdimb * bx + xindex;
      int posy = ydimb * by + yindex;
          
      int off1d =  posy * (xdimb * xdimg) + posx;
      
      uchar4 rgba = rgbimg[off1d];  
      float csum = .299f * rgba.x + .587f * rgba.y + .114f * rgba.z;
      greyscaleimg[off1d] = csum; 
    
}

void convert_to_gs(const uchar4 * const h_rgbaImage, uchar4 * const rbgimg_d,
                            unsigned char* const greyscale_d, size_t nr, size_t nc)
{
  
  const dim3 bs(nr/16+1, nc/16+1, 1); 

  const dim3 gs( 16, 16, 1);  
  
  rgba_to_greyscale<<<gs, bs>>>(rbgimg_d, greyscale_d, nr, nc);
  
  cudaDeviceSynchronize(); 
  checkCudaErrors(cudaGetLastError());
}