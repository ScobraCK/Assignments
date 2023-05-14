#ifndef _KNAPSACK_BNB_

#define _KNAPSACK_BNB_

#define TAKEN 1
#define NOT_TAKEN 0

typedef struct Item {
    int value;
    int weight;
} Item;

typedef struct Node {
    int index;
    int profit;
    int weight;
    double bound;
    int* include;
} Node;

void solve_knapsack(Node node, int max_weight, Item *items, Node *best);

#endif