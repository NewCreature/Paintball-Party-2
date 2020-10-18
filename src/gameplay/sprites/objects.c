#include "../../t3f/sound.h"
#include "../../data.h"
#include "../../misc/fixed_point.h"
#include "../../misc/sound.h"
#include "../camera.h"
#include "object_defines.h"
#include "../../resource.h"
#include "paintball_defines.h"

static bool pp2_object_on_floor(PP2_OBJECT * op)
{
	int i;

	for(i = 0; i < op->object->map.bottom.points; i++)
	{
		if(t3f_get_collision_tilemap_flag(pp2_level->collision_tilemap[op->layer], op->x + op->object->map.bottom.point[i].x, op->y + op->object->map.bottom.point[i].y + 1.0, T3F_COLLISION_FLAG_SOLID_TOP))
		{
			return true;
		}
	}
	return false;
}

static bool pp2_object_on_ice(PP2_OBJECT * op)
{
	int i;
	int cf;
	int slip_center;

	slip_center = t3f_get_collision_tilemap_flag(pp2_level->collision_tilemap[op->layer], op->x + op->object->map.bottom.point[0].x, op->y + op->object->map.bottom.point[0].y + 1.0, T3F_COLLISION_FLAG_SOLID_TOP | PP2_LEVEL_COLLISION_FLAG_ICE);
	if(slip_center == (T3F_COLLISION_FLAG_SOLID_TOP | PP2_LEVEL_COLLISION_FLAG_ICE))
	{
		return true;
	}
	else
	{
		for(i = 1; i < op->object->map.bottom.points; i++)
		{
			cf = t3f_get_collision_tilemap_flag(pp2_level->collision_tilemap[op->layer], op->x + op->object->map.bottom.point[i].x, op->y + op->object->map.bottom.point[i].y + 1.0, T3F_COLLISION_FLAG_SOLID_TOP | PP2_LEVEL_COLLISION_FLAG_ICE);
			if((cf & T3F_COLLISION_FLAG_SOLID_TOP) && !(cf & PP2_LEVEL_COLLISION_FLAG_ICE))
			{
				return false;
			}
		}
		return true;
	}
	return false;
}

/* return conveyor speed if player is on a conveyor, else return 0
   conveyor speed is returned in fixed point notation, convert before using */
static int pp2_object_convey(PP2_OBJECT * op)
{
	int i;
	int cf;

	for(i = 0; i < op->object->map.bottom.points; i++)
	{
		cf = t3f_get_collision_tilemap_flag(pp2_level->collision_tilemap[op->layer], op->x + op->object->map.bottom.point[i].x, op->y + op->object->map.bottom.point[i].y + 1.0, T3F_COLLISION_FLAG_SOLID_TOP | PP2_LEVEL_COLLISION_FLAG_CONVEYOR);
		if(cf == (T3F_COLLISION_FLAG_SOLID_TOP | PP2_LEVEL_COLLISION_FLAG_CONVEYOR))
		{
			return t3f_get_collision_tilemap_data(pp2_level->collision_tilemap[op->layer], op->x + op->object->map.bottom.point[i].x, op->y + op->object->map.bottom.point[i].y + 1.0, 0);
		}
	}
	return 0;
}

int pp2_generate_object(float x, float y, int layer, int type, int flags, int extra)
{
	int i;

	for(i = 0; i < pp2_object_size; i++)
	{
		if(!(pp2_object[i].flags & PP2_OBJECT_FLAG_ACTIVE))
		{
			pp2_object[i].x = x;
			pp2_object[i].y = y;
			pp2_object[i].z = pp2_level->tilemap->layer[layer]->z;
			pp2_object[i].layer = layer;
			pp2_object[i].type = type;
			pp2_object[i].tick = 0;
			pp2_object[i].flags = PP2_OBJECT_FLAG_ACTIVE;
			pp2_object[i].extra = extra;
			switch(type)
			{
				default:
				{
					t3f_recreate_collision_object(pp2_object[i].object, 0, 0, 32, 32, 32, 32, 0);
					break;
				}
			}
			return i;
		}
	}
	return -1;
}

void pp2_object_logic(PP2_GAME * gp, PP2_OBJECT * op)
{
	int i, o;
	bool wswitch = false;

	if(op->flags & PP2_OBJECT_FLAG_ACTIVE)
	{
		switch(op->type)
		{
			case PP2_OBJECT_GENERATOR:
			{
				op->counter--;
				if(op->counter <= 0)
				{
					pp2_generate_object(op->x, op->y, op->layer, op->extra, op->flags, 0);
					op->flags = 0;
				}
				break;
			}
			case PP2_OBJECT_AMMO_NORMAL:
			{
				t3f_move_collision_object_x(op->object, op->x);
				t3f_move_collision_object_y(op->object, op->y);
				for(i = 0; i < PP2_MAX_PLAYERS; i++)
				{
					if(gp->player[i].flags & PP2_PLAYER_FLAG_ACTIVE)
					{
						if(t3f_check_object_collision(op->object, gp->player[i].object[gp->player[i].current_object]))
						{
							if(gp->player[i].ammo[gp->player[i].weapon] <= 0)
							{
								wswitch = true;
								gp->player[i].reload_time = 35;
								gp->player[i].timer[PP2_PLAYER_TIMER_WEAPON_SELECT] = 32;
							}
							gp->player[i].ammo[PP2_PAINTBALL_TYPE_NORMAL] += pp2_option[PP2_OPTION_AMMO_WORTH];
							if(gp->player[i].ammo[PP2_PAINTBALL_TYPE_NORMAL] > 99)
							{
								gp->player[i].ammo[PP2_PAINTBALL_TYPE_NORMAL] = 99;
							}
							if(wswitch)
							{
								pp2_player_next_weapon(&gp->player[i]);
								gp->player[i].last_weapon = -1;
							}
							pp2_play_sample(gp, gp->player[i].character->sample[PP2_SAMPLE_AMMO], gp->player[i].x + gp->player[i].object[0]->map.top.point[0].x, gp->player[i].y + gp->player[i].object[0]->map.left.point[0].y, 1.0, 1.0);
							o = pp2_generate_object(op->x, op->y, op->layer, PP2_OBJECT_GENERATOR, 0, op->type);
							if(o >= 0)
							{
								pp2_object[o].counter = 1800;
							}
							op->flags = 0;
						}
					}
				}
				break;
			}
			case PP2_OBJECT_AMMO_X:
			{
				t3f_move_collision_object_x(op->object, op->x);
				t3f_move_collision_object_y(op->object, op->y);
				for(i = 0; i < PP2_MAX_PLAYERS; i++)
				{
					if(gp->player[i].flags & PP2_PLAYER_FLAG_ACTIVE)
					{
						if(t3f_check_object_collision(op->object, gp->player[i].object[gp->player[i].current_object]))
						{
							if(gp->player[i].ammo[gp->player[i].weapon] <= 0)
							{
								wswitch = true;
								gp->player[i].reload_time = 35;
								gp->player[i].timer[PP2_PLAYER_TIMER_WEAPON_SELECT] = 32;
							}
							gp->player[i].ammo[PP2_PAINTBALL_TYPE_SPLITTER] += pp2_option[PP2_OPTION_AMMO_WORTH];
							if(gp->player[i].ammo[PP2_PAINTBALL_TYPE_SPLITTER] > 99)
							{
								gp->player[i].ammo[PP2_PAINTBALL_TYPE_SPLITTER] = 99;
							}
							if(wswitch)
							{
								pp2_player_next_weapon(&gp->player[i]);
								gp->player[i].last_weapon = -1;
							}
							pp2_play_sample(gp, gp->player[i].character->sample[PP2_SAMPLE_AMMO], gp->player[i].x + gp->player[i].object[0]->map.top.point[0].x, gp->player[i].y + gp->player[i].object[0]->map.left.point[0].y, 1.0, 1.0);
							o = pp2_generate_object(op->x, op->y, op->layer, PP2_OBJECT_GENERATOR, 0, op->type);
							if(o >= 0)
							{
								pp2_object[o].counter = 1800;
							}
							op->flags = 0;
						}
					}
				}
				break;
			}
			case PP2_OBJECT_AMMO_MINE:
			{
				t3f_move_collision_object_x(op->object, op->x);
				t3f_move_collision_object_y(op->object, op->y);
				for(i = 0; i < PP2_MAX_PLAYERS; i++)
				{
					if(gp->player[i].flags & PP2_PLAYER_FLAG_ACTIVE)
					{
						if(t3f_check_object_collision(op->object, gp->player[i].object[gp->player[i].current_object]))
						{
							if(gp->player[i].ammo[gp->player[i].weapon] <= 0)
							{
								wswitch = true;
								gp->player[i].reload_time = 35;
								gp->player[i].timer[PP2_PLAYER_TIMER_WEAPON_SELECT] = 32;
							}
							gp->player[i].ammo[PP2_PAINTBALL_TYPE_MINE] += pp2_option[PP2_OPTION_AMMO_WORTH];
							if(gp->player[i].ammo[PP2_PAINTBALL_TYPE_MINE] > 99)
							{
								gp->player[i].ammo[PP2_PAINTBALL_TYPE_MINE] = 99;
							}
							if(wswitch)
							{
								pp2_player_next_weapon(&gp->player[i]);
								gp->player[i].last_weapon = -1;
							}
							pp2_play_sample(gp, gp->player[i].character->sample[PP2_SAMPLE_AMMO], gp->player[i].x + gp->player[i].object[0]->map.top.point[0].x, gp->player[i].y + gp->player[i].object[0]->map.left.point[0].y, 1.0, 1.0);
							o = pp2_generate_object(op->x, op->y, op->layer, PP2_OBJECT_GENERATOR, 0, op->type);
							if(o >= 0)
							{
								pp2_object[o].counter = 1800;
							}
							op->flags = 0;
						}
					}
				}
				break;
			}
			case PP2_OBJECT_AMMO_BOUNCE:
			{
				t3f_move_collision_object_x(op->object, op->x);
				t3f_move_collision_object_y(op->object, op->y);
				for(i = 0; i < PP2_MAX_PLAYERS; i++)
				{
					if(gp->player[i].flags & PP2_PLAYER_FLAG_ACTIVE)
					{
						if(t3f_check_object_collision(op->object, gp->player[i].object[gp->player[i].current_object]))
						{
							if(gp->player[i].ammo[gp->player[i].weapon] <= 0)
							{
								wswitch = true;
								gp->player[i].reload_time = 35;
								gp->player[i].timer[PP2_PLAYER_TIMER_WEAPON_SELECT] = 32;
							}
							gp->player[i].ammo[PP2_PAINTBALL_TYPE_BOUNCER] += pp2_option[PP2_OPTION_AMMO_WORTH];
							if(gp->player[i].ammo[PP2_PAINTBALL_TYPE_BOUNCER] > 99)
							{
								gp->player[i].ammo[PP2_PAINTBALL_TYPE_BOUNCER] = 99;
							}
							if(wswitch)
							{
								pp2_player_next_weapon(&gp->player[i]);
								gp->player[i].last_weapon = -1;
							}
							pp2_play_sample(gp, gp->player[i].character->sample[PP2_SAMPLE_AMMO], gp->player[i].x + gp->player[i].object[0]->map.top.point[0].x, gp->player[i].y + gp->player[i].object[0]->map.left.point[0].y, 1.0, 1.0);
							o = pp2_generate_object(op->x, op->y, op->layer, PP2_OBJECT_GENERATOR, 0, op->type);
							if(o >= 0)
							{
								pp2_object[o].counter = 1800;
							}
							op->flags = 0;
						}
					}
				}
				break;
			}
			case PP2_OBJECT_AMMO_SEEK:
			{
				t3f_move_collision_object_x(op->object, op->x);
				t3f_move_collision_object_y(op->object, op->y);
				for(i = 0; i < PP2_MAX_PLAYERS; i++)
				{
					if(gp->player[i].flags & PP2_PLAYER_FLAG_ACTIVE)
					{
						if(t3f_check_object_collision(op->object, gp->player[i].object[gp->player[i].current_object]))
						{
							if(gp->player[i].ammo[gp->player[i].weapon] <= 0)
							{
								wswitch = true;
								gp->player[i].reload_time = 35;
								gp->player[i].timer[PP2_PLAYER_TIMER_WEAPON_SELECT] = 32;
							}
							gp->player[i].ammo[PP2_PAINTBALL_TYPE_SEEKER] += pp2_option[PP2_OPTION_AMMO_WORTH];
							if(gp->player[i].ammo[PP2_PAINTBALL_TYPE_SEEKER] > 99)
							{
								gp->player[i].ammo[PP2_PAINTBALL_TYPE_SEEKER] = 99;
							}
							if(wswitch)
							{
								pp2_player_next_weapon(&gp->player[i]);
								gp->player[i].last_weapon = -1;
							}
							pp2_play_sample(gp, gp->player[i].character->sample[PP2_SAMPLE_AMMO], gp->player[i].x + gp->player[i].object[0]->map.top.point[0].x, gp->player[i].y + gp->player[i].object[0]->map.left.point[0].y, 1.0, 1.0);
							o = pp2_generate_object(op->x, op->y, op->layer, PP2_OBJECT_GENERATOR, 0, op->type);
							if(o >= 0)
							{
								pp2_object[o].counter = 1800;
							}
							op->flags = 0;
						}
					}
				}
				break;
			}
			case PP2_OBJECT_AMMO_REFLECTOR:
			{
				t3f_move_collision_object_x(op->object, op->x);
				t3f_move_collision_object_y(op->object, op->y);
				for(i = 0; i < PP2_MAX_PLAYERS; i++)
				{
					if(gp->player[i].flags & PP2_PLAYER_FLAG_ACTIVE)
					{
						if(t3f_check_object_collision(op->object, gp->player[i].object[gp->player[i].current_object]))
						{
							if(gp->player[i].ammo[gp->player[i].weapon] <= 0)
							{
								wswitch = true;
								gp->player[i].reload_time = 35;
								gp->player[i].timer[PP2_PLAYER_TIMER_WEAPON_SELECT] = 32;
							}
							gp->player[i].ammo[PP2_PAINTBALL_TYPE_REFLECTOR] += pp2_option[PP2_OPTION_AMMO_WORTH];
							if(gp->player[i].ammo[PP2_PAINTBALL_TYPE_REFLECTOR] > 99)
							{
								gp->player[i].ammo[PP2_PAINTBALL_TYPE_REFLECTOR] = 99;
							}
							if(wswitch)
							{
								pp2_player_next_weapon(&gp->player[i]);
								gp->player[i].last_weapon = -1;
							}
							pp2_play_sample(gp, gp->player[i].character->sample[PP2_SAMPLE_AMMO], gp->player[i].x + gp->player[i].object[0]->map.top.point[0].x, gp->player[i].y + gp->player[i].object[0]->map.left.point[0].y, 1.0, 1.0);
							o = pp2_generate_object(op->x, op->y, op->layer, PP2_OBJECT_GENERATOR, 0, op->type);
							if(o >= 0)
							{
								pp2_object[o].counter = 1800;
							}
							op->flags = 0;
						}
					}
				}
				break;
			}
			case PP2_OBJECT_AMMO_PMINE:
			{
				t3f_move_collision_object_x(op->object, op->x);
				t3f_move_collision_object_y(op->object, op->y);
				for(i = 0; i < PP2_MAX_PLAYERS; i++)
				{
					if(gp->player[i].flags & PP2_PLAYER_FLAG_ACTIVE)
					{
						if(t3f_check_object_collision(op->object, gp->player[i].object[gp->player[i].current_object]))
						{
							if(gp->player[i].ammo[gp->player[i].weapon] <= 0)
							{
								wswitch = true;
								gp->player[i].reload_time = 35;
								gp->player[i].timer[PP2_PLAYER_TIMER_WEAPON_SELECT] = 32;
							}
							gp->player[i].ammo[PP2_PAINTBALL_TYPE_PMINE] += pp2_option[PP2_OPTION_AMMO_WORTH];
							if(gp->player[i].ammo[PP2_PAINTBALL_TYPE_PMINE] > 99)
							{
								gp->player[i].ammo[PP2_PAINTBALL_TYPE_PMINE] = 99;
							}
							if(wswitch)
							{
								pp2_player_next_weapon(&gp->player[i]);
								gp->player[i].last_weapon = -1;
							}
							pp2_play_sample(gp, gp->player[i].character->sample[PP2_SAMPLE_AMMO], gp->player[i].x + gp->player[i].object[0]->map.top.point[0].x, gp->player[i].y + gp->player[i].object[0]->map.left.point[0].y, 1.0, 1.0);
							o = pp2_generate_object(op->x, op->y, op->layer, PP2_OBJECT_GENERATOR, 0, op->type);
							if(o >= 0)
							{
								pp2_object[o].counter = 1800;
							}
							op->flags = 0;
						}
					}
				}
				break;
			}
			case PP2_OBJECT_AMMO_GHOST:
			{
				t3f_move_collision_object_x(op->object, op->x);
				t3f_move_collision_object_y(op->object, op->y);
				for(i = 0; i < PP2_MAX_PLAYERS; i++)
				{
					if(gp->player[i].flags & PP2_PLAYER_FLAG_ACTIVE)
					{
						if(t3f_check_object_collision(op->object, gp->player[i].object[gp->player[i].current_object]))
						{
							if(gp->player[i].ammo[gp->player[i].weapon] <= 0)
							{
								wswitch = true;
								gp->player[i].reload_time = 35;
								gp->player[i].timer[PP2_PLAYER_TIMER_WEAPON_SELECT] = 32;
							}
							gp->player[i].ammo[PP2_PAINTBALL_TYPE_GHOST] += pp2_option[PP2_OPTION_AMMO_WORTH];
							if(gp->player[i].ammo[PP2_PAINTBALL_TYPE_GHOST] > 99)
							{
								gp->player[i].ammo[PP2_PAINTBALL_TYPE_GHOST] = 99;
							}
							if(wswitch)
							{
								pp2_player_next_weapon(&gp->player[i]);
								gp->player[i].last_weapon = -1;
							}
							pp2_play_sample(gp, gp->player[i].character->sample[PP2_SAMPLE_AMMO], gp->player[i].x + gp->player[i].object[0]->map.top.point[0].x, gp->player[i].y + gp->player[i].object[0]->map.left.point[0].y, 1.0, 1.0);
							o = pp2_generate_object(op->x, op->y, op->layer, PP2_OBJECT_GENERATOR, 0, op->type);
							if(o >= 0)
							{
								pp2_object[o].counter = 1800;
							}
							op->flags = 0;
						}
					}
				}
				break;
			}
			case PP2_OBJECT_POWER_CLOAK:
			{
				t3f_move_collision_object_x(op->object, op->x);
				t3f_move_collision_object_y(op->object, op->y);
				for(i = 0; i < PP2_MAX_PLAYERS; i++)
				{
					if(gp->player[i].flags & PP2_PLAYER_FLAG_ACTIVE)
					{
						if(t3f_check_object_collision(op->object, gp->player[i].object[gp->player[i].current_object]))
						{
							gp->player[i].timer[PP2_PLAYER_TIMER_CLOAK] = 600;
							gp->player[i].flags |= PP2_PLAYER_FLAG_POWER_CLOAK;
							pp2_play_sample(gp, gp->player[i].character->sample[PP2_SAMPLE_CLOAK], gp->player[i].x + gp->player[i].object[0]->map.top.point[0].x, gp->player[i].y + gp->player[i].object[0]->map.left.point[0].y, 1.0, 1.0);
							o = pp2_generate_object(op->x, op->y, op->layer, PP2_OBJECT_GENERATOR, 0, op->type);
							if(o >= 0)
							{
								pp2_object[o].counter = 1800;
							}
							op->flags = 0;
						}
					}
				}
				break;
			}
			case PP2_OBJECT_POWER_JUMP:
			{
				t3f_move_collision_object_x(op->object, op->x);
				t3f_move_collision_object_y(op->object, op->y);
				for(i = 0; i < PP2_MAX_PLAYERS; i++)
				{
					if(gp->player[i].flags & PP2_PLAYER_FLAG_ACTIVE)
					{
						if(t3f_check_object_collision(op->object, gp->player[i].object[gp->player[i].current_object]))
						{
							gp->player[i].timer[PP2_PLAYER_TIMER_JUMP] = 600;
							gp->player[i].flags |= PP2_PLAYER_FLAG_POWER_JUMP;
							pp2_play_sample(gp, gp->player[i].character->sample[PP2_SAMPLE_PJUMP], gp->player[i].x + gp->player[i].object[0]->map.top.point[0].x, gp->player[i].y + gp->player[i].object[0]->map.left.point[0].y, 1.0, 1.0);
							o = pp2_generate_object(op->x, op->y, op->layer, PP2_OBJECT_GENERATOR, 0, op->type);
							if(o >= 0)
							{
								pp2_object[o].counter = 1800;
							}
							op->flags = 0;
						}
					}
				}
				break;
			}
			case PP2_OBJECT_POWER_RUN:
			{
				t3f_move_collision_object_x(op->object, op->x);
				t3f_move_collision_object_y(op->object, op->y);
				for(i = 0; i < PP2_MAX_PLAYERS; i++)
				{
					if(gp->player[i].flags & PP2_PLAYER_FLAG_ACTIVE)
					{
						if(t3f_check_object_collision(op->object, gp->player[i].object[gp->player[i].current_object]))
						{
							gp->player[i].timer[PP2_PLAYER_TIMER_RUN] = 600;
							gp->player[i].flags |= PP2_PLAYER_FLAG_POWER_RUN;
							pp2_play_sample(gp, gp->player[i].character->sample[PP2_SAMPLE_RUN], gp->player[i].x + gp->player[i].object[0]->map.top.point[0].x, gp->player[i].y + gp->player[i].object[0]->map.left.point[0].y, 1.0, 1.0);
							o = pp2_generate_object(op->x, op->y, op->layer, PP2_OBJECT_GENERATOR, 0, op->type);
							if(o >= 0)
							{
								pp2_object[o].counter = 1800;
							}
							op->flags = 0;
						}
					}
				}
				break;
			}
			case PP2_OBJECT_POWER_DEFLECT:
			{
				t3f_move_collision_object_x(op->object, op->x);
				t3f_move_collision_object_y(op->object, op->y);
				for(i = 0; i < PP2_MAX_PLAYERS; i++)
				{
					if(gp->player[i].flags & PP2_PLAYER_FLAG_ACTIVE)
					{
						if(t3f_check_object_collision(op->object, gp->player[i].object[gp->player[i].current_object]))
						{
							gp->player[i].timer[PP2_PLAYER_TIMER_DEFLECT] = 600;
							gp->player[i].flags |= PP2_PLAYER_FLAG_POWER_DEFLECT;
							pp2_play_sample(gp, gp->player[i].character->sample[PP2_SAMPLE_DEFLECT], gp->player[i].x + gp->player[i].object[0]->map.top.point[0].x, gp->player[i].y + gp->player[i].object[0]->map.left.point[0].y, 1.0, 1.0);
							o = pp2_generate_object(op->x, op->y, op->layer, PP2_OBJECT_GENERATOR, 0, op->type);
							if(o >= 0)
							{
								pp2_object[o].counter = 1800;
							}
							op->flags = 0;
						}
					}
				}
				break;
			}
			case PP2_OBJECT_POWER_FLY:
			{
				t3f_move_collision_object_x(op->object, op->x);
				t3f_move_collision_object_y(op->object, op->y);
				for(i = 0; i < PP2_MAX_PLAYERS; i++)
				{
					if(gp->player[i].flags & PP2_PLAYER_FLAG_ACTIVE)
					{
						if(t3f_check_object_collision(op->object, gp->player[i].object[gp->player[i].current_object]))
						{
							gp->player[i].timer[PP2_PLAYER_TIMER_FLY] = 600;
							gp->player[i].flags |= PP2_PLAYER_FLAG_POWER_FLY;
							pp2_play_sample(gp, gp->player[i].character->sample[PP2_SAMPLE_PFLY], gp->player[i].x + gp->player[i].object[0]->map.top.point[0].x, gp->player[i].y + gp->player[i].object[0]->map.left.point[0].y, 1.0, 1.0);
							o = pp2_generate_object(op->x, op->y, op->layer, PP2_OBJECT_GENERATOR, 0, op->type);
							if(o >= 0)
							{
								pp2_object[o].counter = 1800;
							}
							op->flags = 0;
						}
					}
				}
				break;
			}
			case PP2_OBJECT_POWER_TURBO:
			{
				t3f_move_collision_object_x(op->object, op->x);
				t3f_move_collision_object_y(op->object, op->y);
				for(i = 0; i < PP2_MAX_PLAYERS; i++)
				{
					if(gp->player[i].flags & PP2_PLAYER_FLAG_ACTIVE)
					{
						if(t3f_check_object_collision(op->object, gp->player[i].object[gp->player[i].current_object]))
						{
							gp->player[i].timer[PP2_PLAYER_TIMER_TURBO] = 600;
							gp->player[i].flags |= PP2_PLAYER_FLAG_POWER_TURBO;
							pp2_play_sample(gp, gp->player[i].character->sample[PP2_SAMPLE_PTURBO], gp->player[i].x + gp->player[i].object[0]->map.top.point[0].x, gp->player[i].y + gp->player[i].object[0]->map.left.point[0].y, 1.0, 1.0);
							o = pp2_generate_object(op->x, op->y, op->layer, PP2_OBJECT_GENERATOR, 0, op->type);
							if(o >= 0)
							{
								pp2_object[o].counter = 1800;
							}
							op->flags = 0;
						}
					}
				}
				break;
			}
			case PP2_OBJECT_SPRING_UP:
			{
				t3f_move_collision_object_x(op->object, op->x);
				t3f_move_collision_object_y(op->object, op->y);
				for(i = 0; i < PP2_MAX_PLAYERS; i++)
				{
					if(gp->player[i].flags & PP2_PLAYER_FLAG_ACTIVE)
					{
						if(gp->player[i].vy > 0.0 && t3f_check_object_collision(op->object, gp->player[i].object[gp->player[i].current_object]))
						{
							gp->player[i].vy = -28.0;
							gp->player[i].y = t3f_get_object_top_y(gp->player[i].object[0], op->object);
							pp2_player_move_object_y(&gp->player[i]);
							pp2_play_sample(gp, gp->player[i].character->sample[PP2_SAMPLE_SPRING], op->x + op->object->map.top.point[0].x, op->y + op->object->map.left.point[0].y, 1.0, 1.0);
						}
					}
				}
				break;
			}
			case PP2_OBJECT_SPRING_DOWN:
			{
				t3f_move_collision_object_x(op->object, op->x);
				t3f_move_collision_object_y(op->object, op->y);
				for(i = 0; i < PP2_MAX_PLAYERS; i++)
				{
					if(gp->player[i].flags & PP2_PLAYER_FLAG_ACTIVE)
					{
						if(gp->player[i].vy < 0.0 && t3f_check_object_collision(op->object, gp->player[i].object[gp->player[i].current_object]))
						{
							gp->player[i].vy = 14.0;
							gp->player[i].y = t3f_get_object_bottom_y(gp->player[i].object[0], op->object);
							pp2_player_move_object_y(&gp->player[i]);
							pp2_play_sample(gp, gp->player[i].character->sample[PP2_SAMPLE_SPRING], op->x + op->object->map.top.point[0].x, op->y + op->object->map.left.point[0].y, 1.0, 1.0);
						}
					}
				}
				break;
			}
			case PP2_OBJECT_SPRING_LEFT:
			{
				t3f_move_collision_object_x(op->object, op->x);
				t3f_move_collision_object_y(op->object, op->y);
				for(i = 0; i < PP2_MAX_PLAYERS; i++)
				{
					if(gp->player[i].flags & PP2_PLAYER_FLAG_ACTIVE)
					{
						if(gp->player[i].vx > 0.0 && t3f_check_object_collision(op->object, gp->player[i].object[gp->player[i].current_object]))
						{
							gp->player[i].vx = -28.0;
							gp->player[i].x = t3f_get_object_left_x(gp->player[i].object[0], op->object);
							pp2_player_move_object_x(&gp->player[i]);
							pp2_play_sample(gp, gp->player[i].character->sample[PP2_SAMPLE_SPRING], op->x + op->object->map.top.point[0].x, op->y + op->object->map.left.point[0].y, 1.0, 1.0);
						}
					}
				}
				break;
			}
			case PP2_OBJECT_SPRING_RIGHT:
			{
				t3f_move_collision_object_x(op->object, op->x);
				t3f_move_collision_object_y(op->object, op->y);
				for(i = 0; i < PP2_MAX_PLAYERS; i++)
				{
					if(gp->player[i].flags & PP2_PLAYER_FLAG_ACTIVE)
					{
						if(gp->player[i].vx < 0.0 && t3f_check_object_collision(op->object, gp->player[i].object[gp->player[i].current_object]))
						{
							gp->player[i].vx = 28.0;
							gp->player[i].x = t3f_get_object_right_x(gp->player[i].object[0], op->object);
							pp2_player_move_object_x(&gp->player[i]);
							pp2_play_sample(gp, gp->player[i].character->sample[PP2_SAMPLE_SPRING], op->x + op->object->map.top.point[0].x, op->y + op->object->map.left.point[0].y, 1.0, 1.0);
						}
					}
				}
				break;
			}
			case PP2_OBJECT_COIN:
			{
				op->x += op->vx;
				op->x += pp2_fixtof(pp2_object_convey(op));
				t3f_move_collision_object_x(op->object, op->x);
				if(t3f_check_tilemap_collision_left(op->object, pp2_level->collision_tilemap[op->layer]) || t3f_check_tilemap_collision_right(op->object, pp2_level->collision_tilemap[op->layer]))
				{
					op->x = t3f_get_tilemap_collision_x(op->object, pp2_level->collision_tilemap[op->layer]);
					t3f_move_collision_object_x(op->object, op->x);
					op->vx = -op->vx;
				}
				op->y += op->vy;
				t3f_move_collision_object_y(op->object, op->y);
				if(t3f_check_tilemap_collision_top(op->object, pp2_level->collision_tilemap[op->layer]))
				{
					op->y = t3f_get_tilemap_collision_y(op->object, pp2_level->collision_tilemap[op->layer]);
					t3f_move_collision_object_y(op->object, op->y);
					op->vy = 0;
				}
				if(t3f_check_tilemap_collision_bottom(op->object, pp2_level->collision_tilemap[op->layer]))
				{
					if(op->vy > 2.0)
					{
						pp2_play_sample(gp, pp2_sample[PP2_SAMPLE_COIN_LAND], op->x + op->object->map.top.point[0].x, op->y + op->object->map.left.point[0].y, (op->vy - 2.0) / 13.0, 1.0);
					}
					op->y = t3f_get_tilemap_collision_y(op->object, pp2_level->collision_tilemap[op->layer]);
					t3f_move_collision_object_y(op->object, op->y);
					if(op->vy > 2.0)
					{
						op->vy = -op->vy / 2.0;
					}
					else
					{
						op->vy = 0.0;
						op->flags |= PP2_OBJECT_FLAG_GROUND;
					}
				}

				/* apply friction to coins on ground */
				if(op->flags & PP2_OBJECT_FLAG_GROUND && !pp2_object_on_ice(op))
				{
					if(op->vx < 0.0)
					{
						op->vx += 0.5;
						if(op->vx >= 0.0)
						{
							op->vx = 0.0;
						}
					}
					else if(op->vx > 0.0)
					{
						op->vx -= 0.5;
						if(op->vx <= 0.0)
						{
							op->vx = 0.0;
						}
					}
					if(!pp2_object_on_floor(op))
					{
						op->flags &= ~PP2_OBJECT_FLAG_GROUND;
					}
				}
				else
				{
					/* apply gravity */
					op->vy += 1.0;
					if(op->vy > 14.0)
					{
						op->vy = 14.0;
					}
				}

				/* see if anyone picks it up */
				for(i = 0; i < PP2_MAX_PLAYERS; i++)
				{
					if(gp->player[i].flags & PP2_PLAYER_FLAG_ACTIVE && gp->player[i].life > 0)
					{
						if(t3f_check_object_collision(op->object, gp->player[i].object[gp->player[i].current_object]))
						{
							gp->player[i].coins++;
							op->flags = 0;
							pp2_play_sample(gp, gp->player[i].character->sample[PP2_SAMPLE_COIN_PICKUP], op->x + op->object->map.top.point[0].x, op->y + op->object->map.left.point[0].y, 1.0, 1.0);
						}
					}
				}
				if(op->flags & PP2_OBJECT_FLAG_ACTIVE)
				{
					pp2_radar_object[pp2_radar_objects].x = op->x + op->object->map.top.point[0].x;
					pp2_radar_object[pp2_radar_objects].y = op->y + op->object->map.left.point[0].y;
					pp2_radar_object[pp2_radar_objects].player = -1;
					pp2_radar_objects++;
				}
				break;
			}
			case PP2_OBJECT_TILE_READER:
			{
				int tx = op->x / 32;
				int ty = op->y / 32;
//				int oflags = pp2_level->collision_tilemap->data[ty][tx].flags;
				int tflags = pp2_level->tileset->tile[(int)t3f_get_tile(pp2_level->tileset, pp2_level->tilemap->layer[op->layer]->data[ty][tx], pp2_tick)]->user_data[15];
				pp2_level->collision_tilemap[op->layer]->data[ty][tx].flags = 0;
				if(tflags & 1)
				{
					pp2_level->collision_tilemap[op->layer]->data[ty][tx].flags |= T3F_COLLISION_FLAG_SOLID_TOP;
				}
				if(tflags & 2)
				{
					pp2_level->collision_tilemap[op->layer]->data[ty][tx].flags |= T3F_COLLISION_FLAG_SOLID_BOTTOM;
				}
				if(tflags & 4)
				{
					pp2_level->collision_tilemap[op->layer]->data[ty][tx].flags |= T3F_COLLISION_FLAG_SOLID_LEFT;
				}
				if(tflags & 8)
				{
					pp2_level->collision_tilemap[op->layer]->data[ty][tx].flags |= T3F_COLLISION_FLAG_SOLID_RIGHT;
				}
				break;
			}
			default:
			{
				t3f_move_collision_object_x(op->object, op->x);
				t3f_move_collision_object_y(op->object, op->y);
				for(i = 0; i < PP2_MAX_PLAYERS; i++)
				{
					if(gp->player[i].flags & PP2_PLAYER_FLAG_ACTIVE)
					{
						if(t3f_check_object_collision(op->object, gp->player[i].object[gp->player[i].current_object]))
						{
							op->flags = 0;
						}
					}
				}
				break;
			}
		}
		op->tick++;
	}
}

void pp2_object_render(PP2_OBJECT * op, PP2_CAMERA * cp, PP2_RESOURCES * resources)
{
	switch(op->type)
	{
		default:
		{
			if((op->flags & PP2_OBJECT_FLAG_ACTIVE) && op->type >= 0 && resources->object_animation[op->type])
			{
				t3f_draw_animation(resources->object_animation[op->type], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), op->tick, op->x - cp->x, op->y - cp->y, op->z - cp->z, 0);
			}
			break;
		}
	}
}
