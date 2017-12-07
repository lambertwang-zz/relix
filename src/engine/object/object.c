#include "object.h"

// Library
#include <stdlib.h>
#include <string.h>

// Engine
#include "constants.h"
#include "objectManager.h"
#include "render/render.h"
#include "log/log.h"

int updateDefault(Object *o) {
    // pass
    return 0;
}

int renderLightDefault(Object *o, Screen *s) {
    return 0;
}

int renderDefault(Object *o, Screen *s) {
    o->pix.depth = o->pos.z;
    // Position on the screen
    Point rel_pos = (Point){
            o->pos.x - s->camera_bounds.left,
            o->pos.y - s->camera_bounds.top
    };

    putPixelA(s, rel_pos.x, rel_pos.y, o->pix);

    return 0;
}

int listenEvent(struct Object *o, int ev_id, int (*listener)(struct Object *, Event)) {
    Node *n = getTreeNode(&o->event_listeners, ev_id);
    if (n == NULL) {
        insert(&o->event_listeners, listener, ev_id);
    } else {
        n->data = listener;
    }
    registerListener(o, ev_id);
    return 0;
}

void closeDefault(Object *o) {
    Iterator *it = initIterator(&o->event_listeners);
    while (!done(it)) {
        Node *n = getNext(it);
        unregisterListener(o, n->id);
    }
    closeIterator(it);
    closeTree(&o->event_listeners);

    if (o->data != NULL) {
        free(o->data);
    }
    free(o);
}

Object *createObject() {
    static int id_iterator = 1;

    Object *o = malloc(sizeof(Object));

    o->id = id_iterator++;
    strcpy(o->type, "");

    o->pos.x = 0;
    o->pos.y = 0;
    o->pos.z = 0;

    o->pix = PIXEL_NULL;
    o->pix.id = o->id;
    o->pix.depth = o->pos.z;
    o->solid = OBJ_ETHER;

    o->renderLight = &renderLightDefault;
    o->render = &renderDefault;
    o->update = &updateDefault;
    o->close = &closeDefault;

    initTree(&o->event_listeners);

    o->data = NULL;

    return o;
}

