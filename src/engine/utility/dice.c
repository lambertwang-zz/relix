#include "dice.h"

// Library
#include <stdlib.h>
#include <string.h>

// Engine
#include "random.h"

Dice parseDice(String *label) {
    String *temp = createString();
    char *token;
    stringCopy(temp, label);

    Dice new_dice;

    token = strtok(temp->s, "dD");
    if (token == NULL) {
        deleteString(temp);
        return new_dice;
    }
    new_dice.count = atoi(token);

    token = strtok(NULL, "dD");
    if (token == NULL) {
        new_dice.sides = new_dice.count;
        new_dice.count = 1;
    } else {
        new_dice.sides = atoi(token);
    }

    token = strrchr(label->s, '+');
    if (token == NULL) {
        token = strrchr(label->s, '-');
        if (token == NULL) {
            new_dice.flat = 0;
            deleteString(temp);
            return new_dice;
        }
    }
    new_dice.flat = atoi(token);

    deleteString(temp);
    return new_dice;
}

int rollDice(const Dice dice) {
    int i, result = 0;
    for (i = 0; i < dice.count; i++) {
        result += nrandom_i(0, dice.sides) + 1;
    }
    result += dice.flat;

    return result;
}

int sprintDice(String *dest, const Dice dice) {
    return sputf(dest, "%dD%d%+d", dice.count, dice.sides, dice.flat);
}

