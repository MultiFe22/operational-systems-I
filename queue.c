#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

Queue* createQueue(int max)
{
    Queue *queue = (Queue*) malloc(sizeof (Queue));

    int* items = (int*) malloc(sizeof(int) * max);
    int rear = -1;
    int front = -1;

    queue->max = max;
    queue->items = items;
    queue->rear = rear;
    queue->front = front;
}

void freeQueue(Queue* queue)
{
    free(queue->items);
    free(queue);
}

void enqueue(Queue* queue, int item)
{
    if(queue->rear == queue->max - 1)
    {
        printf("Queue Overflow");
        return;
    }
    
    if(queue->front == - 1)
        queue->front = 0;
            
    queue->rear++;
    queue->items[queue->rear] = item;
}

int dequeue(Queue* queue)
{
    if(queue->front == - 1 || queue->front > queue->rear)
    {
        printf("Queue Underflow");
        return -1;
    }
    
    int element = queue->items[queue->front];
    queue->front++;
    return element;
}

int main()
{
    Queue* q = createQueue(10);
    
    enqueue(q, 0);
    dequeue(q);
    freeQueue(q);
    
}