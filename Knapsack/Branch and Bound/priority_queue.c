#include <stdio.h>
#include <stdlib.h>
#include "priority_queue.h"


//compares node using bound
int compare_node(void* a, void* b) {
    double bound1 = ((Node*)a)->bound;
    double bound2 = ((Node*)b)->bound;

    if (bound1 > bound2) {
        return -1;
    } else if (bound1 < bound2) {
        return 1;
    }
    return 0; 
}

PriorityQueue *make_queue(int n) {
    PriorityQueue *PQ = malloc(sizeof(PriorityQueue));
    PQ->heap = malloc(sizeof(Element*) * n);  //can be smaller than n
    PQ->size = 0;
    return PQ;
}

void delete_queue(PriorityQueue *PQ) {
    free(PQ);
}

void enqueue(PriorityQueue *PQ, Node *node) {
    PQ->size++;
    PQ->heap[PQ->size] = node;
    make_heap(PQ, compare_node);
}

Node *dequeue(PriorityQueue *PQ) {
    if (PQ->size == 0) {
        return NULL;
    }
    Node *node = (Node*)root(PQ, compare_node);
    return node;
}