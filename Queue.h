#ifndef QUEUE_H
#define QUEUE_H


typedef struct Qnode {
    int data;
    struct Qnode *next;
} Qnode;

typedef struct Queue_Info {
    Qnode *first;
    Qnode *last;
    Qnode *queue;
} Queue_Info;

void display_list(Queue_Info *Q);

Qnode *search(Queue_Info *Q, int data);

void enqueue(Queue_Info *Q, Qnode *p);

Qnode *dequeue(Queue_Info *Q);

Qnode *pop(Queue_Info *S);


#endif