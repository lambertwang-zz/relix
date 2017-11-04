#include "game/game.h"
#include "term/screen.h"
#include "log/log.h"

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#include "list/rbTree.h"

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
    for (i = 1; i <= 10; i++) {
        insert(&tree, NULL, i);
    }
    for (i = 30; i > 10; i--) {
        insert(&tree, NULL, i);
    }
    for (i = 31; i <= 50; i++) {
        insert(&tree, NULL, i);
    }
	setLevel(1);
    printTree(tree.root);
    printf("Tree: %d\n", tree.count);
    printf("Expect 50\n");
	setLevel(10000);
    for (i = 0; i <= 50; i += 2) {
        removeId(&tree, i);
    }
	setLevel(1);

    printTree(tree.root);
    printf("Tree: %d\n", tree.count);
    printf("Expect 25\n");
    for (i = 51; i <= 80; i++) {
        insert(&tree, NULL, i);
    }
    printTree(tree.root);
    printf("Tree: %d\n", tree.count);
    printf("Expect 55\n");

	setLevel(10000);
    for (i = 25; i < 75; i += 2) {
        removeId(&tree, i);
    }
	setLevel(1);
    printTree(tree.root);
    printf("Tree: %d\n", tree.count);

    for (i = 100; i >= 0; i--) {
        removeId(&tree, i);
    }
    printTree(tree.root);
    printf("Tree: %d\n", tree.count);
    printf("Expect 0\n");

    // initScreen();

    // loop();

    // closeScreen();

    return 0;
}
