#include "ui.h"

// Library
#include <stdlib.h>
#include <string.h>

// Engine
#include "render/render.h"
#include "input/input.h"
#include "log/log.h"

static UiManager ui_manager;
static Element *focus;

int onRenderDefaultElement(Element *e, Screen *s) {
    Rect parent_size;
    if (e->_parent == NULL) {
        parent_size = (Rect){0, s->ts.lines, 0, s->ts.cols};
    } else {
        parent_size = e->_parent->_bounds;
    }

    int width, height;
    width = parent_size.right - parent_size.left;
    height = parent_size.bottom - parent_size.top;

    switch (e->sizing) {
        case SIZE_ABS:
            width = e->width;
            height = e->height;
            break;
        case SIZE_REL:
            width = width * e->width / 100;
            height = height * e->height / 100;
            break;
        case SIZE_NONE:
        case SIZE_PARENT:
        default:
            break;
    }

    Point pos;
    if (e->_parent == NULL) {
        pos = (Point){0, 0, 0};
    } else {
        pos = e->_parent->pos;
    }

    switch (e->positioning) {
        case POS_ABS:
            pos.x += e->pos.x;
            pos.y += e->pos.y;
            break;
        case POS_REL:
            pos.x += (parent_size.right - parent_size.left) * e->pos.x / 100;
            pos.y += (parent_size.bottom - parent_size.top) * e->pos.y / 100;
            break;
        case POS_STATIC:
        default:
            break;
    }

    e->_bounds = (Rect){0, height, 0, width};
    // e->_bounds = (Rect){2, 12, 2, 12};

    if (e->sizing != SIZE_NONE) {
        if (focus == e) {
            oputRectL(s, e->id, pos.x, pos.y, e->_bounds, e->bg_c_focus);
        } else {
            oputRectL(s, e->id, pos.x, pos.y, e->_bounds, e->bg_c);
        }
    }
    int row = 0;
    while (row * width < e->text->len && row + pos.y < height) {
        if (focus == e) {
            rawPutString(s, e->id, row * width, (row + 1) * width, pos, e->text, e->text_c, e->bg_c_focus, 1);
        } else {
            rawPutString(s, e->id, row * width, (row + 1) * width, pos, e->text, e->text_c, e->bg_c, 1);
        }
        row++;
    }

    Iterator *it;

    it = initIterator(&e->_children);
    while (!done(it)) {
        Element *elem = getNext(it)->data;
        elem->onRender(elem, s);
    }
    closeIterator(it);

    return 1;
}

int onEventDefaultElement(Element *e, Event *ev) {
    writeLog(LOG_UI_V, "ui::onEventDefaultElement(): Received event.");
    switch (ev->id) {
        case EVENT_MOUSE:
            if (e->focusable) {
                writeLog(LOG_UI_V, "ui::onEventDefaultElement(): Setting focus.");
                focus = e;
                ev->stop_propagation = 1;
                return 1;
            }
            return 0;
        case EVENT_KEYBOARD:
            return 0;
        default:
            return 0;
    }
}

int registerUiElement(Element *e) {
    writeLog(LOG_UI, "ui::registerUiElement(): Registered UI Element id %d.", e->id);
    return insert(&ui_manager.ui_tree, e, e->id);
}


Element *createElement() {
    static int ui_elem_iterator = 0;

    Element *new_elem = malloc(sizeof(Element));
    new_elem->id = ui_elem_iterator++;
    new_elem->tag = createString();
    new_elem->text = createString();

    // Default Styling
    new_elem->focusable = 0;
    new_elem->positioning = POS_STATIC;
    new_elem->sizing = SIZE_NONE;
    new_elem->width = 0;
    new_elem->height = 0;
    new_elem->pos = (Point){0, 0, UI_DEPTH};
    
    new_elem->text_c = COLOR_WHITE;
    new_elem->bg_c = COLOR_EMPTY;
    new_elem->bg_c_focus = COLOR_EMPTY;

    // Default callbacks
    new_elem->onRender = &onRenderDefaultElement;
    new_elem->onEvent = &onEventDefaultElement;

    new_elem->_parent = NULL;
    initTree(&new_elem->_children);
    new_elem->_bounds = (Rect){0, 0, 0, 0};

    return new_elem;
}

int deleteElement(Element *elem) {
    deleteString(elem->tag);
    deleteString(elem->text);

    Iterator *it;

    it = initIterator(&elem->_children);
    while (!done(it)) {
        Element *elem = getNext(it)->data;
        deleteElement(elem);
    }
    closeIterator(it);

    closeTree(&elem->_children);

    free(elem);
    return 0;
}

int initUi() {
    writeLog(LOG_UI, "ui::initUi(): Initializing UI.");
    initTree(&ui_manager.ui_tree);
    focus = NULL;
    return 0;
}

int clearUi() {
    writeLog(LOG_UI, "ui::clearUi(): Clearing UI.");
    Iterator *it;

    it = initIterator(&ui_manager.ui_tree);
    while (!done(it)) {
        Element *elem = getNext(it)->data;
        deleteElement(elem);
    }
    closeIterator(it);

    return 0;
}

int closeUi() {
    writeLog(LOG_UI, "ui::closeUi(): Closing UI.");
    closeTree(&ui_manager.ui_tree);
    return 0;
}

int renderUi(Screen *s) {
    int elements_rendered = 0;
    Iterator *it;

    it = initIterator(&ui_manager.ui_tree);
    while (!done(it)) {
        Element *elem = getNext(it)->data;
        elements_rendered += elem->onRender(elem, &screen_manager.main_screen);
    }
    closeIterator(it);

    return elements_rendered;
}

int sendUiEvent(Event *ev) {
    Screen screen = screen_manager.main_screen;
    MouseEvent *m_ev;
    switch (ev->id) {
        case EVENT_MOUSE:
            writeLog(LOG_UI_V, "ui::sendUiEvent(): Sending mouse event to UI elements.");
            m_ev = ev->data;
            writeLog(LOG_UI_V, "ui::sendUiEvent(): Finding element at x: %d y: %d.", m_ev->x, m_ev->y);

            // Get ui element under cursor
            if (m_ev->x < 0 || m_ev->y < 0 || m_ev->x >= screen.ts.cols || m_ev->y >= screen.ts.lines) {
                return 0;
            }
            Pixel p = screen_manager.main_screen.pixel_buffer[m_ev->x + m_ev->y * screen_manager.main_screen.ts.cols];
            writeLog(LOG_UI_V, "ui::sendUiEvent(): Id: %d Depth: %d", p.id, p.depth);
            writeLog(LOG_UI_V, "ui::sendUiEvent(): r: %d g: %d b: %d", p.bg.r, p.bg.g, p.bg.b);
            if (p.depth == UI_DEPTH) {
                Element *e = getData(&ui_manager.ui_tree, p.id);
                return e->onEvent(e, ev);
            } else {
                writeLog(LOG_UI_V, "ui::sendUiEvent(): Setting focus to none.");
                focus = NULL;
            }

            return 0;
        case EVENT_KEYBOARD:
            if (focus != NULL) {
                focus->onEvent(focus, ev);
            }
            return 0;
        default:
            break;
    }

    return 0;
}

