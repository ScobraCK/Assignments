#include <stdio.h>
#include <stdlib.h>
#include "knapsack.h"
#include "array.h"


int promising(int index, int profit, int weight, GlobalValues gv){
    int total_weight, j;
    double bound;
    int item_count = gv.best_item_list[0];
    int max_weight = gv.max_weight;

    if (weight >= max_weight) { //case 1: overweight
        return 0;
    }

    //case 2: calculate bound
    bound = profit;
    total_weight = weight;
    for (j = index+1; (j <= item_count) && (total_weight+gv.items[j].weight <= max_weight); j++) {
        total_weight += gv.items[j].weight;
        bound += gv.items[j].value;
    }

    if (j <= item_count) {
        bound += (max_weight - total_weight) * gv.items[j].value / gv.items[j].weight;
    }

    return bound > gv.max_profit;
}

void solve_knapsack(int index, int profit, int weight, int* item_list, GlobalValues gv) {

    if (weight <= gv.max_weight && profit > gv.max_profit) { //update max_profit
        gv.max_profit = profit;
        copyVector(gv.best_item_list, item_list, item_list[0]); 
    }

    if (promising(index, profit, weight, gv)) {
        item_list[index+1] = TAKEN;
        int taken_profit = profit + gv.items[index+1].value;
        int taken_weight = weight + gv.items[index+1].weight;
        solve_knapsack(index+1, taken_profit, taken_weight, item_list, gv);

        item_list[index+1] = NOT_TAKEN;
        solve_knapsack(index+1, profit, weight, item_list, gv);
    }
}
