#include <stdio.h>
#include <stdlib.h>
#include "task.h"

struct TaskQueue* create_task_queue(unsigned capacity) {
    struct TaskQueue* queue = (struct TaskQueue*)malloc(sizeof(struct TaskQueue));
    queue->front = 0;
    queue->rear = capacity - 1;
    queue->capacity = capacity;
    queue->size = 0;
    queue->container = (struct Task*)malloc(capacity * sizeof(struct Task));
    return queue;
}

void destroy_task_queue(struct TaskQueue* queue) {
    if (queue == NULL || queue->container == NULL) {
        return;
    }
    free(queue->container);
    queue->container = NULL;
    free(queue);
}

int task_queue_is_full(struct TaskQueue* queue) {
    return queue->size == queue->capacity;
}

int task_queue_is_empty(struct TaskQueue* queue) {
    return queue->size == 0;
}

void push_task(struct TaskQueue* queue, struct Task item) {
    if (task_queue_is_full(queue)) {
        printf("The queue is full.\n");
        exit(1);
    }
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->container[queue->rear] = item;
    queue->size += 1;
}

struct Task pop_task(struct TaskQueue* queue) {
    if (task_queue_is_empty(queue)) {
        printf("The queue is empty.\n");
        exit(1);
    }
    struct Task item = queue->container[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size -= 1;
    return item;
}