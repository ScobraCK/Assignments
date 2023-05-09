#ifndef _KNAPSACK_

#define _KNAPSACK_

#define TAKEN 1
#define NOT_TAKEN 0

typedef struct Item {
    int value;
    int weight;
} Item;

typedef struct GlobalValues {
    int max_weight;
    Item* items;
    int max_profit;
    int *best_item_list;
} GlobalValues;

void solve_knapsack(int index, int profit, int weight, int* item_list, GlobalValues gv);

#endif