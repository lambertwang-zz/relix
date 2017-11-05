#include "tree.h"

#include "../log/log.h"

#include <stdlib.h>
#include <string.h>

void pushStack(struct Iterator *it, struct Node *node) {
    if (it->stack_count >= it->stack_size) {
        struct Node **temp = malloc(sizeof(struct Node *) * it->stack_size * 2);
        memcpy(temp, it->stack, sizeof(struct Node *) * it->stack_size);
        free(it->stack);
        it->stack = temp;
        it->stack_size *= 2;
    }

    it->stack[it->stack_count++] = node;
}

struct Node *popStack(struct Iterator *it) {
    return it->stack[--it->stack_count];
}

struct Iterator *initIterator(struct Tree *tree) {
    struct Iterator *new = malloc(sizeof(struct Iterator));

    new->tree = tree;

    new->stack = malloc(sizeof(struct Node *) * INIT_STACK_SIZE);
    new->stack_size = INIT_STACK_SIZE;
    new->stack_count = 0;
    new->current = tree->root;

    new->index = 0;

    return new;
}

struct Node *getNode(const struct Iterator *it) {
    return it->current;
}

void *getItem(const struct Iterator *it) {
    return it->current->data;
}

struct Node *getNext(struct Iterator *it) {
    if (done(it)) {
        return NULL;
    }
    while (it->current != NULL) {
        pushStack(it, it->current);
        it->current = it->current->left;
    }
    
    if (it->stack_count) {
        struct Node *temp;
        it->current = popStack(it);
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

void generate(struct Iterator *it) {
    it->current = it->tree->root;
    while (!done(it)) {
        if (it->current != NULL) {
            pushStack(it, it->current);
            it->current = it->current->left;
        } else {
            if (it->stack_count) {
                it->current = popStack(it);
                printf("%d\n", it->current->id);
                it->index++;
                it->current = it->current->right;
            } else {
                continue;
            }
        }
    }
}

int done(const struct Iterator *it) {
    writeLog(10, "Index: %d count: %d", it->index, it->tree->count);
    return it->index >= it->tree->count ? 1 : 0;
}

int closeIterator(struct Iterator *it) {
    free(it->stack);
    free(it);
    return 0;
}

