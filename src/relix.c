#include "game/game.h"

int main(int argc, char **argv) {
    initGame();


    loop();

    closeGame(0);
    return 0;
}
