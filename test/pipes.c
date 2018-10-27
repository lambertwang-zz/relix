#include "game/game.h"
#include "object/object.h"
#include "object/objectManager.h"
#include "log/log.h"
#include "input/input.h"
#include "render/render.h"
#include "utility/random.h"

#include "stdlib.h"
#include "string.h"

#define NORTH 0
#define EAST  1
#define SOUTH 2
#define WEST  3

#define FADE 120.0

#define DEFAULT_TURN_PROB 0.6f

typedef struct tube {
    unsigned int altitude;
    unsigned int pos_x;
    unsigned int pos_y;
    char c[4];
} Tube;

typedef struct pipe {
    int lifetime;

    unsigned char dir;
    unsigned char prev_dir;

    Array history;

    float turn_prob;
    float aspect_ratio;
} Pipe;

void addPipe();

int globalEventListener(Event *ev) {
    KeyboardEvent k_ev = *(KeyboardEvent *)ev->data;

    switch (k_ev.type) {
        case KEYBOARD_NORMAL:
            switch (k_ev.value) {
                case 'q':
                case 'Q':
                case '\e':
                    closeGame();
                    break;
            }
    }
    
    return 0;
}

int renderPipe(Object *o, Screen *s) {
    Pipe *pipe_data = o->data;
    unsigned int i;
    
    int width  = s->camera_bounds.right - s->camera_bounds.left,
        height = s->camera_bounds.bottom - s->camera_bounds.top;

    pipe_data->aspect_ratio = (float) width / (float) height;
    if (pipe_data->lifetime <= 0) {
        o->pix.fg.a = 0.5 + ((float) pipe_data->lifetime) / (FADE * 2.0);
        pipe_data->lifetime--;
    } else {
        o->pix.fg.a = 0.5;
    }

    for (i = 0; i < pipe_data->history.count; i++) {
        Tube *tube = pipe_data->history.data[i];
        strcpy(o->pix.chr, tube->c);
        o->pix.depth = tube->altitude;
        int x = tube->pos_x - s->camera_bounds.left,
            y = tube->pos_y - s->camera_bounds.top;
        putPixelL(s, x, y, o->pix);
        if (pipe_data->lifetime > 0 &&
            (x < 0 || y < 0 || x >= width || y >= height)) {
            pipe_data->lifetime = -1;
        }
    }

    return 1;
}

unsigned char getTurn(unsigned char prev_dir) {
    if (nrandom_f() >= 0.5f) {
        return (prev_dir + 3) % 4;
    }
    return (prev_dir + 1) % 4;
}

int getPipe(char *dst, unsigned char dir1, unsigned char dir2) {
    dir2 = (dir2 + 2) % 4;
    if (dir1 > dir2) {
        dir2 += dir1;
        dir1 = dir2 - dir1;
        dir2 -= dir1;
    }
    
    if (dir1 == NORTH) {
        switch (dir2) {
            case EAST:
                strcpy(dst, "╚");
                return 200;
            case SOUTH:
                strcpy(dst, "║");
                return 186;
            case WEST:
                strcpy(dst, "╝");
                return 188;
        }
    }

    if (dir1 == EAST) {
        switch (dir2) {
            case SOUTH:
                strcpy(dst, "╔");
                return 201;
            case WEST:
                strcpy(dst, "═");
                return 205;
        }
    }

    if (dir1 == SOUTH) {
        switch (dir2) {
            case WEST:
                strcpy(dst, "╗");
                return 187;
        }
    }

    strcpy(dst, " ");
    return ' ';
}

int updatePipe(Object *o) {
    Pipe *pipe_data = o->data;

    if (pipe_data->lifetime == -1) {
        pipe_data->lifetime--;
        addPipe();
        return 0;
    }

    if (pipe_data->lifetime < -FADE) {
        removeObject(o);
        return 0;
    }

    if (pipe_data->lifetime < 0) {
        return 0;
    }

    switch (pipe_data->dir) {
       case NORTH:
           o->pos.y--;
           break;
       case EAST:
           o->pos.x++;
           break;
       case SOUTH:
           o->pos.y++;
           break;
       case WEST:
           o->pos.x--;
           break;
    }

    pipe_data->prev_dir = pipe_data->dir;
    float turn_prob = 1.0 / (pipe_data->aspect_ratio + 1.0);
    if (pipe_data->dir == WEST || pipe_data->dir == EAST) {
        turn_prob = 1 - turn_prob;
    }
    if (nrandom_f() > turn_prob) {
        pipe_data->dir = getTurn(pipe_data->prev_dir);
    }

    Tube *new_tube = malloc(sizeof(Tube));

    new_tube->pos_x = o->pos.x;
    new_tube->pos_y = o->pos.y;
    new_tube->altitude = pipe_data->lifetime++;
    getPipe(new_tube->c, pipe_data->dir, pipe_data->prev_dir);

    push(&pipe_data->history, new_tube);

    return 1;
}

void closePipe(Object *o) {
    closeArray(&((Pipe *) o->data)->history);
    closeDefault(o);
}

void addPipe() {
    struct Object *pipe = createObject();

    pipe->pix.fg = hslToRgb((Color){nrandom_i(0, 256), 192, 255, 1.0});
    pipe->render = &renderPipe;
    pipe->update = &updatePipe;
    pipe->close= &closePipe;

    Pipe *pipe_data = (Pipe *) malloc(sizeof(Pipe));
    pipe_data->lifetime = frame_count;
    pipe_data->dir = nrandom_i(0, 4);
    pipe_data->prev_dir = getTurn(pipe_data->dir);
    pipe_data->turn_prob = DEFAULT_TURN_PROB;
    pipe_data->aspect_ratio = 1.0;
    initArray(&pipe_data->history);

    pipe->data = pipe_data;

    addObject(pipe);
}

int main() {
    initGame();
    initRandom();

    registerGlobalListener(&globalEventListener);

    addPipe();
    addPipe();
    addPipe();

    loop();

    closeGame(0);
    return 0;
}
