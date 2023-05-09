#include <stdio.h>
#include <stdlib.h>
#include "knapsack.h"
#include "array.h"

// Constructs optimal solutions V[item_count+1][max_weight+1].
void solve_knapsack(int max_weight, Item* items, int** V) {
    Item item; //actual items start from index 1
    int temp_value;  //to store the value of the knapsack after an item is added
    int item_count = items[0].value;

    //initilize base cases
    for (int i = 0; i <= item_count; i++)
        V[i][0] = 0;
    for (int j = 0; j <= max_weight; j++)
        V[0][j] = 0;
    
    //loop through i and j
    for (int i = 1; i <= item_count; i++) {
        item = items[i];
        for (int j = 1; j <= max_weight; j++) {
            if (j - item.weight < 0) { //overcapacity
                V[i][j] = V[i-1][j];
            }
            else {
                temp_value = V[i-1][j-item.weight] + item.value;
                V[i][j] = V[i-1][j] > temp_value ? V[i-1][j] : temp_value; //either not worthy or included
            }
        }
    }
}

//finds out the optimal items selected given a V[][]
//returns: int arr[item_count]
//arr[0] = total_weight and has boolean value for each item index
int* find_optimal_items(int max_weight, Item* items, int** V) {
    Item current_item;
    int item_count = items[0].value;
    int* item_list =  vector(item_count+1);
    int weight = 0; //the max value to be added
    int current_item_ind = item_count; //start from optimal solution V[n][W] = V[item_count][max_weight]
    int current_weight = max_weight;
    
    while(current_item_ind > 0) {
        current_item = items[current_item_ind];
        if (V[current_item_ind][current_weight] == V[current_item_ind-1][current_weight]) { //item was not worthy
            item_list[current_item_ind] = NOT_TAKEN;
        }
        else {  //item was taken
            item_list[current_item_ind] = TAKEN;
            current_weight -= current_item.weight;
            weight += current_item.weight;
        }
        current_item_ind--;
    }

    item_list[0] = weight;

    return item_list;
}
