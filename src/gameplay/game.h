#ifndef PP2_GAME_H
#define PP2_GAME_H

#include "sprites/player.h"

#define PP2_GAME_INIT_FLAG_REMATCH   1
#define PP2_GAME_INIT_FLAG_PLAYAGAIN 2
//#define PP2_GAME_INIT_FLAG_DEMO      4

/* place single player modes at end of list to make setup easier */
#define PP2_GAME_MODE_ELIMINATOR  0
#define PP2_GAME_MODE_DEATH_MATCH 1
#define PP2_GAME_MODE_COIN_RUSH   2
#define PP2_GAME_MODE_EXPLORE     7

int pp2_game_spawn_player(PP2_PLAYER * pp);
bool pp2_game_load_data(void);
bool pp2_game_setup(int flags);
bool pp2_game_init(int flags);
void pp2_game_free_data(void);
void pp2_game_render(void);
void pp2_game_over_logic(void);
void pp2_game_over_render(void);
void pp2_camera_logic(int i);
void pp2_game_logic(void);
void pp2_game_paused_logic(void);
void pp2_game_paused_render(void);

#endif
