#include "ui.h"

// Library
#include <stdlib.h>
#include <string.h>

// Engine
#include "render/render.h"

static UiManager ui_manager;

int onRenderDefaultElement(Element *e, Screen *s) {
    putStringL(s, e->pos.x, e->pos.y, e->text, e->text_c, e->bg_c);

    Iterator *it;

    it = initIterator(&e->_children);
    while (!done(it)) {
        Element *elem = getNext(it)->data;
        elem->onRender(elem, s);
    }
    closeIterator(it);

    return 1;
}

Element *createElement() {
    static int ui_elem_iterator = 0;

    Element *new_elem = malloc(sizeof(Element));
    new_elem->id = ui_elem_iterator++;
    new_elem->tag = createString();
    new_elem->text = createString();


    // Default Styling
    new_elem->positioning = POS_STATIC;
    new_elem->pos = (Point){0, 0, UI_DEPTH};
    
    new_elem->text_c = COLOR_WHITE;
    new_elem->bg_c = COLOR_EMPTY;


    // Default callbacks
    new_elem->onRender = &onRenderDefaultElement;
    new_elem->onClick = NULL;

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
    initTree(&ui_manager.ui_tree);
    return 0;
}

int clearUi() {
    Iterator *it;

    it = initIterator(&ui_manager.ui_tree);
    while (!done(it)) {
        Element *elem = getNext(it)->data;
        deleteElement(elem);
    }
    closeIterator(it);

    return 0;
}

int sendUiEvent() {
    return 0;
}

int closeUi() {
    closeTree(&ui_manager.ui_tree);
    return 0;
}

int registerUiElement(Element *e) {
    return insert(&ui_manager.ui_tree, e, e->id);
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

