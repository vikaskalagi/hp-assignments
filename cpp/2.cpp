#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>


#define L (philno + 4) % 5
#define R (philno + 1) % 5

int states[5];
int philosophers[5] = { 0, 1, 2, 3, 4 };

sem_t mutex;
sem_t st[5];

void check(int philno)
{
    if (states[philno] == 1 && states[L] != 0 && states[R] != 0){
        states[philno] = 0;
        sleep(2);
        printf("Philosopher: %d picks forks: %d, %d\n", philno + 1, L + 1, philno + 1);
        printf("Eating philosopher: %d", philno + 1);
        sem_post(&st[philno]);
    }
}

void up(int philno) {
    sem_wait(&mutex);
    
    states[philno] = 1;
    printf("Philosopher: %d hungry\n", philno + 1);
    
    check(philno);
    sem_post(&mutex);
    
    sem_wait(&st[philno]);
    sleep(1);
}


void down(int philno) 
{
    sem_wait(&mutex);
    
    states[philno] = 2;
    printf("Philosopher: %d Releases forks: %d, %d \n", philno + 1, L + 1, philno + 1);
    printf("Thinking philosopher: %d\n", philno + 1);

    check(L);
    check(R);
    sem_post(&mutex);
}

void* fun(void* number) 
{
    while (1){
        int* p = (int*)number;
        sleep(1);
        up(*p);
        sleep(0);
        down(*p);
    }
}

int main() {
    int p;
    pthread_t pthreads[5];
    
    sem_init(&mutex, 0, 1);
    for (p = 0; p < 5; p++)
        sem_init(&st[p], 0, 0);
    for (p = 0; p < 5; p++){
       
        pthread_create(&pthreads[p], NULL, fun, &philosophers[p]);
        printf("Thinking philosopher: %d\n", p + 1);
    }
    for (p = 0; p < 5; p++)
        pthread_join(pthreads[p], NULL);
}