/*
AVL tree implementation
*/

#include <stdio.h>
#include <stdlib.h>
#include "avl_tree.h"


int max(TreeElement a, TreeElement b, int (*cmp)(TreeElement a, TreeElement b)) {
    if (cmp(a, b) > 0) {
        return a;
    }
    return b;
}

//returns self height
int height(Tree T) {
    if (T == NULL) {
        return 0;
    }
    return T->height;
}

//re-calculates height of current node
void update_height(Tree T, int (*cmp)(TreeElement a, TreeElement b)) {
    T->height = max(height(T->ltree), height(T->rtree), cmp) + 1;
}

//makes a new tree node
Tree makeNode(TreeElement e) {
    Tree T = (Tree) malloc(sizeof(struct tree_node));
    T->value = e;
    T->ltree = NULL;
    T->rtree = NULL;
    T->height = 1;
    return T;
}

//variable names same as github example
Tree left_rotate(Tree A, int (*cmp)(TreeElement a, TreeElement b)) {
    Tree B = A->rtree;
    Tree b = B->ltree;
    
    B->ltree = A;
    A->rtree = b;
    
    update_height(A, cmp); //A first as it is subtree of B
    update_height(B, cmp);

    return B;
}

//mirrored from left rotate
Tree right_rotate(Tree A, int (*cmp)(TreeElement a, TreeElement b)) {
    Tree B = A->ltree;
    Tree b = B->rtree;
    
    B->rtree = A;
    A->ltree = b;
    
    update_height(A, cmp); //A first as it is subtree of B
    update_height(B, cmp);

    return B;
}

int get_balance(Tree T) {
    if (T == NULL) {
        return 0;
    }
    return height(T->ltree) - height(T->rtree);
}

//inserts into position recursively
//cmp(a, b) returns 1 if a > b
Tree insert(Tree T, TreeElement key, int (*cmp)(TreeElement a, TreeElement b)) {
    //position found
    if (T == NULL) {
        return makeNode(key);
    }

    //recursively find position
    int compare = cmp(key, T->value);
    if (compare > 0) {  //key > T->value
        T->rtree = insert(T->rtree, key, cmp);
    }
    else if (compare < 0) {
        T->ltree = insert(T->ltree, key, cmp);
    }
    else {
        return T;
    }
    
    //balance tree from bottom up and update balance factor
    //utilizes stack(LIFO) structure of recursive calls
    update_height(T, cmp);
    int balance = get_balance(T);

    //left side is deeper (inserted into left)
    if (balance > 1) {
        if (cmp(key, T->ltree->value) > 0) { //inserted into right of left subtree
            T->ltree = left_rotate(T->ltree, cmp);  //double rotate
        }
        return right_rotate(T, cmp);
    }
    //right side is deeper (inserted into right)
    if (balance < -1) {
        if (cmp(key, T->ltree->value) < 0) { //inserted into left of right subtree
            T->rtree = right_rotate(T->ltree, cmp);  //double rotate
        }
        return left_rotate(T, cmp);
    }

    //no rotation needed (balanced)
    return T;
}

//find minimum value node of tree
Tree min_node(Tree T) {
    Tree node = T;
    while(node->ltree) {
        node = node->ltree;
    }
    return node;
}

//recursively searches until key is found and returns tree after deletion 
Tree delete(Tree T, TreeElement key, int (*cmp)(TreeElement a, TreeElement b)) {
    //not found
    if (T == NULL) {
        return T;
    }

    //recursively find key
    int compare = cmp(key, T->value);
    if (compare > 0) {  //key > T->value
        T->rtree = delete(T->rtree, key, cmp);
    }
    else if (compare < 0)
    {
        T->ltree = delete(T->ltree, key, cmp);
    }
    else {  //current root is to be deleted
        Tree temp = NULL;
        if (T->ltree == NULL && T->rtree == NULL) { //leaf node
            free(T);
            return NULL;
        }
        else if (T->ltree == NULL) {  //replace with right subtree
            temp = T;
            T = T->rtree;  //replace current node with subtree
            free(temp);
        }
        else if (T->rtree == NULL) {  //replace with left subtree
            temp = T;
            T = T->ltree;  //replace current node with subtree
            free(temp);
        }
        //both left and right subtrees exist
        else { 
            temp = min_node(T->rtree); //replace with minimum value node of right subtree
            T->value = temp->value;
            T->rtree = delete(T->rtree, temp->value, cmp); //delete replaced node
        }
    }

    //balance tree from bottom up and update balance factor
    //utilizes stack(LIFO) structure of recursive calls
    update_height(T, cmp);
    int balance = get_balance(T);

    //left side is deeper (deleted from right)
    if (balance > 1) {
        if (get_balance(T->ltree) < 0) { //right side of subtree is deeper
            T->ltree = left_rotate(T->ltree, cmp);
        }
        return right_rotate(T, cmp);
    }
    //right side is deeper (deleted from left)
    if (balance < -1) {
        if (get_balance(T->rtree) > 0) { //right side of subtree is deeper
            T->rtree = right_rotate(T->ltree, cmp);
        }
        return left_rotate(T, cmp);
    }

    //no rotation needed (balanced)
    return T;
}

//search while printing path
//for proof of conecpt, print is coded for int
void search(Tree T, TreeElement key, int (*cmp)(TreeElement a, TreeElement b)) {
    printf("Target: %d\n", key);
    printf("Search path: ");
    Tree node = T;
    int compare;
    int found = 0;

    while(node) {
        printf("%d ", node->value);
        compare = cmp(key, node->value);
        if (compare == 0) {
            found = 1;
            break;
        }
        else if (compare > 0){
            node = node->rtree;
        }
        else {
            node = node->ltree;
        }
    }

    if(found) {
        printf("\nFound\n");
    }
    else {
        printf("\nNot Found\n");  //searched until NULL
    }
}

// chatGPT code
// Function to print the tree in 2D format
void printTree2D(tree_node* root, int space) {
    // Base case
    if (root == NULL) {
        return;
    }

    // Increase spacing between levels
    space += 4;

    // Process right child first
    printTree2D(root->rtree, space);

    // Print current node after spacing
    //printf("\n");
    for (int i = 4; i < space; i++) {
        printf(" ");
    }
    printf("%d\n", root->value);  // Modify this according to your TreeElement structure

    // Process left child
    printTree2D(root->ltree, space);
}

void freeTree(Tree T) {
    if(T) {
        free(T->rtree);
        free(T->ltree);
        free(T);
    }
}