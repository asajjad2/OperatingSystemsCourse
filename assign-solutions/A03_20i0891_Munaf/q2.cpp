#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>
#include <cmath>
#include <fstream>
#include <string>

using namespace std;

int rW, cW, rXi, cXi, rB, cB;
double **B, **W,  **res, *tempMatrix;
int **Xi;

template <typename T> 
void displayMatrix(T** m, int r, int c)
{
    for(int i =0; i< r; i++)
    {
        for(int j = 0; j<c; j++)
        {
            cout <<m[i][j] << " ";
        }
        cout << endl;
    }
}

void* Multiply(void* arg) // returns a single column
{
    int temp = static_cast<int>(reinterpret_cast<std::uintptr_t>(arg));

    tempMatrix = new double[cXi];

    for (int i=0; i<cXi; i++)
    {
        tempMatrix[i]=0;

        for (int j=0; j<rXi; j++)
        {
            tempMatrix[i] += W[temp][j] * Xi[j][i];
        }
    }

    tempMatrix[temp] = tempMatrix[cXi-1];

    pthread_exit((void*)tempMatrix);
}

void* Addition(void* arg)
{
    int temp = static_cast<int>(reinterpret_cast<std::uintptr_t>(arg));
    for(int i=0; i<cB; i++)
    {
        res[temp][i] += B[temp][i];
    }

    pthread_exit(NULL);
}

void* Sigmoid(void* arg)
{
    int temp = static_cast<int>(reinterpret_cast<std::uintptr_t>(arg));

    double sig=1.0;
    for(int i=0; i<cB; i++, sig=1.0)
    {
        sig = 1.0 / (1.0 + pow(2.718, res[temp][i]*-1.0));
        res[temp][i] = (int) sig;
    }

    pthread_exit(NULL);
}

void P3()
{
    pthread_t p3Threads[rW];
    void* tempValue[rW];

    for (int i=0; i<rB; i++)
        pthread_create(&p3Threads[i], NULL, Sigmoid, (void*) i);

    for (int i=0; i<rB; i++)
    {
        pthread_join(p3Threads[i], NULL);
    }

    cout << "\nSigmoid matrix:\n\n";
    displayMatrix(res, rB, cB);
}

void P2()
{
    //cout << "Inside P2\n";
    pthread_t p2Threads[rW];
    void* tempValue[rW];

    for (int i=0; i<rW; i++)
        pthread_create(&p2Threads[i], NULL, Addition, (void*) i);

    for (int i=0; i<rW; i++)
    {
        pthread_join(p2Threads[i], NULL);
    }

    cout << "\nAddition matrix:\n\n";
    displayMatrix(res, rB, cB);
}

void P1()
{
    pthread_t p1Threads[rW];
    void* tempValue[rW];

    for (int i=0; i<rW; i++)
        pthread_create(&p1Threads[i], NULL, Multiply, (void*) i);

    for (int i=0; i<rW; i++)
    {
        pthread_join(p1Threads[i], &tempValue[i]);
    }

    for(int i =0; i< rW; i++)
    {
        for(int j = 0; j<cXi; j++)
        {
            res[i][j] = 0;
            res[i][j] = *((double*)(tempValue[i]+j));
        }
    }
    cout << "\nMultiplication matrix:\n\n";
    displayMatrix(res, rW, cXi);
}

void getData()
{
    fstream infile("data.txt");

    string str;

    // taking input of W
    infile >> str;
    rW = stoi(str);

    infile >> str;
    cW = stoi(str);

    W = new double*[rW];

    for(int i =0; i< rW; i++)
        W[i] = new double[cW];


    for(int i =0; i< rW; i++)
        for(int j = 0; j<cW; j++)
        {
            infile >> str;
            W[i][j] = stof(str);
        }

    //displayMatrix(W, rW, cW);

    // taking input of Xi
    infile >> str;
    rXi = stoi(str);

    infile >> str;
    cXi = stoi(str);

    Xi = new int*[rXi];

    for(int i =0; i< rXi; i++)
        Xi[i] = new int[cXi];

    for(int i =0; i< rXi; i++)
        for(int j = 0; j<cXi; j++)
        {
            infile >> str;
            Xi[i][j] = stoi(str);
        }

    //displayMatrix(Xi, rXi, cXi);

    // taking input of B
    infile >> str;
    rB = stoi(str);

    infile >> str;
    cB = stoi(str);

    B = new double*[rB];

    for(int i =0; i< rB; i++)
        B[i] = new double[cW];


    for(int i =0; i< rB; i++)
        for(int j = 0; j<cB; j++)
        {
            infile >> str;
            B[i][j] = stof(str);
        }

    //displayMatrix(B, rB, cB);

    infile.close();

    res = new double*[rXi]{0};

    for(int i =0; i< rXi; i++)
        res[i] = new double[cB]{0};
}

int main()
{
    getData();

    int fd[2];
    pipe(fd);

    int p1 = fork();

    if(p1==0)
    {
        P1();
        
        close(fd[0]);
        dup2(fd[1], 1);
        close(fd[1]);
        for(int i =0; i< rW; i++)
        {
            for(int j = 0; j<cXi; j++)
            {
                cout << res[i][j] << " ";
            }
        }

        exit(0);
    }
    else if(p1>0)
    {
        wait(NULL);
        close(fd[1]);
        dup2(fd[0], 0);
        close(fd[0]);
        double tempV;
        for(int i =0; i< rW; i++)
        {
            for(int j = 0; j<cXi; j++)
            {
                cin >> tempV;
                res[i][j] = tempV;
            }
        }

        int p2 = fork();

        pipe(fd);

        if(p2 == 0)
        {
            P2();

            close(fd[0]);
            dup2(fd[1], 1);
            close(fd[1]);
            for(int i =0; i< rW; i++)
            {
                for(int j = 0; j<cXi; j++)
                {
                    cout << res[i][j] << " ";
                }
            }

            exit(0);
        }
        else if(p2>0)
        {   
            wait(NULL);

            int p3 = fork();

            if(p3 == 0)
            {
                P3();

                exit(0);
            }

            exit(0);
        }
        exit(0);
    }
    

    return 0;
}