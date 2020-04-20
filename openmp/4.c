#include "omp.h"

static long steps = 100000;
double step;
#define THREADS 2

int main()
{
    
    double pi = 0;
    int noofthreads;
    step = 1/(double)steps;

    omp_set_num_threads(THREADS);

    #pragma omp parallel
    {   

        int i, tid, numth;
        double openmplol, sum1;
        
        numth = omp_get_num_threads();
        tid = omp_get_thread_num();
        
        if(tid == 0) 
            noofthreads = numth;

        numth = omp_get_num_threads();
        tid = omp_get_thread_num();
        
        for(i = tid, sum1 = 0; i<steps; i+=noofthreads) {
            openmplol = (i + 0.5)*step;
            sum1 += 4.0/(1.0 + openmplol*openmplol);
        }
        #pragma omp critical
        {
            pi += sum1*step;
        }
    }    
    return 0;
}