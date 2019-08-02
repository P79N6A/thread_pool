/**
 * (C) 2007-2019 Alibaba Group Holding Limited
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Version: 1.0: main.c 08/03/2019 $
 *
 * Authors:
 *   qianyi.lh <qianyi.lh@alibaba-inc.com>
 */


#include <stdio.h>
#include <stdlib.h>

#include "thread_pool.h"

void sum(void *args) {
    int i, count = 0;
    for (i = 1; i <= 100; ++i) {
        count += i;
    }
    printf("%d\n", count);
}

int main(int argc, char *argv[])
{
    ThreadPool *pool = ThreadPoolInit(4);
    ThreadPoolStart(pool);
    ThreadPoolPushTask(pool, sum, NULL);
    ThreadPoolPushTask(pool, sum, NULL);
    ThreadPoolPushTask(pool, sum, NULL);
    ThreadPoolPushTask(pool, sum, NULL);
    ThreadPoolPushTask(pool, sum, NULL);
    sleep(1);
    ThreadPoolStop(pool);
    ThreadPoolDestroy(pool);

    return EXIT_SUCCESS;
}

