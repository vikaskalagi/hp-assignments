#include "omp.h"
#include <stdio.h>

int main() {
    #pragma omp parallel
    {
        int threadid = omp_get_thread_num();
        printf("hello(%d)", threadid);
        printf(" world)%d\n", threadid);
    }
}