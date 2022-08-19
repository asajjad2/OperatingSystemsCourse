#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>
#include <cmath>
#include <fstream>
#include <string>
#include <chrono>
#include <sys/time.h>
#include <ctime>
#include <queue>
#include <iomanip>

using namespace std;
using namespace std::chrono;


void* UnitOperations(void*);

const int S = 5000;

struct Task {
    int id;
    int taskValue;
    int arrivalTime;
    int unitCount;
    int* unitIdList;
    int* unitValues;
    long int finishTime;
};

queue<Task> tasksQueue;
queue<Task> unitsQueue[5];
queue<Task> doneQueue;

int getTime()
{
    unsigned long int now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    return now;
}

void Reception()
{
    ifstream tempInfile("q3Input.txt");
    string str;
    int count = 0;

    while(getline(tempInfile,str))
        count ++;

    tempInfile.close();

    ifstream infile("q3Input.txt");

    Task tasksList[count];

    for(int i =0; i<count;i++)
    {
        infile >> str;
        tasksList[i].id = stoi(str);

        infile >> str;
        tasksList[i].taskValue = stoi(str);

        infile >> str;
        tasksList[i].unitCount = stoi(str);

        tasksList[i].unitIdList = new int[tasksList[i].unitCount] {0};
        tasksList[i].unitValues = new int[tasksList[i].unitCount] {0};

        for(int j = 0; j<tasksList[i].unitCount; j++)
        {
            infile >> str;
            tasksList[i].unitIdList[j] = stoi(str);
        }

        tasksList[i].arrivalTime = getTime();

        tasksQueue.push(tasksList[i]);
    }
}

void Dispatcher()
{
    while(!tasksQueue.empty())
    {
        Task currentTask = tasksQueue.front();
        tasksQueue.pop();

        pthread_t threadsList[currentTask.unitCount];

        for(int i =0;i <currentTask.unitCount; i++)
        {
            int unitValue = currentTask.unitIdList[i];

            unitsQueue[currentTask.unitIdList[i]].push(currentTask);
            //sleep(1);
            pthread_create(&threadsList[i], NULL, UnitOperations, (void*) unitValue);
        }
        for(int i =0;i <currentTask.unitCount; i++)
        {
            pthread_join(threadsList[i], NULL);
        }
        currentTask.finishTime = getTime();

        doneQueue.push(currentTask);
    }
}

void* UnitOperations(void* arg)
{
    int unitValue = static_cast<int>(reinterpret_cast<std::uintptr_t>(arg));

    Task current = unitsQueue[unitValue].front();
    unitsQueue[unitValue].pop();

    int temp;

    switch(unitValue)
    {
        case 0:
            temp = current.taskValue + 5;
            temp = temp%S;
            current.unitValues[0] = temp;
            break;
        case 1:
            temp = current.taskValue * 7;
            temp = temp%S;
            current.unitValues[1] = temp;
            break;
        case 2:
            temp = pow(current.taskValue, 5);
            temp = temp%S;
            current.unitValues[2] = temp;
            break;
        case 3:
            temp = current.taskValue - 100;
            current.unitValues[3] = temp;
            break;
        case 4:
            temp = pow(current.taskValue, 2);
            current.unitValues[4] = temp;
            break;
    }

    pthread_exit(NULL);
}

void Print()
{
    cout << left << setw(8) << "Task ID" << setw(18) << "Completion Time" << setw(14) << "Unit IDs" << setw(14) << "Task Value 1" << setw(14) << "Task Value 2" << setw(14) << "Task Value 3" << setw(14) << "Task Value 4" << setw(14) << "Task Value 5" << endl;
    while(!doneQueue.empty())
    {
        Task temp = doneQueue.front();
        doneQueue.pop();

        cout << left << setw(8) << temp.id << setw(18) << (temp.finishTime-temp.arrivalTime);
        string strTemp = "";
        string x;
        for(int i =0;i<temp.unitCount; i++)
        {
            x = to_string(temp.unitIdList[i]);
            strTemp += x;
            if(i<temp.unitCount-1)
                strTemp += ",";

        } 

        cout << setw(14) << strTemp;

        for(int i =0;i<temp.unitCount; i++)
        {
            cout << i << "- " << setw(11) << temp.unitValues[i];
        } 

        cout << endl;
    }
}

int main() 
{
    Reception();

    Dispatcher();

    Print();

    return 0;
}