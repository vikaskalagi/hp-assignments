#include <iostream>
#include <pthread.h>
#include <semaphore.h>

using namespace std;

typedef struct node
{
  sem_t* cust;
  sem_t* barb;
  sem_t* mutex;
  int* seats;
}node;

void* customer(void* args);
void* barber(void* args);


void* barber(void* args)
{
  node *a = (node*)args;
  
  sem_t* mutex = a->mutex;
  sem_t* cust = a->cust;
  int* seats = a->seats;
  sem_t* barb = a->barb;

  while(true){
    sem_wait(cust);
    sem_wait(mutex);
    (*seats)++;
    sem_post(mutex);
    sem_post(barb);
  }
}

void* customer(void* args)
{
  node *a = (node*)args;

  sem_t* barb = a->barb;
  int* seats = a->seats;
  sem_t* mutex = a->mutex;
  sem_t* cust = a->cust;
  


  while(true){
    sem_wait(mutex);
    if(*seats > 0){
      (*seats)--;
      sem_post(mutex);
      sem_post(cust);
      sem_wait(barb);
    }
    else{
      sem_post(mutex);
    }
  }
}

int main(){
  sem_t cust, barb, mutex;
  sem_init(&barb,0, 0);
  sem_init(&cust,0, 0);
  sem_init(&mutex, 0, 1);
  int seats = 10;
  pthread_t thread_barber, thread_customer;
  node* a = (node*)malloc(sizeof(node));
  a->cust = &cust;
  a->barb = &barb;
  a->mutex = &mutex;
  a->seats = &seats;
  pthread_create(&thread_barber, NULL, barber, (void*)a);
  pthread_create(&thread_customer, NULL, customer, (void*)a);
  pthread_join(thread_barber, NULL);
  pthread_join(thread_customer, NULL);
  return 0;
}