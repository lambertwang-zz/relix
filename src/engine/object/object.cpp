#include "object.h"

// Library
#include <stdlib.h>
#include <string.h>

// Engine
#include "constants.h"
#include "objectManager.h"
#include "render/render.h"
#include "log/log.h"

namespace rlx {

int Object::render(Screen *s) {
    pix.depth = position.z;
    // Position on the screen
    // Rect bounds = s->getCameraBounds();
    Point rel_pos = (Point){
            position.x, // - bounds.left,
            position.y, // - bounds.top,
            0
    };

    return 1;
}

Object::~Object() {
    delete type;
}

Object::Object() {
    type = new String();

    position.x = 0;
    position.y = 0;
    position.z = 0;

    pix = PIXEL_NULL;
    pix.id = id;
    pix.depth = position.z;
    isSolid = OBJ_ETHER;
}

}
