#include "log/log.h"
#include "utility/dice.h"

#include <stdio.h>

int main(int argc, char **argv) {
    initLog();

    Dice test1 = parseDice("5d4+100");

    printf("Dice %s: %dd%d%+d\n", test1.label, test1.count, test1.sides, test1.flat);
    printf("Rolling %s: %d\n", test1.label, rollDice(test1));

    test1 = parseDice("d6");

    printf("Dice %s: %dd%d%+d\n", test1.label, test1.count, test1.sides, test1.flat);
    printf("Rolling %s: %d\n", test1.label, rollDice(test1));
    printf("Rolling %s: %d\n", test1.label, rollDice(test1));
    printf("Rolling %s: %d\n", test1.label, rollDice(test1));
    printf("Rolling %s: %d\n", test1.label, rollDice(test1));
    printf("Rolling %s: %d\n", test1.label, rollDice(test1));
    printf("Rolling %s: %d\n", test1.label, rollDice(test1));
    printf("Rolling %s: %d\n", test1.label, rollDice(test1));
    printf("Rolling %s: %d\n", test1.label, rollDice(test1));
    printf("Rolling %s: %d\n", test1.label, rollDice(test1));
    printf("Rolling %s: %d\n", test1.label, rollDice(test1));
    printf("Rolling %s: %d\n", test1.label, rollDice(test1));
    printf("Rolling %s: %d\n", test1.label, rollDice(test1));
    printf("Rolling %s: %d\n", test1.label, rollDice(test1));
    printf("Rolling %s: %d\n", test1.label, rollDice(test1));
    printf("Rolling %s: %d\n", test1.label, rollDice(test1));
    printf("Rolling %s: %d\n", test1.label, rollDice(test1));
    printf("Rolling %s: %d\n", test1.label, rollDice(test1));
    printf("Rolling %s: %d\n", test1.label, rollDice(test1));
    printf("Rolling %s: %d\n", test1.label, rollDice(test1));
    printf("Rolling %s: %d\n", test1.label, rollDice(test1));

    test1 = parseDice("10d144+32");

    printf("Dice %s: %dd%d%+d\n", test1.label, test1.count, test1.sides, test1.flat);

    test1 = parseDice("d123+10");

    printf("Dice %s: %dd%d%+d\n", test1.label, test1.count, test1.sides, test1.flat);
    
    test1 = parseDice("3d14-99");

    printf("Dice %s: %dd%d%+d\n", test1.label, test1.count, test1.sides, test1.flat);
    
    test1 = parseDice("d12-3");

    printf("Dice %s: %dd%d%+d\n", test1.label, test1.count, test1.sides, test1.flat);
   
    closeLog();

    return 0;
}
