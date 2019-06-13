#include "input.h"

// Library
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#if defined __linux__
#include <termios.h>
#elif defined _WIN32 || defined _WIN64
#include <wchar.h>
#include <windows.h>
#include <conio.h>
#else
#error "unknown platform"
#endif

// Engine
#include "object/objectManager.h"
#include "log/log.h"
#include "constants.h"
#include "inputEvents.h"

namespace rlx {

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

#if defined __linux__
static struct termios prev_settings;
static int old_fl;
#elif defined _WIN32 || defined _WIN64
HANDLE h_stdin;
HANDLE h_stdout;
DWORD in_settings;
DWORD out_settings;
#else
#error "unknown platform"
#endif

int initInput() {
#if defined __linux__
    writeLog(LOG_INPUT, "input::initInput(): Initialzing input for linux");
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
#elif defined _WIN32 || defined _WIN64
    writeLog(LOG_INPUT, "input::initInput(): Initialzing input for windows");
    h_stdout = GetStdHandle(STD_OUTPUT_HANDLE);
    if (h_stdout == INVALID_HANDLE_VALUE) {
        writeLog(LOG_INPUT, "input::initInput(): Error: Unable to get stdout handle.");
        return -2;
    }

    h_stdin = GetStdHandle(STD_INPUT_HANDLE);
    if (h_stdin == INVALID_HANDLE_VALUE) {
        writeLog(LOG_INPUT, "input::initInput(): Error: Unable to get stdin handle.");
        return -2;
    }

    GetConsoleMode(h_stdin, &in_settings);
    DWORD new_in_settings = in_settings;
    new_in_settings &= ~ENABLE_ECHO_INPUT;
    new_in_settings &= ~ENABLE_LINE_INPUT;
    new_in_settings |= ENABLE_MOUSE_INPUT;
    new_in_settings |= ENABLE_EXTENDED_FLAGS;
    new_in_settings &= ~ENABLE_QUICK_EDIT_MODE;
    // new_settings |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(h_stdin, new_in_settings);

    GetConsoleMode(h_stdout, &out_settings);
    DWORD new_out_settings = out_settings;
    new_out_settings |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(h_stdout, new_out_settings);
#else
    writeLog(LOG_INPUT, "input::initInput(): Error: Unknown platform; cannot initialize input.");
    return -1;
#endif
    // Begin reporting mouse movements
    // printf("\e[1;0'z");
    printf("\e[?1003h");
    printf("\e[?1006h");
    fflush(stdout);

    // Register events with the ObjectManager
    ObjectManager::registerEvent(EVENT_KEYBOARD);
    ObjectManager::registerEvent(EVENT_MOUSE);

    writeLog(LOG_INPUT, "input::initInput(): Successfully initialized input.");

    return 0;
}

int closeInput() {
    writeLog(LOG_INPUT, "input::closeInput(): Closing input");

    // End reporting mouse movements
    printf("\e[?1003l");
    printf("\e[?1006l");
    fflush(stdout);
    
#if defined __linux__
    // Restore old settings
    tcsetattr(0, TCSANOW, &prev_settings);
    fcntl(STDIN_FILENO, F_SETFL, old_fl);
#elif defined _WIN32 || defined _WIN64
    // Restore old settings
    SetConsoleMode(h_stdin, in_settings);
    SetConsoleMode(h_stdout, out_settings);
#else
    writeLog(LOG_INPUT, "input::closeInput(): Error: Unknown platform; cannot close input.");
    return -1;
#endif
    return 0;
}

int createKeyboardEvent(char chr, int type) {
    KeyboardEvent *ev = new KeyboardEvent(chr, type);
    ObjectManager::sendEvent(ev);
    return 0;
}

int createMouseEvent(int x, int y, int button, char status) {
    Screen *main_screen = ScreenManager::getMainScreen();

    MouseEvent *ev = new MouseEvent(status,
        //m x - 1 - main_screen->getMarginX(),
        //m y - 1 - main_screen->getMarginX(),
        x,
        y,
        button);

    ObjectManager::sendEvent(ev);
    return 0;
}

int readCh(unsigned char *out) {
#if defined __linux__
    return read(0, out, 1);
#elif defined _WIN32 || defined _WIN64
    if (_kbhit()) {
        // if (!ReadConsole(h_stdout, &out, 1, &read_result, NULL)) {
        *out = getch();
        return 1;
    }
    // writeLog(LOG_INPUT_V, "KB Not hit");
#else
#error "unknown platform"
#endif
    return -1;
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

int sgrMouse() {
    char button_str[SGR_BUFF_LEN];
    char x_str[SGR_BUFF_LEN];
    char y_str[SGR_BUFF_LEN];
    char buff;

    readSgrValue(button_str);
    readSgrValue(x_str);
    buff = readSgrValue(y_str);

    writeLog(LOG_INPUT_V, "input::csiMouse(): Mouse: Button: %s X: %s Y: %s", button_str, x_str, y_str);
    return createMouseEvent(atoi(x_str), atoi(y_str), atoi(button_str), buff);
}

int x10Mouse() {
    unsigned char button_chr;
    unsigned char x_chr;
    unsigned char y_chr;

    readCh(&button_chr);
    readCh(&x_chr);
    readCh(&y_chr);
    char action = MOUSE_PRESS;
    if (button_chr % 32 == 3) {
        action = MOUSE_RELEASE;
    }

    writeLog(LOG_INPUT_V, "input::x10Mouse(): Mouse: Button: %u X: %u Y: %u", button_chr, x_chr, y_chr);
    return createMouseEvent(x_chr - 32, y_chr - 32, button_chr - 32, action);
}

int csiArrow(char code) {
    writeLog(LOG_INPUT_V, "input::csiArrow(): CSI arrow code: %d", code);
    createKeyboardEvent(code, KEYBOARD_ESCAPE);
    return 0;
}

int csi() {
    unsigned char buff;
    if (readCh(&buff)) {
        writeLog(LOG_INPUT_V, "input::csi(): CSI code: %d", buff);
        switch (buff) {
            case '<': // SGR
                return sgrMouse();
#ifdef __LINUX__
            case 'M': // x10 compat
                return x10Mouse();
#endif
            case ARROW_UP_WIN:
                return csiArrow(ARROW_UP);
            case ARROW_DOWN_WIN:
                return csiArrow(ARROW_DOWN);
#if defined _WIN32 || defined _WIN64
            case ARROW_RIGHT_WIN:
                return csiArrow(ARROW_RIGHT);
#endif
            case ARROW_LEFT_WIN:
                return csiArrow(ARROW_LEFT);
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
            case 0:
                return createKeyboardEvent('\e', KEYBOARD_NORMAL);
            default:
                writeLog(LOG_INPUT, "input::escape(): Unsupported escape character: '%d'", buff);
        }
    }
    return 1;
}


int captureInput() {
    unsigned char buff;
    int read_result;

    do {
        read_result = readCh(&buff);
        if (read_result == -1) {
            return read_result;
        }
#ifdef DEBUG_INPUT
        printf("%d\n", buff);
#endif
        writeLog(LOG_INPUT_V, "input::readInput(): ASCII Input %u  %c", buff, buff);
        if (buff == '\e') {
            writeLog(LOG_INPUT_V, "input::readInput(): Escape code %u", buff);
        }
        switch (buff) {
            case '\e':
                return escape();
            case 224:
                return csi();
            default:
                return createKeyboardEvent(buff, KEYBOARD_NORMAL);
        }
    } while (!read_result);

    return 1;
}

#if defined _WIN32 || defined _WIN64
int winMouseEvent(MOUSE_EVENT_RECORD ev) {
    static unsigned int prev_button_state = 0;

    int x_pos = ev.dwMousePosition.X;
    int y_pos = ev.dwMousePosition.Y;
    int button = MOUSE_NONE;
    char status = MOUSE_RELEASE;
    // int state_diff = prev_button_state ^ ev.dwButtonState;

    switch (ev.dwEventFlags) {
        case 0:
            if (ev.dwButtonState == prev_button_state) {
                break;
            }
            switch (ev.dwButtonState) {
                case FROM_LEFT_1ST_BUTTON_PRESSED:
                    button = MOUSE_LEFT;
                    break;
                case RIGHTMOST_BUTTON_PRESSED:
                    button = MOUSE_RIGHT;
                    break;
                case FROM_LEFT_2ND_BUTTON_PRESSED:
                    button = MOUSE_MIDDLE;
                    break;
            }
            if (button != MOUSE_NONE) {
                status = MOUSE_PRESS;
            }
            break;
        case MOUSE_MOVED:
            button = MOUSE_MOVE;
            break;
        default:
            writeLog(LOG_INPUT_V, "input::winMouseEvent(): Unsupported Mouse Event %d", ev.dwEventFlags);
            return 1;
    }
    prev_button_state = ev.dwButtonState;

    writeLog(LOG_INPUT_V, "input::winMouseEvent(): Mouse: Button: %d X: %d Y: %d", button, x_pos, y_pos);
    return createMouseEvent(x_pos, y_pos, button, status);
}
#endif

// Windows only function
int captureInputEvents() {
#if defined _WIN32 || defined _WIN64
    INPUT_RECORD input;
    DWORD in_result;
    DWORD num_events;
    unsigned int i;
    GetNumberOfConsoleInputEvents(h_stdin, &num_events);

    for (i = 0; i < num_events; i++) {
        if (!ReadConsoleInput(h_stdin, &input, 1, &in_result)) {
            return -1;
        } else {
            switch (input.EventType) {
                case KEY_EVENT:
                    writeLog(LOG_INPUT, "input::readInputEvents(): Unsupported input event");
                    break;
                case MOUSE_EVENT:
                    winMouseEvent(input.Event.MouseEvent);
                    break;
                case WINDOW_BUFFER_SIZE_EVENT:
                    resize_flag = 1;
                    break;
                case FOCUS_EVENT:
                case MENU_EVENT:
                default:
                    writeLog(LOG_INPUT, "input::readInputEvents(): Unsupported input event");
                    break;
            }
            // Handle the event
            return 0;
        }
    }
#endif

    return 1;
}

int readInput() {
    int end_input;
    do {
        end_input = captureInput();
    } while (!end_input);
    // Keyboard events pass (windows only)
    do {
        end_input = captureInputEvents();
    } while (!end_input);
    return end_input;
}

}
