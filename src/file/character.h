#ifndef PP2_CHARACTER_H
#define PP2_CHARACTER_H

#include <allegro5/allegro_audio.h>
#include "../t3f/animation.h"

#define PP2_CHARACTER_MAX_ANIMATIONS          128
#define PP2_CHARACTER_MAX_PIECES                4

#define PP2_CHARACTER_MAX_SAMPLES              64

#define PP2_CHARACTER_MAX_STATES                 80

#define PP2_CHARACTER_STATE_STAND_R_R             0
#define PP2_CHARACTER_STATE_STAND_R_DR            1
#define PP2_CHARACTER_STATE_STAND_R_D             2
#define PP2_CHARACTER_STATE_STAND_R_DL            3
#define PP2_CHARACTER_STATE_STAND_R_L             4
#define PP2_CHARACTER_STATE_STAND_R_UL            5
#define PP2_CHARACTER_STATE_STAND_R_U             6
#define PP2_CHARACTER_STATE_STAND_R_UR            7
#define PP2_CHARACTER_STATE_STAND_L_R             8
#define PP2_CHARACTER_STATE_STAND_L_DR            9
#define PP2_CHARACTER_STATE_STAND_L_D            10
#define PP2_CHARACTER_STATE_STAND_L_DL           11
#define PP2_CHARACTER_STATE_STAND_L_L            12
#define PP2_CHARACTER_STATE_STAND_L_UL           13
#define PP2_CHARACTER_STATE_STAND_L_U            14
#define PP2_CHARACTER_STATE_STAND_L_UR           15
#define PP2_CHARACTER_STATE_WALK_R_R             16
#define PP2_CHARACTER_STATE_WALK_R_DR            17
#define PP2_CHARACTER_STATE_WALK_R_D             18
#define PP2_CHARACTER_STATE_WALK_R_DL            19
#define PP2_CHARACTER_STATE_WALK_R_L             20
#define PP2_CHARACTER_STATE_WALK_R_UL            21
#define PP2_CHARACTER_STATE_WALK_R_U             22
#define PP2_CHARACTER_STATE_WALK_R_UR            23
#define PP2_CHARACTER_STATE_WALK_L_R             24
#define PP2_CHARACTER_STATE_WALK_L_DR            25
#define PP2_CHARACTER_STATE_WALK_L_D             26
#define PP2_CHARACTER_STATE_WALK_L_DL            27
#define PP2_CHARACTER_STATE_WALK_L_L             28
#define PP2_CHARACTER_STATE_WALK_L_UL            29
#define PP2_CHARACTER_STATE_WALK_L_U             30
#define PP2_CHARACTER_STATE_WALK_L_UR            31
#define PP2_CHARACTER_STATE_JUMP_R_R             32
#define PP2_CHARACTER_STATE_JUMP_R_DR            33
#define PP2_CHARACTER_STATE_JUMP_R_D             34
#define PP2_CHARACTER_STATE_JUMP_R_DL            35
#define PP2_CHARACTER_STATE_JUMP_R_L             36
#define PP2_CHARACTER_STATE_JUMP_R_UL            37
#define PP2_CHARACTER_STATE_JUMP_R_U             38
#define PP2_CHARACTER_STATE_JUMP_R_UR            39
#define PP2_CHARACTER_STATE_JUMP_L_R             40
#define PP2_CHARACTER_STATE_JUMP_L_DR            41
#define PP2_CHARACTER_STATE_JUMP_L_D             42
#define PP2_CHARACTER_STATE_JUMP_L_DL            43
#define PP2_CHARACTER_STATE_JUMP_L_L             44
#define PP2_CHARACTER_STATE_JUMP_L_UL            45
#define PP2_CHARACTER_STATE_JUMP_L_U             46
#define PP2_CHARACTER_STATE_JUMP_L_UR            47
#define PP2_CHARACTER_STATE_FALL_R_R             48
#define PP2_CHARACTER_STATE_FALL_R_DR            49
#define PP2_CHARACTER_STATE_FALL_R_D             50
#define PP2_CHARACTER_STATE_FALL_R_DL            51
#define PP2_CHARACTER_STATE_FALL_R_L             52
#define PP2_CHARACTER_STATE_FALL_R_UL            53
#define PP2_CHARACTER_STATE_FALL_R_U             54
#define PP2_CHARACTER_STATE_FALL_R_UR            55
#define PP2_CHARACTER_STATE_FALL_L_R             56
#define PP2_CHARACTER_STATE_FALL_L_DR            57
#define PP2_CHARACTER_STATE_FALL_L_D             58
#define PP2_CHARACTER_STATE_FALL_L_DL            59
#define PP2_CHARACTER_STATE_FALL_L_L             60
#define PP2_CHARACTER_STATE_FALL_L_UL            61
#define PP2_CHARACTER_STATE_FALL_L_U             62
#define PP2_CHARACTER_STATE_FALL_L_UR            63
#define PP2_CHARACTER_STATE_DUCK_R_R             64
#define PP2_CHARACTER_STATE_DUCK_R_DR            65
#define PP2_CHARACTER_STATE_DUCK_R_D             66
#define PP2_CHARACTER_STATE_DUCK_R_DL            67
#define PP2_CHARACTER_STATE_DUCK_R_L             68
#define PP2_CHARACTER_STATE_DUCK_R_UL            69
#define PP2_CHARACTER_STATE_DUCK_R_U             70
#define PP2_CHARACTER_STATE_DUCK_R_UR            71
#define PP2_CHARACTER_STATE_DUCK_L_R             72
#define PP2_CHARACTER_STATE_DUCK_L_DR            73
#define PP2_CHARACTER_STATE_DUCK_L_D             74
#define PP2_CHARACTER_STATE_DUCK_L_DL            75
#define PP2_CHARACTER_STATE_DUCK_L_L             76
#define PP2_CHARACTER_STATE_DUCK_L_UL            77
#define PP2_CHARACTER_STATE_DUCK_L_U             78
#define PP2_CHARACTER_STATE_DUCK_L_UR            79

#define PP2_CHARACTER_FLAG_LEGACY                 1

/*#define PP2_CHARACTER_STATE_STAND_RIGHT           0
#define PP2_CHARACTER_STATE_STAND_LEFT            1
#define PP2_CHARACTER_STATE_STAND_UP_RIGHT        2
#define PP2_CHARACTER_STATE_STAND_UP_LEFT         3
#define PP2_CHARACTER_STATE_WALK_RIGHT            4
#define PP2_CHARACTER_STATE_WALK_LEFT             5
#define PP2_CHARACTER_STATE_DUCK_RIGHT            6
#define PP2_CHARACTER_STATE_DUCK_LEFT             7
#define PP2_CHARACTER_STATE_JUMP_RIGHT            8
#define PP2_CHARACTER_STATE_JUMP_LEFT             9
#define PP2_CHARACTER_STATE_FALL_RIGHT           10
#define PP2_CHARACTER_STATE_FALL_LEFT            11 */

typedef struct
{
	
	int animation;
	float x, y, cx, cy, angle;
	int flags;
	
} PP2_CHARACTER_PIECE;

typedef struct
{
	
	PP2_CHARACTER_PIECE piece[PP2_CHARACTER_MAX_PIECES];
	int pieces;
	PP2_CHARACTER_PIECE fire_piece[PP2_CHARACTER_MAX_PIECES];
	int fire_pieces;
//	PP2_CHARACTER_PIECE flash;
	PP2_CHARACTER_PIECE paintball;
	PP2_CHARACTER_PIECE particle;
	
	int collision_x;
	int collision_y;
	int collision_w;
	int collision_h;
	
	int flags;
	
} PP2_CHARACTER_STATE;

typedef struct
{
	
	char name[128];
	char author[128];
	char comment[256];
	
} PP2_CHARACTER_META_DATA;

typedef struct
{
	
	PP2_CHARACTER_META_DATA info;
	
	T3F_ANIMATION * animation[PP2_CHARACTER_MAX_ANIMATIONS];
	int animations;
	T3F_ATLAS * atlas;
	
	ALLEGRO_SAMPLE * sample[PP2_CHARACTER_MAX_SAMPLES];
	
	PP2_CHARACTER_STATE state[PP2_CHARACTER_MAX_STATES];
	
	int paintball_size;
	int flags;
	
} PP2_CHARACTER;

PP2_CHARACTER * pp2_create_character(void);
void pp2_destroy_character(PP2_CHARACTER * cp);

PP2_CHARACTER * pp2_load_character(const char * fn, int flags);
int pp2_save_character(PP2_CHARACTER * cp, const char * fn);

#endif
