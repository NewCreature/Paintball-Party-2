#include <allegro5/allegro.h>
#include "../joynet/joynet.h"
#include "../joynet/game.h"
#include "../interface/message.h"
#include "../interface/player_setup.h"
#include "../pp2.h"

ALLEGRO_THREAD * pp2_client_keep_alive_thread = NULL;

int pp2_client_callback(ENetEvent * ep, void * data)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;
	char message[1024] = {0};

	switch(ep->type)
	{
		case ENET_EVENT_TYPE_DISCONNECT:
		{
			instance->ui.client_disconnected = true;
			instance->game.client_game->client = NULL;
			instance->game.client_game->state = JOYNET_GAME_STATE_OFF;
			instance->ui.current_menu = PP2_MENU_MAIN;
			instance->ui.menu_stack_size = 0;
			al_show_mouse_cursor(t3f_display);
			instance->state = PP2_STATE_MENU;
			pp2_clear_messages(instance->ui.messages);
			sprintf(message, "Connection lost.");
			pp2_add_message(instance->ui.messages, message, instance->resources.font[PP2_FONT_SMALL], al_map_rgba_f(0.0, 1.0, 0.0, 1.0), 300, PP2_SCREEN_VISIBLE_WIDTH, 0.0);
			joynet_reset_game(instance->game.client_game);
			pp2_player_setup_reset(&instance->game);
			break;
		}
		default:
		{
			break;
		}
	}
	return 1;
}

void * pp2_client_keep_alive_thread_proc(ALLEGRO_THREAD * thread, void * arg)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)arg;
	while(1)
	{
		al_rest(0.0166);
		joynet_poll_client(instance->client);
		if(al_get_thread_should_stop(thread))
		{
			break;
		}
	}
	return NULL;
}

void pp2_spawn_client_keep_alive_thread(PP2_INSTANCE * instance)
{
	if(instance->client)
	{
		pp2_client_keep_alive_thread = al_create_thread(pp2_client_keep_alive_thread_proc, instance);
		if(!pp2_client_keep_alive_thread)
		{
			return;
		}
		al_start_thread(pp2_client_keep_alive_thread);
	}
}

void pp2_kill_client_keep_alive_thread(void)
{
	if(pp2_client_keep_alive_thread)
	{
		al_join_thread(pp2_client_keep_alive_thread, NULL);
	}
}
