#include <SDL.h> // Need for threading
#include <stdbool.h>
#include "Queue.h"

#include "global_def_macros.h"

typedef struct Q_Node_t_def
{
    void * value;
    struct Q_Node_t_def * next;
} Q_Node_t;

Queue_t * QueueCreate(size_t data_sz, int max_sz)
{
    Queue_t * q = malloc(sizeof(Queue_t));
    if (q!=NULL)
    {
        q->head = NULL;
        q->tail = NULL;
        q->len = 0;
        q->max_len = max_sz;
        q->data_sz = data_sz;
    }
    return q;
}

void QueueLock(Queue_t *q);
void QueueUnlock(Queue_t *q);

bool QueueEnqueue(Queue_t * q, void * val)
{
    bool ret = true;
    if ((q!=NULL) && (val!=NULL))
    {  
        Q_Node_t * q_node = malloc(sizeof(Q_Node_t));
        if (q_node != NULL)
        {
            q_node->value = malloc(q->data_sz);
            if (q_node->value != NULL)
            {
                q_node->next = NULL;
                memcpy(q_node->value, val, q->data_sz);
                if (q->tail == NULL) // First element in queue
                {
                    q->tail = (void*)q_node;
                    q->head = (void*)q_node;
                } else
                {
                    ((Q_Node_t*)q->tail)->next = q_node;
                    q->tail = (void*) q_node;
                }
                q->len++;
                //DBG_LOG("Enqueue: %d\n",q->len);
            } else
            {
                free(q_node);
                DBG_ERR("Failed to Create Node\n");
                ret = false;
            }
        } else
        {
            DBG_ERR("Failed to Create Node\n");
            ret = false;
        }
    } else
    {
        DBG_WARN("Null Queue or Value - Value not added\n");
        ret = false;
    }

    if (q->len > q->max_len)
    {
        free(QueueDequeue(q));
        DBG_WARN("Queue Full - Value Dequeued\n");
    }
    return ret;
}

void * QueueDequeue(Queue_t * q) // Make sure to free after dequeueing
{
    void * val = NULL;
    if (q != NULL)
    {
        Q_Node_t * q_node = ((Q_Node_t*)q->head);
        if((q_node != NULL)&&(q->head != NULL) && (q->tail != NULL))
        {
            q->head = ((void*)((Q_Node_t*)q->head)->next);

            val = q_node->value;
            free(q_node);
            q->len--;
            if (q->len == 0)
            {
                q->tail = NULL;
            }
            //DBG_LOG("Dequeue: %d\n",q->len);
        } else
        {
            DBG_WARN("Nothing Dequeued - Empty\n");
        }
    } else
    {
        DBG_WARN("Nothing Dequeued - Null Queue\n");
    }
    return val;
}

void QueueDestroy(Queue_t * q)
{
    if (q != NULL)
    {
        DBG_LOG("Start Destroy Queue: %d\n", q->len);
        int total_queue_sz = q->len;
        for (int i = 0; i <= total_queue_sz; i++)
        {
            free(QueueDequeue(q));
        }
        int len = q->len;
        free(q);
        DBG_LOG("Queue Destroyed: %d\n",len);
    }
}

void QueueLock(Queue_t *q)
{

}

void QueueUnlock(Queue_t *q)
{

}