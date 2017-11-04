#ifndef __TREE_H__
#define __TREE_H__

struct Tree {
    struct Node *root;
    unsigned int count;
};

struct Node {
    void *data;
    unsigned int id;
    unsigned char isRed;

    struct Node *left;
    struct Node *right;
    struct Node *parent;

    struct Tree *tree;
};

void printTree(const struct Node *node);
void printNode(const struct Node *node);

struct Node *grandParent(const struct Node *node);
struct Node *sibling(const struct Node *node);
struct Node *uncle(const struct Node *node);

void rotateLeft(struct Node *node);
void rotateRight(struct Node *node);

struct Node *max(struct Node *node);
struct Node *min(struct Node *node);

struct Node *newNode(void *data, unsigned int id);

int initTree(struct Tree *tree);
void *getData(struct Tree *tree, unsigned int id);
int insert(struct Tree *tree, void *data, unsigned int id);
int removeId(struct Tree *tree, unsigned int id);

#endif

