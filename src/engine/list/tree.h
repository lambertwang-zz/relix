#ifndef __TREE_H__
#define __TREE_H__

// Engine
#include "array.h"

#define INIT_STACK_SIZE 16

typedef struct Iterator {
    const struct Tree *tree;

    struct Array stack;

    int index;
    struct Node *current;
} Iterator;

typedef struct Tree {
    struct Node *root;
    int count;
} Tree;

typedef struct Node {
    void *data;
    unsigned int id;
    unsigned char isRed;

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

Node **max(Node **node);
Node **min(Node **node);

Node *newNode(void *data, unsigned int id);

// External tree functions
int initTree(Tree *tree);
int closeTree(Tree *tree);
int clearTree(Tree *tree);

void *getData(Tree *tree, unsigned int id);
int insert(Tree *tree, void *data, unsigned int id);
int removeId(Tree *tree, unsigned int id);

// Iterator functions
Iterator *initIterator(Tree *tree);
Node *getNode(const Iterator *it);
void *getItem(const Iterator *it);
Node *getNext(Iterator *it);
int done(const Iterator *it);
int closeIterator(Iterator *it);

// Unused
void generate(Iterator *it);

#endif

