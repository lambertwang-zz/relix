#include "dice.h"

// Library
#include <stdlib.h>
#include <string.h>

// Engine
#include "random.h"

Dice parseDice(const char *label) {
    char temp[LABEL_SHORT];
    char *token;
    strncpy(temp, label, LABEL_SHORT);

    Dice new_dice;
    strcpy(new_dice.label, temp);

    token = strtok(temp, "dD");
    if (token == NULL) {
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

    token = strrchr(label, '+');
    if (token == NULL) {
        token = strrchr(label, '-');
        if (token == NULL) {
            new_dice.flat = 0;
            return new_dice;
        }
    }
    new_dice.flat = atoi(token);

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

