#ifndef __DICE_H__
#define __DICE_H__

// Engine
#include "constants.h"
#include "string/string.h"

#define NULL_DICE (Dice){0, 0, 0}

typedef struct Dice {
    // [count]d[sides]+[flat]
    int count;
    int sides;
    int flat;
} Dice;

Dice parseDice(String *label);

int rollDice(const Dice dice);
int maxDice(const Dice dice);

int sprintDice(String *dest, const Dice dice);

#endif

