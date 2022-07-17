#include <stdio.h>
#include<stdlib.h>
#include <time.h>
#include "queue.h"


#define MAX_THREADS 20
#define PAGES_PER_THREAD 50
#define FRAMES_PER_THREAD 4
#define TOTAL_FRAMES 64
#define DELTA_TIME_MS 3000

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
void printFrames();
void printPages();
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
    
    
    for (int i = 0; i < MAX_THREADS; i++)
    {
        clearDisplay();

        handleNewProcess();
        
        for (int i = 0; i < _processCount; i++)
        {
            handleThread(i);
        }

        printDisplay();

        getchar();
    }
}

void printDisplay()
{
    printFrames();
    printPages();
}

void printFrames()
{
    printf("\n");

    printf("Frames\t");
    for (int i = 0; i < TOTAL_FRAMES; i++)
    {
        printf("%-3d", i);
    }

    printf("\n");

    printf("Thread\t");
    for (int i = 0; i < TOTAL_FRAMES; i++)
    {
        if(_frameDatas[i].thread == NULL)
            printf("   ");
        else
            printf("%-3d", _frameDatas[i].thread->index);
    }

    printf("\n\n\n");
}

void printPages()
{
    for (int i = 0; i < _processCount; i++)
    {
        printf("Thread %d Page Table\n", i);

        printf("Page\t");

        for (int j = 0; j < PAGES_PER_THREAD; j++)
        {
            printf("%-3d", j);
        }

        printf("\n");

        printf("Frame\t");
        for (int j = 0; j < PAGES_PER_THREAD; j++)
        {
            if(_threads[i].pageToFrame[j] < 0)
                printf("   ");
            else
                printf("%-3d", _threads[i].pageToFrame[j]);
        }

        printf("\n\n");
    }

    printf("\n");
}


void clearDisplay()
{
    system("cls");
    system("clear");
}


void handleNewProcess()
{
    printf("New Process %d\n", _processCount);
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
    printf("(%d) ", threadIndex);
    Thread* thread = &(_threads[threadIndex]);
    
    int pageToAccess = getRandomPageIndex();

    int frame = thread->pageToFrame[pageToAccess];
    

    if(frame < 0)
        handlePageFault(thread, pageToAccess);
    else
    {
        printf("Thread [%d] accessed frame [%d] \n", threadIndex, frame);
    }
}

void handlePageFault(Thread* thread, int pageToAccess)
{
    int lru = -1;
    if(thread->frameCount >= 4)
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

    printf("Handling page fault at Thread %d with LRU = %d \n", thread->index, lru);

    if(oldThread != NULL)
    {
        oldThread->pageToFrame[lruFrameData->pageInThread] = -1;
        oldThread->frameCount--;
    }

    lruFrameData->thread = thread;
    lruFrameData->pageInThread = pageToAccess;

    thread->pageToFrame[pageToAccess] = lru;
    thread->frameCount++;

    clearValueInQueue(_globalFrameQueueLRU, lru);
    
    enqueue(_globalFrameQueueLRU, lru);
    enqueue(thread->threadFrameQueueLRU, lru);
}

int getRandomPageIndex()
{
    return rand() % PAGES_PER_THREAD;
}