/*
Cache simulator for a school assignment.
Only hits and misses and cycles will be concerned and no actual data will be loaded/stored.

Cache: 1 cycle
Memory: 100 cycles

Simulator will have the following features
-store/load from 32bit memory
-write-allocate or no-write-allocate
-write-through or write-back
-lru (least-recently-used), fifo, or random evictions

reading of file ends when a line is just "\n" of EOF
"\n" is for comments at the bottom of trace file. Also can split trace file with '\n'
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cache.h"

int main(int argc, char const *argv[]) {
    unsigned int sets, blocks, blockSize;
    short writeAlloc, writeBack, replacePolicy;

    int loads, stores, ldHit, ldMiss, stHit, stMiss, cycles;
    loads = stores = ldHit = ldMiss = stHit = stMiss = cycles = 0;

    if (argc != 7) {
        printf("Wrong usage\n"); //maybe add proper useage example
        return 0;
    }

    //read input
    sets = (unsigned int) strtoul(argv[1], NULL, 0);
    blocks = (unsigned int) strtoul(argv[2], NULL, 0);
    blockSize = (unsigned int) strtoul(argv[3], NULL, 0);

    if(strcmp("write-allocate", argv[4]) == 0) {
        writeAlloc = WRITE_ALLOCATE;
    }
    else {
        writeAlloc = NO_WRITE_ALLOCATE;
    }

    if(strcmp("write-back", argv[5]) == 0) {
        writeBack = WRITE_BACK;
    }
    else {
        writeBack = WRITE_THROUGH;
    }

    if (strcmp("lru", argv[6]) == 0) {
        replacePolicy = LRU;
    }
    else if (strcmp("fifo", argv[6]) == 0){
        replacePolicy = FIFO;
    }
    else {
        replacePolicy = RANDOM;
    }

    //make cache
    CACHE C;
    if(makeCache(&C, sets, blocks, blockSize, writeAlloc, writeBack, replacePolicy) < 0) {
        printf("Failed to make cache\n");
        return 1;
    }

    char trace[20];
    unsigned int add;
    int hit;
    while(1) {
        fgets(trace, 20, stdin);
        if (strcmp(trace, "\n")==0) {
            break;
        }
        //could tokenize trace but direct access is more convinient for this case
        add = (unsigned int) strtoul(trace+2, NULL, 0); //add is trace starting with 2 offset
        //loads
        if (trace[0] == 'l') { 
            hit = load(&C, add, &cycles, replacePolicy);
            if(hit) {
                ldHit++;
            }
            else {
                ldMiss++;
            }
            loads++;
        }
        else {
            hit = store(&C, add, &cycles, writeAlloc, writeBack, replacePolicy);
            if(hit) {
                stHit++;
            }
            else {
                stMiss++;
            }
            stores++;
        }
        strcpy(trace, "\n"); //reset trace(in case of EOF)
    }

    //store leftover dirty bits?
    int count=0;
    for(int i = 0; i<C.s; i++) {
        for(int j = 0; j<C.e; j++) {
            if(C.sets[i].lines[j].dirty == 1) {
                count++;
            }
        }
    }
    cycles += MEM_CYCLE*(C.blockSize/4)*count;

    printf("Total loads: %d\nTotal stores: %d\n", loads, stores);
    printf("Loads hits: %d\nLoad misses: %d\n", ldHit, ldMiss);
    printf("Store hits: %d\nStore misses: %d\n", stHit, stMiss);
    printf("Total cycles: %d\n", cycles);
    return 0;
}
