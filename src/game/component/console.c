#include "component.h"

// Engine
#include "ui/ui.h"

// Engine
#include "log/log.h"
#include "render/render.h"

// Game
#include "../relix.h"

#define MAX_SAVED_MESSAGES 128
#define CONSOLE_NAME "Messages:"

static Array console_lines;
static String *console_name;

int onRenderConsoleElement(Element *e, Screen *s) {
    onRenderDefaultElement(e, s);

    int width = e->_bounds.right - e->_bounds.left;

    // Delete excess messages
    while (console_lines.count > MAX_SAVED_MESSAGES) {
        deleteString(deleteArrayIndex(&console_lines, 0));
    }

    Point pos = (Point){e->_bounds.left + 1, e->_bounds.top + 1, 0};

    rawPutString(s, e->id, 0, width - 2, pos, console_name, e->text_c, COLOR_EMPTY, 1);

    if (console_lines.count == 0) {
        return 1;
    }

    unsigned int i = console_lines.count - 1;

    pos.y += 2;
    while (pos.y < e->_bounds.bottom - 1 && i < console_lines.count) {
        int start = 0;
        String *line = getDataArray(&console_lines, i--);
        while (start < line->len && pos.y < e->_bounds.bottom - 1) {
            rawPutString(s, e->id, start, start + width - 2, pos, line, e->text_c, COLOR_EMPTY, 1);
            start += width - 2;
            pos.y++;
        }
    }

    return 1;
}

int expandConsole(Element *el, MouseEvent ev) {
    if (ev.status == MOUSE_PRESS) {
        if (el->height == VSIZE_1_HALF) {
            el->pos = (Point){HSPACE_GAP, VPOS_HALF_1, 0.0};
            el->height = VSIZE_2_HALF;
        } else {
            el->pos = (Point){HSPACE_GAP, VPOS_HALF_2, 0.0};
            el->height = VSIZE_1_HALF;
        }
    }
    return 0;
}

int listenConsoleMessage(Element *el, Event *ev) {
    el = el;
    String *message = *(String **)ev->data;
    writeLog(LOG_USER, "console::listenConsoleMessage(): Received message: '%s'", message->s);

    push(&console_lines, message);

    return 0;
}

void deleteConsoleElement(Element *el) {
    el = el;
    unsigned int i;
    for (i = 0; i < console_lines.count; i++) {
        deleteString(getDataArray(&console_lines, i));
    }
    closeArray(&console_lines);
    deleteString(console_name);
}

void createConsole() {
    initArray(&console_lines);
    console_name = createString();
    sputf(console_name, CONSOLE_NAME);

    Element *console;
    console = createElement();
    console->bg_c = THEME_BG;
    console->bg_c_focus = THEME_BG_FOCUS;
    console->focusable = 1;

    console->positioning = POS_REL;
    console->sizing = SIZE_REL;
    // console->sizing = SIZE_ABS;

    console->pos = (Point){HSPACE_GAP, VPOS_HALF_2, 0.0};

    console->width = 30;
    console->height = VSIZE_1_HALF;

    console->onRender = &onRenderConsoleElement;
    console->onClick = &expandConsole;
    console->onDelete = &deleteConsoleElement;

    registerUiElement(console);
    listenUiEvent(console, EVENT_MESSAGE, &listenConsoleMessage);
}

