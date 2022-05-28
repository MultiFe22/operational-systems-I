#include <stdlib.h>
#include <stdio.h>

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

Queue* createQueue(int max) 
{
    Queue *queue = (Queue*) malloc(sizeof (Queue));
    
    queue->max = max;
    queue->size = 0;
    queue->head = NULL;
    queue->tail = NULL;

    return queue;
}

void freeQueue(Queue* queue) 
{
    QueuedProcess* item;

    while (!isEmpty(queue)) 
    {
        item = dequeue(queue);
        free(item);
    }

    free(queue);
}

int enqueue(Queue* queue, Process* process) 
{
    if (queue->size >= queue->max)
        return 0;

    QueuedProcess* item = (QueuedProcess*) malloc(sizeof(QueuedProcess));

    item->process = process;
    item->prev = NULL;

    if (queue->size == 0) 
    {
        queue->head = item;
        queue->tail = item;
    } 
    else 
    {
        queue->tail->prev = item;
        queue->tail = item;
    }

    queue->size++;

    return 1;
}

Process* dequeue(Queue* queue) 
{
    QueuedProcess* item = queue->head;
    queue->head = (queue->head)->prev;
    queue->size--;
    return item->process;
}

int isEmpty(Queue* queue) 
{
    if (queue == NULL) 
        return 0;

    if (queue->size == 0) 
        return 1;
    
    return 0;
}
