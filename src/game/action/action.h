#ifndef __ACTION_H__
#define __ACTION_H__

#define ACTION_NONE 0
#define ACTION_IMPASSIBLE 1
#define ACTION_MOVE 2
#define ACTION_INTERACT 3
#define ACTION_DOOR 4
#define ACTION_ATTACK 5

#define ACTION_LIGHT 10

typedef struct Action {
    int code;
    struct Point target;
    struct Object *other;
} Action;

#endif

