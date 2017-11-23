#include "game/game.h"
#include "log/log.h"
// #include "relix/menu/menu.h"
//
#include "game/map/map.h"

int main(int argc, char **argv) {
    initGame();

    // addLogLevel(LOG_GAME_V);
    addLogLevel(LOG_INPUT_V);
    addLogLevel(LOG_SCREEN_V);

    // initMainMenu();
    initMap();

    loop();

    closeGame(0);
    return 0;
}
