#include "dice.h"

// Library
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Engine
#include "random.h"

namespace rlx {

Dice::Dice(String *label) {
    count = 0;
    sides = 0;
    flat = 0;

    String *temp = new String();
    char *token;
    temp->copy(label);

    token = strtok(temp->getBuffer(), "dD");
    if (token == NULL) {
        delete temp;
        return;
    }
    count = atoi(token);

    token = strtok(NULL, "dD");
    if (token == NULL) {
        sides = count;
        count = 1;
    } else {
        sides = atoi(token);
    }

    token = strrchr(label->getBuffer(), '+');
    if (token == NULL) {
        token = strrchr(label->getBuffer(), '-');
        if (token == NULL) {
            flat = 0;
            delete temp;
            return;
        }
    }
    flat = atoi(token);

    delete temp;
}

int Dice::roll() const {
    int i, result = 0;
    for (i = 0; i < count; i++) {
        result += nrandom_i(0, sides) + 1;
    }
    result += flat;

    return result;
}

int Dice::max() const {
    return count * sides + flat;
}

int Dice::sprint(String *dest) const {
    return dest->sputf("%dD%d%+d", count, sides, flat);
}

int Dice::print() const {
    return printf("%dD%d%+d", count, sides, flat);
}

}
