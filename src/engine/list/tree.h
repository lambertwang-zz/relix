#ifndef __TREE_H__
#define __TREE_H__

// Engine
#include "array.h"
#include "constants.h"

#define INIT_STACK_SIZE 16

namespace rlx {
    template <class T>
    class Node;

    template <class T>
    class Tree;

    template <class T>
    class Iterator;

    template <class T>
    class Node {
    private:
        friend class Tree<T>;
        friend class Iterator<T>;
        T *data;
        bool is_red;

        Node<T> *left;
        Node<T> *right;
        Node<T> *parent;

        Tree<T> *tree;

        Node<T> *grandParent() const;
        Node<T> *sibling() const;
        Node<T> *uncle() const;
        Node<T> *getRoot();

        void rotateLeft();
        void rotateRight();

        // Insertion helpers
        int insertRecurse(Node<T> *n);

        Node<T> *insert2();
        void insert3_next();
        void insert3();
        Node<T> *insert_repair();

        // Deletion helpers
        void delete1();
        void delete2();
        void delete3();
        void delete4();
        void delete5();
        void delete6();

        static void replaceNode(Node<T> *dest, Node<T> *target);
        static void swapNode(Node<T> **dest, Node<T> *target);
        static T *removeNodeOneChild(Node<T> **indirect);
        static T *removeNode(Node<T> **indirect);
        static T *removeFromTree(Node<T> **node, unsigned long id);

        static Node<T> **maxNode(Node<T> **node);
        static Node<T> **minNode(Node<T> **node);
        static Node<T> *search(Node<T> *node, unsigned long id);

        int depth() const;
        static int checkRedChildren(const Node<T> *node);
        static int minDistToNil(const Node<T> *node);
        static int maxDistToNil(const Node *node);
        static int checkNilDist(const Node *node);
        static void checkRefs(const Node *node);

        void print() const;
        void printTree() const;
    public:
        Node(Tree<T> *init_root, T *init_data);

        unsigned long getId() const { return data->getId(); }
        T *getData() const { return data; }
    };

    template <class T>
    class Tree: public Unique {
    private:
        friend class Node<T>;
        friend class Iterator<T>;
        Node<T> *root;
        unsigned int count;

        static void freeNodes(Node<T> *node);

    public:
        Tree();
        ~Tree();
        void clear();

        unsigned int getCount() const {
            return count;
        }

        void validate() const;

        T *get(unsigned long id) const;
        Node<T> *getNode(unsigned long id) const;

        int insert(T *data);
        T *remove(const T *data);
        T *removeId(unsigned long id);

        int merge(const Tree<T> *other);
    };

    template <class T>
    class Iterator {
    private:
        const Tree<T> *tree;

        unsigned int index;
        Node<T> *current;
    public:
        Iterator(const Tree<T> *init_tree);
        Iterator<T>& operator=(const Iterator<T> &other);

        Node<T> *getNode() const;
        T *getItem() const;
        Node<T> *getNextNode();
        T *getNext();
        bool done() const;
    };

    template class Node<Unique>;
    template class Tree<Unique>;
    template class Iterator<Unique>;
}

#endif
