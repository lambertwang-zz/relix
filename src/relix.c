// Library
#include <signal.h>

// Engine
#include "game/game.h"
#include "log/log.h"
#include "ui/ui.h"

// Game
#include "game/menu/menu.h"
#include "game/world/world.h"
#include "game/relix.h"
#include "game/asset/asset.h"

// int main(int argc, char **argv) {
int main() {
    initGame();

    addLogLevel(0xffffffff);
    addLogLevel(LOG_USER);
    removeLogLevel(LOG_GAME_V);

    if (initResources()) {
        closeGame();
    }

    // initMainMenu();
    useStatusLine();
    initWorld();

    loop();

    return 0;
}
