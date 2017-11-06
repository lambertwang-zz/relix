#include "game/game.h"
#include "objects/object.h"
#include "objects/objectManager.h"
#include "constants.h"
#include "log/log.h"
#include "input/input.h"

#include "stdlib.h"

void nothing() {
    // pass
}

int keyboardListener(struct Object *o, Event ev) {
    writeLog(10, "Received keyboard event");
    KeyboardEvent k_ev = *(KeyboardEvent *)ev.data;
    switch (k_ev.type) {
        case KEYBOARD_NORMAL:
            switch (k_ev.value) {
                case 'q':
                case 'Q':
                    queueClear(&nothing);
                    break;
            }
            return 0;
        case KEYBOARD_ESCAPE:
            writeLog(10, "Received keyboard escape %d", k_ev.value);
            switch (k_ev.value) {
                case ARROW_UP:
                    o->pos.y--;
                    break;
                case ARROW_DOWN:
                    o->pos.y++;
                    break;
                case ARROW_LEFT:
                    o->pos.x--;
                    break;
                case ARROW_RIGHT:
                    o->pos.x++;
                    break;
            }
            return 0;
    }
    
    return 0;
}

int main(int argc, char **argv) {
    initGame();

    addLogLevel(LOG_INPUT_V);

    struct Object *o_1 = malloc(sizeof(struct Object));

    initObject(o_1);

    o_1->pix.chr = '@';
    o_1->pix.fg = 128;

    listenEvent(o_1, EVENT_KEYBOARD, &keyboardListener);

    addObject(o_1);

    loop();

    closeGame(0);
    return 0;
}

