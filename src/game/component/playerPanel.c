#include "component.h"

// Engine
#include "ui/ui.h"

void createPlayerPanel() {
    Element *player_panel;
    player_panel = createElement();
    player_panel->bg_c = (Color){32, 32, 32, 0.7};
    player_panel->bg_c_focus = (Color){32, 32, 32, 0.9};
    player_panel->focusable = 1;

    player_panel->positioning = POS_REL;
    player_panel->sizing = SIZE_REL;
    // player_panel->sizing = SIZE_ABS;

    player_panel->pos = (Point){68, 8, 0.0};

    player_panel->width = 30;
    player_panel->height = 84;

    registerUiElement(player_panel);
}
