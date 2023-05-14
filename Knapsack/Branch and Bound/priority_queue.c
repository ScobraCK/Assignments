#include <stdio.h>
#include <stdlib.h>
#include "priority_queue.h"


//compares node using bound
int compare_node(void* a, void* b) {
    int bound1 = ((Node*)a)->bound;
    int bound2 = ((Node*)b)->bound;

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

void enqueue(PriorityQueue *PQ, Node node) {
    Node *new_node = malloc(sizeof(node));
    new_node->index = node.index;
    new_node->profit = node.profit;
    new_node->weight = node.weight;
    new_node->bound = node.bound;
    new_node->include = node.include;

    PQ->heap[++PQ->size] = new_node;
    make_heap(PQ, compare_node);
}

Node *dequeue(PriorityQueue *PQ) {
    if (PQ->size == 0) {
        return NULL;
    }
    Node *node = (Node*)root(PQ, compare_node);
    return node;
}