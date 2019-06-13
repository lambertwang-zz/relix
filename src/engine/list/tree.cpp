#include "tree.h"

// Library
#include <stdlib.h>
#include <stdio.h>

// Engine
#include "log/log.h"
#include "constants.h"

namespace rlx {

template <class T>
void Tree<T>::validate() const {
    // Root must be black
    if (root != NULL && root->is_red) {
        writeLog(0xffff, "ERROR: ROOT NOT BLACK");
        root->printTree();
    }
    Node<T>::checkRedChildren(root);
    Node<T>::checkNilDist(root);
    Node<T>::checkRefs(root);
}

template <class T>
Tree<T>::Tree() {
    root = NULL;
    count = 0;
}

template <class T>
void Tree<T>::freeNodes(Node<T> *node) {
    if (node != NULL) {
        freeNodes(node->left);
        freeNodes(node->right);
        delete node;
    }
}

template <class T>
Tree<T>::~Tree() {
    freeNodes(root);
}

template <class T>
void Tree<T>::clear() {
    freeNodes(root);
    root = NULL;
    count = 0;
}

template <class T>
Node<T> *Tree<T>::getNode(unsigned long id) const {
    return Node<T>::search(root, id);
}

template <class T>
T *Tree<T>::get(unsigned long id) const {
    const Node<T> *n = Node<T>::search(root, id);
    if (n == NULL) {
        return n->data;
    }
    return n->data;
}

template <class T>
int Tree<T>::merge(const Tree<T> *other) {
    Iterator<T> it(other);

    while (!it.done()) {
        T *data= it.getNext();
        if (insert(data)) {
            writeLog(LOG_ObjectManager,
                "Tree::merge(): Error adding object with id: %d",
                data->getId());

        }
    }

    return 0;
}

}
