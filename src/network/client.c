#include <allegro5/allegro.h>
#include "../joynet/joynet.h"
#include "../joynet/game.h"
#include "../data.h"
#include "../interface/message.h"
#include "../interface/player_setup.h"

ALLEGRO_THREAD * pp2_client_keep_alive_thread = NULL;

int pp2_client_callback(ENetEvent * ep)
{
	char message[1024] = {0};

	switch(ep->type)
	{
		case ENET_EVENT_TYPE_DISCONNECT:
		{
			pp2_client_disconnected = true;
			pp2_client_game->client = NULL;
			pp2_client_game->state = JOYNET_GAME_STATE_OFF;
			pp2_current_menu = PP2_MENU_MAIN;
			pp2_menu_stack_size = 0;
			al_show_mouse_cursor(t3f_display);
			pp2_state = PP2_STATE_MENU;
			pp2_clear_messages(pp2_messages);
			sprintf(message, "Connection lost.");
			pp2_add_message(pp2_messages, message, pp2_font[PP2_FONT_SMALL], al_map_rgba_f(0.0, 1.0, 0.0, 1.0), 300, PP2_SCREEN_VISIBLE_WIDTH, 0.0);
			joynet_reset_game(pp2_client_game);
			pp2_player_setup_reset();
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
	while(1)
	{
		al_rest(0.0166);
		joynet_poll_client(pp2_client);
		if(al_get_thread_should_stop(thread))
		{
			break;
		}
	}
	return NULL;
}

void pp2_spawn_client_keep_alive_thread(void)
{
	if(pp2_client)
	{
		pp2_client_keep_alive_thread = al_create_thread(pp2_client_keep_alive_thread_proc, NULL);
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
