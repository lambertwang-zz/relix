#include "tree.h"

// Library
#include <stdlib.h>
#include <stdio.h>

// Engine
#include "log/log.h"
#include "constants.h"

int depth(const Node *node) {
	if (node->parent == NULL) {
		return 0;
	}
	return 1 + depth(node->parent);
}

int checkRedChildren(const Node *node) {
    if (node == NULL) {
        return 0;
    }
    if (node->isRed) {
        if ((node->left != NULL && node->left->isRed) ||
            (node->right != NULL && node->right->isRed)) {
            writeLog(0xffff, "ERROR: RED CHILDREN MUST BE BLACK");
            printTree(node);
            return 1;
        }
    }
    return checkRedChildren(node->left) + checkRedChildren(node->right);
}

int minDistToNil(const Node *node) {
    if (node == NULL) {
        return 0;
    }
    int distLeft = minDistToNil(node->left), distRight = minDistToNil(node->right);
    return (node->isRed ? 0 : 1) + (distLeft < distRight ? distLeft : distRight);
}

int maxDistToNil(const Node *node) {
    if (node == NULL) {
        return 0;
    }
    int distLeft = maxDistToNil(node->left), distRight = maxDistToNil(node->right);
    return (node->isRed ? 0 : 1) + (distLeft > distRight ? distLeft : distRight);
}

int checkNilDist(const Node *node) {
    if (node == NULL) {
        return 0;
    }
    int distLeft = checkNilDist(node->left), distRight = checkNilDist(node->right);
    if (distLeft || distRight) {
        return 0;
    }
    if (minDistToNil(node) != maxDistToNil(node)) {
        writeLog(0xffff, "ERROR: DISTANCE TO NIL INCONSISTENT %d!!", node->id);
        printTree(node);
        return 1;
    }
    return 0;
}

void checkRefs(const Node *node) {
    if (node != NULL) {
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
	        writeLog(0xffff, "DANGER!");
        }
        if (node->left != NULL && node->parent != NULL && node->left->id == node->parent->id) {
	        writeLog(0xffff, "DANGER!");
        }
        if (node->parent != NULL && node->right != NULL && node->parent->id == node->right->id) {
	        writeLog(0xffff, "DANGER!");
        }
        if (!(node->parent == NULL || (node->parent != NULL && node->parent->left != NULL && node->id == node->parent->left->id) ||
              (node->parent != NULL && node->parent->right!= NULL && node->id == node->parent->right->id))) {
	        writeLog(0xffff, "DANGER!");
        }
        if (node->left != NULL && node->left->parent->id != node->id) {
	        writeLog(0xffff, "DANGER!");
        }
        if (node->right != NULL && node->right->parent->id != node->id) {
	        writeLog(0xffff, "DANGER!");
        }

        fflush(stdout);
	    writeLog(10, buffer);
        checkRefs(node->left);
    }
}

void validateTree(const Tree *tree) {
    // Root must be black
    if (tree->root != NULL && tree->root->isRed) {
        writeLog(0xffff, "ERROR: ROOT NOT BLACK");
        printTree(tree->root);
    }
    checkRedChildren(tree->root);
    checkNilDist(tree->root);
    checkRefs(tree->root);
}

Node *grandParent(const Node *node) {
    if (node->parent != NULL) {
        return node->parent->parent;
    }
    return NULL;
}

Node *sibling(const Node *node) {
    if (node->parent != NULL) {
        if (node->parent->left == node) {
            return node->parent->right;
        }
        return node->parent->left;
    }
    return NULL;
}

Node *uncle(const Node *node) {
    if (grandParent(node) != NULL) {
        return sibling(node->parent);
    }
    return NULL;
}

const Node *getRoot(const Node *node) {
	if (node->parent == NULL) {
		return node;
	}
	return getRoot(node->parent);
}


void printTree(const Node *node) {
    if (node == NULL) {
        return;
    } else {
        printTree(node->right);
        printNode(node);
        printTree(node->left);
    }
}

void printNode(const Node *node) {
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

void rotateLeft(Node *node) {
    Node *new = node->right;
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

void rotateRight(Node *node) {
    Node *new = node->left;
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

Node **maxNode(Node **node) {
    if ((*node)->right == NULL) {
        return node;
    }
    return maxNode(&(*node)->right);
}

Node **minNode(Node **node) {
    if ((*node)->left== NULL) {
        return node;
    }
    return minNode(&(*node)->left);
}

int initTree(Tree *tree) {
    tree->root = NULL;
    tree->count = 0;
    return 0;
}

void freeNodes(Node *node) {
    if (node != NULL) {
        freeNodes(node->left);
        freeNodes(node->right);
        free(node);
    }
}

int closeTree(Tree *tree) {
    freeNodes(tree->root);
    return 0;
}

int clearTree(Tree *tree) {
    freeNodes(tree->root);
    tree->root = NULL;
    tree->count = 0;
    return 0;
}

Node *searchTree(Node *node, int id) {
    if (node == NULL) {
        return NULL;
    }
    if (id > node->id) {
        return searchTree(node->right, id);
    } else if (id < node->id) {
        return searchTree(node->left, id);
    }
    return node;
}

Node *getTreeNode(Tree *tree, int id) {
    return searchTree(tree->root, id);
}

void *getData(Tree *tree, int id) {
    Node *n = searchTree(tree->root, id);
    if (n == NULL) {
        return n;
    }
    return searchTree(tree->root, id)->data;
}

Node *newNode(void *data, int id) {
    Node *newNode = malloc(sizeof(Node));
    newNode->data = data;
    newNode->id = id;
    newNode->isRed = 1;

    newNode->left = NULL;
    newNode->right = NULL;
    newNode->parent = NULL;

    newNode->tree = NULL;

    return newNode;
}


int addNodeToTree(Node *node, Node *new) {
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

