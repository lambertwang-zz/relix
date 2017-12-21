#include "render/render.h"

#include "../light/light.h"
#include "../world/world.h"

#include "player.h"

int renderPlayerLight(Object *o, Screen *s) {
    pointLight(s, getWorldData()->current_map, o->pos, 
        // (Color){255, 255, 135},
        (Color){160, 160, 80, 1.0},
        10);
    return 1;
}

