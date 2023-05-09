#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "knapsack.h"

#define MAX_LINE_LENGTH 1024
void get_items(char* file, Item* items_ptr[]);

int main(int argc, char *argv[]) {
    Item *items;
    int item_count;
    int max_weight;
    int* item_list;

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

    //free memory
    free(items);
    freeVector(item_list);

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
