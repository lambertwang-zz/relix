#include "../utility/clock.h"
#include "../utility/utility.h"
#include "../term/term.h"

#include "../term/screen.h"
#include "stdio.h"

#define frame_time 16666

int loop() {
    // Create variables required for timing outside of loop scope
    struct Clock game_clock;
    initClock(&game_clock);

    unsigned long adjust_time = 0;
    unsigned int frame_count = 0;
    int loop_time = 0;
    int loop_time_saved = 0;

    while (frame_count++ < 600) {
        fflush(stdout);
        // !this->game_over) {
        // Reset clock
        delta(&game_clock);
        // Increment step count
        // this->step_count++;

        // Send EVENT_STEP to all objects
        // df::EventStep p_step_event = df::EventStep(this->step_count);
        // onEvent(&p_step_event);

        // Retrieve input
        // df::InputManager &input_manager = df::InputManager::getInstance();
        // input_manager.getInput();

        // df::WorldManager &world_manager = df::WorldManager::getInstance();
        // Call worldManager update
        // world_manager.update();

        // Send EVENT_BEFOREDRAW to all objects
        // df::EventBeforeDraw p_bd_event = df::EventBeforeDraw();
        // onEvent(&p_bd_event);

        // Call worldManager draw
        // world_manager.draw();
        swapScreen();

        // printf("\e[1;0HLoop time: %d", loop_time);
        if (frame_count % 60 == 0) {
            loop_time_saved = loop_time;
        }

        struct Screen *screen = getScreen();
        printf("Loop time: %5d  ", loop_time_saved);
        printf("Frame_count: %3d  ", frame_count);
        printf("Times_init: %3d  ", screen->times_init);
        printf("Lines: %3d  ", screen->ts.lines);
        printf("Columns: %3d  ", screen->ts.cols);
        fflush(stdout);

        // Swap graphics buffers
        // df::GraphicsManager &graphics_manager =
        //     df::GraphicsManager::getInstance();
        // graphics_manager.swapBuffers();

        // Adjust sleep time for additional framerate accuracy
        loop_time = split(&game_clock);
        unsigned long sleep_time = (frame_time - loop_time - adjust_time);
        // Reset clock before sleeping to calculate actual sleep time
        delta(&game_clock);
        // Multiply microseconds to obtain sleep time in nanoseconds
        if (sleep_time > 0) {
            uSleep(sleep_time);
            // Calculate adjust from difference between expected and actual
            // sleep
            // times
            adjust_time = split(&game_clock) - sleep_time;
        }
    }

    return 0;
}
