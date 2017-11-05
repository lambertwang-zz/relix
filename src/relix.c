#include "game/game.h"
#include "actors/object.h"
#include "actors/objectManager.h"

int main(int argc, char **argv) {
    initGame();

    struct Object o_1;

    initObject(&o_1);

    o_1.pix.chr = '@';
    o_1.pix.fg = 128;

    addObject(&o_1);

    loop();

    closeGame(0);
    return 0;
}
