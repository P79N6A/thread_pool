/**
 * (C) 2007-2019 Alibaba Group Holding Limited
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Version: 1.0: thread_pool.c 08/03/2019 $
 *
 * Authors:
 *   qianyi.lh <qianyi.lh@alibaba-inc.com>
 */

#include "thread_pool.h"

#include <pthread.h>
#include <stdlib.h>

#include "queue.h"


typedef struct ThreadPool {
    Queue *queue;
    pthread_t *threads;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    size_t thread_count;
    int is_start;
} ThreadPool;

typedef struct Task {
    void (*callback)(void *);
    void *args;
} Task;

void *threadStart(void *args) {
    ThreadPool *pool = (ThreadPool *)args;
    while (pool->is_start) {
        pthread_mutex_lock(&pool->mutex);
        while (QueueSize(pool->queue) < 1 && pool->is_start) {
            pthread_cond_wait(&pool->cond, &pool->mutex);
        }
        Task *task = (Task *)QueuePop(pool->queue);
        pthread_mutex_unlock(&pool->mutex);
        if (task && task->callback) {
            task->callback(task->args);
            free(task);
        }
    }
    return NULL;
}

ThreadPool *ThreadPoolInit(size_t thread_count)
{
    ThreadPool *pool = (ThreadPool *)malloc(sizeof(ThreadPool));
    pool->queue = QueueInit();
    pool->thread_count = thread_count;
    pool->threads = (pthread_t *)malloc(sizeof(pthread_t) * thread_count);
    pthread_mutex_init(&pool->mutex, NULL);
    pthread_cond_init(&pool->cond, NULL);
    pool->is_start = 0;
}

void ThreadPoolDestroy(ThreadPool *pool)
{
    ThreadPoolStop(pool);
    pthread_cond_broadcast(&pool->cond);
    int i;
    for (i = 0; i < pool->thread_count; ++i) {
        pthread_join(pool->threads[i], NULL);
    }
    QueueDestroy(pool->queue, free);
    pthread_mutex_destroy(&pool->mutex);
    pthread_cond_destroy(&pool->cond);
}

void ThreadPoolStart(ThreadPool *pool)
{
    if (pool->is_start) {
        return;
    }
    pool->is_start = 1;
    int i;
    for (i = 0; i < pool->thread_count; ++i) {
        pthread_create(&(pool->threads[i]), NULL, threadStart, (void *)pool);
    }
}

void ThreadPoolStop(ThreadPool *pool)
{
    pool->is_start = 0;
}

void ThreadPoolPushTask(ThreadPool *pool, void (*callback)(void *), void *args)
{
    Task *task = (Task *)malloc(sizeof(Task));
    task->callback = callback;
    task->args = args;
    pthread_mutex_lock(&pool->mutex);
    QueuePush(pool->queue, task);
    pthread_mutex_unlock(&pool->mutex);
    pthread_cond_signal(&pool->cond);
}

size_t ThreadPoolTaskCount(ThreadPool *pool)
{
    pthread_mutex_lock(&pool->mutex);
    size_t size = QueueSize(pool->queue);
    pthread_mutex_unlock(&pool->mutex);

    return size;
}

