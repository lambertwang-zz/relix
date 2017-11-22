
#include "game.h"

#include "constants.h"
#include "input/input.h"
#include "log/log.h"
#include "objects/objectManager.h"
#include "term/screen.h"
#include "term/term.h"
#include "utility/clock.h"
#include "utility/random.h"
#include "utility/utility.h"

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

int initGame() {
    frame_count = 0;

    initLog();
    initRandom_s(9877);
    // Init objects and event registration
    initObjects();
    initScreen();
    initInput();
    writeLog(LOG_INIT, "game::initGame(): Initialized game");

    signal(SIGINT, closeGame);
    return 0;
}

void closeGame(int a) {
    closeInput();
    closeScreen();
    closeObjects();
    closeLog();

    exit(0);
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
        // Increment step count
        // this->step_count++;
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

        // Call worldManager draw
        // world_manager.draw();
        int objectsRendered = renderObjects();
        writeLog(LOG_GAME_V, "game::loop(): Rendered %d objects", objectsRendered);
        /*
        int i, j;


        for (i = 0; i < screen.ts.lines; i++) {
            int sat = (int)(((float)i / (float)screen.ts.lines) * 192.0) + 63;
            for (j = 0; j < screen.ts.cols; j++) {
                putPixelRgb(j, i, hslToRgb((Color){(i * 3 + j + frame_count / 5) % 256, sat, 255}));
                // putPixelRgb(j, i, (Color){i * 40, j * 40, 255});
                // putPixelHSL(i, j, (Color){255, 255, 255});
                // putPixelRgb(j, i, (Color){0, 0, 0});
                // putPixel(j, i, i * 16 + j);
            }
        }
        */


        swapScreen();

        if (frame_count % 30 == 0) {
            loop_time_saved = loop_time;
        }
        printf("\e[1G\e[%dC", screen.margin_x);
        printf(" Loop: %5d ", loop_time_saved);
        printf("Frame: %3d ", frame_count);
        printf("Inits: %3d ", screen.times_init);
        printf("Lines: %3d ", screen.ts.lines);
        printf("Cols: %3d ", screen.ts.cols);
        printf("Obj rendered: %3d ", objectsRendered);
        fflush(stdout);

        // Swap graphics buffers
        // df::GraphicsManager &graphics_manager =
        //     df::GraphicsManager::getInstance();
        // graphics_manager.swapBuffers();

        // Adjust sleep time for additional framerate accuracy
        loop_time = split(&game_clock);
        unsigned long sleep_time = (FRAME_TIME - loop_time - adjust_time);
        // Reset clock before sleeping to calculate actual sleep time
        delta(&game_clock);
        // Multiply microseconds to obtain sleep time in nanoseconds
        if (sleep_time > 0) {
            uSleep(sleep_time);
            // Calculate adjust from difference between expected and actual sleep times
            adjust_time = split(&game_clock) - sleep_time;
        }
    }

    return 0;
}
