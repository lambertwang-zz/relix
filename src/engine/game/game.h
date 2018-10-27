#ifndef __GAME_H__
#define __GAME_H__

unsigned int frame_count;

int initGame();
void closeGame();
void setClearScreenMode(int new_clear_screen_mode);
void useStatusLine();
int loop();

#endif

