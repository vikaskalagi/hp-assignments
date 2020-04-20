#include<omp.h>

#define NOOFPTS 1000
#define MI 1000



struct compd {
    double r;
    double k;
};
struct compd pizza;

void fun(struct compd);

int noo = 0;

int main() 
{

    int k, l;
    double A, e;
    double eps = 0.00001;
    
    #pragma omp parallel for default(shared) private(pizza, l) firstprivate(eps)
    for(k = 0; k<NOOFPTS; k++) {
        for(l = 0; l<NOOFPTS; l++) {
            pizza.r = -2.0 + 2.5*(double)(k)/(double)(NOOFPTS) + eps;
            pizza.k = 1.125 * (double)(l)/(double)(NOOFPTS) + eps;
            fun(pizza);
        }
    }
    A = 2.0 * 2.5 * 1.125 * (double)(NOOFPTS*NOOFPTS-noo)/(double)(NOOFPTS*NOOFPTS);
    e = A/(double)(NOOFPTS);
    printf("Area: &lf", A);  
}

void fun(struct compd pizza) 
{

    struct compd node;
    int m;
    double temp;

    node = pizza;
    for(m = 0; m < MI; m++) 
    {

        temp = (node.r*node.k+node.k*node.k) + pizza.r;
        node.k = node.r*node.k*2 + pizza.k;
        node.r = temp;
        if((node.r*node.r + node.k*node.k) > 4.0) 
        {
            #pragma omp atomic         
            noo++;
            break;
        }
    }
    return;

}