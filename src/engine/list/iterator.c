#include "tree.h"

// Library
#include <stdlib.h>

Iterator initIterator(Tree *tree) {
    Iterator new;

    new.tree = tree;
    new.current = NULL;
    new.index = 0;

    return new;
}

struct Node *getNode(const Iterator *it) {
    return it->current;
}

void *getItem(const Iterator *it) {
    return it->current->data;
}

struct Node *getNext(Iterator *it) {
    if (done(it)) {
        return NULL;
    }
    /* using stack
    while (it->current != NULL) {
        push(&it->stack, it->current);
        it->current = it->current->left;
    }
    if (it->stack.count) {
        Node *temp;
        it->current = pop(&it->stack);
        temp = it->current;
        it->current = it->current->right;
        it->index++;
        return temp;
    } else {
        return NULL;
    } */

    // without stack
    if (it->current == NULL) {
        // First node?
        it->current = it->tree->root;
        while (it->current->left != NULL) {
            it->current = it->current->left;
        }
    } else if (it->current->right == NULL) {
        // Subtree max
        // Travel up
        Node *tmp;
        do {
            tmp = it->current;
            it->current = it->current->parent;
        } while (it->current != NULL && it->current->right == tmp);
    } else {
        // Not subtree max
        // Find subtree max
        it->current = it->current->right;
        while (it->current->left != NULL) {
            it->current = it->current->left;
        }
    }
    it->index++;
    return it->current;
}

int done(const Iterator *it) {
    return it->index >= it->tree->count ? 1 : 0;
}
