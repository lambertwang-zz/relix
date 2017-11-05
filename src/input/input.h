#ifndef __INPUT_H__
#define __INPUT_H__

#define SGR_BUFF_LEN 6

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
    unsigned char value;
    unsigned char status;
    int x;
    int y;
} MouseEvent;

typedef struct KeyboardEvent {
    char value;
} KeyboardEvent;

#endif

