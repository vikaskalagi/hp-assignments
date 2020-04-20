#include "omp.h"

static long steps = 100000;
double singlestep;
#define THREADS 2

int main()
{
    double pi, sum[THREADS];
    int i, noofthreads;
    

    singlestep = 1.0/(double)steps;

    omp_set_num_threads(THREADS);
    #pragma omp parallel
    {   

        int i, tid, nthreads;
        double openmplol;
        
        nthreads = omp_get_num_threads();
        tid = omp_get_thread_num();
        
        if(tid == 0) 
            nthreads = noofthreads;

        for(i = tid, sum[tid] = 0; i<steps; i += nthreads)
        {
            openmplol = (i + 0.5)*singlestep;
            sum[tid] += 4.0 / (1.0 + openmplol*openmplol);
        }
    }    
    for(i = 0; i<THREADS; i++)
    {
        pi += sum[i] * singlestep;
    }
    return 0;
}