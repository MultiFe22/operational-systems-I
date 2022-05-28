#ifndef QUEUE_H

    #define QUEUE_H

    typedef struct _Queue
    {
        int max;
        int* items;
        int rear;
        int front;
    } Queue;

    Queue* create_queue(int max);
    void free_queue(Queue* queue);
    void enqueue(Queue* queue, int item);
    int dequeue(Queue* queue);

#endif