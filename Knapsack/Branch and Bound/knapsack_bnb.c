#include <stdio.h>
#include <stdlib.h>
#include "knapsack_bnb.h"
#include "priority_queue.h"

void copy_array(int* original, int* copy, int rows) {
	for(int i = 0; i<rows; i++) {
		copy[i] = original[i];
	}
}

//creates a deepcopy of a node
Node copy_node(Node *original) {
    Node copy;
    copy.index = original->index;
    copy.profit = original->profit;
    copy.weight = original->weight;
    copy.bound = original->bound;

    copy.include = malloc(sizeof(int) * (original->include[0]+1));
    copy_array(original->include, copy.include, original->include[0]+1);

    return copy;
}

int bound(Node node, int max_weight, Item* items){
    int total_weight, j;
    double bound;
    int item_count = node.include[0];

    //case 1: overweight
    if (node.weight > max_weight) { 
        return -1;
    }

    //case 2: calculate bound
    bound = node.profit;
    total_weight = node.weight;
    for (j = node.index+1; (j <= item_count) && (total_weight+items[j].weight <= max_weight); j++) {
        total_weight += items[j].weight;
        bound += items[j].value;
    }

    if (j <= item_count) {
        bound += (max_weight - total_weight) * items[j].value / items[j].weight;
    }

    return bound;
}

void solve_knapsack(Node node, int max_weight, Item *items, Node *best) {
    Node taken, not_taken;
    Node *next_node;
    int next_ind;
    int item_count = node.include[0];

    node.bound = bound(node, max_weight, items); //calculate bound for first node
    PriorityQueue *PQ = make_queue(item_count);
    enqueue(PQ, node);

    while(next_node = dequeue(PQ)) {
        //keep track of best node
        if (next_node->weight <= max_weight && next_node->profit > best->profit) {
            best->index = next_node->index;
            best->profit = next_node->profit;
            best->weight = next_node->weight;
            best->bound = next_node->bound; //unnesecary to keep track of
            copy_array(next_node->include, best->include, item_count+1);
        }

        //stop if bound is lower than profit or last index
        //in backtracking, promising checks for index
        if ((next_node->bound <= best->profit) || next_node->index == item_count) {
            break;
        }

        //expand the tree
        next_ind = ++next_node->index;
        taken = copy_node(next_node);
        not_taken = copy_node(next_node);

        //taken
        taken.include[next_ind] = TAKEN;
        taken.profit = next_node->profit + items[next_ind].value;
        taken.weight = next_node->weight + items[next_ind].weight;
        taken.bound = bound(taken, max_weight, items);
        enqueue(PQ, taken);
        
        //not taken
        not_taken.include[next_ind] = NOT_TAKEN;  //redundant in this case
        not_taken.bound = bound(not_taken, max_weight, items);
        enqueue(PQ, not_taken);

        //free include
        free(next_node->include);
        free(next_node);
        next_node = NULL;
    }

    //free queue
    // while (next_node) {
    //     free(next_node->include);
    //     free(next_node);
    //     next_node = dequeue(PQ);
    // }

    delete_queue(PQ);
}
