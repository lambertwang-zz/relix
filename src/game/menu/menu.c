#include "menu.h"

// Library
#include <stdlib.h>
#include <string.h>

// Engine
#include "constants.h"
#include "input/input.h"
#include "log/log.h"
#include "object/object.h"
#include "object/objectManager.h"
#include "render/render.h"
#include "term/screen.h"

int menuKeyboardListener(struct Object *o, Event ev) {
    KeyboardEvent k_ev = *(KeyboardEvent *)ev.data;
    struct MenuData *menu = o->data;
    switch (k_ev.type) {
        case KEYBOARD_NORMAL:
            switch (k_ev.value) {
            }
            return 0;
        case KEYBOARD_ESCAPE:
            switch (k_ev.value) {
                case ARROW_UP:
                    menu->item_index = (menu->item_index + menu->item_count - 1) % menu->item_count;
                    break;
                case ARROW_DOWN:
                    menu->item_index = (menu->item_index + 1) % menu->item_count;
                    break;
            }
            return 0;
    }
    
    return 0;
}

int menuMouseListener(struct Object *o, Event ev) {
    MouseEvent m_ev = *(MouseEvent *)ev.data;
    struct MenuData *menu = o->data;
    int i, target = -1;

    for (i = 0; i < menu->item_count; i++) {
        if (pointInRect((Point){m_ev.x - menu->items[i].pos.x, 
                                m_ev.y - menu->items[i].pos.y,
                                0},
                        menu->items[i].bounds)) {
            target = i;
            menu->item_index = i;
        }
    }
    if (target == -1) {
        return target;
    }
    switch (m_ev.value) {
        case MOUSE_MOVE:
            
            return 0;
            /*
        case KEYBOARD_ESCAPE:
            switch (k_ev.value) {
                case ARROW_UP:
                    menu->item_index = (menu->item_index + menu->item_count - 1) % menu->item_count;
                    break;
                case ARROW_DOWN:
                    menu->item_index = (menu->item_index + 1) % menu->item_count;
                    break;
            }
            return 0;
    */
    }
    
    return 0;
}

int render_menu(Object *o, Screen *s) {
    struct MenuData *menu = o->data;
    int i;
    Color c = COLOR_WHITE;
    Rect button = (Rect){0, MENU_BUTTON_HEIGHT, 0, MENU_BUTTON_WIDTH};
    int x = (s->ts.cols - MENU_BUTTON_WIDTH) >> 1;
    for (i = 0; i < menu->item_count; i++) {
        Color bg = COLOR_RED;
        if (i != menu->item_index) {
            bg = scaleColor(bg, 0.2);
        }

        int y = s->ts.lines / 2 - (MENU_BUTTON_HEIGHT + 1) * (menu->item_count / 2 - i);
        putRectL(s, x, y, button, bg);
        menu->items[i].pos = (Point){x, y, 0};
        menu->items[i].bounds = button;
        putStringL(s, (screen_manager.main_screen.ts.cols - menu->items[i].label->len) >> 1, y + 1, menu->items[i].label, c, COLOR_EMPTY);
    }

    return 1;
}

void close_menu(struct Object *o) {
    struct MenuData *menu = o->data;
    free(menu->items);
    closeDefault(o);
}

void initMainMenu() {
    struct Object *menu_object = createObject();
    struct MenuData *menu = malloc(sizeof(struct MenuData));

    menu->item_count = 4;
    menu->item_index = 0;
    menu->items = malloc(sizeof(struct MenuItem) * menu->item_count);

    sputf(menu->items[0].label, MENU_START);
    sputf(menu->items[1].label, MENU_START);
    sputf(menu->items[2].label, MENU_START);
    sputf(menu->items[3].label, MENU_START);

    menu_object->pos.z = 100;

    menu_object->data = menu;
    menu_object->render = &render_menu;
    menu_object->close = &close_menu;

    listenEvent(menu_object, EVENT_KEYBOARD, &menuKeyboardListener);
    listenEvent(menu_object, EVENT_MOUSE, &menuMouseListener);

    addObject(menu_object);
}

