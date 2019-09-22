#ifdef WANT_PDCURSES
	#include <pdcurses.h>
#else
	#include <curses.h>
#endif
#include "t3f/t3f.h"
#include "joynet/joynet.h"
#include "joynet/game.h"
#include "joynet/serialize.h"
#include "t3net/t3net.h"
#include "t3net/server_list.h"
#include "defines.h"
#include "version.h"

#define MAX_SERVER_LOGS 13

char * game_state_name[3] =
{
	"Waiting",
	"In Progress",
	"Paused"
};

char server_log[MAX_SERVER_LOGS][256] = {{0}};
int server_logs = 0;
char message[1024] = {0};

JOYNET_SERVER * server;
JOYNET_GAME * game;

/* polling */
ALLEGRO_EVENT_SOURCE server_poll_event_source;
ALLEGRO_THREAD * server_poll_thread = NULL;
char * server_key = NULL;
char server_capacity[64] = {0};
unsigned long ticker = 0;
bool no_poll = false;

int clients;
int players;
int played = 0;
int finished = 0;

/* display stuff */
WINDOW * screen;

void server_add_log(char * text)
{
	int i;

	if(server_logs < MAX_SERVER_LOGS)
	{
		strcpy(server_log[server_logs], text);
		server_logs++;
	}
	else
	{
		for(i = 0; i < MAX_SERVER_LOGS - 1; i++)
		{
			strcpy(server_log[i], server_log[i + 1]);
		}
		strcpy(server_log[MAX_SERVER_LOGS - 1], text);
	}
}

void update_console(void)
{
	int i;

	clear();
	curs_set(0);
	mvwprintw(screen, 0, 0, "Server Status: Online");
	mvwprintw(screen, 1, 0, "Game Status:   %s", game_state_name[game->state]);
	mvwprintw(screen, 2, 0, "Clients:       %02d/%02d", clients, server->max_clients);
	mvwprintw(screen, 3, 0, "Players:       %02d/%02d", game->player_count, game->players);
	mvwprintw(screen, 4, 0, "Games Played:  %d", played);

	mvwprintw(screen, 7, 0, "Server Log");
	mvwprintw(screen, 8, 0, "----------");
	for(i = 0; i < server_logs; i++)
	{
		mvwprintw(screen, 9 + i, 0, "%s", server_log[i]);
	}

	mvwprintw(screen, 23, 0, "Press 'Q' to quit.");
	wrefresh(screen);
	refresh();
}

int server_callback(ENetEvent * ep)
{
	int i;

	switch(ep->type)
	{
		case ENET_EVENT_TYPE_DISCONNECT:
		{
			int client;
			clients = 0;
			for(i = 0; i < server->max_clients; i++)
			{
				if(server->client[i]->peer)
				{
					clients++;
				}
			}
			if(game)
			{
				client = joynet_get_client_from_peer(server, ep->peer);
				sprintf(message, "Client %d disconnected.", client);
				server_add_log(message);
				update_console();
			}
			break;
		}
		case ENET_EVENT_TYPE_CONNECT:
		{
			int client;
			clients = 0;
			for(i = 0; i < server->max_clients; i++)
			{
				if(server->client[i]->peer)
				{
					clients++;
				}
			}
			if(game)
			{
				client = joynet_get_client_from_peer(server, ep->peer);
				sprintf(message, "Client %d connected.", client);
				server_add_log(message);
				update_console();
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

int game_channel_callback(JOYNET_MESSAGE * mp)
{
	switch(mp->type)
	{
		case JOYNET_GAME_MESSAGE_CHECK_ID:
		{
			int client = joynet_get_client_from_peer(server, mp->event->peer);
			short length;
			char id[128];

			joynet_serialize(server->serial_data, mp->data);
			joynet_getw(server->serial_data, &length);
			joynet_read(server->serial_data, id, length);

			sprintf(message, "Client %d ID: %s", client, id);
			server_add_log(message);
			update_console();
			break;
		}
		case JOYNET_GAME_MESSAGE_CONNECT:
		{
			int client = joynet_get_client_from_peer(server, mp->event->peer);
			short player;
			short controller;

			joynet_serialize(server->serial_data, mp->data);
			joynet_getw(server->serial_data, &controller);
			joynet_getw(server->serial_data, &player);

			sprintf(message, "Client %d connecting controller %d to slot %d.", client, controller, player);
			server_add_log(message);
			update_console();
			break;
		}
		case JOYNET_GAME_MESSAGE_DISCONNECT:
		{
			int client = joynet_get_client_from_peer(server, mp->event->peer);
			short player;
			short controller;

			/* disconnect the controller */
			joynet_serialize(server->serial_data, mp->data);
			joynet_getw(server->serial_data, &controller);
			joynet_getw(server->serial_data, &player);

			sprintf(message, "Client %d disconnecting controller %d.", client, controller);
			server_add_log(message);
			update_console();
			break;
		}
		case JOYNET_GAME_MESSAGE_ADD_PLAYER:
		{
			break;
		}
		case JOYNET_GAME_MESSAGE_UPDATE_PLAYER_OPTIONS:
		{
			short player;

			/* store options */
			joynet_serialize(server->serial_data, mp->data);
			joynet_getw(server->serial_data, &player);
			sprintf(message, "Player %d updating options.", player);
			server_add_log(message);
			update_console();
			break;
		}
		case JOYNET_GAME_MESSAGE_UPDATE_OPTIONS:
		{
			sprintf(message, "Updating game options.");
			server_add_log(message);
			update_console();
			break;
		}
		case JOYNET_GAME_MESSAGE_SPECTATE:
		{
			int client = joynet_get_client_from_peer(server, mp->event->peer);

			sprintf(message, "Client %d spectating.", client);
			server_add_log(message);
			update_console();
			break;
		}
		case JOYNET_GAME_MESSAGE_LEAVE:
		{
			int client = joynet_get_client_from_peer(server, mp->event->peer);
			sprintf(message, "Client %d leaving game.", client);
			server_add_log(message);
			update_console();
			break;
		}
		case JOYNET_GAME_MESSAGE_UPLOAD_PLAYER_CONTENT:
		{
			short player;
			short list;
			joynet_serialize(server->serial_data, mp->data);
			joynet_getw(server->serial_data, &player);
			joynet_getw(server->serial_data, &list);
			sprintf(message, "Player %d uploading content list %d.", player, list);
			server_add_log(message);
			update_console();
			break;
		}
		case JOYNET_GAME_MESSAGE_SELECT_PLAYER_CONTENT:
		{
			short player;
			short list;
			unsigned long hash;

			joynet_serialize(server->serial_data, mp->data);
			joynet_getw(server->serial_data, &player);
			joynet_getw(server->serial_data, &list);
			joynet_getl(server->serial_data, (int32_t *)(&hash));

			sprintf(message, "Player %d selecting list %d content.", player, list);
			server_add_log(message);
			update_console();
			break;
		}
		case JOYNET_GAME_MESSAGE_START:
		{
			played++;
			sprintf(message, "Starting game.");
			server_add_log(message);
			update_console();
			break;
		}
		case JOYNET_GAME_MESSAGE_PAUSE:
		{
			sprintf(message, "Pausing game.");
			server_add_log(message);
			update_console();
			break;
		}
		case JOYNET_GAME_MESSAGE_RESUME:
		{
			sprintf(message, "Resuming game.");
			server_add_log(message);
			update_console();
			break;
		}
		case JOYNET_GAME_MESSAGE_END:
		{
			sprintf(message, "Ending game.");
			server_add_log(message);
			update_console();
			break;
		}
		case PP2_GAME_MESSAGE_TYPING:
		case PP2_GAME_MESSAGE_TYPING_DONE:
		{
			ENetPacket * pp;
			int client = joynet_get_client_from_peer(server, mp->event->peer);
			char player[PP2_MAX_PLAYERS] = {0};
			char players = 0;
			char data[PP2_MAX_PLAYERS + 1] = {0};
			int i;

			for(i = 0; i < game->players; i++)
			{
				if(game->player_controller[i] && game->player[i]->client == client)
				{
					player[(int)players] = i;
					players++;
				}
			}
			joynet_serialize(server->serial_data, data);
			joynet_putc(joynet_current_server_game->server->serial_data, players);
			joynet_write(joynet_current_server_game->server->serial_data, player, players);
			for(i = 0; i < server->max_clients; i++)
			{
				if(server->client[i]->peer && (server->client[i]->playing || server->client[i]->spectating))
				{
					pp = joynet_create_packet(mp->type, joynet_current_server_game->server->serial_data);
					enet_peer_send(server->client[i]->peer, JOYNET_CHANNEL_GAME, pp);
				}
			}
			break;
		}
	}
	return 0;
}

int chat_channel_callback(JOYNET_MESSAGE * mp)
{
	switch(mp->type)
	{
		case JOYNET_CHAT_MESSAGE_SET_NAME:
		{
			int client = joynet_get_client_from_peer(server, mp->event->peer);

			sprintf(message, "Client %d set screen name to %s.", client, mp->data);
			server_add_log(message);
			update_console();
			break;
		}
		case JOYNET_CHAT_MESSAGE_SET_GROUP:
		{
			int client = joynet_get_client_from_peer(server, mp->event->peer);
			short group;

			joynet_serialize(server->serial_data, mp->data);
			joynet_getw(server->serial_data, &group);

			sprintf(message, "Client %d joined group %d.", client, group);
			server_add_log(message);
			update_console();
			break;
		}
		case JOYNET_CHAT_MESSAGE_TALK:
		{
			int client = joynet_get_client_from_peer(server, mp->event->peer);

			sprintf(message, "Client %d (%s) announced, \"%s.\"", client, server->client[client]->screen_name, mp->data);
			server_add_log(message);
			update_console();
			break;
		}
		case JOYNET_CHAT_MESSAGE_GROUP_TALK:
		{
			int client = joynet_get_client_from_peer(server, mp->event->peer);

			sprintf(message, "Client %d (%s) said, \"%s.\"", client, server->client[client]->screen_name, mp->data);
			server_add_log(message);
			update_console();
			break;
		}
	}
	return 0;
}

void * server_poll_thread_proc(ALLEGRO_THREAD * thread, void * arg)
{
	ALLEGRO_EVENT_QUEUE * queue = NULL;

	queue = al_create_event_queue();
	if(!queue)
	{
		return NULL;
	}
	al_register_event_source(queue, &server_poll_event_source);
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
				t3net_update_server("www.t3-i.com/t3net2/master/poll.php", 5566, server_key, server_capacity);
			}
		}
	}
	return NULL;
}

void logic(void * data)
{
	ALLEGRO_EVENT poll_event;
	int key = getch();

	if(key == 'q' || key == 'Q')
	{
		t3f_exit();
	}
	joynet_poll_server(server);
	joynet_update_game_server(server, game); // call once per tick

	/* keep server listed on t3-i.com */
	if(!no_poll)
	{
		if(ticker % 3600 == 0)
		{
			sprintf(server_capacity, "%d/%d", game->player_count, game->players);
			poll_event.any.type = 1024;
			al_emit_user_event(&server_poll_event_source, &poll_event, NULL);
		}
	}
	ticker++;
}

int main(int argc, char * argv[])
{
	if(argc < 2)
	{
		printf("Usage: pp2_server <name of server>\n");
		return 1;
	}
	if(!t3f_initialize("Paintball Party 2 Server", 640, 480, 60.0, logic, NULL, T3F_USE_KEYBOARD | T3F_NO_DISPLAY, NULL))
	{
		return 0;
	}
	if(!joynet_init())
	{
		return 1;
	}
	server = joynet_create_server();
	if(!server)
	{
		return 1;
	}
	joynet_set_server_global_callback(server, server_callback);
	joynet_set_server_channel_callback(server, JOYNET_CHANNEL_GAME, game_channel_callback);
	joynet_set_server_channel_callback(server, JOYNET_CHANNEL_CHAT, chat_channel_callback);
	game = joynet_create_game(PP2_VERSION_STRING, JOYNET_GAME_TYPE_CONTROLLERS, 16, 4, NULL);
	if(!game)
	{
		return 1;
	}
	joynet_setup_game_controllers(game, 8, 0, 128);
	if(!joynet_open_game_server(game, server, 5566, 32))
	{
		return 1;
	}
	server_key = t3net_register_server("www.t3-i.com/t3net2/master/poll.php", 5566, "PP2", PP2_VERSION_NETWORK, argv[1], NULL, 1);
	if(!server_key)
	{
		no_poll = true;
	}
	if(!no_poll)
	{
		server_poll_thread = al_create_thread(server_poll_thread_proc, NULL);
		if(!server_poll_thread)
		{
printf("no server thread\n");
			return 0;
		}
		al_init_user_event_source(&server_poll_event_source);
		al_start_thread(server_poll_thread);
	}

	/* init console display */
	screen = initscr();
	noecho();
	cbreak();
	nodelay(screen, TRUE);
	refresh();
	wrefresh(screen);

	/* run application */
	t3f_run();

	endwin();
	delwin(screen);
	joynet_destroy_game(game);
	joynet_close_server(server);
	joynet_destroy_server(server);
	joynet_exit();
	return 0;
}
