#include "log/log.h"
#include "list/tree.h"

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
    initLog();

    int i;
    Tree tree;
    Iterator it;
    initTree(&tree);

    it = initIterator(&tree);
    while (!done(&it)) {
        printf("id: %d\n", getNext(&it)->id);
    }

    for (i = 1; i <= 100; i++) {
        insert(&tree, NULL, i);
    }
    for (i = 200; i > 100; i--) {
        insert(&tree, NULL, i);
    }

    it = initIterator(&tree);
    int sum = 0;
    while (!done(&it)) {
        Node *next = getNext(&it);
        printf("id: %d\t", next->id);
        sum += next->id;
    }
    printf("\nExpect sum: %d to be %d\n", sum, 200 * 201 / 2);

    for (i = 201; i <= 300; i++) {
        insert(&tree, NULL, i);
    }
    printf("Tree: %d\n", tree.count);
    printf("Expect 300\n");

    for (i = 0; i <= 200; i += 2) {
        removeId(&tree, i);
    }
    printf("Tree: %d\n", tree.count);
    printf("Expect 200\n");

    for (i = 301; i <= 400; i++) {
        insert(&tree, NULL, i);
    }
    printf("Tree: %d\n", tree.count);
    printf("Expect 300\n");
    
    it = initIterator(&tree);
    while (!done(&it)) {
        printf("id: %d\t", getNext(&it)->id);
    }
    printf("\n");

    for (i = 25; i < 75; i += 2) {
        removeId(&tree, i);
    }
    printf("Tree: %d\n", tree.count);
    printf("Expect 275\n");

    for (i = 100; i >= 0; i--) {
        removeId(&tree, i);
    }
    printf("Tree: %d\n", tree.count);
    printf("Expect 250\n");

    for (i = 400; i >= 0; i--) {
        removeId(&tree, i);
    }
    printf("Tree: %d\n", tree.count);
    printf("Expect 0\n");

    closeLog();

    return 0;
}
