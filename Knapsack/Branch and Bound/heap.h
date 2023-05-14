#ifndef _HEAP_

#define _HEAP_

#define TRUE 1
#define FALSE 0

typedef void* Element;
typedef struct Heap{
    Element* heap; //0th index is dummy
    int size; //does not error check max size of array
} Heap; 

void make_heap(Heap* H, int (*cmp)(Element a, Element b));
Element root(Heap *H, int (*cmp)(Element a, Element b));

#endif