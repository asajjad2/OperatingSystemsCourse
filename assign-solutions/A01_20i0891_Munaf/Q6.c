#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    int a = 1, b = 2, c = 3, d = 4, e = 5, f = 6;
    int y, u, i;
    
    printf("Input values a, b, c, d, e, f respectively\n");
    scanf("%d %d %d %d %d %d", &a,&b,&c,&d,&e,&f);
    


    pid_t p1 = fork();

    if (p1 == 0)
    {
        exit(a * b);
    }
    else if (p1 > 0)
    {
        wait(&y);
        y/=255;
    }
    pid_t c2 = fork();
    if (c2 == 0)
    {
        exit(c / d);
    }
    else if (c2 > 0)
    {
        wait(&u);
        u/=255;
    }
    pid_t c3 = fork();
    if (c3 == 0)
    {
        if(e<f)
        {
            exit(f-e);
        }
        else
        exit(e - f);
    }
    else if (c3 > 0)
    {
        wait(&i);
        i/=255;
        
        if(e<f)
        {
            i=i*-1;
        }
        
        int x = y+u+i;
        printf("%d ", x);
    }

    return 0;
}