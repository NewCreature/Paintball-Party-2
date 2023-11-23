#ifndef PP2_PLAYER_STRUCT_H
#define PP2_PLAYER_STRUCT_H

#include "t3f/t3f.h"
#include "t3f/input.h"
#include "player_defines.h"
#include "../../file/character.h"
#include "../camera.h"
#include "paintball_struct.h"
#include "particle_struct.h"
#include "../../file/profile.h"
#include "../../defines.h"

typedef struct
{

	PP2_CHARACTER * character;
	T3F_COLLISION_OBJECT * object[3];
	PP2_CAMERA camera;
	T3F_VIEW * view;
	int view_port;
	T3F_INPUT_HANDLER * input_handler;
	bool playing;
	int character_choice, character_choosing;
	unsigned long character_hash;
	int profile_choice;
	char name[256];
	int step;
	bool profile_read;
	PP2_PROFILE * profile;

	int id;
	int weapon, last_weapon;
	int ammo[PP2_PLAYER_MAX_WEAPONS];
	int hits, frags, life, coins;
	int target;
	bool coin_target;
	float coin_target_x, coin_target_y;
	bool choose_weapon;
	int want_weapon;

	int reload_time;
	int flash_time;
	bool jumped_down;
	int fade_time;
	int fade_type;
	int timer[PP2_PLAYER_MAX_TIMERS];

	float x, y, z;
	float vx, vy, ovx, fvx;
	int layer;
	int current_object;
	int state;
	unsigned long tick;
	int flags;

	/* stats */
	unsigned long shots;
	unsigned long total_hits;
	unsigned long shot;

	PP2_PAINTBALL paintball[PP2_MAX_PAINTBALLS];
	PP2_PARTICLE particle[PP2_MAX_PARTICLES];
	PP2_PAINTBALL_TRAIL trail[PP2_MAX_TRAILS];
	int current_trail;

} PP2_PLAYER;

#endif
