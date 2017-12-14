#ifndef __MENU_H__
#define __MENU_H__

// Engine
#include "geometry/geometry.h"
#include "string/string.h"

#define MAX_LABEL_SIZE 128

#define MENU_START "Let's Go!"

#define MENU_BUTTON_WIDTH 32
#define MENU_BUTTON_HEIGHT 3

struct MenuData {
    struct MenuItem *items;
    int item_count;
    int item_index;
};

struct MenuItem {
    String *label;

    Point pos;
    Rect bounds;
};

void initMainMenu();

#endif

