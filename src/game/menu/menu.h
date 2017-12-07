#ifndef __MENU_H__
#define __MENU_H__

#include "geometry/geometry.h"

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
    char label[MAX_LABEL_SIZE];
    int label_len;

    Point pos;
    Rect bounds;
};

void initMainMenu();

#endif

