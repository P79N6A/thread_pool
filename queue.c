/**
 * (C) 2007-2019 Alibaba Group Holding Limited
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Version: 1.0: queue.c 08/03/2019 $
 *
 * Authors:
 *   qianyi.lh <qianyi.lh@alibaba-inc.com>
 */

#include "queue.h"

#include <stdlib.h>


typedef struct Node {
    void *data;
    struct Node *next;
} Node;

typedef struct Queue {
    Node *head;
    Node *tail;
    size_t size;
} Queue;

Queue *QueueInit(void)
{
    Queue *queue = (Queue *)malloc(sizeof(Queue));
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;
    return queue;
}

void QueueDestroy(Queue *queue, void (*dataDestroy)(void *))
{
    Node *node = queue->head;
    while (node != NULL) {
        Node *tmp = node->next;
        if (dataDestroy) {
            dataDestroy(node->data);
        }
        free(node);
        node = tmp;
    }
    free(queue);
}

void QueuePush(Queue *queue, void *data)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->data = data;
    if (queue->tail) {
        queue->tail->next = node;
        queue->tail = node;
    } else {
        queue->head = queue->tail = node;
    }
    queue->size++;
}

void *QueuePop(Queue *queue)
{
    void *data = NULL;
    if (queue->size == 1) {
        data = queue->head->data;
        free(queue->head);
        queue->head = queue->tail = NULL;
        queue->size--;
    } else if (queue->size > 1) {
        data = queue->head->data;
        Node *tmp = queue->head;
        queue->head = queue->head->next;
        free(tmp);
        queue->size--;
    }

    return data;
}

size_t QueueSize(Queue *queue)
{
    return queue->size;
}

