# 0/1 Knapsack Problem - Branch and bound
Algorithm HW 3
<br>Solution for the 0/1 knapsack problem using branch & bound.

## Problem description
- Given 𝑛 items and a "knapsack.”
- Item 𝑖 has weight 𝑤<sub>𝑖</sub> > 0 and has value 𝑣<sub>𝑖</sub> > 0.
- Knapsack has capacity of 𝑊.
- Goal: Fill knapsack so as to maximize total value.


## Algorithm
The main algorithm is essentially the same as the backtracking version. (see https://github.com/ScobraCK/Assignments/tree/main/Knapsack/Backtracking)
<br>The structure of the code will also be kept the same.

Only difference is instead of expanding the state space tree as a DFS, we expand it as a best fit search. The *best* node being the node with highest bound. Thus, we be using a priority queue(heap). We will pull from the queue until the bound of the next item in the heap is lower than the profit of the best node.

The bound calculation is the same as with backtracking, however, instead of using a promising function we will be keeping track of the bound in the Node struct. A heap of these nodes will be constructed with the comparing function using the bound as the key.
In case the node is overweight, the bound will be set to -1 to indicate so.
```c
typedef struct Node {
    int index;
    int profit;
    int weight;
    double bound;
    int* include;
} Node;
```
