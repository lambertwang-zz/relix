#include "object.h"
#include "objectManager.h"
#include "constants.h"
#include "render/render.h"
#include "term/screen.h"
#include "log/log.h"

#include "stdlib.h"
#include "string.h"

int update_default(const struct Object *o) {
    // pass
    return 0;
}

int render_default(const struct Object *o) {
    // Position on the screen
    Point rel_pos = (Point){
            o->pos.x - screen.camera_bounds.left,
            o->pos.y - screen.camera_bounds.top
    };

    if (rel_pos.x >= 0 && rel_pos.y >= 0 &&
        rel_pos.x < screen.ts.cols &&
        rel_pos.y < screen.ts.lines) {
        putPixel(rel_pos.x, rel_pos.y, o->pix);
        return 1;
    }

    return 0;
}

int listenEvent(struct Object *o, int ev_id, int (*listener)(struct Object *, Event)) {
    if (ev_id == o->events_size) {
        writeLog(LOG_OBJECTMANAGER, "object::listenEvent(): Expanding object event list size %d for event id %d", o->events_size, ev_id);
        int (**tmp_l)(struct Object *, Event) = malloc(sizeof(int (*)(struct Object *, Event)) * o->events_size * 2);
        memcpy(tmp_l, o->event_listeners, sizeof(int (*)(struct Object *, Event)) * o->events_size);
        free(o->event_listeners);
        o->event_listeners = tmp_l;
        int i;
        for (i = o->events_size; i < o->events_size * 2; i++) {
            o->event_listeners[i] = NULL;
        }
        o->events_size *= 2;
    }
    o->event_listeners[ev_id] = listener;
    registerListener(o, ev_id);
    return 0;
}

void close_default(struct Object *o) {
    int i;
    for (i = 0; i < o->events_size; i++) {
        if (o->event_listeners[i] != NULL) {
            unregisterListener(o, i);
        }
    }
    free(o->event_listeners);
    if (o->data != NULL) {
        free(o->data);
    }
    free(o);
}

void initObject(struct Object *o) {
    static int id_iterator = 1;
    o->id = id_iterator++;

    o->pos.x = 0;
    o->pos.y = 0;
    o->pos.z = 0;

    o->pix = (Pixel){0, 0, COLOR_EMPTY, COLOR_EMPTY, ' ', o->id, o->pos.z};

    o->render = &render_default;
    o->update = &update_default;
    o->close = &close_default;

    o->event_listeners = malloc(sizeof(int (*)(struct Object *)) * INIT_EVENT_COUNT);
    o->events_size = INIT_EVENT_COUNT;
    int i;
    for (i = 0; i < o->events_size; i++) {
        o->event_listeners[i] = NULL;
    }

    o->data = NULL;
}

