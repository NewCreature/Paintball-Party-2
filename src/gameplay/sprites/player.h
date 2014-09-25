#ifndef PP2_PLAYER_H
#define PP2_PLAYER_H

#include "t3f/t3f.h"
#include "t3f/tilemap.h"
#include "t3f/collision.h"
#include "t3f/view.h"
#include "../../file/character.h"
#include "../../defines.h"
#include "../camera.h"
#include "paintball.h"
#include "particle.h"

#define PP2_PLAYER_MAX_WEAPONS 16
#define PP2_PLAYER_MAX_TIMERS  16

#define PP2_PLAYER_FLAG_ACTIVE         1
#define PP2_PLAYER_FLAG_GROUND         2
#define PP2_PLAYER_FLAG_POWER_CLOAK    4
#define PP2_PLAYER_FLAG_POWER_JUMP     8
#define PP2_PLAYER_FLAG_POWER_RUN     16
#define PP2_PLAYER_FLAG_POWER_DEFLECT 32
#define PP2_PLAYER_FLAG_POWER_FLY     64
#define PP2_PLAYER_FLAG_POWER_TURBO  128
#define PP2_PLAYER_FLAG_TYPING       256

#define PP2_PLAYER_TIMER_CLOAK         0
#define PP2_PLAYER_TIMER_JUMP          1
#define PP2_PLAYER_TIMER_RUN           2
#define PP2_PLAYER_TIMER_DEFLECT       3
#define PP2_PLAYER_TIMER_FLY           4
#define PP2_PLAYER_TIMER_TURBO         5
#define PP2_PLAYER_TIMER_WEAPON_SELECT 6
#define PP2_PLAYER_TIMER_STOMPED       7

#define PP2_PLAYER_STEP_SELECT_PROFILE     0 // selecting profile
#define PP2_PLAYER_STEP_SELECTED_PROFILE   1 // wait for network to go to step 2
#define PP2_PLAYER_STEP_SELECT_CHARACTER   2 // player is selecting
#define PP2_PLAYER_STEP_CHARACTER_WAIT     3 // if waiting, render preview semi-transparent
#define PP2_PLAYER_STEP_CHARACTER_FOUND    4 // character was found so render only darkly
#define PP2_PLAYER_STEP_SELECTED_CHARACTER 5 // character was selected
#define PP2_PLAYER_STEP_DONE               6 // player is done

typedef struct
{
	
	PP2_CHARACTER * character;
	T3F_COLLISION_OBJECT * object[3];
	PP2_CAMERA camera;
	T3F_VIEW * view;
	int controller;
	bool playing;
	int character_choice, character_choosing;
	unsigned long character_hash;
	int profile_choice;
	char name[256];
	int step;
	bool profile_read;
	
	int id;
	int weapon, last_weapon;
	int ammo[PP2_PLAYER_MAX_WEAPONS];
	int hits, frags, life, coins;
	int target;
	
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

void pp2_player_next_weapon(PP2_PLAYER * pp);
void pp2_player_move_object_x(PP2_PLAYER * pp);
void pp2_player_move_object_y(PP2_PLAYER * pp);
void pp2_player_receive_hit(PP2_PLAYER * pp, int dealer);
void pp2_player_logic(PP2_PLAYER * pp);
void pp2_player_render(PP2_PLAYER * pp, PP2_CAMERA * cp);

#endif
