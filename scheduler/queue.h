#ifndef QUEUE_H

    #define QUEUE_H
    #include "process.h"
    typedef struct _QueuedProcess 
    {
        Process* process;
        struct _QueuedProcess *prev;
    } QueuedProcess;

    typedef struct _Queue 
    {
        QueuedProcess* head;
        QueuedProcess* tail;
        int size;
        int max;
    } Queue;

    Queue* createQueue(int max);
    void freeQueue(Queue *queue);
    int enqueue(Queue *queue, Process *item);
    Process* dequeue(Queue *queue);
    int isEmpty(Queue* queue);
    //QueuedProcess* dequeueInternal(Queue* queue) ;

#endif