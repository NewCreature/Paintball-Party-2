#ifndef PP2_GAME_STRUCT_H
#define PP2_GAME_STRUCT_H

#include "../file/level.h"
#include "sprites/player_struct.h"
#include "sprites/object_struct.h"
#include "sprites/radar_struct.h"

typedef struct
{

  /* gameplay resources */
  PP2_LEVEL * level;
  ALLEGRO_BITMAP * radar_bitmap[PP2_LEVEL_MAX_LAYERS];
  ALLEGRO_FILE * replay_file;
  int replay_player;
  bool replay_rewind;

  /* gameplay objects */
  PP2_PLAYER player[PP2_MAX_PLAYERS];
  PP2_OBJECT * object;
  int objects;
  int object_size;
  PP2_RADAR_OBJECT radar_object[PP2_MAX_RADAR_OBJECTS];
  int radar_objects;
  int radar_offset_x;
  int radar_offset_y;
  PP2_PARTICLE particle[PP2_MAX_PARTICLES];
  int current_particle;

  /* gameplay variables */
  int option[PP2_MAX_OPTIONS];
  int winner;
  int award_accuracy;
  int award_fodder;
  int award_trigger;
  int time_left;
  int coins_needed;
  unsigned int seed;
  int local_player; // if only one player is local, set this to that player
  unsigned long tick;
  int next_state;

} PP2_GAME;

#endif
