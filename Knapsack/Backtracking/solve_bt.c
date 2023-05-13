#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "knapsack_bt.h"

#define MAX_LINE_LENGTH 1024
void get_items(char* file, Item* items_ptr[]);
int compare_item(const void* a, const void* b); //for qsort()

int main(int argc, char *argv[]) {
    Item *items;
    int item_count;
    int max_weight;

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

    //init first node
    Node node = {0, 0, 0, NULL};
    node.include = calloc(item_count+1, sizeof(int));
    node.include[0] = item_count; //item_list init to NOT_TAKEN = 0

    //init best node
    Node *best = malloc(sizeof(Node));
    best->index = 0;
    best->profit = 0;
    best->weight = 0;
    best->include = calloc(item_count+1, sizeof(int));
    best->include[0] = item_count;  //index 0

    //sort items using built in qsort()
    qsort(items+1, item_count, sizeof(Item), compare_item);  //index 0 is not part of sort
    
    //test sort
    // for(int i = 1; i <= item_count; i++) {
    //     printf("%d. Value: %d, Weight: %d\n", i, items[i].value, items[i].weight);  
    // }

    //solve
    solve_knapsack(node, max_weight, items, best);

    //print result
    for(int i = 1; i <= best->index; i++) {
        if (best->include[i]==TAKEN) { //TAKEN
            printf("%d. Value: %d, Weight: %d\n", i, items[i].value, items[i].weight);
        }
    }
    printf("\nTotal Value: %d, Total Weight: %d\n", best->profit, best->weight);

    //free memory
    free(items);
    free(node.include);
    free(best->include);
    free(best);

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

int compare_item(const void* a, const void* b) {
    double ratio1 = (double)((Item*)a)->value / ((Item*)a)->weight;
    double ratio2 = (double)((Item*)b)->value / ((Item*)b)->weight;

    if (ratio1 > ratio2) {
        return -1;
    } else if (ratio1 < ratio2) {
        return 1;
    }
    return 0; 
}