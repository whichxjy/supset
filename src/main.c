#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include "data.h"
#include "queue.h"
#include "task.h"
#include "subset.h"

#define NTHREADS 2

struct TaskQueue* task_queue;
pthread_mutex_t task_queue_lock;

struct Queue** supset;
pthread_mutex_t* supset_locks;

void* do_task(void* arg) {
    while (1) {
        // pull a task
        pthread_mutex_lock(&task_queue_lock);
        if (task_queue_is_empty(task_queue)) {
            pthread_mutex_unlock(&task_queue_lock);
            break;
        }
        struct Task task = pop_task(task_queue);
        pthread_mutex_unlock(&task_queue_lock);

        // compute result
        int i_size = sizeof(I_SETS[0]) / sizeof(I_SETS[0][0]);
        int t_size = sizeof(T_SETS[0]) / sizeof(T_SETS[0][0]);
        if (is_subset(I_SETS[task.i_index], i_size, T_SETS[task.t_index], t_size)) {
            // store result
            pthread_mutex_lock(&supset_locks[task.i_index]);
            push(supset[task.i_index], task.t_index);
            pthread_mutex_unlock(&supset_locks[task.i_index]);
        }
    }
    return NULL;
}

int main() {
    int i_sets_size = sizeof(I_SETS) / sizeof(I_SETS[0]);
    int t_sets_size = sizeof(T_SETS) / sizeof(T_SETS[0]);

    // start time
    struct timeval start;
    gettimeofday(&start, NULL);

    // create and init task queue
    task_queue = create_task_queue(i_sets_size * t_sets_size);
    for (int i = 0; i < i_sets_size; i++) {
        for (int t = 0; t < t_sets_size; t++) {
            struct Task task;
            task.i_index = i;
            task.t_index = t;
            push_task(task_queue, task);
        }
    }

    // init mutex for task queue
    if (pthread_mutex_init(&task_queue_lock, NULL) != 0) {
        printf("Fail to init mutex for task queue.\n");
        exit(1);
    }

    // create supset array
    supset = (struct Queue**)malloc(i_sets_size * sizeof(struct Queue*));
    for (int i = 0; i < i_sets_size; i++) {
        supset[i] = create_queue(t_sets_size);
    }

    // init mutexes for supset array
    supset_locks = (pthread_mutex_t*)malloc(i_sets_size * sizeof(pthread_mutex_t));
    for (int i = 0; i < i_sets_size; i++) {
        if (pthread_mutex_init(&supset_locks[i], NULL) != 0) {
            printf("Fail to init mutex for supset.\n");
            exit(1);
        }
    }

    // create threads
    pthread_t threads[NTHREADS];
    for (int i = 0; i < NTHREADS; i++) {
        int error = pthread_create(&threads[i], NULL, &do_task, NULL);
        if (error != 0) {
            printf("Fail to create thread.\n");
            exit(1);
        }
    }

    // wait threads
    for (int i = 0; i < NTHREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // end time
    struct timeval stop;
    gettimeofday(&stop, NULL);
    printf("It took %lu μs.\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);

    // write result
    FILE* output_file = fopen("output.txt", "w");
    if (output_file == NULL) {
        printf("Fail to open output file.\n");
        exit(1);
    }

    for (int i = 0; i < i_sets_size; i++) {
        fprintf(output_file, "SupSet(%d) = {", i);
        int size = queue_size(supset[i]);
        if (size > 0) {
            fprintf(output_file, "%d", pop(supset[i]));
            for (int j = 1; j < size; j++) {
                fprintf(output_file, ", %d", pop(supset[i]));
            }
        }
        fprintf(output_file, "}\n|SupSet(%d)| = %d\n", i, size);
    }

    // clean up
    destroy_task_queue(task_queue);
    for (int i = 0; i < i_sets_size; i++) {
        destroy_queue(supset[i]);
    }
    free(supset);
    free(supset_locks);
    fclose(output_file);
}