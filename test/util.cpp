#include "log/log.h"
#include "utility/dice.h"
#include "term/color.h"

#include <stdio.h>

using namespace rlx;

int main() {
    initLog();

    String tmp = String();
    Dice test1 = Dice(&tmp);

    tmp.sputf("5d4+100");
    test1 = Dice(&tmp);

    tmp.print();
    printf("\n");
    test1.print();
    printf("\n");
    printf("Rolling: %d\n", test1.roll());

    tmp.sputf("d6");
    test1 = Dice(&tmp);

    tmp.print();
    printf("\n");
    test1.print();
    printf("\n");
    printf("Rolling: %d\n", test1.roll());
    printf("Rolling: %d\n", test1.roll());
    printf("Rolling: %d\n", test1.roll());
    printf("Rolling: %d\n", test1.roll());
    printf("Rolling: %d\n", test1.roll());
    printf("Rolling: %d\n", test1.roll());
    printf("Rolling: %d\n", test1.roll());
    printf("Rolling: %d\n", test1.roll());
    printf("Rolling: %d\n", test1.roll());
    printf("Rolling: %d\n", test1.roll());
    printf("Rolling: %d\n", test1.roll());
    printf("Rolling: %d\n", test1.roll());
    printf("Rolling: %d\n", test1.roll());
    printf("Rolling: %d\n", test1.roll());
    printf("Rolling: %d\n", test1.roll());
    printf("Rolling: %d\n", test1.roll());
    printf("Rolling: %d\n", test1.roll());
    printf("Rolling: %d\n", test1.roll());
    printf("Rolling: %d\n", test1.roll());
    printf("Rolling: %d\n", test1.roll());

    tmp.sputf("10d144+32");
    test1 = Dice(&tmp);

    tmp.print();
    printf("\n");
    test1.print();
    printf("\n");

    tmp.sputf("d123+10");
    test1 = Dice(&tmp);

    tmp.print();
    printf("\n");
    test1.print();
    printf("\n");
    
    tmp.sputf("3d14-99");
    test1 = Dice(&tmp);

    tmp.print();
    printf("\n");
    test1.print();
    printf("\n");
    
    tmp.sputf("d12-3");
    test1 = Dice(&tmp);

    tmp.print();
    printf("\n");
    test1.print();
    printf("\n");

    /*
    printf("\n\bBegin Color Tests\n\n");

    printf("Expect 5 = %d\n", (255 - 6) / 45);
    printf("Expect 1 = %d\n", (95 - 6) / 45);
    printf("\e[48;5;58mWhat color is this?\e[0m\n");

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
