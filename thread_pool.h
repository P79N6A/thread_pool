/**
 * (C) 2007-2019 Alibaba Group Holding Limited
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Version: 1.0: thread_pool.h 08/03/2019 $
 *
 * Authors:
 *   qianyi.lh <qianyi.lh@alibaba-inc.com>
 */

#include <stddef.h>

typedef struct ThreadPool ThreadPool;

ThreadPool *ThreadPoolInit(size_t thread_count);
void ThreadPoolDestroy(ThreadPool *pool);
void ThreadPoolStart(ThreadPool *pool);
void ThreadPoolStop(ThreadPool *pool);
void ThreadPoolPushTask(ThreadPool *pool, void (*callback)(void *), void *args);
size_t ThreadPoolTaskCount(ThreadPool *pool);

