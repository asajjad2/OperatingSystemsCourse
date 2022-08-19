#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>

using namespace std;

char* setup(char* str, int n )
{
    string temp = to_string(n);
    char arr[20];

    for(int i=0;i<temp.length();i++)
        arr[i] = temp[i];

    strcat(str, ":");
    strcat(str, arr);

    return str;
}

int main()
{
    int fd1[2];// = pipe();
    int fd2[2];// = pipe();
    int fd3[2];// = pipe();
    int fd4[2];//= pipe();
    int fd5[2];// = pipe();
    int fd6[2];

    pipe(fd1);
    pipe(fd2);
    pipe(fd3);
    pipe(fd4);
    pipe(fd5);
    pipe(fd6);

    char arr[20] = {"Hello"};

    system("touch test.txt");

    int output_fd = open("test.txt" , O_WRONLY | O_APPEND);

    int input_fd = open("test.txt", O_RDONLY);

    int p1 = fork();

    if(p1==0)
    {
        char* str = setup(arr, getpid());
        dup2(output_fd , 1);
        //if(d<0)
        {
            //cout << "error agaya\n";
        }
        //else
        {
            //cout << str << endl;
        }

        cout << str << endl;
        write(fd1[1], str, 100);
        
        int c1 = fork();

        if(c1==0)
        {
            read(fd1[0], arr, 100);
            char* str = setup(arr, getpid());
            cout << str << endl;
            write(fd2[1], arr, 100);
            int c2 = fork();

            if(c2==0)
            {
                read(fd2[0], arr, 100);
                char* str = setup(arr, getpid());
                cout << str << endl;
                write(fd3[1], arr, 100);
                int c3 = fork();

                if(c3==0)
                {
                    read(fd3[0], arr, 100);
                    char* str = setup(arr, getpid());
                    cout << str << endl;
                    write(fd4[1], arr, 100);
                    int c4 = fork();
                    
                    if(c4==0)
                    {
                        read(fd4[0], arr, 100);
                        char* str = setup(arr, getpid());
                        cout << str << endl;
                        write(fd5[1], arr, 100);
                        int c5 = fork();

                        if(c5 == 0)
                        {
                            read(fd5[0], arr, 100);
                            char* str = setup(arr, getpid());
                            cout << str << endl;
                            write(fd6[1], arr, 100);
                            exit(0);
                        }
                        exit(0);
                    }
                    exit(0);
                    
                }
                exit(0);
            }
            exit(0);
        }
        exit(0);
    }
    if(p1>0)
    {
        read(fd6[0], arr, 20);
        char* str = setup(arr, getpid());
        cout << str << endl;
        exit(0);
    }

    cout << "Testing\n";

    string str;
    dup2(input_fd, 0);
    cin >> str;

    cout << str;

    exit(0);

    return 0;
}