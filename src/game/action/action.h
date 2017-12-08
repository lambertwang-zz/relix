#ifndef __ACTION_H__
#define __ACTION_H__

#define ACTION_IMPASSIBLE 0
#define ACTION_MOVE 1
#define ACTION_INTERACT 2
#define ACTION_DOOR 3

#define ACTION_LIGHT 10

typedef struct Action {
    int code;
    struct Point target;
    struct Object *other;
} Action;

#endif

