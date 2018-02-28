#ifndef __TREE_H__
#define __TREE_H__

// Engine
#include "array.h"

#define INIT_STACK_SIZE 16

typedef struct Iterator {
    const struct Tree *tree;

    // struct Array stack;

    unsigned int index;
    struct Node *current;
} Iterator;

typedef struct Tree {
    struct Node *root;
    unsigned int count;
} Tree;

typedef struct Node {
    void *data;
    int id;
    char isRed;

    struct Node *left;
    struct Node *right;
    struct Node *parent;

    struct Tree *tree;
} Node;

// Internal tree functions
void validateTree(const Tree *tree);

void printTree(const Node *node);
void printNode(const Node *node);

Node *grandParent(const Node *node);
Node *sibling(const Node *node);
Node *uncle(const Node *node);

void rotateLeft(Node *node);
void rotateRight(Node *node);

Node **maxNode(Node **node);
Node **minNode(Node **node);

Node *newNode(void *data, int id);

// External tree functions
int initTree(Tree *tree);
int closeTree(Tree *tree);
int clearTree(Tree *tree);

void *getData(Tree *tree, int id);
Node *getTreeNode(Tree *tree, int id);
int insert(Tree *tree, void *data, int id);
int removeId(Tree *tree, int id);

// Iterator functions
Iterator initIterator(Tree *tree);
Node *getNode(const Iterator *it);
void *getItem(const Iterator *it);
Node *getNext(Iterator *it);
int done(const Iterator *it);

// Unused
// void generate(Iterator *it);

#endif

