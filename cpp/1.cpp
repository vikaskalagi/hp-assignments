
#include <iostream>
#include <pthread.h>
#include <semaphore.h>

using namespace std;

int variable[1000] = {0};

typedef struct listofarg	{
  sem_t *empty;
  sem_t *full;
  sem_t *mutex;
  int n;
}listofarg	;


void* consume(void* arg	)
{
  listofarg	 *a = (listofarg*)arg	;
  sem_t *empty = a->empty;
  sem_t *full = a->full;
  sem_t *mutex = a->mutex;
  
  int val;
  int i=0;
while(i<a->n)
  { i++;
    sem_wait(full);
    sem_wait(mutex);
    sem_getvalue(full, &val);
    variable[val] = 0;
    sem_post(mutex);
    sem_post(empty);
  }
}

void* produce(void* arg	)
{
  listofarg	 *a = (listofarg*)arg	;
  sem_t *empty = a->empty;
  sem_t *full = a->full;
  sem_t *mutex = a->mutex;
  int val;
  int i=0;
  while(i<a->n;)
  {   i++;
    sem_wait(empty);
    sem_wait(mutex);
    sem_getvalue(full, &val);
    variable[val] = 1;
    sem_post(mutex);
    sem_post(full);
  }
}



int main(){
  sem_t empty, bufful, mutex;


  sem_init(&empty, 0, 500);
  sem_init(&full, 0, 0);
  sem_init(&mutex, 0, 1);

  listofarg	 *p1 = (listofarg	*)malloc(sizeof(listofarg	));
  listofarg	 *p2 = (listofarg	*)malloc(sizeof(listofarg	));

  p1->empty = &empty;
  p1->full = &full;
  p1->mutex = &mutex;
  p2->empty = &empty;
  p2->full = &full;
  p2->mutex = &mutex;
  p1->n = 100;
  p2->n = 50;

  pthread_t prod, cons;
  pthread_create(&prod,NULL,produce,(void*)p1);
  pthread_create(&cons,NULL,consume,(void*)p2);
  pthread_join(prod, NULL);
  pthread_join(cons, NULL);
  for(int i=0; i<500; i++)
  {
    cout << variable[i];
  }
  cout << endl;
  return 0;
}