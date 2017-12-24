#include "log/log.h"
#include "utility/dice.h"
#include "term/color.h"

#include <stdio.h>

int main() {
    initLog();

    String *tmp = createString();

    Dice test1 = parseDice(tmp);

    sputf(tmp, "5d4+100");
    test1 = parseDice(tmp);

    printf("Dice %s: %dd%d%+d\n", tmp->s, test1.count, test1.sides, test1.flat);
    printf("Rolling %s: %d\n", tmp->s, rollDice(test1));

    sputf(tmp, "d6");
    test1 = parseDice(tmp);

    printf("Dice %s: %dd%d%+d\n", tmp->s, test1.count, test1.sides, test1.flat);
    printf("Rolling %s: %d\n", tmp->s, rollDice(test1));
    printf("Rolling %s: %d\n", tmp->s, rollDice(test1));
    printf("Rolling %s: %d\n", tmp->s, rollDice(test1));
    printf("Rolling %s: %d\n", tmp->s, rollDice(test1));
    printf("Rolling %s: %d\n", tmp->s, rollDice(test1));
    printf("Rolling %s: %d\n", tmp->s, rollDice(test1));
    printf("Rolling %s: %d\n", tmp->s, rollDice(test1));
    printf("Rolling %s: %d\n", tmp->s, rollDice(test1));
    printf("Rolling %s: %d\n", tmp->s, rollDice(test1));
    printf("Rolling %s: %d\n", tmp->s, rollDice(test1));
    printf("Rolling %s: %d\n", tmp->s, rollDice(test1));
    printf("Rolling %s: %d\n", tmp->s, rollDice(test1));
    printf("Rolling %s: %d\n", tmp->s, rollDice(test1));
    printf("Rolling %s: %d\n", tmp->s, rollDice(test1));
    printf("Rolling %s: %d\n", tmp->s, rollDice(test1));
    printf("Rolling %s: %d\n", tmp->s, rollDice(test1));
    printf("Rolling %s: %d\n", tmp->s, rollDice(test1));
    printf("Rolling %s: %d\n", tmp->s, rollDice(test1));
    printf("Rolling %s: %d\n", tmp->s, rollDice(test1));
    printf("Rolling %s: %d\n", tmp->s, rollDice(test1));

    sputf(tmp, "10d144+32");
    test1 = parseDice(tmp);

    printf("Dice %s: %dd%d%+d\n", tmp->s, test1.count, test1.sides, test1.flat);

    sputf(tmp, "d123+10");
    test1 = parseDice(tmp);

    printf("Dice %s: %dd%d%+d\n", tmp->s, test1.count, test1.sides, test1.flat);
    
    sputf(tmp, "3d14-99");
    test1 = parseDice(tmp);

    printf("Dice %s: %dd%d%+d\n", tmp->s, test1.count, test1.sides, test1.flat);
    
    sputf(tmp, "d12-3");
    test1 = parseDice(tmp);

    printf("Dice %s: %dd%d%+d\n", tmp->s, test1.count, test1.sides, test1.flat);

    deleteString(tmp);

    printf("\n\bBegin Color Tests\n\n");

    printf("Expect 5 = %d\n", (255 - 6) / 45);
    printf("Expect 1 = %d\n", (95 - 6) / 45);
    printf("\e[48;5;58mWhat color is this?\e[0m\n");

    /*
    Color test2 = (Color){0, 0, 0};
    printf("Color %d, %d, %d\nResult: %d\nExpect: %d\n", test2.r, test2.g, test2.b, rgbToTerm(test2), 16);

    test2 = (Color){7, 7, 7};
    printf("Color %d, %d, %d\nResult: %d\nExpect: %d\n", test2.r, test2.g, test2.b, rgbToTerm(test2), 16);

    test2 = (Color){8, 8, 8};
    printf("Color %d, %d, %d\nResult: %d\nExpect: %d\n", test2.r, test2.g, test2.b, rgbToTerm(test2), 232);

    test2 = (Color){95, 95, 0};
    printf("Color %d, %d, %d\nResult: %d\nExpect: %d\n", test2.r, test2.g, test2.b, rgbToTerm(test2), 58);
    */
   
    closeLog();

    return 0;
}
