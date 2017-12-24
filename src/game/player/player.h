#ifndef __PLAYER_H__
#define __PLAYER_H__

// Engine
#include "object/object.h"

Object *addPlayer(Point start);

int renderPlayerLight(Object *o, Screen *s);

// Listeners
int playerListener(Object *o, Event *ev);

#endif

