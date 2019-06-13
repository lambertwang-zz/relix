#include "ui.h"

// Library
#include <stdlib.h>

// Engine
#include "log/log.h"
#include "input/input.h"

int ensureUiEventExists(char *function, int ev_id) {
    if (getData(&getUiManager()->event_listeners, ev_id) == NULL) {
        writeLog(LOG_ObjectManager, "uiListener::%s(): ERROR: Event id %d not registered", function, ev_id);
        return 2;
    }

    return 0;
}

int sendUiEvent(Event *ev) {
    Screen screen = screen_manager.main_screen;
    MouseEvent *m_ev;
    UiManager *ui_manager = getUiManager();

    // Handle input events separately
    switch (ev->id) {
        case EVENT_MOUSE:
            writeLog(LOG_UI_V, "uiListener::sendUiEvent(): Sending mouse event to UI elements.");
            m_ev = ev->data;
            writeLog(LOG_UI_V, "uiListener::sendUiEvent(): Finding element at x: %d y: %d.", m_ev->x, m_ev->y);

            // Get ui element under cursor
            if (m_ev->x < 0 || m_ev->y < 0 || m_ev->x >= screen.ts.cols || m_ev->y >= screen.ts.lines) {
                return 0;
            }
            Pixel p = screen_manager.main_screen.pixel_buffer[m_ev->x + m_ev->y * screen_manager.main_screen.ts.cols];
            writeLog(LOG_UI_V, "uiListener::sendUiEvent(): Id: %d Depth: %d", p.id, p.depth);
            writeLog(LOG_UI_V, "uiListener::sendUiEvent(): r: %d g: %d b: %d", p.bg.r, p.bg.g, p.bg.b);
            if (p.depth == UI_DEPTH) {
                Element *e = getData(&ui_manager->ui_tree, p.id);
                return e->onEvent(e, ev);
            } else {
                writeLog(LOG_UI_V, "uiListener::sendUiEvent(): Setting focus to none.");
                setFocus(NULL);
            }

            return 0;
        case EVENT_KEYBOARD:
            if (getFocus() != NULL) {
                getFocus()->onEvent(getFocus(), ev);
            }
            return 0;
        default:
            break;
    }

    // For all other events
    if (ensureUiEventExists("sendUiEvent", ev->id)) {
        return 1;
    }

    Iterator it = initIterator(getData(&ui_manager->event_listeners, ev->id));
    while (!done(&it) && !ev->stop_propagation) {
        Element *el = getNext(&it)->data;

        int (*listener)(Element *, Event *) = getData(&el->event_listeners, ev->id);
        if (listener == NULL) {
            writeLog(LOG_ObjectManager, "uiListener::sendUiEvent(): WARNING: No listener for Event id %d for element", ev->id, el->id);
            continue;
        }
        listener(el, ev);
    }

    return 0;
}

int registerUiListener(const Element *el, int ev_id) {
    if (ensureUiEventExists("registerListener", ev_id)) {
        return 1;
    }

    if (insert(getData(&getUiManager()->event_listeners, ev_id), (void *)el, el->id)) {
        writeLog(LOG_ObjectManager, "uiListener::registerUiListener(): WARN: Object id %d already registered to event id %d", el->id, ev_id);
        return 1;
    }
    
    return 0;
}

int unregisterUiListener(const Element *el, int ev_id) {
    if (ensureUiEventExists("unregisterListener", ev_id)) {
        return 1;
    }

    if (removeId(getData(&getUiManager()->event_listeners, ev_id), el->id)) {
        writeLog(LOG_ObjectManager, "uiListener::unregisterUiListener(): WARN: Object id %d already unregistered from event id %d", el->id, ev_id);
        return 1;
    }
    
    return 0;
}


