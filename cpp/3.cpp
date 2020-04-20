#include <iostream>
#include <pthread.h>
#include <string>
#include <fstream>
#include <unistd.h>
#include <stdio.h>
#include <semaphore.h>




using namespace std;

typedef struct node
{
  sem_t *mutex;
  sem_t *mutex2;
  int *rnum;
  string text;
}node;

ofstream f1("file1.txt");
ifstream f2("file1.txt");


void* read(void* a)
{
  node* b = (node*)a;
  sem_t *mutex = b->mutex;
  sem_t *mutex2 = b->mutex2;
  int *rnum = b->rnum;
  int temp;
  string text;
  sem_wait(mutex);
  (*rnum)++;
  if(*rnum == 1)
    sem_wait(mutex2);
  sem_post(mutex);

  while (getline(f2,text)){
      cout << text << '\n';
  }

  sem_wait(mutex);
  (*rnum)--;
  if(*rnum == 0)
    sem_post(mutex2);
  sem_post(mutex);
}

void* write(void* a){
  node* b = (node*)a;
  sem_t *mutex = b->mutex;
  sem_t *mutex2 = b->mutex2;
  int *rnum = b->rnum;
  string text = b->text;
  sem_wait(mutex2);
  f1 << text << "\n";
  sem_post(mutex2);
}

int main(){

  sem_t mutex;
  sem_t mutex2;
  int rnum = 0;
  string text = "some text";
  sem_init(&mutex, 0, 1);
  sem_init(&mutex2, 0, 1);
  node *para = (node*)malloc(sizeof(node));
  para->mutex = &mutex;
  para->mutex2 = &mutex2;
  para->rnum = &rnum;
  para->text = text;

  pthread_t readthread, writethread;
  pthread_create(&writethread, NULL, write, (void*)para);
  pthread_join(writethread, NULL);
  f1.close();

  sleep(5);

  pthread_create(&readthread, NULL, read, (void*)para);
  pthread_join(readthread, NULL);
  f2.close();
  

}