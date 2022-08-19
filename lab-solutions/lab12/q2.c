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
    int num = (rand() % (3 - 1 + 1)) + 1;
    sem_wait(&sem);
    sem_getvalue(&sem, &value);
    printf("\nShips: %d Want to cross bridge: %d\n\n", num, value);

    for (int i = 0; i < num; i++)
    {
        printf("Ships going --- Ship # %d\n", i + 1);
    }
    pthread_exit(0);
}

void *allow_ships(void *arg)
{
    int value;

    int num = (rand() % (3 - 1 + 1)) + 1;
    sem_wait(&sem);
    sem_getvalue(&sem, &value);
    printf("\nCars: %d Want to cross bridge: %d\n\n", num, value);
    for (int i = 0; i < num; i++)
    {
        printf("Cars going --- Car # %d\n", i + 1);
    }
    pthread_exit(0);
}

int main()
{

    pthread_t tid[4];
    sem_init(&sem, 0, 4);

    for (int i = 0; i < 8; i++)
    {
        if (i % 2 == 0)
        {
            pthread_create(&tid[i], NULL, allow_cars, NULL);
        }
        else
        {
            pthread_create(&tid[i], NULL, allow_ships, NULL);
        }
    }

    int value;
    sleep(3);
    printf(" Main : Going to Post\n");
    sem_post(&sem);
    sem_getvalue(&sem, &value);
    printf(" Main : After Post sem state :%d \n", value);
    sleep(3);
    printf(" Main : Going to Post\n");
    sem_post(&sem);
    sem_getvalue(&sem, &value);
    printf(" Main : After Post sem state :%d \n", value);
    sleep(3);
    printf(" Main : Going to Post\n");
    sem_post(&sem);
    sem_getvalue(&sem, &value);
    printf(" Main : After Post sem state :%d \n", value);
    sleep(3);
    printf(" Main : Going to Post\n");
    sem_post(&sem);
    sem_getvalue(&sem, &value);
    printf(" Main : After Post sem state :%d \n", value);
    pthread_exit(NULL);
}