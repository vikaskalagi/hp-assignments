#include "omp.h"

static long steps = 100000;
double singlestep;
#define THREADS 2


int main()
{
    
    int i;
    double variable, pi;
    double sum = 0;

    singlestep = 1/ (double)steps;

    #pragma omp parallel
    {   
        double variable;

        #pragma omp for reduction(+:sum)
        {
            for(i = 0; i<steps; i++)
            {
                variable = (i + 0.5)*singlestep;
                sum += 4.0/(1.0 + variable*variable);
            }
        }
        pi = singlestep*sum;
    }    
    return 0;
}