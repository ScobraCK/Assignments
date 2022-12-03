#ifndef _CACHE_

#define _CACHE_

//macros for cache write options
#define NO_WRITE_ALLOCATE 0
#define WRITE_ALLOCATE 1
#define WRITE_THROUGH 0
#define WRITE_BACK 1
#define LRU 0
#define FIFO 1
#define RANDOM 2

#define CACHE_CYCLE 1
#define MEM_CYCLE 100

#define HIT 1
#define MISS 0

typedef struct LINE {
    short valid;
    short dirty;
    unsigned int tag; //tag bits
    int data; //unused dummy var for block data
} LINE;

typedef struct SET {
    int next; //line index after last cached block
    int *lru; //array of line index for lru-cache(lower index = recently used)
    LINE* lines;
} SET;

typedef struct CACHE {
    int s; //no of sets
    int e; //no of lines(blocks)
    SET* sets;
    int setBitCount;
    int offsetBitCount;
} CACHE;

int makeCache(CACHE *, unsigned int , unsigned int , unsigned int ,short , short , short );
int load(CACHE *C, unsigned int add, int* cycles, short policy);

#endif