#include "ui.h"

// Library
#include <stdlib.h>

// Engine
#include "log/log.h"
#include "render/render.h"

int onRenderDefaultElement(Element *e, Screen *s) {
    Rect parent_size;
    if (e->_parent == NULL) {
        // If no parent, set 'parent' to screen
        parent_size = (Rect){0, s->ts.lines, 0, s->ts.cols};
    } else {
        parent_size = e->_parent->_bounds;
    }

    int width, height;
    width = parent_size.right - parent_size.left;
    height = parent_size.bottom - parent_size.top;

    // Calculate component size
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


    // Calculate component position
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

    if (e->sizing != SIZE_NONE) {
        if (getFocus() == e) {
            oputRectL(s, e->id, pos.x, pos.y, e->_bounds, e->bg_c_focus);
        } else {
            oputRectL(s, e->id, pos.x, pos.y, e->_bounds, e->bg_c);
        }
    }
    int row = 0;
    while (row * width < e->text->len && row + pos.y < height) {
        if (getFocus() == e) {
            rawPutString(s, e->id, row * width, (row + 1) * width, pos, e->text, e->text_c, e->bg_c_focus, 1);
        } else {
            rawPutString(s, e->id, row * width, (row + 1) * width, pos, e->text, e->text_c, e->bg_c, 1);
        }
        row++;
    }

    // Render children
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
    writeLog(LOG_UI_V, "element::onEventDefaultElement(): Received event.");
    switch (ev->id) {
        case EVENT_MOUSE:
            if (e->focusable) {
                writeLog(LOG_UI_V, "element::onEventDefaultElement(): Setting focus.");
                setFocus(e);
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
    initTree(&new_elem->event_listeners);

    new_elem->data = NULL;

    new_elem->_parent = NULL;
    initTree(&new_elem->_children);
    new_elem->_bounds = (Rect){0, 0, 0, 0};

    return new_elem;
}

int deleteElement(Element *elem) {
    deleteString(elem->tag);
    deleteString(elem->text);
    
    // Remove element from listener trees
    Iterator *it = initIterator(&elem->event_listeners);
    while (!done(it)) {
        Node *n = getNext(it);
        unregisterUiListener(elem, n->id);
    }
    closeIterator(it);
    closeTree(&elem->event_listeners);

    if (elem->data != NULL) {
        free(elem->data);
    }

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

int listenUiEvent(Element *el, int ev_id, int (*listener)(Element *, Event *)) {
    Node *n = getTreeNode(&el->event_listeners, ev_id);
    if (n == NULL) {
        insert(&el->event_listeners, listener, ev_id);
    } else {
        n->data = listener;
    }
    registerUiListener(el, ev_id);
    return 0;
}

