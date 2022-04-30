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

void DisplayList(Queue_Info *Q);

Qnode *search(Queue_Info *Q, int data);

void Enqueue(Queue_Info *Q, Qnode *p);

Qnode *Dequeue(Queue_Info *Q);

Qnode *Pop(Queue_Info *S);


#endif