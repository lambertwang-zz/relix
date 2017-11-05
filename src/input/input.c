#include "input.h"
#include "../actors/objectManager.h"
#include "../log/log.h"
#include "../constants.h"

#include <stdio.h>
#include <termios.h>
#include <unistd.h>

static struct termios prev_settings;

int initInput() {
    writeLog(LOG_INPUT, "input::initInput(): Initialzing input");

    struct termios new_settings;
    // Save old termios settings
    tcgetattr(0, &prev_settings);
    new_settings = prev_settings;
    new_settings.c_lflag &= ~ICANON;
    new_settings.c_lflag &= ~ECHO;
    tcsetattr(0, TCSANOW, &new_settings);

    // Begin reporting mouse movements
    // printf("\e[1;0'z");
    printf("\e[?1003h");
    printf("\e[?1006h");
    fflush(stdout);

    // Register events with the worldmanager
}

int closeInput() {
    writeLog(LOG_INPUT, "input::closeInput(): Closing input");

    // End reporting mouse movements
    printf("\e[?1003l");
    printf("\e[?1006l");
    fflush(stdout);
    
    // Restore old settings
    tcsetattr(0, TCSANOW, &prev_settings);
}

void createKeyboardEvent() {
}

void createMouseEvent() {
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

void csiMouse() {
    char buff;

    char button_str[SGR_BUFF_LEN];
    char x_str[SGR_BUFF_LEN];
    char y_str[SGR_BUFF_LEN];
    unsigned char button_type;

    readSgrValue(button_str);
    readSgrValue(x_str);
    buff = readSgrValue(y_str);
    button_type = buff;

    writeLog(LOG_INPUT_V, "input::csoMouse(): Mouse: Button: %s X: %s Y: %s", button_str, x_str, y_str);
#ifdef DEBUG_INPUT
    printf("Button: %s X: %s Y: %s\n", button_str, x_str, y_str);
#endif
}

void csi() {
    char buff;
    if (read(0, &buff, 1)) {
        if (buff == '<') {
            csiMouse();
            // printf("Begin CSI\n");
        }
    }
}

void escape() {
    char buff;
    if(read(0, &buff, 1)) {
        if (buff == '[') {
            csi();
        }
    }
}

int readInput() {
    int chars_read = 0;
    char buff;
    do {
        chars_read = read(0, &buff, 1);
        if (buff == '\e') {
            escape();
        } else {
            writeLog(LOG_INPUT_V, "input::readInput(): ASCII Input %d", buff);
            createKeyboardEvent(buff);
#ifdef DEBUG_INPUT
            printf("%d\n", buff);
#endif
        }
    } while (!chars_read);
}

