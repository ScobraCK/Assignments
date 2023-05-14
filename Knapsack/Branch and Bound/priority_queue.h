#ifndef _PRIORITY_Q_

#define _PRIORITY_Q_

#include "heap.h"
#include "knapsack_bnb.h"

typedef Heap PriorityQueue;

PriorityQueue *make_queue(int n);
void delete_queue(PriorityQueue *PQ);
void enqueue(PriorityQueue *PQ, Node node);
Node *dequeue(PriorityQueue *PQ);

#endif