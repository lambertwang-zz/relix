#include "game/game.h"
#include "log/log.h"
#include "relix/menu/menu.h"

int main(int argc, char **argv) {
    initGame();

    // addLogLevel(LOG_GAME_V);

    initMainMenu();

    loop();

    closeGame(0);
    return 0;
}
