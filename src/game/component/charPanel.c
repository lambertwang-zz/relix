#include "component.h"

// Library
#include <stdlib.h>

// Engine
#include "log/log.h"
#include "ui/ui.h"
#include "render/render.h"

// Game
#include "../character/character.h"
#include "../relix.h"

struct CharPanel {
    Object *data;
    String *buffer;
};

int onRenderCharPanelEl(Element *e, Screen *s) {
    onRenderDefaultElement(e, s);

    int width = e->_bounds.right - e->_bounds.left;
    Point pos = (Point){e->_bounds.left + 1, e->_bounds.top + 1, 0};

    struct CharPanel *data = e->data;
    Object *obj_data = data->data;
    CharacterData *char_data = obj_data == NULL ? NULL : obj_data->data;


    // Display name
    sputf(data->buffer, "Name: %s", char_data == NULL ? "" : char_data->name->s);
    rawPutString(s, e->id, 0, width - 2, pos, data->buffer, e->text_c, COLOR_EMPTY, 1);

    // Render portrait
    // Pull pixels from the screen buffer (unethical?)
    if (obj_data != NULL) {
        Point s_pos = obj_data->pos;
        s_pos.x -= s->camera_bounds.left + 2;
        s_pos.y -= s->camera_bounds.top + 2;

        int i, j;
        for (j = 0; j < 5; j++) {
            for (i = 0; i < 5; i++) {
                Pixel p = getPixel(s, (Point){s_pos.x + i, s_pos.y + j, 0});
                if (p.depth != UI_DEPTH) {
                    p.id = e->id;
                    p.depth = UI_DEPTH;
                } else {
                    p = PIXEL_BLANK;
                }
                putPixelL(s, pos.x + i, pos.y + j + 2, p);
            }
        }
    }

    return 1;
}

int listenCharInfo(Element *el, Event *ev) {
    writeLog(LOG_USER, "charPanel::listenCharInfo(): Got some char info");
    ((struct CharPanel *) el->data)->data = *(Object **)ev->data;

    return 0;
}

void deleteCharPanelEl(Element *el) {
    deleteString(((struct CharPanel *) el->data)->buffer);
}


void createCharPanel() {
    Element *char_panel;
    char_panel = createElement();
    char_panel->bg_c = THEME_BG;
    char_panel->bg_c_focus = THEME_BG_FOCUS;
    char_panel->focusable = 1;

    char_panel->positioning = POS_REL;
    char_panel->sizing = SIZE_REL;

    char_panel->pos = (Point){68, VPOS_THIRD_1, 0.0};

    char_panel->width = 30;
    char_panel->height = VSIZE_1_THIRD;

    struct CharPanel *data = malloc(sizeof(struct CharPanel));
    char_panel->data = data;
    data->data = NULL;
    data->buffer = createString();

    char_panel->onRender = &onRenderCharPanelEl;
    char_panel->onDelete = &deleteCharPanelEl;

    registerUiElement(char_panel);
    listenUiEvent(char_panel, EVENT_CHAR_INFO, &listenCharInfo);
}

