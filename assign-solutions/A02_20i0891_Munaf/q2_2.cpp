#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string>
#include <fcntl.h>

using namespace std;

int main() 
{
    // capitals

    char str[256];
    int fifo_read = 0;

    fifo_read = open("q2_pipe_1",O_RDONLY);

    read (fifo_read ,str , sizeof(str));

    close(fifo_read);

    string strTemp =  str; //tolower(str);

    int capitalCount = 0;

    if(fifo_read>0)
    {
        for(int i=0;i<strTemp.length();i++)
        {
            
            if(strTemp[i] >='A' && strTemp[i] <= 'Z' )
                capitalCount++;
        }   
    }

    cout << "Number of Capitals: " << capitalCount << endl;


    int fifo_write = open("q2_pipe_1",O_WRONLY);

    if(fifo_write>0)
    {
        write(fifo_write, str, sizeof(str));
        close(fifo_write);
    }

    return 0;
}