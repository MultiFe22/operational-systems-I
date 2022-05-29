#ifndef PROCESS_H   
#define PROCESS_H

typedef struct _Process {
    int pid;
    int arrivalTime;
    int ppid;
    int burstTime;
    int priority;
    int status;
    int ioType;
    int ioTime;
    int remainingTime;
    int waitingTime;
    int turnaroundTime;
} Process;

#endif // PROCESS_H