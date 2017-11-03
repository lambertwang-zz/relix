#include "rbTree.h"

#include <stdlib.h>
#include <stdio.h>

void insert_repair(struct Node *node);

int insertRecurse(struct Node *node, struct Node *new) {
    if (node->id == new->id) {
        return 1;
    }
    if (node->id > new->id) {
        if (node->left == NULL) {
            node->left = new;
            node->left->parent = node;
            return 0;
        }
        insertRecurse(node->left, new);
        return 0;
    }
    if (node->right == NULL) {
        node->right = new;
        node->right->parent = node;
        return 0;
    }
    insertRecurse(node->right, new);
    return 0;
}

void insert1(struct Node *node) {
    return;
}

void insert2(struct Node *node) {
    node->parent->isRed = 0;
    uncle(node)->isRed = 0;
    struct Node *g = grandParent(node);
    g->isRed = 1;
    insert_repair(g);
}

void insert3(struct Node *node) {
    struct Node *p = node->parent;
    // struct Node *g = grandparent(node);

//   if (n == g->left->right) {
// rotate_left(p);
// n = n->left;
// } else if (n == g->right->left) {
// rotate_right(p);
// n = n->right; 
// }
// 
//    insert_case4step2(n);
}

void insert_repair(struct Node *node) {
    if (node->parent == NULL) {
        node->isRed = 0;
    } else if (!node->parent->isRed) {
        insert1(node);
    } else if (uncle(node)->isRed) {
        insert2(node);
    } else {
        insert3(node);
    }
}

int insert(struct Tree *tree, void *data, unsigned int id) {
    struct Node *new = newNode(data, id);
    if (tree->root == NULL) {
        tree->root = new;
        tree->root->isRed = 0;
        tree->count = 1;
        return 0;
    }

    int result = insertRecurse(tree->root, new);
    if (result) {
        free(new);
        return result;
    }
    tree->count++;

    insert_repair(new);
    return 0;
}

