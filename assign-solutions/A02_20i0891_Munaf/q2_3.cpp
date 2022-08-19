#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string>
#include <fcntl.h>

using namespace std;

// calcualte vowels
int main() 
{
    // reverse

    char str[256];
    int fifo_read = 0;

    fifo_read = open("q2_pipe_1",O_RDONLY);

    read (fifo_read ,str , sizeof(str));

    close(fifo_read);

    string strTemp =  str; //tolower(str);

    cout << "Reverse: ";

    if(fifo_read>0)
    {
        for(int i=strTemp.length()-1;i>0;i--)
        {
            cout << strTemp[i];
        }   
    }

    return 0;
}