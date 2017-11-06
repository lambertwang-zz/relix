#ifndef __INPUT_H__
#define __INPUT_H__

#define SGR_BUFF_LEN 6

// Arrow keys
#define ARROW_UP 'A'
#define ARROW_DOWN 'B'
#define ARROW_RIGHT 'C'
#define ARROW_LEFT 'D'

// Keyboard event types
// A-Z, a-z, 0-9, etc
#define KEYBOARD_NORMAL 1
// Arrow keys, etc
#define KEYBOARD_ESCAPE 2

// Mouse action values
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

typedef struct MouseEvent {
    unsigned char status;
    int x;
    int y;
    int value;
} MouseEvent;

typedef struct KeyboardEvent {
    int type;
    char value;
} KeyboardEvent;

#endif

