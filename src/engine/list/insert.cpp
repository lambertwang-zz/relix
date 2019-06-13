
#include "tree.h"

// Library
#include <stdlib.h>

// Engine
#include "log/log.h"

namespace rlx {

template <class T>
int Node<T>::insertRecurse(Node<T> *n) {
    if (getId() == n->getId()) {
        return 1;
    }
    if (getId() > n->getId()) {
        if (left == NULL) {
            left = n;
            n->parent = this;
            return 0;
        }
        return left->insertRecurse(n);
    }
    if (right == NULL) {
        right = n;
        n->parent = this;
        return 0;
    }
    return right->insertRecurse(n);
}

/*
void insert1(Node *node) {
    return;
}
*/

template <class T>
Node<T> *Node<T>::insert2() {
    parent->is_red = 0;
    uncle()->is_red = 0;
    Node<T> *g = grandParent();
    g->is_red = 1;
	
	return g;
}

template <class T>
void Node<T>::insert3_next() {
    Node<T> *p = parent;
    Node<T> *g = grandParent();
    
    if (this == p->left) {
        g->rotateRight();
    } else {
        g->rotateLeft();
    }
    p->is_red = 0;
    g->is_red = 1;
}

template <class T>
void Node<T>::insert3() {
    Node<T> *g = grandParent();
    Node<T> *n = this;

    if (g->left != NULL && this == g->left->right) {
        parent->rotateLeft();
        n = n->left;
    } else if (g->right != NULL && this == g->right->left) {
        parent->rotateRight();
        n = n->right; 
    }

    n->insert3_next();
}

template <class T>
Node<T> *Node<T>::insert_repair() {
    if (parent == NULL) {
        is_red = 0;
  		return NULL;
    }
    if (!parent->is_red) {
        // insert1(node);
        return NULL;
    }
    Node<T> *u = uncle();
    if (u != NULL && u->is_red) {
        return insert2();
    }
    insert3();
	return NULL;
}

template <class T>
int Tree<T>::insert(T *data) {

#ifdef DEBUG_TREES
    writeLog(0xffff, "Inserting node with id %lu", data->getId());
#endif

    Node<T> *n = new Node<T>(this, data);
    if (root == NULL) {
        root = n;
        root->is_red = false;
        count = 1;

#ifdef DEBUG_TREES
        validate();
#endif

        return 0;
    }

    int result = root->insertRecurse(n);

    if (result) {
        delete n;

#ifdef DEBUG_TREES
        validate();
#endif

        return 1;
    }
    count++;

	Node<T> *forRepair = n;
	do {
    	forRepair = forRepair->insert_repair();
	} while (forRepair != NULL);

#ifdef DEBUG_TREES
    validate();
#endif

    return 0;
}

}
