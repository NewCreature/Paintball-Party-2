#include <allegro5/allegro5.h>
#include "../t3f/t3f.h"
#include "../interface/title.h"
#include "../avc/avc.h"
#include "game.h"
#include "init.h"
#include "game_defines.h"
#include "replay.h"
#include "../resource.h"
#include "sprites/objects.h"

static int pp2_replay_camera_time = 0;
static float pp2_replay_fade = 1.0;
static int pp2_replay_input_offset = 0;
static int pp2_replay_tick = 0;
static bool pp2_replay_step = false;

static bool pp2_avc_replay_init(void * data);
static bool pp2_avc_replay_logic(void * data);
static void pp2_avc_replay_render(void * data);

static void pp2_replay_find_next_player(PP2_GAME * gp)
{
	int i;

	for(i = gp->replay_player + 1; i < PP2_MAX_PLAYERS; i++)
	{
		if(gp->player[i].playing && (gp->player[i].flags & PP2_PLAYER_FLAG_ACTIVE))
		{
			gp->replay_player = i;
			break;
		}
	}
	if(i >= PP2_MAX_PLAYERS)
	{
		for(i = 0; i < PP2_MAX_PLAYERS; i++)
		{
			if(gp->player[i].playing && (gp->player[i].flags & PP2_PLAYER_FLAG_ACTIVE))
			{
				gp->replay_player = i;
				break;
			}
		}
	}
	gp->local_player = gp->replay_player;
}

bool pp2_record_replay(PP2_GAME * gp, const char * fn, PP2_RESOURCES * resources)
{
	int i;
	char header[4] = {'P', '2', 'R', PP2_REPLAY_VERSION};

	gp->replay_file = al_fopen(fn, "wb");
	if(gp->replay_file)
	{
		al_fwrite(gp->replay_file, header, 4);
		for(i = 0; i < gp->client_game->options; i++)
		{
			al_fwrite32le(gp->replay_file, *gp->client_game->option[i]);
		}
		for(i = 0; i < PP2_MAX_PLAYERS; i++)
		{
			if(gp->player[i].playing)
			{
				al_fputc(gp->replay_file, 1);
				al_fwrite32le(gp->replay_file, resources->character_database->entry[gp->client_game->player[i]->selected_content_index[PP2_CONTENT_CHARACTERS]]->checksum);
				al_fwrite16le(gp->replay_file, strlen(gp->client_game->player[i]->name) + 1);
				al_fwrite(gp->replay_file, gp->client_game->player[i]->name, strlen(gp->client_game->player[i]->name) + 1);
			}
			else
			{
				al_fputc(gp->replay_file, 0);
			}
		}
		al_fwrite32le(gp->replay_file, resources->level_database->entry[gp->client_game->player[0]->selected_content_index[PP2_CONTENT_LEVELS]]->checksum);
		return true;
	}
	return false;
}

static const char * pp2_replay_fn = NULL;
static int pp2_replay_fl;

bool pp2_play_replay(PP2_GAME * gp, const char * fn, int flags, PP2_INTERFACE * ip, PP2_RESOURCES * resources)
{
	unsigned long choice = 0;
	int entry;
	int i, c;
	char header[4] = {0};
	char buffer[256] = {0};
	bool fail = false;

	if((flags & PP2_REPLAY_FLAG_CAPTURE) && !pp2_replay_fn)
	{
		pp2_replay_fn = fn;
		pp2_replay_fl = flags;
		al_stop_timer(t3f_timer);
		avc_start_capture(t3f_display, "myvideo.mp4", pp2_avc_replay_init, pp2_avc_replay_logic, pp2_avc_replay_render, 60, 0, resources);
		al_resume_timer(t3f_timer);
		pp2_replay_fn = NULL;
		return false;
	}
	gp->replay_flags = flags;
	gp->replay_done = false;
	pp2_replay_step = false;
	pp2_replay_fade = 1.0;
	gp->replay_file = al_fopen(fn, "rb");
	if(gp->replay_file)
	{
		pp2_replay_input_offset = 0;
		al_fread(gp->replay_file, header, 4);
		if(header[0] != 'P' || header[1] != '2' || header[2] != 'R')
		{
			fail = true;
		}
		if(header[3] != PP2_REPLAY_VERSION)
		{
			if(!(flags & PP2_REPLAY_FLAG_THEATER) && !(flags & PP2_REPLAY_FLAG_DEMO))
			{
				sprintf(buffer, "Replay is from incompatible version of the game.");
				pp2_add_message(ip->messages, buffer, resources->font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 0.0, 0.0, 1.0), 300, PP2_SCREEN_VISIBLE_WIDTH, 0.0);
			}
			fail = true;
		}
		if(fail)
		{
			al_fclose(gp->replay_file);
			gp->replay_file = NULL;
			gp->replay_flags = 0;
			return false;
		}
		pp2_replay_input_offset += 4;
		for(i = 0; i < gp->client_game->options; i++)
		{
			*gp->client_game->option[i] = al_fread32le(gp->replay_file);
			pp2_replay_input_offset += 4;
		}
		for(i = 0; i < PP2_MAX_PLAYERS; i++)
		{
			gp->player[i].playing = al_fgetc(gp->replay_file);
			pp2_replay_input_offset += 1;
//			gp->client_game->player[i]->local = 0;
			gp->player[i].controller = ip->controller[i];
			if(gp->player[i].playing)
			{
				choice = al_fread32le(gp->replay_file);
				pp2_replay_input_offset += 4;
				entry = pp2_database_find_entry(resources->character_database, choice);
				if(entry < 0)
				{
					al_fclose(gp->replay_file);
					gp->replay_file = NULL;
					gp->replay_flags = 0;
					return false;
				}
				gp->player[i].character_choice = entry;
				c = al_fread16le(gp->replay_file);
				pp2_replay_input_offset += 2;
				al_fread(gp->replay_file, gp->player[i].name, c);
				pp2_replay_input_offset += c;
			}
		}
		ip->level_hash = al_fread32le(gp->replay_file);
		pp2_replay_input_offset += 4;
		entry = pp2_database_find_entry(resources->level_database, ip->level_hash);
		if(entry < 0)
		{
			al_fclose(gp->replay_file);
			gp->replay_file = NULL;
			gp->replay_flags = 0;
			return false;
		}
		ip->level_choice = entry;
		pp2_game_init(gp, (flags & PP2_REPLAY_FLAG_CAPTURE) ? PP2_GAME_INIT_FLAG_CAPTURE : 0, ip, resources);
		gp->next_state = PP2_STATE_REPLAY;

		/* randomize camera */
		pp2_replay_camera_time = 300 + rand() % 120;
		if(flags & PP2_REPLAY_FLAG_THEATER)
		{
			for(i = 0; i < rand() % 16; i++)
			{
				pp2_replay_find_next_player(gp);
			}
		}
		return true;
	}
	return false;
}

void pp2_finish_replay(PP2_GAME * gp, PP2_RESOURCES * resources)
{
	pp2_game_free_data(gp, resources);
	al_fclose(gp->replay_file);
	gp->replay_file = NULL;
}

void pp2_finish_replay_recording(PP2_GAME * gp)
{
	al_fclose(gp->replay_file);
	gp->replay_file = NULL;
}

static bool pp2_avc_replay_init(void * data)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;
	return pp2_play_replay(&instance->game, pp2_replay_fn, pp2_replay_fl, &instance->ui, data);
}

bool pp2_replay_logic_tick(PP2_GAME * gp, PP2_INTERFACE * ip, PP2_RESOURCES * resources)
{
	int i, j;
	char bits[PP2_MAX_PLAYERS] = {0};
	const char * rp = NULL;
	bool played = false;
	bool ret = true;

	if(!gp->replay_done && !al_feof(gp->replay_file))
	{
		for(i = 0; i < PP2_MAX_PLAYERS; i++)
		{
			if(gp->player[i].playing)
			{
				bits[i] = al_fgetc(gp->replay_file);
			}
		}
		gp->radar_objects = 0;
		if(gp->time_left > 0)
		{
			gp->time_left--;
		}
		for(i = 0; i < PP2_MAX_PLAYERS; i++)
		{
			if(gp->player[i].playing)
			{
				for(j = 0; j < 8; j++)
				{
					gp->player[i].controller->state[j].down = bits[i] & (1 << j);
				}
				t3f_update_controller(gp->player[i].controller);
				pp2_player_logic(gp, &gp->player[i], resources);
				pp2_camera_logic(gp, i);
				if(gp->option[PP2_OPTION_TRAILS])
				{
					for(j = 0; j < PP2_MAX_TRAILS; j++)
					{
						pp2_paintball_trail_logic(&gp->player[i].trail[j]);
					}
				}
				for(j = 0; j < PP2_MAX_PAINTBALLS; j++)
				{
					pp2_paintball_logic(gp, &gp->player[i].paintball[j], resources);
				}
				for(j = 0; j < PP2_MAX_PARTICLES; j++)
				{
					pp2_particle_logic(gp, &gp->particle[j]);
					pp2_particle_logic(gp, &gp->player[i].particle[j]);
				}
			}
		}
		for(i = 0; i < gp->object_size; i++)
		{
			pp2_object_logic(gp, &gp->object[i], resources);
		}
		gp->tick++;
		if((gp->replay_flags & PP2_REPLAY_FLAG_DEMO) || (gp->replay_flags & PP2_REPLAY_FLAG_THEATER))
		{
			pp2_replay_fade -= 1.0 / 30.0;
			if(pp2_replay_fade < 0.0)
			{
				pp2_replay_fade = 0.0;
			}
		}
		if(!(gp->player[gp->replay_player].flags & PP2_PLAYER_FLAG_ACTIVE))
		{
			pp2_replay_find_next_player(gp);
		}
	}
	else if((gp->replay_flags & PP2_REPLAY_FLAG_DEMO) || (gp->replay_flags & PP2_REPLAY_FLAG_CAPTURE))
	{
		pp2_replay_fade += 1.0 / 30.0;
		if(pp2_replay_fade >= 1.0)
		{
			pp2_title_setup(ip);
			al_show_mouse_cursor(t3f_display);
			gp->next_state = PP2_STATE_TITLE;
			pp2_finish_replay(gp, resources);
			ret = false;
		}
	}
	else if(gp->replay_flags & PP2_REPLAY_FLAG_THEATER)
	{
		pp2_replay_fade += 1.0 / 30.0;
		if(pp2_replay_fade >= 1.0)
		{
			/* play next replay */
			pp2_finish_replay(gp, resources);
			while(ip->replay_file_number < al_get_native_file_dialog_count(ip->replay_filechooser) && !played)
			{
				rp = al_get_native_file_dialog_path(ip->replay_filechooser, ip->replay_file_number);
				if(rp)
				{
					if(pp2_play_replay(gp, rp, PP2_REPLAY_FLAG_THEATER, ip, resources))
					{
						played = true;
					}
				}
				ip->replay_file_number++;
			}
			if(!played)
			{
				al_destroy_native_file_dialog(ip->replay_filechooser);
				ip->replay_filechooser = NULL;
				gp->next_state = PP2_STATE_MENU;
				ret = false;
			}
		}
	}
	return ret;
}

bool pp2_avc_replay_logic(void * data)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;
	return pp2_replay_logic_tick(&instance->game, &instance->ui, &instance->resources);
}

void pp2_replay_logic(PP2_GAME * gp, PP2_INTERFACE * ip, PP2_RESOURCES * resources)
{
	int i, j;
	int process_ticks = 1;

	if(!(gp->replay_flags & PP2_REPLAY_FLAG_DEMO))
	{
		if(t3f_key[ALLEGRO_KEY_TAB])
		{
			pp2_replay_find_next_player(gp);
			t3f_key[ALLEGRO_KEY_TAB] = 0;
		}
		if(t3f_key[ALLEGRO_KEY_EQUALS])
		{
			gp->player[gp->replay_player].camera.z += 4.0;
			if(gp->player[gp->replay_player].camera.z > 240.0)
			{
				gp->player[gp->replay_player].camera.z = 240.0;
			}
		}
		if(t3f_key[ALLEGRO_KEY_MINUS])
		{
			gp->player[gp->replay_player].camera.z -= 4.0;
			if(gp->player[gp->replay_player].camera.z < 0.0)
			{
				gp->player[gp->replay_player].camera.z = 0.0;
			}
		}
		if(t3f_key[ALLEGRO_KEY_ENTER])
		{
			if(pp2_replay_step)
			{
				pp2_replay_step = false;
			}
			else
			{
				pp2_replay_step = true;
			}
			t3f_key[ALLEGRO_KEY_ENTER] = 0;
		}
		if(t3f_key[ALLEGRO_KEY_UP])
		{
			al_stop_timer(t3f_timer);
			al_fseek(gp->replay_file, pp2_replay_input_offset, ALLEGRO_SEEK_SET);
			pp2_game_setup(gp, 0, ip, resources);
			al_start_timer(t3f_timer);
			t3f_key[ALLEGRO_KEY_UP] = 0;
		}
		if(!pp2_replay_step)
		{
			if(t3f_key[ALLEGRO_KEY_RIGHT])
			{
				for(i = 0; i < 4; i++)
				{
					process_ticks = 4;
				}
			}
			if(t3f_key[ALLEGRO_KEY_LEFT])
			{
				j = gp->tick - 60;
				if(j < 0)
				{
					j = 0;
				}
				al_stop_timer(t3f_timer);
				al_fseek(gp->replay_file, pp2_replay_input_offset, ALLEGRO_SEEK_SET);
				gp->replay_rewind = true;
				pp2_game_setup(gp, 0, ip, resources);
				process_ticks = j;
				al_start_timer(t3f_timer);
				t3f_key[ALLEGRO_KEY_LEFT] = 0;
			}
		}
	}
	if(gp->replay_flags != 0 || t3f_key[ALLEGRO_KEY_C])
	{
		if(gp->tick > pp2_replay_camera_time)
		{
			pp2_replay_find_next_player(gp);
			pp2_replay_camera_time += 300 + rand() % 120;
		}
	}
	if(pp2_replay_step)
	{
		if(t3f_key[ALLEGRO_KEY_LEFT])
		{
			j = gp->tick - 1;
			if(j < 0)
			{
				j = 0;
			}
			al_stop_timer(t3f_timer);
			al_fseek(gp->replay_file, pp2_replay_input_offset, ALLEGRO_SEEK_SET);
			gp->replay_rewind = true;
			pp2_game_setup(gp, 0, ip, resources);
			process_ticks = j;
			al_start_timer(t3f_timer);
			t3f_key[ALLEGRO_KEY_LEFT] = 0;
		}
		else if(t3f_key[ALLEGRO_KEY_RIGHT])
		{
			process_ticks = 1;
			t3f_key[ALLEGRO_KEY_RIGHT] = 0;
		}
		else
		{
			process_ticks = 0;
		}
	}
	else if(t3f_key[ALLEGRO_KEY_S])
	{
		if(pp2_replay_tick % 2)
		{
			process_ticks = 0;
		}
	}
	for(i = 0; i < process_ticks; i++)
	{
		if(!pp2_replay_logic_tick(gp, ip, resources))
		{
			gp->next_state = PP2_STATE_TITLE;
			gp->replay_rewind = false;
			break;
		}
	}
	pp2_replay_tick++;
}

void pp2_replay_render(PP2_GAME * gp, PP2_THEME * theme, PP2_RESOURCES * resources)
{
	pp2_game_render(gp, theme, resources);
	if(((gp->replay_flags & PP2_REPLAY_FLAG_DEMO) || (gp->replay_flags & PP2_REPLAY_FLAG_THEATER)) && pp2_replay_fade > 0.0)
	{
		t3f_select_view(t3f_default_view);
		al_draw_filled_rectangle(0.0, 0.0, PP2_SCREEN_WIDTH, PP2_SCREEN_HEIGHT, al_map_rgba_f(0.0, 0.0, 0.0, pp2_replay_fade));
	}
}

void pp2_avc_replay_render(void * data)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;
	pp2_replay_render(&instance->game, instance->theme, &instance->resources);
}
