/*
Functions for initiating cache

tag, set, offset bits will be int's with other bits set to 0
*/

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cache.h"


//returns log2 of n but returns -1 if n is not a power of zero
int getBitCount(unsigned int n) {
    int loc = 0; //location of set bit(power of 2 has 1 set bit)
    if(!n) {
        return -1; //n = 0
    }

    while(n) {
        //check bits one by one
        if(n & 1) {
            break;
        }
        n >>= 1;
        loc++;
    }
    if((n | 1) != 1) {
        return -1; //more than 1 set bit
    }
    else {
        return loc;
    }
}

//make an initialized LINE array
LINE *makeSetLines(int e) {
    LINE *lines = malloc(sizeof(LINE)*e);
    memset(lines, 0, sizeof(LINE)*e);
    return lines;
}

//makes a cache to specified properties
//returns -1 if failed, 0 if success
int makeCache(CACHE *C, unsigned int sets, unsigned int blocks, unsigned int blockSize,\
 short alloc, short wBack, short policy) {
    C->s = sets;
    C->e = blocks;
    C->blockSize = blockSize;
    C->sets = malloc(sizeof(SET)*C->s);

    //check if line count(blocks) is power of 2
    if(getBitCount(C->e) < 0) {
        return -1;
    }

    //check set and blocksize
    C->setBitCount = getBitCount(C->s);
    C->offsetBitCount = getBitCount(C->blockSize); //at least 4(2^2)
    if(C->setBitCount < 0 || C->offsetBitCount < 2) {
        return -1;
    }

    //initialize sets
    for(unsigned int i = 0; i < C->s; i++) {
        C->sets[i].next = 0;
        C->sets[i].lru = malloc(sizeof(int) * C->e);
        memset(C->sets[i].lru, 0, sizeof(int)*C->e);
        C->sets[i].lines = makeSetLines(C->e);
    }

    return 0;
}

//finds set index from address
int findSetInd(unsigned int add, int setBits, int offsetBits) {
    unsigned int setInd = 1;
    //get set bit mask
    setInd <<= setBits;     //10000 <- shifted by no of set bits(example with 4)
    setInd -= 1;            //01111 <- no of set bits are 1
    setInd <<= offsetBits;  //111100 <- shifted by offset(example with 2) 

    setInd &= add; //setInd is now set bits of address with offset
    setInd >>= offsetBits; //setInd is now the set index
    return (int)setInd;
}

//finds if there is a block with matching tag in the set
//returns -1 if there is none
//shiftCount is bitCount of offset and set combined
int findLine(SET *S, unsigned int add, int e, int setBits, int offsetBits) {
    unsigned int tagComp = UINT_MAX; //all 1 bits
    tagComp <<= (setBits + offsetBits); // & with this only leaves tag bits
    for(int i = 0; i < e; i++) {
        if(S->lines[i].tag == (add & tagComp)) {
            return i;
        }
    }
    return -1; //not found
}

//updates lru
//shifts lru and puts updateline in lru[0]
//hit: if newline was already in lru(hit)
//miss: shifts all and adds updateLine
//not the most efficient code but does its job
void updateLRU(int *lru, int updateLine, int e, int hit) {
    int lruInd = e-1; //index to shift
    int i;
    //shifts from hit index(decresing)
    if(hit) {
        for(i = 0; i < e; i++) { 
            if(updateLine == lru[i]) {
                lruInd = i;
                break;
            }
        }
    }
    //shift lru
    for(i = lruInd; i > 0; i--) {
        lru[i] = lru[i-1];
    }
    lru[0] = updateLine;
}

//returns index to evict and puts evict index in lru[0]
//not the most efficient code but does its job
int evictLRU(int *lru, int e) {
    int evicted = lru[e-1];
    //shift lru
    for(int i = e-1; i > 0; i--) {
        lru[i] = lru[i-1];
    }
    lru[0] = evicted;
    return evicted;
}

//fetches block info from memory to cache
//returns 1 if replaced dirty bit(wrote to memory), else 0
int fetch(LINE *L, unsigned int add, int setBits, int offsetBits) {
    int dirty = 0;
    if (L->valid == 1) { //check dirty bit if replacing
        if (L->dirty) { //if a valid block was replaced
            dirty = 1;
        }
    }
    else {
        L->valid = 1;
    }
    
    unsigned int comp = UINT_MAX; 
    comp <<= (setBits + offsetBits);
    L->tag = add & comp;

    //also would fetch block data

    return dirty;
}

//fetches block info from memory to cache according to policy
//returns 1 if replaced dirty bit(wrote to memory), else 0
//dirty is only updated(=1) when write-back is used
int fetchData(CACHE *C, SET* set, unsigned int add, short policy) {
    int dirty;
    int next = set->next;
    //track new blocks with next and replacing next will be FIFO
    //no need to check for valid bits as next is filled in order
    if(policy == FIFO) {
        dirty = fetch(&(set->lines[next]), add, C->setBitCount, C->offsetBitCount);
        set->next = (next + 1) % C->e; //loop from 0~E(no of lines)
    }
    else if (policy == RANDOM) {
        if (next < C->e) { //there are non-valid lines left
            dirty = fetch(&(set->lines[next]), add, C->setBitCount, C->offsetBitCount);
            set->next = next + 1;
        }
        else { //fetch to random
            int randLine = rand() % C->e;
            dirty = fetch(&(set->lines[randLine]), add, C->setBitCount, C->offsetBitCount);
        }
    }
    else { //LRU
        if (next < C->e) {
            updateLRU(set->lru, next, C->e, MISS);
            dirty = fetch(&(set->lines[next]), add, C->setBitCount, C->offsetBitCount);
            set->next = next + 1;
        }
        else { //evict and fetch to evicted
            int evicted = evictLRU(set->lru, C->e);
            dirty = fetch(&(set->lines[evicted]), add, C->setBitCount, C->offsetBitCount); 
        }
    }
    return dirty;
}

//returns 1 if hit, 0 if miss
int load(CACHE *C, unsigned int add, int* cycles, short policy) {
    int setInd = findSetInd(add, C->setBitCount, C->offsetBitCount);
    SET *set = &(C->sets[setInd]);  
    int lineInd = findLine(set, add, C->e, C->setBitCount, C->offsetBitCount);
    
    //hit
    if((lineInd != -1) && set->lines[lineInd].valid) {
        if (policy == LRU) {
            updateLRU(set->lru, lineInd, C->e, HIT);
        }
        *cycles += CACHE_CYCLE;
        return HIT;
    }

    //miss
    int dirty; //flag for if diry data was replaced
    dirty = fetchData(C, set, add, policy);
    *cycles += MEM_CYCLE * (C->blockSize/4) * (1 + dirty); //1 for fetch + 1 if dirty eviction
    *cycles += CACHE_CYCLE; //read fetched from cash
    return MISS;
}

int store(CACHE *C, unsigned int add, int* cycles, short alloc, short wBack, short policy) {
    int setInd = findSetInd(add, C->setBitCount, C->offsetBitCount);
    SET *set = &(C->sets[setInd]);  
    int lineInd = findLine(set, add, C->e, C->setBitCount, C->offsetBitCount);

    //hit
    if((lineInd != -1) && set->lines[lineInd].valid) {
        if (policy == LRU) { //update lru
            updateLRU(set->lru, lineInd, C->e, HIT);
        }

        if (wBack == WRITE_BACK) {
            //function to set dirty and update cache data
            set->lines[lineInd].dirty = 1;
            *cycles += CACHE_CYCLE;
        }
        else { //write through
            //function to update cache data
            *cycles += (CACHE_CYCLE + MEM_CYCLE*(C->blockSize)/4);
        }
        return HIT;
    }

    //miss
    if (alloc == WRITE_ALLOCATE) {
        int dirty; //flag for if diry data was replaced
        dirty = fetchData(C, set, add, policy);
        *cycles += MEM_CYCLE * (C->blockSize/4) * (1 + dirty); //block is fetched
        
        *cycles += CACHE_CYCLE; //writes to cache
    } else { //no-wirte-allocate
        *cycles += MEM_CYCLE * (C->blockSize/4); //write directly to memory
    }
    
    return MISS;
}