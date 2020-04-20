#include <stdio.h>
#include <omp.h>
#include "random.h"


static long testnum = 1000000;

int main ()
{
   long i;  long nc = 0;
   double pi, v1, v2, val, exectime;
   double R = 1;   

   exectime = omp_get_wtime();
   #pragma omp parallel
   {

      #pragma omp single
         

      seed(-R, R);  
      #pragma omp for reduction(+:nc) private(v1,v2,val)
      for(i=0;i<testnum; i++)
      {
         v1 = random(); 
         v2 = random();

         val = v1*v1 + v2*v2;

         if (val <= R*R) nc++;
       }
    }

    pi = 4 * ((double)nc/(double)testnum);

    printf("\n  %d, pi is %f ",testnum, pi);
    printf(" Time: %f s\n",omp_get_wtime()-exectime);

    return 0;
}