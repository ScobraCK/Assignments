# Cache Simulator
Simple cache simulator for a school assignment.

## Features(requirements)
* store/load from 32bit memory(hex)
* write-allocate or no-write-allocate
* write-through or write-back
* lru (least-recently-used), fifo, or random evictions

Assume that loads/stores from/to the cache take one processor cycle. 
Loads/stores from/to memory take 100 processor cycles for each 4-byte quantity that is transferred . 

## Inputs
* number of sets in the cache (a positive power-of-2)
* number of blocks in each set (a positive power-of-2)
* number of bytes in each block (a positive power-of-2, at least 4)
* write-allocate or no-write-allocate
* write-through or write-back
* lru (least-recently-used), fifo, or random evictions

## Output
* Total loads
* Total stores
* Load hits
* Load misses
* Store hits
* Store misses
* Total cycles

## Useage Example
For a cache with 256 sets of 4 blocks each with 16 bytes of memory per block.

```./csim 256 4 16 write-allocate write-back lru < sometracefile```

Trace file example (3rd token is ignored)
```
s 0x1fffff78 1
l 0x1fffff80 1
l 0x1fffff98 6
l 0x1fffffa0 2
l 0x1fffffa8 2
```
