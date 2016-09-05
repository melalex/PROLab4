#include <stdio.h>
#include <malloc.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include "stack.h"

pthread_t thread1;
pthread_t thread2;
pthread_t thread3;
pthread_t thread4;
pthread_t thread5;

pthread_mutex_t mut_q = PTHREAD_MUTEX_INITIALIZER;
sem_t sem_q;
sem_t sem_tr34;
sem_t completedProducers;

void* thread_producer(void* arg)
{
    sem_wait(&completedProducers);

    int num = *(int*)arg,
        sem_value;

    while (1)
    {
        if (length == MAX_LENGTH)
            break;

        if((num == 3 || num == 4) && sem_trywait(&sem_tr34) == 0)
        {
            sem_getvalue(&sem_q, &sem_value);

            if (pthread_mutex_trylock(&mut_q) == 0) {
                push(num);

                sem_getvalue(&sem_q, &sem_value);
                printf("Producer thread%d: semaphore=%d; length=%d CREATED; \n",
                       num, sem_value, length);

                pthread_mutex_unlock(&mut_q);
            }
            else {
                printf("Producer thread%d does some useful work\n", num);
            }

            sem_post(&sem_q);
            sem_post(&sem_tr34);
        }
    }

    sem_post(&completedProducers);

    printf("Producer thread%d  stopped !!!\n",num);

    return NULL;
}

void* thread_consumer (void* arg)
{
    int num = *(int*)arg,
        curr_elem,
        sem_value;

    while (1)
    {
        sem_getvalue(&sem_q, &completedProducers);
        if(completedProducers == 4 && length == 0)
            break;

        sem_wait(&sem_q);

        if(pthread_mutex_trylock(&mut_q) == 0)
        {
            curr_elem = pop();

            sem_getvalue(&sem_q, &sem_value);
            printf("Consumer thread%d: semaphore=%d; element %d TAKEN; \n",
                   num, sem_value, curr_elem);

            pthread_mutex_unlock(&mut_q);
        }
        else
        {
            printf("Consumer thread%d does some useful work\n", num);
        }
   }

    printf("Consumer thread%d  stopped !!!\n",num);

    return NULL;
}

int main()
{
    sem_init (&sem_q, 0, 0);
    sem_init (&sem_tr34, 0, 1);
    sem_init (&completedProducers, 0, 4);

    int thread1_number = 1;
    int thread2_number = 2;
    int thread3_number = 3;
    int thread4_number = 4;
    int thread5_number = 5;

    pthread_create (&thread1,NULL,&thread_producer,(void*)&thread1_number);
    pthread_create (&thread2,NULL,&thread_consumer,(void*)&thread2_number);
    pthread_create (&thread3,NULL,&thread_producer,(void*)&thread3_number);
    pthread_create (&thread4,NULL,&thread_producer,(void*)&thread4_number);
    pthread_create (&thread4,NULL,&thread_producer,(void*)&thread5_number);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);
    pthread_join(thread5, NULL);

    printf("All threads stopped !!!\n");

    return 0;
}