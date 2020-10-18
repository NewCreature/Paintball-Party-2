#include "t3f/tilemap.h"
#include "t3f/t3f.h"
#include "t3f/sound.h"
#include "t3f/draw.h"
#include "../joynet/joynet.h"
#include "../legacy/level.h"
#include "../legacy/character.h"
#include "../file/music.h"
#include "../data.h"
#include "../init.h"
#include "../tables.h"
#include "../misc/date.h"
#include "../interface/menu.h"
#include "sprites/player.h"
#include "sprites/paintball.h"
#include "sprites/particle.h"
#include "game.h"
#include "rules.h"
#include "scoreboard.h"
#include "replay.h"
#include "../resource.h"
#include "../pp2.h"

char * chared_state_name[PP2_CHARACTER_MAX_STATES] =
{
	"PP2_CHARACTER_STATE_STAND_R_R",
	"PP2_CHARACTER_STATE_STAND_R_DR",
	"PP2_CHARACTER_STATE_STAND_R_D",
	"PP2_CHARACTER_STATE_STAND_R_DL",
	"PP2_CHARACTER_STATE_STAND_R_L",
	"PP2_CHARACTER_STATE_STAND_R_UL",
	"PP2_CHARACTER_STATE_STAND_R_U",
	"PP2_CHARACTER_STATE_STAND_R_UR",
	"PP2_CHARACTER_STATE_STAND_L_R",
	"PP2_CHARACTER_STATE_STAND_L_DR",
	"PP2_CHARACTER_STATE_STAND_L_D",
	"PP2_CHARACTER_STATE_STAND_L_DL",
	"PP2_CHARACTER_STATE_STAND_L_L",
	"PP2_CHARACTER_STATE_STAND_L_UL",
	"PP2_CHARACTER_STATE_STAND_L_U",
	"PP2_CHARACTER_STATE_STAND_L_UR",
	"PP2_CHARACTER_STATE_WALK_R_R",
	"PP2_CHARACTER_STATE_WALK_R_DR",
	"PP2_CHARACTER_STATE_WALK_R_D",
	"PP2_CHARACTER_STATE_WALK_R_DL",
	"PP2_CHARACTER_STATE_WALK_R_L",
	"PP2_CHARACTER_STATE_WALK_R_UL",
	"PP2_CHARACTER_STATE_WALK_R_U",
	"PP2_CHARACTER_STATE_WALK_R_UR",
	"PP2_CHARACTER_STATE_WALK_L_R",
	"PP2_CHARACTER_STATE_WALK_L_DR",
	"PP2_CHARACTER_STATE_WALK_L_D",
	"PP2_CHARACTER_STATE_WALK_L_DL",
	"PP2_CHARACTER_STATE_WALK_L_L",
	"PP2_CHARACTER_STATE_WALK_L_UL",
	"PP2_CHARACTER_STATE_WALK_L_U",
	"PP2_CHARACTER_STATE_WALK_L_UR",
	"PP2_CHARACTER_STATE_JUMP_R_R",
	"PP2_CHARACTER_STATE_JUMP_R_DR",
	"PP2_CHARACTER_STATE_JUMP_R_D",
	"PP2_CHARACTER_STATE_JUMP_R_DL",
	"PP2_CHARACTER_STATE_JUMP_R_L",
	"PP2_CHARACTER_STATE_JUMP_R_UL",
	"PP2_CHARACTER_STATE_JUMP_R_U",
	"PP2_CHARACTER_STATE_JUMP_R_UR",
	"PP2_CHARACTER_STATE_JUMP_L_R",
	"PP2_CHARACTER_STATE_JUMP_L_DR",
	"PP2_CHARACTER_STATE_JUMP_L_D",
	"PP2_CHARACTER_STATE_JUMP_L_DL",
	"PP2_CHARACTER_STATE_JUMP_L_L",
	"PP2_CHARACTER_STATE_JUMP_L_UL",
	"PP2_CHARACTER_STATE_JUMP_L_U",
	"PP2_CHARACTER_STATE_JUMP_L_UR",
	"PP2_CHARACTER_STATE_FALL_R_R",
	"PP2_CHARACTER_STATE_FALL_R_DR",
	"PP2_CHARACTER_STATE_FALL_R_D",
	"PP2_CHARACTER_STATE_FALL_R_DL",
	"PP2_CHARACTER_STATE_FALL_R_L",
	"PP2_CHARACTER_STATE_FALL_R_UL",
	"PP2_CHARACTER_STATE_FALL_R_U",
	"PP2_CHARACTER_STATE_FALL_R_UR",
	"PP2_CHARACTER_STATE_FALL_L_R",
	"PP2_CHARACTER_STATE_FALL_L_DR",
	"PP2_CHARACTER_STATE_FALL_L_D",
	"PP2_CHARACTER_STATE_FALL_L_DL",
	"PP2_CHARACTER_STATE_FALL_L_L",
	"PP2_CHARACTER_STATE_FALL_L_UL",
	"PP2_CHARACTER_STATE_FALL_L_U",
	"PP2_CHARACTER_STATE_FALL_L_UR",
	"PP2_CHARACTER_STATE_DUCK_R_R",
	"PP2_CHARACTER_STATE_DUCK_R_DR",
	"PP2_CHARACTER_STATE_DUCK_R_D",
	"PP2_CHARACTER_STATE_DUCK_R_DL",
	"PP2_CHARACTER_STATE_DUCK_R_L",
	"PP2_CHARACTER_STATE_DUCK_R_UL",
	"PP2_CHARACTER_STATE_DUCK_R_U",
	"PP2_CHARACTER_STATE_DUCK_R_UR",
	"PP2_CHARACTER_STATE_DUCK_L_R",
	"PP2_CHARACTER_STATE_DUCK_L_DR",
	"PP2_CHARACTER_STATE_DUCK_L_D",
	"PP2_CHARACTER_STATE_DUCK_L_DL",
	"PP2_CHARACTER_STATE_DUCK_L_L",
	"PP2_CHARACTER_STATE_DUCK_L_UL",
	"PP2_CHARACTER_STATE_DUCK_L_U",
	"PP2_CHARACTER_STATE_DUCK_L_UR"
};

typedef struct
{

	float x, y, z;
	int layer;

} PP2_SPAWN_POINT;

static PP2_SPAWN_POINT available_portal[32];
static int available_portals = 0;
static bool show_scores = false;

int pp2_game_spawn_player(PP2_PLAYER * pp)
{
	int i, chosen;
	int skipped = 0;
	PP2_SPAWN_POINT p[32];
	int pc = 0;
	int r;

	/* choose one of the available portals */
	if(available_portals > 0)
	{
		for(i = 0; i < available_portals; i++)
		{
			p[i].x = available_portal[i].x;
			p[i].y = available_portal[i].y;
			p[i].layer = available_portal[i].layer;
		}
		pc = available_portals;

		/* see if any portals are available */
		while(pc > 0)
		{
			r = joynet_rand();
			chosen = r % pc;
			pp->x = p[chosen].x + pp2_object_animation[PP2_OBJECT_PORTAL]->frame[0]->width / 2 - (pp->object[0]->map.right.point[0].x - pp->object[0]->map.left.point[0].x) / 2 - pp->object[0]->map.left.point[0].x;
			pp->y = p[chosen].y + pp2_object_animation[PP2_OBJECT_PORTAL]->frame[0]->height - pp->object[0]->map.bottom.point[0].y - 1;
			pp->z = p[chosen].z;
			pp->layer = p[chosen].layer;
			pp2_player_move_object_x(pp);
			pp2_player_move_object_x(pp);
			pp2_player_move_object_y(pp);
			pp2_player_move_object_y(pp);

			/* if portal not occupied, spawn player there */
			for(i = 0; i < PP2_MAX_PLAYERS; i++)
			{
				if(pp2_player[i].flags & PP2_PLAYER_FLAG_ACTIVE)
				{
					if(t3f_check_object_collision(pp->object[0], pp2_player[i].object[0]))
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
		if(skipped >= available_portals)
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
	pp->ammo[PP2_PAINTBALL_TYPE_NORMAL] = pp2_option[PP2_OPTION_START_AMMO_NORMAL];
	pp->ammo[PP2_PAINTBALL_TYPE_SPLITTER] = pp2_option[PP2_OPTION_START_AMMO_X];
	pp->ammo[PP2_PAINTBALL_TYPE_MINE] = pp2_option[PP2_OPTION_START_AMMO_MINE];
	pp->ammo[PP2_PAINTBALL_TYPE_BOUNCER] = pp2_option[PP2_OPTION_START_AMMO_BOUNCE];
	pp->ammo[PP2_PAINTBALL_TYPE_SEEKER] = pp2_option[PP2_OPTION_START_AMMO_SEEK];
	pp->ammo[PP2_PAINTBALL_TYPE_REFLECTOR] = pp2_option[PP2_OPTION_START_AMMO_REFLECTOR];
	pp->ammo[PP2_PAINTBALL_TYPE_PMINE] = pp2_option[PP2_OPTION_START_AMMO_PMINE];
	pp->ammo[PP2_PAINTBALL_TYPE_GHOST] = pp2_option[PP2_OPTION_START_AMMO_GHOST];
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
	switch(pp2_option[PP2_OPTION_GAME_MODE])
	{
		case PP2_GAME_MODE_ELIMINATOR:
		{
			pp->life = pp2_option[PP2_OPTION_ELIMINATION_HITS];
			break;
		}
		case PP2_GAME_MODE_DEATH_MATCH:
		case PP2_GAME_MODE_COIN_RUSH:
		{
			pp->life = pp2_option[PP2_OPTION_LIFE];
			break;
		}
	}
	if(!pp2_replay_rewind)
	{
		t3f_queue_sample(pp->character->sample[PP2_SAMPLE_START]);
		t3f_play_sample(pp->character->sample[PP2_SAMPLE_TELE_IN], 1.0, 0.0, 1.0);
	}
	return 1;
}

ALLEGRO_BITMAP * pp2_get_radar_image(PP2_LEVEL * lp, int layer)
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
	pp2_radar_offset_x = 1;
	pp2_radar_offset_y = 1;
	if(width > height)
	{
		pp2_radar_offset_y = (width - height) / 2 + 1;
		size = width;
	}
	else
	{
		pp2_radar_offset_x = (height - width) / 2 + 1;
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
				al_put_pixel(pp2_radar_offset_x + j - start_x, pp2_radar_offset_y + i - start_y, al_map_rgba_f(0.0, 1.0, 0.0, 1.0));
			}
			else if(lp->collision_tilemap[layer]->data[i][j].flags & PP2_LEVEL_COLLISION_FLAG_SECRET)
			{
				al_put_pixel(pp2_radar_offset_x + j - start_x, pp2_radar_offset_y + i - start_y, al_map_rgba_f(0.0, 1.0, 0.0, 1.0));
			}
			else if(lp->collision_tilemap[layer]->data[i][j].flags & (T3F_COLLISION_FLAG_SOLID_BOTTOM | T3F_COLLISION_FLAG_SOLID_LEFT | T3F_COLLISION_FLAG_SOLID_RIGHT | T3F_COLLISION_FLAG_SOLID_TOP))
			{
				al_put_pixel(pp2_radar_offset_x + j - start_x, pp2_radar_offset_y + i - start_y, al_map_rgba_f(0.0, 0.75, 0.0, 1.0));
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
				al_put_pixel(pp2_radar_offset_x + tx - start_x, pp2_radar_offset_y + ty - start_y, al_map_rgba_f(0.0, 0.75, 0.0, 1.0));
			}
		}
	}
	al_draw_rectangle(pp2_radar_offset_x - 1 + 0.5, pp2_radar_offset_y - 1 + 0.5, pp2_radar_offset_x + width - 2 + 0.5, pp2_radar_offset_y + height - 2 + 0.5, al_map_rgba_f(0.0, 1.0, 0.0, 1.0), 1.0);

	/* restore state and get a bitmap of the correct type */
	al_restore_state(&old_state);
	return_bitmap = al_clone_bitmap(bitmap);
	al_destroy_bitmap(bitmap);

	return return_bitmap;
}

bool pp2_game_load_data(void)
{
	int i, j;
	int entry;

	entry = pp2_database_find_entry(pp2_level_database, pp2_level_hash);
	if(entry < 0)
	{
		return false;
	}

	if(!strcasecmp(al_get_path_extension(pp2_level_database->entry[entry]->path), ".p2l"))
	{
		pp2_level = pp2_load_level((char *)al_path_cstr(pp2_level_database->entry[entry]->path, '/'), 1);
	}
	else
	{
		pp2_level = pp2_load_legacy_level((char *)al_path_cstr(pp2_level_database->entry[entry]->path, '/'), 1);
	}
	if(!pp2_level)
	{
		return false;
	}
	t3f_atlas_tileset(pp2_level->tileset);
	pp2_fortify_level(pp2_level);
	for(i = 0; i < pp2_level->tilemap->layers; i++)
	{
		if(pp2_level->collision_tilemap[i])
		{
			pp2_radar_bitmap[i] = pp2_get_radar_image(pp2_level, i);
			if(!pp2_radar_bitmap[i])
			{
				return false;
			}
		}
	}
	for(i = 0; i < PP2_MAX_PLAYERS; i++)
	{
		if(pp2_player[i].playing)
		{
			/* load character */
			if(!strcmp(al_get_path_extension(pp2_character_database->entry[pp2_player[i].character_choice]->path), ".p2c"))
			{
				pp2_player[i].character = pp2_load_character((char *)al_path_cstr(pp2_character_database->entry[pp2_player[i].character_choice]->path, '/'), 1);
			}
			else
			{
				pp2_player[i].character = pp2_load_legacy_character((char *)al_path_cstr(pp2_character_database->entry[pp2_player[i].character_choice]->path, '/'), 1);
			}
			if(!pp2_player[i].character)
			{
				printf("character fail!\n");
				return false;
			}

			/* fill in empty samples with defaults */
			for(j = 0; j < 64; j++)
			{
				if(!pp2_player[i].character->sample[j])
				{
					pp2_player[i].character->sample[j] = pp2_sample[j];
				}
			}

			/* create collision objects */
			pp2_player[i].object[0] = t3f_create_collision_object(pp2_player[i].character->state[0].collision_x, pp2_player[i].character->state[0].collision_y, pp2_player[i].character->state[0].collision_w, pp2_player[i].character->state[0].collision_h, 32, 32, 0);
			pp2_player[i].object[1] = t3f_create_collision_object(pp2_player[i].character->state[0].collision_x, pp2_player[i].character->state[PP2_CHARACTER_STATE_DUCK_R_R].collision_y, pp2_player[i].character->state[0].collision_w, pp2_player[i].character->state[0].collision_h - (pp2_player[i].character->state[PP2_CHARACTER_STATE_DUCK_R_R].collision_y - pp2_player[i].character->state[0].collision_y), 32, 32, 0);
			if(pp2_player[i].character->state[0].collision_w > 24)
			{
				pp2_player[i].object[2] = t3f_create_collision_object(pp2_player[i].character->state[0].collision_x + 12, pp2_player[i].character->state[0].collision_y, pp2_player[i].character->state[0].collision_w - 24, pp2_player[i].character->state[0].collision_h, 32, 32, 0);
			}
			else
			{
				pp2_player[i].object[2] = t3f_create_collision_object(pp2_player[i].character->state[0].collision_x, pp2_player[i].character->state[0].collision_y, pp2_player[i].character->state[0].collision_w, pp2_player[i].character->state[0].collision_h, 32, 32, 0);
			}
			for(j = 0; j < PP2_MAX_PAINTBALLS; j++)
			{
				pp2_player[i].paintball[j].object = t3f_create_collision_object(0, 0, 32, 32, 32, 32, 0);
			}
		}
	}
	pp2_object_size = pp2_level->objects + 256;
	pp2_object = malloc(sizeof(PP2_OBJECT) * (pp2_object_size));
	if(!pp2_object)
	{
		return false;
	}
	for(i = 0; i < pp2_object_size; i++)
	{
		memset(&pp2_object[i], 0, sizeof(PP2_OBJECT));
		pp2_object[i].object = t3f_create_collision_object(0, 0, 32, 32, 32, 32, 0);
	}
	return true;
}

/* free in-game data */
void pp2_game_free_data(void)
{
	int i, j;

	for(i = 0; i < pp2_object_size; i++)
	{
		t3f_destroy_collision_object(pp2_object[i].object);
	}
	free(pp2_object);
	pp2_object = NULL;
	for(i = 0; i < PP2_MAX_PLAYERS; i++)
	{
		if(pp2_player[i].playing)
		{
			t3f_destroy_collision_object(pp2_player[i].object[2]);
			t3f_destroy_collision_object(pp2_player[i].object[1]);
			t3f_destroy_collision_object(pp2_player[i].object[0]);
			for(j = 0; j < 64; j++)
			{
				/* remove pointers to default samples before destroying character */
				if(pp2_player[i].character->sample[j] == pp2_sample[j])
				{
					pp2_player[i].character->sample[j] = NULL;
				}
			}
			pp2_destroy_character(pp2_player[i].character);
			for(j = 0; j < PP2_MAX_PAINTBALLS; j++)
			{
				t3f_destroy_collision_object(pp2_player[i].paintball[j].object);
			}
			t3f_destroy_view(pp2_player[i].view);
		}
	}
	for(i = 0; i < pp2_level->tilemap->layers; i++)
	{
		if(pp2_radar_bitmap[i])
		{
			al_destroy_bitmap(pp2_radar_bitmap[i]);
			pp2_radar_bitmap[i] = NULL;
		}
	}
	pp2_destroy_level(pp2_level);
}

bool pp2_game_setup(int flags)
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

	if(pp2_level->flags & PP2_LEVEL_FLAG_LEGACY)
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

	joynet_srand(pp2_seed);
	pp2_replay_player = -2;
	pp2_local_player = -1;
	pp2_coins_needed = 0;
	for(i = 0; i < PP2_MAX_PLAYERS; i++)
	{
		pp2_player[i].id = i;
		pp2_player[i].view_port = -1;
	}
	if(pp2_replay_file)
	{
		for(i = 0; i < PP2_MAX_PLAYERS; i++)
		{
			if(pp2_player[i].playing)
			{
				pp2_replay_player = i;
				pp2_local_player = i;
				break;
			}
		}
		for(i = 0; i < PP2_MAX_PLAYERS; i++)
		{
			pp2_player[i].view = t3f_create_view(t3f_default_view->left, t3f_default_view->top, PP2_SCREEN_VISIBLE_WIDTH, PP2_SCREEN_VISIBLE_HEIGHT, vwidth / 2, vheight / 2, cflags);
		}
	}
	else
	{
		/* get player names */
		for(i = 0; i < PP2_MAX_PLAYERS; i++)
		{
			if(pp2_player[i].playing)
			{
				strcpy(pp2_player[i].name, pp2_client_game->player[i]->name);
			}
		}

		for(i = 0; i < PP2_MAX_PLAYERS; i++)
		{
			if(pp2_player[i].playing && pp2_client_game->player[i]->local)
			{
				local_player_count++;

				/* update profile */
				pp2_profiles.item[pp2_player[i].profile_choice].plays++;
			}
		}
		if(local_player_count == 0)
		{
			for(i = 0; i < PP2_MAX_PLAYERS; i++)
			{
				if(pp2_player[i].playing)
				{
					pp2_replay_player = i;
					break;
				}
			}
			for(i = 0; i < PP2_MAX_PLAYERS; i++)
			{
				pp2_player[i].view = t3f_create_view(t3f_default_view->left, t3f_default_view->top, PP2_SCREEN_VISIBLE_WIDTH, PP2_SCREEN_VISIBLE_HEIGHT, vwidth / 2, vheight / 2, cflags);
			}
		}
		else if(local_player_count == 1)
		{
			for(i = 0; i < PP2_MAX_PLAYERS; i++)
			{
				if(pp2_player[i].playing && pp2_client_game->player[i]->local)
				{
					pp2_player[i].view = t3f_create_view(t3f_default_view->left, t3f_default_view->top, PP2_SCREEN_VISIBLE_WIDTH, PP2_SCREEN_VISIBLE_HEIGHT, vwidth / 2, vheight / 2, cflags);
					pp2_local_player = i;
				}
				else
				{
					pp2_player[i].view = t3f_create_view(t3f_default_view->left, t3f_default_view->top, PP2_SCREEN_WIDTH, PP2_SCREEN_HEIGHT, vwidth / 2, vheight / 2, cflags);
				}
			}
		}
		else if(local_player_count == 2)
		{
			for(i = 0; i < PP2_MAX_PLAYERS; i++)
			{
				if(pp2_player[i].playing)
				{
					if(pp2_client_game->player[i]->local && c < 2)
					{
						pp2_player[i].view = t3f_create_view(cx2[c], cy2[c], PP2_SCREEN_VISIBLE_WIDTH / 2, cheight, vwidth / 2, vheight / 2, cflags);
						pp2_player[i].view_port = c;
						c++;
					}
					else
					{
						pp2_player[i].view = t3f_create_view(t3f_default_view->left, t3f_default_view->top, PP2_SCREEN_WIDTH, PP2_SCREEN_HEIGHT, vwidth / 2, vheight / 2, cflags);
					}
				}
			}
		}
		else
		{
			for(i = 0; i < PP2_MAX_PLAYERS; i++)
			{
				if(pp2_player[i].playing)
				{
					if(pp2_client_game->player[i]->local && c < 4)
					{
						pp2_player[i].view = t3f_create_view(cx[c], cy[c], PP2_SCREEN_VISIBLE_WIDTH / 2, PP2_SCREEN_VISIBLE_HEIGHT / 2, vwidth / 2, vheight / 2, cflags);
						pp2_player[i].view_port = c;
						c++;
					}
					else
					{
						pp2_player[i].view = t3f_create_view(t3f_default_view->left, t3f_default_view->top, al_get_display_width(t3f_display), al_get_display_height(t3f_display), vwidth / 2, vheight / 2, cflags);
					}
				}
			}
		}
	}
	for(i = 0; i < PP2_MAX_PLAYERS; i++)
	{
		if(pp2_player[i].playing)
		{
			t3f_set_view_virtual_dimensions(pp2_player[i].view, vwidth, vheight);
			t3f_select_view(pp2_player[i].view);
		}
	}

	/* reset player variables */
	for(i = 0; i < PP2_MAX_PLAYERS; i++)
	{
		pp2_player[i].flags = 0;
		pp2_player[i].hits = 0;
		pp2_player[i].frags = 0;
		pp2_player[i].shots = 0;
		pp2_player[i].total_hits = 0;
		pp2_player[i].shot = 0;
		if(!pp2_replay_file)
		{
			pp2_player[i].camera.z = 120.0;
		}
	}

	/* reset objects */
	for(i = 0; i < pp2_object_size; i++)
	{
		pp2_object[i].flags = 0;
	}
	/* set up objects */
	for(i = 0; i < pp2_level->objects; i++)
	{
		pp2_generate_object(pp2_level->object[i].x, pp2_level->object[i].y, pp2_level->object[i].layer, pp2_level->object[i].type, pp2_level->object[i].flags, 0);
	}

	/* set up portal list */
	available_portals = 0;
	for(i = 0; i < pp2_object_size; i++)
	{
		if((pp2_object[i].flags & PP2_OBJECT_FLAG_ACTIVE) && pp2_object[i].type == PP2_OBJECT_PORTAL && available_portals < 32)
		{
			available_portal[available_portals].x = pp2_object[i].x;
			available_portal[available_portals].y = pp2_object[i].y;
			available_portal[available_portals].z = pp2_level->tilemap->layer[pp2_object[i].layer]->z;
			available_portal[available_portals].layer = pp2_object[i].layer;
			available_portals++;
		}
	}

	/* spawn players */
	for(i = 0; i < PP2_MAX_PLAYERS; i++)
	{
		if(pp2_player[i].playing)
		{
			if(!pp2_game_spawn_player(&pp2_player[i]))
			{
				return false;
			}
			t3f_clear_controller_state(pp2_controller[i]);
			pp2_player[i].id = i;
			pp2_camera_logic(i);
			pp2_player[i].coins = 0;
		}
	}
	if(pp2_option[PP2_OPTION_GAME_MODE] == PP2_GAME_MODE_COIN_RUSH)
	{
		pp2_coins_needed = available_portals;
	}

	if(!pp2_option[PP2_OPTION_RANDOMIZE_ITEMS])
	{
		for(i = 0; i < pp2_object_size; i++)
		{
			if(pp2_object[i].flags & PP2_OBJECT_FLAG_ACTIVE)
			{
				switch(pp2_object[i].type)
				{
					case PP2_OBJECT_AMMO_NORMAL:
					{
						if(!pp2_option[PP2_OPTION_ENABLE_AMMO_NORMAL])
						{
							pp2_object[i].flags = 0;
						}
						break;
					}
					case PP2_OBJECT_AMMO_X:
					{
						if(!pp2_option[PP2_OPTION_ENABLE_AMMO_X])
						{
							pp2_object[i].flags = 0;
						}
						break;
					}
					case PP2_OBJECT_AMMO_MINE:
					{
						if(!pp2_option[PP2_OPTION_ENABLE_AMMO_MINE])
						{
							pp2_object[i].flags = 0;
						}
						break;
					}
					case PP2_OBJECT_AMMO_BOUNCE:
					{
						if(!pp2_option[PP2_OPTION_ENABLE_AMMO_BOUNCE])
						{
							pp2_object[i].flags = 0;
						}
						break;
					}
					case PP2_OBJECT_AMMO_SEEK:
					{
						if(!pp2_option[PP2_OPTION_ENABLE_AMMO_SEEK])
						{
							pp2_object[i].flags = 0;
						}
						break;
					}
					case PP2_OBJECT_AMMO_REFLECTOR:
					{
						if(!pp2_option[PP2_OPTION_ENABLE_AMMO_REFLECTOR])
						{
							pp2_object[i].flags = 0;
						}
						break;
					}
					case PP2_OBJECT_AMMO_PMINE:
					{
						if(!pp2_option[PP2_OPTION_ENABLE_AMMO_PMINE])
						{
							pp2_object[i].flags = 0;
						}
						break;
					}
					case PP2_OBJECT_AMMO_GHOST:
					{
						if(!pp2_option[PP2_OPTION_ENABLE_AMMO_GHOST])
						{
							pp2_object[i].flags = 0;
						}
						break;
					}
					case PP2_OBJECT_POWER_CLOAK:
					{
						if(!pp2_option[PP2_OPTION_ENABLE_POWERUP_CLOAK])
						{
							pp2_object[i].flags = 0;
						}
						break;
					}
					case PP2_OBJECT_POWER_JUMP:
					{
						if(!pp2_option[PP2_OPTION_ENABLE_POWERUP_JUMP])
						{
							pp2_object[i].flags = 0;
						}
						break;
					}
					case PP2_OBJECT_POWER_RUN:
					{
						if(!pp2_option[PP2_OPTION_ENABLE_POWERUP_RUN])
						{
							pp2_object[i].flags = 0;
						}
						break;
					}
					case PP2_OBJECT_POWER_DEFLECT:
					{
						if(!pp2_option[PP2_OPTION_ENABLE_POWERUP_DEFLECT])
						{
							pp2_object[i].flags = 0;
						}
						break;
					}
					case PP2_OBJECT_POWER_FLY:
					{
						if(!pp2_option[PP2_OPTION_ENABLE_POWERUP_FLY])
						{
							pp2_object[i].flags = 0;
						}
						break;
					}
					case PP2_OBJECT_POWER_TURBO:
					{
						if(!pp2_option[PP2_OPTION_ENABLE_POWERUP_TURBO])
						{
							pp2_object[i].flags = 0;
						}
						break;
					}
					case PP2_OBJECT_PORTAL:
					{
						pp2_object[i].flags = 0;
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

		if(pp2_option[PP2_OPTION_ENABLE_AMMO_NORMAL])
		{
			ilist[ilist_size] = PP2_OBJECT_AMMO_NORMAL;
			ilist_size++;
		}
		if(pp2_option[PP2_OPTION_ENABLE_AMMO_X])
		{
			ilist[ilist_size] = PP2_OBJECT_AMMO_X;
			ilist_size++;
		}
		if(pp2_option[PP2_OPTION_ENABLE_AMMO_MINE])
		{
			ilist[ilist_size] = PP2_OBJECT_AMMO_MINE;
			ilist_size++;
		}
		if(pp2_option[PP2_OPTION_ENABLE_AMMO_BOUNCE])
		{
			ilist[ilist_size] = PP2_OBJECT_AMMO_BOUNCE;
			ilist_size++;
		}
		if(pp2_option[PP2_OPTION_ENABLE_AMMO_SEEK])
		{
			ilist[ilist_size] = PP2_OBJECT_AMMO_SEEK;
			ilist_size++;
		}
		if(pp2_option[PP2_OPTION_ENABLE_AMMO_REFLECTOR])
		{
			ilist[ilist_size] = PP2_OBJECT_AMMO_REFLECTOR;
			ilist_size++;
		}
		if(pp2_option[PP2_OPTION_ENABLE_AMMO_PMINE])
		{
			ilist[ilist_size] = PP2_OBJECT_AMMO_PMINE;
			ilist_size++;
		}
		if(pp2_option[PP2_OPTION_ENABLE_AMMO_GHOST])
		{
			ilist[ilist_size] = PP2_OBJECT_AMMO_GHOST;
			ilist_size++;
		}
		if(pp2_option[PP2_OPTION_ENABLE_POWERUP_CLOAK])
		{
			ilist[ilist_size] = PP2_OBJECT_POWER_CLOAK;
			ilist_size++;
		}
		if(pp2_option[PP2_OPTION_ENABLE_POWERUP_JUMP])
		{
			ilist[ilist_size] = PP2_OBJECT_POWER_JUMP;
			ilist_size++;
		}
		if(pp2_option[PP2_OPTION_ENABLE_POWERUP_RUN])
		{
			ilist[ilist_size] = PP2_OBJECT_POWER_RUN;
			ilist_size++;
		}
		if(pp2_option[PP2_OPTION_ENABLE_POWERUP_DEFLECT])
		{
			ilist[ilist_size] = PP2_OBJECT_POWER_DEFLECT;
			ilist_size++;
		}
		if(pp2_option[PP2_OPTION_ENABLE_POWERUP_FLY])
		{
			ilist[ilist_size] = PP2_OBJECT_POWER_FLY;
			ilist_size++;
		}
		if(pp2_option[PP2_OPTION_ENABLE_POWERUP_TURBO])
		{
			ilist[ilist_size] = PP2_OBJECT_POWER_TURBO;
			ilist_size++;
		}

		for(i = 0; i < pp2_object_size; i++)
		{
			if(pp2_object[i].flags & PP2_OBJECT_FLAG_ACTIVE)
			{
				switch(pp2_object[i].type)
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
						pp2_object[i].type = ilist[r];
						break;
					}
				}
			}
		}
	}

	for(i = 0; i < pp2_object_size; i++)
	{
		if(pp2_object[i].flags & PP2_OBJECT_FLAG_ACTIVE && pp2_object[i].type == PP2_OBJECT_PORTAL)
		{
			pp2_object[i].flags = 0;
		}
	}

	switch(pp2_option[PP2_OPTION_GAME_MODE])
	{
		case PP2_GAME_MODE_ELIMINATOR:
		{
			break;
		}
		case PP2_GAME_MODE_DEATH_MATCH:
		{
			pp2_time_left = pp2_option[PP2_OPTION_TIME_LIMIT] * 3600;
			break;
		}
		case PP2_GAME_MODE_COIN_RUSH:
		{
			pp2_time_left = pp2_option[PP2_OPTION_TIME_LIMIT] * 3600;
			for(i = 0; i < available_portals; i++)
			{
				o = pp2_generate_object(available_portal[i].x + pp2_object_animation[PP2_OBJECT_PORTAL]->frame[0]->width / 2 - pp2_object_animation[PP2_OBJECT_COIN]->frame[0]->width / 2, available_portal[i].y + pp2_object_animation[PP2_OBJECT_PORTAL]->frame[0]->height / 2 - pp2_object_animation[PP2_OBJECT_COIN]->frame[0]->height / 2, available_portal[i].layer, PP2_OBJECT_COIN, PP2_OBJECT_FLAG_ACTIVE, 0);
				if(o >= 0)
				{
					pp2_object[o].vx = 0.0;
					pp2_object[o].vy = 0.0;
					t3f_recreate_collision_object(pp2_object[o].object, 0, 0, 16, 16, 32, 32, 0);
					t3f_move_collision_object_xy(pp2_object[o].object, pp2_object[i].x, pp2_object[i].y);
				}
				else
				{
					printf("failed to generate coin\n");
				}
			}
			break;
		}
	}
	pp2_winner = -1;
	r = joynet_rand(); // random number in case we need to pick random music
	entry = pp2_database_find_entry(pp2_level_database, pp2_level_hash);
	if(entry < 0)
	{
		entry = 0;
	}
	if(flags & PP2_GAME_INIT_FLAG_CAPTURE)
	{
		t3f_stop_music();
	}
	music_file = pp2_find_music((char *)al_path_cstr(pp2_level_database->entry[entry]->path, '/'));
	if(music_file)
	{
		if(!(pp2_replay_flags & PP2_REPLAY_FLAG_DEMO) && !(!(pp2_replay_flags & PP2_REPLAY_FLAG_DEMO)))
		{
			pp2_play_music(music_file);
		}
	}
	else
	{
		if(!(pp2_replay_flags & PP2_REPLAY_FLAG_DEMO) && !(pp2_replay_flags & PP2_REPLAY_FLAG_THEATER))
		{
			pp2_play_music((char *)al_path_cstr(pp2_music_database->entry[r % pp2_music_database->entries]->path, '/'));
		}
	}
	if(!pp2_replay_file)
	{
		if(pp2_replay_player == -2)
		{
			pp2_replay_player = -1;
		}
		sprintf(tempfn, "replays/%s.p2r", pp2_get_date_string());
		pp2_record_replay(t3f_get_filename(t3f_data_path, tempfn, buf, 1024));
		al_hide_mouse_cursor(t3f_display);
		pp2_state = PP2_STATE_GAME;
	}
	pp2_tick = 0;
	if(!(flags & PP2_GAME_INIT_FLAG_CAPTURE))
	{
		al_start_timer(t3f_timer);
	}
	return true;
}

bool pp2_game_init(int flags, PP2_RESOURCES * resources)
{
	if(!(flags & PP2_GAME_INIT_FLAG_CAPTURE))
	{
		al_stop_timer(t3f_timer);
	}

	/* if we are watching a replay, show the first player */
	al_identity_transform(&pp2_identity_transform);
	al_use_transform(&pp2_identity_transform);
	if(pp2_replay_file)
	{
		if(pp2_replay_flags & PP2_REPLAY_FLAG_DEMO)
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

	if(!pp2_game_load_data())
	{
		printf("data fail!\n");
		return false;
	}
	return pp2_game_setup(flags);
}

static bool pp2_camera_clamp_left(int i)
{
	if(pp2_player[i].camera.x < pp2_level->room.x * 32 - pp2_player[i].view->left)
	{
		pp2_player[i].camera.x = pp2_level->room.x * 32 - pp2_player[i].view->left;
		return true;
	}
	return false;
}

static bool pp2_camera_clamp_right(int i)
{
	float right_offset = pp2_player[i].view->virtual_width - (pp2_player[i].view->virtual_width - (pp2_player[i].view->right - pp2_player[i].view->left)) / 2;

	if(pp2_player[i].camera.x + right_offset > pp2_level->room.bx * 32 + 32)
	{
		pp2_player[i].camera.x = pp2_level->room.bx * 32 + 32 - right_offset;
		return true;
	}
	return false;
}

static bool pp2_camera_clamp_top(int i)
{
	if(pp2_player[i].camera.y < pp2_level->room.y * 32 - pp2_player[i].view->top)
	{
		pp2_player[i].camera.y = pp2_level->room.y * 32 - pp2_player[i].view->top;
		return true;
	}
	return false;
}

static bool pp2_camera_clamp_bottom(int i)
{
	float bottom_offset = pp2_player[i].view->virtual_height - (pp2_player[i].view->virtual_height - (pp2_player[i].view->bottom - pp2_player[i].view->top)) / 2;
	if(pp2_player[i].camera.y + bottom_offset > pp2_level->room.by * 32 + 32)
	{
		pp2_player[i].camera.y = pp2_level->room.by * 32 + 32 - bottom_offset;
		return true;
	}
	return false;
}

void pp2_camera_logic(int i)
{
	pp2_player[i].camera.x = pp2_player[i].x - pp2_player[i].view->virtual_width / 2 + pp2_player[i].object[0]->map.top.point[0].x;
	if(!pp2_camera_clamp_left(i))
	{
		if(pp2_camera_clamp_right(i))
		{
			pp2_camera_clamp_left(i);
		}
	}
	pp2_player[i].camera.y = pp2_player[i].y - pp2_player[i].view->virtual_height / 2 + pp2_player[i].object[0]->map.left.point[0].y;
	if(!pp2_camera_clamp_top(i))
	{
		if(pp2_camera_clamp_bottom(i))
		{
			pp2_camera_clamp_top(i);
		}
	}
}

void pp2_game_logic(void)
{
	int i, j;

	show_scores = false;
	/* fill in local controller data and send it off */
	for(i = 0; i < 4; i++)
	{
		t3f_read_controller(pp2_controller[i]);
		for(j = 0; j < 8; j++)
		{
			pp2_client_game->controller[i]->button[j] = pp2_controller[i]->state[j].down;
		}

		/* see if a player wants to see the scores */
		if(pp2_controller[i]->state[PP2_CONTROLLER_SCORES].down)
		{
			show_scores = true;
		}
	}
	joynet_update_game(pp2_client_game, 1);

	while(joynet_get_input_buffer_frames(pp2_client_game->input_buffer) > 0)
	{
		joynet_game_logic(pp2_client_game);
		pp2_radar_objects = 0;
		if(pp2_winner < 0 && pp2_time_left > 0)
		{
			pp2_time_left--;
		}
		for(i = 0; i < PP2_MAX_PLAYERS; i++)
		{
			if(pp2_client_game->player[i]->playing)
			{
				if(pp2_replay_file)
				{
					al_fputc(pp2_replay_file, pp2_client_game->player_controller[i]->bits[0]);
				}
				for(j = 0; j < 8; j++)
				{
					pp2_controller[i]->state[j].down = pp2_client_game->player_controller[i]->button[j];
				}
				t3f_update_controller(pp2_controller[i]);
				pp2_player_logic(&pp2_player[i]);
				if(pp2_option[PP2_OPTION_TRAILS])
				{
					for(j = 0; j < PP2_MAX_TRAILS; j++)
					{
						pp2_paintball_trail_logic(&pp2_player[i].trail[j]);
					}
				}
				for(j = 0; j < PP2_MAX_PAINTBALLS; j++)
				{
					pp2_paintball_logic(&pp2_player[i].paintball[j]);
				}
				for(j = 0; j < PP2_MAX_PARTICLES; j++)
				{
					pp2_particle_logic(&pp2_particle[j]);
					pp2_particle_logic(&pp2_player[i].particle[j]);
				}

				if(pp2_player[i].fade_time != 0 && pp2_player[i].fade_type == 0)
				{
					pp2_player[i].camera.z += 4.0;
				}
				else if(pp2_player[i].camera.z > 0.0)
				{
					pp2_player[i].camera.z -= 4.0;
				}

				/* camera logic */
				if(pp2_winner < 0)
				{
					pp2_camera_logic(i);
				}
			}
		}
		for(i = 0; i < pp2_object_size; i++)
		{
			pp2_object_logic(&pp2_object[i]);
		}
		if(pp2_winner < 0)
		{
			pp2_process_rules();
			if(pp2_winner >= 0 && pp2_player[pp2_winner].character->sample[PP2_SAMPLE_WIN])
			{
				t3f_play_sample(pp2_player[pp2_winner].character->sample[PP2_SAMPLE_WIN], 1.0, 0.0, 1.0);
			}
		}
		pp2_tick++;
	}
}

ALLEGRO_COLOR pp2_game_get_ammo_color(int player, int weapon, float a)
{
	if(pp2_player[player].ammo[weapon] > 5)
	{
		return al_map_rgba_f(0.0, 1.0 * a, 0.0, a);
	}
	else if(pp2_player[player].ammo[weapon] > 1)
	{
		return al_map_rgba_f(1.0 * a, 1.0 * a, 0.0, a);
	}
	else
	{
		return al_map_rgba_f(1.0 * a, 0.0, 0.0, a);
	}
}

/* renders scoreboard */
void pp2_game_render_scoreboard(const char * title)
{
	PP2_SCOREBOARD_ENTRY score[PP2_MAX_PLAYERS];
	ALLEGRO_COLOR color;
	int scores = 0;
	int i;
	float y;

	switch(pp2_option[PP2_OPTION_GAME_MODE])
	{
		case PP2_GAME_MODE_ELIMINATOR:
		{
			for(i = 0; i < PP2_MAX_PLAYERS; i++)
			{
				if(pp2_player[i].playing)
				{
					score[scores].player = i;
					if(pp2_player[i].flags & PP2_PLAYER_FLAG_ACTIVE)
					{
						score[scores].score = 1;
					}
					else
					{
						score[scores].score = 0;
					}
					scores++;
				}
			}
			qsort(score, scores, sizeof(PP2_SCOREBOARD_ENTRY), pp2_scoreboard_sorter);
			y = 240.0 - (float)((scores + 2) * al_get_font_line_height(pp2_font[PP2_FONT_SMALL])) / 2.0;
			break;
		}
		case PP2_GAME_MODE_DEATH_MATCH:
		{
			for(i = 0; i < PP2_MAX_PLAYERS; i++)
			{
				if(pp2_player[i].playing)
				{
					score[scores].player = i;
					score[scores].score = pp2_player[i].frags;
					scores++;
				}
			}
			qsort(score, scores, sizeof(PP2_SCOREBOARD_ENTRY), pp2_scoreboard_sorter);
			y = 240.0 - (float)((scores + 2) * al_get_font_line_height(pp2_font[PP2_FONT_SMALL])) / 2.0;
			break;
		}
		case PP2_GAME_MODE_COIN_RUSH:
		{
			for(i = 0; i < PP2_MAX_PLAYERS; i++)
			{
				if(pp2_player[i].playing)
				{
					score[scores].player = i;
					score[scores].score = pp2_player[i].coins;
					scores++;
				}
			}
			qsort(score, scores, sizeof(PP2_SCOREBOARD_ENTRY), pp2_scoreboard_sorter);
			y = 240.0 - (float)((scores + 2) * al_get_font_line_height(pp2_font[PP2_FONT_SMALL])) / 2.0;
			break;
		}
		default:
		{
			scores = 0;
			y = 240.0;
		}
	}

	/* render the scores */
	if(scores > 0)
	{
		t3f_select_view(NULL);
		al_hold_bitmap_drawing(false);
		al_draw_filled_rectangle(0.0, 0.0, PP2_SCREEN_WIDTH, PP2_SCREEN_HEIGHT, al_map_rgba_f(0.0, 0.0, 0.0, 0.5));
		al_hold_bitmap_drawing(true);
		al_draw_textf(pp2_font[PP2_FONT_SMALL], al_map_rgba_f(0.0, 0.0, 0.0, 0.5), PP2_SCREEN_WIDTH / 2 + 2, y + 2, ALLEGRO_ALIGN_CENTRE, "%s", title);
		al_draw_textf(pp2_font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), PP2_SCREEN_WIDTH / 2, y, ALLEGRO_ALIGN_CENTRE, "%s", title);
		y += al_get_font_line_height(pp2_font[PP2_FONT_SMALL]) * 2;
		for(i = 0; i < scores; i++)
		{
			if(i == 0)
			{
				if(pp2_tick % 2)
				{
					color = al_map_rgba_f(1.0, 1.0, 0.0, 1.0);
				}
				else
				{
					color = al_map_rgba_f(1.0, 1.0, 1.0, 1.0);
				}
			}
			else
			{
				color = al_map_rgba_f(0.5, 0.5, 0.5, 1.0);
			}
			al_draw_textf(pp2_font[PP2_FONT_SMALL], al_map_rgba_f(0.0, 0.0, 0.0, 0.5), 200 + 2, y + i * al_get_font_line_height(pp2_font[PP2_FONT_SMALL]) + 2, 0, "%s", pp2_player[score[i].player].name);
			al_draw_textf(pp2_font[PP2_FONT_SMALL], al_map_rgba_f(0.0, 0.0, 0.0, 0.5), PP2_SCREEN_WIDTH / 2 + 200 + 2, y + i * al_get_font_line_height(pp2_font[PP2_FONT_SMALL]) + 2, ALLEGRO_ALIGN_RIGHT, "%d", score[i].score);
			al_draw_textf(pp2_font[PP2_FONT_SMALL], color, 200, y + i * al_get_font_line_height(pp2_font[PP2_FONT_SMALL]), 0, "%s", pp2_player[score[i].player].name);
			al_draw_textf(pp2_font[PP2_FONT_SMALL], color, PP2_SCREEN_WIDTH / 2 + 200, y + i * al_get_font_line_height(pp2_font[PP2_FONT_SMALL]), ALLEGRO_ALIGN_RIGHT, "%d", score[i].score);
		}
		al_hold_bitmap_drawing(false);
	}
}

static void pp2_game_render_hud_weapon_type(int i, int j, ALLEGRO_COLOR color, PP2_RESOURCES * resources)
{
	float start_angle = -ALLEGRO_PI / 2.0;
	float angle_step = (ALLEGRO_PI * 2.0) / 8.0;
	float cx, cy, angle;

	angle = start_angle + angle_step * (float)j;
	cx = pp2_player[i].x + pp2_player[i].object[0]->map.top.point[0].x;
	cy = pp2_player[i].y + pp2_player[i].object[0]->map.left.point[0].y;
	t3f_draw_animation(resources->animation[PP2_ANIMATION_HUD_AMMO_NORMAL + j], color, pp2_tick, cx - 16.0 + 64.0 * cos(angle) - pp2_player[i].camera.x, cy - 16.0 + 64.0 * sin(angle) - pp2_player[i].camera.y, -pp2_player[i].camera.z, 0);
}

/* renders one player's view */
void pp2_game_render_player_view(int i, PP2_RESOURCES * resources)
{
	int j, k;
	float cx, cy, a, s, ox = 6.0, oy = 0.0, sx = 2.0, sy = 2.0;

	t3f_select_view(pp2_player[i].view);

	al_hold_bitmap_drawing(true);

	/* render the background */
	if(pp2_level->bg)
	{
		t3f_draw_animation(pp2_level->bg, t3f_color_white, pp2_tick, 0, 0, 0, 0);
	}
	for(j = 0; j <= pp2_player[i].layer; j++)
	{
		t3f_render_tilemap(pp2_level->tilemap, pp2_level->tileset, j, pp2_tick, pp2_player[i].camera.x, pp2_player[i].camera.y, pp2_player[i].camera.z, t3f_color_white);
	}

	/* draw game objects over background */
	for(j = 0; j < pp2_object_size; j++)
	{
		pp2_object_render(&pp2_object[j], &pp2_player[i].camera);
	}
	for(j = 0; j < PP2_MAX_PARTICLES; j++)
	{
		pp2_particle_render(&pp2_particle[j], &pp2_player[i].camera);
	}
	for(j = 0; j < PP2_MAX_PLAYERS; j++)
	{
		if(pp2_option[PP2_OPTION_TRAILS])
		{
			for(k = 0; k < PP2_MAX_TRAILS; k++)
			{
				pp2_paintball_trail_render(&pp2_player[j].trail[k], &pp2_player[i].camera);
			}
		}
		for(k = 0; k < PP2_MAX_PAINTBALLS; k++)
		{
			pp2_paintball_render(&pp2_player[j].paintball[k], &pp2_player[i].camera, resources);
		}
	}
	for(j = 0; j < PP2_MAX_PLAYERS; j++)
	{
		pp2_player_render(&pp2_player[j], &pp2_player[i].camera);
	}
	for(j = 0; j < PP2_MAX_PLAYERS; j++)
	{
		for(k = 0; k < PP2_MAX_PARTICLES; k++)
		{
			pp2_particle_render(&pp2_player[j].particle[k], &pp2_player[i].camera);
		}
	}

	/* draw foreground */
	for(j = pp2_player[i].layer + 1; j < pp2_level->tilemap->layers; j++)
	{
		t3f_render_tilemap(pp2_level->tilemap, pp2_level->tileset, j, pp2_tick, pp2_player[i].camera.x, pp2_player[i].camera.y, pp2_player[i].camera.z, t3f_color_white);
	}
	if(pp2_level->fg)
	{
		t3f_draw_animation(pp2_level->fg, t3f_color_white, pp2_tick, 0, 0, 0, 0);
	}

	/* draw the HUD */
	if(pp2_player[i].target >= 0)
	{
		cx = pp2_player[i].x + pp2_player[i].object[0]->map.top.point[0].x;
		cy = pp2_player[i].y + pp2_player[i].object[0]->map.left.point[0].y;
		a = atan2((pp2_player[pp2_player[i].target].y + pp2_player[pp2_player[i].target].object[pp2_player[pp2_player[i].target].current_object]->map.left.point[0].y) - cy, pp2_player[pp2_player[i].target].x + pp2_player[pp2_player[i].target].object[pp2_player[pp2_player[i].target].current_object]->map.top.point[0].x - cx);
		al_draw_tinted_rotated_bitmap(resources->bitmap[PP2_BITMAP_TARGET], al_map_rgba_f(1.0, 0.0, 0.0, 1.0), 8 - 64, 8, cx - pp2_player[i].camera.x, cy - pp2_player[i].camera.y, a, 0);
	}
	if(pp2_option[PP2_OPTION_GAME_MODE] == PP2_GAME_MODE_COIN_RUSH)
	{
		if(pp2_player[i].coin_target)
		{
			cx = pp2_player[i].x + pp2_player[i].object[0]->map.top.point[0].x;
			cy = pp2_player[i].y + pp2_player[i].object[0]->map.left.point[0].y;
			a = atan2(pp2_player[i].coin_target_y - cy, pp2_player[i].coin_target_x - cx);
			al_draw_tinted_rotated_bitmap(resources->bitmap[PP2_BITMAP_TARGET], al_map_rgba_f(1.0, 1.0, 0.0, 1.0), 8 - 64, 8, cx - pp2_player[i].camera.x, cy - pp2_player[i].camera.y, a, 0);
		}
	}
	if(pp2_player[i].choose_weapon)
	{
		ALLEGRO_COLOR wcolor;

		for(j = 0; j < PP2_PLAYER_MAX_WEAPONS; j++)
		{
			if(!pp2_player[i].ammo[j])
			{
				wcolor = al_map_rgba_f(0.25, 0.25, 0.25, 0.5);
			}
			else
			{
				wcolor = t3f_color_white;
			}
			pp2_game_render_hud_weapon_type(i, j, j == pp2_player[i].want_weapon ? al_map_rgba_f(0.0, 1.0, 0.0, 1.0) : wcolor, resources);
		}
	}
	else
	{
		if(pp2_player[i].timer[PP2_PLAYER_TIMER_WEAPON_SELECT])
		{
			pp2_game_render_hud_weapon_type(i, pp2_player[i].weapon, ((pp2_player[i].timer[PP2_PLAYER_TIMER_WEAPON_SELECT] / 4) % 2 == 0) ? al_map_rgba_f(0.0, 1.0, 0.0, 1.0) : t3f_color_white, resources);

		}
	}
	for(j = 0; j < PP2_MAX_PLAYERS; j++)
	{
		if(pp2_player[j].flags & PP2_PLAYER_FLAG_TYPING)
		{
			al_draw_bitmap(resources->bitmap[PP2_BITMAP_TYPING], pp2_player[j].x + pp2_player[j].object[0]->map.top.point[0].x - al_get_bitmap_width(resources->bitmap[PP2_BITMAP_TYPING]) / 2 - pp2_player[i].camera.x, pp2_player[j].y + pp2_player[j].object[0]->map.top.point[0].y - al_get_bitmap_height(resources->bitmap[PP2_BITMAP_TYPING]) - pp2_player[i].camera.y, 0);
		}
	}
	/* highlight target */
	al_hold_bitmap_drawing(false);
	if(pp2_player[i].target >= 0)
	{
		j = pp2_player[i].target;
		k = pp2_player[pp2_player[i].target].current_object;
		al_draw_rectangle(t3f_project_x(pp2_player[j].x + pp2_player[j].object[k]->map.left.point[0].x - 8 - pp2_player[i].camera.x, -pp2_player[i].camera.z), t3f_project_y(pp2_player[j].y + pp2_player[j].object[k]->map.top.point[0].y - 8 - pp2_player[i].camera.y, -pp2_player[i].camera.z), t3f_project_x(pp2_player[j].x + pp2_player[j].object[k]->map.right.point[0].x + 8 - pp2_player[i].camera.x, -pp2_player[i].camera.z), t3f_project_y(pp2_player[j].y + pp2_player[j].object[k]->map.bottom.point[0].y + 8 - pp2_player[i].camera.y, -pp2_player[i].camera.z), al_map_rgba_f(0.5, 0.0, 0.0, 0.5), 2.0);
	}
	al_hold_bitmap_drawing(true);

	/* draw player names */
	for(j = 0; j < PP2_MAX_PLAYERS; j++)
	{
		if((pp2_player[j].flags & PP2_PLAYER_FLAG_ACTIVE) && ((j != i && !((pp2_player[j].flags & PP2_PLAYER_FLAG_POWER_CLOAK))) || pp2_replay_player >= 0))
		{
			al_draw_text(pp2_font[PP2_FONT_SMALL], al_map_rgba_f(0.0, 0.0, 0.0, 0.5), t3f_project_x(pp2_player[j].x + pp2_player[j].object[0]->map.top.point[0].x - pp2_player[i].camera.x, -pp2_player[i].camera.z) + 2, t3f_project_y(pp2_player[j].y + pp2_player[j].object[0]->map.top.point[0].y - al_get_font_line_height(pp2_font[PP2_FONT_SMALL]) - pp2_player[i].camera.y, -pp2_player[i].camera.z) + 2, ALLEGRO_ALIGN_CENTRE, pp2_player[j].name);
			al_draw_text(pp2_font[PP2_FONT_SMALL], al_map_rgba_f(0.0, 1.0, 0.0, 1.0), t3f_project_x(pp2_player[j].x + pp2_player[j].object[0]->map.top.point[0].x - pp2_player[i].camera.x, -pp2_player[i].camera.z), t3f_project_y(pp2_player[j].y + pp2_player[j].object[0]->map.top.point[0].y - al_get_font_line_height(pp2_font[PP2_FONT_SMALL]) - pp2_player[i].camera.y, -pp2_player[i].camera.z), ALLEGRO_ALIGN_CENTRE, pp2_player[j].name);
		}
	}

	switch(pp2_option[PP2_OPTION_GAME_MODE])
	{
		case PP2_GAME_MODE_ELIMINATOR:
		{
			al_draw_textf(pp2_font[PP2_FONT_COMIC_16], al_map_rgba_f(0.0, 0.0, 0.0, 0.5), pp2_player[i].view->left + ox + sx, pp2_player[i].view->top + oy + sy, 0, "Ammo: %02d", pp2_player[i].ammo[pp2_player[i].weapon]);
			al_draw_textf(pp2_font[PP2_FONT_COMIC_16], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), pp2_player[i].view->left + ox, pp2_player[i].view->top + oy, 0, "Ammo: %02d", pp2_player[i].ammo[pp2_player[i].weapon]);
			oy += al_get_font_line_height(pp2_font[PP2_FONT_COMIC_16]);
			if(pp2_option[PP2_OPTION_LIFE] > 1)
			{
				al_draw_textf(pp2_font[PP2_FONT_COMIC_16], al_map_rgba_f(0.0, 0.0, 0.0, 0.5), pp2_player[i].view->left + ox + sx, pp2_player[i].view->top + oy + sy, 0, "Life: %02d", pp2_player[i].life);
				al_draw_textf(pp2_font[PP2_FONT_COMIC_16], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), pp2_player[i].view->left + ox, pp2_player[i].view->top + oy, 0, "Life: %02d", pp2_player[i].life);
				oy += al_get_font_line_height(pp2_font[PP2_FONT_COMIC_16]);
			}
			break;
		}
		case PP2_GAME_MODE_DEATH_MATCH:
		{
			al_draw_textf(pp2_font[PP2_FONT_COMIC_16], al_map_rgba_f(0.0, 0.0, 0.0, 0.5), pp2_player[i].view->left + ox + sx, pp2_player[i].view->top + oy + sy, 0, "Ammo: %02d", pp2_player[i].ammo[pp2_player[i].weapon]);
			al_draw_textf(pp2_font[PP2_FONT_COMIC_16], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), pp2_player[i].view->left + ox, pp2_player[i].view->top + oy, 0, "Ammo: %02d", pp2_player[i].ammo[pp2_player[i].weapon]);
			oy += al_get_font_line_height(pp2_font[PP2_FONT_COMIC_16]);
			if(pp2_option[PP2_OPTION_LIFE] > 1)
			{
				al_draw_textf(pp2_font[PP2_FONT_COMIC_16], al_map_rgba_f(0.0, 0.0, 0.0, 0.5), pp2_player[i].view->left + ox + sx, pp2_player[i].view->top + oy + sy, 0, "Life: %02d", pp2_player[i].life);
				al_draw_textf(pp2_font[PP2_FONT_COMIC_16], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), pp2_player[i].view->left + ox, pp2_player[i].view->top + oy, 0, "Life: %02d", pp2_player[i].life);
				oy += al_get_font_line_height(pp2_font[PP2_FONT_COMIC_16]);
			}
			al_draw_textf(pp2_font[PP2_FONT_COMIC_16], al_map_rgba_f(0.0, 0.0, 0.0, 0.5), pp2_player[i].view->left + ox + sx, pp2_player[i].view->top + oy + sy, 0, "Frags: %02d", pp2_player[i].frags);
			al_draw_textf(pp2_font[PP2_FONT_COMIC_16], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), pp2_player[i].view->left + ox, pp2_player[i].view->top + oy, 0, "Frags: %02d", pp2_player[i].frags);
			oy += al_get_font_line_height(pp2_font[PP2_FONT_COMIC_16]);
			if(pp2_option[PP2_OPTION_TIME_LIMIT] > 0)
			{
				al_draw_textf(pp2_font[PP2_FONT_COMIC_16], al_map_rgba_f(0.0, 0.0, 0.0, 0.5), pp2_player[i].view->virtual_width / 2 + sx, pp2_player[i].view->top + sy, ALLEGRO_ALIGN_CENTRE, "%02d:%02d", (pp2_time_left + 59) / 3600, ((pp2_time_left + 59) / 60) % 60);
				al_draw_textf(pp2_font[PP2_FONT_COMIC_16], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), pp2_player[i].view->virtual_width / 2, pp2_player[i].view->top, ALLEGRO_ALIGN_CENTRE, "%02d:%02d", (pp2_time_left + 59) / 3600, ((pp2_time_left + 59) / 60) % 60);
			}
			break;
		}
		case PP2_GAME_MODE_COIN_RUSH:
		{
			al_draw_textf(pp2_font[PP2_FONT_COMIC_16], al_map_rgba_f(0.0, 0.0, 0.0, 0.5), pp2_player[i].view->left + ox + sx, pp2_player[i].view->top + oy + sy, 0, "Ammo: %02d", pp2_player[i].ammo[pp2_player[i].weapon]);
			al_draw_textf(pp2_font[PP2_FONT_COMIC_16], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), pp2_player[i].view->left + ox, pp2_player[i].view->top + oy, 0, "Ammo: %02d", pp2_player[i].ammo[pp2_player[i].weapon]);
			oy += al_get_font_line_height(pp2_font[PP2_FONT_COMIC_16]);
			if(pp2_option[PP2_OPTION_LIFE] > 1)
			{
				al_draw_textf(pp2_font[PP2_FONT_COMIC_16], al_map_rgba_f(0.0, 0.0, 0.0, 0.5), pp2_player[i].view->left + ox + sx, pp2_player[i].view->top + oy + sy, 0, "Life: %02d", pp2_player[i].life);
				al_draw_textf(pp2_font[PP2_FONT_COMIC_16], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), pp2_player[i].view->left + ox, pp2_player[i].view->top + oy, 0, "Life: %02d", pp2_player[i].life);
				oy += al_get_font_line_height(pp2_font[PP2_FONT_COMIC_16]);
			}
			al_draw_textf(pp2_font[PP2_FONT_COMIC_16], al_map_rgba_f(0.0, 0.0, 0.0, 0.5), pp2_player[i].view->left + ox + sx, pp2_player[i].view->top + oy + sy, 0, "Coins: %d of %d", pp2_player[i].coins, pp2_coins_needed);
			al_draw_textf(pp2_font[PP2_FONT_COMIC_16], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), pp2_player[i].view->left + ox, pp2_player[i].view->top + oy, 0, "Coins: %d of %d", pp2_player[i].coins, pp2_coins_needed);
			oy += al_get_font_line_height(pp2_font[PP2_FONT_COMIC_16]);
			if(pp2_option[PP2_OPTION_TIME_LIMIT] > 0)
			{
				al_draw_textf(pp2_font[PP2_FONT_COMIC_16], al_map_rgba_f(0.0, 0.0, 0.0, 0.5), pp2_player[i].view->virtual_width / 2 + sx, pp2_player[i].view->top + sy, ALLEGRO_ALIGN_CENTRE, "%02d:%02d", (pp2_time_left + 59) / 3600, ((pp2_time_left + 59) / 60) % 60);
				al_draw_textf(pp2_font[PP2_FONT_COMIC_16], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), pp2_player[i].view->virtual_width / 2, pp2_player[i].view->top, ALLEGRO_ALIGN_CENTRE, "%02d:%02d", (pp2_time_left + 59) / 3600, ((pp2_time_left + 59) / 60) % 60);
			}
			break;
		}
	}

	/* draw radar */
	a = 0.5;
	al_draw_tinted_scaled_bitmap(pp2_radar_bitmap[pp2_player[i].layer], al_map_rgba_f(a, a, a, a), 0, 0, al_get_bitmap_width(pp2_radar_bitmap[pp2_player[i].layer]), al_get_bitmap_height(pp2_radar_bitmap[pp2_player[i].layer]), pp2_player[i].view->right - 96 - 8 - 1, pp2_player[i].view->top + 8, 96, 96, 0);
	s = (float)96.0 / (float)(al_get_bitmap_width(pp2_radar_bitmap[pp2_player[i].layer]));
	for(j = 0; j < pp2_radar_objects; j++)
	{
		cx = pp2_radar_object[j].x - (float)(pp2_level->tileset->width * pp2_level->room.x);
		cx /= (float)pp2_level->tileset->width;
		cx += (float)pp2_radar_offset_x;
		cx *= s;
		cy = pp2_radar_object[j].y  - (float)(pp2_level->tileset->height * pp2_level->room.y);
		cy /= (float)pp2_level->tileset->height;
		cy += (float)pp2_radar_offset_y;
		cy *= s;
		if(pp2_radar_object[j].player < 0)
		{
			al_draw_tinted_bitmap(resources->bitmap[PP2_BITMAP_RADAR_BLIP], al_map_rgba_f(0.0, 1.0, 1.0, 1.0), pp2_player[i].view->right - 96 - 8 - 1 + cx - 1.0, pp2_player[i].view->top + 8 + cy - 1.0, 0);
		}
		else
		{
			if(pp2_radar_object[j].player != i)
			{
				if(pp2_player[pp2_radar_object[j].player].flags & PP2_PLAYER_FLAG_POWER_CLOAK)
				{
				}
				else
				{
					al_draw_tinted_bitmap(resources->bitmap[PP2_BITMAP_RADAR_BLIP], al_map_rgba_f(1.0, 0.0, 0.0, 1.0), pp2_player[i].view->right - 96 - 8 - 1 + cx - 1.0, pp2_player[i].view->top + 8 + cy - 1.0, 0);
				}
			}
			else
			{
				al_draw_tinted_bitmap(resources->bitmap[PP2_BITMAP_RADAR_BLIP], al_map_rgba_f(1.0, 1.0, 0.0, 1.0), pp2_player[i].view->right - 96 - 8 - 1 + cx - 1.0, pp2_player[i].view->top + 8 + cy - 1.0, 0);
			}
		}
	}
	al_hold_bitmap_drawing(false);
}

static void render_single_viewport_backdrop(int i, ALLEGRO_COLOR color, PP2_RESOURCES * resources)
{
	switch(i)
	{
		case 0:
		{
			t3f_draw_scaled_bitmap(resources->bitmap[PP2_BITMAP_EMPTY_PLAYER], color, t3f_default_view->left, t3f_default_view->top, 0.0, PP2_SCREEN_VISIBLE_WIDTH / 2, PP2_SCREEN_VISIBLE_HEIGHT / 2, 0);
			break;
		}
		case 1:
		{
			t3f_draw_scaled_bitmap(resources->bitmap[PP2_BITMAP_EMPTY_PLAYER], color, PP2_SCREEN_WIDTH / 2, PP2_SCREEN_HEIGHT / 2, 0.0, PP2_SCREEN_VISIBLE_WIDTH / 2, PP2_SCREEN_VISIBLE_HEIGHT / 2, 0);
			break;
		}
		case 2:
		{
			t3f_draw_scaled_bitmap(resources->bitmap[PP2_BITMAP_EMPTY_PLAYER], color, PP2_SCREEN_WIDTH / 2, t3f_default_view->top, 0.0, PP2_SCREEN_VISIBLE_WIDTH / 2, PP2_SCREEN_VISIBLE_HEIGHT / 2, 0);
			break;
		}
		case 3:
		{
			t3f_draw_scaled_bitmap(resources->bitmap[PP2_BITMAP_EMPTY_PLAYER], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), t3f_default_view->left, PP2_SCREEN_HEIGHT / 2, 0.0, PP2_SCREEN_VISIBLE_WIDTH / 2, PP2_SCREEN_VISIBLE_HEIGHT / 2, 0);
			break;
		}
	}
}

static void render_viewport_backdrop(PP2_RESOURCES * resources)
{
	float x, y, w, h;
	bool used[4] = {false};
	int i, c, p;

	al_clear_to_color(t3f_color_black);
	al_hold_bitmap_drawing(true);
	c = 0;
	p = -1;
	for(i = 0; i < PP2_MAX_PLAYERS; i++)
	{
		if(pp2_player[i].flags & PP2_PLAYER_FLAG_ACTIVE)
		{
			if(pp2_player[i].view_port >= 0)
			{
				used[pp2_player[i].view_port] = true;
				c++;
			}
			p = i;
		}
	}
	if(!c && p >= 0)
	{
		x = pp2_player[p].view->offset_x;
		y = pp2_player[p].view->offset_y;
		w = pp2_player[p].view->width;
		h = pp2_player[p].view->height;
		t3f_draw_scaled_bitmap(resources->bitmap[PP2_BITMAP_EMPTY_PLAYER], al_map_rgba_f(0.0, 0.0, 0.0, 1.0), x, y, 0.0, w, h, 0);
	}
	else
	{
		for(i = 0; i < 4; i++)
		{
			if(used[i])
			{
				render_single_viewport_backdrop(i, t3f_color_black, resources);
			}
			else
			{
				render_single_viewport_backdrop(i, t3f_color_white, resources);
			}
		}
	}
	if(pp2_winner >= 0)
	{
		x = pp2_player[pp2_winner].view->offset_x;
		y = pp2_player[pp2_winner].view->offset_y;
		w = pp2_player[pp2_winner].view->width;
		h = pp2_player[pp2_winner].view->height;
		t3f_draw_scaled_bitmap(resources->bitmap[PP2_BITMAP_EMPTY_PLAYER], al_map_rgba_f(0.0, 0.0, 0.0, 1.0), x, y, 0.0, w, h, 0);
	}
	al_hold_bitmap_drawing(false);
}

void pp2_game_render(PP2_RESOURCES * resources)
{
	int i;

	/* draw empty players */
	t3f_select_view(t3f_default_view);
	render_viewport_backdrop(resources);

	/* render player cameras */
	for(i = 0; i < PP2_MAX_PLAYERS; i++)
	{
		if(pp2_player[i].flags & PP2_PLAYER_FLAG_ACTIVE && ((pp2_client_game->player[i]->local && pp2_replay_player < 0) || (i == pp2_replay_player) || (i == pp2_winner)))
		{
			pp2_game_render_player_view(i, resources);
		}
	}

	if(t3f_key[ALLEGRO_KEY_TILDE] || t3f_key[104] || show_scores)
	{
		pp2_game_render_scoreboard("Current Scores");
	}
}

void pp2_game_over_logic(void)
{
	bool scale_done = false;
	int i;

	pp2_game_logic();
	if(pp2_client_game->player[pp2_winner]->local)
	{
		if(pp2_player[pp2_winner].view->offset_x > t3f_default_view->left)
		{
			pp2_player[pp2_winner].view->offset_x -= 8.0;
		}
		if(pp2_player[pp2_winner].view->offset_y > t3f_default_view->top)
		{
			pp2_player[pp2_winner].view->offset_y -= 4.5;
		}
		if(pp2_player[pp2_winner].view->width < PP2_SCREEN_VISIBLE_WIDTH)
		{
			pp2_player[pp2_winner].view->width += 8.0;
	//		pp2_player[pp2_winner].camera.z++;
		}
		else
		{
			scale_done = true;
		}
		if(pp2_player[pp2_winner].view->height < PP2_SCREEN_VISIBLE_HEIGHT)
		{
			pp2_player[pp2_winner].view->height += 4.5;
		}
		pp2_player[pp2_winner].view->need_update = true;
		t3f_select_view(pp2_player[pp2_winner].view);
		if(!pp2_camera_clamp_left(pp2_winner))
		{
			if(pp2_camera_clamp_right(pp2_winner))
			{
				pp2_camera_clamp_left(pp2_winner);
			}
		}
		if(!pp2_camera_clamp_top(pp2_winner))
		{
			if(pp2_camera_clamp_bottom(pp2_winner))
			{
				pp2_camera_clamp_top(pp2_winner);
			}
		}
	}
	if(scale_done)
	{
		/* only the master can bring up the menu */
		if(!pp2_client || pp2_client->master)
		{
			for(i = 0; i < PP2_MAX_PLAYERS; i++)
			{
				if(pp2_client_game->player[i]->playing && pp2_client_game->player[i]->local)
				{
					if(pp2_controller[i]->state[PP2_CONTROLLER_FIRE].pressed)
					{
						pp2_joystick_menu_activation = true;
						joynet_pause_game(pp2_client_game);
						break;
					}
				}
			}
		}
	}
}

void pp2_game_over_render(PP2_RESOURCES * resources)
{
	t3f_select_view(t3f_default_view);
	render_viewport_backdrop(resources);
	pp2_game_render_player_view(pp2_winner, resources);
	t3f_select_view(t3f_default_view);
	al_hold_bitmap_drawing(true);
	pp2_game_render_scoreboard("Final Scores");
	al_hold_bitmap_drawing(false);
}

void pp2_game_paused_logic(PP2_INSTANCE * instance)
{
	if(!pp2_client || pp2_client->master)
	{
		pp2_process_menu(pp2_menu[pp2_current_menu], instance);
//		t3f_process_gui(pp2_menu[pp2_current_menu]);
	}
}

void pp2_game_paused_render(PP2_RESOURCES * resources)
{
	ALLEGRO_STATE old_state;
	ALLEGRO_TRANSFORM identity;
//	pp2_game_render();
	if(!pp2_client || pp2_client->master)
	{
		t3f_select_view(t3f_default_view);
		if(resources->bitmap[PP2_BITMAP_SCREEN_COPY])
		{
			al_store_state(&old_state, ALLEGRO_STATE_TRANSFORM);
			al_identity_transform(&identity);
			al_use_transform(&identity);
			al_draw_bitmap(resources->bitmap[PP2_BITMAP_SCREEN_COPY], 0, 0, 0);
			al_restore_state(&old_state);
		}
		al_draw_filled_rectangle(0.0, 0.0, PP2_SCREEN_WIDTH, PP2_SCREEN_HEIGHT, al_map_rgba_f(0.0, 0.0, 0.0, 0.5));
		al_hold_bitmap_drawing(true);
		al_draw_tinted_bitmap(resources->bitmap[PP2_BITMAP_MENU_LOGO], al_map_rgba_f(0.0, 0.0, 0.0, 0.5), PP2_SCREEN_WIDTH / 2 - al_get_bitmap_width(resources->bitmap[PP2_BITMAP_MENU_LOGO]) / 2 + 2, 0.0 + 2, 0);
		al_draw_bitmap(resources->bitmap[PP2_BITMAP_MENU_LOGO], PP2_SCREEN_WIDTH / 2 - al_get_bitmap_width(resources->bitmap[PP2_BITMAP_MENU_LOGO]) / 2, 0.0, 0);
		t3f_render_gui(pp2_menu[pp2_current_menu]);
		al_hold_bitmap_drawing(false);
	}
	else
	{
		t3f_select_view(t3f_default_view);
		if(resources->bitmap[PP2_BITMAP_SCREEN_COPY])
		{
			al_store_state(&old_state, ALLEGRO_STATE_TRANSFORM);
			al_identity_transform(&identity);
			al_use_transform(&identity);
			al_draw_bitmap(resources->bitmap[PP2_BITMAP_SCREEN_COPY], 0, 0, 0);
			al_restore_state(&old_state);
		}
		al_draw_filled_rectangle(0.0, 0.0, PP2_SCREEN_WIDTH, PP2_SCREEN_HEIGHT, al_map_rgba_f(0.0, 0.0, 0.0, 0.5));
		al_hold_bitmap_drawing(true);
		al_draw_tinted_bitmap(resources->bitmap[PP2_BITMAP_MENU_LOGO], al_map_rgba_f(0.0, 0.0, 0.0, 0.5), PP2_SCREEN_WIDTH / 2 - al_get_bitmap_width(resources->bitmap[PP2_BITMAP_MENU_LOGO]) / 2 + 2, 0.0 + 2, 0);
		al_draw_bitmap(resources->bitmap[PP2_BITMAP_MENU_LOGO], PP2_SCREEN_WIDTH / 2 - al_get_bitmap_width(resources->bitmap[PP2_BITMAP_MENU_LOGO]) / 2, 0.0, 0);
		al_hold_bitmap_drawing(false);
	}
}
