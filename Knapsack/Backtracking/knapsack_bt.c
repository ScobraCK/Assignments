#include <stdio.h>
#include <stdlib.h>
#include "knapsack_bt.h"

void copy_array(Include *original, Include *copy, int rows) {
    Include copy_arr = *copy;
    Include original_arr = *original;
	for(int i = 0; i<rows; i++) {
		copy_arr[i] = original_arr[i];
	}
}

//creates deep copy for include
void copy_node(Node *original, Node *copy) {
    copy->index = original->index;
    copy->profit = original->profit;
    copy->weight = original->weight;

    copy->include = malloc(sizeof(int) * (original->include[0]+1));
    copy_array(&(original->include), &(copy->include), original->include[0]+1);
}

int promising(Node node, int max_weight, Item* items, Node *best){
    int total_weight, j;
    double bound;
    int item_count = node.include[0];

    //case 1: overweight
    if (node.weight >= max_weight) { 
        return 0;
    }

    //case 2: calculate bound
    bound = node.profit;
    total_weight = node.weight;
    for (j = node.index+1; (j <= item_count) && (total_weight+items[j].weight <= max_weight); j++) {
        total_weight += items[j].weight;
        bound += items[j].value;
    }

    if (j <= item_count) {
        bound += (double)(max_weight - total_weight) * (double)items[j].value / (double)items[j].weight;
    }

    return bound > best->profit;
}

void solve_knapsack(Node node, int max_weight, Item *items, Node *best) {
    //keep track of best node
    if (node.weight <= max_weight && node.profit > best->profit) {
        best->index = node.index;
        best->profit = node.profit;
        best->weight = node.weight;
        copy_array(&(node.include), &(best->include), node.include[0]+1);
    }

    //check promising
    if (promising(node, max_weight, items, best)) {
        int next_ind = ++node.index;

        //expand the tree
        Node taken, not_taken;
        copy_node(&node, &taken);
        copy_node(&node, &not_taken);

        //taken
        taken.include[next_ind] = TAKEN;
        taken.profit = node.profit + items[next_ind].value;
        taken.weight = node.weight + items[next_ind].weight;
        solve_knapsack(taken, max_weight, items ,best);

        //not taken
        not_taken.include[next_ind] = NOT_TAKEN;  //redundant in this case
        solve_knapsack(not_taken, max_weight, items, best);
        
        //free copied Include
        free(taken.include);
        free(not_taken.include);
    }
}
