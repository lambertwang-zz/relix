#include "game.h"

// Library
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

// Engine
#include "constants.h"
#include "input/input.h"
#include "log/log.h"
#include "object/objectManager.h"
#include "term/screen.h"
#include "ui/ui.h"
#include "utility/clock.h"
#include "utility/random.h"
#include "utility/utility.h"


int initGame() {
    frame_count = 0;

    initLog();
    initRandom_s(9877);
    // Init objects and event registration
    initObjects();
    initInput();
    initScreenManager();
    writeLog(LOG_INIT, "game::initGame(): Initialized game");

    signal(SIGINT, closeGame);
    return 0;
}

void closeGame(int a) {
    closeScreenManager();
    closeInput();
    closeObjects();
    closeLog();

    exit(a);
}

int loop() {
    // Create variables required for timing outside of loop scope
    struct Clock game_clock;
    initClock(&game_clock);

    unsigned long adjust_time = 0;
    int loop_time = 0;
    int loop_time_saved = 0;

    while (1) {
        fflush(stdout);
        // !this->game_over) {
        // Reset clock
        delta(&game_clock);
        frame_count++;
        writeLog(LOG_GAME_V, "game::loop(): Beginning frame %d", frame_count);

        // Send EVENT_STEP to all objects
        // df::EventStep p_step_event = df::EventStep(this->step_count);
        // onEvent(&p_step_event);

        // Retrieve input
        // df::InputManager &input_manager = df::InputManager::getInstance();
        // input_manager.getInput();
        readInput();
        writeLog(LOG_GAME_V, "game::loop(): Reading input");

        // df::WorldManager &world_manager = df::WorldManager::getInstance();
        // Call worldManager update
        // world_manager.update();
        updateObjects();
        writeLog(LOG_GAME_V, "game::loop(): Updated objects");

        // Send EVENT_BEFOREDRAW to all objects
        // df::EventBeforeDraw p_bd_event = df::EventBeforeDraw();
        // onEvent(&p_bd_event);

        // Check that it's okay to write to the buffer
        sem_wait(&screen_manager.writes_allowed);

        // Ui layer is always on top and opaque. 
        // Ui will render INT_MAX to the depth buffer.
        int elementsRendered = renderUi();
        writeLog(LOG_GAME_V, "game::loop(): Rendered %d UI elements", elementsRendered);

        // Render lighting before rendering objects
        int lightsRendered = renderObjectLights();
        writeLog(LOG_GAME_V, "game::loop(): Rendered %d lights", lightsRendered);

        int objectsRendered = renderObjects();
        writeLog(LOG_GAME_V, "game::loop(): Rendered %d objects", objectsRendered);

        // Let the renderer know that we're done preparing the frame
        sem_post(&screen_manager.reads_allowed);

        // swapScreen();

        // TODO: we can't safely write the status line anymore because
        // the game loop doesn't own output
        if (frame_count % 30 == 0) {
            loop_time_saved = loop_time;
        }
        // printf("\e[1G\e[%dC", 0);
        // printf(" Loop: %5d ", loop_time_saved);
        // printf("Frame: %3d ", frame_count);
        // printf("Inits: %3d ", screen.times_init);
        // printf("Lines: %3d ", screen.ts.lines);
        // printf("Cols: %3d ", screen.ts.cols);
        // printf("Obj rendered: %3d ", objectsRendered);
        // fflush(stdout);

        // Swap graphics buffers
        // df::GraphicsManager &graphics_manager =
        //     df::GraphicsManager::getInstance();
        // graphics_manager.swapBuffers();

        // Adjust sleep time for additional framerate accuracy
        loop_time = split(&game_clock);
        long sleep_time = (FRAME_TIME - loop_time - adjust_time);
        // Reset clock before sleeping to calculate actual sleep time
        delta(&game_clock);
        // Multiply microseconds to obtain sleep time in nanoseconds
        if (sleep_time > 0) {
            uSleep(sleep_time);
            // Calculate adjust from difference between expected and actual sleep times
            adjust_time = split(&game_clock) - sleep_time;
        } else {
            adjust_time = 0;
        }
    }

    return 0;
}
