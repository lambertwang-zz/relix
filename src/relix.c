#include "game/game.h"
#include "term/screen.h"

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#include "list/rbTree.h"

void terminate(int a) {
    closeScreen();
    exit(0);
}

void printTree(struct Node *node) {
    if (node == NULL) {
        return;
    }
    printTree(node->left);

    printf("id: %d, left: %d, right %d\n", node->id, 
        node->left != NULL ? node->left->id : -1, 
        node->right != NULL ? node->right->id : -1);
    printTree(node->right);
}

int main(int argc, char **argv) {
    signal(SIGINT, terminate);
    /*
    int i;
    struct RbTree tree;
    initRbTree(&tree);
    for (i = 0; i <= 10 ; i++) {
        addData(&tree, NULL, i);
    }
    for (i = 30; i > 10; i--) {
        addData(&tree, NULL, i);
    }
    for (i = 31; i < 50; i++) {
        addData(&tree, NULL, i);
    }
    printTree(tree.root);
    printf("Tree: %d\n", tree.count);
    for (i = 0; i < 50; i += 2) {
        removeData(&tree, i);
    }
    printTree(tree.root);
    printf("Tree: %d\n", tree.count);
    for (i = 50; i < 80; i++) {
        addData(&tree, NULL, i);
    }
    printTree(tree.root);
    printf("Tree: %d\n", tree.count);
    for (i = 25; i < 75; i += 2) {
        removeData(&tree, i);
    }
    printTree(tree.root);
    printf("Tree: %d\n", tree.count);
    */

    initScreen();

    loop();

    closeScreen();

    return 0;
}
