#ifndef __INPUT_H__
#define __INPUT_H__

namespace rlx {

    #define SGR_BUFF_LEN 6

    // Arrow keys
    #define ARROW_UP 'A'
    #define ARROW_DOWN 'B'
    #define ARROW_RIGHT 'C'
    #define ARROW_LEFT 'D'
    #define ARROW_UP_WIN 72
    #define ARROW_DOWN_WIN 80
    #define ARROW_RIGHT_WIN 77
    #define ARROW_LEFT_WIN 75

    // Keyboard event types
    // A-Z, a-z, 0-9, etc
    #define KEYBOARD_NORMAL 1
    // Arrow keys, etc
    #define KEYBOARD_ESCAPE 2

    // Mouse action values
    #define MOUSE_NONE -1
    #define MOUSE_LEFT 0
    #define MOUSE_RIGHT 2
    #define MOUSE_MIDDLE 1
    #define MOUSE_MOVE 35

    // Mouse action types
    #define MOUSE_PRESS 'M'
    #define MOUSE_RELEASE 'm'

    int initInput();
    int closeInput();

    int readInput();
}

#endif

