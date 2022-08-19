#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

// 2

void sig_quit(int signo)
{
    printf("caught SIGQUIT\n");
    signal(SIGQUIT, SIG_DFL);
}

void sig_term(int signo)
{
    printf("caught SIGINT\n");
    signal(SIGINT, SIG_DFL);
}

int main()
{
    sigset_t new_mask, old_mask, pending_mask;
    signal(SIGQUIT, sig_quit);
    signal(SIGINT, sig_term);

    sigemptyset(&new_mask);
    sigaddset(&new_mask, SIGINT);
    sigaddset(&new_mask, SIGQUIT);

    sigprocmask(SIG_BLOCK, &new_mask, &old_mask);
    for (int i = 0; i < 10; i++)
    {
        sleep(2);
        printf("\n%d\n", i);
    }

    sigpending(&pending_mask);

    if (sigismember(&pending_mask, SIGQUIT) && sigismember(&pending_mask, SIGINT))
    {
        printf("\nSIGQUIT && SIGINT pending\n");
    }

    sigprocmask(SIG_SETMASK, &old_mask, NULL);
    printf("SIGQUIT & SIGINT unblocked\n");

    // while (1)
    // {
    // }

    exit(0);
}
