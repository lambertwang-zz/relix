#include "tree.h"

// Library
#include <stdlib.h>
#include <stdio.h>

// Engine
#include "log/log.h"
#include "constants.h"

namespace rlx {

template <class T>
Node<T>::Node(Tree<T> *init_tree, T *init_data) {
    data = init_data;
    is_red = true;

    left = NULL;
    right = NULL;
    parent = NULL;

    tree = init_tree;
}

template <class T>
int Node<T>::depth() const {
	if (parent == NULL) {
		return 0;
	}
	return 1 + parent->depth();
}

template <class T>
int Node<T>::checkRedChildren(const Node<T> *node) {
    if (node == NULL) {
        return 0;
    }
    if (node->is_red) {
        if ((node->left != NULL && node->left->is_red) ||
            (node->right != NULL && node->right->is_red)) {
            writeLog(0xffff, "ERROR: RED CHILDREN MUST BE BLACK");
            node->printTree();
            return 1;
        }
    }
    return checkRedChildren(node->left) + checkRedChildren(node->right);
}

template <class T>
int Node<T>::minDistToNil(const Node<T> *node) {
    if (node == NULL) {
        return 0;
    }
    int distLeft = minDistToNil(node->left), distRight = minDistToNil(node->right);
    return (node->is_red ? 0 : 1) + (distLeft < distRight ? distLeft : distRight);
}

template <class T>
int Node<T>::maxDistToNil(const Node *node) {
    if (node == NULL) {
        return 0;
    }
    int distLeft = maxDistToNil(node->left), distRight = maxDistToNil(node->right);
    return (node->is_red ? 0 : 1) + (distLeft > distRight ? distLeft : distRight);
}

template <class T>
int Node<T>::checkNilDist(const Node *node) {
    if (node == NULL) {
        return 0;
    }
    int distLeft = checkNilDist(node->left), distRight = checkNilDist(node->right);
    if (distLeft || distRight) {
        return 0;
    }
    if (minDistToNil(node) != maxDistToNil(node)) {
        writeLog(0xffff, "ERROR: DISTANCE TO NIL INCONSISTENT %d!!", node->getId());
        node->printTree();
        return 1;
    }
    return 0;
}

template <class T>
void Node<T>::checkRefs(const Node *node) {
    if (node != NULL) {
        bool error = false;
        checkRefs(node->right);


        if (node->left != NULL &&
            node->right != NULL &&
            node->left->getId() == node->right->getId()) {

	        writeLog(0xffff, "DANGER! Left and right nodes are the same.");
            error = true;
        }

        if (node->left != NULL &&
            node->parent != NULL &&
            node->left->getId() == node->parent->getId()) {

	        writeLog(0xffff, "DANGER! Left and parent nodes are the same.");
            error = true;
        }

        if (node->parent != NULL &&
            node->right != NULL &&
            node->parent->getId() == node->right->getId()) {

	        writeLog(0xffff, "DANGER! Right and parent nodes are the same.");
            error = true;
        }

        if (!(node->parent == NULL ||
              (node->parent != NULL &&
               node->parent->left != NULL &&
               node->getId() == node->parent->left->getId()) ||
              (node->parent != NULL &&
               node->parent->right!= NULL &&
               node->getId() == node->parent->right->getId()))) {

	        writeLog(0xffff, "DANGER!");
            error = true;
        }

        if (node->left != NULL &&
            node->left->parent->getId() != node->getId()) {

	        writeLog(0xffff, "DANGER! This node is not its left's parent.");
            error = true;
        }

        if (node->right != NULL &&
            node->right->parent->getId() != node->getId()) {

	        writeLog(0xffff, "DANGER! This node is not its right's parent.");
            error = true;
        }

        if (node->tree != NULL &&
            node->tree->root == node &&
            node->parent != NULL) {

	        writeLog(0xffff, "DANGER! Tree root node has parent.");
            error = true;
        }

        if (node->tree == NULL) {
	        writeLog(0xffff, "DANGER! Node has lost its tree.");
            error = true;
        }

        if (error) {
            node->printTree();
            fflush(stdout);
        }

        checkRefs(node->left);
    }
}

template <class T>
Node<T> *Node<T>::grandParent() const {
    if (parent != NULL) {
        return parent->parent;
    }
    return NULL;
}

template <class T>
Node<T> *Node<T>::sibling() const {
    if (parent != NULL) {
        if (parent->left == this) {
            return parent->right;
        }
        return parent->left;
    }
    return NULL;
}

template <class T>
Node<T> *Node<T>::uncle() const {
    if (grandParent() != NULL) {
        return parent->sibling();
    }
    return NULL;
}

template <class T>
Node<T> *Node<T>::getRoot() {
	if (parent == NULL) {
		return this;
	}
	return parent->getRoot();
}

template <class T>
void Node<T>::print() const {
	char buffer[512];
	int i, c = 0;
	for (i = 0; i <= depth(); i++) {
		c += sprintf(buffer + c, "\t");
	}
	sprintf(buffer + c, 
        "%lu %s, P: %lu, L: %lu, R: %lu", 
        getId(), 
        is_red ? "red" : "black",
        parent != NULL ? parent->getId() : 0,
        left != NULL ? left->getId() : 0,
        right != NULL ? right->getId() : 0);

    fflush(stdout);
	writeLog(0xffff, buffer);
}

template <class T>
void Node<T>::printTree() const {
    if (right != NULL) {
        right->printTree();
    }
    print();
    if (left != NULL) {
        left->printTree();
    }
}


template <class T>
void Node<T>::rotateLeft() {
    Node *n = right;
    right = n->left;
	if (right != NULL) {
		right->parent = this;
	}
    n->left = this;
    n->parent = parent;
    if (parent != NULL) {
        if (parent->left == this) {
            parent->left = n;
        } else {
            parent->right = n;
        }
    }
    parent = n;

    if (tree->root == this) {
        tree->root = n;
    }
}

template <class T>
void Node<T>::rotateRight() {
    Node *n = left;
    left = n->right;
	if (left != NULL) {
		left->parent = this;
	}
    n->right = this;
    n->parent = parent;
    if (parent != NULL) {
        if (parent->left == this) {
            parent->left = n;
        } else {
            parent->right = n;
        }
    }
    parent = n;

    if (tree->root == this) {
        tree->root = n;
    }
}

template <class T>
Node<T> **Node<T>::maxNode(Node<T> **node) {
    if ((*node)->right == NULL) {
        return node;
    }
    return maxNode(&(*node)->right);
}

template <class T>
Node<T> **Node<T>::minNode(Node<T> **node) {
    if ((*node)->left== NULL) {
        return node;
    }
    return minNode(&(*node)->left);
}

template <class T>
Node<T> *Node<T>::search(Node<T> *node, unsigned long id) {
    if (node == NULL) {
        return NULL;
    }
    if (id > node->getId()) {
        return search(node->right, id);
    } else if (id < node->getId()) {
        return search(node->left, id);
    }
    return node;
}

template <class T>
int addNodeToTree(Node<T> *node, Node<T> *new_node) {
    if (node->getId() > new_node->getId()) {
        if (node->left == NULL) {
            node->left = new_node;
            new_node->parent = node->left;
            return 0;
        }
        return addNodeToTree(node->left, new_node);
    }
    if (node->right == NULL) {
        node->right = new_node;
        new_node->parent = node->right;
        return 0;
    }
    return addNodeToTree(node->right, new_node);
}

}
