#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

struct Queue* create_queue(unsigned capacity) {
    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
    queue->front = 0;
    queue->rear = capacity - 1;
    queue->capacity = capacity;
    queue->size = 0;
    queue->container = (int*)malloc(capacity * sizeof(int));
    return queue;
}

void destroy_queue(struct Queue* queue) {
    if (queue == NULL || queue->container == NULL) {
        return;
    }
    free(queue->container);
    queue->container = NULL;
    free(queue);
}

int queue_is_full(struct Queue* queue) {
    return queue->size == queue->capacity;
}

int queue_is_empty(struct Queue* queue) {
    return queue->size == 0;
}

int queue_size(struct Queue* queue) {
    return queue->size;
}

void push(struct Queue* queue, int item) {
    if (queue_is_full(queue)) {
        printf("The queue is full.\n");
        exit(1);
    }
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->container[queue->rear] = item;
    queue->size += 1;
}

int pop(struct Queue* queue) {
    if (queue_is_empty(queue)) {
        printf("The queue is empty.\n");
        exit(1);
    }
    int item = queue->container[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size -= 1;
    return item;
}