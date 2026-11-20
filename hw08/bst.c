/* a port of TutorialsPoint's BinarySearchTree.c, with no globals */
#include "bst.h"
#include <stdlib.h>

/* from the original file's struct node, unchanged: */
struct node {
   int data;
   struct node *leftChild;
   struct node *rightChild;
};

struct bst {
    struct node *root; /* replaces the original's global; NULL when empty */
    size_t count; /* the number of nodes in the tree */
};

/* new: replaces the original's global "struct node *root = NULL" */
bst_t *bst_create(void) {

    struct bst *newTree = malloc(sizeof *newTree);
    if (newTree == NULL) {
        return NULL;
    }
    newTree->count = 0;
    newTree->root = NULL;

    return newTree;
}

/*  from the original file's insert(): same loop and names
    takes the tree t, checks it and malloc
    returns 0 or -1 and updates its count */
int bst_insert(bst_t *t, int key) {
    if (t == NULL) {
        return -1;
    }
    struct node *tempNode = (struct node*) malloc(sizeof(struct node));
    if (tempNode == NULL) {
        return -1;
    }

    tempNode->data = key;
    tempNode->leftChild = NULL;
    tempNode->rightChild = NULL;

    struct node *current;
    struct node *parent;

    //if tree is empty
    if (t->root == NULL) {
        t->root = tempNode;
        t->count ++;
        return 0;
    }
    else {
        current = t->root;
        parent = NULL;

        while (1) {
            parent = current;

            //go to left of the tree
            if (key < parent->data) {
                current = current->leftChild;
                //insert to the left
                if (current == NULL) {
                   parent->leftChild = tempNode;
                   t->count ++;
                   return 0;
                }
            }  //go to right of the tree
            else {
                current = current->rightChild;

                //insert to the right
                if (current == NULL) {
                   parent->rightChild = tempNode;
                   t->count ++;
                   return 0;
                }
            }
        }
   }
}

/* helper function to store n in *visits, unless it is NULL */
static void set_visits(size_t *visits, size_t count) {
    if (visits != NULL) {
        *visits = count;
    }
}

/* from the original's search(), without printing*/
bool bst_contains(const bst_t *t, int key, size_t *visits) {
    if (t == NULL) {
        set_visits(visits, 0);
        return false;
    }
    const struct node *current = t->root;
    size_t count = 0;
    while (current->data != key) {
        if (current != NULL) {
            count ++;
        }
        // data @ current node > data we are looking for
        // so we need to traverse the path where data < data current node
        // go to left tree
        if(current->data > key) {
            current = current->leftChild;
        }
        //else go to right tree
        else {
            current = current->rightChild;
        }

        //not found
        if (current == NULL) {
            set_visits(visits, count);
            return false;
        }
    }
    count ++; /* the while loop exited without returning, which means the key was found */
    set_visits(visits, count);
    return true;
}

/* new function: original had no size function */
size_t bst_size(const bst_t *t) {
    if (t == NULL) {
        return 0;
    }
    return t->count;
}

/* new function, recursive helper function to determine height of a node */
static size_t node_height(const struct node *n) {
    if (n == NULL) {
        return 0;
    }
    size_t left = node_height(n->leftChild);
    size_t right = node_height(n->rightChild);
    size_t longer;
    if (left > right) {
        longer = left;
    }
    else {
        longer = right;
    }

    return longer + 1;
}

/* new function, original had no height function */
size_t bst_height(const bst_t *t) {
    if (t == NULL) {
        return 0;
    }

    return node_height(t->root);
}

/* helper function to destroy a node */
static void node_destroy(struct node *n) {
    if (n != NULL) {
        node_destroy(n->leftChild);
        node_destroy(n->rightChild);
        free(n);
    }
}

/* new function, becuase the original never freed anything*/
void bst_destroy(bst_t *t) {
    if (t != NULL) {
        node_destroy(t->root);
        free(t);
    }
}
