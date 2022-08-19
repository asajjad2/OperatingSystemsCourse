#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

 int main()
 {
     sigset_t newmask;
     sigemptyset(&newmask);
     sigaddset(&newmask, SIGQUIT);
     sigaddset(&newmask, SIGTSTP);
     sigaddset(&newmask, SIGINT);
     sigprocmask(SIG_BLOCK, &newmask, NULL);

     int i = 0;

     for (i;; i++)
     {
        printf("\n%d\n", i);
         printf("My PID is %d\n", getpid());
         printf("you can't close me using conventional commands\n");
         sleep(2);
     }
}
