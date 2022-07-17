#ifndef QUEUE_H

    #define QUEUE_H
    typedef struct _QueuedProcess 
    {
        int process;
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
    int enqueue(Queue *queue, int item);
    int dequeue(Queue *queue);
    int isEmpty(Queue* queue);
    void clearValueInQueue(Queue *queue, int value);
    //QueuedProcess* dequeueInternal(Queue* queue) ;

#endif