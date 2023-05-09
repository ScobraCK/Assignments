#ifndef _KNAPSACK_

#define _KNAPSACK_

#define TAKEN 1
#define NOT_TAKEN 0

typedef struct Item {
    int value;
    int weight;
} Item;

void solve_knapsack(int max_weight, Item* items, int** V);
int* find_optimal_items(int max_weight, Item* items, int** V);

#endif