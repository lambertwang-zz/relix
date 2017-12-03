#include "render/render.h"

#include "../light/light.h"
#include "../world/world.h"

#include "player.h"

int renderPlayerLight(Object *o, Screen *s) {
    pointLight(s, world.current_map, o->pos, COLOR_WHITE, 10);
    return 1;
}

