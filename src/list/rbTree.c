
#include "rbTree.h"

#include <stdlib.h>
#include <stdio.h>

struct Node *grandParent(struct Node *node) {
    if (node->parent != NULL) {
        return node->parent->parent;
    }
    return NULL;
}

struct Node *sibling(struct Node *node) {
    if (node->parent != NULL) {
        if (node->parent->left == node) {
            return node->parent->right;
        }
        return node->parent->left;
    }
    return NULL;
}

struct Node *uncle(struct Node *node) {
    if (grandParent(node) != NULL) {
        return sibling(node->parent);
    }
    return NULL;
}

void rotateLeft(struct Node *node) {
    struct Node *new = node->right;
    node->right = new->left;
    new->left = node;
    new->parent = node->parent;
    node->parent = new;
}

void rotateRight(struct Node *node) {
    struct Node *new = node->left;
    node->left= new->right;
    new->right= node;
    new->parent = node->parent;
    node->parent = new;
}

int initTree(struct Tree *tree) {
    tree->root = NULL;
    tree->count = 0;
    return 0;
}

void *searchTree(struct Node *node, unsigned int id) {
    if (node == NULL) {
        return NULL;
    }
    if (id > node->id) {
        return searchTree(node->right, id);
    } else if (id < node->id) {
        return searchTree(node->left, id);
    }
    return node->data;
}

void *getData(struct Tree *tree, unsigned int id) {
    return searchTree(tree->root, id);
}

struct Node *newNode(void *data, unsigned int id) {
    struct Node *newNode = malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->id = id;
    newNode->isRed = 1;

    newNode->left = NULL;
    newNode->right = NULL;
    newNode->parent = NULL;

    return newNode;
}


int addNodeToTree(struct Node *node, struct Node *new) {
    if (node->id > new->id) {
        if (node->left == NULL) {
            node->left = new;
            new->parent = node->left;
            return 0;
        }
        return addNodeToTree(node->left, new);
    }
    if (node->right == NULL) {
        node->right = new;
        new->parent = node->right;
        return 0;
    }
    return addNodeToTree(node->right, new);
}


int removeFromTree(struct Node **indirect, unsigned int id) {
    if (*indirect == NULL) {
        return -1;
    }
    if ((*indirect)->id == id) {
        struct Node *tmp = *indirect;
        if ((*indirect)->right != NULL) {
            if ((*indirect)->left != NULL) {
                addNodeToTree((*indirect)->right, (*indirect)->left);
            }
            *indirect = (*indirect)->right;
            (*indirect)->parent = tmp->parent;
            free(tmp);
            return 0;
        }
        if ((*indirect)->left != NULL) {
            *indirect = (*indirect)->left;
            (*indirect)->parent = tmp->parent;
            free(tmp);
            return 0;
        }
        *indirect = NULL;
        free(tmp);
        return 0;
    }
    if ((*indirect)->id > id) {
        return removeFromTree(&(*indirect)->left, id);
    }
    return removeFromTree(&(*indirect)->right, id);
}

int removeData(struct Tree *tree, unsigned int id) {
    if (!removeFromTree(&(tree->root), id)) {
        tree->count--;
        return 0;
    }
    return -1;
}

