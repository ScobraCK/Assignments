#ifndef _AVL_

#define _AVL_

typedef int TreeElement;  //change to needed

typedef struct tree_node *tree_pointer;
typedef tree_pointer Tree;
typedef tree_pointer Subtree;
typedef struct tree_node {
    TreeElement value;
    Subtree ltree;
    Subtree rtree;
    int height;
} tree_node;

Tree insert(Tree T, TreeElement key, int (*cmp)(TreeElement a, TreeElement b));
Tree delete(Tree T, TreeElement key, int (*cmp)(TreeElement a, TreeElement b));
void search(Tree T, TreeElement key, int (*cmp)(TreeElement a, TreeElement b));
void printTree2D(tree_node* root, int space);
void freeTree(Tree T);

#endif