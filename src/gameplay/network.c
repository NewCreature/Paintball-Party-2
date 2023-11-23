#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include "../joynet/joynet.h"
#include "../joynet/serialize.h"
#include "../t3f/sound.h"
#include "../pp2.h"
#include "../file/database/character.h"
#include "../file/database/level.h"
#include "../file/music.h"
#include "../legacy/animation.h"
#include "../network/server.h"
#include "../network/client.h"
#include "../interface/menu_proc.h"
#include "../interface/player_setup.h"
#include "../version.h"
#include "game.h"
#include "init.h"
#include "replay.h"

int pp2_game_channel_callback(JOYNET_MESSAGE * mp, void * data)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	switch(mp->type)
	{
		case JOYNET_GAME_MESSAGE_CHECK_ID:
		{
			joynet_watch_game(instance->game.client_game);
			break;
		}
		case JOYNET_GAME_MESSAGE_CHECK_ID_FAIL:
		{
			short len;
			char message[256] = {0};
			char buffer[1024] = {0};

			joynet_serialize(instance->game.client_game->serial_data, mp->data);
			joynet_getw(instance->game.client_game->serial_data, &len);
			joynet_read(instance->game.client_game->serial_data, message, len);
			pp2_add_message(instance->ui.messages, "Server and client are not compatible.", instance->resources.font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 0.0, 0.0, 1.0), 900, 640, 0.0);
			sprintf(buffer, "Server version: %s", message);
			pp2_add_message(instance->ui.messages, buffer, instance->resources.font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 0.0, 0.0, 1.0), 900, 640, 0.0);
			sprintf(buffer, "Client version: %s", PP2_VERSION_STRING);
			pp2_add_message(instance->ui.messages, buffer, instance->resources.font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 0.0, 0.0, 1.0), 900, 640, 0.0);
			enet_peer_disconnect(instance->client->peer, JOYNET_DISCONNECT_CLIENT_CLOSED);
			instance->ui.client_disconnected = true;
			pp2_select_menu(&instance->ui, PP2_MENU_MAIN);
			instance->ui.menu_stack_size = 0;
			pp2_player_setup_reset(&instance->game);
			break;
		}
		case JOYNET_GAME_MESSAGE_CONNECT:
		{
			short controller;
			short port;

			t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
			joynet_serialize(instance->game.client_game->serial_data, mp->data);
			joynet_getw(instance->game.client_game->serial_data, &controller);
			joynet_getw(instance->game.client_game->serial_data, &port);

			instance->game.player[port].step = PP2_PLAYER_STEP_SELECT_PROFILE;
			if(instance->game.client_game->player[port]->local)
			{
				joynet_update_player_options(instance->game.client_game, port);
			}
			instance->game.player[port].input_handler = instance->ui.input_handler[port];
			instance->game.player[port].playing = true;
			instance->game.player[port].profile_read = false;

			/* load blank animation initially */
			if(instance->ui.player_preview[port])
			{
				pp2_destroy_character_preview(instance->ui.player_preview[port]);
			}
			instance->ui.player_preview[port] = pp2_load_character_preview("data/graphics/empty.preview");
			break;
		}
		case JOYNET_GAME_MESSAGE_ADD_PLAYER:
		{
			short player;

			joynet_serialize(instance->game.client_game->serial_data, mp->data);
			joynet_getw(instance->game.client_game->serial_data, &player);

			if(!instance->game.client_game->player[player]->local)
			{
				if(instance->game.player[player].step <= 0)
				{
					if(instance->ui.player_preview[player])
					{
						pp2_destroy_character_preview(instance->ui.player_preview[player]);
					}
					instance->ui.player_preview[player] = pp2_load_character_preview("data/graphics/empty.preview");
				}
			}
			instance->game.player[player].input_handler = instance->ui.input_handler[player];
			instance->game.player[player].playing = true;

			/* use global network ID until player selects a profile */
			if(instance->game.client_game->player[player]->local && instance->game.player[player].step == PP2_PLAYER_STEP_SELECT_PROFILE)
			{
				strcpy(instance->game.client_game->player[player]->name, instance->ui.network_id);
				joynet_update_player_options(instance->game.client_game, player);
			}
			break;
		}
		case JOYNET_GAME_MESSAGE_REMOVE_PLAYER:
		{
			short player;

			joynet_serialize(instance->game.client_game->serial_data, mp->data);
			joynet_getw(instance->game.client_game->serial_data, &player);
			instance->game.player[player].playing = false;
			instance->game.player[player].flags = 0;

			/* if we removed a local player, get out of the game and inform the
			 * player */
			if(instance->game.client_game->player[player]->local)
			{
				pp2_add_message(instance->ui.messages, "You have been removed from the game.", instance->resources.font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 0.0, 0.0, 1.0), 900, 640, 0.0);
				al_show_mouse_cursor(t3f_display);
				instance->state = PP2_STATE_MENU;
				if(instance->server_thread)
				{
					instance->ui.current_menu = PP2_MENU_MAIN_HOST;
					instance->ui.menu_stack_size = 0;
				}
				else if(instance->client)
				{
					instance->ui.current_menu = PP2_MENU_MAIN_CLIENT;
					instance->ui.menu_stack_size = 0;
				}
				else
				{
					instance->ui.current_menu = PP2_MENU_MAIN;
					instance->ui.menu_stack_size = 0;
				}
				if(instance->theme->menu_music_fn)
				{
					pp2_play_music(instance->theme->menu_music_fn);
				}
				else if(instance->theme->theme_music_fn)
				{
					pp2_play_music(instance->theme->theme_music_fn);
				}
				else
				{
					pp2_stop_music();
				}
			}
			break;
		}
		case JOYNET_GAME_MESSAGE_UPDATE_OPTION:
		{
			break;
		}
		case JOYNET_GAME_MESSAGE_UPDATE_PLAYER_OPTIONS:
		{
			short player;
			int i;

			joynet_serialize(instance->game.client_game->serial_data, mp->data);
			joynet_getw(instance->game.client_game->serial_data, &player);

			/* as soon as local player selects profile, update selections from profile */
			if(instance->game.client_game->player[player]->local)
			{
				if(instance->game.player[player].step == PP2_PLAYER_STEP_SELECTED_PROFILE)
				{
					/* select content from profile */
					instance->game.player[player].step = PP2_PLAYER_STEP_CHARACTER_WAIT;
					joynet_update_player_options(instance->game.client_game, player);
					joynet_select_game_content(instance->game.client_game, player, PP2_CONTENT_CHARACTERS, instance->game.player[player].profile->character);
					instance->game.player[player].character_choice = pp2_database_find_entry(instance->resources.character_database, instance->game.player[player].profile->character);
					if(instance->game.player[player].character_choice < 0)
					{
						instance->game.player[player].character_choice = 0;
					}
					instance->game.player[player].character_choosing = -1;
					for(i = 0; i < instance->game.client_game->content_list[PP2_CONTENT_CHARACTERS]->count; i++)
					{
						if(instance->game.client_game->content_list[PP2_CONTENT_CHARACTERS]->hash[i] == instance->game.player[player].profile->character)
						{
							instance->game.player[player].character_choosing = i;
							break;
						}
					}
					if(instance->game.player[player].character_choosing < 0)
					{
						instance->game.player[player].character_choosing = 0;
					}
					if(instance->ui.player_preview[player])
					{
						pp2_destroy_character_preview(instance->ui.player_preview[player]);
					}
					instance->ui.player_preview[player] = pp2_load_character_preview(((PP2_CHARACTER_DATABASE_EXTRA *)instance->resources.character_database->entry[instance->game.player[player].character_choice]->extra)->preview);
					instance->game.player[player].profile_read = true;
				}
			}
			if(instance->game.player[player].step == PP2_PLAYER_STEP_DONE)
			{
				if(instance->ui.player_preview[player]->sound)
				{
					t3f_play_sample(instance->ui.player_preview[player]->sound, 1.0, 0.0, 1.0);
					memcpy(&instance->ui.player_preview[player]->sound_id, &t3f_sample_id, sizeof(ALLEGRO_SAMPLE_ID));
				}
				else
				{
					t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
				}
			}
			break;
		}
		case JOYNET_GAME_MESSAGE_SELECT_PLAYER_CONTENT:
		{
			short player;
			short list;

			joynet_serialize(instance->game.client_game->serial_data, mp->data);
			joynet_getw(instance->game.client_game->serial_data, &player);
			joynet_getw(instance->game.client_game->serial_data, &list);
			if(list == PP2_CONTENT_CHARACTERS)
			{
				if(instance->ui.player_preview[player])
				{
					pp2_destroy_character_preview(instance->ui.player_preview[player]);
				}
				instance->ui.player_preview[player] = pp2_load_character_preview(((PP2_CHARACTER_DATABASE_EXTRA *)instance->resources.character_database->entry[instance->game.client_game->player[player]->selected_content_index[PP2_CONTENT_CHARACTERS]]->extra)->preview);
				instance->game.player[player].character_choice = instance->game.client_game->player[player]->selected_content_index[PP2_CONTENT_CHARACTERS];
				if(instance->game.client_game->player[player]->local)
				{
					instance->game.player[player].profile->character = instance->resources.character_database->entry[instance->game.player[player].character_choice]->checksum;
				}
				if(instance->game.player[player].step == PP2_PLAYER_STEP_CHARACTER_WAIT)
				{
					instance->game.player[player].step = PP2_PLAYER_STEP_CHARACTER_FOUND;
					joynet_update_player_options(instance->game.client_game, player);
				}
			}
			else if(list == PP2_CONTENT_LEVELS)
			{
				int entry;

				instance->ui.level_hash = instance->game.client_game->player[player]->selected_content[PP2_CONTENT_LEVELS];
				entry = pp2_database_find_entry(instance->resources.level_database, instance->ui.level_hash);
				if(entry >= 0)
				{
					if(instance->ui.level_preview)
					{
						pp2_destroy_level_preview(instance->ui.level_preview);
					}
					instance->ui.level_preview = pp2_load_level_preview(((PP2_LEVEL_DATABASE_EXTRA *)instance->resources.level_database->entry[entry]->extra)->preview);
					if(instance->ui.level_preview)
					{
						instance->ui.level_chosen = 1; // so we know the level choice propogated through the network
					}
				}
			}
			break;
		}
		case JOYNET_GAME_MESSAGE_START:
		{
			pp2_spawn_client_keep_alive_thread(instance);
			if(!pp2_game_init(&instance->game, 0, &instance->ui, &instance->resources))
			{
				printf("could not start game\n");
			}
			else
			{
				instance->state = PP2_STATE_GAME;
			}
			pp2_kill_client_keep_alive_thread();
			break;
		}
		case JOYNET_GAME_MESSAGE_PAUSE:
		{
			char message[64] = {0};
			int i;

			instance->old_state = instance->state;
			al_show_mouse_cursor(t3f_display);
			instance->state = PP2_STATE_GAME_PAUSED;
			if(instance->resources.bitmap[PP2_BITMAP_SCREEN_COPY])
			{
				al_destroy_bitmap(instance->resources.bitmap[PP2_BITMAP_SCREEN_COPY]);
			}
			instance->resources.bitmap[PP2_BITMAP_SCREEN_COPY] = al_clone_bitmap(al_get_backbuffer(t3f_display));
			if(!instance->client || instance->client->master)
			{
				if(instance->old_state == PP2_STATE_GAME)
				{
					instance->ui.current_menu = PP2_MENU_PAUSE;
				}
				else
				{
					instance->ui.current_menu = PP2_MENU_GAME_OVER;
				}
			}
			else
			{
				pp2_add_message(instance->ui.messages, message, instance->resources.font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 0.0, 0.0, 1.0), 300, PP2_SCREEN_VISIBLE_WIDTH, 0.0);
			}
			if(instance->ui.joystick_menu_activation)
			{
				instance->ui.menu[instance->ui.current_menu]->hover_element = 0;
				instance->ui.joystick_menu_activation = false;
				for(i = 0; i < PP2_MAX_PLAYERS; i++)
				{
					if(instance->game.client_game->player[i]->playing && instance->game.client_game->player[i]->local)
					{
						instance->ui.input_handler[i]->element[PP2_CONTROLLER_FIRE].pressed = false;
						instance->ui.input_handler[i]->element[PP2_CONTROLLER_FIRE].held = 2;
					}
				}
			}
			else
			{
				instance->ui.menu[instance->ui.current_menu]->hover_element = -1;
			}
			break;
		}
		case JOYNET_GAME_MESSAGE_RESUME:
		{
			instance->state = instance->old_state;
			break;
		}
		case JOYNET_GAME_MESSAGE_END:
		{
			if(instance->game.replay_file)
			{
				pp2_finish_replay_recording(&instance->game);
			}
			pp2_game_free_data(&instance->game, &instance->resources);
			if(!instance->client || instance->client->master)
			{
				switch(instance->game.end_game_option)
				{
					case PP2_END_GAME_QUIT:
					{
						al_show_mouse_cursor(t3f_display);
						instance->state = PP2_STATE_MENU;
						if(instance->server_thread)
						{
							instance->ui.current_menu = PP2_MENU_MAIN_HOST;
							instance->ui.menu_stack_size = 0;
						}
						else if(instance->client)
						{
							instance->ui.current_menu = PP2_MENU_MAIN_CLIENT;
							instance->ui.menu_stack_size = 0;
						}
						else
						{
							instance->ui.current_menu = PP2_MENU_MAIN;
							instance->ui.menu_stack_size = 0;
						}
						if(instance->theme->menu_music_fn)
						{
							pp2_play_music(instance->theme->menu_music_fn);
						}
						else if(instance->theme->theme_music_fn)
						{
							pp2_play_music(instance->theme->theme_music_fn);
						}
						else
						{
							pp2_stop_music();
						}
						break;
					}
					case PP2_END_GAME_REMATCH:
					{
						joynet_start_game(instance->game.client_game);
						break;
					}
					case PP2_END_GAME_PLAY_AGAIN:
					{
						instance->game.seed = time(0);
						joynet_start_game(instance->game.client_game);
						break;
					}
					case PP2_END_GAME_NEW:
					{
						al_show_mouse_cursor(t3f_display);
						instance->state = PP2_STATE_PLAYER_SETUP;
						if(instance->theme->menu_music_fn)
						{
							pp2_play_music(instance->theme->menu_music_fn);
						}
						else if(instance->theme->theme_music_fn)
						{
							pp2_play_music(instance->theme->theme_music_fn);
						}
						else
						{
							pp2_stop_music();
						}
						break;
					}
				}
			}
			else
			{
				switch(instance->game.end_game_option)
				{
					case PP2_END_GAME_QUIT:
					{
						al_show_mouse_cursor(t3f_display);
						instance->state = PP2_STATE_MENU;
						instance->ui.current_menu = PP2_MENU_MAIN_CLIENT;
						instance->ui.menu_stack_size = 0;
						if(instance->theme->menu_music_fn)
						{
							pp2_play_music(instance->theme->menu_music_fn);
						}
						else if(instance->theme->theme_music_fn)
						{
							pp2_play_music(instance->theme->theme_music_fn);
						}
						else
						{
							pp2_stop_music();
						}
						break;
					}
					case PP2_END_GAME_NEW:
					{
						al_show_mouse_cursor(t3f_display);
						instance->state = PP2_STATE_PLAYER_SETUP;
						if(instance->theme->menu_music_fn)
						{
							pp2_play_music(instance->theme->menu_music_fn);
						}
						else if(instance->theme->theme_music_fn)
						{
							pp2_play_music(instance->theme->theme_music_fn);
						}
						else
						{
							pp2_stop_music();
						}
						break;
					}
					default:
					{
						break;
					}
				}
			}
			break;
		}
		case PP2_GAME_MESSAGE_TYPING:
		{
			char player = 0;
			char players = 0;
			int i;

			joynet_serialize(instance->game.client_game->serial_data, mp->data);
			joynet_getc(instance->game.client_game->serial_data, &players);
			for(i = 0; i < players; i++)
			{
				joynet_getc(instance->game.client_game->serial_data, &player);
				instance->game.player[(int)player].flags |= PP2_PLAYER_FLAG_TYPING;
			}
			break;
		}
		case PP2_GAME_MESSAGE_TYPING_DONE:
		{
			char player = 0;
			char players = 0;
			int i;

			joynet_serialize(instance->game.client_game->serial_data, mp->data);
			joynet_getc(instance->game.client_game->serial_data, &players);
			for(i = 0; i < players; i++)
			{
				joynet_getc(instance->game.client_game->serial_data, &player);
				instance->game.player[(int)player].flags = instance->game.player[i].flags & ~PP2_PLAYER_FLAG_TYPING;
			}
			break;
		}
	}
	return 0;
}

int pp2_chat_callback(char * user, char * message, void * data)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;
	char bmessage[1024] = {0};
	char buser[256] = {0};
	float tab = 0.0;

	sprintf(buser, "%s: ", user);
	tab = t3f_get_text_width(instance->resources.font[PP2_FONT_SMALL], buser);
	sprintf(bmessage, "%s%s", buser, message);
	pp2_add_message(instance->ui.messages, bmessage, instance->resources.font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 1.0, 0.0, 1.0), -1, 640, tab);

	return 0;
}
