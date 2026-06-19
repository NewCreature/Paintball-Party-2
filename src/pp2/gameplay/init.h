#ifndef PP2_GAMEPLAY_INIT_H
#define PP2_GAMEPLAY_INIT_H

#include "sprites/player.h"
#include "../resource.h"
#include "../pp2.h"
#include "game_struct.h"

int pp2_game_spawn_player(PP2_GAME * gp, PP2_PLAYER * pp, PP2_RESOURCES * resources);
bool pp2_game_load_data(PP2_GAME * gp, PP2_INTERFACE * ip, PP2_RESOURCES * resources);
bool pp2_game_setup(PP2_GAME * gp, int flags, PP2_INTERFACE * ip, PP2_RESOURCES * resources);
bool pp2_game_init(PP2_GAME * gp, int flags, PP2_INTERFACE * ip, PP2_RESOURCES * resources);
void pp2_game_free_data(PP2_GAME * gp, PP2_RESOURCES * resources);

#endif
