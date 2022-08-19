#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t sem;

void *allow_cars(void *arg)
{
    int value;

    srand(time(0));
    int num = (rand() % (20 - 1 + 1)) + 1;
    sem_wait(&sem);
    sem_getvalue(&sem, &value);
    printf("\nShips: %d\n\n", num);

    for (int i = 0; i < num; i++)
    {
        printf("Ships going --- Ship # %d\n", i + 1);
    }
    pthread_exit(0);
}

void *allow_ships(void *arg)
{
    int value;

    int num = (rand() % (20 - 1 + 1)) + 1;
    sem_wait(&sem);
    sem_getvalue(&sem, &value);
    printf("\nCars: %d\n\n", num);
    for (int i = 0; i < num; i++)
    {
        printf("Cars going --- Car # %d\n", i + 1);
    }
    pthread_exit(0);
}

int main()
{

    pthread_t tid1, tid2;
    sem_init(&sem, 0, 1);

    pthread_create(&tid1, NULL, allow_cars, NULL);
    pthread_create(&tid1, NULL, allow_ships, NULL);
    sem_post(&sem);
    pthread_exit(NULL);
}