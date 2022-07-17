#include <stdlib.h>
#include <stdio.h>
#include "queue.h"


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
    while (!isEmpty(queue)) 
    {
        dequeue(queue);
    }

    free(queue);
}

int enqueue(Queue* queue, int process) 
{
    if (queue->size >= queue->max)
        return 0;

    removeFromQueue(queue, process);

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

int peek(Queue* queue) 
{
    if (isEmpty(queue))
    {
        printf("Queue is empty\n");
        return -1;
    }

    return queue->head->process;
}

int dequeue(Queue* queue) 
{
    if (isEmpty(queue))
    {
        printf("Queue is empty\n");
        return -1;
    }
    QueuedProcess* item = queue->head;
    queue->head = (queue->head)->prev;
    queue->size--;
    int process = item->process;
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

void removeFromQueue(Queue* queue, int value)
{
    QueuedProcess* item = queue->head;
    QueuedProcess* last = item;

    while(item != NULL)
    {
        if(item->process == value)
        {
            if(item == queue->head)
            {
                queue->head = item->prev;
            }

            if(item == queue->tail)
            {
                queue->tail = last;
            }

            last->prev = item->prev;

            queue->size--;
        }

        last = item;
        item = item->prev;
    }
}