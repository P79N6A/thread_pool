/**
 * (C) 2007-2019 Alibaba Group Holding Limited
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Version: 1.0: blocking_queue.h 08/03/2019 $
 *
 * Authors:
 *   qianyi.lh <qianyi.lh@alibaba-inc.com>
 */

#include "queue.h"

typedef struct BlockingQueue BlockingQueue;

BlockingQueue *BlockingQueueInit(void);
void BlockingQueueDestroy(BlockingQueue *queue, void (*dataDestroy)(void *));
void BlockingQueuePush(BlockingQueue *queue, void *data);
void *BlockingQueuePop(BlockingQueue *queue);
size_t BlockingQueueSize(BlockingQueue *queue);

