#include "tree.h"

// Library
#include <stdlib.h>

namespace rlx {

template <class T>
Iterator<T>::Iterator(const Tree<T> *init_tree) {
    tree = init_tree;
    current = NULL;
    index = 0;
}

template <class T>
Iterator<T>& Iterator<T>::operator=(const Iterator<T> &other) {
    tree = other.tree;
    current = other.current;
    index = other.index;
    return *this;
}

template <class T>
Node<T> *Iterator<T>::getNode() const {
    return current;
}

template <class T>
T *Iterator<T>::getItem() const {
    return current->data;
}

template <class T>
Node<T> *Iterator<T>::getNextNode() {
    if (done()) {
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
    if (current == NULL) {
        // First node?
        current = tree->root;
        while (current->left != NULL) {
            current = current->left;
        }
    } else if (current->right == NULL) {
        // Subtree max
        // Travel up
        Node<T> *tmp;
        do {
            tmp = current;
            current = current->parent;
        } while (current != NULL && current->right == tmp);
    } else {
        // Not subtree max
        // Find subtree max
        current = current->right;
        while (current->left != NULL) {
            current = current->left;
        }
    }
    index++;
    return current;
}

template <class T>
T *Iterator<T>::getNext() {
    Node<T> *node = getNextNode();
    if (node == NULL) {
        return NULL;
    }

    return node->getData();
}

template <class T>
bool Iterator<T>::done() const {
    return index >= tree->count ? true : false;
}

}
