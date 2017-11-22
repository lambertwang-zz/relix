#ifndef __WORLD_H__
#define __WORLD_H__

#include "../actions/action.h"

Action moveToTile(struct Object *obj, int x, int y);

int getDefaultAction(struct Object *obj, struct Object *target);

#endif

