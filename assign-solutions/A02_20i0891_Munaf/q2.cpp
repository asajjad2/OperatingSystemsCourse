#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

// starter program for Q2
int main() 
{
    string str;
    int fifo_write;

    cout << "Enter a string: ";
    getline(cin, str);

    //int f1 = mkfifo("q2_pipe_1", 0777);

    cout << "1\n";

    //system("./q2_1");

    fifo_write = open("q2_pipe_1",O_WRONLY);

    cout << "1\n";

    char s[256];

    for(int i =0;i<str.length(); i++)
    {
        s[i] = str[i];
    }
    cout << "1\n";

    if(fifo_write>0)
    {
        write(fifo_write, s, sizeof(str));
        close(fifo_write);
    }

    return 0;
}