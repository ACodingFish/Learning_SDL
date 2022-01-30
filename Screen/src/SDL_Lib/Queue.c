#include <SDL.h> // Need for threading
#include <stdbool.h>
#include "Queue.h"

#include "global_def_macros.h"

typedef struct Q_Node_t_def
{
    void * value;
    struct Q_Node_t_def * next;
} Q_Node_t;

void QueueLock(Queue_t *q);
void QueueUnlock(Queue_t *q);
bool QueueTryLock(Queue_t *q);


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

        q->mutex = (void*)SDL_CreateMutex();
        if (q->mutex == NULL)
        {
            free(q);
            q = NULL;
            DBG_ERR("Queue not made - Mutex Fail\n");
        }
    } else
    {
        DBG_ERR("Queue not made - Malloc Fail\n");
    }
    return q;
}

bool QueueEnqueue(Queue_t * q, void * val)
{
    QueueLock(q);
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
                q_node = NULL;
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
    QueueUnlock(q);

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
    if (QueueTryLock(q) != false)
    {
        if (q != NULL)
        {
            Q_Node_t * q_node = ((Q_Node_t*)q->head);
            if((q_node != NULL)&&(q->head != NULL) && (q->tail != NULL))
            {
                q->head = ((void*)((Q_Node_t*)q->head)->next);

                val = q_node->value;
                free(q_node);
                q_node = NULL;
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
        QueueUnlock(q);
    }
    return val;
}

void QueueDestroy(Queue_t * q)
{
    if (q != NULL)
    {
        DBG_LOG("Start Destroy Queue: %d\n", q->len);
        int total_queue_sz = q->len;
        
        void * q_item = NULL;
        do
        {
            q_item = QueueDequeue(q);
            free(q_item);
        } while (q_item != NULL);

        total_queue_sz = q->len;

        if (q->mutex != NULL)
        {
            SDL_DestroyMutex(q->mutex);
            q->mutex = NULL;
        }
        free(q);
        q = NULL; // this does nothing really -> Won't affect original pointer
        DBG_LOG("Queue Destroyed: %d\n",total_queue_sz);
    }
}

void QueueLock(Queue_t *q)
{
    if ((q != NULL) && (q->mutex != NULL))
    {
        SDL_LockMutex((SDL_mutex*)q->mutex);
    }
}

bool QueueTryLock(Queue_t *q)
{
    bool ret = false;
    if ((q != NULL) && (q->mutex != NULL))
    {
       ret = (SDL_TryLockMutex((SDL_mutex*)q->mutex) == 0);
    }
    return ret;
}

void QueueUnlock(Queue_t *q)
{
    if ((q != NULL) && (q->mutex != NULL))
    {
        SDL_UnlockMutex((SDL_mutex*)q->mutex);
    }
}