
#include "rbTree.h"
#include "../log/log.h"

#include <stdlib.h>
#include <stdio.h>

struct Node *grandParent(const struct Node *node) {
    if (node->parent != NULL) {
        return node->parent->parent;
    }
    return NULL;
}

struct Node *sibling(const struct Node *node) {
    if (node->parent != NULL) {
        if (node->parent->left == node) {
            return node->parent->right;
        }
        return node->parent->left;
    }
    return NULL;
}

struct Node *uncle(const struct Node *node) {
    if (grandParent(node) != NULL) {
        return sibling(node->parent);
    }
    return NULL;
}

const struct Node *getRoot(const struct Node *node) {
	if (node->parent == NULL) {
		return node;
	}
	return getRoot(node->parent);
}

int depth(const struct Node *node) {
	if (node->parent == NULL) {
		return 0;
	}
	return 1 + depth(node->parent);
}

void printTree(const struct Node *node) {
    if (node == NULL) {
        return;
    } else {
        printTree(node->right);
        printNode(node);
        printTree(node->left);
    }
}

void printNode(const struct Node *node) {
	char buffer[512];
	int i, c = 0;
	for (i = 0; i < depth(node); i++) {
		c += sprintf(buffer + c, "\t");
	}
	sprintf(buffer + c, 
		"%d %s, P: %d, L: %d, R: %d", node->id, 
        node->isRed ? "red" : "black",
		node->parent != NULL ? node->parent->id : -1,
		node->left!= NULL ? node->left->id : -1,
		node->right!= NULL ? node->right->id : -1);
	writeLog(10, buffer);
}

void rotateLeft(struct Node *node) {
    struct Node *new = node->right;
	writeLog(10, "Rotate L node %d, new %d", node->id, new->id);
    node->right = new->left;
	if (node->right != NULL) {
		node->right->parent = node;
	}
    new->left = node;
    new->parent = node->parent;
    if (node->parent != NULL) {
        if (node->parent->left == node) {
            node->parent->left = new;
        } else {
            node->parent->right = new;
        }
    }
    node->parent = new;

    if (node->tree != NULL) {
		writeLog(10, "replacing root");
        node->tree->root = new;
        new->tree = node->tree;
        node->tree = NULL;
    }
}

void rotateRight(struct Node *node) {
    struct Node *new = node->left;
	writeLog(10, "Rotate R node %d, new %d", node->id, new->id);
    node->left= new->right;
	if (node->left != NULL) {
		node->left->parent = node;
	}
    new->right= node;
    new->parent = node->parent;
    if (node->parent != NULL) {
        if (node->parent->left == node) {
            node->parent->left = new;
        } else {
            node->parent->right = new;
        }
    }
    node->parent = new;

    if (node->tree != NULL) {
		writeLog(10, "replacing root");
        node->tree->root = new;
        new->tree = node->tree;
        node->tree = NULL;
    }
}

struct Node *max(struct Node *node) {
    if (node->right == NULL) {
        return node;
    }
    return max(node->right);
}

struct Node *min(struct Node *node) {
    if (node->left== NULL) {
        return node;
    }
    return min(node->left);
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

    newNode->tree = NULL;

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

