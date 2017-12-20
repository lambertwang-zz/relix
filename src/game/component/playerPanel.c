#include "component.h"

// Engine
#include "ui/ui.h"

void createPlayerPanel() {
    Element *player_panel;
    player_panel = createElement();
    player_panel->bg_c = (Color){64, 64, 64, 1.0};
    player_panel->bg_c_focus = (Color){96, 96, 96, 1.0};
    player_panel->focusable = 1;

    player_panel->positioning = POS_REL;
    player_panel->sizing = SIZE_REL;
    // player_panel->sizing = SIZE_ABS;

    player_panel->pos = (Point){68, 8, 0.0};

    player_panel->width = 28;
    player_panel->height = 84;

    registerUiElement(player_panel);
}
