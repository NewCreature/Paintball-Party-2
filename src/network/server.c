#include <allegro5/allegro.h>
#include "../joynet/joynet.h"
#include "../joynet/game.h"
#include "../joynet/serialize.h"
#include "../data.h"
#include "../version.h"
#include "../interface/message.h"
#include "../pp2.h"

ALLEGRO_EVENT_SOURCE pp2_server_poll_event_source;
char * pp2_server_key = NULL;
char pp2_server_capacity[64] = {0};
ALLEGRO_THREAD * pp2_server_poll_thread = NULL;
unsigned long pp2_server_ticker = 0;
static JOYNET_GAME * pp2_server_game = NULL;
static JOYNET_SERVER * pp2_server = NULL;

int pp2_server_callback(ENetEvent * ep, void * data)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;
	char message[1024] = {0};

	switch(ep->type)
	{
		case ENET_EVENT_TYPE_DISCONNECT:
		{
			int client;
			if(joynet_current_server_game)
			{
				client = joynet_get_client_from_peer(joynet_current_server_game->server, ep->peer);
				sprintf(message, "Client %d disconnected.", client);
				pp2_add_message(instance->interface.messages, message, instance->resources.font[PP2_FONT_SMALL], al_map_rgba_f(0.0, 1.0, 0.0, 1.0), 300, PP2_SCREEN_VISIBLE_WIDTH, 0.0);
				pp2_server_ticker = 0;
			}
			break;
		}
		case ENET_EVENT_TYPE_CONNECT:
		{
			int client;
			if(joynet_current_server_game)
			{
				client = joynet_get_client_from_peer(joynet_current_server_game->server, ep->peer);
				sprintf(message, "Client %d connected.", client);
				pp2_add_message(instance->interface.messages, message, instance->resources.font[PP2_FONT_SMALL], al_map_rgba_f(0.0, 1.0, 0.0, 1.0), 300, PP2_SCREEN_VISIBLE_WIDTH, 0.0);
			}
			break;
		}
		default:
		{
			break;
		}
	}
	return 1;
}

int pp2_server_game_channel_callback(JOYNET_MESSAGE * mp, void * data)
{
	int i;

	switch(mp->type)
	{
		case JOYNET_GAME_MESSAGE_CONNECT:
		case JOYNET_GAME_MESSAGE_DISCONNECT:
		{
			pp2_server_ticker = 0;
			break;
		}
		case PP2_GAME_MESSAGE_TYPING:
		case PP2_GAME_MESSAGE_TYPING_DONE:
		{
			ENetPacket * pp;
			int client = joynet_get_client_from_peer(pp2_server, mp->event->peer);
			char player[PP2_MAX_PLAYERS] = {0};
			char players = 0;
			char data[PP2_MAX_PLAYERS + 1] = {0};

			for(i = 0; i < pp2_server_game->players; i++)
			{
				if(pp2_server_game->player[i]->playing && pp2_server_game->player[i]->client == client)
				{
					player[(int)players] = i;
					players++;
				}
			}
			joynet_serialize(pp2_server->serial_data, data);
			joynet_putc(joynet_current_server_game->server->serial_data, players);
			joynet_write(joynet_current_server_game->server->serial_data, player, players);
			for(i = 0; i < pp2_server->max_clients; i++)
			{
				if(pp2_server->client[i]->peer && (pp2_server->client[i]->playing || pp2_server->client[i]->spectating))
				{
					pp = joynet_create_packet(mp->type, joynet_current_server_game->server->serial_data);
					enet_peer_send(pp2_server->client[i]->peer, JOYNET_CHANNEL_GAME, pp);
				}
			}
			break;
		}
	}
	return 1;
}

void * pp2_server_poll_thread_proc(ALLEGRO_THREAD * thread, void * arg)
{
	ALLEGRO_EVENT_QUEUE * queue = NULL;

	queue = al_create_event_queue();
	if(!queue)
	{
		return NULL;
	}
	al_register_event_source(queue, &pp2_server_poll_event_source);
	while(1)
	{
		ALLEGRO_EVENT event;
		if(al_get_thread_should_stop(thread))
		{
			break;
		}
		else
		{
			al_wait_for_event(queue, &event);
			if(event.type == 1024)
			{
				t3net_update_server("www.t3-i.com/t3net2/master/poll.php", 5566,  pp2_server_key, pp2_server_capacity);
			}
		}
	}
	return NULL;
}

/* run server in its own thread so we don't have to worry about it */
void * pp2_server_thread_proc(ALLEGRO_THREAD * thread, void * arg)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)arg;
	ALLEGRO_TIMER * timer = NULL;
	ALLEGRO_EVENT_QUEUE * queue = NULL;
	char message[1024] = {0};
	bool no_poll = false;
	ALLEGRO_EVENT poll_event;

	/* create server data */
	pp2_server = joynet_create_server();
	if(!pp2_server)
	{
		return NULL;
	}
	joynet_set_server_global_callback(pp2_server, pp2_server_callback, instance);
	joynet_set_server_channel_callback(pp2_server, JOYNET_CHANNEL_GAME, pp2_server_game_channel_callback, instance);
	pp2_server_game = joynet_create_game(PP2_VERSION_STRING, JOYNET_GAME_TYPE_CONTROLLERS, 16, 4, NULL, NULL);
	if(!pp2_server_game)
	{
		joynet_destroy_server(pp2_server);
		pp2_server = NULL;
		return NULL;
	}
	joynet_setup_game_controllers(pp2_server_game, 8, 0, 128);
	if(!joynet_open_game_server(pp2_server_game, pp2_server, 5566, 32))
	{
		joynet_destroy_server(pp2_server);
		pp2_server = NULL;
		joynet_destroy_game(pp2_server_game);
		pp2_server_game = NULL;
		return NULL;
	}
	if(instance->lan_arg == 0)
	{
		pp2_server_key = t3net_register_server("www.t3-i.com/t3net2/master/poll.php", 5566, "PP2", PP2_VERSION_NETWORK, pp2_server_name, NULL, false);
		if(!pp2_server_key)
		{
			no_poll = true;
			pp2_add_message(instance->interface.messages, "Server is not public!", instance->resources.font[PP2_FONT_SMALL], al_map_rgba_f(0.0, 1.0, 0.0, 1.0), 300, PP2_SCREEN_VISIBLE_WIDTH, 0.0);
		}
	}
	else
	{
		no_poll = true;
	}
	if(!no_poll)
	{
		pp2_server_poll_thread = al_create_thread(pp2_server_poll_thread_proc, instance);
		if(!pp2_server_poll_thread)
		{
			return NULL;
		}
		al_init_user_event_source(&pp2_server_poll_event_source);
		al_start_thread(pp2_server_poll_thread);
	}

	/* set up timer and event queue */
	timer = al_create_timer(1.0 / 60.0);
	if(!timer)
	{
		return NULL;
	}
	queue = al_create_event_queue();
	if(!queue)
	{
		return NULL;
	}
	sprintf(message, "Server is now open.");
	pp2_add_message(instance->interface.messages, message, instance->resources.font[PP2_FONT_SMALL], al_map_rgba_f(0.0, 1.0, 0.0, 1.0), 300, PP2_SCREEN_VISIBLE_WIDTH, 0.0);
	al_register_event_source(queue, al_get_timer_event_source(timer));
	pp2_server_ticker = 0; // set to 0 so we update the capacity column immediately
	al_start_timer(timer);
	while(1)
	{
		ALLEGRO_EVENT event;
		al_wait_for_event(queue, &event);
		if(al_get_thread_should_stop(thread))
		{
			break;
		}
		else // only do this if we are not stopping the thread so we don't update closed servers
		{
			if(event.type == ALLEGRO_EVENT_TIMER)
			{
				if(pp2_server)
				{
					joynet_poll_server(pp2_server);
				}
				if(pp2_server_game)
				{
					joynet_update_game_server(pp2_server, pp2_server_game); // call once per tick
				}
				/* keep server listed on t3-i.com */
				if(!no_poll)
				{
					if(pp2_server_ticker % 3600 == 0)
					{
						sprintf(pp2_server_capacity, "%d/%d", pp2_server_game->player_count, pp2_server_game->players);
						poll_event.any.type = 1024;
						al_emit_user_event(&pp2_server_poll_event_source, &poll_event, NULL);
					}
				}
				pp2_server_ticker++;
			}
		}
	}
	if(!no_poll)
	{
		poll_event.any.type = 1025;
		al_emit_user_event(&pp2_server_poll_event_source, &poll_event, NULL);
		t3net_unregister_server("www.t3-i.com/t3net2/master/poll.php", 5566,  pp2_server_key);
	}
	al_destroy_event_queue(queue);
	al_destroy_timer(timer);
	joynet_close_server(pp2_server);
	joynet_destroy_server(pp2_server);
	pp2_server = NULL;
	joynet_destroy_game(pp2_server_game);
	pp2_server_game = NULL;
	sprintf(message, "Server closed.");
	pp2_add_message(instance->interface.messages, message, instance->resources.font[PP2_FONT_SMALL], al_map_rgba_f(0.0, 1.0, 0.0, 1.0), 300, PP2_SCREEN_VISIBLE_WIDTH, 0.0);
	return NULL;
}
