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
#include "legacy/animation.h"
#include "pp2.h"
#include "gameplay/network.h"

static void pp2_event_handler(ALLEGRO_EVENT * event, void * data)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	switch(event->type)
	{
		case ALLEGRO_EVENT_DISPLAY_RESIZE:
		{
			t3f_event_handler(event);
			pp2_adjust_menus(&instance->interface);
			break;
		}
		default:
		{
			t3f_event_handler(event);
		}
	}
}

void pp2_logic(void * data)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

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
		else if(instance->state == PP2_STATE_GAME || instance->state == PP2_STATE_GAME_OVER)
		{
			joynet_pause_game(pp2_client_game);
		}
		else if(instance->state == PP2_STATE_REPLAY)
		{
			if((pp2_replay_flags & PP2_REPLAY_FLAG_DEMO) || (pp2_replay_flags & PP2_REPLAY_FLAG_THEATER))
			{
				pp2_replay_done = true;
			}
			else
			{
				pp2_finish_replay(&instance->game, &instance->resources);
				instance->interface.current_menu = PP2_MENU_MAIN;
				instance->interface.menu_stack_size = 0;
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
				instance->state = PP2_STATE_MENU;
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

	switch(instance->state)
	{
		case PP2_STATE_INTRO:
		{
			pp2_intro_logic(instance);
			break;
		}
		case PP2_STATE_TITLE:
		{
			if(!pp2_title_logic(&instance->interface, &instance->game, &instance->resources))
			{
				instance->state = PP2_STATE_T_TITLE_MENU;
			}
			break;
		}
		case PP2_STATE_T_TITLE_MENU:
		{
			if(!pp2_t_title_menu_logic(instance))
			{
				instance->state = PP2_STATE_MENU;
			}
			break;
		}
		case PP2_STATE_MENU:
		{
			pp2_menu_logic(instance);
			break;
		}
		case PP2_STATE_PLAYER_SETUP:
		{
			pp2_player_setup_logic(&instance->interface, &instance->game, instance);
			instance->interface.tick++;
			break;
		}
		case PP2_STATE_LEVEL_SETUP:
		{
			pp2_level_setup_logic(instance);
			break;
		}
		case PP2_STATE_GAME:
		{
			pp2_game_logic(&instance->game, &instance->interface, &instance->resources);
			if(instance->game.next_state)
			{
				instance->state = instance->game.next_state;
			}
			break;
		}
		case PP2_STATE_GAME_PAUSED:
		{
			pp2_game_paused_logic(instance);
			break;
		}
		case PP2_STATE_REPLAY:
		{
			pp2_replay_logic(&instance->game, &instance->interface, &instance->resources);
			if(instance->game.next_state)
			{
				instance->state = instance->game.next_state;
			}
			break;
		}
		case PP2_STATE_THEATER:
		{
			pp2_replay_logic(&instance->game, &instance->interface, &instance->resources);
			if(instance->game.next_state)
			{
				instance->state = instance->game.next_state;
			}
			break;
		}
		case PP2_STATE_GAME_OVER:
		{
			pp2_game_over_logic(&instance->game, &instance->interface, &instance->resources);
			break;
		}
	}
	pp2_message_logic(pp2_messages);
}

void pp2_render(void * data)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	switch(instance->state)
	{
		case PP2_STATE_INTRO:
		{
			pp2_intro_render(&instance->resources);
			break;
		}
		case PP2_STATE_TITLE:
		{
			pp2_title_render(&instance->interface, &instance->resources);
			break;
		}
		case PP2_STATE_T_TITLE_MENU:
		{
			pp2_t_title_menu_render(&instance->interface, &instance->resources);
			break;
		}
		case PP2_STATE_MENU:
		{
			pp2_menu_render(&instance->interface, &instance->resources);
			break;
		}
		case PP2_STATE_PLAYER_SETUP:
		{
			pp2_player_setup_render(&instance->interface, &instance->game, &instance->resources);
			break;
		}
		case PP2_STATE_LEVEL_SETUP:
		{
			pp2_level_setup_render(&instance->interface, &instance->resources);
			break;
		}
		case PP2_STATE_GAME:
		{
			pp2_game_render(&instance->game, &instance->resources);
			break;
		}
		case PP2_STATE_GAME_PAUSED:
		{
			pp2_game_paused_render(&instance->interface, &instance->resources);
			break;
		}
		case PP2_STATE_REPLAY:
		{
			pp2_replay_render(&instance->game, &instance->resources);
			break;
		}
		case PP2_STATE_THEATER:
		{
			pp2_replay_render(&instance->game, &instance->resources);
			break;
		}
		case PP2_STATE_GAME_OVER:
		{
			pp2_game_over_render(&instance->game, &instance->resources);
			break;
		}
	}
	t3f_select_view(t3f_default_view);
	pp2_message_render(pp2_messages, t3f_default_view->left, t3f_default_view->top);
//	al_draw_textf(resources->font[PP2_FONT_HUD], 0.0, 0.0, 0, "%d", pp2_messages->messages);
	if(pp2_entering_text == 2)
	{
		al_draw_textf(instance->resources.font[PP2_FONT_SMALL], al_map_rgba_f(0.0, 0.0, 0.0, 0.5), 0.0 + 1.0, 464.0 + 1.0, 0, "%s", pp2_entered_text);
		al_draw_textf(instance->resources.font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), 0.0, 464.0, 0, "%s", pp2_entered_text);
	}
//	printf("players: %d\n", pp2_client_game->player_count);
}

bool pp2_initialize(PP2_INSTANCE * instance, int argc, char * argv[])
{
	char buf[1024];
	const char * val;
	int i;

	if(!t3f_initialize("Paintball Party 2", PP2_SCREEN_WIDTH, PP2_SCREEN_HEIGHT, 60.0, pp2_logic, pp2_render, T3F_DEFAULT | T3F_USE_MOUSE | T3F_USE_JOYSTICK | T3F_FORCE_ASPECT | T3F_FILL_SCREEN, instance))
	{
		return false;
	}

	for(i = 1; i < argc; i++)
	{
		if(!strcmp(argv[i], "--enable-capture"))
		{
			instance->interface.use_ffmpeg = true;
		}
	}

	t3f_set_event_handler(pp2_event_handler);

	val = al_get_config_value(t3f_config, "Game Config", "theme_file");
	if(!val)
	{
		val = "data/themes/default.ini";
	}
	instance->theme = pp2_load_theme(val);
	if(!instance->theme)
	{
		printf("Unable to load theme!\n");
		return false;
	}

	if(instance->theme->bitmap_load_fn)
	{
		t3f_load_resource((void **)&instance->resources.bitmap[PP2_BITMAP_LOADING], T3F_RESOURCE_TYPE_BITMAP, instance->theme->bitmap_load_fn, 0, 0, 0);
		if(!instance->resources.bitmap[PP2_BITMAP_LOADING])
		{
			return false;
		}
	}
	if(instance->theme->font_load_info_fn)
	{
		t3f_load_resource((void **)&instance->resources.font[PP2_FONT_SMALL], T3F_RESOURCE_TYPE_BITMAP_FONT, instance->theme->font_load_info_fn, 0, 0, 0);
		if(!instance->resources.font[PP2_FONT_SMALL])
		{
			return false;
		}
	}
	pp2_set_database_callback(pp2_database_callback);
	pp2_register_legacy_character_bitmap_resource_loader();

	/* create user directory structure */
	pp2_setup_directories(&instance->resources);

	pp2_show_load_screen("Creating controllers", &instance->resources);
	for(i = 0; i < PP2_MAX_PLAYERS; i++)
	{
		instance->interface.controller[i] = t3f_create_controller(9);
		if(!instance->interface.controller[i])
		{
			return false;
		}
	}
	if(!pp2_load_config(&instance->interface, &instance->game, t3f_get_filename(t3f_config_path, "pp2.ini", buf, 1024)))
	{
		pp2_autodetect_controllers();
	}
	pp2_show_load_screen("Controllers configured.", &instance->resources);
	if(!pp2_load_profiles(&instance->interface.profiles, t3f_get_filename(t3f_data_path, "pp2.profiles", buf, 1024)))
	{
		pp2_clear_profiles(&instance->interface.profiles);
		pp2_add_profile(&instance->interface.profiles, "Guest");
	}
	pp2_show_load_screen("Profiles loaded.", &instance->resources);

	pp2_show_load_screen("Loading images", &instance->resources);
	if(!pp2_load_images(&instance->resources))
	{
		return false;
	}
	pp2_show_load_screen("Loading fonts", &instance->resources);
	t3f_load_resource((void **)&instance->resources.font[PP2_FONT_COMIC_16], T3F_RESOURCE_TYPE_BITMAP_FONT, "data/fonts/comic_16.pcx", 1, 0, 0);
	if(!instance->resources.font[PP2_FONT_COMIC_16])
	{
		return false;
	}
	t3f_load_resource((void **)&instance->resources.font[PP2_FONT_HUD], T3F_RESOURCE_TYPE_BITMAP_FONT, "data/fonts/hud.pcx", 1, 0, 0);
	if(!instance->resources.font[PP2_FONT_HUD])
	{
		return false;
	}
	t3f_load_resource((void **)&instance->resources.font[PP2_FONT_COMIC_10], T3F_RESOURCE_TYPE_BITMAP_FONT, "data/fonts/comic_10.pcx", 1, 0, 0);
	if(!instance->resources.font[PP2_FONT_COMIC_10])
	{
		return false;
	}
	t3f_load_resource((void **)&instance->resources.font[PP2_FONT_COMIC_12], T3F_RESOURCE_TYPE_BITMAP_FONT, "data/fonts/comic_12.pcx", 1, 0, 0);
	if(!instance->resources.font[PP2_FONT_COMIC_12])
	{
		return false;
	}
	t3f_load_resource((void **)&instance->resources.font[PP2_FONT_COMIC_14], T3F_RESOURCE_TYPE_BITMAP_FONT, "data/fonts/comic_14.pcx", 1, 0, 0);
	if(!instance->resources.font[PP2_FONT_COMIC_14])
	{
		return false;
	}

	if(t3f_flags & T3F_USE_SOUND)
	{
		pp2_show_load_screen("Loading sounds", &instance->resources);
		if(!pp2_load_sounds(&instance->resources))
		{
			return false;
		}
	}

	pp2_show_load_screen("Loading animations", &instance->resources);
	if(!pp2_load_animations(&instance->resources))
	{
		return false;
	}

	pp2_show_load_screen("Creating level database", &instance->resources);
	if(!pp2_build_level_database(&instance->interface, &instance->resources))
	{
		printf("Error building level database!\n");
		return false;
	}
	if(pp2_database_find_entry(instance->resources.level_database, instance->interface.level_hash) < 0)
	{
		instance->interface.level_hash = instance->resources.level_database->entry[0]->checksum;
	}

	pp2_show_load_screen("Creating character database", &instance->resources);
	if(!pp2_build_character_database(&instance->interface, &instance->resources))
	{
		printf("Error building character database!\n");
		return false;
	}

	pp2_show_load_screen("Creating music database", &instance->resources);
	if(!pp2_build_music_database(&instance->interface, &instance->resources))
	{
		printf("Error building music database!\n");
		return false;
	}

	pp2_show_load_screen("Creating demo database", &instance->resources);
	if(!pp2_build_demo_database(&instance->resources))
	{
		printf("Error building demo database!\n");
		return false;
	}

	if(!pp2_setup_joynet(instance))
	{
		printf("Error setting up JoyNet!\n");
		return false;
	}

	pp2_messages = pp2_create_message_list();
	if(!pp2_messages)
	{
		return false;
	}

	pp2_show_load_screen("Setting up menus", &instance->resources);
	pp2_menu_initialize(&instance->interface, &instance->resources);
	for(i = 0; i < PP2_MAX_PLAYERS; i++)
	{
		instance->game.player[i].character_choice = 0;
		instance->game.player[i].character_choosing = 0;
	}
	pp2_title_build_credits(&instance->interface.credits);
	if(instance->theme->theme_music_fn)
	{
		pp2_play_music(instance->theme->theme_music_fn);
	}
	instance->state = PP2_STATE_TITLE;
	instance->interface.tick = 0;
	srand(time(0));

	return true;
}

void pp2_exit(PP2_INSTANCE * instance)
{
	char buf[1024];
	int i;

	pp2_save_config(&instance->interface, &instance->game, t3f_get_filename(t3f_config_path, "pp2.ini", buf, 1024));
	pp2_save_profiles(&instance->interface.profiles, t3f_get_filename(t3f_data_path, "pp2.profiles", buf, 1024));
	pp2_destroy_database(instance->resources.level_database);
	pp2_destroy_database(instance->resources.character_database);
	pp2_destroy_database(instance->resources.music_database);
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
		if(instance->resources.bitmap[i])
		{
			t3f_destroy_resource(instance->resources.bitmap[i]);
		}
	}
	for(i = 0; i < PP2_MAX_OBJECT_TYPES; i++)
	{
		if(instance->resources.object_animation[i])
		{
			t3f_destroy_animation(instance->resources.object_animation[i]);
		}
	}
}

int main(int argc, char * argv[])
{
	PP2_INSTANCE * pp2_instance;

	pp2_instance = malloc(sizeof(PP2_INSTANCE));
	if(!pp2_instance)
	{
		return -1;
	}
	memset(pp2_instance, 0, sizeof(PP2_INSTANCE));
	if(!pp2_initialize(pp2_instance, argc, argv))
	{
		printf("Initialization failed!\n");
		return -1;
	}
	t3f_run();
	pp2_exit(pp2_instance);
	free(pp2_instance);
	t3f_finish();
	return 0;
}
