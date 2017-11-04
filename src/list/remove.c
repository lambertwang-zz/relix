
#include "rbTree.h"
#include "../log/log.h"

#include <stdlib.h>

#include <stdio.h> // remove

void delete1(struct Node *node);
void delete2(struct Node *node);
void delete3(struct Node *node);
void delete4(struct Node *node);
void delete5(struct Node *node);
void delete6(struct Node *node);

// replaces the values inside the node
// Dest is retained, target must be freed
void replaceNode(struct Node *dest, struct Node *target) {
	writeLog(10, "Dest %d, target %d", dest->id, target->id);
    dest->data = target->data;
    dest->id = target->id;
    if (target->parent != NULL) {
        if (target->parent->left == target) {
            target->parent->left = NULL;
        } else {
            target->parent->right= NULL;
        }
    }
}

// Swaps all references to the nodes 
// target is retained, dest must be freed
void swapNode(struct Node **dest, struct Node *target) {
	writeLog(10, "Dest %d, target %d", (*dest)->id, target->id);
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
    // if (target->parent != NULL) {
    //     if (target->parent->left == target) {
    //         target->parent->left = NULL;
    //     } else {
    //         target->parent->right= NULL;
    //     }
    // }
    (*dest) = target;
}

// Node is root and black. Done
void delete1(struct Node *node) {
	writeLog(10, "delete1");
    if (node->parent != NULL) {
        delete2(node);
    }
}

void delete2(struct Node *node) {
	writeLog(10, "delete2");
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
	writeLog(10, "delete3");
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
	writeLog(10, "delete4");
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
	writeLog(10, "delete5");
    struct Node *s = sibling(node);

    if  (!s->isRed) { /* this if statement is trivial,
                         due to case 2 (even though case 2 changed the sibling to a sibling's child,
                         the sibling's child can't be red, since no red parent can have a red child). */
        /* the following statements just force the red to be on the left of the left of the parent,
         *    or right of the right, so case six will rotate correctly. */
        if (node == node->parent->left &&
            !s->right->isRed &&
            s->left->isRed) { /* this last test is trivial too due to cases 2-4. */
            s->isRed = 1;
            s->left->isRed= 0;
            rotateRight(s);
        } else if (node == node->parent->right &&
            !s->left->isRed &&
            s->right->isRed) {/* this last test is trivial too due to cases 2-4. */
            s->isRed = 1;
            s->right->isRed= 0;
            rotateLeft(s);
        }
    }
    delete6(node);
}

void delete6(struct Node *node) {
	writeLog(10, "delete6");
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
		writeLog(10, "No children. Node will not be missed");
        *indirect = NULL;
        free(node);
		return 0;
    }

	writeLog(10, "Parent of 1");
    swapNode(indirect, child);
	if (child->parent != NULL) {
		printTree(child->parent);
	} else {
		printTree(child);
	}
    if (!node->isRed) {
        if (child->isRed) {
            child->isRed = 0;
        } else {
            delete1(child);
        }
    }
	writeLog(10, "Attempting free now");
    free(node);
	return 0;
}

void removeNode(struct Node **node) {
	printTree(*node);
    if ((*node)->left != NULL && (*node)->right != NULL) {
		writeLog(10, "Two children: performing replace");
		struct Node *target = min((*node)->right);
		writeLog(10, "Found target %d", target->id);
        replaceNode(*node, target);
		writeLog(10, "replace successful");
		if (target->parent != *node) {
			writeLog(10, "Adopting target's children");
			target->parent->left = target->right;
		}
		writeLog(10, "Attempting free");
		free(target);
    } else {
		writeLog(10, "One or less child");
        removeNodeOneChild(node);
    }
	writeLog(10, "RemoveNode done?");
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
	writeLog(10, "Found node %d", (*node)->id);
    removeNode(node);
    return 0;
}

int removeId(struct Tree *tree, unsigned int id) {
	writeLog(10, "Removing %d", id);
    if (tree->root == NULL) {
		writeLog(10, "Tree empty", id);
        return 1;
    }
    if (!removeFromTree(&tree->root, id)) {
        tree->count--;
		writeLog(10, "Remove successful");
		printTree(tree->root);
        return 0;
    }
    return -1;
}

