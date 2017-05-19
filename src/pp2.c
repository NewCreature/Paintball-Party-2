#include "t3f/t3f.h"
#include "t3f/controller.h"
#include "t3f/gui.h"
#include "t3f/sound.h"
#include "t3f/resource.h"
#include <stdio.h>
#include <stdlib.h>
#include "joynet/joynet.h"
#include "joynet/game.h"
#include "t3net/t3net.h"

#include "file/music.h"
#include "gameplay/replay.h"
#include "interface/menu_proc.h"
#include "interface/intro.h"
#include "gameplay/game.h"
#include "interface/menu.h"
#include "interface/player_setup.h"
#include "interface/level_setup.h"
#include "file/config.h"
#include "data.h"
#include "tables.h"
#include "text_entry.h"
#include "init.h"

void pp2_logic(void * data)
{
	/* network logic */
	if(pp2_client)
	{
		joynet_poll_client(pp2_client);
		if(pp2_client_disconnected)
		{
			joynet_destroy_client(pp2_client);
			pp2_client = NULL;
			pp2_client_disconnected = false;
		}
	}

	/* sound queue logic */
	t3f_poll_sound_queue();

	pp2_enter_text();
	if(t3f_key[ALLEGRO_KEY_ESCAPE])
	{
		if(pp2_entering_text)
		{
			pp2_entering_text = 0;
			pp2_entered_text[0] = 0;
		}
		else if(pp2_state == PP2_STATE_GAME || pp2_state == PP2_STATE_GAME_OVER)
		{
			joynet_pause_game(pp2_client_game);
		}
		else if(pp2_state == PP2_STATE_REPLAY)
		{
			if((pp2_replay_flags & PP2_REPLAY_FLAG_DEMO) || (pp2_replay_flags & PP2_REPLAY_FLAG_THEATER))
			{
				pp2_replay_done = true;
			}
			else
			{
				pp2_finish_replay();
				pp2_current_menu = PP2_MENU_MAIN;
				pp2_menu_stack_size = 0;
				pp2_play_music("data/music/theme.ogg");
				pp2_state = PP2_STATE_MENU;
			}
		}
		else
		{
			pp2_menu_proc_overlay_back(NULL, 0, NULL);
		}
		t3f_key[ALLEGRO_KEY_ESCAPE] = 0;
	}

	/* online chat */
	if(t3f_key[ALLEGRO_KEY_T] && !pp2_entering_text && pp2_client)
	{
		ENetPacket * pp;

		pp2_entering_text = 2;
		pp2_entering_text_pos = 0;
		pp2_entered_text[0] = 0;
		t3f_clear_keys();
		pp = joynet_create_packet(PP2_GAME_MESSAGE_TYPING, NULL);
		enet_peer_send(pp2_client->peer, JOYNET_CHANNEL_GAME, pp);
		t3f_key[ALLEGRO_KEY_T] = 0;
	}

	switch(pp2_state)
	{
		case PP2_STATE_INTRO:
		{
			pp2_intro_logic();
			break;
		}
		case PP2_STATE_TITLE:
		{
			pp2_title_logic();
			break;
		}
		case PP2_STATE_T_TITLE_MENU:
		{
			pp2_t_title_menu_logic();
			break;
		}
		case PP2_STATE_MENU:
		{
			pp2_menu_logic();
			break;
		}
		case PP2_STATE_PLAYER_SETUP:
		{
			pp2_player_setup_logic();
			pp2_tick++;
			break;
		}
		case PP2_STATE_LEVEL_SETUP:
		{
			pp2_level_setup_logic();
			break;
		}
		case PP2_STATE_GAME:
		{
			pp2_game_logic();
			break;
		}
		case PP2_STATE_GAME_PAUSED:
		{
			pp2_game_paused_logic();
			break;
		}
		case PP2_STATE_REPLAY:
		{
			pp2_replay_logic();
			break;
		}
		case PP2_STATE_THEATER:
		{
			pp2_replay_logic();
			break;
		}
		case PP2_STATE_GAME_OVER:
		{
			pp2_game_over_logic();
			break;
		}
	}
	pp2_message_logic(pp2_messages);
}

void pp2_render(void * data)
{
	switch(pp2_state)
	{
		case PP2_STATE_INTRO:
		{
			pp2_intro_render();
			break;
		}
		case PP2_STATE_TITLE:
		{
			pp2_title_render();
			break;
		}
		case PP2_STATE_T_TITLE_MENU:
		{
			pp2_t_title_menu_render();
			break;
		}
		case PP2_STATE_MENU:
		{
			pp2_menu_render();
			break;
		}
		case PP2_STATE_PLAYER_SETUP:
		{
			pp2_player_setup_render();
			break;
		}
		case PP2_STATE_LEVEL_SETUP:
		{
			pp2_level_setup_render();
			break;
		}
		case PP2_STATE_GAME:
		{
			pp2_game_render();
			break;
		}
		case PP2_STATE_GAME_PAUSED:
		{
			pp2_game_paused_render();
			break;
		}
		case PP2_STATE_REPLAY:
		{
			pp2_replay_render();
			break;
		}
		case PP2_STATE_THEATER:
		{
			pp2_replay_render();
			break;
		}
		case PP2_STATE_GAME_OVER:
		{
			pp2_game_over_render();
			break;
		}
	}
	t3f_select_view(t3f_default_view);
	pp2_message_render(pp2_messages, 0.0, 0.0);
//	al_draw_textf(pp2_font[PP2_FONT_HUD], 0.0, 0.0, 0, "%d", pp2_messages->messages);
	if(pp2_entering_text == 2)
	{
		al_draw_textf(pp2_font[PP2_FONT_SMALL], al_map_rgba_f(0.0, 0.0, 0.0, 0.5), 0.0 + 1.0, 464.0 + 1.0, 0, "%s", pp2_entered_text);
		al_draw_textf(pp2_font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), 0.0, 464.0, 0, "%s", pp2_entered_text);
	}
//	printf("players: %d\n", pp2_client_game->player_count);
}

bool pp2_initialize(int argc, char * argv[])
{
	int i;

	if(!t3f_initialize("Paintball Party 2", PP2_SCREEN_WIDTH, PP2_SCREEN_HEIGHT, 60.0, pp2_logic, pp2_render, T3F_DEFAULT | T3F_USE_MOUSE | T3F_USE_JOYSTICK | T3F_FORCE_ASPECT, NULL))
	{
		return false;
	}

	t3f_load_resource((void **)&pp2_bitmap[PP2_BITMAP_LOADING], T3F_RESOURCE_TYPE_BITMAP, "data/graphics/loading.png", 0, 0, 0);
	if(!pp2_bitmap[PP2_BITMAP_LOADING])
	{
		return false;
	}
	t3f_load_resource((void **)&pp2_font[PP2_FONT_SMALL], T3F_RESOURCE_TYPE_BITMAP_FONT, "data/fonts/chared_font.png", 0, 0, 0);
	if(!pp2_font[PP2_FONT_SMALL])
	{
		return false;
	}
	pp2_set_database_callback(pp2_database_callback);

	/* create user directory structure */
	pp2_setup_directories();

	pp2_show_load_screen("Creating controllers");
	for(i = 0; i < PP2_MAX_PLAYERS; i++)
	{
		pp2_controller[i] = t3f_create_controller(9);
		if(!pp2_controller[i])
		{
			return false;
		}
	}
	if(!pp2_load_config(t3f_get_filename(t3f_config_path, "pp2.ini")))
	{
		pp2_autodetect_controllers();
	}
	pp2_show_load_screen("Controllers configured.");
	if(!pp2_load_profiles(&pp2_profiles, t3f_get_filename(t3f_data_path, "pp2.profiles")))
	{
		pp2_clear_profiles(&pp2_profiles);
		pp2_add_profile(&pp2_profiles, "Guest");
	}
	pp2_show_load_screen("Profiles loaded.");

	pp2_show_load_screen("Loading images");
	if(!pp2_load_images())
	{
		return false;
	}
	pp2_show_load_screen("Loading fonts");
	t3f_load_resource((void **)&pp2_font[PP2_FONT_COMIC_16], T3F_RESOURCE_TYPE_BITMAP_FONT, "data/fonts/comic_16.pcx", 1, 0, 0);
	if(!pp2_font[PP2_FONT_COMIC_16])
	{
		return false;
	}
	t3f_load_resource((void **)&pp2_font[PP2_FONT_HUD], T3F_RESOURCE_TYPE_BITMAP_FONT, "data/fonts/hud.pcx", 1, 0, 0);
	if(!pp2_font[PP2_FONT_HUD])
	{
		return false;
	}
	t3f_load_resource((void **)&pp2_font[PP2_FONT_COMIC_10], T3F_RESOURCE_TYPE_BITMAP_FONT, "data/fonts/comic_10.pcx", 1, 0, 0);
	if(!pp2_font[PP2_FONT_COMIC_10])
	{
		return false;
	}
	t3f_load_resource((void **)&pp2_font[PP2_FONT_COMIC_12], T3F_RESOURCE_TYPE_BITMAP_FONT, "data/fonts/comic_12.pcx", 1, 0, 0);
	if(!pp2_font[PP2_FONT_COMIC_12])
	{
		return false;
	}
	t3f_load_resource((void **)&pp2_font[PP2_FONT_COMIC_14], T3F_RESOURCE_TYPE_BITMAP_FONT, "data/fonts/comic_14.pcx", 1, 0, 0);
	if(!pp2_font[PP2_FONT_COMIC_14])
	{
		return false;
	}

	if(t3f_flags & T3F_USE_SOUND)
	{
		pp2_show_load_screen("Loading sounds");
		if(!pp2_load_sounds())
		{
			return false;
		}
	}

	pp2_show_load_screen("Loading animations");
	if(!pp2_load_animations())
	{
		return false;
	}

	pp2_show_load_screen("Creating level database");
	if(!pp2_build_level_database())
	{
		printf("Error building level database!\n");
		return false;
	}

	pp2_show_load_screen("Creating character database");
	if(!pp2_build_character_database())
	{
		printf("Error building character database!\n");
		return false;
	}

	pp2_show_load_screen("Creating music database");
	if(!pp2_build_music_database())
	{
		printf("Error building music database!\n");
		return false;
	}

	pp2_show_load_screen("Creating demo database");
	if(!pp2_build_demo_database())
	{
		printf("Error building demo database!\n");
		return false;
	}

	if(!pp2_setup_joynet())
	{
		printf("Error setting up JoyNet!\n");
		return false;
	}

	pp2_messages = pp2_create_message_list();
	if(!pp2_messages)
	{
		return false;
	}

	pp2_show_load_screen("Setting up menus");
	pp2_menu_initialize();
	for(i = 0; i < PP2_MAX_PLAYERS; i++)
	{
		pp2_player[i].character_choice = 0;
		pp2_player[i].character_choosing = 0;
	}
	pp2_title_build_credits(&pp2_credits);
	pp2_play_music("data/music/theme.ogg");
	pp2_state = PP2_STATE_TITLE;
	pp2_tick = 0;
	srand(time(0));

	return true;
}

void pp2_exit(void)
{
	int i;

	pp2_save_config(t3f_get_filename(t3f_config_path, "pp2.ini"));
	pp2_save_profiles(&pp2_profiles, t3f_get_filename(t3f_data_path, "pp2.profiles"));
	pp2_destroy_database(pp2_level_database);
	pp2_destroy_database(pp2_character_database);
	pp2_destroy_database(pp2_music_database);
	pp2_stop_music();
	if(pp2_server_thread)
	{
		al_join_thread(pp2_server_thread, NULL);
		al_destroy_thread(pp2_server_thread);
	}
	if(pp2_client)
	{
		joynet_disconnect_client_from_server(pp2_client);
		joynet_destroy_client(pp2_client);
	}
	joynet_destroy_game(pp2_client_game);

	for(i = 0; i < PP2_MAX_BITMAPS; i++)
	{
		if(pp2_bitmap[i])
		{
			t3f_destroy_resource(pp2_bitmap[i]);
		}
	}
	for(i = 0; i < PP2_MAX_OBJECT_TYPES; i++)
	{
		if(pp2_object_animation[i])
		{
			t3f_destroy_animation(pp2_object_animation[i]);
		}
	}
}

int main(int argc, char * argv[])
{
	if(!pp2_initialize(argc, argv))
	{
		printf("Initialization failed!\n");
		return -1;
	}
	t3f_run();
	pp2_exit();
	return 0;
}
