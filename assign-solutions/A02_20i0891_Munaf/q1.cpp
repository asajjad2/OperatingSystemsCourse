#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>

using namespace std;

int main()
{   
	int fd[2];  
	pipe(fd);
	pid_t pid = fork();
	system("touch file.txt");
	if (pid == 0)		
	{
        cout << "\nexecuting ls -l/\n";
		dup2(fd[0],0);

        char *args[] = {"ls", "-l", "/", "file.txt", NULL};
		execv("/bin/ls", args);
		exit(1);
	}
	else
	{
		wait(NULL);
		pid = fork();
		if (pid == 0)
		{
            cout << "\nexecuting grep a\n";
			dup2(fd[0], 0);
            
			char *args[] = {"grep", "a" ,"file.txt", NULL};
			execv("/usr/bin/grep", args);
			exit(1);
		}
		else
		{
			wait(NULL);
			pid = fork();
			if (pid == 0)
			{
                cout << "\nexecuting  sort -r\n";
				dup2(fd[0], 0);

                char *args[] = {"sort", "-r", "file.txt", NULL};
				execv("/bin/sort", args);
				exit(1);
			}
			else
			{
				wait(NULL);
				pid = fork();
				if (pid == 0)
				{
                    cout << "\nexecuting  wc > file.txt\n";
					dup2(fd[0], 0);
                    execlp("wc", "wc", "file.txt", (char*)NULL);
                    exit(1);
				}
				else
				{
					wait(NULL);
                    int status;
                    waitpid(pid, &status, 0);
				}
			}
		}
	}
	return 0;
}