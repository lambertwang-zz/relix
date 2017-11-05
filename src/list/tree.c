#include "tree.h"
#include "../log/log.h"
#include "../constants.h"

#include <stdlib.h>
#include <stdio.h>

int depth(const struct Node *node) {
	if (node->parent == NULL) {
		return 0;
	}
	return 1 + depth(node->parent);
}

int checkRedChildren(const struct Node *node) {
    if (node == NULL) {
        return 0;
    }
    if (node->isRed) {
        if ((node->left != NULL && node->left->isRed) ||
            (node->right != NULL && node->right->isRed)) {
            writeLog(10, "ERROR: RED CHILDREN MUST BE BLACK");
            printTree(node);
            return 1;
        }
    }
    return checkRedChildren(node->left) + checkRedChildren(node->right);
}

int minDistToNil(const struct Node *node) {
    if (node == NULL) {
        return 0;
    }
    int distLeft = minDistToNil(node->left), distRight = minDistToNil(node->right);
    return (node->isRed ? 0 : 1) + (distLeft < distRight ? distLeft : distRight);
}

int maxDistToNil(const struct Node *node) {
    if (node == NULL) {
        return 0;
    }
    int distLeft = maxDistToNil(node->left), distRight = maxDistToNil(node->right);
    return (node->isRed ? 0 : 1) + (distLeft > distRight ? distLeft : distRight);
}

int checkNilDist(const struct Node *node) {
    if (node == NULL) {
        return 0;
    }
    int distLeft = checkNilDist(node->left), distRight = checkNilDist(node->right);
    if (distLeft || distRight) {
        return 0;
    }
    if (minDistToNil(node) != maxDistToNil(node)) {
        writeLog(10, "ERROR: DISTANCE TO NIL INCONSISTENT %d!!", node->id);
        printTree(node);
        return 1;
    }
    return 0;
}

int checkRefs(const struct Node *node) {
    if (node == NULL) {
        return 1;
    }
    checkRefs(node->right);
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

    if (node->left != NULL && node->right != NULL && node->left->id == node->right->id) {
	    writeLog(10, "DANGER!");
    }
    if (node->left != NULL && node->parent != NULL && node->left->id == node->parent->id) {
	    writeLog(10, "DANGER!");
    }
    if (node->parent != NULL && node->right != NULL && node->parent->id == node->right->id) {
	    writeLog(10, "DANGER!");
    }
    if (!(node->parent == NULL || (node->parent != NULL && node->parent->left != NULL && node->id == node->parent->left->id) ||
          (node->parent != NULL && node->parent->right!= NULL && node->id == node->parent->right->id))) {
	    writeLog(10, "DANGER!");
    }
    if (node->left != NULL && node->left->parent->id != node->id) {
	    writeLog(10, "DANGER!");
    }
    if (node->right != NULL && node->right->parent->id != node->id) {
	    writeLog(10, "DANGER!");
    }

    fflush(stdout);
	writeLog(10, buffer);
    checkRefs(node->left);
}

int validateTree(const struct Tree *tree) {
    // Root must be black
    if (tree->root != NULL && tree->root->isRed) {
        writeLog(10, "ERROR: ROOT NOT BLACK");
        printTree(tree->root);
    }
    checkRedChildren(tree->root);
    checkNilDist(tree->root);
    checkRefs(tree->root);
}

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
    fflush(stdout);
	writeLog(10, buffer);
}

void rotateLeft(struct Node *node) {
    struct Node *new = node->right;
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
        node->tree->root = new;
        new->tree = node->tree;
        node->tree = NULL;
    }
}

void rotateRight(struct Node *node) {
    struct Node *new = node->left;
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
        node->tree->root = new;
        new->tree = node->tree;
        node->tree = NULL;
    }
}

struct Node **max(struct Node **node) {
    if ((*node)->right == NULL) {
        return node;
    }
    return max(&(*node)->right);
}

struct Node **min(struct Node **node) {
    if ((*node)->left== NULL) {
        return node;
    }
    return min(&(*node)->left);
}

int initTree(struct Tree *tree) {
    tree->root = NULL;
    tree->count = 0;
    return 0;
}

void freeNodes(struct Node *node) {
    if (node != NULL) {
        freeNodes(node->left);
        freeNodes(node->right);
        free(node);
    }
}

int closeTree(struct Tree *tree) {
    freeNodes(tree->root);
    return 0;
}

int clearTree(struct Tree *tree) {
    freeNodes(tree->root);
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

