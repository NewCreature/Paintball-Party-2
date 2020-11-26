#include "t3f/t3f.h"
#include "game_struct.h"
#include "../resource.h"
#include "sprites/object_struct.h"
#include "sprites/object_defines.h"
#include "sprites/player.h"
#include "sprites/paintball_defines.h"
#include "game_defines.h"
#include "../interface/interface.h"
#include "../legacy/level.h"
#include "../legacy/character.h"
#include "sprites/objects.h"
#include "game.h"
#include "../file/music.h"
#include "replay.h"
#include "../misc/date.h"
#include "../init.h"

static void pp2_gameplay_play_music(PP2_GAME * gp, const char * fn)
{
	if(!gp->replay_rewind)
	{
		pp2_play_music(fn);
	}
}

int pp2_game_spawn_player(PP2_GAME * gp, PP2_PLAYER * pp, PP2_RESOURCES * resources)
{
	int i, chosen;
	int skipped = 0;
	PP2_SPAWN_POINT p[32];
	int pc = 0;
	int r;

	/* choose one of the available portals */
	if(gp->available_portals > 0)
	{
		for(i = 0; i < gp->available_portals; i++)
		{
			p[i].x = gp->available_portal[i].x;
			p[i].y = gp->available_portal[i].y;
			p[i].layer = gp->available_portal[i].layer;
		}
		pc = gp->available_portals;

		/* see if any portals are available */
		while(pc > 0)
		{
			r = joynet_rand();
			chosen = r % pc;
			pp->x = p[chosen].x + resources->object_animation[PP2_OBJECT_PORTAL]->frame[0]->width / 2 - (pp->object[0]->map.right.point[0].x - pp->object[0]->map.left.point[0].x) / 2 - pp->object[0]->map.left.point[0].x;
			pp->y = p[chosen].y + resources->object_animation[PP2_OBJECT_PORTAL]->frame[0]->height - pp->object[0]->map.bottom.point[0].y - 1;
			pp->z = p[chosen].z;
			pp->layer = p[chosen].layer;
			pp2_player_move_object_x(pp);
			pp2_player_move_object_x(pp);
			pp2_player_move_object_y(pp);
			pp2_player_move_object_y(pp);

			/* if portal not occupied, spawn player there */
			for(i = 0; i < PP2_MAX_PLAYERS; i++)
			{
				if(gp->player[i].flags & PP2_PLAYER_FLAG_ACTIVE)
				{
					if(t3f_check_object_collision(pp->object[0], gp->player[i].object[0]))
					{
						skipped++;
						break;
					}
				}
			}
			if(i == PP2_MAX_PLAYERS)
			{
				break;
			}
			for(i = chosen; i < pc - 1; i++)
			{
				p[i].x = p[i + 1].x;
				p[i].y = p[i + 1].y;
				p[i].z = p[i + 1].z;
				p[i].layer = p[i + 1].layer;
			}
			pc--;
		}
		if(skipped >= gp->available_portals)
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}

	/* put player at the new location */
	pp->vx = 0.0;
	pp->vy = 0.0;
	pp->flash_time = 0;
	pp->tick = 0;
	pp->current_object = 0;
	pp->reload_time = 0;
	for(i = 0; i < PP2_PLAYER_MAX_TIMERS; i++)
	{
		pp->timer[i] = 0;
	}
	pp->flags = PP2_PLAYER_FLAG_ACTIVE;
	pp->fade_time = 1;
	pp->fade_type = 1;
	pp->jumped_down = false;

	/* set up weapons */
	pp->ammo[PP2_PAINTBALL_TYPE_NORMAL] = gp->option[PP2_OPTION_START_AMMO_NORMAL];
	pp->ammo[PP2_PAINTBALL_TYPE_SPLITTER] = gp->option[PP2_OPTION_START_AMMO_X];
	pp->ammo[PP2_PAINTBALL_TYPE_MINE] = gp->option[PP2_OPTION_START_AMMO_MINE];
	pp->ammo[PP2_PAINTBALL_TYPE_BOUNCER] = gp->option[PP2_OPTION_START_AMMO_BOUNCE];
	pp->ammo[PP2_PAINTBALL_TYPE_SEEKER] = gp->option[PP2_OPTION_START_AMMO_SEEK];
	pp->ammo[PP2_PAINTBALL_TYPE_REFLECTOR] = gp->option[PP2_OPTION_START_AMMO_REFLECTOR];
	pp->ammo[PP2_PAINTBALL_TYPE_PMINE] = gp->option[PP2_OPTION_START_AMMO_PMINE];
	pp->ammo[PP2_PAINTBALL_TYPE_GHOST] = gp->option[PP2_OPTION_START_AMMO_GHOST];
	pp->weapon = 0;
	pp->last_weapon = 0;
	for(i = 0; i <= PP2_PAINTBALL_TYPE_GHOST; i++)
	{
		if(pp->ammo[i])
		{
			pp->weapon = i;
			break;
		}
	}

	pp->state = PP2_CHARACTER_STATE_STAND_R_R;
	for(i = 0; i < PP2_MAX_PAINTBALLS; i++)
	{
		pp->paintball[i].flags = 0;
	}
	for(i = 0; i < PP2_MAX_PARTICLES; i++)
	{
		pp->particle[i].flags = 0;
	}
	pp->flash_time = 0;
	pp->target = -1;

	/* mode specific stuff */
	switch(gp->option[PP2_OPTION_GAME_MODE])
	{
		case PP2_GAME_MODE_ELIMINATOR:
		{
			pp->life = gp->option[PP2_OPTION_ELIMINATION_HITS];
			break;
		}
		case PP2_GAME_MODE_DEATH_MATCH:
		case PP2_GAME_MODE_COIN_RUSH:
		{
			pp->life = gp->option[PP2_OPTION_LIFE];
			break;
		}
	}
	if(!gp->replay_rewind)
	{
		t3f_queue_sample(pp->character->sample[PP2_SAMPLE_START]);
		t3f_play_sample(pp->character->sample[PP2_SAMPLE_TELE_IN], 1.0, 0.0, 1.0);
	}
	return 1;
}

ALLEGRO_BITMAP * pp2_get_radar_image(PP2_GAME * gp, PP2_LEVEL * lp, int layer)
{
	ALLEGRO_BITMAP * bitmap = NULL;
	ALLEGRO_BITMAP * return_bitmap = NULL;
	ALLEGRO_STATE old_state;
	int width, height, size;
	int start_x, start_y, end_x, end_y;
	int i, j, tx, ty;

	/* create the radar bitmap */
	al_store_state(&old_state, ALLEGRO_STATE_NEW_BITMAP_PARAMETERS | ALLEGRO_STATE_TARGET_BITMAP);
	al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
	width = lp->room.bx - lp->room.x + 1 + 2;
	height = lp->room.by - lp->room.y + 1 + 2;
	gp->radar_offset_x = 1;
	gp->radar_offset_y = 1;
	if(width > height)
	{
		gp->radar_offset_y = (width - height) / 2 + 1;
		size = width;
	}
	else
	{
		gp->radar_offset_x = (height - width) / 2 + 1;
		size = height;
	}
	bitmap = al_create_bitmap(size, size);
	if(!bitmap)
	{
		return NULL;
	}

	/* draw the map on the radar image */
	al_set_target_bitmap(bitmap);
	al_clear_to_color(al_map_rgba_f(0.0, 0.5, 0.0, 1.0));
	start_x = lp->room.x;
	end_x = lp->room.bx + 1;
	if(end_x > lp->collision_tilemap[layer]->width)
	{
		end_x = lp->collision_tilemap[layer]->width;
	}
	start_y = lp->room.y;
	if(start_y < 0)
	{
		start_y = 0;
	}
	end_y = lp->room.by + 1;
	if(end_y > lp->collision_tilemap[layer]->height)
	{
		end_y = lp->collision_tilemap[layer]->height;
	}

	/* render radar image */
	for(i = start_y; i < end_y; i++)
	{
		for(j = start_x; j < end_x; j++)
		{
			if(lp->collision_tilemap[layer]->data[i][j].flags & T3F_COLLISION_FLAG_SOLID_BOTTOM &&
			        lp->collision_tilemap[layer]->data[i][j].flags & T3F_COLLISION_FLAG_SOLID_LEFT &&
			        lp->collision_tilemap[layer]->data[i][j].flags & T3F_COLLISION_FLAG_SOLID_RIGHT &&
			        lp->collision_tilemap[layer]->data[i][j].flags & T3F_COLLISION_FLAG_SOLID_TOP)
			{
				al_put_pixel(gp->radar_offset_x + j - start_x, gp->radar_offset_y + i - start_y, al_map_rgba_f(0.0, 1.0, 0.0, 1.0));
			}
			else if(lp->collision_tilemap[layer]->data[i][j].flags & PP2_LEVEL_COLLISION_FLAG_SECRET)
			{
				al_put_pixel(gp->radar_offset_x + j - start_x, gp->radar_offset_y + i - start_y, al_map_rgba_f(0.0, 1.0, 0.0, 1.0));
			}
			else if(lp->collision_tilemap[layer]->data[i][j].flags & (T3F_COLLISION_FLAG_SOLID_BOTTOM | T3F_COLLISION_FLAG_SOLID_LEFT | T3F_COLLISION_FLAG_SOLID_RIGHT | T3F_COLLISION_FLAG_SOLID_TOP))
			{
				al_put_pixel(gp->radar_offset_x + j - start_x, gp->radar_offset_y + i - start_y, al_map_rgba_f(0.0, 0.75, 0.0, 1.0));
			}
		}
	}
	for(i = 0; i < lp->objects; i++)
	{
		if(lp->object[i].layer == layer && lp->object[i].type == PP2_OBJECT_TILE_READER)
		{
			tx = (int)lp->object[i].x / lp->tileset->width;
			ty = (int)lp->object[i].y / lp->tileset->height;
			if(!(lp->collision_tilemap[layer]->data[ty][tx].flags & PP2_LEVEL_COLLISION_FLAG_SECRET))
			{
				al_put_pixel(gp->radar_offset_x + tx - start_x, gp->radar_offset_y + ty - start_y, al_map_rgba_f(0.0, 0.75, 0.0, 1.0));
			}
		}
	}
	al_draw_rectangle(gp->radar_offset_x - 1 + 0.5, gp->radar_offset_y - 1 + 0.5, gp->radar_offset_x + width - 2 + 0.5, gp->radar_offset_y + height - 2 + 0.5, al_map_rgba_f(0.0, 1.0, 0.0, 1.0), 1.0);

	/* restore state and get a bitmap of the correct type */
	al_restore_state(&old_state);
	return_bitmap = al_clone_bitmap(bitmap);
	al_destroy_bitmap(bitmap);

	return return_bitmap;
}

bool pp2_game_load_data(PP2_GAME * gp, PP2_INTERFACE * ip, PP2_RESOURCES * resources)
{
	int i, j;
	int entry;

	entry = pp2_database_find_entry(resources->level_database, ip->level_hash);
	if(entry < 0)
	{
		return false;
	}

	if(!strcasecmp(al_get_path_extension(resources->level_database->entry[entry]->path), ".p2l"))
	{
		gp->level = pp2_load_level((char *)al_path_cstr(resources->level_database->entry[entry]->path, '/'), 1);
	}
	else
	{
		gp->level = pp2_load_legacy_level((char *)al_path_cstr(resources->level_database->entry[entry]->path, '/'), 1);
	}
	if(!gp->level)
	{
		return false;
	}
	t3f_atlas_tileset(gp->level->tileset);
	pp2_fortify_level(gp->level);
	for(i = 0; i < gp->level->tilemap->layers; i++)
	{
		if(gp->level->collision_tilemap[i])
		{
			gp->radar_bitmap[i] = pp2_get_radar_image(gp, gp->level, i);
			if(!gp->radar_bitmap[i])
			{
				return false;
			}
		}
	}
	for(i = 0; i < PP2_MAX_PLAYERS; i++)
	{
		if(gp->player[i].playing)
		{
			/* load character */
			if(!strcmp(al_get_path_extension(resources->character_database->entry[gp->player[i].character_choice]->path), ".p2c"))
			{
				gp->player[i].character = pp2_load_character((char *)al_path_cstr(resources->character_database->entry[gp->player[i].character_choice]->path, '/'), 1);
			}
			else
			{
				gp->player[i].character = pp2_load_legacy_character((char *)al_path_cstr(resources->character_database->entry[gp->player[i].character_choice]->path, '/'), 1);
			}
			if(!gp->player[i].character)
			{
				printf("character fail!\n");
				return false;
			}

			/* fill in empty samples with defaults */
			for(j = 0; j < 64; j++)
			{
				if(!gp->player[i].character->sample[j])
				{
					gp->player[i].character->sample[j] = resources->sample[j];
				}
			}

			/* create collision objects */
			gp->player[i].object[0] = t3f_create_collision_object(gp->player[i].character->state[0].collision_x, gp->player[i].character->state[0].collision_y, gp->player[i].character->state[0].collision_w, gp->player[i].character->state[0].collision_h, 32, 32, 0);
			gp->player[i].object[1] = t3f_create_collision_object(gp->player[i].character->state[0].collision_x, gp->player[i].character->state[PP2_CHARACTER_STATE_DUCK_R_R].collision_y, gp->player[i].character->state[0].collision_w, gp->player[i].character->state[0].collision_h - (gp->player[i].character->state[PP2_CHARACTER_STATE_DUCK_R_R].collision_y - gp->player[i].character->state[0].collision_y), 32, 32, 0);
			if(gp->player[i].character->state[0].collision_w > 24)
			{
				gp->player[i].object[2] = t3f_create_collision_object(gp->player[i].character->state[0].collision_x + 12, gp->player[i].character->state[0].collision_y, gp->player[i].character->state[0].collision_w - 24, gp->player[i].character->state[0].collision_h, 32, 32, 0);
			}
			else
			{
				gp->player[i].object[2] = t3f_create_collision_object(gp->player[i].character->state[0].collision_x, gp->player[i].character->state[0].collision_y, gp->player[i].character->state[0].collision_w, gp->player[i].character->state[0].collision_h, 32, 32, 0);
			}
			for(j = 0; j < PP2_MAX_PAINTBALLS; j++)
			{
				gp->player[i].paintball[j].object = t3f_create_collision_object(0, 0, 32, 32, 32, 32, 0);
			}
		}
	}
	gp->object_size = gp->level->objects + 256;
	gp->object = malloc(sizeof(PP2_OBJECT) * (gp->object_size));
	if(!gp->object)
	{
		return false;
	}
	for(i = 0; i < gp->object_size; i++)
	{
		memset(&gp->object[i], 0, sizeof(PP2_OBJECT));
		gp->object[i].object = t3f_create_collision_object(0, 0, 32, 32, 32, 32, 0);
	}
	return true;
}

/* free in-game data */
void pp2_game_free_data(PP2_GAME * gp, PP2_RESOURCES * resources)
{
	int i, j;

	for(i = 0; i < gp->object_size; i++)
	{
		t3f_destroy_collision_object(gp->object[i].object);
	}
	free(gp->object);
	gp->object = NULL;
	for(i = 0; i < PP2_MAX_PLAYERS; i++)
	{
		if(gp->player[i].playing)
		{
			t3f_destroy_collision_object(gp->player[i].object[2]);
			t3f_destroy_collision_object(gp->player[i].object[1]);
			t3f_destroy_collision_object(gp->player[i].object[0]);
			for(j = 0; j < 64; j++)
			{
				/* remove pointers to default samples before destroying character */
				if(gp->player[i].character->sample[j] == resources->sample[j])
				{
					gp->player[i].character->sample[j] = NULL;
				}
			}
			pp2_destroy_character(gp->player[i].character);
			for(j = 0; j < PP2_MAX_PAINTBALLS; j++)
			{
				t3f_destroy_collision_object(gp->player[i].paintball[j].object);
			}
			t3f_destroy_view(gp->player[i].view);
		}
	}
	for(i = 0; i < gp->level->tilemap->layers; i++)
	{
		if(gp->radar_bitmap[i])
		{
			al_destroy_bitmap(gp->radar_bitmap[i]);
			gp->radar_bitmap[i] = NULL;
		}
	}
	pp2_destroy_level(gp->level);
}

bool pp2_game_setup(PP2_GAME * gp, int flags, PP2_INTERFACE * ip, PP2_RESOURCES * resources)
{
	char buf[1024];
	int i, r, o;
	int entry;
	int cx[4];
	int cy[4];
	int cx2[2];
	int cy2[2];
	int cheight;
	int cflags;
	int vwidth;
	int vheight;
	int c = 0;
	int local_player_count = 0;
	char * music_file = NULL;
	char tempfn[1024] = {0};

	if(gp->level->flags & PP2_LEVEL_FLAG_LEGACY)
	{
		vwidth = 640;
		vheight = 480;
		cflags = T3F_FORCE_ASPECT;
		cx[0] = t3f_default_view->left;
		cy[0] = t3f_default_view->top;
		cx[1] = PP2_SCREEN_WIDTH / 2;
		cy[1] = PP2_SCREEN_HEIGHT / 2;
		cx[2] = PP2_SCREEN_WIDTH / 2;
		cy[2] = t3f_default_view->top;
		cx[3] = t3f_default_view->left;
		cy[3] = PP2_SCREEN_HEIGHT / 2;
		cx2[0] = cx[0];
		cy2[0] = cy[0];
		cx2[1] = cx[1];
		cy2[1] = cy[1];
		cheight = PP2_SCREEN_VISIBLE_HEIGHT / 2;
	}
	else
	{
		vwidth = 960;
		vheight = 540;
		cflags = T3F_FORCE_ASPECT | T3F_FILL_SCREEN;
		cx[0] = t3f_default_view->left;
		cy[0] = t3f_default_view->top;
		cx[1] = PP2_SCREEN_WIDTH / 2;
		cy[1] = PP2_SCREEN_HEIGHT / 2;
		cx[2] = PP2_SCREEN_WIDTH / 2;
		cy[2] = t3f_default_view->top;
		cx[3] = t3f_default_view->left;
		cy[3] = PP2_SCREEN_HEIGHT / 2;
		cx2[0] = t3f_default_view->left;
		cy2[0] = 0;
		cx2[1] = PP2_SCREEN_WIDTH / 2;
		cy2[1] = 0;
		cheight = PP2_SCREEN_VISIBLE_HEIGHT;
	}

	joynet_srand(gp->seed);
	gp->replay_player = -2;
	gp->local_player = -1;
	gp->coins_needed = 0;
	for(i = 0; i < PP2_MAX_PLAYERS; i++)
	{
		gp->player[i].id = i;
		gp->player[i].view_port = -1;
	}
	if(gp->replay_file)
	{
		for(i = 0; i < PP2_MAX_PLAYERS; i++)
		{
			if(gp->player[i].playing)
			{
				gp->replay_player = i;
				gp->local_player = i;
				break;
			}
		}
		for(i = 0; i < PP2_MAX_PLAYERS; i++)
		{
			gp->player[i].view = t3f_create_view(t3f_default_view->left, t3f_default_view->top, PP2_SCREEN_VISIBLE_WIDTH, PP2_SCREEN_VISIBLE_HEIGHT, vwidth / 2, vheight / 2, cflags);
		}
	}
	else
	{
		/* get player names */
		for(i = 0; i < PP2_MAX_PLAYERS; i++)
		{
			if(gp->player[i].playing)
			{
				strcpy(gp->player[i].name, gp->client_game->player[i]->name);
			}
		}

		for(i = 0; i < PP2_MAX_PLAYERS; i++)
		{
			if(gp->player[i].playing && gp->client_game->player[i]->local)
			{
				local_player_count++;

				/* update profile */
				gp->player[i].profile->plays++;
			}
		}
		if(local_player_count == 0)
		{
			for(i = 0; i < PP2_MAX_PLAYERS; i++)
			{
				if(gp->player[i].playing)
				{
					gp->replay_player = i;
					break;
				}
			}
			for(i = 0; i < PP2_MAX_PLAYERS; i++)
			{
				gp->player[i].view = t3f_create_view(t3f_default_view->left, t3f_default_view->top, PP2_SCREEN_VISIBLE_WIDTH, PP2_SCREEN_VISIBLE_HEIGHT, vwidth / 2, vheight / 2, cflags);
			}
		}
		else if(local_player_count == 1)
		{
			for(i = 0; i < PP2_MAX_PLAYERS; i++)
			{
				if(gp->player[i].playing && gp->client_game->player[i]->local)
				{
					gp->player[i].view = t3f_create_view(t3f_default_view->left, t3f_default_view->top, PP2_SCREEN_VISIBLE_WIDTH, PP2_SCREEN_VISIBLE_HEIGHT, vwidth / 2, vheight / 2, cflags);
					gp->local_player = i;
				}
				else
				{
					gp->player[i].view = t3f_create_view(t3f_default_view->left, t3f_default_view->top, PP2_SCREEN_WIDTH, PP2_SCREEN_HEIGHT, vwidth / 2, vheight / 2, cflags);
				}
			}
		}
		else if(local_player_count == 2)
		{
			for(i = 0; i < PP2_MAX_PLAYERS; i++)
			{
				if(gp->player[i].playing)
				{
					if(gp->client_game->player[i]->local && c < 2)
					{
						gp->player[i].view = t3f_create_view(cx2[c], cy2[c], PP2_SCREEN_VISIBLE_WIDTH / 2, cheight, vwidth / 2, vheight / 2, cflags);
						gp->player[i].view_port = c;
						c++;
					}
					else
					{
						gp->player[i].view = t3f_create_view(t3f_default_view->left, t3f_default_view->top, PP2_SCREEN_WIDTH, PP2_SCREEN_HEIGHT, vwidth / 2, vheight / 2, cflags);
					}
				}
			}
		}
		else
		{
			for(i = 0; i < PP2_MAX_PLAYERS; i++)
			{
				if(gp->player[i].playing)
				{
					if(gp->client_game->player[i]->local && c < 4)
					{
						gp->player[i].view = t3f_create_view(cx[c], cy[c], PP2_SCREEN_VISIBLE_WIDTH / 2, PP2_SCREEN_VISIBLE_HEIGHT / 2, vwidth / 2, vheight / 2, cflags);
						gp->player[i].view_port = c;
						c++;
					}
					else
					{
						gp->player[i].view = t3f_create_view(t3f_default_view->left, t3f_default_view->top, al_get_display_width(t3f_display), al_get_display_height(t3f_display), vwidth / 2, vheight / 2, cflags);
					}
				}
			}
		}
	}
	for(i = 0; i < PP2_MAX_PLAYERS; i++)
	{
		if(gp->player[i].playing)
		{
			t3f_set_view_virtual_dimensions(gp->player[i].view, vwidth, vheight);
			t3f_select_view(gp->player[i].view);
		}
	}

	/* reset player variables */
	for(i = 0; i < PP2_MAX_PLAYERS; i++)
	{
		gp->player[i].flags = 0;
		gp->player[i].hits = 0;
		gp->player[i].frags = 0;
		gp->player[i].shots = 0;
		gp->player[i].total_hits = 0;
		gp->player[i].shot = 0;
		if(!gp->replay_file)
		{
			gp->player[i].camera.z = 120.0;
		}
	}

	/* reset objects */
	for(i = 0; i < gp->object_size; i++)
	{
		gp->object[i].flags = 0;
	}
	/* set up objects */
	for(i = 0; i < gp->level->objects; i++)
	{
		pp2_generate_object(gp, gp->level->object[i].x, gp->level->object[i].y, gp->level->object[i].layer, gp->level->object[i].type, gp->level->object[i].flags, 0);
	}

	/* set up portal list */
	gp->available_portals = 0;
	for(i = 0; i < gp->object_size; i++)
	{
		if((gp->object[i].flags & PP2_OBJECT_FLAG_ACTIVE) && gp->object[i].type == PP2_OBJECT_PORTAL && gp->available_portals < 32)
		{
			gp->available_portal[gp->available_portals].x = gp->object[i].x;
			gp->available_portal[gp->available_portals].y = gp->object[i].y;
			gp->available_portal[gp->available_portals].z = gp->level->tilemap->layer[gp->object[i].layer]->z;
			gp->available_portal[gp->available_portals].layer = gp->object[i].layer;
			gp->available_portals++;
		}
	}

	/* spawn players */
	for(i = 0; i < PP2_MAX_PLAYERS; i++)
	{
		if(gp->player[i].playing)
		{
			if(!pp2_game_spawn_player(gp, &gp->player[i], resources))
			{
				return false;
			}
			t3f_clear_controller_state(ip->controller[i]);
			gp->player[i].id = i;
			pp2_camera_logic(gp, i);
			gp->player[i].coins = 0;
		}
	}
	if(gp->option[PP2_OPTION_GAME_MODE] == PP2_GAME_MODE_COIN_RUSH)
	{
		gp->coins_needed = gp->available_portals;
	}

	if(!gp->option[PP2_OPTION_RANDOMIZE_ITEMS])
	{
		for(i = 0; i < gp->object_size; i++)
		{
			if(gp->object[i].flags & PP2_OBJECT_FLAG_ACTIVE)
			{
				switch(gp->object[i].type)
				{
					case PP2_OBJECT_AMMO_NORMAL:
					{
						if(!gp->option[PP2_OPTION_ENABLE_AMMO_NORMAL])
						{
							gp->object[i].flags = 0;
						}
						break;
					}
					case PP2_OBJECT_AMMO_X:
					{
						if(!gp->option[PP2_OPTION_ENABLE_AMMO_X])
						{
							gp->object[i].flags = 0;
						}
						break;
					}
					case PP2_OBJECT_AMMO_MINE:
					{
						if(!gp->option[PP2_OPTION_ENABLE_AMMO_MINE])
						{
							gp->object[i].flags = 0;
						}
						break;
					}
					case PP2_OBJECT_AMMO_BOUNCE:
					{
						if(!gp->option[PP2_OPTION_ENABLE_AMMO_BOUNCE])
						{
							gp->object[i].flags = 0;
						}
						break;
					}
					case PP2_OBJECT_AMMO_SEEK:
					{
						if(!gp->option[PP2_OPTION_ENABLE_AMMO_SEEK])
						{
							gp->object[i].flags = 0;
						}
						break;
					}
					case PP2_OBJECT_AMMO_REFLECTOR:
					{
						if(!gp->option[PP2_OPTION_ENABLE_AMMO_REFLECTOR])
						{
							gp->object[i].flags = 0;
						}
						break;
					}
					case PP2_OBJECT_AMMO_PMINE:
					{
						if(!gp->option[PP2_OPTION_ENABLE_AMMO_PMINE])
						{
							gp->object[i].flags = 0;
						}
						break;
					}
					case PP2_OBJECT_AMMO_GHOST:
					{
						if(!gp->option[PP2_OPTION_ENABLE_AMMO_GHOST])
						{
							gp->object[i].flags = 0;
						}
						break;
					}
					case PP2_OBJECT_POWER_CLOAK:
					{
						if(!gp->option[PP2_OPTION_ENABLE_POWERUP_CLOAK])
						{
							gp->object[i].flags = 0;
						}
						break;
					}
					case PP2_OBJECT_POWER_JUMP:
					{
						if(!gp->option[PP2_OPTION_ENABLE_POWERUP_JUMP])
						{
							gp->object[i].flags = 0;
						}
						break;
					}
					case PP2_OBJECT_POWER_RUN:
					{
						if(!gp->option[PP2_OPTION_ENABLE_POWERUP_RUN])
						{
							gp->object[i].flags = 0;
						}
						break;
					}
					case PP2_OBJECT_POWER_DEFLECT:
					{
						if(!gp->option[PP2_OPTION_ENABLE_POWERUP_DEFLECT])
						{
							gp->object[i].flags = 0;
						}
						break;
					}
					case PP2_OBJECT_POWER_FLY:
					{
						if(!gp->option[PP2_OPTION_ENABLE_POWERUP_FLY])
						{
							gp->object[i].flags = 0;
						}
						break;
					}
					case PP2_OBJECT_POWER_TURBO:
					{
						if(!gp->option[PP2_OPTION_ENABLE_POWERUP_TURBO])
						{
							gp->object[i].flags = 0;
						}
						break;
					}
					case PP2_OBJECT_PORTAL:
					{
						gp->object[i].flags = 0;
						break;
					}
				}
			}
		}
	}
	else
	{
		int ilist[20] = {0};
		int ilist_size = 0;

		if(gp->option[PP2_OPTION_ENABLE_AMMO_NORMAL])
		{
			ilist[ilist_size] = PP2_OBJECT_AMMO_NORMAL;
			ilist_size++;
		}
		if(gp->option[PP2_OPTION_ENABLE_AMMO_X])
		{
			ilist[ilist_size] = PP2_OBJECT_AMMO_X;
			ilist_size++;
		}
		if(gp->option[PP2_OPTION_ENABLE_AMMO_MINE])
		{
			ilist[ilist_size] = PP2_OBJECT_AMMO_MINE;
			ilist_size++;
		}
		if(gp->option[PP2_OPTION_ENABLE_AMMO_BOUNCE])
		{
			ilist[ilist_size] = PP2_OBJECT_AMMO_BOUNCE;
			ilist_size++;
		}
		if(gp->option[PP2_OPTION_ENABLE_AMMO_SEEK])
		{
			ilist[ilist_size] = PP2_OBJECT_AMMO_SEEK;
			ilist_size++;
		}
		if(gp->option[PP2_OPTION_ENABLE_AMMO_REFLECTOR])
		{
			ilist[ilist_size] = PP2_OBJECT_AMMO_REFLECTOR;
			ilist_size++;
		}
		if(gp->option[PP2_OPTION_ENABLE_AMMO_PMINE])
		{
			ilist[ilist_size] = PP2_OBJECT_AMMO_PMINE;
			ilist_size++;
		}
		if(gp->option[PP2_OPTION_ENABLE_AMMO_GHOST])
		{
			ilist[ilist_size] = PP2_OBJECT_AMMO_GHOST;
			ilist_size++;
		}
		if(gp->option[PP2_OPTION_ENABLE_POWERUP_CLOAK])
		{
			ilist[ilist_size] = PP2_OBJECT_POWER_CLOAK;
			ilist_size++;
		}
		if(gp->option[PP2_OPTION_ENABLE_POWERUP_JUMP])
		{
			ilist[ilist_size] = PP2_OBJECT_POWER_JUMP;
			ilist_size++;
		}
		if(gp->option[PP2_OPTION_ENABLE_POWERUP_RUN])
		{
			ilist[ilist_size] = PP2_OBJECT_POWER_RUN;
			ilist_size++;
		}
		if(gp->option[PP2_OPTION_ENABLE_POWERUP_DEFLECT])
		{
			ilist[ilist_size] = PP2_OBJECT_POWER_DEFLECT;
			ilist_size++;
		}
		if(gp->option[PP2_OPTION_ENABLE_POWERUP_FLY])
		{
			ilist[ilist_size] = PP2_OBJECT_POWER_FLY;
			ilist_size++;
		}
		if(gp->option[PP2_OPTION_ENABLE_POWERUP_TURBO])
		{
			ilist[ilist_size] = PP2_OBJECT_POWER_TURBO;
			ilist_size++;
		}

		for(i = 0; i < gp->object_size; i++)
		{
			if(gp->object[i].flags & PP2_OBJECT_FLAG_ACTIVE)
			{
				switch(gp->object[i].type)
				{
					case PP2_OBJECT_AMMO_NORMAL:
					case PP2_OBJECT_AMMO_X:
					case PP2_OBJECT_AMMO_MINE:
					case PP2_OBJECT_AMMO_BOUNCE:
					case PP2_OBJECT_AMMO_SEEK:
					case PP2_OBJECT_AMMO_REFLECTOR:
					case PP2_OBJECT_AMMO_PMINE:
					case PP2_OBJECT_AMMO_GHOST:
					case PP2_OBJECT_POWER_CLOAK:
					case PP2_OBJECT_POWER_JUMP:
					case PP2_OBJECT_POWER_RUN:
					case PP2_OBJECT_POWER_DEFLECT:
					case PP2_OBJECT_POWER_FLY:
					case PP2_OBJECT_POWER_TURBO:
					{
						r = joynet_rand() % ilist_size;
						gp->object[i].type = ilist[r];
						break;
					}
				}
			}
		}
	}

	for(i = 0; i < gp->object_size; i++)
	{
		if(gp->object[i].flags & PP2_OBJECT_FLAG_ACTIVE && gp->object[i].type == PP2_OBJECT_PORTAL)
		{
			gp->object[i].flags = 0;
		}
	}

	switch(gp->option[PP2_OPTION_GAME_MODE])
	{
		case PP2_GAME_MODE_ELIMINATOR:
		{
			break;
		}
		case PP2_GAME_MODE_DEATH_MATCH:
		{
			gp->time_left = gp->option[PP2_OPTION_TIME_LIMIT] * 3600;
			break;
		}
		case PP2_GAME_MODE_COIN_RUSH:
		{
			gp->time_left = gp->option[PP2_OPTION_TIME_LIMIT] * 3600;
			for(i = 0; i < gp->available_portals; i++)
			{
				o = pp2_generate_object(gp, gp->available_portal[i].x + resources->object_animation[PP2_OBJECT_PORTAL]->frame[0]->width / 2 - resources->object_animation[PP2_OBJECT_COIN]->frame[0]->width / 2, gp->available_portal[i].y + resources->object_animation[PP2_OBJECT_PORTAL]->frame[0]->height / 2 - resources->object_animation[PP2_OBJECT_COIN]->frame[0]->height / 2, gp->available_portal[i].layer, PP2_OBJECT_COIN, PP2_OBJECT_FLAG_ACTIVE, 0);
				if(o >= 0)
				{
					gp->object[o].vx = 0.0;
					gp->object[o].vy = 0.0;
					t3f_recreate_collision_object(gp->object[o].object, 0, 0, 16, 16, 32, 32, 0);
					t3f_move_collision_object_xy(gp->object[o].object, gp->object[i].x, gp->object[i].y);
				}
				else
				{
					printf("failed to generate coin\n");
				}
			}
			break;
		}
	}
	gp->winner = -1;
	r = joynet_rand(); // random number in case we need to pick random music
	entry = pp2_database_find_entry(resources->level_database, ip->level_hash);
	if(entry < 0)
	{
		entry = 0;
	}
	if(flags & PP2_GAME_INIT_FLAG_CAPTURE)
	{
		t3f_stop_music();
	}
	music_file = pp2_find_music((char *)al_path_cstr(resources->level_database->entry[entry]->path, '/'));
	if(music_file)
	{
		if(!(gp->replay_flags & PP2_REPLAY_FLAG_DEMO) && !(!(gp->replay_flags & PP2_REPLAY_FLAG_DEMO)))
		{
			pp2_gameplay_play_music(gp, music_file);
		}
	}
	else
	{
		if(!(gp->replay_flags & PP2_REPLAY_FLAG_DEMO) && !(gp->replay_flags & PP2_REPLAY_FLAG_THEATER))
		{
			pp2_gameplay_play_music(gp, (char *)al_path_cstr(resources->music_database->entry[r % resources->music_database->entries]->path, '/'));
		}
	}
	if(!gp->replay_file)
	{
		if(gp->replay_player == -2)
		{
			gp->replay_player = -1;
		}
		sprintf(tempfn, "replays/%s.p2r", pp2_get_date_string());
		pp2_record_replay(gp, t3f_get_filename(t3f_data_path, tempfn, buf, 1024), resources);
		al_hide_mouse_cursor(t3f_display);
	}
	gp->tick = 0;
	if(!(flags & PP2_GAME_INIT_FLAG_CAPTURE))
	{
		al_start_timer(t3f_timer);
	}
	return true;
}

bool pp2_game_init(PP2_GAME * gp, int flags, PP2_INTERFACE * ip, PP2_RESOURCES * resources)
{
	ALLEGRO_TRANSFORM identity_transform;

	if(!(flags & PP2_GAME_INIT_FLAG_CAPTURE))
	{
		al_stop_timer(t3f_timer);
	}

	/* if we are watching a replay, show the first player */
	al_identity_transform(&identity_transform);
	al_use_transform(&identity_transform);
	if(gp->replay_file)
	{
		if(gp->replay_flags & PP2_REPLAY_FLAG_DEMO)
		{
			pp2_show_load_screen("Loading demo", resources);
		}
		else
		{
			pp2_show_load_screen("Loading replay", resources);
		}
	}
	else
	{
		pp2_show_load_screen("Loading game", resources);
	}

	if(!pp2_game_load_data(gp, ip, resources))
	{
		printf("data fail!\n");
		return false;
	}
	gp->next_state = 0;
	return pp2_game_setup(gp, flags, ip, resources);
}
