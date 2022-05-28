#include <stdio.h>
#include "process.h"
#include <stdlib.h>





Process* createProcess(int pid, int arrivalTime, int burstTime, int priority)
{
    Process* p = (Process*)malloc(sizeof(Process));
    p->pid = pid;
    p->arrivalTime = arrivalTime;
    p->burstTime = burstTime;
    p->priority = priority;
    p->remainingTime = burstTime;
    p->waitingTime = 0;
    p->turnaroundTime = 0;
    return p;
}




int main(int argc, char const *argv[])
{
    Process* p1 = createProcess(1, 1, 1, 1);
    printf("%d\n" ,p1->arrivalTime);
    /* code */
    return 0;
}
