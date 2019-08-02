/**
 * (C) 2007-2019 Alibaba Group Holding Limited
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Version: 1.0: blocking_queue.c 08/03/2019 $
 *
 * Authors:
 *   qianyi.lh <qianyi.lh@alibaba-inc.com>
 */

#include "blocking_queue.h"

#include <stdlib.h>
#include <pthread.h>

typedef struct BlockingQueue {
    Queue *queue;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} BlockingQueue;

BlockingQueue *BlockingQueueInit(void)
{
    BlockingQueue *queue = (BlockingQueue *)malloc(sizeof(BlockingQueue));
    queue->queue = QueueInit();
    pthread_mutex_init(&queue->mutex, NULL);
    pthread_cond_init(&queue->cond, NULL);

    return queue;
}

void BlockingQueueDestroy(BlockingQueue *queue, void (*dataDestroy)(void *))
{
    QueueDestroy(queue->queue, dataDestroy);
    pthread_mutex_destroy(&queue->mutex);
    pthread_cond_destroy(&queue->cond);
}

void BlockingQueuePush(BlockingQueue *queue, void *data)
{
    pthread_mutex_lock(&queue->mutex);
    QueuePush(queue->queue, data);
    pthread_cond_signal(&queue->cond);
    pthread_mutex_unlock(&queue->mutex);
}

void *BlockingQueuePop(BlockingQueue *queue)
{
    pthread_mutex_lock(&queue->mutex);
    while (QueueSize(queue->queue) < 1) {
        pthread_cond_wait(&queue->cond, &queue->mutex);
    }
    void *data = QueuePop(queue->queue);
    pthread_mutex_unlock(&queue->mutex);

    return data;
}

size_t BlockingQueueSize(BlockingQueue *queue)
{
    pthread_mutex_lock(&queue->mutex);
    size_t size = QueueSize(queue->queue);
    pthread_mutex_unlock(&queue->mutex);

    return size;
}

