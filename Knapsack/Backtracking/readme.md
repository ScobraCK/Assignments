# 0/1 Knapsack Problem - Backtracking
Algorithm HW 3
<br>Solution for the 0/1 knapsack problem using backtracking.

## Problem description
- Given 𝑛 items and a "knapsack.”
- Item 𝑖 has weight 𝑤<sub>𝑖</sub> > 0 and has value 𝑣<sub>𝑖</sub> > 0.
- Knapsack has capacity of 𝑊.
- Goal: Fill knapsack so as to maximize total value.


## Algorithm
1. Sort the items in order of value/weight.
2. We construct a state space tree where the options are whether the item is taken or not taken in the sorted order.<br>The search method is a DFS search.
3. We define the promising function to determine whether the node should be further expanded or pruned.
4. The best node is kept tracked and the search is over when all promising nodes have been visited.

### Nodes
The nodes of the tree will have 4 parameters. 
* index: The item index. Also corresponds to the level of the tree.
* profit: Current profit of items taken.
* weight: Current weight of items taken.
* include: An array indicating whether an item was taken or not taken.
```c
typedef struct Node {
    int index;
    int profit;
    int weight;
    int *include;
} Node;
```

### Promising Function
There are 2 cases where a node is non-promising.

Case 1:  Weights of included items exceeds 𝑊: 𝑤𝑒𝑖𝑔ℎ𝑡 ≥ 𝑊.

Case 2: Even including all the remaining possible items can’t exceed the existing best profit. 

This is done by calculating the profit bound.
<br>As the items are sorted in max value by weight, the bound can be calculated by adding the value of the items in order from the current profit until there are no more items or the maximum weight is reached. If the knapsack has space but the next item weighs more than the available space, the fraction of the item is taken instead.