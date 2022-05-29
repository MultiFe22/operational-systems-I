#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "queue.h"
#include "process.h"
#include "scheduler.h"

int main(int argc, char const *argv[])
{
    init();

    printProcessesData();

    while(completedProcesses < MAX_PROCESSES)
    {
        onClockTick(processes);
    }

    finishProgram();

    return 0;
}

void finishProgram()
{
    printf("All processes are complete\n");

    freeQueue(highPriorityQueue);
    freeQueue(lowPriorityQueue);
    freeQueue(ioQueue);

    for (int i = 0; i < MAX_PROCESSES; i++)
    {
        free(processes[i]);
    }
}

void init()
{
    srand(time(NULL));
    highPriorityQueue = createQueue(MAX_PROCESSES);
    lowPriorityQueue = createQueue(MAX_PROCESSES);
    ioQueue = createQueue(MAX_PROCESSES);

    for (int i = 0; i < MAX_PROCESSES; i++)
    {
        processes[i] = createProcess();
    }

    sortProcessesByArrivalTimeAsc(processes, MAX_PROCESSES);
}

void printProcessesData()
{
    printf("%-12s%-12s%-12s%-12s%-12s%-12s\n", "PID", "ArrivalTime", "BurstTime", "EnterIOTime", "IOTime", "IOType");
    for (int i = 0; i < MAX_PROCESSES; i++)
    {
        printf("%-12d%-12d%-12d%-12d%-12d%-12d\n", processes[i]->pid, processes[i]->arrivalTime, processes[i]->burstTime, processes[i]->enterIOTime, processes[i]->ioTime, processes[i]->ioType);
    }
}

Process* createProcess()
{
    static int count = 0;
    Process* p = (Process*) malloc(sizeof(Process));

    p->pid = ++count;
     p->status = STATUS_NotArrived;

    p->arrivalTime = rand() % 100;
    p->burstTime = rand() % 20;
    p->ioTime = rand() % 20;
    p->enterIOTime = rand() % p->burstTime;
    p->ioType = rand() % 3;

    p->remainingBurstTime = p->burstTime;
    p->remainingIOTime = p->ioTime;
    p->exitTime = -1;
    p->waitingTime = 0;
    p->turnaroundTime = 0;
    
   
    return p;
}

void sortProcessesByArrivalTimeAsc()
{
    int i, j;
    for (i = 0; i < MAX_PROCESSES - 1; i++)
    {
        for (j = 0; j < MAX_PROCESSES - i - 1; j++)
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


void onClockTick()
{
    printf("Clock Tick! %d\n", processorClock);

    handleSlice();

    handleProcessArrival();

    handleNext();

    tickProcesses();

    processorClock++;    
}

void handleProcessArrival()
{
    Process* process = checkNextArrival(processes);
    while(process != NULL)
    {
        printf("Process %d Arrived\n", process->pid);
        enqueue(highPriorityQueue, process);
        process->status = STATUS_InQueue;
        arrivedProcesses++;
        process = checkNextArrival(processes);
    }
}

Process* checkNextArrival()
{
    static int arrivedCount = 0;
    if(arrivedCount >= MAX_PROCESSES)
        return NULL;

    if(processes[arrivedCount]->arrivalTime == processorClock)
    {
        Process *process = processes[arrivedCount];
        arrivedCount++;
        return process;
    }

    return NULL;
}

void handleNext()
{
    if(currentCPUProcess == NULL)
    {
        currentCPUProcess = dequeueNextCPUProcess();
        if(currentCPUProcess != NULL)
        {
            currentCPUProcess->status = STATUS_InProcessing;
            printf("Process %d is now being processed\n", currentCPUProcess->pid);
        }
    }

    if(currentIOProcess == NULL)
    {
        currentIOProcess = dequeueNextIOProcess();
        if(currentIOProcess != NULL)
        {
            currentIOProcess->status = STATUS_InIO;
            printf("Process %d is now in IO\n", currentIOProcess->pid);
        }
    }
}

void handleSlice()
{
    if(currentCPUProcess == NULL)
        return;

    if(processorClock == 0)
        return;

    if(processorClock % sliceTime == 0)
    {
        printf("Process %d was interrupted\n", currentCPUProcess->pid);
        enqueue(lowPriorityQueue, currentCPUProcess);
        currentCPUProcess->status = STATUS_InQueue;
        currentCPUProcess = NULL;
    }
}

void tickProcesses()
{
    for (int i = 0; i < MAX_PROCESSES; i++)
    {
        tickProcess(processes[i]);
    }
}

void tickProcess(Process* process)
{
    if(process == NULL)
        return;

    switch(process->status)
    {
        case STATUS_NotArrived:
        break;

        case STATUS_InQueue:
            (process->turnaroundTime)++;
            (process->waitingTime)++;
        break;

        case STATUS_InIO: 
            (process->turnaroundTime)++;
            (process->remainingIOTime)--;

            int ioProgression = process->ioTime - process->remainingIOTime;
            printf("IO process %d [%d/%d]\n", process->pid, ioProgression, process->ioTime);

            if(process->remainingIOTime <= 0)
            {
                printf("Process %d finished IO\n", process->pid);
                process->status = STATUS_InQueue;
                currentIOProcess = NULL;
                switch(process->ioType)
                {
                    case 0: //disco
                        enqueue(lowPriorityQueue, process);
                    break;

                    case 1: //fita
                        enqueue(highPriorityQueue, process);
                    break;

                    case 2: //impressora
                        enqueue(highPriorityQueue, process);
                    break;
                }
            }
        break;

        case STATUS_InProcessing: 
            (process->turnaroundTime)++;
            (process->remainingBurstTime)--;

            int cpuProgression = process->burstTime - process->remainingBurstTime;
            printf("CPU process %d [%d/%d]\n", process->pid, cpuProgression, process->burstTime);

            if(process->remainingBurstTime <= 0)
            {
                printf("Process %d completed execution\n", process->pid);
                completedProcesses++;
                process->exitTime = processorClock;
                process->status = STATUS_Complete;
                currentCPUProcess = NULL;
            }
            else if(process->burstTime - process->remainingBurstTime == process->enterIOTime)
            {
                printf("Process %d is now queued for IO\n", process->pid);
                process->status = STATUS_InQueue;
                enqueue(ioQueue, process);
                currentCPUProcess = NULL;
            }

        break;

        case STATUS_Complete: 
        break;
    }
}

Process* dequeueNextCPUProcess()
{
    if(!isEmpty(highPriorityQueue))
        return dequeue(highPriorityQueue);
    if(!isEmpty(lowPriorityQueue))
        return dequeue(lowPriorityQueue);

    return NULL;
}

Process* dequeueNextIOProcess()
{
    if(!isEmpty(ioQueue))
        return dequeue(ioQueue);

    return NULL;
}