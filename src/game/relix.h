#ifndef __RELIX_H___
#define __RELIX_H__

// Game log levels
#define LOG_USER    0x00ff0000
#define LOG_MAP     0x00010000
#define LOG_LOAD    0x00020000
#define LOG_LOAD_V  0x02000000
#define LOG_WORLD   0x00040000
#define LOG_WORLD_V 0x04000000

// Event ids
#define EVENT_MAP 8

#define EVENT_TICK_PLAYER 16
#define EVENT_TICK_OTHER 17

#define EVENT_OPEN_DOOR 24

#define EVENT_MESSAGE 32
#define EVENT_CHAR_INFO 33

// Assets
#define ASSET_DIR "assets/"

// Render depths
#define PLAYER_DEPTH 16
#define MONSTER_DEPTH 8
#define LOOT_DEPTH 4
#define WORLD_DEPTH 0

// Object types
#define TYPE_PLAYER "player"
#define TYPE_MONSTER "monster"

#endif

