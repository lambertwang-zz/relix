#ifndef __UI_H__
#define __UI_H__

// Engine
#include "event/event.h"
#include "constants.h"
#include "list/tree.h"
#include "term/screen.h"
#include "string/string.h"

// Style definitions
// Positioned to top-left of parent
#define POS_STATIC 0
// Position in pixels
#define POS_ABS 1
// Position in % of parent
#define POS_REL 2

#define SIZE_NONE 0
// Absolute size in pixels
#define SIZE_ABS 1
// Size relative to parent in percentage
#define SIZE_REL 2
// Match size to parent
#define SIZE_PARENT 3

typedef struct UiManager {
    Tree ui_tree;
} UiManager;

typedef struct Element {
    int id;

    // Unique label for the element
    String *tag;

    int focusable;

    // Text content of the element
    String *text;

    // Styling
    int positioning;
    int sizing;

    int width;
    int height;

    // Top left coordinates for the element
    Point pos;

    // Text and background color
    Color text_c;
    Color bg_c;
    Color bg_c_focus;

    // Function callbacks
    int (*onRender)(struct Element *, Screen *);
    int (*onEvent)(struct Element *, Event *);

    // Private
    struct Element *_parent;
    Tree _children;
    Rect _bounds;
} Element;

int registerUiElement(Element *e);

Element *createElement();

int initUi();
int closeUi();
int renderUi();

int sendUiEvent(Event *ev);

#endif

