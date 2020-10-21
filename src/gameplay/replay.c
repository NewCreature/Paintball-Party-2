#include <allegro5/allegro5.h>
#include "../t3f/t3f.h"
#include "../data.h"
#include "../interface/title.h"
#include "../avc/avc.h"
#include "game.h"
#include "game_defines.h"
#include "replay.h"
#include "../resource.h"

int pp2_replay_flags = 0;
bool pp2_replay_done = false;
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

	for(i = pp2_replay_player + 1; i < PP2_MAX_PLAYERS; i++)
	{
		if(gp->player[i].playing && (gp->player[i].flags & PP2_PLAYER_FLAG_ACTIVE))
		{
			pp2_replay_player = i;
			break;
		}
	}
	if(i >= PP2_MAX_PLAYERS)
	{
		for(i = 0; i < PP2_MAX_PLAYERS; i++)
		{
			if(gp->player[i].playing && (gp->player[i].flags & PP2_PLAYER_FLAG_ACTIVE))
			{
				pp2_replay_player = i;
				break;
			}
		}
	}
	pp2_local_player = pp2_replay_player;
}

bool pp2_record_replay(PP2_GAME * gp, const char * fn)
{
	int i;
	char header[4] = {'P', '2', 'R', PP2_REPLAY_VERSION};

	pp2_replay_file = al_fopen(fn, "wb");
	if(pp2_replay_file)
	{
		al_fwrite(pp2_replay_file, header, 4);
		for(i = 0; i < pp2_client_game->options; i++)
		{
			al_fwrite32le(pp2_replay_file, *pp2_client_game->option[i]);
		}
		for(i = 0; i < PP2_MAX_PLAYERS; i++)
		{
			if(gp->player[i].playing)
			{
				al_fputc(pp2_replay_file, 1);
				al_fwrite32le(pp2_replay_file, pp2_character_database->entry[pp2_client_game->player[i]->selected_content_index[PP2_CONTENT_CHARACTERS]]->checksum);
				al_fwrite16le(pp2_replay_file, strlen(pp2_client_game->player[i]->name) + 1);
				al_fwrite(pp2_replay_file, pp2_client_game->player[i]->name, strlen(pp2_client_game->player[i]->name) + 1);
			}
			else
			{
				al_fputc(pp2_replay_file, 0);
			}
		}
		al_fwrite32le(pp2_replay_file, pp2_level_database->entry[pp2_client_game->player[0]->selected_content_index[PP2_CONTENT_LEVELS]]->checksum);
		return true;
	}
	return false;
}

static const char * pp2_replay_fn = NULL;
static int pp2_replay_fl;

bool pp2_play_replay(PP2_GAME * gp, const char * fn, int flags, PP2_RESOURCES * resources)
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
	pp2_replay_flags = flags;
	pp2_replay_done = false;
	pp2_replay_step = false;
	pp2_replay_fade = 1.0;
	pp2_replay_file = al_fopen(fn, "rb");
	if(pp2_replay_file)
	{
		pp2_replay_input_offset = 0;
		al_fread(pp2_replay_file, header, 4);
		if(header[0] != 'P' || header[1] != '2' || header[2] != 'R')
		{
			fail = true;
		}
		if(header[3] != PP2_REPLAY_VERSION)
		{
			if(!(flags & PP2_REPLAY_FLAG_THEATER) && !(flags & PP2_REPLAY_FLAG_DEMO))
			{
				sprintf(buffer, "Replay is from incompatible version of the game.");
				pp2_add_message(pp2_messages, buffer, resources->font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 0.0, 0.0, 1.0), 300, PP2_SCREEN_VISIBLE_WIDTH, 0.0);
			}
			fail = true;
		}
		if(fail)
		{
			al_fclose(pp2_replay_file);
			pp2_replay_file = NULL;
			pp2_replay_flags = 0;
			return false;
		}
		pp2_replay_input_offset += 4;
		for(i = 0; i < pp2_client_game->options; i++)
		{
			*pp2_client_game->option[i] = al_fread32le(pp2_replay_file);
			pp2_replay_input_offset += 4;
		}
		for(i = 0; i < PP2_MAX_PLAYERS; i++)
		{
			gp->player[i].playing = al_fgetc(pp2_replay_file);
			pp2_replay_input_offset += 1;
//			pp2_client_game->player[i]->local = 0;
			gp->player[i].controller = i;
			if(gp->player[i].playing)
			{
				choice = al_fread32le(pp2_replay_file);
				pp2_replay_input_offset += 4;
				entry = pp2_database_find_entry(pp2_character_database, choice);
				if(entry < 0)
				{
					al_fclose(pp2_replay_file);
					pp2_replay_file = NULL;
					pp2_replay_flags = 0;
					return false;
				}
				gp->player[i].character_choice = entry;
				c = al_fread16le(pp2_replay_file);
				pp2_replay_input_offset += 2;
				al_fread(pp2_replay_file, gp->player[i].name, c);
				pp2_replay_input_offset += c;
			}
		}
		pp2_level_hash = al_fread32le(pp2_replay_file);
		pp2_replay_input_offset += 4;
		entry = pp2_database_find_entry(pp2_level_database, pp2_level_hash);
		if(entry < 0)
		{
			al_fclose(pp2_replay_file);
			pp2_replay_file = NULL;
			pp2_replay_flags = 0;
			return false;
		}
		pp2_level_choice = entry;
		pp2_game_init(gp, (flags & PP2_REPLAY_FLAG_CAPTURE) ? PP2_GAME_INIT_FLAG_CAPTURE : 0, resources);
		pp2_state = PP2_STATE_REPLAY;

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

void pp2_finish_replay(PP2_GAME * gp)
{
	pp2_game_free_data(gp);
	al_fclose(pp2_replay_file);
	pp2_replay_file = NULL;
}

void pp2_finish_replay_recording(void)
{
	al_fclose(pp2_replay_file);
	pp2_replay_file = NULL;
}

static bool pp2_avc_replay_init(void * data)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;
	return pp2_play_replay(&instance->game, pp2_replay_fn, pp2_replay_fl, data);
}

bool pp2_replay_logic_tick(PP2_GAME * gp, PP2_RESOURCES * resources)
{
	int i, j;
	char bits[PP2_MAX_PLAYERS] = {0};
	const char * rp = NULL;
	bool played = false;
	bool ret = true;

	if(!pp2_replay_done && !al_feof(pp2_replay_file))
	{
		for(i = 0; i < PP2_MAX_PLAYERS; i++)
		{
			if(gp->player[i].playing)
			{
				bits[i] = al_fgetc(pp2_replay_file);
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
					pp2_controller[i]->state[j].down = bits[i] & (1 << j);
				}
				t3f_update_controller(pp2_controller[i]);
				pp2_player_logic(gp, &gp->player[i], resources);
				pp2_camera_logic(gp, i);
				if(pp2_option[PP2_OPTION_TRAILS])
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
			pp2_object_logic(gp, &gp->object[i]);
		}
		pp2_tick++;
		if((pp2_replay_flags & PP2_REPLAY_FLAG_DEMO) || (pp2_replay_flags & PP2_REPLAY_FLAG_THEATER))
		{
			pp2_replay_fade -= 1.0 / 30.0;
			if(pp2_replay_fade < 0.0)
			{
				pp2_replay_fade = 0.0;
			}
		}
		if(!(gp->player[pp2_replay_player].flags & PP2_PLAYER_FLAG_ACTIVE))
		{
			pp2_replay_find_next_player(gp);
		}
	}
	else if((pp2_replay_flags & PP2_REPLAY_FLAG_DEMO) || (pp2_replay_flags & PP2_REPLAY_FLAG_CAPTURE))
	{
		pp2_replay_fade += 1.0 / 30.0;
		if(pp2_replay_fade >= 1.0)
		{
			pp2_title_setup();
			al_show_mouse_cursor(t3f_display);
			pp2_state = PP2_STATE_TITLE;
			pp2_finish_replay(gp);
			ret = false;
		}
	}
	else if(pp2_replay_flags & PP2_REPLAY_FLAG_THEATER)
	{
		pp2_replay_fade += 1.0 / 30.0;
		if(pp2_replay_fade >= 1.0)
		{
			/* play next replay */
			pp2_finish_replay(gp);
			while(pp2_replay_file_number < al_get_native_file_dialog_count(pp2_replay_filechooser) && !played)
			{
				rp = al_get_native_file_dialog_path(pp2_replay_filechooser, pp2_replay_file_number);
				if(rp)
				{
					if(pp2_play_replay(gp, rp, PP2_REPLAY_FLAG_THEATER, resources))
					{
						played = true;
					}
				}
				pp2_replay_file_number++;
			}
			if(!played)
			{
				al_destroy_native_file_dialog(pp2_replay_filechooser);
				pp2_replay_filechooser = NULL;
				pp2_state = PP2_STATE_MENU;
				ret = false;
			}
		}
	}
	return ret;
}

bool pp2_avc_replay_logic(void * data)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;
	return pp2_replay_logic_tick(&instance->game, &instance->resources);
}

void pp2_replay_logic(PP2_GAME * gp, PP2_RESOURCES * resources)
{
	bool skip = false;
	int i, j;

	if(!(pp2_replay_flags & PP2_REPLAY_FLAG_DEMO))
	{
		if(t3f_key[ALLEGRO_KEY_TAB])
		{
			pp2_replay_find_next_player(gp);
			t3f_key[ALLEGRO_KEY_TAB] = 0;
		}
		if(t3f_key[ALLEGRO_KEY_EQUALS])
		{
			gp->player[pp2_replay_player].camera.z += 4.0;
			if(gp->player[pp2_replay_player].camera.z > 240.0)
			{
				gp->player[pp2_replay_player].camera.z = 240.0;
			}
		}
		if(t3f_key[ALLEGRO_KEY_MINUS])
		{
			gp->player[pp2_replay_player].camera.z -= 4.0;
			if(gp->player[pp2_replay_player].camera.z < 0.0)
			{
				gp->player[pp2_replay_player].camera.z = 0.0;
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
			al_fseek(pp2_replay_file, pp2_replay_input_offset, ALLEGRO_SEEK_SET);
			pp2_game_setup(gp, 0, resources);
			al_start_timer(t3f_timer);
			t3f_key[ALLEGRO_KEY_UP] = 0;
		}
		if(!pp2_replay_step)
		{
			if(t3f_key[ALLEGRO_KEY_RIGHT])
			{
				skip = true;
				for(i = 0; i < 4; i++)
				{
					if(!pp2_replay_logic_tick(gp, resources))
					{
						break;
					}
				}
			}
			if(t3f_key[ALLEGRO_KEY_LEFT])
			{
				j = pp2_tick - 60;
				if(j < 0)
				{
					j = 0;
				}
				al_stop_timer(t3f_timer);
				al_fseek(pp2_replay_file, pp2_replay_input_offset, ALLEGRO_SEEK_SET);
				pp2_replay_rewind = true;
				pp2_game_setup(gp, 0, resources);
				for(i = 0; i < j; i++)
				{
					pp2_replay_logic_tick(gp, resources);
				}
				pp2_replay_rewind = false;
				al_start_timer(t3f_timer);
				t3f_key[ALLEGRO_KEY_LEFT] = 0;
			}
		}
	}
	if(pp2_replay_flags != 0 || t3f_key[ALLEGRO_KEY_C])
	{
		if(pp2_tick > pp2_replay_camera_time)
		{
			pp2_replay_find_next_player(gp);
			pp2_replay_camera_time += 300 + rand() % 120;
		}
	}
	if(pp2_replay_step)
	{
		if(t3f_key[ALLEGRO_KEY_LEFT])
		{
			j = pp2_tick - 1;
			if(j < 0)
			{
				j = 0;
			}
			al_stop_timer(t3f_timer);
			al_fseek(pp2_replay_file, pp2_replay_input_offset, ALLEGRO_SEEK_SET);
			pp2_replay_rewind = true;
			pp2_game_setup(gp, 0, resources);
			for(i = 0; i < j; i++)
			{
				pp2_replay_logic_tick(gp, resources);
			}
			pp2_replay_rewind = false;
			al_start_timer(t3f_timer);
			t3f_key[ALLEGRO_KEY_LEFT] = 0;
		}
		if(t3f_key[ALLEGRO_KEY_RIGHT])
		{
			pp2_replay_logic_tick(gp, resources);
			t3f_key[ALLEGRO_KEY_RIGHT] = 0;
		}
	}
	else if(t3f_key[ALLEGRO_KEY_S])
	{
		pp2_replay_tick++;
		if(pp2_replay_tick % 2 == 0)
		{
			pp2_replay_logic_tick(gp, resources);
		}
	}
	else
	{
		pp2_replay_tick = 0;
		if(!skip)
		{
			pp2_replay_logic_tick(gp, resources);
		}
	}
}

void pp2_replay_render(PP2_GAME * gp, PP2_RESOURCES * resources)
{
	pp2_game_render(gp, resources);
	if(((pp2_replay_flags & PP2_REPLAY_FLAG_DEMO) || (pp2_replay_flags & PP2_REPLAY_FLAG_THEATER)) && pp2_replay_fade > 0.0)
	{
		t3f_select_view(t3f_default_view);
		al_draw_filled_rectangle(0.0, 0.0, PP2_SCREEN_WIDTH, PP2_SCREEN_HEIGHT, al_map_rgba_f(0.0, 0.0, 0.0, pp2_replay_fade));
	}
}

void pp2_avc_replay_render(void * data)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;
	pp2_replay_render(&instance->game, &instance->resources);
}
