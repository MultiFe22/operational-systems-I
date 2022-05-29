#ifndef PROCESS_H   
#define PROCESS_H

typedef struct _Process 
{
    int pid;
    int ppid;
    
    int priority;
    int status;
    int ioType;

    int arrivalTime;
    int exitTime;
    int burstTime;
    int enterIOTime;
    int ioTime;
    int remainingBurstTime;
    int remainingIOTime;
    int waitingTime;
    int turnaroundTime;    
} Process;

#endif // PROCESS_H