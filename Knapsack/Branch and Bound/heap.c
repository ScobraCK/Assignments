#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

void siftdown(Heap* H, int index, int (*cmp)(Element a, Element b)) {
    Element *heap = H->heap;
    int parent, larger_child;
    Element node = heap[index];
    int spot_found = FALSE;
    int size = H->size;

    parent = index;
    while(2*parent <= size && !spot_found) {
        if (2*parent < size && (cmp(heap[2*parent], heap[2*parent+1]) > 0)) {
            larger_child = 2*parent+1;
        }
        else {
            larger_child = 2*parent;
        }

        if (cmp(node, heap[larger_child]) > 0) {
            heap[parent] = heap[larger_child];
            parent = larger_child;
        }
        else {
            spot_found = TRUE;
        }
    }
    heap[parent] = node;
}

//construct a heap from Element*
void make_heap(Heap* H, int (*cmp)(Element a, Element b)) {
    for (int i = H->size/2; i>=1; i--) {
        siftdown(H, i, cmp);
    }
}

Element root(Heap *H, int (*cmp)(Element a, Element b)) {
    Element e = H->heap[1];
    H->heap[1] = H->heap[H->size--];
    siftdown(H, 1, cmp);
    return e;
}