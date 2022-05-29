#include <stdio.h>
#include "process.h"
#include <stdlib.h>
#include <time.h>
#include "queue.h"

int processorClock = 0;
const int MAX_PROCESSES = 10;
int numProcesses = 0;
int sliceTime = 4;
Queue* highPriorityQueue;
Queue* lowPriorityQueue;
Queue* ioQueue;

Process* createProcess();
int generatePid();
int generateRandArrivalTime();
int generateRandBurstTime();
int generateRandIoTime();
int generateRandIoType();
void sortProcessesByArrivalTimeAsc(Process* processes[], int numberOfProcesses);
Process* checkNextArrival(Process* processes[]);
void onClockTick(Process* processes[]);


int main(int argc, char const *argv[])
{
    srand(time(NULL));
    Process* processes[MAX_PROCESSES];
    highPriorityQueue = createQueue(MAX_PROCESSES);
    lowPriorityQueue = createQueue(MAX_PROCESSES);
    ioQueue = createQueue(MAX_PROCESSES);

    for (int i = 0; i < MAX_PROCESSES; i++)
    {
        processes[i] = createProcess();
    }

    sortProcessesByArrivalTimeAsc(processes, MAX_PROCESSES);
    
    //PRINT all the pid and arrival time in the array
    for (int i = 0; i < MAX_PROCESSES; i++)
    {
        printf("%d %d\n", processes[i]->pid, processes[i]->arrivalTime);
    }

    

    /* code */
    return 0;
}


Process* createProcess()
{
    static int count = 0;
    Process* p = (Process*)malloc(sizeof(Process));
    p->pid = ++count;;
    p->arrivalTime = rand() % 100;
    p->burstTime = rand() % 20;;
    p->priority = 1;
    p->remainingTime = p->burstTime;
    p->waitingTime = 0;
    p->turnaroundTime = 0;
    p->ioTime = rand() % 20;
    p->ioType = rand() % 3;
    return p;
}

void sortProcessesByArrivalTimeAsc(Process* processes[], int numberOfProcesses)
{
    int i, j;
    for (i = 0; i < numberOfProcesses - 1; i++)
    {
        for (j = 0; j < numberOfProcesses - i - 1; j++)
        {
            if (processes[j]->arrivalTime > processes[j + 1]->arrivalTime)
            {
                Process* temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }
}

Process* checkNextArrival(Process* processes[])
{
    static int arrivedCount = 0;
    if(arrivedCount >= MAX_PROCESSES)
        return NULL;
    
    if(processes[++arrivedCount]->arrivalTime == processorClock)
        return processes[arrivedCount];

    return NULL;
}


// void onClockTick(Process* processes[])
// {
//     Process* process = checkNextArrival(processes);
//     while(process != NULL)
//     {
//         enqueue(highPriorityQueue, process);
//         process = checkNextArrival(processes);
//     }
    
// }