#ifndef __PLAYER_H__
#define __PLAYER_H__

// Engine
#include "object/object.h"

#define TYPE_PLAYER "player"

Object *addPlayer(Point start);

int renderPlayerLight(Object *o, Screen *s);

#endif

