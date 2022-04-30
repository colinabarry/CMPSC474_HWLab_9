#include "Queue.h"
#include "Util.h"
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// #define DEBUG
#pragma region /* DEBUG_PRINT macro */
#ifdef DEBUG
#define DEBUG_PRINT printf
#else
#define DEBUG_PRINT(...)
#endif
#pragma endregion

#define SHM_NAME "calculated_data"


/* declare thread functions */
void *user_input(void *arg);
void *add_processing_queue(void *arg);
void *process_nodes(void *arg);
void *send_to_smem(void *arg);

int _get_input();

/* declare mutex locks */
pthread_mutex_t lock_input;
pthread_mutex_t lock_plist;
pthread_mutex_t lock_slist;
/* declare semaphores */
sem_t input_ready;
sem_t input_taken;
sem_t empty_pslots;
sem_t filled_pslots;
sem_t empty_sslots;
sem_t filled_sslots;
/* declare global ints */
int input;
int end_time = 1;
/* declare queues */
Queue_Info processing_queue;
Queue_Info sending_queue;


int main()
{
    /* declare pthreads */
    pthread_t pt_user_input;
    pthread_t pt_add_processing_queue;
    pthread_t pt_process_nodes;
    pthread_t pt_send_to_smem;
    /* declare res - to be used for error checking */
    int res;

    /* initialize processing queue */
    processing_queue.first = NULL;
    processing_queue.last = NULL;
    processing_queue.queue = NULL;
    /* initialize sending queue */
    sending_queue.first = NULL;
    sending_queue.last = NULL;
    sending_queue.queue = NULL;


    /* initialize the three required mutex locks and check for errors */
    res = pthread_mutex_init(&lock_input, NULL);
    check_error(res, "pthread_mutex_init lock_input");
    res = pthread_mutex_init(&lock_plist, NULL);
    check_error(res, "pthread_mutex_init lock_plist");
    res = pthread_mutex_init(&lock_slist, NULL);
    check_error(res, "pthread_mutex_init lock_slist");


    /* initialize the six required semaphores and check for errors */
    res = sem_init(&input_ready, 0, 0);
    check_error(res, "sem_init input_ready");
    res = sem_init(&input_taken, 0, 0);
    check_error(res, "sem_init input_taken");
    res = sem_init(&empty_pslots, 0, 5);
    check_error(res, "sem_init empty_pslots");
    res = sem_init(&filled_pslots, 0, 0);
    check_error(res, "sem_init filled_pslots");
    res = sem_init(&empty_sslots, 0, 5);
    check_error(res, "sem_init empty_sslots");
    res = sem_init(&filled_sslots, 0, 0);
    check_error(res, "sem_init filled_sslots");


    /* create the four required threads and check for errors */
    res = pthread_create(&pt_user_input, NULL, user_input, NULL);
    check_error(res, "pthread_create user_input");
    res = pthread_create(&pt_add_processing_queue, NULL, add_processing_queue,
                         NULL);
    check_error(res, "pthread_create add_processing_queue");
    res = pthread_create(&pt_process_nodes, NULL, process_nodes, NULL);
    check_error(res, "pthread_create process_nodes");
    res = pthread_create(&pt_send_to_smem, NULL, send_to_smem, NULL);
    check_error(res, "pthread_create send_to_smem");

    /* join threads and exit the program*/
    res = pthread_join(pt_user_input, NULL);
    check_error(res, "pthread_join user_input");

    exit(EXIT_SUCCESS);
}


void *user_input(void *arg)
{
    int res;
    end_time = 1;
    while (_get_input()) {
        res = sem_post(&input_ready);
        check_error(res, "sem_post input_ready");
        res = sem_wait(&input_taken);
        check_error(res, "sem_wait input_taken");
    }
    end_time = -1;
    pthread_exit(NULL);
}


int _get_input()
{
    /* declare res - to be used to check for input of -1 */
    int res;
    /* prompt the user for input */
    printf("Enter a temperature reading in C: ");
    /* lock input */
    pthread_mutex_lock(&lock_input);
    fscanf(stdin, "%d", &input);
    /* if the user enters -1, return false, otherwise return true */
    if (input == -1) res = 0;
    else res = 1;
    /* unlock input */
    pthread_mutex_unlock(&lock_input);
    return res;
}


void *add_processing_queue(void *arg)
{
    while (end_time != -1) {
        sem_wait(&input_ready);
        sem_wait(&empty_pslots);

        int new_input;
        pthread_mutex_lock(&lock_input);
        new_input = input;
        pthread_mutex_unlock(&lock_input);

        pthread_mutex_lock(&lock_plist);
        Qnode *new_node = (Qnode *)malloc(sizeof(Qnode));
        new_node->data = new_input;
        enqueue(&processing_queue, new_node);
        display_list(&processing_queue);
        pthread_mutex_unlock(&lock_plist);

        sem_post(&input_taken);
        sem_post(&filled_pslots);
    }
    pthread_exit(NULL);
}


void *process_nodes(void *arg)
{
    while (end_time != -1) {
        sem_wait(&filled_pslots);

        Qnode *new_node;
        pthread_mutex_lock(&lock_plist);
        new_node = dequeue(&processing_queue);
        pthread_mutex_unlock(&lock_plist);

        sem_post(&empty_pslots);
    }
}


void *send_to_smem(void *arg)
{
}
