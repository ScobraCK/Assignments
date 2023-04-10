#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "knapsack.h"
#include "array.h"

#define MAX_LINE_LENGTH 1024
void get_items(char* file, Item* items_ptr[]);

int main(int argc, char *argv[]) {
    Item *items;
    int item_count;
    int max_weight;
    int **V = NULL;
    int* item_list;
    int total_weight = 0;

    //usage and checks
    if (argc != 3)  {
        printf("Usage: %s item_file max_weight\n", argv[0]);
        exit(0);
    }
    if ((max_weight = atoi(argv[2])) < 0) {
        printf("Weight should be a non-negative integer. Given weight: %d", max_weight);
        exit(0);
    }

    //read item file
    get_items(argv[1], &items);
    item_count = items[0].value;
    V = array(item_count+1, max_weight+1);

    //get V[][]
    solve_knapsack(max_weight, items, V);
    if (item_count < 10 && max_weight < 10) {  //only print if dataset is small, for testing purposes
        printArray(V, item_count+1, max_weight+1, "V:");
    }
    printf("\n----------Optimal Items----------\n");

    //get optimal item list
    item_list = find_optimal_items(max_weight, items, V);
    printf("Selected items:\n");
    int current_item_no = 1;
    for (int i = 1; i <= item_count; i++) {
        if (item_list[i]) { //TAKEN
            printf("%d. Value: %d, Weight: %d\n", current_item_no++, items[i].value, items[i].weight);
        }
    }
    printf("\nTotal Value: %d, Total Weight: %d\n", V[item_count][max_weight], item_list[0]); //total weight saved in item_list[0]

    //free memory
    free(items);
    freeVector(item_list);
    freeArray(V, item_count+1);

    return 0;
}

//get items from file. items[0] will be (item_count, 0)
void get_items(char* file, Item* items_ptr[]) {
    FILE *fp;
    char line[MAX_LINE_LENGTH];
    Item *items = NULL;
    int item_count = 0;

    fp = fopen(file, "r");
    if (fp == NULL) {
        printf("Error: Cannot open file.\n");
        exit(1);
    }

    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
        char *token;
        Item item;

        token = strtok(line, ",");
        item.value = atoi(token);

        token = strtok(NULL, ",");
        item.weight = atoi(token);

        item_count++;
        items = realloc(items, (item_count+1) * sizeof(Item));
        items[item_count] = item;
    }

    fclose(fp);

    items[0].value = item_count;
    items[0].weight = 0;
    *items_ptr = items;
}
