
#include "tree.h"

#include <stdlib.h>

void delete1(struct Node *node);
void delete2(struct Node *node);
void delete3(struct Node *node);
void delete4(struct Node *node);
void delete5(struct Node *node);
void delete6(struct Node *node);

// replaces the values inside the node
// Dest is retained, target must be freed
void replaceNode(struct Node *dest, struct Node *target) {
    dest->data = target->data;
    dest->id = target->id;
}

// Swaps all references to the nodes 
// target is retained, dest must be freed
void swapNode(struct Node **dest, struct Node *target) {
    target->parent = (*dest)->parent;
    if ((*dest)->parent != NULL) {
        if ((*dest)->parent->left == *dest) {
            (*dest)->parent->left = target;
        } else {
            (*dest)->parent->right= target;
        }
    }
	if ((*dest)->tree != NULL) {
		target->tree = (*dest)->tree;
	}
    (*dest) = target;
}

// Node is root and black. Done
void delete1(struct Node *node) {
    if (node->parent != NULL) {
        delete2(node);
    }
}

void delete2(struct Node *node) {
   	struct Node *s = sibling(node);

    if (s != NULL && s->isRed) {
        node->parent->isRed = 1;
        s->isRed = 0;
        if (node == node->parent->left) {
            rotateLeft(node->parent);
        } else {
            rotateRight(node->parent);
        }
    }
    delete3(node);
}

void delete3(struct Node *node) {
    struct Node *s = sibling(node);

    if (!node->parent->isRed &&
        !s->isRed &&
        (s->left == NULL || !s->left->isRed) &&
        (s->right == NULL || !s->right->isRed)) {
        s->isRed= 1;
        delete1(node->parent);
    } else {
        delete4(node);
    }
}

void delete4(struct Node *node) {
    struct Node *s = sibling(node);
    if (node->parent->isRed &&
        (s == NULL || !s->isRed) &&
        (s->left == NULL || !s->left->isRed) &&
        (s->right == NULL || !s->right->isRed)) {
        s->isRed = 1;
        node->parent->isRed = 0;
    } else {
        delete5(node);
    }
}

void delete5(struct Node *node) {
    struct Node *s = sibling(node);

    if  (!s->isRed) { /* this if statement is trivial,
                         due to case 2 (even though case 2 changed the sibling to a sibling's child,
                         the sibling's child can't be red, since no red parent can have a red child). */
        /* the following statements just force the red to be on the left of the left of the parent,
         *    or right of the right, so case six will rotate correctly. */
        if (node == node->parent->left &&
            (s->right == NULL || !s->right->isRed) &&
            (s->left != NULL && s->left->isRed)) { /* this last test is trivial too due to cases 2-4. */
            s->isRed = 1;
            s->left->isRed= 0;
            rotateRight(s);
        } else if (node == node->parent->right &&
            (s->left == NULL || !s->left->isRed) &&
            (s->right != NULL && s->right->isRed)) {/* this last test is trivial too due to cases 2-4. */
            s->isRed = 1;
            s->right->isRed= 0;
            rotateLeft(s);
        }
    }
    delete6(node);
}

void delete6(struct Node *node) {
    struct Node *s = sibling(node);

    s->isRed = node->parent->isRed;
    node->parent->isRed = 0;
  
    if (node == node->parent->left) {
        s->right->isRed = 0;
        rotateLeft(node->parent);
    } else {
        s->left->isRed= 0;
        rotateRight(node->parent);
    }
}

// At most one child
int removeNodeOneChild(struct Node **indirect) {
	struct Node *node = *indirect;
    struct Node *child = node->left == NULL ? node->right : node->left;
    if (child == NULL) {
        if (!node->isRed) {
            delete1(node);
        }
        *indirect = NULL;
        free(node);
		return 0;
    }

    swapNode(indirect, child);
    if (!node->isRed) {
        if (child->isRed) {
            child->isRed = 0;
        } else {
            delete1(child);
        }
    }
    free(node);
	return 0;
}

void removeNode(struct Node **indirect) {
    struct Node *node = *indirect;
    if (node->left != NULL && node->right != NULL) {
		struct Node **target = min(&node->right);
        replaceNode(node, *target);
        indirect = target;
    }
    removeNodeOneChild(indirect);
}


int removeFromTree(struct Node **node, unsigned int id) {
    if (*node == NULL) {
        return -1;
    }
    if ((*node)->id > id) {
        return removeFromTree(&(*node)->left, id);
    }
    if ((*node)->id < id) {
        return removeFromTree(&(*node)->right, id);
    }
    removeNode(node);
    return 0;
}

int removeId(struct Tree *tree, unsigned int id) {
    if (tree->root == NULL) {
#ifdef DEBUG_TREES
        validateTree(tree);
#endif
        return 1;
    }
    if (!removeFromTree(&tree->root, id)) {
        tree->count--;
#ifdef DEBUG_TREES
        validateTree(tree);
#endif
        return 0;
    }
#ifdef DEBUG_TREES
    validateTree(tree);
#endif
    return -1;
}

