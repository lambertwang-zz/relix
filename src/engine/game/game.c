#include "game.h"

// Library
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

static Element *status_line;

int initGame() {
    frame_count = 0;

    initLog();
    initRandom_s(9877);
    // Init objects and event registration
    initObjects();
    initUi();
    initInput();
    initScreenManager();
    writeLog(LOG_INIT, "game::initGame(): Initialized game");

    signal(SIGINT, closeGame);
    // signal(SIGSEGV, closeGame);

    status_line = NULL;

    return 0;
}

void closeGame(int a) {
    closeScreenManager();
    closeInput();
    closeUi();
    closeObjects();
    closeLog();

    exit(0);
}

void useStatusLine() {
    status_line = createElement();
    status_line->bg_c = COLOR_BLACK;
    status_line->bg_c_focus = COLOR_RED;
    status_line->focusable = 1;

    registerUiElement(status_line);
}

int loop() {
    // Create variables required for timing outside of loop scope
    struct Clock game_clock;
    initClock(&game_clock);

    unsigned long adjust_time = 0;
    int loop_time = 0;
    int loop_time_saved = 0;
    frame_count = 0;
    Screen *screen = &screen_manager.main_screen;
    screen->times_init = 0;

    while (!screen_manager.is_closed) {
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
        writeLog(LOG_GAME_V, "game::loop(): Reading input");
        readInput();

        // Clear the current screen buffer
        // clearScreen(&screen_manager.main_screen);
        int i;

        // df::WorldManager &world_manager = df::WorldManager::getInstance();
        // Call worldManager update
        // world_manager.update();
        writeLog(LOG_GAME_V, "game::loop(): Updating objects");
        updateObjects();

        // Send EVENT_BEFOREDRAW to all objects
        // df::EventBeforeDraw p_bd_event = df::EventBeforeDraw();
        // onEvent(&p_bd_event);
 

        int elementsRendered;
        int lightsRendered;
        int objectsRendered;

        // Check that it's okay to write to the buffer
        // Otherwise, skip this frame
        if (sem_trywait(&screen_manager.writes_allowed)) {
            for (i = 0; i < screen->ts.lines * screen->ts.cols; i++) {
                screen->light_buffer[i] = COLOR_BLANK;
                screen->pixel_buffer[i] = PIXEL_BLANK;
            }

            // Ui layer is always on top and opaque. 
            // Ui will render INT_MAX to the depth buffer.
            elementsRendered = renderUi();
            writeLog(LOG_GAME_V, "game::loop(): Rendered %d UI elements", elementsRendered);

            // Render lighting before rendering objects
            lightsRendered = renderObjectLights();
            writeLog(LOG_GAME_V, "game::loop(): Rendered %d lights", lightsRendered);

            objectsRendered = renderObjects();
            writeLog(LOG_GAME_V, "game::loop(): Rendered %d objects", objectsRendered);

            // Let the renderer know that we're done preparing the frame
            sem_post(&screen_manager.reads_allowed);
            writeLog(LOG_GAME_V, "game::loop(): Posted buffer to renderer.");

            if (status_line != NULL) {
                String *status_label = createString();
                sputf(status_label, "Loop: %5d Frame: %5d Obj rendered: %3d Times Init %3d", 
                        loop_time_saved, 
                        frame_count, 
                        objectsRendered,
                        screen->times_init);
                writeLog(LOG_GAME_V, "game::loop(): Status %s.", status_label->s);
                stringCopy(status_line->text, status_label);
                deleteString(status_label);
            }
        }

        if (frame_count % 30 == 0) {
            loop_time_saved = loop_time;
        }

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

    closeGame(0);

    return 0;
}
