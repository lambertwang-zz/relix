#include "game.h"

// Library
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Engine
#include "constants.h"
#include "input/input.h"
#include "input/inputEvents.h"
#include "log/log.h"
#include "object/objectManager.h"
#include "term/screen.h"
#include "ui/ui.h"
#include "ui/statusLine.h"
#include "utility/clock.h"
#include "utility/random.h"
#include "utility/utility.h"

namespace rlx {

static StatusLine *status_line;
static int status_mode;
static int is_gameover;
static int clear_screen_mode;

void terminate(int signum) {
    (void) signum;
    is_gameover = 1;
}

int initGame() {
    is_gameover = 0;
    clear_screen_mode = CLEAR_BEFORE_RENDER;
    frame_count = 0;

    // initLog();
    initRandom_s(9877);
    // Init objects and event registration
    ObjectManager::initObjects();
    UiManager::initUi();
    initInput();
    ScreenManager::initScreen();
    writeLog(LOG_INIT, "game::initGame(): Initialized game");

    signal(SIGINT, terminate);
    // signal(SIGSEGV, closeGame);

    status_line = NULL;

    return 0;
}

void closeGame() {
    ScreenManager::closeScreen();
    closeInput();
    UiManager::closeUi();
    ObjectManager::closeObjects();
    closeLog();

    exit(0);
}

int changeStatusMode(Element *el, MouseEvent *ev) {
    (void) el;
    if (ev->getStatus() == MOUSE_PRESS) {
        status_mode = (status_mode + 1) % 3;
    }
    return 0;
}

void useStatusLine() {
    status_line = new StatusLine();
}

void setClearScreenMode(int new_clear_screen_mode) {
    clear_screen_mode = new_clear_screen_mode;
}

int loop() {
    // Create variables required for timing outside of loop scope
    Clock game_clock = Clock();

    unsigned long adjust_time = 0;
    int loop_time = 0;
    int loop_time_saved = 0;
    frame_count = 0;
    Screen *screen = ScreenManager::getMainScreen();

    while (!is_gameover) {
        // Reset clock
        game_clock.delta();
        frame_count++;
        writeLog(LOG_GAME_V, "game::loop(): Beginning frame %d", frame_count);

        // Send EVENT_STEP to all objects
        // df::EventStep p_step_event = df::EventStep(this->step_count);
        // onEvent(&p_step_event);

        // Retrieve input
        writeLog(LOG_GAME_V, "game::loop(): Reading input");
        readInput();

        // Clear the current screen buffer
        // clearScreen(&screen_manager.main_screen);

        // Call worldManager update
        writeLog(LOG_GAME_V, "game::loop(): Updating objects");
        ObjectManager::updateObjects();

        // Send EVENT_BEFOREDRAW to all objects
        // df::EventBeforeDraw p_bd_event = df::EventBeforeDraw();
        // onEvent(&p_bd_event);

        int elementsRendered;
        int lightsRendered;
        int objectsRendered;

        // Check that it's okay to write to the buffer
        // Otherwise, skip this frame
        if (ScreenManager::waitForWrites() == WRITES_ALLOWED) { 
            if (clear_screen_mode == CLEAR_BEFORE_RENDER) {
                screen->clear();
                // for (i = 0; i < screen->ts.lines * screen->ts.cols; i++) {
                //     screen->light_buffer[i] = COLOR_BLANK;
                //     screen->pixel_buffer[i] = PIXEL_BLANK;
                // }
            }

            // Render lighting before rendering objects
            lightsRendered = ObjectManager::renderObjectLights();
            writeLog(LOG_GAME_V,
                "game::loop(): Rendered %d lights",
                lightsRendered);

            objectsRendered = ObjectManager::renderObjects();
            writeLog(LOG_GAME_V,
                "game::loop(): Rendered %d objects",
                objectsRendered);

            // Ui layer is always on top.
            // Ui will render INT_MAX to the depth buffer.
            elementsRendered = UiManager::renderUi();
            writeLog(LOG_GAME_V,
                "game::loop(): Rendered %d UI elements",
                elementsRendered);

            // Let the renderer know that we're done preparing the frame
            ScreenManager::allowReads();
            writeLog(LOG_GAME_V, "game::loop(): Posted buffer to renderer.");

            if (status_line != NULL) {
                String *status_label = new String();
                switch (status_mode) {
                case 0:
                    status_label->sputf("Loop: %5d Frame: %5d",
                        loop_time_saved, 
                        frame_count);

                    break;
                case 1:
                    status_label->sputf("Objects: %4d Elements: %4d Lights: %4d", 
                        objectsRendered,
                        elementsRendered,
                        lightsRendered);

                    break;
                case 2:
                default:
                    status_label->sputf("Screen info TBD");
                    // status_label->sputf("Cols: %3d Lines: %3d Times Init %3d", 
                    //     screen->getSize().cols,
                    //     screen->getSize().lines,
                    //     screen->getTimesInit());

                    break;
                }
                writeLog(LOG_GAME_V,
                    "game::loop(): Status %s.",
                    status_label->getBuffer());

                status_line->setText(status_label);
                delete status_label;
            }
        } else {
            writeLog(LOG_GAME_V, "game::loop(): Renderer not ready for writes.");
        }

        if (frame_count % 30 == 0) {
            loop_time_saved = loop_time;
        }

        // Adjust sleep time for additional framerate accuracy
        loop_time = game_clock.split();
        long sleep_time = (FRAME_TIME - loop_time - adjust_time);
        // Reset clock before sleeping to calculate actual sleep time
        game_clock.delta();
        // Multiply microseconds to obtain sleep time in nanoseconds
        if (sleep_time > 0) {
            uSleep(sleep_time);
            // Calculate adjust from difference between expected and actual
            // sleep times
            adjust_time = game_clock.split() - sleep_time;
        } else {
            adjust_time = 0;
        }
    }

    closeGame();

    return 0;
}

}
