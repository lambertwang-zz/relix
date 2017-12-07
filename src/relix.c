// Library
#include <signal.h>

// Engine
#include "game/game.h"
#include "log/log.h"

// Game
#include "game/menu/menu.h"
#include "game/world/world.h"
#include "game/relix.h"
#include "game/asset/asset.h"

void terminate(int a) {
    closeGame(a);
}

int main(int argc, char **argv) {
    initGame();
    signal(SIGINT, terminate);

    // addLogLevel(LOG_GAME_V);
    // addLogLevel(LOG_GAME_V);
    // addLogLevel(LOG_INPUT_V);
    // addLogLevel(LOG_SCREEN_V);
    addLogLevel(0xffffffff);
    addLogLevel(LOG_OBJECTMANAGER_V);
    addLogLevel(LOG_USER);
    addLogLevel(LOG_RESOURCE_V);
    // removeLogLevel(LOG_RESOURCE_V);

    // if (initResources()) {
    //     closeGame(1);
    //     return 0;
    // }

    initMainMenu();
    initWorld();

    loop();

    closeGame(0);
    return 0;
}
