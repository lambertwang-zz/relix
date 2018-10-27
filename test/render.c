#include "game/game.h"
#include "object/object.h"
#include "object/objectManager.h"
#include "constants.h"
#include "log/log.h"
#include "input/input.h"
#include "render/render.h"

#include "stdlib.h"
#include "string.h"

int testkeyboardListener(struct Object *o, Event *ev) {
    o = o;
    KeyboardEvent k_ev = *(KeyboardEvent *)ev->data;
    switch (k_ev.type) {
        case KEYBOARD_NORMAL:
            return 0;
        case KEYBOARD_ESCAPE:
            switch (k_ev.value) {
                case ARROW_UP:
                    break;
                case ARROW_DOWN:
                    break;
                case ARROW_LEFT:
                    break;
                case ARROW_RIGHT:
                    break;
            }
            return 0;
    }
    
    return 0;
}

static int counter;

int renderTest(Object *o, Screen *s) {
    o = o;
    Pixel p = PIXEL_BLANK;

    int result;

    int i, j;
    for (j = 0; j < s->ts.lines; j++) {
        for (i = 0; i < s->ts.cols; i++) {
            p.bg = hslToRgb((Color){(i + j * 2 + counter) % 256, 64 + j, 255, 1.0});
            p.fg = hslToRgb((Color){(j * 3 - i - counter) % 256, 255 - j, 128, 1.0});
            strcpy(p.chr, "║");
            p.chr[2] += (i - j + 300 + counter) % 30;
            result += putPixelL(s, i, j, p);
        }
    }
    counter++;

    return result;
}


int main() {
    initGame();
    addLogLevel(0xffffffff);

    struct Object *o_1 = createObject();

    counter = 0;

    o_1->render = &renderTest;

    listenEvent(o_1, EVENT_KEYBOARD, &testkeyboardListener);

    addObject(o_1);

    loop();

    closeGame(0);
    return 0;
}

