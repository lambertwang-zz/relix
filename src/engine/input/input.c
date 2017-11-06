#include "input.h"
#include "../objects/objectManager.h"
#include "../log/log.h"
#include "../constants.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

static struct termios prev_settings;
static int old_fl;

int initInput() {
    writeLog(LOG_INPUT, "input::initInput(): Initialzing input");

    struct termios new_settings;
    // Save old termios settings
    tcgetattr(0, &prev_settings);
    new_settings = prev_settings;
    new_settings.c_lflag &= ~ICANON;
    new_settings.c_lflag &= ~ECHO;
    tcsetattr(0, TCSANOW, &new_settings);

    // Set stdin to non-blocking
    old_fl = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, old_fl | O_NONBLOCK);

    // Begin reporting mouse movements
    // printf("\e[1;0'z");
    printf("\e[?1003h");
    printf("\e[?1006h");
    fflush(stdout);

    // Register events with the worldmanager
    registerEvent(EVENT_KEYBOARD);
    registerEvent(EVENT_MOUSE);

    return 0;
}

int closeInput() {
    writeLog(LOG_INPUT, "input::closeInput(): Closing input");

    // End reporting mouse movements
    printf("\e[?1003l");
    printf("\e[?1006l");
    fflush(stdout);
    
    // Restore old settings
    tcsetattr(0, TCSANOW, &prev_settings);
    fcntl(STDIN_FILENO, F_SETFL, old_fl);

    return 0;
}

int createKeyboardEvent(char chr, int type) {
    Event ev;
    ev.id = EVENT_KEYBOARD;
    ev.data = malloc(sizeof(KeyboardEvent));
    ((KeyboardEvent *) ev.data)->value = chr;
    ((KeyboardEvent *) ev.data)->type = type;
    sendEvent(ev);
    return 0;
}

int createMouseEvent(int x, int y, int button, char status) {
    Event ev;
    ev.id = EVENT_MOUSE;
    ev.data = malloc(sizeof(MouseEvent));
    ((MouseEvent *) ev.data)->x = x - 1;
    ((MouseEvent *) ev.data)->y = y - 1;
    ((MouseEvent *) ev.data)->value = button;
    ((MouseEvent *) ev.data)->status = status;
    sendEvent(ev);
    return 0;
}

char readSgrValue(char *buff) {
    int i;
    char end;
    for (i = 0; i < SGR_BUFF_LEN - 1; i++) {
        read(0, buff + i, 1);
        if (buff[i] < 48 || buff[i] > 57) {
            end = buff[i];
            buff[i] = '\0';
            return end;
        }
    }
    buff[i] = '\0';
    return '\0';
}

int csiMouse() {
    char button_str[SGR_BUFF_LEN];
    char x_str[SGR_BUFF_LEN];
    char y_str[SGR_BUFF_LEN];
    char buff;

    readSgrValue(button_str);
    readSgrValue(x_str);
    buff = readSgrValue(y_str);

    writeLog(LOG_INPUT_V, "input::csoMouse(): Mouse: Button: %s X: %s Y: %s", button_str, x_str, y_str);
#ifdef DEBUG_INPUT
    printf("Button: %s X: %s Y: %s\n", button_str, x_str, y_str);
#endif
    return createMouseEvent(atoi(x_str), atoi(y_str), atoi(button_str), buff);
}

int csiArrow(char code) {
    createKeyboardEvent(code, KEYBOARD_ESCAPE);
    return 0;
}

int csi() {
    char buff;
    if (read(0, &buff, 1)) {
        switch (buff) {
            case '<':
                return csiMouse();
            case ARROW_UP:
            case ARROW_DOWN:
            case ARROW_RIGHT:
            case ARROW_LEFT:
                return csiArrow(buff);
            default:
                writeLog(LOG_INPUT, "input::csi(): Unsupported csi character: '%d'", buff);
        }
    }
    return 1;
}

int escape() {
    char buff;
    if(read(0, &buff, 1)) {
        switch (buff) {
            case '[':
                return csi();
            default:
                writeLog(LOG_INPUT, "input::escape(): Unsupported escape character: '%d'", buff);
        }
    }
    return 1;
}


int captureInput() {
    char buff;
    int read_result;

    do {
        read_result = read(0, &buff, 1);
        if (read_result == -1) {
            return read_result;
        }
#ifdef DEBUG_INPUT
        printf("%d\n", buff);
#endif
        if (buff == '\e') {
            return escape();
        }
        writeLog(LOG_INPUT_V, "input::readInput(): ASCII Input %d", buff);
        return createKeyboardEvent(buff, KEYBOARD_NORMAL);
    } while (!read_result);
    return 1;
}

int readInput() {
    int end_input;
    do {
        end_input = captureInput();
    } while (!end_input);
    return end_input;
}

