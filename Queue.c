#include "Queue.h"
#include <stdio.h>
#include <stdlib.h>

#define DEBUG
#ifdef DEBUG
#define DEBUG_PRINT printf
#else
#define DEBUG_PRINT(...)
#endif

// Search function
//     Searches all queue to find the node with the value.
//     Returns a pointer with the node.

// Enqueue function
//     Add a node into the end of the queue.

// Dequeue function
//     Remove a node from the end of the queue.
//  int main(){

//      int x;
//      Qnode * tmp;
//      Queue_Info QH;
//      QH.first = NULL;
//      QH.Last = NULL;
//      QH.Queue = NULL;

//      do{
//          printf("Enter A number : \n");
//          scanf("%d",&x);
//          tmp = (Qnode *) malloc(sizeof(Qnode));
//          tmp->data = x;
//          Enqueue(&QH,tmp);

//      }while(x!=-1);

//      DisplayList(&QH);

//      while(QH.first != NULL){
//           printf("______________________________\n");
//          Dequeue(&QH);
//           printf("_____________**________________\n");
//          DisplayList(&QH);
//          printf("______________________________\n");
//      }

//  }

void DisplayList(Queue_Info *Q)
{
    Qnode *t;

    if (Q == NULL) {
        perror("Queue Header is Null");
    }
    t = Q->first;
    while (t != NULL) {
        DEBUG_PRINT(" Data %d \n", t->data);
        t = t->next;
    }
}

Qnode *search(Queue_Info *Q, int data)
{
    if (Q == NULL) {
        perror("Queue Header is Null");
    }

    if (Q->queue == NULL) {
        perror("Queue is empty");
    }

    Qnode *ptr;
    ptr = Q->queue;

    while (ptr != NULL) {
        if (ptr->data == data) {
            return ptr;
        }
        ptr = ptr->next;
    }

    return NULL;
}

void Enqueue(Queue_Info *Q, Qnode *p)
{
    if (p == NULL) {
        perror("No node to add ");
    }

    if (Q->first == NULL) {
        Q->queue = p;
        Q->first = p;
        Q->last = p;
    } else {
        p->next = NULL;
        Q->last->next = p;
        Q->last = p;
    }
    return;
}

Qnode *Dequeue(Queue_Info *Q)
{
    if (Q == NULL) {
        perror("No Queue to search");
    }

    if (Q->first == NULL || Q->last == NULL || Q->queue == NULL) {
        perror("Empty Queue, Nothing to remove");
        return NULL;
    }

    Qnode *p = Q->first;

    Q->first = Q->first->next;
    Q->queue = Q->first;

    return p;
}

Qnode *Pop(Queue_Info *S)
{
    if (S == NULL) {
        perror("No Stack to Pop");
    }

    if (S->first == NULL) {
        perror("Empty Queue, Nothing to remove");
        return NULL;
    }

    return Dequeue(S);
}