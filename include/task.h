#ifndef _TASK_H_
#define _TASK_H_

struct Task {
    int i_index;
    int t_index;
};

struct TaskQueue {
    unsigned front;
    unsigned rear;
    unsigned capacity;
    unsigned size;
    struct Task* container;
};

struct TaskQueue* create_task_queue(unsigned capacity);
void destroy_task_queue(struct TaskQueue* queue);
int task_queue_is_full(struct TaskQueue* queue);
int task_queue_is_empty(struct TaskQueue* queue);
void push_task(struct TaskQueue* queue, struct Task item);
struct Task pop_task(struct TaskQueue* queue);

#endif