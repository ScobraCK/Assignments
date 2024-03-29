#include <stdio.h>
#include <stdlib.h>
#include "avl_tree.h"

int compare_element(TreeElement a, TreeElement b) {
    if (a > b) {
        return 1;
    } else if (a < b) {
        return -1;
    }
    return 0; 
}

int main(void) {
    Tree T = NULL;

    printf("------------Insertion------------\n");
    T = insert(T, 2, compare_element);
    printTree2D(T, 0);
    printf("------------------------\n");

    T = insert(T, 1, compare_element);
    printTree2D(T, 0);
    printf("------------------------\n");

    T = insert(T, 8, compare_element);
    printTree2D(T, 0);
    printf("------------------------\n");

    T = insert(T, 5, compare_element);
    printTree2D(T, 0);
    printf("------------------------\n");

    T = insert(T, 4, compare_element);
    printTree2D(T, 0);
    printf("------------------------\n");

    T = insert(T, 3, compare_element);
    printTree2D(T, 0);
    printf("------------------------\n");

    T = insert(T, 9, compare_element);
    printTree2D(T, 0);
    printf("------------Searching------------\n");

    search(T, 9, compare_element);
    search(T, 4, compare_element);
    search(T, 6, compare_element);
    printf("------------Deletion------------\n");

    printTree2D(T, 0);
    printf("------------Delete 4------------\n");

    T = delete(T, 4, compare_element);
    printTree2D(T, 0);
    printf("------------Delete 8------------\n");

    T = delete(T, 8, compare_element);
    printTree2D(T, 0);
    printf("------------Delete 9------------\n");

    T = delete(T, 9, compare_element);
    printTree2D(T, 0);
    printf("------------Delete 1------------\n");

    T = delete(T, 1, compare_element);
    printTree2D(T, 0);

    freeTree(T);
    return 0;

}