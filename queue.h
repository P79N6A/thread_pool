/**
 * (C) 2007-2019 Alibaba Group Holding Limited
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Version: 1.0: queue.h 08/03/2019 $
 *
 * Authors:
 *   qianyi.lh <qianyi.lh@alibaba-inc.com>
 */

#include <stddef.h>

typedef struct Queue Queue;

Queue *QueueInit(void);
void QueueDestroy(Queue *queue, void (*dataDestroy)(void *));
void QueuePush(Queue *queue, void *data);
void *QueuePop(Queue *queue);
size_t QueueSize(Queue *queue);

