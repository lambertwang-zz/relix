#include "game/game.h"
#include "term/screen.h"

int main(int argc, char **argv) {
    initScreen();

    loop();

    return 0;
}
