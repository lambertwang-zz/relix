#include "component.h"

// Engine
#include "ui/ui.h"

void createPlayerPanel() {
    Element *player_panel;
    player_panel = createElement();
    player_panel->bg_c = THEME_BG;
    player_panel->bg_c_focus = THEME_BG_FOCUS;
    player_panel->focusable = 1;

    player_panel->positioning = POS_REL;
    player_panel->sizing = SIZE_REL;
    // player_panel->sizing = SIZE_ABS;

    player_panel->pos = (Point){68, VPOS_THIRD_2, 0.0};

    player_panel->width = 30;
    player_panel->height = VSIZE_2_THIRD;

    registerUiElement(player_panel);
}

