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
#include "../data.h"
#include "../version.h"
#include "game.h"
#include "replay.h"

static PP2_INSTANCE * _pp2_instance_ref = NULL;

void pp2_set_network_instance(PP2_INSTANCE * instance)
{
	_pp2_instance_ref = instance;
}

int pp2_game_channel_callback(JOYNET_MESSAGE * mp)
{
	switch(mp->type)
	{
		case JOYNET_GAME_MESSAGE_CHECK_ID:
		{
			joynet_watch_game(pp2_client_game);
			break;
		}
		case JOYNET_GAME_MESSAGE_CHECK_ID_FAIL:
		{
			short len;
			char message[256] = {0};
			char buffer[1024] = {0};

			joynet_serialize(pp2_client_game->serial_data, mp->data);
			joynet_getw(pp2_client_game->serial_data, &len);
			joynet_read(pp2_client_game->serial_data, message, len);
			pp2_add_message(pp2_messages, "Server and client are not compatible.", pp2_font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 0.0, 0.0, 1.0), 900, 640, 0.0);
			sprintf(buffer, "Server version: %s", message);
			pp2_add_message(pp2_messages, buffer, pp2_font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 0.0, 0.0, 1.0), 900, 640, 0.0);
			sprintf(buffer, "Client version: %s", PP2_VERSION_STRING);
			pp2_add_message(pp2_messages, buffer, pp2_font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 0.0, 0.0, 1.0), 900, 640, 0.0);
			enet_peer_disconnect(pp2_client->peer, JOYNET_DISCONNECT_CLIENT_CLOSED);
			pp2_client_disconnected = true;
			pp2_select_menu(PP2_MENU_MAIN);
			pp2_menu_stack_size = 0;
			pp2_player_setup_reset();
			break;
		}
		case JOYNET_GAME_MESSAGE_CONNECT:
		{
			short controller;
			short port;

			t3f_play_sample(pp2_sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
			joynet_serialize(pp2_client_game->serial_data, mp->data);
			joynet_getw(pp2_client_game->serial_data, &controller);
			joynet_getw(pp2_client_game->serial_data, &port);

			pp2_player[port].step = PP2_PLAYER_STEP_SELECT_PROFILE;
			if(pp2_client_game->player[port]->local)
			{
				joynet_update_player_options(pp2_client_game, port);
			}
			pp2_player[port].controller = port;
			pp2_player[port].playing = true;
			pp2_player[port].profile_read = false;

			/* load blank animation initially */
			if(pp2_player_preview[port])
			{
				pp2_destroy_character_preview(pp2_player_preview[port]);
			}
			pp2_player_preview[port] = pp2_load_character_preview("data/graphics/empty.preview");
			break;
		}
		case JOYNET_GAME_MESSAGE_ADD_PLAYER:
		{
			short player;

			joynet_serialize(pp2_client_game->serial_data, mp->data);
			joynet_getw(pp2_client_game->serial_data, &player);

			if(!pp2_client_game->player[player]->local)
			{
				if(pp2_player[player].step <= 0)
				{
					if(pp2_player_preview[player])
					{
						pp2_destroy_character_preview(pp2_player_preview[player]);
					}
					pp2_player_preview[player] = pp2_load_character_preview("data/graphics/empty.preview");
				}
			}
			pp2_player[player].controller = player;
			pp2_player[player].playing = true;

			/* use global network ID until player selects a profile */
			if(pp2_client_game->player[player]->local && pp2_player[player].step == PP2_PLAYER_STEP_SELECT_PROFILE)
			{
				strcpy(pp2_client_game->player[player]->name, pp2_network_id);
				joynet_update_player_options(pp2_client_game, player);
			}
			break;
		}
		case JOYNET_GAME_MESSAGE_REMOVE_PLAYER:
		{
			short player;

			joynet_serialize(pp2_client_game->serial_data, mp->data);
			joynet_getw(pp2_client_game->serial_data, &player);
			pp2_player[player].playing = false;
			pp2_player[player].flags = 0;

			/* if we removed a local player, get out of the game and inform the
			 * player */
			if(pp2_client_game->player[player]->local)
			{
				pp2_add_message(pp2_messages, "You have been removed from the game.", pp2_font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 0.0, 0.0, 1.0), 900, 640, 0.0);
				al_show_mouse_cursor(t3f_display);
				pp2_state = PP2_STATE_MENU;
				if(pp2_server_thread)
				{
					pp2_current_menu = PP2_MENU_MAIN_HOST;
					pp2_menu_stack_size = 0;
				}
				else if(pp2_client)
				{
					pp2_current_menu = PP2_MENU_MAIN_CLIENT;
					pp2_menu_stack_size = 0;
				}
				else
				{
					pp2_current_menu = PP2_MENU_MAIN;
					pp2_menu_stack_size = 0;
				}
				if(_pp2_instance_ref->theme->menu_music_fn)
				{
					pp2_play_music(_pp2_instance_ref->theme->menu_music_fn);
				}
				else if(_pp2_instance_ref->theme->theme_music_fn)
				{
					pp2_play_music(_pp2_instance_ref->theme->theme_music_fn);
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

			joynet_serialize(pp2_client_game->serial_data, mp->data);
			joynet_getw(pp2_client_game->serial_data, &player);

			/* as soon as local player selects profile, update selections from profile */
			if(pp2_client_game->player[player]->local)
			{
				if(pp2_player[player].step == PP2_PLAYER_STEP_SELECTED_PROFILE)
				{
					/* select content from profile */
					pp2_player[player].step = PP2_PLAYER_STEP_CHARACTER_WAIT;
					joynet_update_player_options(pp2_client_game, player);
					joynet_select_game_content(pp2_client_game, player, PP2_CONTENT_CHARACTERS, pp2_profiles.item[pp2_player[player].profile_choice].character);
					pp2_player[player].character_choice = pp2_database_find_entry(pp2_character_database, pp2_profiles.item[pp2_player[player].profile_choice].character);
					if(pp2_player[player].character_choice < 0)
					{
						pp2_player[player].character_choice = 0;
					}
					pp2_player[player].character_choosing = -1;
					for(i = 0; i < pp2_client_game->content_list[PP2_CONTENT_CHARACTERS]->count; i++)
					{
						if(pp2_client_game->content_list[PP2_CONTENT_CHARACTERS]->hash[i] == pp2_profiles.item[pp2_player[player].profile_choice].character)
						{
							pp2_player[player].character_choosing = i;
							break;
						}
					}
					if(pp2_player[player].character_choosing < 0)
					{
						pp2_player[player].character_choosing = 0;
					}
					if(pp2_player_preview[player])
					{
						pp2_destroy_character_preview(pp2_player_preview[player]);
					}
					pp2_player_preview[player] = pp2_load_character_preview(((PP2_CHARACTER_DATABASE_EXTRA *)pp2_character_database->entry[pp2_player[player].character_choice]->extra)->preview);
					pp2_player[player].profile_read = true;
				}
			}
			if(pp2_player[player].step == PP2_PLAYER_STEP_DONE)
			{
				if(pp2_player_preview[player]->sound)
				{
					t3f_play_sample(pp2_player_preview[player]->sound, 1.0, 0.0, 1.0);
					memcpy(&pp2_player_preview[player]->sound_id, &t3f_sample_id, sizeof(ALLEGRO_SAMPLE_ID));
				}
				else
				{
					t3f_play_sample(pp2_sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
				}
			}
			break;
		}
		case JOYNET_GAME_MESSAGE_SELECT_PLAYER_CONTENT:
		{
			short player;
			short list;

			joynet_serialize(pp2_client_game->serial_data, mp->data);
			joynet_getw(pp2_client_game->serial_data, &player);
			joynet_getw(pp2_client_game->serial_data, &list);
			if(list == PP2_CONTENT_CHARACTERS)
			{
				if(pp2_player_preview[player])
				{
					pp2_destroy_character_preview(pp2_player_preview[player]);
				}
				pp2_player_preview[player] = pp2_load_character_preview(((PP2_CHARACTER_DATABASE_EXTRA *)pp2_character_database->entry[pp2_client_game->player[player]->selected_content_index[PP2_CONTENT_CHARACTERS]]->extra)->preview);
				pp2_player[player].character_choice = pp2_client_game->player[player]->selected_content_index[PP2_CONTENT_CHARACTERS];
				if(pp2_client_game->player[player]->local)
				{
					pp2_profiles.item[pp2_player[player].profile_choice].character = pp2_character_database->entry[pp2_player[player].character_choice]->checksum;
				}
				if(pp2_player[player].step == PP2_PLAYER_STEP_CHARACTER_WAIT)
				{
					pp2_player[player].step = PP2_PLAYER_STEP_CHARACTER_FOUND;
					joynet_update_player_options(pp2_client_game, player);
				}
			}
			else if(list == PP2_CONTENT_LEVELS)
			{
				int entry;

				pp2_level_hash = pp2_client_game->player[player]->selected_content[PP2_CONTENT_LEVELS];
				entry = pp2_database_find_entry(pp2_level_database, pp2_level_hash);
				if(entry >= 0)
				{
					if(pp2_level_preview)
					{
						pp2_destroy_level_preview(pp2_level_preview);
					}
					pp2_level_preview = pp2_load_level_preview(((PP2_LEVEL_DATABASE_EXTRA *)pp2_level_database->entry[entry]->extra)->preview);
					if(pp2_level_preview)
					{
						pp2_level_chosen = 1; // so we know the level choice propogated through the network
					}
				}
			}
			break;
		}
		case JOYNET_GAME_MESSAGE_START:
		{
			pp2_spawn_client_keep_alive_thread();
			if(!pp2_game_init(0, &_pp2_instance_ref->resources))
			{
				printf("could not start game\n");
			}
			pp2_kill_client_keep_alive_thread();
			break;
		}
		case JOYNET_GAME_MESSAGE_PAUSE:
		{
			char message[64] = {0};
			int i;

			pp2_old_state = pp2_state;
			al_show_mouse_cursor(t3f_display);
			pp2_state = PP2_STATE_GAME_PAUSED;
			if(_pp2_instance_ref->resources.bitmap[PP2_BITMAP_SCREEN_COPY])
			{
				al_destroy_bitmap(_pp2_instance_ref->resources.bitmap[PP2_BITMAP_SCREEN_COPY]);
			}
			_pp2_instance_ref->resources.bitmap[PP2_BITMAP_SCREEN_COPY] = al_clone_bitmap(al_get_backbuffer(t3f_display));
			if(!pp2_client || pp2_client->master)
			{
				if(pp2_old_state == PP2_STATE_GAME)
				{
					pp2_current_menu = PP2_MENU_PAUSE;
				}
				else
				{
					pp2_current_menu = PP2_MENU_GAME_OVER;
				}
			}
			else
			{
				pp2_add_message(pp2_messages, message, pp2_font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 0.0, 0.0, 1.0), 300, PP2_SCREEN_VISIBLE_WIDTH, 0.0);
			}
			if(pp2_joystick_menu_activation)
			{
				pp2_menu[pp2_current_menu]->hover_element = 0;
				pp2_joystick_menu_activation = false;
				for(i = 0; i < PP2_MAX_PLAYERS; i++)
				{
					if(pp2_client_game->player[i]->playing && pp2_client_game->player[i]->local)
					{
						pp2_controller[i]->state[PP2_CONTROLLER_FIRE].pressed = false;
						pp2_controller[i]->state[PP2_CONTROLLER_FIRE].held = 2;
					}
				}
			}
			else
			{
				pp2_menu[pp2_current_menu]->hover_element = -1;
			}
			break;
		}
		case JOYNET_GAME_MESSAGE_RESUME:
		{
			pp2_state = pp2_old_state;
			break;
		}
		case JOYNET_GAME_MESSAGE_END:
		{
			if(pp2_replay_file)
			{
				pp2_finish_replay_recording();
			}
			pp2_game_free_data();
			if(!pp2_client || pp2_client->master)
			{
				switch(pp2_end_game_option)
				{
					case PP2_END_GAME_QUIT:
					{
						al_show_mouse_cursor(t3f_display);
						pp2_state = PP2_STATE_MENU;
						if(pp2_server_thread)
						{
							pp2_current_menu = PP2_MENU_MAIN_HOST;
							pp2_menu_stack_size = 0;
						}
						else if(pp2_client)
						{
							pp2_current_menu = PP2_MENU_MAIN_CLIENT;
							pp2_menu_stack_size = 0;
						}
						else
						{
							pp2_current_menu = PP2_MENU_MAIN;
							pp2_menu_stack_size = 0;
						}
						if(_pp2_instance_ref->theme->menu_music_fn)
						{
							pp2_play_music(_pp2_instance_ref->theme->menu_music_fn);
						}
						else if(_pp2_instance_ref->theme->theme_music_fn)
						{
							pp2_play_music(_pp2_instance_ref->theme->theme_music_fn);
						}
						else
						{
							pp2_stop_music();
						}
						break;
					}
					case PP2_END_GAME_REMATCH:
					{
						joynet_start_game(pp2_client_game);
						break;
					}
					case PP2_END_GAME_PLAY_AGAIN:
					{
						pp2_seed = time(0);
						joynet_start_game(pp2_client_game);
						break;
					}
					case PP2_END_GAME_NEW:
					{
						al_show_mouse_cursor(t3f_display);
						pp2_state = PP2_STATE_PLAYER_SETUP;
						if(_pp2_instance_ref->theme->menu_music_fn)
						{
							pp2_play_music(_pp2_instance_ref->theme->menu_music_fn);
						}
						else if(_pp2_instance_ref->theme->theme_music_fn)
						{
							pp2_play_music(_pp2_instance_ref->theme->theme_music_fn);
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
				switch(pp2_end_game_option)
				{
					case PP2_END_GAME_QUIT:
					{
						al_show_mouse_cursor(t3f_display);
						pp2_state = PP2_STATE_MENU;
						pp2_current_menu = PP2_MENU_MAIN_CLIENT;
						pp2_menu_stack_size = 0;
						if(_pp2_instance_ref->theme->menu_music_fn)
						{
							pp2_play_music(_pp2_instance_ref->theme->menu_music_fn);
						}
						else if(_pp2_instance_ref->theme->theme_music_fn)
						{
							pp2_play_music(_pp2_instance_ref->theme->theme_music_fn);
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
						pp2_state = PP2_STATE_PLAYER_SETUP;
						if(_pp2_instance_ref->theme->menu_music_fn)
						{
							pp2_play_music(_pp2_instance_ref->theme->menu_music_fn);
						}
						else if(_pp2_instance_ref->theme->theme_music_fn)
						{
							pp2_play_music(_pp2_instance_ref->theme->theme_music_fn);
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

			joynet_serialize(pp2_client_game->serial_data, mp->data);
			joynet_getc(pp2_client_game->serial_data, &players);
			for(i = 0; i < players; i++)
			{
				joynet_getc(pp2_client_game->serial_data, &player);
				pp2_player[(int)player].flags |= PP2_PLAYER_FLAG_TYPING;
			}
			break;
		}
		case PP2_GAME_MESSAGE_TYPING_DONE:
		{
			char player = 0;
			char players = 0;
			int i;

			joynet_serialize(pp2_client_game->serial_data, mp->data);
			joynet_getc(pp2_client_game->serial_data, &players);
			for(i = 0; i < players; i++)
			{
				joynet_getc(pp2_client_game->serial_data, &player);
				pp2_player[(int)player].flags = pp2_player[i].flags & ~PP2_PLAYER_FLAG_TYPING;
			}
			break;
		}
	}
	return 0;
}

int pp2_chat_callback(char * user, char * message)
{
	char bmessage[1024] = {0};
	char buser[256] = {0};
	float tab = 0.0;

	sprintf(buser, "%s: ", user);
	tab = al_get_text_width(pp2_font[PP2_FONT_SMALL], buser);
	sprintf(bmessage, "%s%s", buser, message);
	pp2_add_message(pp2_messages, bmessage, pp2_font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 1.0, 0.0, 1.0), -1, 640, tab);

	return 0;
}
