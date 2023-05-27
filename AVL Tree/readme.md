# AVL Tree

An AVL tree is a binary tree where the difference in the heights between the left and right subtrees is at most 1.
Both subtrees are also both AVL trees. To keep track of the balance of the tree, each node will keep track of a balance factor. The balance factor of all nodes should be -1, 0, or 1 to maintain an AVL tree. By utilizing the balance factor, we can know which side of the tree has more height by only comparing the two subtrees.

Balance factor = (height of left subtree) - (height of right subtree)

## Balancing
Two functions will be used to rebalance the tree. Left rotation and right rotation. For the diagrams, state 1 is assumed to be balanced.

### Single Rotation
With the intial state being state 1, perfroming a left rotation on A-B will make B the root while moving down A as the left subtree of B. The original left subtree of B(b) will then become the right subtree of A. Resulting in state 2. As a result, the height will be shifted to the left side. A double rotation is performed when the outer node(c) has higher height.

A right rotation is the same as left rotation but mirrored. In this case, a right rotation on B-A in state 2 will result in state 1. As a result, the height will be shifted to the right side.

     State 1              State 2
        A                    B 
      /   \                /   \
     a     B              A     c 
          / \            / \  
         b   c          a   b
Note that all non-capital nodes don't have to be leaf nodes and are AVL trees in themselves. 

A left rotation will be performed in the case a node under c causes A's balance factor to become -2, thus moving c up 1 height keeps the balance. And vice versa for right rotations.

### Double Rotation
There are cases where a double rotation will be needed, either a left-right rotation or right-left rotation depending on the orientation. Below example shows a right-left rotation where a right rotation is performed on B-C resulting in state 2. Then a left rotation is performed on A-C resulting in state 3. The end result is the height shifted to the left side. A double rotation is performed when the inner node(c1/c2) has higher height.

A left-right rotation will be performed in a mirror situation making the height shift to the right.

     State 1              State 2               State 3     
        A                    A                     C
      /   \                /   \                 /   \
     a     B              a     C               A     B
          / \                  / \             / \   / \
         C   b                c1  B           a  c1 c2  b
        / \                      / \
       c1 c2                    c2  b
A right-left rotation will be perfomed in case c1 or c2 causes A's balance factor to become -2, thus moving them up. And vice versa for left-right rotations.

## Inserting
When inserting we will recursively go into the subtrees where the key will go until we find an NULL node. If NULL we insert into that node. Due to the properties of the stack(LIFO) while we are recursively searching for the correct place, we will return back from the bottom starting from the node we inserted into. By checking the balance and updaing the height as we return we can automatically adjust the AVL tree. Depending on the balance factor and where we inserted, we will perform the nessecary rotations.

Due to the nature of inserting, in case balancing needs to be performed, the height of the balanced node will not change before and after insertion. Meaning that only one step of balancing (either single or double rotation) needs to be done for the tree to be balanced as the parent nodes have no change in balance factor(note that balance factor is calculated from the height of the subtree nodes).

Inserting can be done in O(log n) time.

## Deleting
When deleting a node, there will be 3 cases.
* Case 1: Leaf node
<br>The node to be deleted is a leaf node and can be simply deleted.
* Case 2: One subtree is NULL
<br>The node to be deleted is replaced by the non-NULL subtree.
* Case 3: Both subtrees exist
<br>The node to be replaced will be replaced by the minimum node of the right subtree(or maximum of left subtree). Then we will need to delete the minimum node that we just moved by calling the delete function again for the moved node.

For balancing when we delete a node, it is similar to inserting in that we check for the balance factor from bottom up while doing the nessecary rotations. In the case for deletion, we need to compare the balance factor of the subtree in question to know if a single or double rotation is needed.

Unlike inserting, multiple rotations may have to be performed to fully balance the tree. This is where the code recursively checking the balance factor for each step comes in.

Deleting can be done in O(log n) time.

## Searching
Searching is simple and it is already implemented into the insertig and deleting algorithms. If the key is larger than the node, we search the right subtree. If the key is smaller we search the left subtree. We repeat until we find the node or reach a NULL node. If NULL the key does not exist and we stop the search.

Searching can be done in O(log n) time.

## Code
### Data structure
We use a tree structure with a *value* to use as a key, and pointers to the *left* and *right* subtrees. We also need an aditional value *height* to keep track of the height for the balance factor.
The TreeElement can be adjusted to the needs of the tree and an int was used for this demonstration.
```c
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
```

The main functions will be the insert, delete, and search operations. A cmp function is taken as an argument to compare the TreeElement. 

This function will return 1 if a > b, -1 if a < b and 0 if same.
```c
Tree insert(Tree T, TreeElement key, int (*cmp)(TreeElement a, TreeElement b));
Tree delete(Tree T, TreeElement key, int (*cmp)(TreeElement a, TreeElement b));
void search(Tree T, TreeElement key, int (*cmp)(TreeElement a, TreeElement b));
```

The main.c provided will be a simple example of some insert, delete and searching operations hardcoded as a test case.

## Reference
Referenced for code: https://www.programiz.com/dsa/avl-tree