#ifndef SCHEDULER_H   
#define SCHEDULER_H

#include "process.h"
#include "queue.h"

#define MAX_PROCESSES 4
#define MAX_ARRIVAL_TIME 10
#define MAX_BURST_TIME 10
#define MAX_IO_TIME 10
#define CHANCE_TO_NEED_IO 50

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
int elapsedSlice = 0;

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
void printInitialData();
void printFinalData();

#endif