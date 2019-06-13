
#include "tree.h"

// Library
#include <stdlib.h>

// Engine
#include "log/log.h"

namespace rlx {

// replaces the values inside the node
// Dest is retained, target must be freed
template <class T>
void Node<T>::replaceNode(Node *dest, Node *target) {
    dest->data = target->data;
}

// Swaps all references to the nodes 
// target is retained, dest must be freed
template <class T>
void Node<T>::swapNode(Node **dest, Node *target) {
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
template <class T>
void Node<T>::delete1() {
    if (parent != NULL) {
        delete2();
    }
}

template <class T>
void Node<T>::delete2() {
   	Node *s = sibling();

    if (s != NULL && s->is_red) {
        parent->is_red = true;
        s->is_red = false;
        if (this == parent->left) {
            parent->rotateLeft();
        } else {
            parent->rotateRight();
        }
    }
    delete3();
}

template <class T>
void Node<T>::delete3() {
    Node *s = sibling();

    if (!parent->is_red &&
        !s->is_red &&
        (s->left == NULL || !s->left->is_red) &&
        (s->right == NULL || !s->right->is_red)) {
        s->is_red= true;
        parent->delete1();
    } else {
        delete4();
    }
}

template <class T>
void Node<T>::delete4() {
    Node *s = sibling();
    if (parent->is_red &&
        (s == NULL || !s->is_red) &&
        (s->left == NULL || !s->left->is_red) &&
        (s->right == NULL || !s->right->is_red)) {
        s->is_red = true;
        parent->is_red = false;
    } else {
        delete5();
    }
}

template <class T>
void Node<T>::delete5() {
    Node *s = sibling();

    if  (!s->is_red) {
        /*  this if statement is trivial, due to case 2 (even though case 2
            changed the sibling to a sibling's child, the sibling's child can't
            be red, since no red parent can have a red child). */
        /*  the following statements just force the red to be on the left of
            the left of the parent, or right of the right, so case six will
            rotate correctly. */
        if (this == parent->left &&
            (s->right == NULL || !s->right->is_red) &&
            (s->left != NULL && s->left->is_red)) { /* this last test is trivial too due to cases 2-4. */

            s->is_red = true;
            s->left->is_red= false;
            s->rotateRight();
        } else if (this == parent->right &&
            (s->left == NULL || !s->left->is_red) &&
            (s->right != NULL && s->right->is_red)) {/* this last test is trivial too due to cases 2-4. */

            s->is_red = true;
            s->right->is_red= false;
            s->rotateLeft();
        }
    }
    delete6();
}

template <class T>
void Node<T>::delete6() {
    Node *s = sibling();

    s->is_red = parent->is_red;
    parent->is_red = false;
  
    if (this == parent->left) {
        s->right->is_red = false;
        parent->rotateLeft();
    } else {
        s->left->is_red= false;
        parent->rotateRight();
    }
}

// At most one child
template <class T>
T *Node<T>::removeNodeOneChild(Node<T> **indirect) {
    T *target = NULL;
	Node *node = *indirect;
    Node *child = node->left == NULL ? node->right : node->left;
    if (child == NULL) {
        if (!node->is_red) {
            node->delete1();
        }
        *indirect = NULL;
        target = node->getData();
        delete node;
        return target;
    }

    swapNode(indirect, child);
    if (!node->is_red) {
        if (child->is_red) {
            child->is_red = 0;
        } else {
            child->delete1();
        }
    }
    target = node->getData();
    delete node;
    return target;
}

template <class T>
T *Node<T>::removeNode(Node<T> **indirect) {
    Node<T> *node = *indirect;
    if (node->left != NULL && node->right != NULL) {
		Node **target = minNode(&node->right);
        replaceNode(node, *target);
        indirect = target;
    }
    return removeNodeOneChild(indirect);
}


template <class T>
T *Node<T>::removeFromTree(Node<T> **node, unsigned long id) {
    if (*node == NULL || (*node)->getId() < 0) {
        return NULL;
    }
    if ((*node)->getId() > id) {
        return removeFromTree(&(*node)->left, id);
    }
    if ((*node)->getId() < id) {
        return removeFromTree(&(*node)->right, id);
    }
    return removeNode(node);
}

template <class T>
T *Tree<T>::removeId(unsigned long id) {
#ifdef DEBUG_TREES
    writeLog(0xffff, "Inserting node with id %lu", id);
#endif

    if (root == NULL) {

#ifdef DEBUG_TREES
        validate();
#endif

        return NULL;
    }

    T *target = Node<T>::removeFromTree(&root, id);
    if (target != NULL) {
        count--;

#ifdef DEBUG_TREES
        validate();
#endif

        return target;
    }

#ifdef DEBUG_TREES
    validate();
#endif

    return NULL;
}

}
