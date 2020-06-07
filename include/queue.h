#ifndef _QUEUE_H_
#define _QUEUE_H_

struct Queue {
    unsigned front;
    unsigned rear;
    unsigned capacity;
    unsigned size;
    int* container;
};

struct Queue* create_queue(unsigned capacity);
void destroy_queue(struct Queue* queue);
int queue_is_full(struct Queue* queue);
int queue_is_empty(struct Queue* queue);
int queue_size(struct Queue* queue);
void push(struct Queue* queue, int item);
int pop(struct Queue* queue);

#endif