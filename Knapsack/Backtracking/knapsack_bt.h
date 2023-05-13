#ifndef _KNAPSACK_BT_

#define _KNAPSACK_BT_

#define TAKEN 1
#define NOT_TAKEN 0

typedef struct Item {
    int value;
    int weight;
} Item;

typedef int *Include;

typedef struct Node {
    int index;
    int profit;
    int weight;
    Include include;
} Node;

void solve_knapsack(Node node, int max_weight, Item *items, Node *best);

#endif