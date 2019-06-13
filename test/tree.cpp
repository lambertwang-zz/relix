#include "log/log.h"
#include "list/tree.h"

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

using namespace rlx;

class DummyObj: public rlx::Unique {
    private:
    public: 
        DummyObj(unsigned long init_id) {
            setId(init_id);
        }
};

int main() {
    rlx::initLog();
    rlx::setFlush(true);

    long i;
    rlx::Tree<rlx::Unique> *tree = new Tree<rlx::Unique>();
    rlx::Iterator<rlx::Unique> it = Iterator<rlx::Unique>(tree);
    while (!it.done()) {
        printf("id: %lu\n", it.getNext()->getId());
    }

    for (i = 1; i <= 100; i++) {
        tree->insert(new DummyObj(i));
    }
    for (i = 200; i > 100; i--) {
        tree->insert(new DummyObj(i));
    }

    it = Iterator<Unique>(tree);
    unsigned long sum = 0;
    while (!it.done()) {
        Node<Unique> *next = it.getNextNode();
        printf("id: %lu\t", next->getId());
        sum += next->getId();
    }
    printf("\nExpect sum: %lu to be %d\n", sum, 200 * 201 / 2);

    for (i = 201; i <= 300; i++) {
        tree->insert(new DummyObj(i));
    }
    printf("Tree: %d\n", tree->getCount());
    printf("Expect 300\n");

    for (i = 1; i <= 200; i += 2) {
        Unique *tmp = tree->removeId(i);
        if (tmp == NULL) {
            printf("Failed to delete %ld\n", i);
        } else {
            delete tree->removeId(i);
        }
    }
    printf("Tree: %d\n", tree->getCount());
    printf("Expect 200\n");

    for (i = 301; i <= 400; i++) {
        tree->insert(new DummyObj(i));
    }
    printf("Tree: %d\n", tree->getCount());
    printf("Expect 300\n");
    
    it = Iterator<Unique>(tree);
    while (!it.done()) {
        printf("id: %lu\t", it.getNext()->getId());
    }
    printf("\n");

    for (i = 25; i < 75; i += 2) {
        delete tree->removeId(i);
    }
    printf("Tree: %d\n", tree->getCount());
    printf("Expect 275\n");

    for (i = 100; i >= 0; i--) {
        delete tree->removeId(i);
    }
    printf("Tree: %d\n", tree->getCount());
    printf("Expect 250\n");

    for (i = 400; i >= 0; i--) {
        delete tree->removeId(i);
    }
    printf("Tree: %d\n", tree->getCount());
    printf("Expect 0\n");

    rlx::closeLog();

    return 0;
}
