#include <math.h>
#include "../../t3f/t3f.h"
#include "../../t3f/sound.h"
#include "../../data.h"
#include "../../tables.h"
#include "../../misc/fixed_point.h"
#include "../../misc/sound.h"
#include "../game.h"
#include "../game_defines.h"
#include "object_defines.h"
#include "../camera.h"
#include "player.h"
#include "paintball_defines.h"
#include "particle_defines.h"
#include "../../resource.h"

static int pp2_find_closest_player(PP2_GAME * gp, PP2_PLAYER * pp)
{
	int i;
	float current;
	float closest = 1000000.0;
	int closest_i = -1;

	for(i = 0; i < PP2_MAX_PLAYERS; i++)
	{
		if(i != pp->id && (gp->player[i].flags & PP2_PLAYER_FLAG_ACTIVE) && !(gp->player[i].flags & PP2_PLAYER_FLAG_POWER_CLOAK) && gp->player[i].fade_time <= 0)
		{
			current = t3f_distance(pp->x, pp->y, gp->player[i].x, gp->player[i].y);
			if(current < closest)
			{
				closest = current;
				closest_i = i;
			}
		}
	}
	return closest_i;
}

static void pp2_find_closest_coin(PP2_GAME * gp, PP2_PLAYER * pp)
{
	int i;
	float current;
	float closest = 1000000.0;
	int closest_i = -1;
	int closest_type = -1;

	pp->coin_target = false;
	for(i = 0; i < PP2_MAX_PLAYERS; i++)
	{
		if(i != pp->id && (gp->player[i].flags & PP2_PLAYER_FLAG_ACTIVE) && !(gp->player[i].flags & PP2_PLAYER_FLAG_POWER_CLOAK) && gp->player[i].coins > 0)
		{
			current = t3f_distance(pp->x, pp->y, gp->player[i].x, gp->player[i].y);
			if(current < closest)
			{
				closest = current;
				closest_i = i;
				closest_type = 0;
			}
		}
	}
	for(i = 0; i < pp2_object_size; i++)
	{
		if(pp2_object[i].flags & PP2_OBJECT_FLAG_ACTIVE && pp2_object[i].type == PP2_OBJECT_COIN)
		{
			current = t3f_distance(pp->x, pp->y, pp2_object[i].x, pp2_object[i].y);
			if(current < closest)
			{
				closest = current;
				closest_i = i;
				closest_type = 1;
			}
		}
	}
	if(closest_i >= 0)
	{
		if(closest_type == 0)
		{
			pp->coin_target = true;
			pp->coin_target_x = gp->player[closest_i].x + gp->player[closest_i].object[0]->map.top.point[0].x;
			pp->coin_target_y = gp->player[closest_i].y + gp->player[closest_i].object[0]->map.left.point[0].y;
		}
		else
		{
			pp->coin_target = true;
			pp->coin_target_x = pp2_object[closest_i].x + pp2_object[closest_i].object->map.top.point[0].x;
			pp->coin_target_y = pp2_object[closest_i].y + pp2_object[closest_i].object->map.left.point[0].y;
		}
	}
}

void pp2_player_change_state(PP2_PLAYER * pp, int state)
{
	if(state >= PP2_CHARACTER_STATE_WALK_R_R && state <= PP2_CHARACTER_STATE_WALK_L_UR && pp->state >= PP2_CHARACTER_STATE_WALK_R_R && pp->state <= PP2_CHARACTER_STATE_WALK_L_UR)
	{
	}
	else if(state != pp->state)
	{
		pp->tick = 0;
	}
	pp->state = state;
}

void pp2_player_move_object_x(PP2_PLAYER * pp)
{
	t3f_move_collision_object_x(pp->object[0], pp->x);
	t3f_move_collision_object_x(pp->object[1], pp->x);
	t3f_move_collision_object_x(pp->object[2], pp->x);
}

void pp2_player_move_object_y(PP2_PLAYER * pp)
{
	t3f_move_collision_object_y(pp->object[0], pp->y);
	t3f_move_collision_object_y(pp->object[1], pp->y);
	t3f_move_collision_object_y(pp->object[2], pp->y);
}

static bool pp2_player_on_floor(PP2_GAME * gp, PP2_PLAYER * pp)
{
	int i;

	for(i = 0; i < pp->object[pp->current_object]->map.bottom.points; i++)
	{
		if(t3f_get_collision_tilemap_flag(gp->level->collision_tilemap[pp->layer], pp->x + pp->object[pp->current_object]->map.bottom.point[i].x, pp->y + pp->object[pp->current_object]->map.bottom.point[i].y + 1.0, T3F_COLLISION_FLAG_SOLID_TOP))
		{
			return true;
		}
	}
	return false;
}

/* returns true if player is on slippery surface */
static bool pp2_player_slip(PP2_GAME * gp, PP2_PLAYER * pp)
{
	int i;
	int cf;
	int slip_center;

	if((pp->flags & PP2_PLAYER_FLAG_GROUND))
	{
		slip_center = t3f_get_collision_tilemap_flag(gp->level->collision_tilemap[pp->layer], pp->x + pp->object[pp->current_object]->map.bottom.point[0].x, pp->y + pp->object[pp->current_object]->map.bottom.point[0].y + 1.0, T3F_COLLISION_FLAG_SOLID_TOP | PP2_LEVEL_COLLISION_FLAG_ICE);
		if(slip_center == (T3F_COLLISION_FLAG_SOLID_TOP | PP2_LEVEL_COLLISION_FLAG_ICE))
		{
			return true;
		}
		else
		{
			for(i = 1; i < pp->object[pp->current_object]->map.bottom.points; i++)
			{
				cf = t3f_get_collision_tilemap_flag(gp->level->collision_tilemap[pp->layer], pp->x + pp->object[pp->current_object]->map.bottom.point[i].x, pp->y + pp->object[pp->current_object]->map.bottom.point[i].y + 1.0, T3F_COLLISION_FLAG_SOLID_TOP | PP2_LEVEL_COLLISION_FLAG_ICE);
				if((cf & T3F_COLLISION_FLAG_SOLID_TOP) && !(cf & PP2_LEVEL_COLLISION_FLAG_ICE))
				{
					return false;
				}
			}
			return true;
		}
	}
	return false;
}

/* return conveyor speed if player is on a conveyor, else return 0
   conveyor speed is returned in fixed point notation, convert before using */
static int pp2_player_convey(PP2_GAME * gp, PP2_PLAYER * pp)
{
	int i;
	int cf;

	if((pp->flags & PP2_PLAYER_FLAG_GROUND))
	{
		for(i = 0; i < pp->object[pp->current_object]->map.bottom.points; i++)
		{
			cf = t3f_get_collision_tilemap_flag(gp->level->collision_tilemap[pp->layer], pp->x + pp->object[pp->current_object]->map.bottom.point[i].x, pp->y + pp->object[pp->current_object]->map.bottom.point[i].y + 1.0, T3F_COLLISION_FLAG_SOLID_TOP | PP2_LEVEL_COLLISION_FLAG_CONVEYOR);
			if(cf == (T3F_COLLISION_FLAG_SOLID_TOP | PP2_LEVEL_COLLISION_FLAG_CONVEYOR))
			{
				return t3f_get_collision_tilemap_data(gp->level->collision_tilemap[pp->layer], pp->x + pp->object[pp->current_object]->map.bottom.point[i].x, pp->y + pp->object[pp->current_object]->map.bottom.point[i].y + 1.0, 0);
			}
		}
	}
	return 0;
}

/* bump player around the edge of a tile to make it easier to make certain jumps */
static float pp2_player_fudge_x(T3F_COLLISION_OBJECT * cp, T3F_COLLISION_TILEMAP * tmp, int dir)
{
	int tw = tmp->tile_width;
	float rx;

    /* if sprite was moving left */
    if(dir == 0)
    {
	    rx = (((int)(cp->x + cp->map.left.point[0].x) / tw) * tw) + tw - (int)(cp->map.left.point[0].x);
	    if(rx < 0)
	    {
		    rx -= tw;
	    }
        return rx;
    }

    /* if sprite was moving right */
    else if(dir == 1)
    {
	    rx = (((int)(cp->x + cp->map.right.point[0].x) / tw) * tw) - (cp->map.right.point[0].x - cp->map.left.point[0].x) - 1 - (int)cp->map.left.point[0].x;
	    if(rx < 0)
	    {
		    rx -= tw;
	    }
        return rx;
    }

    /* if sprite wasn't moving */
    else
    {
        return cp->x;
    }
}

static void pp2_player_stop(PP2_PLAYER * pp)
{
	if(pp->state >= PP2_CHARACTER_STATE_WALK_R_R && pp->state <= PP2_CHARACTER_STATE_WALK_L_UR)
	{
		pp->state = (pp->state % 16) + PP2_CHARACTER_STATE_STAND_R_R;
	}
}

static bool pp2_player_check_paintball_side_collision(PP2_GAME * gp, float x, float y, T3F_COLLISION_LIST * lp, int layer, int flags)
{
	int i;
	int fl;

	for(i = 0; i < lp->points; i++)
	{
		fl = t3f_get_collision_tilemap_flag(gp->level->collision_tilemap[layer], x + lp->point[i].x, y + lp->point[i].y, flags);
		if(fl == flags)
		{
			return true;
		}
	}
	return false;
}

static bool pp2_player_paintball_position_ok(PP2_GAME * gp, PP2_PAINTBALL * pp, int dir)
{
	switch(dir)
	{
		case 0:
		{
			if(pp2_player_check_paintball_side_collision(gp, pp->x, pp->y, &(pp->object->map.right), pp->layer, T3F_COLLISION_FLAG_SOLID_LEFT | T3F_COLLISION_FLAG_SOLID_RIGHT))
			{
				return false;
			}
			break;
		}
		case 1:
		{
			if(pp2_player_check_paintball_side_collision(gp, pp->x, pp->y, &(pp->object->map.right), pp->layer, T3F_COLLISION_FLAG_SOLID_LEFT | T3F_COLLISION_FLAG_SOLID_RIGHT))
			{
				return false;
			}
			else if(pp2_player_check_paintball_side_collision(gp, pp->x, pp->y, &(pp->object->map.bottom), pp->layer, T3F_COLLISION_FLAG_SOLID_BOTTOM | T3F_COLLISION_FLAG_SOLID_TOP))
			{
				return false;
			}
			break;
		}
		case 2:
		{
			if(pp2_player_check_paintball_side_collision(gp, pp->x, pp->y, &(pp->object->map.bottom), pp->layer, T3F_COLLISION_FLAG_SOLID_BOTTOM | T3F_COLLISION_FLAG_SOLID_TOP))
			{
				return false;
			}
			break;
		}
		case 3:
		{
			if(pp2_player_check_paintball_side_collision(gp, pp->x, pp->y, &(pp->object->map.left), pp->layer, T3F_COLLISION_FLAG_SOLID_LEFT | T3F_COLLISION_FLAG_SOLID_RIGHT))
			{
				return false;
			}
			else if(pp2_player_check_paintball_side_collision(gp, pp->x, pp->y, &(pp->object->map.bottom), pp->layer, T3F_COLLISION_FLAG_SOLID_BOTTOM | T3F_COLLISION_FLAG_SOLID_TOP))
			{
				return false;
			}
			break;
		}
		case 4:
		{
			if(pp2_player_check_paintball_side_collision(gp, pp->x, pp->y, &(pp->object->map.left), pp->layer, T3F_COLLISION_FLAG_SOLID_LEFT | T3F_COLLISION_FLAG_SOLID_RIGHT))
			{
				return false;
			}
			break;
		}
		case 5:
		{
			if(pp2_player_check_paintball_side_collision(gp, pp->x, pp->y, &(pp->object->map.left), pp->layer, T3F_COLLISION_FLAG_SOLID_LEFT | T3F_COLLISION_FLAG_SOLID_RIGHT))
			{
				return false;
			}
			else if(pp2_player_check_paintball_side_collision(gp, pp->x, pp->y, &(pp->object->map.top), pp->layer, T3F_COLLISION_FLAG_SOLID_BOTTOM | T3F_COLLISION_FLAG_SOLID_TOP))
			{
				return false;
			}
			break;
		}
		case 6:
		{
			if(pp2_player_check_paintball_side_collision(gp, pp->x, pp->y, &(pp->object->map.top), pp->layer, T3F_COLLISION_FLAG_SOLID_BOTTOM | T3F_COLLISION_FLAG_SOLID_TOP))
			{
				return false;
			}
			break;
		}
		case 7:
		{
			if(pp2_player_check_paintball_side_collision(gp, pp->x, pp->y, &(pp->object->map.right), pp->layer, T3F_COLLISION_FLAG_SOLID_LEFT | T3F_COLLISION_FLAG_SOLID_RIGHT))
			{
				return false;
			}
			else if(pp2_player_check_paintball_side_collision(gp, pp->x, pp->y, &(pp->object->map.top), pp->layer, T3F_COLLISION_FLAG_SOLID_BOTTOM | T3F_COLLISION_FLAG_SOLID_TOP))
			{
				return false;
			}
			break;
		}
	}
	return true;
}

static void pp2_player_adjust_paintball_position(PP2_PAINTBALL * pp, int dir)
{
	float tx, ty;

	switch(dir)
	{
		case 0:
		{
		    pp->x = (((int)(pp->object->x + pp->object->map.right.point[0].x) / 32) * 32) - (pp->object->map.right.point[0].x - pp->object->map.left.point[0].x) - 1 - (int)pp->object->map.left.point[0].x;
		    pp->object->x = pp->x;
		    pp->object->ox = pp->x;
			break;
		}
		case 1:
		{
		    pp->x = (((int)(pp->object->x + pp->object->map.right.point[0].x) / 32) * 32) - (pp->object->map.right.point[0].x - pp->object->map.left.point[0].x) - 1 - (int)pp->object->map.left.point[0].x;
		    pp->object->x = pp->x;
		    pp->object->ox = pp->x;
		    pp->y = (((int)(pp->object->y + pp->object->map.bottom.point[0].y) / 32) * 32) - (pp->object->map.bottom.point[0].y - pp->object->map.top.point[0].y) - 1 - (int)pp->object->map.top.point[0].y;
		    pp->object->y = pp->y;
		    pp->object->oy = pp->y;
			break;
		}
		case 2:
		{
		    pp->y = (((int)(pp->object->y + pp->object->map.bottom.point[0].y) / 32) * 32) - (pp->object->map.bottom.point[0].y - pp->object->map.top.point[0].y) - 1 - (int)pp->object->map.top.point[0].y;
		    pp->object->y = pp->y;
		    pp->object->oy = pp->y;
			break;
		}
		case 3:
		{
			tx = (((int)(pp->object->x + pp->object->map.left.point[0].x) / 32) * 32) + 32;
			pp->x =  tx - (int)(pp->object->map.left.point[0].x);
		    pp->object->x = pp->x;
		    pp->object->ox = pp->x;
		    pp->y = (((int)(pp->object->y + pp->object->map.bottom.point[0].y) / 32) * 32) - (pp->object->map.bottom.point[0].y - pp->object->map.top.point[0].y) - 1 - (int)pp->object->map.top.point[0].y;
		    pp->object->y = pp->y;
		    pp->object->oy = pp->y;
			break;
		}
		case 4:
		{
			tx = (((int)(pp->object->x + pp->object->map.left.point[0].x) / 32) * 32) + 32;
			pp->x =  tx - (int)(pp->object->map.left.point[0].x);
		    pp->object->x = pp->x;
		    pp->object->ox = pp->x;
			break;
		}
		case 5:
		{
			tx = (((int)(pp->object->x + pp->object->map.left.point[0].x) / 32) * 32) + 32;
			pp->x =  tx - (int)(pp->object->map.left.point[0].x);
		    pp->object->x = pp->x;
		    pp->object->ox = pp->x;
			ty = (((int)(pp->object->y + pp->object->map.top.point[0].y) / 32) * 32) + 32;
			pp->y =  ty - (int)(pp->object->map.top.point[0].y);
		    pp->object->y = pp->y;
		    pp->object->oy = pp->y;
			break;
		}
		case 6:
		{
			ty = (((int)(pp->object->y + pp->object->map.top.point[0].y) / 32) * 32) + 32;
			pp->y =  ty - (int)(pp->object->map.top.point[0].y);
		    pp->object->y = pp->y;
		    pp->object->oy = pp->y;
			break;
		}
		case 7:
		{
		    pp->x = (((int)(pp->object->x + pp->object->map.right.point[0].x) / 32) * 32) - (pp->object->map.right.point[0].x - pp->object->map.left.point[0].x) - 1 - (int)pp->object->map.left.point[0].x;
		    pp->object->x = pp->x;
		    pp->object->ox = pp->x;
			ty = (((int)(pp->object->y + pp->object->map.top.point[0].y) / 32) * 32) + 32;
			pp->y =  ty - (int)(pp->object->map.top.point[0].y);
		    pp->object->y = pp->y;
		    pp->object->oy = pp->y;
			break;
		}
	}
}

static void pp2_player_generate_paintball(PP2_GAME * gp, PP2_PLAYER * pp)
{
	int dir = pp->state % 8;
	int p;

	p = pp2_create_paintball(gp, pp->id, pp->weapon, pp->x + pp->character->state[pp->state].paintball.x, pp->y + pp->character->state[pp->state].paintball.y, pp2_angle_table[pp->state]);
	if(p >= 0)
	{
		/* adjust position if we are in a solid wall */
		if(!pp2_player_paintball_position_ok(gp, &pp->paintball[p], dir))
		{
			pp2_player_adjust_paintball_position(&pp->paintball[p], dir);
		}
		pp->paintball[p].leaving = 4;
		if(pp->weapon == PP2_PAINTBALL_TYPE_SEEKER)
		{
			pp->paintball[p].target = pp->target;
		}
		else if(pp->weapon == PP2_PAINTBALL_TYPE_REFLECTOR)
		{
			pp->paintball[p].counter = 5;
		}
	}
	pp2_play_sample(gp, pp->character->sample[PP2_SAMPLE_FIRE], pp->paintball[p].x, pp->paintball[p].y, 1.0, 1.0);
}

/* player is on a floor that is only solid on top */
static bool pp2_player_partial_floor(PP2_GAME * gp, PP2_PLAYER * pp)
{
	int i;
	int f;

	if(pp->flags & PP2_PLAYER_FLAG_GROUND)
	{
		for(i = 0; i < pp->object[pp->current_object]->map.bottom.points; i++)
		{
			f = t3f_get_collision_tilemap_flag(gp->level->collision_tilemap[pp->layer], pp->x + pp->object[pp->current_object]->map.bottom.point[i].x, pp->y + pp->object[pp->current_object]->map.bottom.point[i].y + 1.0, T3F_COLLISION_FLAG_SOLID_TOP | T3F_COLLISION_FLAG_SOLID_LEFT | T3F_COLLISION_FLAG_SOLID_RIGHT | T3F_COLLISION_FLAG_SOLID_BOTTOM);
			if((f & T3F_COLLISION_FLAG_SOLID_TOP) && ((f & T3F_COLLISION_FLAG_SOLID_BOTTOM) || (f & T3F_COLLISION_FLAG_SOLID_LEFT) || (f & T3F_COLLISION_FLAG_SOLID_RIGHT)))
			{
				return false;
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}

void pp2_player_next_weapon(PP2_PLAYER * pp)
{
	int start = pp->weapon;

	pp->last_weapon = pp->weapon;
	pp->weapon++;
	while(pp->weapon != start)
	{
		if(pp->weapon >= PP2_PLAYER_MAX_WEAPONS)
		{
			pp->weapon = 0;
		}
		if(pp->weapon == start)
		{
			break;
		}
		if(pp->ammo[pp->weapon])
		{
			break;
		}
		pp->weapon++;
	}
}

static bool pp2_player_select_weapon(PP2_PLAYER * pp, int weapon)
{
	if(pp->ammo[weapon] && pp->weapon != weapon)
	{
		pp->weapon = weapon;
		pp->reload_time = 35;
		pp->timer[PP2_PLAYER_TIMER_WEAPON_SELECT] = 30;
		return true;
	}
	return false;
}

void pp2_player_drop_coin(PP2_GAME * gp, PP2_PLAYER * pp, PP2_RESOURCES * resources)
{
	int o;
	float a;

	o = pp2_generate_object(gp, pp->x + pp->object[0]->map.top.point[0].x - 8, pp->y + pp->object[0]->map.left.point[0].y - 8, pp->layer, PP2_OBJECT_COIN, 0, 0);
	if(o >= 0)
	{
		a = 1.5 * ALLEGRO_PI + joynet_drand() * ALLEGRO_PI - ALLEGRO_PI / 2.0;
		pp2_object[o].vx = cos(a) * 6.0;
		pp2_object[o].vy = sin(a) * 6.0;
		pp2_object[o].tick = joynet_rand() % resources->object_animation[PP2_OBJECT_COIN]->frame_list_total;
		t3f_recreate_collision_object(pp2_object[o].object, 0, 0, 16, 16, 32, 32, 0);
		t3f_move_collision_object_xy(pp2_object[o].object, pp2_object[o].x, pp2_object[o].y);
	}
}

/* player receives a hit */
void pp2_player_receive_hit(PP2_GAME * gp, PP2_PLAYER * pp, int dealer, PP2_RESOURCES * resources)
{
	int i;

	gp->player[dealer].hits++;
	pp->life--;
	if(pp->life <= 0)
	{
		/* don't score for when you hit yourself */
		if(dealer != pp->id)
		{
			gp->player[dealer].frags++;
		}
		if(!pp2_replay_rewind)
		{
			t3f_play_sample(pp->character->sample[PP2_SAMPLE_TELE_OUT], 1.0, 0.0, 1.0);
		}
		if(pp2_option[PP2_OPTION_GAME_MODE] == PP2_GAME_MODE_COIN_RUSH)
		{
			for(i = 0; i < pp->coins; i++)
			{
				pp2_player_drop_coin(gp, pp, resources);
			}
			pp->coins = 0;
		}
	}
	pp->flash_time = 30;

	/* update profile */
	if(pp2_replay_player < 0)
	{
		if(pp2_client_game->player[pp->id]->local)
		{
			pp2_profiles.item[pp->profile_choice].shot++;
		}
		pp->shot++;
	}
}

void pp2_handle_player_to_player_collision_x(PP2_PLAYER * p1, PP2_PLAYER * p2)
{
	p1->x = t3f_get_object_collision_x(p1->object[p1->current_object], p2->object[p2->current_object]);
	if(p2->vx >= -0.1 && p2->vx <= 0.1)
	{
		p2->vx += p1->vx;
	}
	else
	{
		if(fabs(p1->fvx) > 0.1)
		{
			p2->vx += p1->vx / 2.0;
		}
		else if(fabs(p2->fvx) > 0.1)
		{
			p1->vx += p2->vx / 2.0;
		}
	}
	if(p2->vx > 5.0)
	{
		p2->vx = 5.0;
	}
	else if(p2->vx < -5.0)
	{
		p2->vx = -5.0;
	}
	p1->vx = p2->vx;
	pp2_player_move_object_x(p1);
}

void pp2_handle_player_to_player_collisions_x(PP2_GAME * gp, PP2_PLAYER * pp)
{
	int i;

	for(i = 0; i < PP2_MAX_PLAYERS; i++)
	{
		if(i != pp->id && gp->player[i].flags & PP2_PLAYER_FLAG_ACTIVE && !(gp->player[i].fade_time > 0 && gp->player[i].fade_type == 0) && t3f_check_object_collision(pp->object[pp->current_object], gp->player[i].object[gp->player[i].current_object]))
		{
			pp2_handle_player_to_player_collision_x(pp, &gp->player[i]);
		}
	}
}

void pp2_handle_player_to_player_collision_y(PP2_GAME * gp, PP2_PLAYER * p1, PP2_PLAYER * p2, PP2_RESOURCES * resources)
{
	PP2_PLAYER * top_player;
	PP2_PLAYER * bottom_player;

	/* determine which player is on top */
	if(p1->object[p1->current_object]->y + p1->object[p1->current_object]->map.top.point[0].y < p2->object[p1->current_object]->y + p2->object[p1->current_object]->map.top.point[0].y)
	{
		top_player = p1;
		bottom_player = p2;
	}
	else
	{
		top_player = p2;
		bottom_player = p1;
	}

	/* change velocities */
	top_player->vy = -15.0;
	bottom_player->vy = 0.0;

	/* reposition player so it doesn't overlap the other */
	p1->y = t3f_get_object_collision_y(p1->object[p1->current_object], p2->object[p2->current_object]);
	pp2_player_move_object_y(top_player);

	/* bottom player is standing so make them duck */
	if(bottom_player->state >= PP2_CHARACTER_STATE_STAND_R_R && bottom_player->state <= PP2_CHARACTER_STATE_WALK_L_UR)
	{
		if(bottom_player->character->flags & PP2_CHARACTER_FLAG_LEGACY)
		{
			if(bottom_player->state % 16 < 8)
			{
				pp2_player_change_state(bottom_player, PP2_CHARACTER_STATE_DUCK_R_R);
			}
			else
			{
				pp2_player_change_state(bottom_player, PP2_CHARACTER_STATE_DUCK_L_L);
			}
		}
		else
		{
			pp2_player_change_state(bottom_player, (bottom_player->state % 16) + PP2_CHARACTER_STATE_DUCK_R_R);
		}

		/* stomped players are stuck for a few ticks (adjusted to make it even) */
		if(bottom_player->id > top_player->id)
		{
			bottom_player->timer[PP2_PLAYER_TIMER_STOMPED] = 3;
		}
		else
		{
			bottom_player->timer[PP2_PLAYER_TIMER_STOMPED] = 2;
		}
	}

	/* top player jumps */
	if(top_player->character->flags & PP2_CHARACTER_FLAG_LEGACY)
	{
		if(top_player->state % 16 < 8)
		{
			pp2_player_change_state(top_player, PP2_CHARACTER_STATE_JUMP_R_R);
			top_player->flags &= ~PP2_PLAYER_FLAG_GROUND;
		}
		else
		{
			pp2_player_change_state(top_player, PP2_CHARACTER_STATE_JUMP_L_L);
			top_player->state = PP2_CHARACTER_STATE_JUMP_L_L;
			top_player->flags &= ~PP2_PLAYER_FLAG_GROUND;
		}
	}
	else
	{
		pp2_player_change_state(top_player, (top_player->state % 16) + PP2_CHARACTER_STATE_JUMP_R_R);
		top_player->flags &= ~PP2_PLAYER_FLAG_GROUND;
	}

	/* handle jump and bump logic */
	if(pp2_option[PP2_OPTION_STOMP_HITS])
	{
		pp2_player_receive_hit(gp, bottom_player, top_player->id, resources);
	}
}

void pp2_handle_player_to_player_collisions_y(PP2_GAME * gp, PP2_PLAYER * pp, PP2_RESOURCES * resources)
{
	int i;

	for(i = 0; i < PP2_MAX_PLAYERS; i++)
	{
		if(i != pp->id && gp->player[i].flags & PP2_PLAYER_FLAG_ACTIVE && !(gp->player[i].fade_time > 0 && gp->player[i].fade_type == 0) && t3f_check_object_collision(pp->object[pp->current_object], gp->player[i].object[gp->player[i].current_object]))
		{
			pp2_handle_player_to_player_collision_y(gp, pp, &gp->player[i], resources);
		}
	}
}

static void pp2_move_player(PP2_GAME * gp, PP2_PLAYER * pp, PP2_RESOURCES * resources)
{
	int convey = 0;

	pp->ovx = 0.0;
	convey = pp2_player_convey(gp, pp);
	if(convey)
	{
		pp->ovx = pp2_fixtof(convey);
	}
	pp->x += pp->vx + pp->ovx;
	pp2_player_move_object_x(pp);
	pp2_handle_player_to_player_collisions_x(gp, pp);
	if(!t3f_get_collision_tilemap_flag(gp->level->collision_tilemap[pp->layer], pp->object[pp->current_object]->x + pp->object[pp->current_object]->map.bottom.point[0].x, pp->object[pp->current_object]->y + pp->object[pp->current_object]->map.bottom.point[0].y, T3F_COLLISION_FLAG_SLOPE_TOP))
	{
		if(t3f_check_tilemap_collision_left(pp->object[pp->current_object], gp->level->collision_tilemap[pp->layer]) || t3f_check_tilemap_collision_right(pp->object[pp->current_object], gp->level->collision_tilemap[pp->layer]))
		{
			pp->x = t3f_get_tilemap_collision_x(pp->object[pp->current_object], gp->level->collision_tilemap[pp->layer]);
			pp2_player_move_object_x(pp);
			if(pp->ovx == 0.0)
			{
				pp->vx = 0.0;
				pp2_player_stop(pp);
			}
		}
	}
	if(pp->flags & PP2_PLAYER_FLAG_GROUND)
	{
		if(!pp2_player_on_floor(gp, pp))
		{
			if(pp->character->flags & PP2_CHARACTER_FLAG_LEGACY)
			{
				switch(pp->state)
				{
					case PP2_CHARACTER_STATE_STAND_R_R:
					case PP2_CHARACTER_STATE_STAND_R_U:
					case PP2_CHARACTER_STATE_WALK_R_R:
					case PP2_CHARACTER_STATE_DUCK_R_R:
					{
						pp->state = PP2_CHARACTER_STATE_FALL_R_R;
						break;
					}
					case PP2_CHARACTER_STATE_STAND_L_L:
					case PP2_CHARACTER_STATE_STAND_L_U:
					case PP2_CHARACTER_STATE_WALK_L_L:
					case PP2_CHARACTER_STATE_DUCK_L_L:
					{
						pp->state = PP2_CHARACTER_STATE_FALL_L_L;
						break;
					}
				}
			}
			else
			{
				pp->state = (pp->state % 16) + PP2_CHARACTER_STATE_FALL_R_R;
			}
			pp->flags ^= PP2_PLAYER_FLAG_GROUND;
		}
	}

	pp->y += pp->vy;
	pp2_player_move_object_y(pp);
	pp2_handle_player_to_player_collisions_y(gp, pp, resources);
	if(t3f_check_tilemap_collision_bottom(pp->object[pp->current_object], gp->level->collision_tilemap[pp->layer]))
	{
		pp->y = t3f_get_tilemap_collision_y(pp->object[pp->current_object], gp->level->collision_tilemap[pp->layer]);
		pp2_player_move_object_y(pp);
		pp->flags |= PP2_PLAYER_FLAG_GROUND;
		pp->vy = 0.0;
		pp->state = PP2_CHARACTER_STATE_STAND_R_R + pp->state % 16;
		pp2_play_sample(gp, pp->character->sample[PP2_SAMPLE_LAND], pp->x + pp->object[0]->map.top.point[0].x, pp->y + pp->object[0]->map.bottom.point[0].y, 1.0, 1.0);
	}
	else if(t3f_check_tilemap_collision_top(pp->object[pp->current_object], gp->level->collision_tilemap[pp->layer]))
	{
		if(!t3f_check_tilemap_collision_top(pp->object[2], gp->level->collision_tilemap[pp->layer]))
		{
			float oldx = pp->x;
			int i;

			/* need to move to the right */
			if(fmodf(pp->object[2]->x + pp->object[2]->map.left.point[0].x, 32.0) < fmodf(pp->object[0]->x + pp->object[0]->map.left.point[0].x, 32.0))
			{
				pp->x = pp2_player_fudge_x(pp->object[0], gp->level->collision_tilemap[pp->layer], 0);
			}

			/* need to move to the left */
			else
			{
				pp->x = pp2_player_fudge_x(pp->object[0], gp->level->collision_tilemap[pp->layer], 1);
			}
			pp2_player_move_object_x(pp);
			for(i = 0; i < PP2_MAX_PLAYERS; i++)
			{

				/* another player is blocking you so revert to old x position and hit head */
				if(i != pp->id && (gp->player[i].flags & PP2_PLAYER_FLAG_ACTIVE) && t3f_check_object_collision(pp->object[pp->current_object], gp->player[i].object[gp->player[i].current_object]))
				{
					pp->x = oldx;
					pp2_player_move_object_x(pp);
					pp->y = t3f_get_tilemap_collision_y(pp->object[pp->current_object], gp->level->collision_tilemap[pp->layer]);
					pp->vy = 0.0;
					pp2_player_move_object_y(pp);
					if(!(pp->flags & PP2_PLAYER_FLAG_POWER_FLY))
					{
						pp2_play_sample(gp, pp->character->sample[PP2_SAMPLE_BUMP], pp->x + pp->object[0]->map.top.point[0].x, pp->y + pp->object[0]->map.top.point[0].y, 1.0, 1.0);
					}
					break;
				}
			}
		}
		else
		{
			pp->y = t3f_get_tilemap_collision_y(pp->object[pp->current_object], gp->level->collision_tilemap[pp->layer]);
			pp->vy = 0.0;
			pp2_player_move_object_y(pp);
			if(!(pp->flags & PP2_PLAYER_FLAG_POWER_FLY))
			{
				pp2_play_sample(gp, pp->character->sample[PP2_SAMPLE_BUMP], pp->x + pp->object[0]->map.top.point[0].x, pp->y + pp->object[0]->map.top.point[0].y, 1.0, 1.0);
			}
		}
	}
}

static bool pp2_want_weapon(PP2_PLAYER * pp, int weapon)
{
	if(pp->ammo[weapon])
	{
		pp->want_weapon = weapon;
		return true;
	}
	return false;
}

static void pp2_control_player(PP2_GAME * gp, PP2_PLAYER * pp)
{
	bool switched = false;

	if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_FIRE].held && pp->ammo[pp->weapon] && pp->reload_time <= 0 && pp2_winner < 0)
	{
		pp2_player_generate_paintball(gp, pp);

		/* update profile */
		if(pp2_replay_player < 0 && pp2_option[PP2_OPTION_GAME_MODE] != PP2_GAME_MODE_EXPLORE)
		{
			if(pp2_client_game->player[pp->id]->local)
			{
				pp2_profiles.item[pp->profile_choice].shots++;
			}
			pp->shots++;
		}

		pp->ammo[pp->weapon]--;
		if(pp->ammo[pp->weapon] <= 0)
		{
			pp2_player_next_weapon(pp);
			if(pp->ammo[pp->weapon] > 0)
			{
				switched = true;
			}
		}
		if(pp->ammo[pp->weapon] > 0)
		{
			pp->reload_time = 35;
			if(switched)
			{
				pp->timer[PP2_PLAYER_TIMER_WEAPON_SELECT] = 32;
			}
		}
		else if(switched)
		{
			pp->timer[PP2_PLAYER_TIMER_WEAPON_SELECT] = 32;
		}
	}
	if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_SELECT].held)
	{
		if(!pp->choose_weapon)
		{
			pp->choose_weapon = true;
			pp->want_weapon = -1;
		}
		if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_UP].held && pp2_controller[pp->controller]->state[PP2_CONTROLLER_RIGHT].held)
		{
			pp2_want_weapon(pp, 1);
		}
		else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_DOWN].held && pp2_controller[pp->controller]->state[PP2_CONTROLLER_RIGHT].held)
		{
			pp2_want_weapon(pp, 3);

		}
		else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_DOWN].held && pp2_controller[pp->controller]->state[PP2_CONTROLLER_LEFT].held)
		{
			pp2_want_weapon(pp, 5);
		}
		else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_UP].held && pp2_controller[pp->controller]->state[PP2_CONTROLLER_LEFT].held)
		{
			pp2_want_weapon(pp, 7);
		}
		else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_UP].held)
		{
			pp2_want_weapon(pp, 0);
		}
		else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_RIGHT].held)
		{
			pp2_want_weapon(pp, 2);
		}
		else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_DOWN].held)
		{
			pp2_want_weapon(pp, 4);
		}
		else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_LEFT].held)
		{
			pp2_want_weapon(pp, 6);
		}
/*		pp2_player_next_weapon(pp);
		if(pp->ammo[pp->weapon] > 0)
		{
			pp->reload_time = 35;
			pp->timer[PP2_PLAYER_TIMER_WEAPON_SELECT] = 32;
		} */
	}
	else
	{
		if(pp->choose_weapon)
		{
			if(pp->want_weapon >= 0)
			{
				pp2_player_select_weapon(pp, pp->want_weapon);
			}
			pp->choose_weapon = false;
		}
	}
	if(pp->reload_time)
	{
		if(pp->flags & PP2_PLAYER_FLAG_POWER_TURBO)
		{
			pp->reload_time -= 2;
		}
		else
		{
			pp->reload_time--;
		}
		if(pp->reload_time == 25)
		{
			pp2_play_sample(gp, pp->character->sample[PP2_SAMPLE_RELOAD_A], pp->x + pp->object[0]->map.top.point[0].x, pp->y + pp->object[0]->map.left.point[0].y, 1.0, 1.0);
		}
		else if(pp->reload_time == 9)
		{
			pp2_play_sample(gp, pp->character->sample[PP2_SAMPLE_RELOAD_B], pp->x + pp->object[0]->map.top.point[0].x, pp->y + pp->object[0]->map.left.point[0].y, 1.0, 1.0);
		}
	}
	if(pp->flash_time)
	{
		pp->flash_time--;
	}
}

void pp2_generate_fly_particle(PP2_GAME * gp, PP2_PLAYER * pp, PP2_RESOURCES * resources)
{
	float a;

	a = 1.5 * ALLEGRO_PI + joynet_drand() * 2.0 - 1.0;
	gp->particle[gp->current_particle].type = 1;
	gp->particle[gp->current_particle].x = pp->x + pp->object[0]->map.bottom.point[0].x - resources->object_animation[PP2_OBJECT_JET]->frame[0]->width / 2;
	gp->particle[gp->current_particle].y = pp->y + pp->object[0]->map.bottom.point[0].y - resources->object_animation[PP2_OBJECT_JET]->frame[0]->height / 2;
	gp->particle[gp->current_particle].vx = cos(a) * 0.5;
	gp->particle[gp->current_particle].vy = sin(a) * 0.5;
	gp->particle[gp->current_particle].total_life = resources->object_animation[PP2_OBJECT_JET]->frame_list_total;
	gp->particle[gp->current_particle].life = gp->particle[gp->current_particle].total_life;
	gp->particle[gp->current_particle].tick = 0;
	gp->particle[gp->current_particle].flags = PP2_PARTICLE_FLAG_ACTIVE;
	gp->current_particle++;
	if(gp->current_particle >= PP2_MAX_PARTICLES)
	{
		gp->current_particle = 0;
	}
}

void pp2_legacy_player_logic(PP2_GAME * gp, PP2_PLAYER * pp, PP2_RESOURCES * resources)
{
	bool friction = false;
	float mx = 4.0;

	if(pp->flags & PP2_PLAYER_FLAG_POWER_RUN)
	{
		mx = 5.0;
	}

	/* adjust state */
	if(!pp->choose_weapon)
	{
		switch(pp->state)
		{
			case PP2_CHARACTER_STATE_STAND_R_R:
			{
				if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_JUMP].pressed)
				{
					pp->state = PP2_CHARACTER_STATE_JUMP_R_R;
					pp->vy = -15.0;
					if(pp->flags & PP2_PLAYER_FLAG_GROUND)
					{
						pp->flags ^= PP2_PLAYER_FLAG_GROUND;
					}
					pp->tick = 0;
					pp->current_object = 0;
					pp2_play_sample(gp, pp->character->sample[PP2_SAMPLE_JUMP], pp->x + pp->object[0]->map.bottom.point[0].x, pp->y + pp->object[0]->map.bottom.point[0].y, 1.0, 1.0);
				}
				else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_RIGHT].held)
				{
					pp->state = PP2_CHARACTER_STATE_WALK_R_R;
					pp->current_object = 0;
					pp->tick = 0;
				}
				else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_LEFT].held)
				{
					if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_STRAFE].held)
					{
						pp->state = PP2_CHARACTER_STATE_WALK_R_R;
						pp->current_object = 0;
					}
					else
					{
						pp->state = PP2_CHARACTER_STATE_WALK_L_L;
						pp->current_object = 0;
						pp->tick = 0;
					}
				}
				else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_UP].held)
				{
					pp->state = PP2_CHARACTER_STATE_STAND_R_U;
					pp->current_object = 0;
					pp->tick = 0;
				}
				else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_DOWN].held)
				{
					pp->state = PP2_CHARACTER_STATE_DUCK_R_R;
					pp->current_object = 1;
					pp->tick = 0;
				}
				break;
			}
			case PP2_CHARACTER_STATE_STAND_L_L:
			{
				if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_JUMP].pressed)
				{
					pp->state = PP2_CHARACTER_STATE_JUMP_L_L;
					pp->vy = -15.0;
					if(pp->flags & PP2_PLAYER_FLAG_GROUND)
					{
						pp->flags ^= PP2_PLAYER_FLAG_GROUND;
					}
					pp->current_object = 0;
					pp->tick = 0;
					pp2_play_sample(gp, pp->character->sample[PP2_SAMPLE_JUMP], pp->x + pp->object[0]->map.bottom.point[0].x, pp->y + pp->object[0]->map.bottom.point[0].y, 1.0, 1.0);
				}
				else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_RIGHT].held)
				{
					if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_STRAFE].held)
					{
						pp->state = PP2_CHARACTER_STATE_WALK_L_L;
						pp->current_object = 0;
					}
					else
					{
						pp->state = PP2_CHARACTER_STATE_WALK_R_R;
						pp->current_object = 0;
						pp->tick = 0;
					}
				}
				else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_LEFT].held)
				{
					pp->state = PP2_CHARACTER_STATE_WALK_L_L;
					pp->current_object = 0;
					pp->tick = 0;
				}
				else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_UP].held)
				{
					pp->state = PP2_CHARACTER_STATE_STAND_L_U;
					pp->current_object = 0;
					pp->tick = 0;
				}
				else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_DOWN].held)
				{
					pp->state = PP2_CHARACTER_STATE_DUCK_L_L;
					pp->current_object = 1;
					pp->tick = 0;
				}
				break;
			}
			case PP2_CHARACTER_STATE_STAND_R_U:
			{
				if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_JUMP].pressed)
				{
					pp->state = PP2_CHARACTER_STATE_JUMP_R_R;
					pp->vy = -15.0;
					if(pp->flags & PP2_PLAYER_FLAG_GROUND)
					{
						pp->flags ^= PP2_PLAYER_FLAG_GROUND;
					}
					pp->tick = 0;
					pp->current_object = 0;
					pp2_play_sample(gp, pp->character->sample[PP2_SAMPLE_JUMP], pp->x + pp->object[0]->map.bottom.point[0].x, pp->y + pp->object[0]->map.bottom.point[0].y, 1.0, 1.0);
				}
				else if(!pp2_controller[pp->controller]->state[PP2_CONTROLLER_UP].held)
				{
					pp->state = PP2_CHARACTER_STATE_STAND_R_R;
					pp->current_object = 0;
					pp->tick = 0;
				}
				break;
			}
			case PP2_CHARACTER_STATE_STAND_L_U:
			{
				if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_JUMP].pressed)
				{
					pp->state = PP2_CHARACTER_STATE_JUMP_L_L;
					pp->vy = -15.0;
					if(pp->flags & PP2_PLAYER_FLAG_GROUND)
					{
						pp->flags ^= PP2_PLAYER_FLAG_GROUND;
					}
					pp->current_object = 0;
					pp->tick = 0;
					pp2_play_sample(gp, pp->character->sample[PP2_SAMPLE_JUMP], pp->x + pp->object[0]->map.bottom.point[0].x, pp->y + pp->object[0]->map.bottom.point[0].y, 1.0, 1.0);
				}
				else if(!pp2_controller[pp->controller]->state[PP2_CONTROLLER_UP].held)
				{
					pp->state = PP2_CHARACTER_STATE_STAND_L_L;
					pp->current_object = 0;
					pp->tick = 0;
				}
				break;
			}
			case PP2_CHARACTER_STATE_WALK_R_R:
			{
				if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_JUMP].pressed)
				{
					pp->state = PP2_CHARACTER_STATE_JUMP_R_R;
					pp->vy = -15.0;
					if(pp->flags & PP2_PLAYER_FLAG_GROUND)
					{
						pp->flags ^= PP2_PLAYER_FLAG_GROUND;
					}
					pp->current_object = 0;
					pp->tick = 0;
					pp2_play_sample(gp, pp->character->sample[PP2_SAMPLE_JUMP], pp->x + pp->object[0]->map.bottom.point[0].x, pp->y + pp->object[0]->map.bottom.point[0].y, 1.0, 1.0);
				}
				else if(!pp2_controller[pp->controller]->state[PP2_CONTROLLER_RIGHT].held)
				{
					if(!pp2_controller[pp->controller]->state[PP2_CONTROLLER_STRAFE].held)
					{
						pp->state = PP2_CHARACTER_STATE_STAND_R_R;
						pp->current_object = 0;
						pp->tick = 0;
					}
				}
				else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_LEFT].held)
				{
					if(!pp2_controller[pp->controller]->state[PP2_CONTROLLER_STRAFE].held)
					{
						pp->state = PP2_CHARACTER_STATE_WALK_L_L;
						pp->current_object = 0;
						pp->tick = 0;
					}
				}
				else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_DOWN].held)
				{
					pp->state = PP2_CHARACTER_STATE_DUCK_R_R;
					pp->current_object = 1;
					pp->tick = 0;
				}
				else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_UP].held)
				{
					pp->state = PP2_CHARACTER_STATE_STAND_R_U;
					pp->current_object = 0;
					pp->tick = 0;
				}
				break;
			}
			case PP2_CHARACTER_STATE_WALK_L_L:
			{
				if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_JUMP].pressed)
				{
					pp->state = PP2_CHARACTER_STATE_JUMP_L_L;
					pp->vy = -15.0;
					if(pp->flags & PP2_PLAYER_FLAG_GROUND)
					{
						pp->flags ^= PP2_PLAYER_FLAG_GROUND;
					}
					pp->current_object = 0;
					pp->tick = 0;
					pp2_play_sample(gp, pp->character->sample[PP2_SAMPLE_JUMP], pp->x + pp->object[0]->map.bottom.point[0].x, pp->y + pp->object[0]->map.bottom.point[0].y, 1.0, 1.0);
				}
				else if(!pp2_controller[pp->controller]->state[PP2_CONTROLLER_LEFT].held)
				{
					if(!pp2_controller[pp->controller]->state[PP2_CONTROLLER_STRAFE].held)
					{
						pp->state = PP2_CHARACTER_STATE_STAND_L_L;
						pp->current_object = 0;
						pp->tick = 0;
					}
				}
				else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_RIGHT].held)
				{
					if(!pp2_controller[pp->controller]->state[PP2_CONTROLLER_STRAFE].held)
					{
						pp->state = PP2_CHARACTER_STATE_WALK_R_R;
						pp->current_object = 0;
						pp->tick = 0;
					}
				}
				else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_DOWN].held)
				{
					pp->state = PP2_CHARACTER_STATE_DUCK_L_L;
					pp->current_object = 1;
					pp->tick = 0;
				}
				else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_UP].held)
				{
					pp->state = PP2_CHARACTER_STATE_STAND_L_U;
					pp->current_object = 0;
					pp->tick = 0;
				}
				break;
			}
			case PP2_CHARACTER_STATE_JUMP_R_R:
			{
				if(pp->vy > 0.0)
				{
					pp->state = PP2_CHARACTER_STATE_FALL_R_R;
					pp->current_object = 0;
					pp->tick = 0;
				}
				else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_LEFT].held && !pp2_controller[pp->controller]->state[PP2_CONTROLLER_STRAFE].held)
				{
					pp->state = PP2_CHARACTER_STATE_JUMP_L_L;
					pp->current_object = 0;
				}
				break;
			}
			case PP2_CHARACTER_STATE_JUMP_L_L:
			{
				if(pp->vy > 0.0)
				{
					pp->state = PP2_CHARACTER_STATE_FALL_L_L;
					pp->current_object = 0;
					pp->tick = 0;
				}
				else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_RIGHT].held && !pp2_controller[pp->controller]->state[PP2_CONTROLLER_STRAFE].held)
				{
					pp->state = PP2_CHARACTER_STATE_JUMP_R_R;
					pp->current_object = 0;
				}
				break;
			}
			case PP2_CHARACTER_STATE_FALL_R_R:
			{
				if(pp->vy < 0.0)
				{
					pp->state = PP2_CHARACTER_STATE_JUMP_R_R;
					pp->current_object = 0;
					pp->tick = 0;
				}
				else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_LEFT].held && !pp2_controller[pp->controller]->state[PP2_CONTROLLER_STRAFE].held)
				{
					pp->state = PP2_CHARACTER_STATE_FALL_L_L;
					pp->current_object = 0;
				}
				break;
			}
			case PP2_CHARACTER_STATE_FALL_L_L:
			{
				if(pp->vy < 0.0)
				{
					pp->state = PP2_CHARACTER_STATE_JUMP_L_L;
					pp->current_object = 0;
					pp->tick = 0;
				}
				else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_RIGHT].held && !pp2_controller[pp->controller]->state[PP2_CONTROLLER_STRAFE].held)
				{
					pp->state = PP2_CHARACTER_STATE_FALL_R_R;
					pp->current_object = 0;
				}
				break;
			}
			case PP2_CHARACTER_STATE_DUCK_R_R:
			{
				if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_JUMP].pressed && pp2_player_partial_floor(gp, pp) && !(gp->level->flags & PP2_LEVEL_FLAG_LEGACY))
				{
					pp->state = PP2_CHARACTER_STATE_FALL_R_R;
					pp->vy = 0.0;
					if(pp->flags & PP2_PLAYER_FLAG_GROUND)
					{
						pp->flags ^= PP2_PLAYER_FLAG_GROUND;
					}
					pp->y += 1.0;
					pp2_player_move_object_y(pp);
					pp2_player_move_object_y(pp);
					pp->current_object = 0;
					pp->tick = 0;
					pp->jumped_down = true;
					pp2_play_sample(gp, pp->character->sample[PP2_SAMPLE_JUMP], pp->x + pp->object[0]->map.bottom.point[0].x, pp->y + pp->object[0]->map.bottom.point[0].y, 1.0, 1.0);
				}
				else if(!pp2_controller[pp->controller]->state[PP2_CONTROLLER_DOWN].held && pp->timer[PP2_PLAYER_TIMER_STOMPED] == 0)
				{
					pp->state = PP2_CHARACTER_STATE_STAND_R_R;
					pp->current_object = 0;
					pp->tick = 0;
				}
				break;
			}
			case PP2_CHARACTER_STATE_DUCK_L_L:
			{
				if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_JUMP].pressed && pp2_player_partial_floor(gp, pp) && !(gp->level->flags & PP2_LEVEL_FLAG_LEGACY))
				{
					pp->state = PP2_CHARACTER_STATE_FALL_L_L;
					pp->vy = 0.0;
					if(pp->flags & PP2_PLAYER_FLAG_GROUND)
					{
						pp->flags ^= PP2_PLAYER_FLAG_GROUND;
					}
					pp->y += 1.0;
					pp2_player_move_object_y(pp);
					pp2_player_move_object_y(pp);
					pp->current_object = 0;
					pp->tick = 0;
					pp->jumped_down = true;
					pp2_play_sample(gp, pp->character->sample[PP2_SAMPLE_JUMP], pp->x + pp->object[0]->map.bottom.point[0].x, pp->y + pp->object[0]->map.bottom.point[0].y, 1.0, 1.0);
				}
				else if(!pp2_controller[pp->controller]->state[PP2_CONTROLLER_DOWN].held && pp->timer[PP2_PLAYER_TIMER_STOMPED] == 0)
				{
					pp->state = PP2_CHARACTER_STATE_STAND_L_L;
					pp->current_object = 0;
					pp->tick = 0;
				}
				break;
			}
		}
	}
	if(!pp2_controller[pp->controller]->state[PP2_CONTROLLER_JUMP].held)
	{
		pp->jumped_down = false;
	}
	if(pp2_player_slip(gp, pp))
	{
		friction = false;
	}
	else if(!pp2_controller[pp->controller]->state[PP2_CONTROLLER_LEFT].held && !pp2_controller[pp->controller]->state[PP2_CONTROLLER_RIGHT].held)
	{
		friction = true;
	}
	else if((pp->state >= PP2_CHARACTER_STATE_STAND_R_R && pp->state <= PP2_CHARACTER_STATE_STAND_R_UR) || (pp->state >= PP2_CHARACTER_STATE_STAND_L_R && pp->state <= PP2_CHARACTER_STATE_STAND_L_UR) || (pp->state >= PP2_CHARACTER_STATE_DUCK_R_R && pp->state <= PP2_CHARACTER_STATE_DUCK_R_UR) || (pp->state >= PP2_CHARACTER_STATE_DUCK_L_R && pp->state <= PP2_CHARACTER_STATE_DUCK_L_UR))
	{
		friction = true;
	}
	else if(pp->choose_weapon)
	{
		friction = true;
	}

	/* adjust velocities (accelerate) */
	if(pp->flags & PP2_PLAYER_FLAG_POWER_FLY)
	{
		if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_JUMP].held && !pp->jumped_down)
		{
			pp2_generate_fly_particle(gp, pp, resources);
			pp->vy -= 1.0;
  		if(pp->tick % 6 == 0)
  		{
				pp2_play_sample(gp, pp->character->sample[PP2_SAMPLE_FLY], pp->x + pp->object[0]->map.bottom.point[0].x, pp->y + pp->object[0]->map.bottom.point[0].y, 1.0, 1.0);
  		}
 		}
 		else
 		{
   		if(!(pp->flags & PP2_PLAYER_FLAG_GROUND))
   		{
				pp->vy += 1.0;
			}
   	}
		if(pp->vy < -10.0)
		{
			pp->vy = -10.0;
		}
		if(pp->vy > 14.0)
		{
			pp->vy = 14.0;
		}
	}
	else if(!(pp->flags & PP2_PLAYER_FLAG_GROUND))
	{
		if(pp->vy >= 0.0 || pp2_controller[pp->controller]->state[PP2_CONTROLLER_JUMP].held)
		{
			if(pp->flags & PP2_PLAYER_FLAG_POWER_JUMP)
			{
				pp->vy += 0.8;
			}
			else
			{
				pp->vy += 1.0;
			}
		}
		else
		{
			pp->vy += 2.0;
		}
		if(pp->vy > 14.0)
		{
			pp->vy = 14.0;
		}
	}
	if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_RIGHT].held)
	{
		if(!(pp->state >= PP2_CHARACTER_STATE_STAND_R_R && pp->state <= PP2_CHARACTER_STATE_STAND_R_UR) && !(pp->state >= PP2_CHARACTER_STATE_STAND_L_R && pp->state <= PP2_CHARACTER_STATE_STAND_L_UR) && !(pp->state >= PP2_CHARACTER_STATE_DUCK_R_R && pp->state <= PP2_CHARACTER_STATE_DUCK_R_UR) && !(pp->state >= PP2_CHARACTER_STATE_DUCK_L_R && pp->state <= PP2_CHARACTER_STATE_DUCK_L_UR) && !pp->choose_weapon)
		{
			if(pp->vx < mx)
			{
				pp->fvx = 1.0;
				pp->vx += 1.0;
				if(pp->vx > mx)
				{
					pp->vx = mx;
				}
			}
		}
	}
	else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_LEFT].held)
	{
		if(!(pp->state >= PP2_CHARACTER_STATE_STAND_R_R && pp->state <= PP2_CHARACTER_STATE_STAND_R_UR) && !(pp->state >= PP2_CHARACTER_STATE_STAND_L_R && pp->state <= PP2_CHARACTER_STATE_STAND_L_UR) && !(pp->state >= PP2_CHARACTER_STATE_DUCK_R_R && pp->state <= PP2_CHARACTER_STATE_DUCK_R_UR) && !(pp->state >= PP2_CHARACTER_STATE_DUCK_L_R && pp->state <= PP2_CHARACTER_STATE_DUCK_L_UR) && !pp->choose_weapon)
		{
			if(pp->vx > -mx)
			{
				pp->fvx = -1.0;
				pp->vx -= 1.0;
				if(pp->vx < -mx)
				{
					pp->vx = -mx;
				}
			}
		}
	}
	else
	{
		pp->fvx = 0.0;
	}
	if(friction)
	{
		bool stopped = false;
		if(pp->vx < 0.0)
		{
			pp->vx += 0.5;
			if(pp->vx >= 0.0)
			{
				pp->vx = 0.0;
				stopped = true;
			}
		}
		else if(pp->vx > 0.0)
		{
			pp->vx -= 0.5;
			if(pp->vx <= 0.0)
			{
				pp->vx = 0.0;
				stopped = true;
			}
		}
		if(stopped)
		{
			pp2_player_stop(pp);
		}
	}
}

void pp2_player_strafe_logic(PP2_PLAYER * pp)
{
	if((pp->state >= PP2_CHARACTER_STATE_STAND_R_R && pp->state <= PP2_CHARACTER_STATE_STAND_L_UR) || (pp->state >= PP2_CHARACTER_STATE_WALK_R_R && pp->state <= PP2_CHARACTER_STATE_WALK_L_UR) || (pp->state >= PP2_CHARACTER_STATE_DUCK_R_R && pp->state <= PP2_CHARACTER_STATE_DUCK_L_UR))
	{
		if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_DOWN].held)
		{
			pp2_player_change_state(pp, (pp->state % 16) + PP2_CHARACTER_STATE_DUCK_R_R);
			pp->current_object = 1;
		}
		else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_LEFT].held)
		{
			pp2_player_change_state(pp, (pp->state % 8) + PP2_CHARACTER_STATE_WALK_L_R);
			pp->current_object = 0;
		}
		else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_RIGHT].held)
		{
			pp2_player_change_state(pp, (pp->state % 8) + PP2_CHARACTER_STATE_WALK_R_R);
			pp->current_object = 0;
		}
		else
		{
			pp2_player_change_state(pp, (pp->state % 16) + PP2_CHARACTER_STATE_STAND_R_R);
			pp->current_object = 0;
		}
	}
	else if((pp->state >= PP2_CHARACTER_STATE_JUMP_R_R && pp->state <= PP2_CHARACTER_STATE_JUMP_L_UR))
	{
		if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_LEFT].held)
		{
			pp2_player_change_state(pp, (pp->state % 8) + PP2_CHARACTER_STATE_JUMP_L_R);
			pp->current_object = 0;
		}
		else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_RIGHT].held)
		{
			pp2_player_change_state(pp, (pp->state % 8) + PP2_CHARACTER_STATE_JUMP_R_R);
			pp->current_object = 0;
		}
	}
	else if((pp->state >= PP2_CHARACTER_STATE_FALL_R_R && pp->state <= PP2_CHARACTER_STATE_FALL_L_UR))
	{
		if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_LEFT].held)
		{
			pp2_player_change_state(pp, (pp->state % 8) + PP2_CHARACTER_STATE_FALL_L_R);
			pp->current_object = 0;
		}
		else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_RIGHT].held)
		{
			pp2_player_change_state(pp, (pp->state % 8) + PP2_CHARACTER_STATE_FALL_R_R);
			pp->current_object = 0;
		}
	}
}

void pp2_player_rules(PP2_PLAYER * pp)
{
	switch(pp2_option[PP2_OPTION_GAME_MODE])
	{
		/* when you are eliminated you are removed from the game */
		case PP2_GAME_MODE_ELIMINATOR:
		{
			if(pp->life <= 0 && pp->fade_time == 0)
			{
				pp->fade_time = 30;
				pp->fade_type = 0;
			}
			break;
		}
		case PP2_GAME_MODE_DEATH_MATCH:
		case PP2_GAME_MODE_COIN_RUSH:
		{
			if(pp->life <= 0 && pp->fade_time == 0)
			{
				pp->fade_time = 30;
				pp->fade_type = 0;
			}
			break;
		}
	}
}

void pp2_player_logic(PP2_GAME * gp, PP2_PLAYER * pp, PP2_RESOURCES * resources)
{
	int old_target;

	if(!(pp->flags & PP2_PLAYER_FLAG_ACTIVE))
	{
		return;
	}

	pp2_player_rules(pp);

	/* fading out */
	if(pp->fade_type == 0)
	{
		if(pp->fade_time > 0)
		{
			pp->fade_time--;
			if(pp->fade_time <= 0)
			{
				if(pp2_option[PP2_OPTION_GAME_MODE] == PP2_GAME_MODE_DEATH_MATCH || pp2_option[PP2_OPTION_GAME_MODE] == PP2_GAME_MODE_COIN_RUSH)
				{
					pp->flags = 0;
					if(pp2_winner < 0)
					{
						pp2_game_spawn_player(gp, pp, resources);
					}
				}
				else
				{
					pp->flags = 0;
				}
			}
			return;
		}
	}

	/* fading in */
	else
	{
		if(pp->fade_time < 30)
		{
			pp->fade_time++;
			if(pp->fade_time >= 30)
			{
				pp->fade_time = 0;
				pp->fade_type = 0;
			}
			return;
		}
	}

	/* run the legacy logic for older characters */
	if(pp->character->flags & PP2_CHARACTER_FLAG_LEGACY)
	{
		pp2_legacy_player_logic(gp, pp, resources);
	}
	else
	{
		bool friction = false;
		float mx = 4.0;

		if(pp->flags & PP2_PLAYER_FLAG_POWER_RUN)
		{
			mx = 5.0;
		}

		if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_STRAFE].held)
		{
			pp2_player_strafe_logic(pp);
		}
		else if(pp->choose_weapon)
		{

		}
		else
		{
			/* adjust state */
			switch(pp->state)
			{
				case PP2_CHARACTER_STATE_STAND_R_R:
				case PP2_CHARACTER_STATE_STAND_R_UR:
				case PP2_CHARACTER_STATE_STAND_R_DR:
				case PP2_CHARACTER_STATE_STAND_R_U:
				case PP2_CHARACTER_STATE_STAND_R_D:
				case PP2_CHARACTER_STATE_STAND_R_L:
				case PP2_CHARACTER_STATE_STAND_R_UL:
				case PP2_CHARACTER_STATE_STAND_R_DL:
				case PP2_CHARACTER_STATE_STAND_L_R:
				case PP2_CHARACTER_STATE_STAND_L_UR:
				case PP2_CHARACTER_STATE_STAND_L_DR:
				case PP2_CHARACTER_STATE_STAND_L_U:
				case PP2_CHARACTER_STATE_STAND_L_D:
				case PP2_CHARACTER_STATE_STAND_L_L:
				case PP2_CHARACTER_STATE_STAND_L_UL:
				case PP2_CHARACTER_STATE_STAND_L_DL:
				case PP2_CHARACTER_STATE_WALK_R_R:
				case PP2_CHARACTER_STATE_WALK_R_UR:
				case PP2_CHARACTER_STATE_WALK_R_DR:
				case PP2_CHARACTER_STATE_WALK_R_U:
				case PP2_CHARACTER_STATE_WALK_R_D:
				case PP2_CHARACTER_STATE_WALK_R_L:
				case PP2_CHARACTER_STATE_WALK_R_UL:
				case PP2_CHARACTER_STATE_WALK_R_DL:
				case PP2_CHARACTER_STATE_WALK_L_R:
				case PP2_CHARACTER_STATE_WALK_L_UR:
				case PP2_CHARACTER_STATE_WALK_L_DR:
				case PP2_CHARACTER_STATE_WALK_L_U:
				case PP2_CHARACTER_STATE_WALK_L_D:
				case PP2_CHARACTER_STATE_WALK_L_L:
				case PP2_CHARACTER_STATE_WALK_L_UL:
				case PP2_CHARACTER_STATE_WALK_L_DL:
				{
					if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_LEFT].held && pp2_controller[pp->controller]->state[PP2_CONTROLLER_UP].held)
					{
						pp2_player_change_state(pp, PP2_CHARACTER_STATE_WALK_L_UL);
						pp->current_object = 0;
					}
					else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_RIGHT].held && pp2_controller[pp->controller]->state[PP2_CONTROLLER_UP].held)
					{
						pp2_player_change_state(pp, PP2_CHARACTER_STATE_WALK_R_UR);
						pp->current_object = 0;
					}
					else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_LEFT].held && pp2_controller[pp->controller]->state[PP2_CONTROLLER_DOWN].held)
					{
						pp2_player_change_state(pp, PP2_CHARACTER_STATE_WALK_L_DL);
						pp->current_object = 0;
					}
					else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_RIGHT].held && pp2_controller[pp->controller]->state[PP2_CONTROLLER_DOWN].held)
					{
						pp2_player_change_state(pp, PP2_CHARACTER_STATE_WALK_R_DR);
						pp->current_object = 0;
					}
					else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_LEFT].held)
					{
						pp2_player_change_state(pp, PP2_CHARACTER_STATE_WALK_L_L);
						pp->current_object = 0;
					}
					else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_RIGHT].held)
					{
						pp2_player_change_state(pp, PP2_CHARACTER_STATE_WALK_R_R);
						pp->current_object = 0;
					}
					else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_UP].held)
					{
						if(pp->state % 16 < 8)
						{
							pp2_player_change_state(pp, PP2_CHARACTER_STATE_STAND_R_U);
						}
						else
						{
							pp2_player_change_state(pp, PP2_CHARACTER_STATE_STAND_L_U);
						}
						pp->current_object = 0;
					}
					else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_DOWN].held)
					{
						if(pp->state % 16 < 8)
						{
							pp2_player_change_state(pp, PP2_CHARACTER_STATE_DUCK_R_R);
						}
						else
						{
							pp2_player_change_state(pp, PP2_CHARACTER_STATE_DUCK_L_L);
						}
						pp->current_object = 1;
					}
					else
					{
						if(pp->state % 16 < 8)
						{
							pp2_player_change_state(pp, PP2_CHARACTER_STATE_STAND_R_R);
						}
						else
						{
							pp2_player_change_state(pp, PP2_CHARACTER_STATE_STAND_L_L);
						}
					}
					break;
				}
				case PP2_CHARACTER_STATE_JUMP_R_R:
				case PP2_CHARACTER_STATE_JUMP_R_UR:
				case PP2_CHARACTER_STATE_JUMP_R_DR:
				case PP2_CHARACTER_STATE_JUMP_R_U:
				case PP2_CHARACTER_STATE_JUMP_R_D:
				case PP2_CHARACTER_STATE_JUMP_R_L:
				case PP2_CHARACTER_STATE_JUMP_R_UL:
				case PP2_CHARACTER_STATE_JUMP_R_DL:
				case PP2_CHARACTER_STATE_JUMP_L_R:
				case PP2_CHARACTER_STATE_JUMP_L_UR:
				case PP2_CHARACTER_STATE_JUMP_L_DR:
				case PP2_CHARACTER_STATE_JUMP_L_U:
				case PP2_CHARACTER_STATE_JUMP_L_D:
				case PP2_CHARACTER_STATE_JUMP_L_L:
				case PP2_CHARACTER_STATE_JUMP_L_UL:
				case PP2_CHARACTER_STATE_JUMP_L_DL:
				{
					if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_LEFT].held && pp2_controller[pp->controller]->state[PP2_CONTROLLER_UP].held)
					{
						pp2_player_change_state(pp, PP2_CHARACTER_STATE_JUMP_L_UL);
						pp->current_object = 0;
					}
					else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_RIGHT].held && pp2_controller[pp->controller]->state[PP2_CONTROLLER_UP].held)
					{
						pp2_player_change_state(pp, PP2_CHARACTER_STATE_JUMP_R_UR);
						pp->current_object = 0;
					}
					else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_LEFT].held && pp2_controller[pp->controller]->state[PP2_CONTROLLER_DOWN].held)
					{
						pp2_player_change_state(pp, PP2_CHARACTER_STATE_JUMP_L_DL);
						pp->current_object = 0;
					}
					else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_RIGHT].held && pp2_controller[pp->controller]->state[PP2_CONTROLLER_DOWN].held)
					{
						pp2_player_change_state(pp, PP2_CHARACTER_STATE_JUMP_R_DR);
						pp->current_object = 0;
					}
					else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_LEFT].held)
					{
						pp2_player_change_state(pp, PP2_CHARACTER_STATE_JUMP_L_L);
						pp->current_object = 0;
					}
					else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_RIGHT].held)
					{
						pp2_player_change_state(pp, PP2_CHARACTER_STATE_JUMP_R_R);
						pp->current_object = 0;
					}
					else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_UP].held)
					{
						if(pp->state >= PP2_CHARACTER_STATE_JUMP_R_R && pp->state <= PP2_CHARACTER_STATE_JUMP_R_UR)
						{
							pp2_player_change_state(pp, PP2_CHARACTER_STATE_JUMP_R_U);
						}
						else
						{
							pp2_player_change_state(pp, PP2_CHARACTER_STATE_JUMP_L_U);
						}
						pp->current_object = 0;
					}
					else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_DOWN].held)
					{
						if(pp->state >= PP2_CHARACTER_STATE_JUMP_R_R && pp->state <= PP2_CHARACTER_STATE_JUMP_R_UR)
						{
							pp2_player_change_state(pp, PP2_CHARACTER_STATE_JUMP_R_D);
						}
						else
						{
							pp2_player_change_state(pp, PP2_CHARACTER_STATE_JUMP_L_D);
						}
						pp->current_object = 1;
					}
					else
					{
						if(pp->state >= PP2_CHARACTER_STATE_JUMP_R_R && pp->state <= PP2_CHARACTER_STATE_JUMP_R_UR)
						{
							pp2_player_change_state(pp, PP2_CHARACTER_STATE_JUMP_R_R);
						}
						else
						{
							pp2_player_change_state(pp, PP2_CHARACTER_STATE_JUMP_L_L);
						}
					}

					/* if we started falling, change states */
					if(pp->vy >= 0.0)
					{
						pp2_player_change_state(pp, PP2_CHARACTER_STATE_FALL_R_R + pp->state % 16);
					}
					break;
				}
				case PP2_CHARACTER_STATE_FALL_R_R:
				case PP2_CHARACTER_STATE_FALL_R_UR:
				case PP2_CHARACTER_STATE_FALL_R_DR:
				case PP2_CHARACTER_STATE_FALL_R_U:
				case PP2_CHARACTER_STATE_FALL_R_D:
				case PP2_CHARACTER_STATE_FALL_R_L:
				case PP2_CHARACTER_STATE_FALL_R_UL:
				case PP2_CHARACTER_STATE_FALL_R_DL:
				case PP2_CHARACTER_STATE_FALL_L_R:
				case PP2_CHARACTER_STATE_FALL_L_UR:
				case PP2_CHARACTER_STATE_FALL_L_DR:
				case PP2_CHARACTER_STATE_FALL_L_U:
				case PP2_CHARACTER_STATE_FALL_L_D:
				case PP2_CHARACTER_STATE_FALL_L_L:
				case PP2_CHARACTER_STATE_FALL_L_UL:
				case PP2_CHARACTER_STATE_FALL_L_DL:
				{
					if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_LEFT].held && pp2_controller[pp->controller]->state[PP2_CONTROLLER_UP].held)
					{
						pp2_player_change_state(pp, PP2_CHARACTER_STATE_FALL_L_UL);
						pp->current_object = 0;
					}
					else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_RIGHT].held && pp2_controller[pp->controller]->state[PP2_CONTROLLER_UP].held)
					{
						pp2_player_change_state(pp, PP2_CHARACTER_STATE_FALL_R_UR);
						pp->current_object = 0;
					}
					else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_LEFT].held && pp2_controller[pp->controller]->state[PP2_CONTROLLER_DOWN].held)
					{
						pp2_player_change_state(pp, PP2_CHARACTER_STATE_FALL_L_DL);
						pp->current_object = 0;
					}
					else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_RIGHT].held && pp2_controller[pp->controller]->state[PP2_CONTROLLER_DOWN].held)
					{
						pp2_player_change_state(pp, PP2_CHARACTER_STATE_FALL_R_DR);
						pp->current_object = 0;
					}
					else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_LEFT].held)
					{
						pp2_player_change_state(pp, PP2_CHARACTER_STATE_FALL_L_L);
						pp->current_object = 0;
					}
					else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_RIGHT].held)
					{
						pp2_player_change_state(pp, PP2_CHARACTER_STATE_FALL_R_R);
						pp->current_object = 0;
					}
					else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_UP].held)
					{
						if(pp->state >= PP2_CHARACTER_STATE_JUMP_R_R && pp->state <= PP2_CHARACTER_STATE_FALL_R_UR)
						{
							pp2_player_change_state(pp, PP2_CHARACTER_STATE_FALL_R_U);
						}
						else
						{
							pp2_player_change_state(pp, PP2_CHARACTER_STATE_FALL_L_U);
						}
						pp->current_object = 0;
					}
					else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_DOWN].held)
					{
						if(pp->state >= PP2_CHARACTER_STATE_FALL_R_R && pp->state <= PP2_CHARACTER_STATE_FALL_R_UR)
						{
							pp2_player_change_state(pp, PP2_CHARACTER_STATE_FALL_R_D);
						}
						else
						{
							pp2_player_change_state(pp, PP2_CHARACTER_STATE_FALL_L_D);
						}
						pp->current_object = 1;
					}
					else
					{
						if(pp->state >= PP2_CHARACTER_STATE_FALL_R_R && pp->state <= PP2_CHARACTER_STATE_FALL_R_UR)
						{
							pp2_player_change_state(pp, PP2_CHARACTER_STATE_FALL_R_R);
						}
						else
						{
							pp2_player_change_state(pp, PP2_CHARACTER_STATE_FALL_L_L);
						}
					}
					/* if we are going up, change states */
					if(pp->vy < 0.0)
					{
						pp2_player_change_state(pp, PP2_CHARACTER_STATE_JUMP_R_R + pp->state % 16);
					}
					break;
				}
				case PP2_CHARACTER_STATE_DUCK_R_R:
				case PP2_CHARACTER_STATE_DUCK_R_UR:
				case PP2_CHARACTER_STATE_DUCK_R_DR:
				case PP2_CHARACTER_STATE_DUCK_R_U:
				case PP2_CHARACTER_STATE_DUCK_R_D:
				case PP2_CHARACTER_STATE_DUCK_R_L:
				case PP2_CHARACTER_STATE_DUCK_R_UL:
				case PP2_CHARACTER_STATE_DUCK_R_DL:
				case PP2_CHARACTER_STATE_DUCK_L_R:
				case PP2_CHARACTER_STATE_DUCK_L_UR:
				case PP2_CHARACTER_STATE_DUCK_L_DR:
				case PP2_CHARACTER_STATE_DUCK_L_U:
				case PP2_CHARACTER_STATE_DUCK_L_D:
				case PP2_CHARACTER_STATE_DUCK_L_L:
				case PP2_CHARACTER_STATE_DUCK_L_UL:
				case PP2_CHARACTER_STATE_DUCK_L_DL:
				{
					if(pp->timer[PP2_PLAYER_TIMER_STOMPED] == 0)
					{
						if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_LEFT].held && pp2_controller[pp->controller]->state[PP2_CONTROLLER_UP].held)
						{
							pp2_player_change_state(pp, PP2_CHARACTER_STATE_WALK_L_UL);
							pp->current_object = 0;
						}
						else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_RIGHT].held && pp2_controller[pp->controller]->state[PP2_CONTROLLER_UP].held)
						{
							pp2_player_change_state(pp, PP2_CHARACTER_STATE_WALK_R_UR);
							pp->current_object = 0;
						}
						else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_LEFT].held && pp2_controller[pp->controller]->state[PP2_CONTROLLER_DOWN].held)
						{
							pp2_player_change_state(pp, PP2_CHARACTER_STATE_DUCK_L_DL);
							pp->current_object = 0;
						}
						else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_RIGHT].held && pp2_controller[pp->controller]->state[PP2_CONTROLLER_DOWN].held)
						{
							pp2_player_change_state(pp, PP2_CHARACTER_STATE_DUCK_R_DR);
							pp->current_object = 0;
						}
						else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_LEFT].held)
						{
							pp2_player_change_state(pp, PP2_CHARACTER_STATE_WALK_L_L);
							pp->current_object = 0;
						}
						else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_RIGHT].held)
						{
							pp2_player_change_state(pp, PP2_CHARACTER_STATE_WALK_R_R);
							pp->current_object = 0;
						}
						else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_UP].held)
						{
							if(pp->state % 16 < 8)
							{
								pp2_player_change_state(pp, PP2_CHARACTER_STATE_STAND_R_U);
							}
							else
							{
								pp2_player_change_state(pp, PP2_CHARACTER_STATE_STAND_L_U);
							}
							pp->current_object = 0;
						}
						else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_DOWN].held)
						{
							if(pp->state % 16 < 8)
							{
								pp2_player_change_state(pp, PP2_CHARACTER_STATE_DUCK_R_R);
							}
							else
							{
								pp2_player_change_state(pp, PP2_CHARACTER_STATE_DUCK_L_L);
							}
							pp->current_object = 1;
						}
						else
						{
							if(pp->state % 16 < 8)
							{
								pp2_player_change_state(pp, PP2_CHARACTER_STATE_STAND_R_R);
							}
							else
							{
								pp2_player_change_state(pp, PP2_CHARACTER_STATE_STAND_L_L);
							}
						}
					}
					break;
				}
			}
		}

		/* player wants to jump */
		if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_JUMP].pressed)
		{

			/* jump down a level if pressing down */
			if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_DOWN].held && pp2_player_partial_floor(gp, pp) && !(gp->level->flags & PP2_LEVEL_FLAG_LEGACY))
			{
				pp2_player_change_state(pp, (pp->state % 16) + PP2_CHARACTER_STATE_FALL_R_R);
				pp->vy = 0.0;
				if(pp->flags & PP2_PLAYER_FLAG_GROUND)
				{
					pp->flags ^= PP2_PLAYER_FLAG_GROUND;
				}
				pp->y += 1.0;
				pp2_player_move_object_y(pp);
				pp2_player_move_object_y(pp);
				pp->current_object = 0;
				pp->jumped_down = true;
				pp2_play_sample(gp, pp->character->sample[PP2_SAMPLE_JUMP], pp->x + pp->object[0]->map.bottom.point[0].x, pp->y + pp->object[0]->map.bottom.point[0].y, 1.0, 1.0);
			}

			/* otherwise jump up */
			else if(pp->flags & PP2_PLAYER_FLAG_GROUND)
			{
				pp2_player_change_state(pp, (pp->state % 16) + PP2_CHARACTER_STATE_JUMP_R_R);
				pp->vy = -15.0;
				if(pp->flags & PP2_PLAYER_FLAG_GROUND)
				{
					pp->flags ^= PP2_PLAYER_FLAG_GROUND;
				}
				pp->current_object = 0;
				pp2_play_sample(gp, pp->character->sample[PP2_SAMPLE_JUMP], pp->x + pp->object[0]->map.bottom.point[0].x, pp->y + pp->object[0]->map.bottom.point[0].y, 1.0, 1.0);
			}
		}

		/* see if player is going from jump to fall */
		if(pp->state >= PP2_CHARACTER_STATE_JUMP_R_R && pp->state <= PP2_CHARACTER_STATE_JUMP_L_UR)
		{
			if(pp->vy > 0.0)
			{
				pp2_player_change_state(pp, (pp->state % 16) + PP2_CHARACTER_STATE_FALL_R_R);
				pp->current_object = 0;
			}
		}

		if(!pp2_controller[pp->controller]->state[PP2_CONTROLLER_JUMP].held)
		{
			pp->jumped_down = false;
		}
		if(!pp2_controller[pp->controller]->state[PP2_CONTROLLER_LEFT].held && !pp2_controller[pp->controller]->state[PP2_CONTROLLER_RIGHT].held)
		{
			friction = true;
		}
		else if((pp->state >= PP2_CHARACTER_STATE_STAND_R_R && pp->state <= PP2_CHARACTER_STATE_STAND_R_UR) || (pp->state >= PP2_CHARACTER_STATE_STAND_L_R && pp->state <= PP2_CHARACTER_STATE_STAND_L_UR) || (pp->state >= PP2_CHARACTER_STATE_DUCK_R_R && pp->state <= PP2_CHARACTER_STATE_DUCK_R_UR) || (pp->state >= PP2_CHARACTER_STATE_DUCK_L_R && pp->state <= PP2_CHARACTER_STATE_DUCK_L_UR))
		{
			friction = true;
		}
		else if(pp->vx < -mx || pp->vx > mx)
		{
			friction = true;
		}

		/* adjust velocities (accelerate) */
		if(pp->flags & PP2_PLAYER_FLAG_POWER_FLY)
		{
			if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_JUMP].held && !pp->jumped_down)
			{
				pp2_generate_fly_particle(gp, pp, resources);
				pp->vy -= 1.0;
				if(pp->tick % 6 == 0)
				{
	//	        	generate_object(PP_OBJECT_JET, itofix(pp_game_data.player[i].cmap[pp_game_data.player[i].state].x + pp_game_data.player[i].cmap[pp_game_data.player[i].state].rx + pp_game_data.player[i].cmap[pp_game_data.player[i].state].bottom.point[0].x - pp_object_ani[PP_OBJECT_JET]->w / 2), itofix(pp_game_data.player[i].cmap[pp_game_data.player[i].state].y + pp_game_data.player[i].cmap[pp_game_data.player[i].state].ry + pp_game_data.player[i].cmap[pp_game_data.player[i].state].bottom.point[0].y - pp_object_ani[PP_OBJECT_JET]->h / 2));
					pp2_play_sample(gp, pp->character->sample[PP2_SAMPLE_FLY], pp->x + pp->object[0]->map.bottom.point[0].x, pp->y + pp->object[0]->map.bottom.point[0].y, 1.0, 1.0);
				}
			}
			else
			{
				if(!(pp->flags & PP2_PLAYER_FLAG_GROUND))
				{
					pp->vy += 1.0;
				}
			}
			if(pp->vy < -10.0)
			{
				pp->vy = -10.0;
			}
			if(pp->vy > 14.0)
			{
				pp->vy = 14.0;
			}
		}
		else if(!(pp->flags & PP2_PLAYER_FLAG_GROUND))
		{
			if(pp->vy >= 0.0 || pp2_controller[pp->controller]->state[PP2_CONTROLLER_JUMP].held)
			{
				if(pp->flags & PP2_PLAYER_FLAG_POWER_JUMP)
				{
					pp->vy += 0.8;
				}
				else
				{
					pp->vy += 1.0;
				}
			}
			else
			{
				pp->vy += 2.0;
			}
			if(pp->vy > 14.0)
			{
				pp->vy = 14.0;
			}
		}
		if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_RIGHT].held)
		{
			if(!(pp->state >= PP2_CHARACTER_STATE_STAND_R_R && pp->state <= PP2_CHARACTER_STATE_STAND_R_UR) && !(pp->state >= PP2_CHARACTER_STATE_STAND_L_R && pp->state <= PP2_CHARACTER_STATE_STAND_L_UR) && !(pp->state >= PP2_CHARACTER_STATE_DUCK_R_R && pp->state <= PP2_CHARACTER_STATE_DUCK_R_UR) && !(pp->state >= PP2_CHARACTER_STATE_DUCK_L_R && pp->state <= PP2_CHARACTER_STATE_DUCK_L_UR))
			{
				if(pp->vx < mx)
				{
					pp->fvx = 1.0;
					pp->vx += 1.0;
					if(pp->vx > mx)
					{
						pp->vx = mx;
					}
				}
			}
		}
		else if(pp2_controller[pp->controller]->state[PP2_CONTROLLER_LEFT].held)
		{
			if(!(pp->state >= PP2_CHARACTER_STATE_STAND_R_R && pp->state <= PP2_CHARACTER_STATE_STAND_R_UR) && !(pp->state >= PP2_CHARACTER_STATE_STAND_L_R && pp->state <= PP2_CHARACTER_STATE_STAND_L_UR) && !(pp->state >= PP2_CHARACTER_STATE_DUCK_R_R && pp->state <= PP2_CHARACTER_STATE_DUCK_R_UR) && !(pp->state >= PP2_CHARACTER_STATE_DUCK_L_R && pp->state <= PP2_CHARACTER_STATE_DUCK_L_UR))
			{
				if(pp->vx > -mx)
				{
					pp->fvx = -1.0;
					pp->vx -= 1.0;
					if(pp->vx < -mx)
					{
						pp->vx = -mx;
					}
				}
			}
		}
		else
		{
			pp->fvx = 0.0;
		}
		if(friction)
		{
			bool stopped = false;
			if(pp->vx < 0.0)
			{
				pp->vx += 0.5;
				if(pp->vx >= 0.0)
				{
					pp->vx = 0.0;
					stopped = true;
				}
			}
			else if(pp->vx > 0.0)
			{
				pp->vx -= 0.5;
				if(pp->vx <= 0.0)
				{
					pp->vx = 0.0;
					stopped = true;
				}
			}
			if(stopped)
			{
				pp2_player_stop(pp);
			}
		}
	}

	/* select correct collision object */
	if(pp->state >= PP2_CHARACTER_STATE_DUCK_R_R && pp->state <= PP2_CHARACTER_STATE_DUCK_L_UR)
	{
		pp->current_object = 1;
	}
	else
	{
		pp->current_object = 0;
	}

	/* move player */
	pp2_move_player(gp, pp, resources);
	pp2_control_player(gp, pp);
	pp->tick++;

	/* find target */
	if(pp->weapon == PP2_PAINTBALL_TYPE_SEEKER && pp->ammo[pp->weapon] > 0)
	{
		old_target = pp->target;
		pp->target = pp2_find_closest_player(gp, pp);
		if(pp->target != old_target && pp->target >= 0)
		{
			pp2_play_sample(gp, pp->character->sample[PP2_SAMPLE_TARGET], pp->x, pp->y, 1.0, 1.0);
		}
	}
	else
	{
		pp->target = -1;
	}

	/* find closest coin */
	if(pp2_option[PP2_OPTION_GAME_MODE] == PP2_GAME_MODE_COIN_RUSH)
	{
		pp2_find_closest_coin(gp, pp);
	}

	/* handle weapon select timer */
	if(pp->timer[PP2_PLAYER_TIMER_WEAPON_SELECT] > 0)
	{
		pp->timer[PP2_PLAYER_TIMER_WEAPON_SELECT]--;
	}
	if(pp->timer[PP2_PLAYER_TIMER_STOMPED] > 0)
	{
		pp->timer[PP2_PLAYER_TIMER_STOMPED]--;
	}

	/* handle powerup timers */
	if(pp->flags & PP2_PLAYER_FLAG_POWER_CLOAK)
	{
		pp->timer[PP2_PLAYER_TIMER_CLOAK]--;
		if(pp->timer[PP2_PLAYER_TIMER_CLOAK] <= 0)
		{
			pp->flags ^= PP2_PLAYER_FLAG_POWER_CLOAK;
		}
	}
	if(pp->flags & PP2_PLAYER_FLAG_POWER_JUMP)
	{
		pp->timer[PP2_PLAYER_TIMER_JUMP]--;
		if(pp->timer[PP2_PLAYER_TIMER_JUMP] <= 0)
		{
			pp->flags ^= PP2_PLAYER_FLAG_POWER_JUMP;
		}
	}
	if(pp->flags & PP2_PLAYER_FLAG_POWER_RUN)
	{
		pp->timer[PP2_PLAYER_TIMER_RUN]--;
		if(pp->timer[PP2_PLAYER_TIMER_RUN] <= 0)
		{
			pp->flags ^= PP2_PLAYER_FLAG_POWER_RUN;
		}
	}
	if(pp->flags & PP2_PLAYER_FLAG_POWER_DEFLECT)
	{
		pp->timer[PP2_PLAYER_TIMER_DEFLECT]--;
		if(pp->timer[PP2_PLAYER_TIMER_DEFLECT] <= 0)
		{
			pp->flags ^= PP2_PLAYER_FLAG_POWER_DEFLECT;
		}
	}
	if(pp->flags & PP2_PLAYER_FLAG_POWER_FLY)
	{
		pp->timer[PP2_PLAYER_TIMER_FLY]--;
		if(pp->timer[PP2_PLAYER_TIMER_FLY] <= 0)
		{
			pp->flags ^= PP2_PLAYER_FLAG_POWER_FLY;
		}
	}
	if(pp->flags & PP2_PLAYER_FLAG_POWER_TURBO)
	{
		pp->timer[PP2_PLAYER_TIMER_TURBO]--;
		if(pp->timer[PP2_PLAYER_TIMER_TURBO] <= 0)
		{
			pp->flags ^= PP2_PLAYER_FLAG_POWER_TURBO;
		}
	}
	if(pp->flags & PP2_PLAYER_FLAG_ACTIVE)
	{
		pp2_radar_object[pp2_radar_objects].x = pp->x + pp->object[pp->current_object]->map.top.point[0].x;
		pp2_radar_object[pp2_radar_objects].y = pp->y + pp->object[pp->current_object]->map.left.point[0].y;
		pp2_radar_object[pp2_radar_objects].player = pp->id;
		pp2_radar_objects++;
	}
}

void pp2_player_render(PP2_PLAYER * pp, PP2_CAMERA * cp)
{
	int i;
	ALLEGRO_COLOR tint_color;
	float alpha;

	if(pp->fade_time)
	{
		alpha = (float)pp->fade_time / 30.0;
		tint_color = al_map_rgba_f(alpha, alpha, alpha, alpha);
	}
	else
	{
		if(pp->flags & PP2_PLAYER_FLAG_POWER_CLOAK && pp->flash_time <= 0)
		{
			tint_color = al_map_rgba_f(0.5, 0.5, 0.5, 0.5);
		}
		else
		{
			alpha = (float)pp->flash_time / 30.0;
			tint_color = al_map_rgba_f(1.0, 1.0 - alpha, 1.0 - alpha, 1.0);
		}
	}
	if(pp->flags & PP2_PLAYER_FLAG_ACTIVE)
	{
		if((pp->flags & PP2_PLAYER_FLAG_POWER_CLOAK) && cp != &pp->camera)
		{
		}
		else
		{
			for(i = 0; i < pp->character->state[pp->state].pieces; i++)
			{
				t3f_draw_rotated_animation(pp->character->animation[pp->character->state[pp->state].piece[i].animation], tint_color, pp->tick, pp->character->state[pp->state].piece[i].cx, pp->character->state[pp->state].piece[i].cy, pp->x + pp->character->state[pp->state].piece[i].x - cp->x, pp->y + pp->character->state[pp->state].piece[i].y - cp->y, -cp->z, pp->character->state[pp->state].piece[i].angle, pp->character->state[pp->state].piece[i].flags);
			}
		}
	}
}
