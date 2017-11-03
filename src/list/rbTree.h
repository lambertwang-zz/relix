#ifndef __RBTREE_H__
#define __RBTREE_H__

struct Node {
    void *data;
    unsigned int id;
    unsigned char isRed;

    struct Node *left;
    struct Node *right;
    struct Node *parent;
};

struct Tree {
    struct Node *root;
    unsigned int count;
};

struct Node *grandParent(struct Node *node);
struct Node *sibling(struct Node *node);
struct Node *uncle(struct Node *node);

void rotateLeft(struct Node *node);
void rotateRight(struct Node *node);

struct Node *newNode(void *data, unsigned int id);

int initTree(struct Tree *tree);
void *getData(struct Tree *tree, unsigned int id);
int insert(struct Tree *tree, void *data, unsigned int id);
int removeData(struct Tree *tree, unsigned int id);

#endif

