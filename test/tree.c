#include "../src/game/game.h"
#include "../src/term/screen.h"
#include "../src/log/log.h"

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#include "../src/list/rbTree.h"

void terminate(int a) {
    closeScreen();
    closeLog();
    exit(0);
}

int main(int argc, char **argv) {
    initLog();
    signal(SIGINT, terminate);

    int i;
    struct Tree tree;
    initTree(&tree);

	setLevel(10000);
    for (i = 1; i <= 100; i++) {
        insert(&tree, NULL, i);
    }
    for (i = 200; i > 100; i--) {
        insert(&tree, NULL, i);
    }
    for (i = 201; i <= 300; i++) {
        insert(&tree, NULL, i);
    }
	setLevel(1);

    printf("Tree: %d\n", tree.count);
    printf("Expect 300\n");

	setLevel(1);
    for (i = 0; i <= 200; i += 2) {
        removeId(&tree, i);
    }
	setLevel(1);

    printf("Tree: %d\n", tree.count);
    printf("Expect 200\n");

	setLevel(10000);
    for (i = 301; i <= 400; i++) {
        insert(&tree, NULL, i);
    }
	setLevel(1);
    printTree(tree.root);
    printf("Tree: %d\n", tree.count);
    printf("Expect 300\n");

	setLevel(10000);
    for (i = 25; i < 75; i += 2) {
        removeId(&tree, i);
    }
	setLevel(1);
    printTree(tree.root);
    printf("Tree: %d\n", tree.count);
    printf("Expect 275\n");

    for (i = 100; i >= 0; i--) {
        removeId(&tree, i);
    }
    printTree(tree.root);
    printf("Tree: %d\n", tree.count);
    printf("Expect 250\n");

    for (i = 300; i >= 0; i--) {
        removeId(&tree, i);
    }
    printTree(tree.root);
    printf("Tree: %d\n", tree.count);
    printf("Expect 0\n");

    return 0;
}
