#include "ui.h"

// Library
#include <stdlib.h>
#include <string.h>

// Engine
#include "log/log.h"

static UiManager ui_manager;
static Element *focus;

UiManager *getUiManager() {
    return &ui_manager;
}

Element *getFocus() {
    return focus;
}

void setFocus(Element *elem) {
    focus = elem;
}

int registerUiElement(Element *e) {
    writeLog(LOG_UI, "ui::registerUiElement(): Registered UI Element id %d.", e->id);
    return insert(&ui_manager.ui_tree, e, e->id);
}


int initUi() {
    writeLog(LOG_UI, "ui::initUi(): Initializing UI.");
    initTree(&ui_manager.ui_tree);
    initTree(&ui_manager.event_listeners);
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

    it = initIterator(&ui_manager.event_listeners);
    while (!done(it)) {
        Tree *tree = getNext(it)->data;
        clearTree(tree);
    }
    closeIterator(it);


    return 0;
}

int closeUi() {
    writeLog(LOG_UI, "ui::closeUi(): Closing UI.");
    clearUi();
    closeTree(&ui_manager.ui_tree);

    Iterator *it = initIterator(&ui_manager.event_listeners);
    while (!done(it)) {
        Tree *tree = getNext(it)->data;
        closeTree(tree);
        free(tree);
    }
    closeIterator(it);
    closeTree(&ui_manager.event_listeners);

    return 0;
}

int renderUi() {
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

