#include <stdio.h>
#include <omp.h>
#include <stdlib.h>



struct onenode 
{
   int content;
   int fibo;
   struct onenode* nxt;
};

struct onenode* fun(struct onenode* z);


int fibonacci(int n) 
{
   int var1, var2;
   if (n < 2) {
      return (n);
   } else {
      var1 = fibonacci(n - 1);
      var2 = fibonacci(n - 2);
	  return (var1 + var2);
   }
}

void middleware(struct onenode* z) 
{
   int n, temp;
   n = z->content;
   temp = fibonacci(n);

   z->fibo = temp;

}

struct onenode* fun(struct onenode* z) 
{
    int i;
    struct onenode* head = NULL;
    struct onenode* temp = NULL;
    
    head = malloc(sizeof(struct onenode));
    z = head;
    z->content = 38;
    z->fibo = 0;
    for (i=0; i< 5; i++) {
       temp  = malloc(sizeof(struct onenode));
       z->nxt = temp;
       z = temp;
       z->content = 38 + i + 1;
       z->fibo = i+1;
    }
    z->nxt = NULL;
    return head;
}

int main() 
{
   
     struct onenode *z=NULL;
     struct onenode *temp=NULL;
     struct onenode *head=NULL;

     

     z = fun(z);
     head = z;

    

	#pragma omp parallel 
	{
            #pragma omp master
                  printf("Threads:      %d\n", omp_get_num_threads());

		#pragma omp single
		{
			z=head;
			while (z) {
				#pragma omp task firstprivate(z) 
				{
					middleware(z);
				}
			  z = z->nxt;
		   }
		}
	}


     z = head;
	 while (z != NULL) 
     {
        printf("%d : %d\n",z->content, z->fibo);
        temp = z->nxt;
        free (z);
        z = temp;
     }  
	 free (z);

    

     return 0;
}