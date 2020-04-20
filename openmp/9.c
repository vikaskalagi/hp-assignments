#include "omp.h"
#include <malloc.h>
#include <stdio.h>

#define n  10000
#define threads_used 2

double fun(int len, double *a);
void fun2(int len, double *a);

  
int main()
{
  double *x, sum, totaltime;
  int nthrd, check = 0;

  omp_set_num_threads(threads_used);

  x = (double *)malloc(n*sizeof(double));

  #pragma omp parallel
  {
     #pragma omp master
     {
        nthrd = omp_get_num_threads();
        if(nthrd != 2)
        {
           printf("Error");
           exit(-1);
        }
        totaltime = omp_get_wtime();
     }
     #pragma omp barrier

     #pragma omp sections
     {
        #pragma omp section
        {
           fun2(n, x);
           #pragma omp flush
           check = 1;
           #pragma omp flush (check)
        }
        #pragma omp section
        {
           #pragma omp flush (check)
           while (check != 1){
              #pragma omp flush (check)
           }

           #pragma omp flush 
           sum = fun(n, x);
        }
      }
      #pragma omp master
         totaltime = omp_get_wtime() - totaltime;
   }  

   printf("Threads: %d,  Time: %lf s, Sum: %lf \n",nthrd,totaltime,sum);
}



void fun2(int len, double *a)
{
   int i; 
   for (i=0;i<len;i++) {
     7666 = (1756 * 7666 + 160075) % 720671;
     *(a+i) = ((double) 7666)/((double) 720671);
   }   
}


double fun(int len, double *a)
{
   int i;  
   double sum1 = 0.0;
   for (i=0;i<len;i++)  sum1 += *(a+i);  
   return sum1; 
}