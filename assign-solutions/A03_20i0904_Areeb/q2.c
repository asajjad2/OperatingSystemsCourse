#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>
#include <sys/wait.h>

int *matrixXI;
float *matrixB, **matrixW, *result;
int rowsB, rowsXI, rowsW, colsW;

void takeInput()
{
    // allocating memory and getting input
    // main matrix W
    printf("Input number of rows and columns for matrix W\n");
    scanf("%d%d", &rowsW, &colsW);
    matrixW = malloc(rowsW * sizeof(float));
    for (int i = 0; i < rowsW; i++)
    {
        matrixW[i] = malloc(colsW * sizeof(float));
    }
    printf("Input matrix W row-vise\n");

    for (int i = 0; i < rowsW; i++)
    {
        for (int j = 0; j < colsW; j++)
        {
            scanf("%f", &matrixW[i][j]);
        }
    }

    // matrix Xi
    printf("Input number of rows for matrix Xi\n");
    scanf("%d", &rowsXI);
    while (rowsXI != colsW)
    {
        printf("rows for matrix Xi need to be same as columns for matrix W, Input Again..\n");
        scanf("%d", &rowsXI);
    }
    matrixXI = malloc(rowsXI * sizeof(int));
    // for (int i = 0; i < rowsXI; i++)
    // {
    //     matrixXI[i] = malloc(colsXI * sizeof(int));
    // }
    printf("Input matrix Xi\n");

    for (int i = 0; i < rowsXI; i++)
    {
        // for (int j = 0; j < colsXI; j++)
        // {
        scanf("%d", &matrixXI[i]);
        // }
    }

    // matrix B
    rowsB = rowsXI;
    matrixB = malloc(rowsB * sizeof(float));
    // for (int i = 0; i < rowsB; i++)
    // {
    //     matrixB[i] = malloc(colsB * sizeof(float));
    // }
    printf("Input matrix B\n");
    for (int i = 0; i < rowsB; i++)
    {
        // for (int j = 0; j < colsB; j++)
        // {
        scanf("%f", &matrixB[i]);
        // }
    }

    result = malloc(rowsB * sizeof(float));
}

void *multiplyMatrixRow(void *arg)
{
    float *result = malloc(sizeof(float));
    *result = 0;
    int index = *((int *)arg);
    for (int i = 0; i < colsW; i++)
    {
        *result += matrixW[index][i] * matrixXI[i];
    }
    pthread_exit((void *)result);
}

void *addMatrixRow(void *arg)
{
    float *result = malloc(sizeof(float));
    *result = 0;
    int index = *((int *)arg);
    *result += result[index] + matrixB[index];
    pthread_exit((void *)result);
}

void *sigmoid(void *arg)
{
    printf("entered sigmoid\n");
    double *result = malloc(sizeof(double));
    *result = 0;
    int index = *((int *)arg);
    double sig = exp((double)-1 * result[index]);
    *result = 1 / (1 + sig);
    result[index] = *result;
    pthread_exit(NULL);
}

int main()
{

    pid_t p_id1, p_id2, p_id3;
    int fd[2];
    pipe(fd);

    takeInput();

    p_id1 = fork();
    if (p_id1 < 0)
    {
        printf("process1 failed creation\n");
    }
    else if (p_id1 == 0)
    {
        close(fd[0]);
        pthread_t thread_arr[rowsW];
        for (int i = 0; i < rowsW; i++)
        {
            pthread_create(&thread_arr[i], NULL, multiplyMatrixRow, (void *)(intptr_t)i);
        }
        dup2(fd[1], 1);
        void *multiplicationResult;
        for (int i = 0; i < rowsW; i++)
        {
            pthread_join(thread_arr[i], &multiplicationResult);
            printf("%f\n", *((float *)multiplicationResult));
        }
        exit(0);
    }
    else
    {
        close(fd[1]);
        wait(NULL);
        dup2(fd[0], 0);
        for (int i = 0; i < rowsW; i++)
        {
            scanf("%f", &result[i]);
            printf("%.2f\n", result[i]);
        }
        close(fd[0]);
        pipe(fd);
        p_id2 = fork();

        if (p_id2 < 0)
        {
            printf("process2 creation failed\n");
        }
        else if (p_id2 == 0)
        {
            close(fd[0]);
            pthread_t thread_arr[rowsB];
            for (int i = 0; i < rowsB; i++)
            {
                pthread_create(&thread_arr[i], NULL, multiplyMatrixRow, (void *)(intptr_t)i);
            }
            dup2(fd[1], 1);
            void *additionResult;
            for (int i = 0; i < rowsB; i++)
            {
                pthread_join(thread_arr[i], &additionResult);
                printf("%f\n", *((float *)additionResult));
            }
            exit(0);
        }
        else
        {
            close(fd[1]);
            wait(NULL);
            dup2(fd[0], 0);
            for (int i = 0; i < rowsB; i++)
            {
                scanf("%f", &result[i]);
                printf("%.2f\n", result[i]);
            }
            close(fd[0]);
            pipe(fd);
            p_id3 = fork();
            if (p_id3 < 0)
            {
                printf("process3 creation failed\n");
            }
            else if (p_id3 == 0)
            {
                close(fd[0]);
                pthread_t thread_arr[rowsB];
                for (int i = 0; i < rowsB; i++)
                {
                    pthread_create(&thread_arr[i], NULL, sigmoid, (void *)(intptr_t)i);
                }

                dup2(fd[1], 1);

                for (int i = 0; i < rowsW; i++)
                {
                    pthread_join(thread_arr[i], NULL);
                    printf("%f\n", (result[i]));
                }
                exit(0);
            }
            else
            {
                close(fd[1]);
                wait(NULL);
                dup2(fd[0], 0);

                for (int i = 0; i < rowsW; i++)
                {
                    scanf("%f", &result[i]);
                    printf("%.2f\n", result[i]);
                }
            }
            close(fd[0]);
        }
    }
}
