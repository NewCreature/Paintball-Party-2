#include "../../t3f/t3f.h"
#include "../../t3f/draw.h"
#include "../../t3f/sound.h"
#include "../../joynet/joynet.h"
#include "../../data.h"
#include "../../misc/fixed_point.h"
#include "../../misc/sound.h"
#include "../camera.h"
#include "paintball.h"

static bool pp2_paintball_on_floor(PP2_PAINTBALL * pp)
{
	int i;

	if(pp->flags & PP2_PAINTBALL_FLAG_LANDB)
	{
		for(i = 0; i < pp->object->map.bottom.points; i++)
		{
			if(t3f_get_collision_tilemap_flag(pp2_level->collision_tilemap[pp->layer], pp->object->x + pp->object->map.bottom.point[i].x, pp->object->y + pp->object->map.bottom.point[i].y + 1.0, T3F_COLLISION_FLAG_SOLID_TOP))
			{
				return true;
			}
		}
	}
	else if(pp->flags & PP2_PAINTBALL_FLAG_LANDT)
	{
		for(i = 0; i < pp->object->map.top.points; i++)
		{
			if(t3f_get_collision_tilemap_flag(pp2_level->collision_tilemap[pp->layer], pp->object->x + pp->object->map.top.point[i].x, pp->object->y + pp->object->map.top.point[i].y - 1.0, T3F_COLLISION_FLAG_SOLID_BOTTOM))
			{
				return true;
			}
		}
	}
	else if(pp->flags & PP2_PAINTBALL_FLAG_LANDL)
	{
		for(i = 0; i < pp->object->map.left.points; i++)
		{
			if(t3f_get_collision_tilemap_flag(pp2_level->collision_tilemap[pp->layer], pp->object->x + pp->object->map.left.point[i].x - 1.0, pp->object->y + pp->object->map.left.point[i].y, T3F_COLLISION_FLAG_SOLID_RIGHT))
			{
				return true;
			}
		}
	}
	else if(pp->flags & PP2_PAINTBALL_FLAG_LANDR)
	{
		for(i = 0; i < pp->object->map.left.points; i++)
		{
			if(t3f_get_collision_tilemap_flag(pp2_level->collision_tilemap[pp->layer], pp->object->x + pp->object->map.right.point[i].x + 1.0, pp->object->y + pp->object->map.right.point[i].y, T3F_COLLISION_FLAG_SOLID_LEFT))
			{
				return true;
			}
		}
	}
	return false;
}

static bool pp2_paintball_slip(PP2_PAINTBALL * pp)
{
	int i;
	int cf;
	int slip_center;

	if((pp->flags & PP2_PAINTBALL_FLAG_LANDB))
	{
		slip_center = t3f_get_collision_tilemap_flag(pp2_level->collision_tilemap[pp->layer], pp->object->x + pp->object->map.bottom.point[0].x, pp->object->y + pp->object->map.bottom.point[0].y + 1.0, T3F_COLLISION_FLAG_SOLID_TOP | PP2_LEVEL_COLLISION_FLAG_ICE);
		if(slip_center == (T3F_COLLISION_FLAG_SOLID_TOP | PP2_LEVEL_COLLISION_FLAG_ICE))
		{
			return true;
		}
		else
		{
			for(i = 1; i < pp->object->map.bottom.points; i++)
			{
				cf = t3f_get_collision_tilemap_flag(pp2_level->collision_tilemap[pp->layer], pp->object->x + pp->object->map.bottom.point[i].x, pp->object->y + pp->object->map.bottom.point[i].y + 1.0, T3F_COLLISION_FLAG_SOLID_TOP | PP2_LEVEL_COLLISION_FLAG_ICE);
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

static int pp2_paintball_convey(PP2_PAINTBALL * pp)
{
	int i;
	int cf;

	if((pp->flags & PP2_PAINTBALL_FLAG_LANDB))
	{
		for(i = 0; i < pp->object->map.bottom.points; i++)
		{
			cf = t3f_get_collision_tilemap_flag(pp2_level->collision_tilemap[pp->layer], pp->object->x + pp->object->map.bottom.point[i].x, pp->object->y + pp->object->map.bottom.point[i].y + 1.0, T3F_COLLISION_FLAG_SOLID_TOP | PP2_LEVEL_COLLISION_FLAG_CONVEYOR);
			if(cf == (T3F_COLLISION_FLAG_SOLID_TOP | PP2_LEVEL_COLLISION_FLAG_CONVEYOR))
			{
				return t3f_get_collision_tilemap_data(pp2_level->collision_tilemap[pp->layer], pp->object->x + pp->object->map.bottom.point[i].x, pp->object->y + pp->object->map.bottom.point[i].y + 1.0, 0);
			}
		}
	}
	return 0;
}

int pp2_create_paintball(int owner, int type, float x, float y, float angle)
{
	int i;
	float ox, oy;

	for(i = 0; i < PP2_MAX_PAINTBALLS; i++)
	{
		if(!(pp2_player[owner].paintball[i].flags & PP2_PAINTBALL_FLAG_ACTIVE))
		{
			pp2_player[owner].paintball[i].owner = owner;
			pp2_player[owner].paintball[i].type = type;
			pp2_player[owner].paintball[i].x = x;
			pp2_player[owner].paintball[i].y = y;
			pp2_player[owner].paintball[i].z = pp2_level->tilemap->layer[pp2_player[owner].layer]->z;
			pp2_player[owner].paintball[i].layer = pp2_player[owner].layer;
			pp2_player[owner].paintball[i].cx = pp2_player[owner].character->state[pp2_player[owner].state].paintball.cx;
			pp2_player[owner].paintball[i].cy = pp2_player[owner].character->state[pp2_player[owner].state].paintball.cy;
			pp2_player[owner].paintball[i].angle = angle;
			pp2_player[owner].paintball[i].vx = 16.0 * cos(pp2_player[owner].paintball[i].angle);
			pp2_player[owner].paintball[i].vy = 16.0 * sin(pp2_player[owner].paintball[i].angle);
			pp2_player[owner].paintball[i].leaving = 2;
			ox = (float)pp2_player[owner].character->animation[pp2_player[owner].character->state[pp2_player[owner].state].paintball.animation]->frame[0]->width / 2.0 - (float)pp2_player[owner].character->paintball_size;
			oy = (float)pp2_player[owner].character->animation[pp2_player[owner].character->state[pp2_player[owner].state].paintball.animation]->frame[0]->height / 2.0 - (float)pp2_player[owner].character->paintball_size;
			t3f_recreate_collision_object(pp2_player[owner].paintball[i].object, ox, oy, pp2_player[owner].character->paintball_size * 2, pp2_player[owner].character->paintball_size * 2, 32, 32, 0);
			pp2_player[owner].paintball[i].object->x = pp2_player[owner].paintball[i].x;
			pp2_player[owner].paintball[i].object->y = pp2_player[owner].paintball[i].y;
			pp2_player[owner].paintball[i].object->ox = pp2_player[owner].paintball[i].x;
			pp2_player[owner].paintball[i].object->oy = pp2_player[owner].paintball[i].y;
			pp2_player[owner].paintball[i].tick = 0;
			pp2_player[owner].paintball[i].flags = PP2_PAINTBALL_FLAG_ACTIVE;
			return i;
		}
	}
	return -1;
}

int pp2_create_particle(PP2_PLAYER * pp, float x, float y, float z)
{
	int i;

	for(i = 0; i < PP2_MAX_PARTICLES; i++)
	{
		if(!(pp->particle[i].flags & PP2_PARTICLE_FLAG_ACTIVE))
		{
			pp->particle[i].x = x;
			pp->particle[i].y = y;
			pp->particle[i].z = z;
			pp->particle[i].tick = 0;
			pp->particle[i].type = 0;
			pp->particle[i].flags = PP2_PARTICLE_FLAG_ACTIVE;
			return i;
		}
	}
	return -1;
}

void pp2_create_splat(PP2_PAINTBALL * pp, int extra)
{
	int i;
	int p;

	switch(extra)
	{
		case 1:
		{
			if(pp->vx > 0)
			{
				for(i = 0; i < 24; i++)
				{
					p = pp2_create_particle(&pp2_player[pp->owner], pp->x + pp->object->map.right.point[0].x - pp2_player[pp->owner].character->state[pp2_player[pp->owner].state].particle.cx, pp->y + pp->object->map.left.point[0].y - pp2_player[pp->owner].character->state[pp2_player[pp->owner].state].particle.cy, 0.0);
					if(p >= 0)
					{
						pp2_player[pp->owner].particle[p].vx = joynet_drand() * -4.0 + joynet_drand();
						pp2_player[pp->owner].particle[p].vy = joynet_drand() * 8.0 - 4.0;
						pp2_player[pp->owner].particle[p].vz = joynet_drand() * 8.0 - 4.0;
						pp2_player[pp->owner].particle[p].total_life = 15 + joynet_rand() % 15;
						pp2_player[pp->owner].particle[p].life = pp2_player[pp->owner].particle[p].total_life;
						pp2_player[pp->owner].particle[p].owner = pp->owner;
					}
				}
			}
			else if(pp->vx < 0)
			{
				for(i = 0; i < 24; i++)
				{
					p = pp2_create_particle(&pp2_player[pp->owner], pp->x + pp->object->map.left.point[0].x - pp2_player[pp->owner].character->state[pp2_player[pp->owner].state].particle.cx, pp->y + pp->object->map.left.point[0].y - pp2_player[pp->owner].character->state[pp2_player[pp->owner].state].particle.cy, 0.0);
					if(p >= 0)
					{
						pp2_player[pp->owner].particle[p].vx = joynet_drand() * 4.0 - joynet_drand();
						pp2_player[pp->owner].particle[p].vy = joynet_drand() * 8.0 - 4.0;
						pp2_player[pp->owner].particle[p].vz = joynet_drand() * 8.0 - 4.0;
						pp2_player[pp->owner].particle[p].total_life = 15 + joynet_rand() % 15;
						pp2_player[pp->owner].particle[p].life = pp2_player[pp->owner].particle[p].total_life;
						pp2_player[pp->owner].particle[p].owner = pp->owner;
					}
				}
			}
			else
			{
				for(i = 0; i < 24; i++)
				{
					p = pp2_create_particle(&pp2_player[pp->owner], pp->x + pp->object->map.top.point[0].x - pp2_player[pp->owner].character->state[pp2_player[pp->owner].state].particle.cx, pp->y + pp->object->map.left.point[0].y - pp2_player[pp->owner].character->state[pp2_player[pp->owner].state].particle.cy, 0.0);
					if(p >= 0)
					{
						pp2_player[pp->owner].particle[p].vx = joynet_drand() * 4.0 - joynet_drand();
						pp2_player[pp->owner].particle[p].vy = joynet_drand() * 8.0 - 4.0;
						pp2_player[pp->owner].particle[p].vz = joynet_drand() * 8.0 - 4.0;
						pp2_player[pp->owner].particle[p].total_life = 15 + joynet_rand() % 15;
						pp2_player[pp->owner].particle[p].life = pp2_player[pp->owner].particle[p].total_life;
						pp2_player[pp->owner].particle[p].owner = pp->owner;
					}
				}
			}
			break;
		}
		case 2:
		{
			if(pp->vy > 0)
			{
				for(i = 0; i < 24; i++)
				{
					p = pp2_create_particle(&pp2_player[pp->owner], pp->x + pp->object->map.top.point[0].x - pp2_player[pp->owner].character->state[pp2_player[pp->owner].state].particle.cx, pp->y + pp->object->map.bottom.point[0].y - pp2_player[pp->owner].character->state[pp2_player[pp->owner].state].particle.cy, 0.0);
					if(p >= 0)
					{
						pp2_player[pp->owner].particle[p].vx = joynet_drand() * 8.0 - 4.0;
						pp2_player[pp->owner].particle[p].vy = joynet_drand() * -4.0 + joynet_drand();
						pp2_player[pp->owner].particle[p].vz = joynet_drand() * 8.0 - 4.0;
						pp2_player[pp->owner].particle[p].total_life = 15 + joynet_rand() % 15;
						pp2_player[pp->owner].particle[p].life = pp2_player[pp->owner].particle[p].total_life;
						pp2_player[pp->owner].particle[p].owner = pp->owner;
					}
				}
			}
			else if(pp->vy < 0)
			{
				for(i = 0; i < 24; i++)
				{
					p = pp2_create_particle(&pp2_player[pp->owner], pp->x + pp->object->map.top.point[0].x - pp2_player[pp->owner].character->state[pp2_player[pp->owner].state].particle.cx, pp->y + pp->object->map.top.point[0].y - pp2_player[pp->owner].character->state[pp2_player[pp->owner].state].particle.cy, 0.0);
					if(p >= 0)
					{
						pp2_player[pp->owner].particle[p].vx = joynet_drand() * 8.0 - 4.0;
						pp2_player[pp->owner].particle[p].vy = joynet_drand() * 4.0 - joynet_drand();
						pp2_player[pp->owner].particle[p].vz = joynet_drand() * 8.0 - 4.0;
						pp2_player[pp->owner].particle[p].total_life = 15 + joynet_rand() % 15;
						pp2_player[pp->owner].particle[p].life = pp2_player[pp->owner].particle[p].total_life;
						pp2_player[pp->owner].particle[p].owner = pp->owner;
					}
				}
			}
			else
			{
				for(i = 0; i < 24; i++)
				{
					p = pp2_create_particle(&pp2_player[pp->owner], pp->x + pp->object->map.top.point[0].x - pp2_player[pp->owner].character->state[pp2_player[pp->owner].state].particle.cx, pp->y + pp->object->map.left.point[0].y - pp2_player[pp->owner].character->state[pp2_player[pp->owner].state].particle.cy, 0.0);
					if(p >= 0)
					{
						pp2_player[pp->owner].particle[p].vx = joynet_drand() * 8.0 - 4.0;
						pp2_player[pp->owner].particle[p].vy = joynet_drand() * 4.0 - joynet_drand();
						pp2_player[pp->owner].particle[p].vz = joynet_drand() * 8.0 - 4.0;
						pp2_player[pp->owner].particle[p].total_life = 15 + joynet_rand() % 15;
						pp2_player[pp->owner].particle[p].life = pp2_player[pp->owner].particle[p].total_life;
						pp2_player[pp->owner].particle[p].owner = pp->owner;
					}
				}
			}
			break;
		}
		default:
		{
			for(i = 0; i < 24; i++)
			{
				p = pp2_create_particle(&pp2_player[pp->owner], pp->x + pp->object->map.top.point[0].x - pp2_player[pp->owner].character->state[pp2_player[pp->owner].state].particle.cx, pp->y + pp->object->map.left.point[0].y - pp2_player[pp->owner].character->state[pp2_player[pp->owner].state].particle.cy, 0.0);
				if(p >= 0)
				{
					pp2_player[pp->owner].particle[p].vx = joynet_drand() * 8.0 - 4.0;
					pp2_player[pp->owner].particle[p].vy = joynet_drand() * 8.0 - 4.0;
					pp2_player[pp->owner].particle[p].vz = joynet_drand() * 8.0 - 4.0;
					pp2_player[pp->owner].particle[p].total_life = 15 + joynet_rand() % 15;
					pp2_player[pp->owner].particle[p].life = pp2_player[pp->owner].particle[p].total_life;
					pp2_player[pp->owner].particle[p].owner = pp->owner;
				}
			}
			break;
		}
	}
}

void pp2_destroy_paintball(PP2_PAINTBALL * pp, int extra)
{
	switch(pp->type)
	{
		case PP2_PAINTBALL_TYPE_SPLITTER:
		{
			pp2_create_paintball(pp->owner, PP2_PAINTBALL_TYPE_NORMAL, pp->x, pp->y, pp->angle - 0.75 * ALLEGRO_PI);
			pp2_create_paintball(pp->owner, PP2_PAINTBALL_TYPE_NORMAL, pp->x, pp->y, pp->angle + 0.75 * ALLEGRO_PI);
			pp->flags = 0;
			pp2_play_sample(pp2_player[pp->owner].character->sample[PP2_SAMPLE_SPLAT], pp->x + pp->object->map.top.point[0].x, pp->y + pp->object->map.left.point[0].x, 1.0, 1.0 + joynet_drand() / 4.0 - 0.125);
			break;
		}
		case PP2_PAINTBALL_TYPE_PMINE:
		{
			pp2_create_paintball(pp->owner, PP2_PAINTBALL_TYPE_NORMAL, pp->x, pp->y, 0.0);
			pp2_create_paintball(pp->owner, PP2_PAINTBALL_TYPE_NORMAL, pp->x, pp->y, 0.25 * ALLEGRO_PI);
			pp2_create_paintball(pp->owner, PP2_PAINTBALL_TYPE_NORMAL, pp->x, pp->y, 0.50 * ALLEGRO_PI);
			pp2_create_paintball(pp->owner, PP2_PAINTBALL_TYPE_NORMAL, pp->x, pp->y, 0.75 * ALLEGRO_PI);
			pp2_create_paintball(pp->owner, PP2_PAINTBALL_TYPE_NORMAL, pp->x, pp->y, 1.00 * ALLEGRO_PI);
			pp2_create_paintball(pp->owner, PP2_PAINTBALL_TYPE_NORMAL, pp->x, pp->y, 1.25 * ALLEGRO_PI);
			pp2_create_paintball(pp->owner, PP2_PAINTBALL_TYPE_NORMAL, pp->x, pp->y, 1.50 * ALLEGRO_PI);
			pp2_create_paintball(pp->owner, PP2_PAINTBALL_TYPE_NORMAL, pp->x, pp->y, 1.75 * ALLEGRO_PI);
			pp->flags = 0;
			pp2_play_sample(pp2_player[pp->owner].character->sample[PP2_SAMPLE_SPLAT], pp->x + pp->object->map.top.point[0].x, pp->y + pp->object->map.left.point[0].x, 1.0, 1.0 + joynet_drand() / 4.0 - 0.125);
			break;
		}
		default:
		{
			pp->flags = 0;
			pp2_play_sample(pp2_player[pp->owner].character->sample[PP2_SAMPLE_SPLAT], pp->x + pp->object->map.top.point[0].x, pp->y + pp->object->map.left.point[0].x, 1.0, 1.0 + joynet_drand() / 4.0 - 0.125);
			break;
		}
	}
	pp2_create_splat(pp, extra);
}

/* figure difference between angles */
static double angle_diff(double theta1, double theta2)
{
	double dif = fmod(theta2 - theta1, 2.0 * ALLEGRO_PI); // in range
	if(theta1 > theta2)
	{
		dif += 2.0 * ALLEGRO_PI;
	}
	if(dif >= ALLEGRO_PI)
	{
		dif = -(dif - 2.0 * ALLEGRO_PI);
	}
	return dif;
}

void pp2_paintball_logic(PP2_PAINTBALL * pp)
{
	int i;
	int convey;

	if(!(pp->flags & PP2_PAINTBALL_FLAG_ACTIVE))
	{
		return;
	}
	if(pp->deflect > 0)
	{
		pp->deflect--;
	}
	if(!(pp->flags & (PP2_PAINTBALL_FLAG_LANDB | PP2_PAINTBALL_FLAG_LANDT | PP2_PAINTBALL_FLAG_LANDL | PP2_PAINTBALL_FLAG_LANDR)))
	{
		pp2_player[pp->owner].trail[pp2_player[pp->owner].current_trail].x = pp->x;
		pp2_player[pp->owner].trail[pp2_player[pp->owner].current_trail].y = pp->y;
		pp2_player[pp->owner].trail[pp2_player[pp->owner].current_trail].cx = pp->cx;
		pp2_player[pp->owner].trail[pp2_player[pp->owner].current_trail].cy = pp->cy;
		pp2_player[pp->owner].trail[pp2_player[pp->owner].current_trail].angle = pp->angle;
		pp2_player[pp->owner].trail[pp2_player[pp->owner].current_trail].tick = pp->tick;
		pp2_player[pp->owner].trail[pp2_player[pp->owner].current_trail].owner = pp->owner;
		if(pp->type == PP2_PAINTBALL_TYPE_GHOST)
		{
			pp2_player[pp->owner].trail[pp2_player[pp->owner].current_trail].counter = PP2_PAINTBALL_TRAIL_TIME / 2;
		}
		else
		{
			pp2_player[pp->owner].trail[pp2_player[pp->owner].current_trail].counter = PP2_PAINTBALL_TRAIL_TIME;
		}
		pp2_player[pp->owner].trail[pp2_player[pp->owner].current_trail].active = true;
		pp2_player[pp->owner].current_trail++;
		if(pp2_player[pp->owner].current_trail >= PP2_MAX_TRAILS)
		{
			pp2_player[pp->owner].current_trail = 0;
		}
	}
	switch(pp->type)
	{
		case PP2_PAINTBALL_TYPE_NORMAL:
		case PP2_PAINTBALL_TYPE_SPLITTER:
		{
			pp->x += pp->vx;
			t3f_move_collision_object_x(pp->object, pp->x);
			if(!t3f_get_collision_tilemap_flag(pp2_level->collision_tilemap[pp->layer], pp->object->x + pp->object->map.bottom.point[0].x, pp->object->y + pp->object->map.bottom.point[0].y, T3F_COLLISION_FLAG_SLOPE_TOP))
			{
				if(t3f_check_tilemap_collision_left(pp->object, pp2_level->collision_tilemap[pp->layer]) || t3f_check_tilemap_collision_right(pp->object, pp2_level->collision_tilemap[pp->layer]))
				{
					pp->x = t3f_get_tilemap_collision_x(pp->object, pp2_level->collision_tilemap[pp->layer]);
					pp2_destroy_paintball(pp, 1);
				}
			}
			pp->y += pp->vy;
			t3f_move_collision_object_y(pp->object, pp->y);
			if(t3f_check_tilemap_collision_top(pp->object, pp2_level->collision_tilemap[pp->layer]) || t3f_check_tilemap_collision_bottom(pp->object, pp2_level->collision_tilemap[pp->layer]))
			{
				pp->y = t3f_get_tilemap_collision_y(pp->object, pp2_level->collision_tilemap[pp->layer]);
				pp2_destroy_paintball(pp, 2);
			}
			break;
		}
		case PP2_PAINTBALL_TYPE_MINE:
		{
			pp->x += pp->vx;
			convey = pp2_paintball_convey(pp);
			if(convey)
			{
				pp->x += pp2_fixtof(convey);
			}
			t3f_move_collision_object_x(pp->object, pp->x);
			if(t3f_check_tilemap_collision_left(pp->object, pp2_level->collision_tilemap[pp->layer]))
			{
				pp->x = t3f_get_tilemap_collision_x(pp->object, pp2_level->collision_tilemap[pp->layer]);
				t3f_move_collision_object_x(pp->object, pp->x);
				pp->flags |= PP2_PAINTBALL_FLAG_LANDL;
				pp->vx = 0.0;
				pp->vy = 0.0;
			}
			if(t3f_check_tilemap_collision_right(pp->object, pp2_level->collision_tilemap[pp->layer]))
			{
				pp->x = t3f_get_tilemap_collision_x(pp->object, pp2_level->collision_tilemap[pp->layer]);
				t3f_move_collision_object_x(pp->object, pp->x);
				pp->flags |= PP2_PAINTBALL_FLAG_LANDR;
				pp->vx = 0.0;
				pp->vy = 0.0;
			}
			pp->y += pp->vy;
			t3f_move_collision_object_y(pp->object, pp->y);
			if(t3f_check_tilemap_collision_bottom(pp->object, pp2_level->collision_tilemap[pp->layer]))
			{
				pp->y = t3f_get_tilemap_collision_y(pp->object, pp2_level->collision_tilemap[pp->layer]);
				t3f_move_collision_object_y(pp->object, pp->y);
				pp->vy = 0.0;
				pp->flags |= PP2_PAINTBALL_FLAG_LANDB;
				if(!pp2_paintball_slip(pp))
				{
					pp->vx = 0.0;
				}
			}
			else if(t3f_check_tilemap_collision_top(pp->object, pp2_level->collision_tilemap[pp->layer]))
			{
				pp->y = t3f_get_tilemap_collision_y(pp->object, pp2_level->collision_tilemap[pp->layer]);
				t3f_move_collision_object_y(pp->object, pp->y);
				pp->flags |= PP2_PAINTBALL_FLAG_LANDT;
				pp->vx = 0.0;
				pp->vy = 0.0;
			}
			if((pp->flags & (PP2_PAINTBALL_FLAG_LANDB | PP2_PAINTBALL_FLAG_LANDT | PP2_PAINTBALL_FLAG_LANDL | PP2_PAINTBALL_FLAG_LANDR)) && !pp2_paintball_on_floor(pp))
			{
				pp->flags &= ~(PP2_PAINTBALL_FLAG_LANDB | PP2_PAINTBALL_FLAG_LANDT | PP2_PAINTBALL_FLAG_LANDL | PP2_PAINTBALL_FLAG_LANDR);
			}

			/* add gravity */
			if(!(pp->flags & (PP2_PAINTBALL_FLAG_LANDB | PP2_PAINTBALL_FLAG_LANDT | PP2_PAINTBALL_FLAG_LANDL | PP2_PAINTBALL_FLAG_LANDR)))
			{
				pp->angle = atan2(pp->object->y - pp->object->oy, pp->object->x - pp->object->ox);
				pp->vy += 1.0;
				if(pp->vy > 14.0)
				{
					pp->vy = 14.0;
				}
			}
			break;
		}
		case PP2_PAINTBALL_TYPE_PMINE:
		{
			pp->x += pp->vx;
			convey = pp2_paintball_convey(pp);
			if(convey)
			{
				pp->x += pp2_fixtof(convey);
			}
			t3f_move_collision_object_x(pp->object, pp->x);
			if(t3f_check_tilemap_collision_left(pp->object, pp2_level->collision_tilemap[pp->layer]))
			{
				pp->x = t3f_get_tilemap_collision_x(pp->object, pp2_level->collision_tilemap[pp->layer]);
				t3f_move_collision_object_x(pp->object, pp->x);
				pp->flags |= PP2_PAINTBALL_FLAG_LANDL;
				pp->vx = 0.0;
				pp->vy = 0.0;
			}
			if(t3f_check_tilemap_collision_right(pp->object, pp2_level->collision_tilemap[pp->layer]))
			{
				pp->x = t3f_get_tilemap_collision_x(pp->object, pp2_level->collision_tilemap[pp->layer]);
				t3f_move_collision_object_x(pp->object, pp->x);
				pp->flags |= PP2_PAINTBALL_FLAG_LANDR;
				pp->vx = 0.0;
				pp->vy = 0.0;
			}
			pp->y += pp->vy;
			t3f_move_collision_object_y(pp->object, pp->y);
			if(t3f_check_tilemap_collision_bottom(pp->object, pp2_level->collision_tilemap[pp->layer]))
			{
				pp->y = t3f_get_tilemap_collision_y(pp->object, pp2_level->collision_tilemap[pp->layer]);
				t3f_move_collision_object_y(pp->object, pp->y);
				pp->vy = 0.0;
				pp->flags |= PP2_PAINTBALL_FLAG_LANDB;
				if(!pp2_paintball_slip(pp))
				{
					pp->vx = 0.0;
				}
			}
			else if(t3f_check_tilemap_collision_top(pp->object, pp2_level->collision_tilemap[pp->layer]))
			{
				pp->y = t3f_get_tilemap_collision_y(pp->object, pp2_level->collision_tilemap[pp->layer]);
				t3f_move_collision_object_y(pp->object, pp->y);
				pp->flags |= PP2_PAINTBALL_FLAG_LANDT;
				pp->vx = 0.0;
				pp->vy = 0.0;
			}
			if(!pp2_paintball_on_floor(pp) && (pp->flags & (PP2_PAINTBALL_FLAG_LANDB | PP2_PAINTBALL_FLAG_LANDT | PP2_PAINTBALL_FLAG_LANDL | PP2_PAINTBALL_FLAG_LANDR)))
			{
				pp->flags &= ~(PP2_PAINTBALL_FLAG_LANDB | PP2_PAINTBALL_FLAG_LANDT | PP2_PAINTBALL_FLAG_LANDL | PP2_PAINTBALL_FLAG_LANDR);
			}

			/* add gravity */
			if(!(pp->flags & (PP2_PAINTBALL_FLAG_LANDB | PP2_PAINTBALL_FLAG_LANDT | PP2_PAINTBALL_FLAG_LANDL | PP2_PAINTBALL_FLAG_LANDR)))
			{
				pp->angle = atan2(pp->object->y - pp->object->oy, pp->object->x - pp->object->ox);
				pp->vy += 1.0;
				if(pp->vy > 14.0)
				{
					pp->vy = 14.0;
				}
			}

			/* if anyone is close, set it off */
			else
			{
				for(i = 0; i < PP2_MAX_PLAYERS; i++)
				{
					if((pp2_player[i].flags & PP2_PLAYER_FLAG_ACTIVE) && i != pp->owner && t3f_distance(pp2_player[i].x + pp2_player[i].object[pp2_player[i].current_object]->map.top.point[0].x, pp2_player[i].y + pp2_player[i].object[pp2_player[i].current_object]->map.left.point[0].y, pp->x, pp->y) < 96.0)
					{
						pp2_destroy_paintball(pp, 0);
						break;
					}
				}
			}
			break;
		}
		case PP2_PAINTBALL_TYPE_BOUNCER:
		{
			pp->x += pp->vx;
			t3f_move_collision_object_x(pp->object, pp->x);
			if(!t3f_get_collision_tilemap_flag(pp2_level->collision_tilemap[pp->layer], pp->object->x + pp->object->map.bottom.point[0].x, pp->object->y + pp->object->map.bottom.point[0].y, T3F_COLLISION_FLAG_SLOPE_TOP))
			{
				if(t3f_check_tilemap_collision_left(pp->object, pp2_level->collision_tilemap[pp->layer]) || t3f_check_tilemap_collision_right(pp->object, pp2_level->collision_tilemap[pp->layer]))
				{
					pp->x = t3f_get_tilemap_collision_x(pp->object, pp2_level->collision_tilemap[pp->layer]);
					pp2_destroy_paintball(pp, 1);
				}
			}
			pp->y += pp->vy;
			t3f_move_collision_object_y(pp->object, pp->y);
			if(t3f_check_tilemap_collision_top(pp->object, pp2_level->collision_tilemap[pp->layer]) || t3f_check_tilemap_collision_bottom(pp->object, pp2_level->collision_tilemap[pp->layer]))
			{
				pp->y = t3f_get_tilemap_collision_y(pp->object, pp2_level->collision_tilemap[pp->layer]);
				t3f_move_collision_object_y(pp->object, pp->y);
				if(pp->vy > 0)
				{
					pp->vy = -6;
					pp2_play_sample(pp2_player[pp->owner].character->sample[PP2_SAMPLE_BOUNCE], pp->x + pp->object->map.top.point[0].x, pp->y + pp->object->map.left.point[0].x, 1.0, 1.0);
				}
				else
				{
					pp->vy = -pp->vy;
					pp2_play_sample(pp2_player[pp->owner].character->sample[PP2_SAMPLE_BOUNCE], pp->x + pp->object->map.top.point[0].x, pp->y + pp->object->map.left.point[0].x, 1.0, 1.0);
				}
			}

			/* add gravity */
			pp->angle = atan2(pp->object->y - pp->object->oy, pp->object->x - pp->object->ox);
			pp->vy += 1.0;
			if(pp->vy > 14.0)
			{
				pp->vy = 14.0;
			}
			break;
		}
		case PP2_PAINTBALL_TYPE_SEEKER:
		{
			pp->x += pp->vx;
			t3f_move_collision_object_x(pp->object, pp->x);
			if(!t3f_get_collision_tilemap_flag(pp2_level->collision_tilemap[pp->layer], pp->object->x + pp->object->map.bottom.point[0].x, pp->object->y + pp->object->map.bottom.point[0].y, T3F_COLLISION_FLAG_SLOPE_TOP))
			{
				if(t3f_check_tilemap_collision_left(pp->object, pp2_level->collision_tilemap[pp->layer]) || t3f_check_tilemap_collision_right(pp->object, pp2_level->collision_tilemap[pp->layer]))
				{
					pp->x = t3f_get_tilemap_collision_x(pp->object, pp2_level->collision_tilemap[pp->layer]);
					pp2_destroy_paintball(pp, 1);
				}
			}
			pp->y += pp->vy;
			t3f_move_collision_object_y(pp->object, pp->y);
			if(t3f_check_tilemap_collision_top(pp->object, pp2_level->collision_tilemap[pp->layer]) || t3f_check_tilemap_collision_bottom(pp->object, pp2_level->collision_tilemap[pp->layer]))
			{
				pp->y = t3f_get_tilemap_collision_y(pp->object, pp2_level->collision_tilemap[pp->layer]);
				pp2_destroy_paintball(pp, 2);
			}

			/* seek */
			if(pp->leaving <= 0 && pp->target >= 0)
			{
				float target_angle;
				float diff;

				/* untarget cloaked or dead player */
				if((pp2_player[pp->target].flags & PP2_PLAYER_FLAG_POWER_CLOAK) || pp2_player[pp->target].life <= 0)
				{
					pp->target = -1;
				}
				else
				{
					target_angle = atan2((pp2_player[pp->target].y + pp2_player[pp->target].object[pp2_player[pp->target].current_object]->map.left.point[0].y) - pp->y, pp2_player[pp->target].x + pp2_player[pp->target].object[pp2_player[pp->target].current_object]->map.top.point[0].x - pp->x);
					if(target_angle < 0.0)
					{
						target_angle += 2.0 * ALLEGRO_PI;
					}

					diff = angle_diff(target_angle, pp->angle);

					/* if we are close enough, go ahead and set the angle exactly,
					 * this makes the paintball move smoothly instead of jerking
					 * between angles */
					if(diff > 0.2)
					{
						if(angle_diff(pp->angle - 0.0625, target_angle) < diff)
						{
		           			pp->angle = fmod(pp->angle - 0.0625, 2 * ALLEGRO_PI);
						}
						else
						{
		           			pp->angle = fmod(pp->angle + 0.0625, 2 * ALLEGRO_PI);
						}
					}
					pp->vx = 8.0 * cos(pp->angle);
					pp->vy = 8.0 * sin(pp->angle);
				}
			}
			break;
		}
		case PP2_PAINTBALL_TYPE_REFLECTOR:
		{
			pp->x += pp->vx;
			t3f_move_collision_object_x(pp->object, pp->x);
			if(!t3f_get_collision_tilemap_flag(pp2_level->collision_tilemap[pp->layer], pp->object->x + pp->object->map.bottom.point[0].x, pp->object->y + pp->object->map.bottom.point[0].y, T3F_COLLISION_FLAG_SLOPE_TOP))
			{
				if(t3f_check_tilemap_collision_left(pp->object, pp2_level->collision_tilemap[pp->layer]) || t3f_check_tilemap_collision_right(pp->object, pp2_level->collision_tilemap[pp->layer]))
				{
					pp->x = t3f_get_tilemap_collision_x(pp->object, pp2_level->collision_tilemap[pp->layer]);
					t3f_move_collision_object_x(pp->object, pp->x);
					if(pp->counter > 0)
					{
						pp->vx = -pp->vx;
						pp->angle = atan2(pp->vy, pp->vx);
						pp2_play_sample(pp2_player[pp->owner].character->sample[PP2_SAMPLE_BOUNCE], pp->x + pp->object->map.top.point[0].x, pp->y + pp->object->map.left.point[0].x, 1.0, 1.0);
						pp->counter--;
					}
					else
					{
						pp2_destroy_paintball(pp, 1);
					}
				}
			}
			pp->y += pp->vy;
			t3f_move_collision_object_y(pp->object, pp->y);
			if(t3f_check_tilemap_collision_top(pp->object, pp2_level->collision_tilemap[pp->layer]) || t3f_check_tilemap_collision_bottom(pp->object, pp2_level->collision_tilemap[pp->layer]))
			{
				pp->y = t3f_get_tilemap_collision_y(pp->object, pp2_level->collision_tilemap[pp->layer]);
				t3f_move_collision_object_y(pp->object, pp->y);
				if(pp->counter > 0)
				{
					pp->vy = -pp->vy;
					pp->angle = atan2(pp->vy, pp->vx);
					pp2_play_sample(pp2_player[pp->owner].character->sample[PP2_SAMPLE_BOUNCE], pp->x + pp->object->map.top.point[0].x, pp->y + pp->object->map.left.point[0].x, 1.0, 1.0);
					pp->counter--;
				}
				else
				{
					pp2_destroy_paintball(pp, 2);
				}
			}
			break;
		}
		case PP2_PAINTBALL_TYPE_GHOST:
		{
			pp->x += pp->vx;
			t3f_move_collision_object_x(pp->object, pp->x);
			pp->y += pp->vy;
			t3f_move_collision_object_y(pp->object, pp->y);
			break;
		}
	}

	/* deactivate paintball if it goes off the playfield */
	if(pp->x < pp2_level->room.x * 32 - 128 || pp->x > pp2_level->room.bx * 32 + 32 + 128 || pp->y < pp2_level->room.y * 32 - 128 || pp->y > pp2_level->room.by * 32 + 32 + 128)
	{
		pp->flags = 0;
		return;
	}

	/* check collision with players */
	for(i = 0; i < PP2_MAX_PLAYERS; i++)
	{
		if(pp2_player[i].flags & PP2_PLAYER_FLAG_ACTIVE && pp2_player[i].fade_time <= 0)
		{
			if(t3f_check_object_collision(pp->object, pp2_player[i].object[pp2_player[i].current_object]))
			{
				if(pp->owner == i && pp->leaving > 0)
				{
				}
				else
				{
					if(pp2_player[i].flags & PP2_PLAYER_FLAG_POWER_DEFLECT)
					{
						if(pp->deflect <= 0)
						{

							/* if paintball is not moving, destroy it */
							if(fabs(pp->vx) < 0.01 && fabs(pp->vy) < 0.01)
							{
								pp2_destroy_paintball(pp, 0);
							}
							else
							{
								pp->vx = -pp->vx;
								pp->vy = -pp->vy;
								pp->deflect = 2;
								if(pp->type == PP2_PAINTBALL_TYPE_SEEKER)
								{
									if(pp->owner == i)
									{
										pp->target = -1;
									}
									else
									{
										pp->target = pp->owner;
									}
									pp->angle = fmod(pp->angle - ALLEGRO_PI, 2 * ALLEGRO_PI); // reverse angle
								}
								pp2_play_sample(pp2_player[pp->owner].character->sample[PP2_SAMPLE_RICOCHET], pp->x + pp->object->map.top.point[0].x, pp->y + pp->object->map.left.point[0].x, 1.0, 1.0);
							}
						}
					}
					else
					{
						if(pp2_winner < 0)
						{
							pp2_play_sample(pp2_player[i].character->sample[PP2_SAMPLE_HIT], pp2_player[i].x + pp2_player[i].object[0]->map.top.point[0].x, pp2_player[i].y + pp2_player[i].object[0]->map.left.point[0].x, 1.0, 1.0);

							/* don't deal damage when player is recently hit or if they are spawning */
							if(pp2_player[i].flash_time <= 0 && !(pp2_player[i].fade_time > 0 && pp2_player[i].fade_type != 0))
							{
								pp2_player_receive_hit(&pp2_player[i], pp->owner);
							}
						}
						pp2_destroy_paintball(pp, 0);
					}

					/* update profile */
					if(pp2_replay_player < 0)
					{
						if(pp2_client_game->player[pp->owner]->local)
						{
							if(pp->owner != i)
							{
								pp2_profiles.item[pp2_player[pp->owner].profile_choice].hits++;
							}
						}
						if(pp->owner != i)
						{
							pp2_player[pp->owner].total_hits++;
						}
					}
				}
			}
		}
	}
	if(pp->leaving)
	{
		pp->leaving--;
	}
	pp->tick++;
}

void pp2_paintball_render(PP2_PAINTBALL * pp, PP2_CAMERA * cp, PP2_RESOURCES * resources)
{
	ALLEGRO_COLOR tint_color;
	float hw, hh;

	if(pp->type == PP2_PAINTBALL_TYPE_GHOST)
	{
		tint_color = al_map_rgba_f(0.5, 0.5, 0.5, 0.5);
	}
	else
	{
		tint_color = al_map_rgba_f(1.0, 1.0, 1.0, 1.0);
	}
	if(pp->flags & PP2_PAINTBALL_FLAG_ACTIVE)
	{
		hw = pp->cx * 2 + PP2_PAINTBALL_HIGHLIGHT_SIZE * 2;
		hh = pp->cy * 2 + PP2_PAINTBALL_HIGHLIGHT_SIZE * 2;
		t3f_draw_scaled_bitmap(resources->bitmap[PP2_BITMAP_HIGHLIGHT], al_map_rgba_f(0.5, 0.5, 0.5, 0.5), pp->x - cp->x - PP2_PAINTBALL_HIGHLIGHT_SIZE, pp->y - cp->y - PP2_PAINTBALL_HIGHLIGHT_SIZE, pp->z - cp->z, hw, hh, 0);
		t3f_draw_rotated_animation(pp2_player[pp->owner].character->animation[pp2_player[pp->owner].character->state[pp->state].paintball.animation], tint_color, pp->tick, pp->cx, pp->cy, pp->x - cp->x + pp->cx, pp->y - cp->y + pp->cy, pp->z - cp->z, pp->angle, 0);
	}
}

void pp2_paintball_trail_logic(PP2_PAINTBALL_TRAIL * pp)
{
	if(pp->active && pp->counter > 0)
	{
		pp->counter--;
		if(pp->counter == 0)
		{
			pp->active = false;
		}
	}
}

void pp2_paintball_trail_render(PP2_PAINTBALL_TRAIL * pp, PP2_CAMERA * cp)
{
	float alpha;

	if(pp->active)
	{
		alpha = (float)pp->counter / (float)PP2_PAINTBALL_TRAIL_TIME;
		t3f_draw_rotated_animation(pp2_player[pp->owner].character->animation[pp2_player[pp->owner].character->state[pp2_player[pp->owner].state].paintball.animation], al_map_rgba_f(alpha, alpha, alpha, alpha), pp->tick, pp->cx, pp->cy, pp->x - cp->x + pp->cx, pp->y - cp->y + pp->cy, pp->z - cp->z, pp->angle, 0);
	}
}
