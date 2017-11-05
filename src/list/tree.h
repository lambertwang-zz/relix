#ifndef __TREE_H__
#define __TREE_H__

#define INIT_STACK_SIZE 16

struct Iterator {
    const struct Tree *tree;

    struct Node **stack;
    int stack_size;
    int stack_count;

    int index;
    struct Node *current;
};

struct Tree {
    struct Node *root;
    int count;
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

int validateTree(const struct Tree *tree);

void printTree(const struct Node *node);
void printNode(const struct Node *node);

struct Node *grandParent(const struct Node *node);
struct Node *sibling(const struct Node *node);
struct Node *uncle(const struct Node *node);

void rotateLeft(struct Node *node);
void rotateRight(struct Node *node);

struct Node **max(struct Node **node);
struct Node **min(struct Node **node);

struct Node *newNode(void *data, unsigned int id);

int initTree(struct Tree *tree);
int closeTree(struct Tree *tree);
int clearTree(struct Tree *tree);

void *getData(struct Tree *tree, unsigned int id);
int insert(struct Tree *tree, void *data, unsigned int id);
int removeId(struct Tree *tree, unsigned int id);

struct Iterator *initIterator(struct Tree *tree);
struct Node *getNode(const struct Iterator *it);
void *getItem(const struct Iterator *it);
struct Node *getNext(struct Iterator *it);
int done(const struct Iterator *it);
int closeIterator(struct Iterator *it);

void generate(struct Iterator *it);

#endif

