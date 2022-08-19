#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    printf(" P1 id : %d \n", getpid());
    printf(" P1's parent's id : %d \n\n", getppid());

    pid_t p2 = fork();

    if (p2 == 0)
    {
        printf(" P2 id : %d \n", getpid());
        printf(" P2's parent's id : %d \n\n", getppid());

        pid_t p4 = fork();

        if (p4 == 0)
        {
            printf(" P4 id : %d \n", getpid());
            printf(" P4's parent's id : %d \n\n", getppid());
            exit(1);
        }
        else if (p4 > 0)
        {
            int a;
            wait(&a);
            exit(1);
        }
    }
    else if (p2 > 0)
    {
        int a;
        wait(&a);

        pid_t p3 = fork();
        if (p3 == 0)
        {
            printf(" P3 id : %d \n", getpid());
            printf(" P3's parent's id : %d \n\n", getppid());
            exit(1);
        }
        else if (p3 > 0)
        {
            wait(&a);
            exit(1);
        }
    }

    return 0;
}