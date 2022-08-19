#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main()
{
    int p1 = fork();

    if(p1==0)
    {
        int p2 = fork();

        if(p2==0)
        {
            char* test[] = {"-l", NULL};
			execvp("ls", test);

            exit(90);
        }
        int temp;
        wait(&temp);

        if(temp == 90)
            printf("Child failed\n");
        else
            printf("Child Successful\n");
    }

    return 0;
}