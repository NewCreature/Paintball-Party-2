#ifndef PP2_GAME_STRUCT_H
#define PP2_GAME_STRUCT_H

#include "../file/level.h"
#include "sprites/player_struct.h"

typedef struct
{

  PP2_LEVEL * level;
  PP2_PLAYER player[PP2_MAX_PLAYERS];

} PP2_GAME;

#endif
