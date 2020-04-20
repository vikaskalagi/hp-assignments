#include <stdlib.h>
#include <stdio.h>
#include "omp.h"

#define N 5
#define FS 30
#define MAX 10

struct node {
   int data;
   int fd;
   struct node* next;
};

int fibonacci(int n1) {
   int var1, var2;
   if (n1 < 2) {
      return (n);
   } else {
      var1 = fibonacci(n1 - 1);
      var2 = fibonacci(n1 - 2);
	  return (var1 + var2);
   }
}

void middleware(struct node* p) 
{
   int z;
   z = p->data;
   p->fd = fibonacci(z);
}

struct node* fun(struct node* p) {
    int i;
    struct node* head = NULL;
    struct node* tempvar = NULL;
    
    head = malloc(sizeof(struct node));
    p = head;
    p->data = F;
    p->fd = 0;
    for (i=0; i< N; i++) {
       tempvar  = malloc(sizeof(struct node));
       p->next = tempvar;
       p = tempvar;
       p->data = F + i + 1;
       p->fd = i+1;
    }
    p->next = NULL;
    return head;
}

int main(int argc, char *argv[]) {
     double starttime, endtime;
     struct node *p=NULL;
     struct node *tempvar=NULL;
     struct node *head=NULL;
     struct node *arrayp[MAX]; 
     int i, c=0;
     
 
     p = fun(p);
     head = p;


     starttime = omp_get_wtime();
     {
        while (p != NULL) {
		   middleware(p);
		   p = p->next;
        }
     }

     endtime = omp_get_wtime();

     printf("serial Compute Time: %f seconds\n", endtime - starttime);


     p = head;

     starttime = omp_get_wtime();
     {
        
        while (p != NULL) {
	  	   p = p->next;
               c++;
        }
      

        p = head;
        for(i=0; i<c; i++) {
               arrayp[i] = p;
               p = p->next;
        }
       
        
        #pragma omp parallel 
        {
           #pragma omp single
               printf(" %d threads \n",omp_get_num_threads());
           #pragma omp for schedule(static,1)
           for(i=0; i<c; i++)
		   middleware(arrayp[i]);
        }
     }

     endtime = omp_get_wtime();
     p = head;
	 while (p != NULL) {
        printf("%d : %d\n",p->data, p->fd);
        tempvar = p->next;
        free (p);
        p = tempvar;
     }  
     free (p);

     printf("Time: %f s\n", endtime - starttime);

     return 0;
}