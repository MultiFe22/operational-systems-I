#ifndef SCHEDULER_H   
#define SCHEDULER_H

#include "process.h"
#include "queue.h"

#define MAX_PROCESSES 10

#define STATUS_NotArrived 0
#define STATUS_InQueue 1
#define STATUS_InIO 2
#define STATUS_InProcessing 3
#define STATUS_Complete 4

int processorClock = 0;
int arrivedProcesses = 0;
int completedProcesses = 0;
int sliceTime = 4;
Queue* highPriorityQueue;
Queue* lowPriorityQueue;
Queue* ioQueue;
Process* processes[MAX_PROCESSES];
Process* currentCPUProcess;
Process* currentIOProcess;

Process* createProcess();
int generatePid();
int generateRandArrivalTime();
int generateRandBurstTime();
int generateRandIoTime();
int generateRandIoType();
void sortProcessesByArrivalTimeAsc();
Process* checkNextArrival();
void onClockTick();
void handleNext();
void handleSlice();
void tickProcesses();
void tickProcess(Process* process);
void handleProcessArrival();
Process* dequeueNextCPUProcess();
Process* dequeueNextIOProcess();
void finishProgram();
void init();
void printProcessesData();

#endif