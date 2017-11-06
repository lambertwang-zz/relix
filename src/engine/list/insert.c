#include "tree.h"

#include <stdlib.h>

int insertRecurse(struct Node *node, struct Node *new) {
    if (node->id == new->id) {
        return 1;
    }
    if (node->id > new->id) {
        if (node->left == NULL) {
            node->left = new;
            new->parent = node;
            return 0;
        }
        return insertRecurse(node->left, new);
    }
    if (node->right == NULL) {
        node->right = new;
        new->parent = node;
        return 0;
    }
    return insertRecurse(node->right, new);
}

void insert1(struct Node *node) {
    return;
}

struct Node *insert2(struct Node *node) {
    node->parent->isRed = 0;
    uncle(node)->isRed = 0;
    struct Node *g = grandParent(node);
    g->isRed = 1;
	
	return g;
}

void insert3_next(struct Node *node) {
    struct Node *p = node->parent;
    struct Node *g = grandParent(node);
    
    if (node == p->left) {
        rotateRight(g);
    } else {
        rotateLeft(g);
    }
    p->isRed = 0;
    g->isRed = 1;
}

void insert3(struct Node *node) {
    struct Node *g = grandParent(node);

    if (g->left != NULL && node == g->left->right) {
        rotateLeft(node->parent);
        node = node->left;
    } else if (g->right != NULL && node == g->right->left) {
        rotateRight(node->parent);
        node = node->right; 
    }

    insert3_next(node);
}

struct Node *insert_repair(struct Node *node) {
    if (node->parent == NULL) {
        node->isRed = 0;
  		return NULL;
    }
    if (!node->parent->isRed) {
        insert1(node);
        return NULL;
    }
    struct Node *u = uncle(node);
    if (u != NULL && u->isRed) {
        return insert2(node);
    }
    insert3(node);
	return NULL;
}

int insert(struct Tree *tree, void *data, unsigned int id) {
    struct Node *new = newNode(data, id);
    if (tree->root == NULL) {
        tree->root = new;
        tree->root->isRed = 0;
        tree->count = 1;
        new->tree = tree;
#ifdef DEBUG_TREES
        validateTree(tree);
#endif
        return 0;
    }

    int result = insertRecurse(tree->root, new);

    if (result) {
        free(new);
#ifdef DEBUG_TREES
        validateTree(tree);
#endif
        return 1;
    }
    tree->count++;

	struct Node *forRepair = new;
	do {
    	forRepair = insert_repair(forRepair);
	} while (forRepair != NULL);

#ifdef DEBUG_TREES
        validateTree(tree);
#endif
    return 0;
}

