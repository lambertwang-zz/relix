#ifndef __UI_H__
#define __UI_H__

// Engine
#include "constants.h"
#include "list/tree.h"
#include "term/screen.h"
#include "string/string.h"

// Style definitions
#define POS_STATIC 0
// #define POS_REL 1
// #define POS_ABS 2
// #define POS_FIXED 3

typedef struct UiManager {
    Tree ui_tree;
} UiManager;

typedef struct Element {
    int id;

    // Unique label for the element
    String *tag;

    // Text content of the element
    String *text;

    // Styling
    int positioning;

    // Top left coordinates for the element
    Point pos;

    // Text and background color
    Color text_c;
    Color bg_c;

    // Function callbacks
    int (*onRender)(struct Element *, Screen *);
    int (*onClick)(struct Element *);

    // Private
    Tree _children;
    Rect _bounds;
} Element;

Element *createElement();

int initUi();
int closeUi();
int renderUi();

int registerUiElement(Element *e);

#endif

