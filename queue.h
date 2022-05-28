#ifndef QUEUE_H

    #define QUEUE_H

    typedef struct _Queue
    {
        int max;
        Process* items[];
        int rear;
        int front;
    } Queue;

    Queue* create_queue(int max);
    void free_queue(Queue* queue);
    void enqueue(Queue* queue, int item);
    Queue* dequeue(Queue* queue);

#endif