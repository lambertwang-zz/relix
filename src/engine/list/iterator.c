#include "tree.h"

#include <stdlib.h>
#include <string.h>

Iterator *initIterator(Tree *tree) {
    Iterator *new = malloc(sizeof(Iterator));

    new->tree = tree;

    initArray(&new->stack);
    new->current = tree->root;

    new->index = 0;

    return new;
}

int closeIterator(Iterator *it) {
    closeArray(&it->stack);
    free(it);
    return 0;
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
    }

    /*
    if (it->index == 0) {
        it->current = it->tree->root;
        writeLog(10, "initializing tree");
        while (it->current->left != NULL) {
            writeLog(10, "Finding min");
            printTree(it->current);
            // pushStack(it, it->current);
            it->current = it->current->left;
        }
    } else if (it->current->right == NULL) {
        writeLog(10, "No right, walk up");

        const struct Node *temp;
        do {
            writeLog(10, "Walk up");
            printTree(it->current);
            temp = it->current;
            it->current = it->current->parent; //popStack(it);
        } while (it->current != NULL && it->current->right == temp);
    } else {
        writeLog(10, "Has right, find min");
        // pushStack(it, it->current);
        it->current = it->current->right;

        while (it->current->left != NULL) {
            writeLog(10, "Finding min");
            printTree(it->current);
            // pushStack(it, it->current);
            it->current = it->current->left;
        }
    }
    writeLog(10, "Found %d", it->current->id);
    printTree(it->current);
    it->index++;
    return it->current;
    */
}

void generate(Iterator *it) {
    it->current = it->tree->root;
    while (!done(it)) {
        if (it->current != NULL) {
            push(&it->stack, it->current);
            it->current = it->current->left;
        } else {
            if (it->stack.count) {
                it->current = pop(&it->stack);
                it->index++;
                it->current = it->current->right;
            } else {
                continue;
            }
        }
    }
}

int done(const Iterator *it) {
    return it->index >= it->tree->count ? 1 : 0;
}
