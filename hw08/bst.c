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
    if(t->root == NULL) {
        t->root = tempNode;
        t->count ++;
        return 0;
    }
    else {
        current = t->root;
        parent = NULL;

        while(1) {
            parent = current;

            //go to left of the tree
            if(key < parent->data) {
                current = current->leftChild;
                //insert to the left
                if(current == NULL) {
                   parent->leftChild = tempNode;
                   t->count ++;
                   return 0;
                }
            }  //go to right of the tree
            else {
                current = current->rightChild;

                //insert to the right
                if(current == NULL) {
                   parent->rightChild = tempNode;
                   t->count ++;
                   return 0;
                }
            }
        }
   }
}
