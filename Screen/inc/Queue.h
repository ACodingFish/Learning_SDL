#ifndef _QUEUE_H_
#define _QUEUE_H_

//https://codereview.stackexchange.com/questions/141238/implementing-a-generic-queue-in-c

// Attempt at a generic (and eventually thread-safe) queue

typedef struct Queue_t_def
{
    void *head;
    void *tail;
    int len; // length of queue
    int max_len; // max length of queue

    size_t data_sz; // size of data in each node
    void* mutex; // to implement with threading
} Queue_t;

Queue_t * QueueCreate(size_t data_sz, int max_sz);
bool QueueEnqueue(Queue_t * q, void * val);
void * QueueDequeue(Queue_t * q);
void QueueDestroy(Queue_t * q);


#endif //_QUEUE_H_