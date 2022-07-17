#include <stdio.h>
#include<stdlib.h>
#include <time.h>
#include "queue.h"

#define MAX_THREADS 20
#define PAGES_PER_THREAD 50
#define FRAMES_PER_THREAD 4
#define TOTAL_FRAMES 64

#define UINT_MAX 4294967295

#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define RESET "\x1B[0m"

typedef struct _Thread
{
    int index;
    int* pageToFrame;
    int frameCount;
    Queue* threadFrameQueueLRU;
} Thread;

typedef struct _FrameData
{
    Thread* thread;
    int pageInThread;
} FrameData;

int _processCount = 0;
int _totalFrameCount = 0;
Thread* _threads;
FrameData* _frameDatas;
Queue* _globalFrameQueueLRU;

void printDisplay();
void printMemory();
void printThreads();
void clearDisplay();
void handleNewProcess();
void handleThread(int threadIndex);
void handlePageFault(Thread* thread, int pageToAccess);
int getRandomPageIndex();

int main()
{
    srand(time(NULL));  

    _globalFrameQueueLRU = createQueue(TOTAL_FRAMES);

    _threads = (Thread*) malloc(MAX_THREADS * sizeof(Thread));
    _frameDatas = (FrameData*) malloc(TOTAL_FRAMES * sizeof(FrameData));
    for (int i = 0; i < TOTAL_FRAMES; i++)
        _frameDatas->thread = NULL;
    
    
    for (int i = 0; i < UINT_MAX; i++)
    {
        clearDisplay();
        handleNewProcess();
        
        for (int i = 0; i < _processCount; i++)
        {
            handleThread(i);
        }

        printDisplay();

        char c = getchar();
        if (c == 'q')
            exit(0);
    }
}

void printDisplay()
{
    printMemory();
    printThreads();
}

void printMemory()
{
    printf("\n");
    printf(GRN"Memory\n"RESET);

    printf(MAG"Frames\t"RESET);
    for (int i = 0; i < TOTAL_FRAMES; i++)
    {
        printf("%-3d", i);
    }

    printf("\n");

    printf(CYN"Thread\t"RESET);
    for (int i = 0; i < TOTAL_FRAMES; i++)
    {
        if(_frameDatas[i].thread == NULL)
            printf("   ");
        else
            printf("%-3d", _frameDatas[i].thread->index);
    }

    printf("\n");

    printf(YEL "Fila\t" RESET);

    QueuedProcess* process = _globalFrameQueueLRU->head;
    while(process != NULL)
    {
        printf("%-3d", process->process);
        process = process->prev;
    }

    printf("\n\n\n");
}

void printThreads()
{
    for (int i = 0; i < _processCount; i++)
    {
        printf(GRN"Thread %d\n"RESET, i);

        printf(MAG"Page\t"RESET);

        for (int j = 0; j < PAGES_PER_THREAD; j++)
        {
            printf("%-3d", j);
        }

        printf("\n");

        printf(CYN"Frame\t"RESET);
        for (int j = 0; j < PAGES_PER_THREAD; j++)
        {
            if(_threads[i].pageToFrame[j] < 0)
                printf("   ");
            else
                printf("%-3d", _threads[i].pageToFrame[j]);
        }
        
        printf("\n");

        printf(YEL "Fila\t" RESET);

        QueuedProcess* process = _threads[i].threadFrameQueueLRU->head;
        while(process != NULL)
        {
            printf("%-3d", process->process);
            process = process->prev;
        }       

        printf("\n\n");
    }

    printf("\n");
}


void clearDisplay()
{
    system("clear");
}


void handleNewProcess()
{
    if (_processCount >= MAX_THREADS)
        return;
    printf("New Process "GRN"%d"RESET"\n", _processCount);
    Thread* thread = &(_threads[_processCount]);

    thread->index = _processCount;
    thread->frameCount = 0;
    thread->pageToFrame = (int*) malloc(PAGES_PER_THREAD * sizeof(int));
    thread->threadFrameQueueLRU = createQueue(FRAMES_PER_THREAD);

    for (int i = 0; i < PAGES_PER_THREAD; i++)
    {
        thread->pageToFrame[i] = -1;
    }
    
    _processCount++;
}

void handleThread(int threadIndex)
{
    Thread* thread = &(_threads[threadIndex]);
    
    int pageToAccess = getRandomPageIndex();

    printf(GRN"Process %d"RESET" wants to access page "GRN"%d"RESET". ", threadIndex, pageToAccess);

    int frame = thread->pageToFrame[pageToAccess];
    if(frame < 0)
    {
        printf("Page fault! ");
        handlePageFault(thread, pageToAccess);
    }
    else
    {
        printf("Accessed frame " GRN "%d" RESET, frame);
        enqueue(_globalFrameQueueLRU, frame);
        enqueue(thread->threadFrameQueueLRU, frame);
    }
    printf("\n");
}

void handlePageFault(Thread* thread, int pageToAccess)
{
    int lru = -1;
    if(thread->frameCount >= FRAMES_PER_THREAD)
    {
        lru = dequeue(thread->threadFrameQueueLRU);
    }
    else
    {
        for (int i = 0; i < TOTAL_FRAMES; i++)
        {
            if(_frameDatas[i].thread == NULL)
            {
                lru = i;
                break;
            }
        }

        if(lru < 0)
            lru = dequeue(_globalFrameQueueLRU);
    }

    FrameData* lruFrameData = &(_frameDatas[lru]);
    Thread* oldThread = lruFrameData->thread;

    printf("Swapping to frame "GRN"%d"RESET"", lru);

    if(oldThread != NULL)
    {
        oldThread->pageToFrame[lruFrameData->pageInThread] = -1;
        oldThread->frameCount--;
        removeFromQueue(oldThread->threadFrameQueueLRU, lru);
    }

    lruFrameData->thread = thread;
    lruFrameData->pageInThread = pageToAccess;

    thread->pageToFrame[pageToAccess] = lru;
    thread->frameCount++;
    
    enqueue(_globalFrameQueueLRU, lru);
    enqueue(thread->threadFrameQueueLRU, lru);
}

int getRandomPageIndex()
{
    return rand() % PAGES_PER_THREAD;
}
