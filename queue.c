#include <stdlib.h>
#include <stdio.h>
#include "queue.h"
#include "process.h"


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
    //QueuedProcess* item;

    while (!isEmpty(queue)) 
    {
        dequeue(queue);
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

// QueuedProcess* dequeueInternal(Queue* queue) 
// {
//     QueuedProcess* item = queue->head;
//     queue->head = (queue->head)->prev;
//     queue->size--;
//     return item;
// }

Process* peek(Queue* queue) 
{
    if (isEmpty(queue))
    {
        printf("Queue is empty\n");
        return NULL;
    }

    return queue->head->process;
}

Process* dequeue(Queue* queue) 
{
    if (isEmpty(queue))
    {
        printf("Queue is empty\n");
        return NULL;
    }
    QueuedProcess* item = queue->head;
    queue->head = (queue->head)->prev;
    queue->size--;
    Process* process = item->process;
    free(item);
    return process;
}

int isEmpty(Queue* queue) 
{
    if (queue == NULL) 
        return 0;

    if (queue->size == 0) 
        return 1;
    
    return 0;
}

void foreach(Queue* queue, void (*func)(Process*))
{
    Process* process = peek(queue);
    while(process != NULL)
    {
        (*func)(process);
        process = peek(queue);
    }
}