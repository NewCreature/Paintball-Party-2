#include "t3f/t3f.h"
#include "t3f/music.h"
#include "t3f/sound.h"
#include "joynet/game.h"
#include "../data.h"
#include "../init.h"
#include "../text_entry.h"
#include "../tables.h"
#include "../version.h"
#include "../defines.h"
#include "../gameplay/game.h"
#include "../gameplay/game_defines.h"
#include "../gameplay/replay.h"
#include "../gameplay/network.h"
#include "../file/database/level.h"
#include "../network/server.h"
#include "../network/client.h"
#include "../file/music.h"
#include "player_setup.h"
#include "menu.h"
#include "menu_proc.h"
#include "level_setup.h"
#include "preview.h"
#include "interface.h"

static void select_first_menu_item(PP2_INTERFACE * ip)
{
	int i;

	for(i = 0; i < ip->menu[ip->current_menu]->elements; i++)
	{
		if(!(ip->menu[ip->current_menu]->element[i].flags & T3F_GUI_ELEMENT_STATIC) && ip->menu[ip->current_menu]->element[i].proc)
		{
			ip->menu[ip->current_menu]->hover_element = i;
			break;
		}
	}
}

void pp2_select_menu(PP2_INTERFACE * ip, int menu)
{
	ip->previous_menu[ip->menu_stack_size] = ip->current_menu;
	ip->menu_stack_size++;
	ip->current_menu = menu;
	if(!ip->joystick_menu_activation)
	{
		ip->menu[ip->current_menu]->hover_element = -1;
	}
	else
	{
		select_first_menu_item(ip);
	}
}

void pp2_select_previous_menu(PP2_INTERFACE * ip)
{
	if(ip->menu_stack_size > 0)
	{
		ip->current_menu = ip->previous_menu[ip->menu_stack_size - 1];
		ip->menu_stack_size--;
	}
}

int pp2_menu_proc_profiles_left(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_NEXT], 1.0, 0.0, 1.0);
	instance->interface.selected_profile--;
	if(instance->interface.selected_profile < 0)
	{
		instance->interface.selected_profile = instance->interface.profiles.items - 1;
	}
	pp2_generate_profiles_menu(&instance->interface, &instance->resources);
	return 1;
}

int pp2_menu_proc_profiles_right(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_NEXT], 1.0, 0.0, 1.0);
	instance->interface.selected_profile++;
	if(instance->interface.selected_profile >= instance->interface.profiles.items)
	{
		instance->interface.selected_profile = 0;
	}
	pp2_generate_profiles_menu(&instance->interface, &instance->resources);
	return 1;
}

void pp2_generate_profiles_menu(PP2_INTERFACE * ip, PP2_RESOURCES * resources)
{
	int i;
	unsigned long best_current = 0;
	unsigned long best = 0;
	int besti = -1;
	unsigned long most_current;
	unsigned long most = 0;
	int mosti = -1;
	unsigned long shots_current;
	unsigned long shots = 0;
	int shotsi = -1;
	unsigned long hits_current;
	unsigned long hits = 0;
	int hitsi = -1;

	if(ip->menu[PP2_MENU_PROFILES])
	{
		t3f_destroy_gui(ip->menu[PP2_MENU_PROFILES]);
	}
	ip->menu[PP2_MENU_PROFILES] = t3f_create_gui(0, 0);
	if(ip->selected_profile > 0)
	{
		t3f_add_gui_text_element(ip->menu[PP2_MENU_PROFILES], NULL, ip->profiles.item[ip->selected_profile].name, (void **)&resources->font[PP2_FONT_COMIC_16], 320, 240 + 24 * 0, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
		t3f_add_gui_text_element(ip->menu[PP2_MENU_PROFILES], pp2_menu_proc_profiles_left, "<", (void **)&resources->font[PP2_FONT_COMIC_16], 320 - al_get_text_width(resources->font[PP2_FONT_COMIC_16], "A long profile name") / 2 - al_get_text_width(resources->font[PP2_FONT_COMIC_16], "<"), 240 + 24 * 0, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
		t3f_add_gui_text_element(ip->menu[PP2_MENU_PROFILES], pp2_menu_proc_profiles_right, ">", (void **)&resources->font[PP2_FONT_COMIC_16], 320 + al_get_text_width(resources->font[PP2_FONT_COMIC_16], "A long profile name") / 2, 240 + 24 * 0, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);

		sprintf(ip->menu_text[0], "Games Played: %lu", ip->profiles.item[ip->selected_profile].plays);
		t3f_add_gui_text_element(ip->menu[PP2_MENU_PROFILES], NULL, ip->menu_text[0], (void **)&resources->font[PP2_FONT_COMIC_10], 320, 240 + 24 * 2 + 14 * 0, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
		sprintf(ip->menu_text[1], "Wins: %lu", ip->profiles.item[ip->selected_profile].wins);
		t3f_add_gui_text_element(ip->menu[PP2_MENU_PROFILES], NULL, ip->menu_text[1], (void **)&resources->font[PP2_FONT_COMIC_10], 320, 240 + 24 * 2 + 14 * 1, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
		sprintf(ip->menu_text[2], "Losses: %lu", ip->profiles.item[ip->selected_profile].losses);
		t3f_add_gui_text_element(ip->menu[PP2_MENU_PROFILES], NULL, ip->menu_text[2], (void **)&resources->font[PP2_FONT_COMIC_10], 320, 240 + 24 * 2 + 14 * 2, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
		if(ip->profiles.item[ip->selected_profile].wins + ip->profiles.item[ip->selected_profile].losses > 0)
		{
			sprintf(ip->menu_text[3], "Percentage: %lu%%", (ip->profiles.item[ip->selected_profile].wins * 100) / (ip->profiles.item[ip->selected_profile].wins + ip->profiles.item[ip->selected_profile].losses));
		}
		else
		{
			sprintf(ip->menu_text[3], "Percentage: N/A");
		}
		t3f_add_gui_text_element(ip->menu[PP2_MENU_PROFILES], NULL, ip->menu_text[3], (void **)&resources->font[PP2_FONT_COMIC_10], 320, 240 + 24 * 2 + 14 * 3, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
		sprintf(ip->menu_text[4], "Shots: %lu", ip->profiles.item[ip->selected_profile].shots);
		t3f_add_gui_text_element(ip->menu[PP2_MENU_PROFILES], NULL, ip->menu_text[4], (void **)&resources->font[PP2_FONT_COMIC_10], 320, 240 + 24 * 2 + 14 * 5, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
		sprintf(ip->menu_text[5], "Hits: %lu", ip->profiles.item[ip->selected_profile].hits);
		t3f_add_gui_text_element(ip->menu[PP2_MENU_PROFILES], NULL, ip->menu_text[5], (void **)&resources->font[PP2_FONT_COMIC_10], 320, 240 + 24 * 2 + 14 * 6, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
		if(ip->profiles.item[ip->selected_profile].shots > 0)
		{
			sprintf(ip->menu_text[6], "Accuracy: %lu%%", (ip->profiles.item[ip->selected_profile].hits * 100) / (ip->profiles.item[ip->selected_profile].shots));
		}
		else
		{
			sprintf(ip->menu_text[6], "Accuracy: N/A");
		}
		t3f_add_gui_text_element(ip->menu[PP2_MENU_PROFILES], NULL, ip->menu_text[4], (void **)&resources->font[PP2_FONT_COMIC_10], 320, 240 + 24 * 2 + 14 * 7, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
	}
	else
	{
		t3f_add_gui_text_element(ip->menu[PP2_MENU_PROFILES], NULL, "Everyone", (void **)&resources->font[PP2_FONT_COMIC_16], 320, 240 + 24 * 0, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
		t3f_add_gui_text_element(ip->menu[PP2_MENU_PROFILES], pp2_menu_proc_profiles_left, "<", (void **)&resources->font[PP2_FONT_COMIC_16], 320 - al_get_text_width(resources->font[PP2_FONT_COMIC_16], "A long profile name") / 2 - al_get_text_width(resources->font[PP2_FONT_COMIC_16], "<"), 240 + 24 * 0, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
		t3f_add_gui_text_element(ip->menu[PP2_MENU_PROFILES], pp2_menu_proc_profiles_right, ">", (void **)&resources->font[PP2_FONT_COMIC_16], 320 + al_get_text_width(resources->font[PP2_FONT_COMIC_16], "A long profile name") / 2, 240 + 24 * 0, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);

		for(i = 1; i < ip->profiles.items; i++)
		{
			if(ip->profiles.item[i].wins + ip->profiles.item[i].losses > 0)
			{
				best_current = (ip->profiles.item[i].wins * 100) / (ip->profiles.item[i].wins + ip->profiles.item[i].losses);
				if(best_current > best)
				{
					besti = i;
					best = best_current;
				}
			}
			most_current = ip->profiles.item[i].plays;
			if(most_current > most)
			{
				mosti = i;
				most = most_current;
			}
			if(ip->profiles.item[i].shots > 0 && ip->profiles.item[i].hits > 0)
			{
				shots_current = (ip->profiles.item[i].shots * 100) / (ip->profiles.item[i].hits);
				if(shots_current > shots)
				{
					shotsi = i;
					shots = shots_current;
				}
			}
			hits_current = ip->profiles.item[i].shot;
			if(hits_current > hits)
			{
				hitsi = i;
				hits = hits_current;
			}
		}
		if(besti >= 0)
		{
			sprintf(ip->menu_text[0], "Best Player: %s", ip->profiles.item[besti].name);
		}
		else
		{
			sprintf(ip->menu_text[0], "Best Player: N/A");
		}
		t3f_add_gui_text_element(ip->menu[PP2_MENU_PROFILES], NULL, ip->menu_text[0], (void **)&resources->font[PP2_FONT_COMIC_10], 320, 240 + 24 * 2 + 14 * 0, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
		if(mosti >= 0)
		{
			sprintf(ip->menu_text[1], "Most Addicted: %s", ip->profiles.item[mosti].name);
		}
		else
		{
			sprintf(ip->menu_text[1], "Most Addicted: N/A");
		}
		t3f_add_gui_text_element(ip->menu[PP2_MENU_PROFILES], NULL, ip->menu_text[1], (void **)&resources->font[PP2_FONT_COMIC_10], 320, 240 + 24 * 2 + 14 * 1, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
		if(shotsi >= 0)
		{
			sprintf(ip->menu_text[2], "Most Accurate: %s", ip->profiles.item[shotsi].name);
		}
		else
		{
			sprintf(ip->menu_text[2], "Most Accurate: N/A");
		}
		t3f_add_gui_text_element(ip->menu[PP2_MENU_PROFILES], NULL, ip->menu_text[2], (void **)&resources->font[PP2_FONT_COMIC_10], 320, 240 + 24 * 2 + 14 * 2, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
		if(hitsi >= 0)
		{
			sprintf(ip->menu_text[3], "Cannon Fodder: %s", ip->profiles.item[hitsi].name);
		}
		else
		{
			sprintf(ip->menu_text[3], "Cannon Fodder: N/A");
		}
		t3f_add_gui_text_element(ip->menu[PP2_MENU_PROFILES], NULL, ip->menu_text[3], (void **)&resources->font[PP2_FONT_COMIC_10], 320, 240 + 24 * 2 + 14 * 3, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
		t3f_add_gui_text_element(ip->menu[PP2_MENU_PROFILES], NULL, "", (void **)&resources->font[PP2_FONT_COMIC_10], 320, 240 + 24 * 2 + 14 * 7, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
	}
	t3f_center_gui(ip->menu[PP2_MENU_PROFILES], 200.0, 456.0);
	t3f_add_gui_text_element(ip->menu[PP2_MENU_PROFILES], pp2_menu_proc_overlay_back, "< Back", (void **)&resources->font[PP2_FONT_COMIC_16], t3f_default_view->left, t3f_default_view->bottom - al_get_font_line_height(resources->font[PP2_FONT_COMIC_16]) - ip->menu[PP2_MENU_PROFILES]->oy, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW);
}

int pp2_menu_proc_main_play(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	instance->state = PP2_STATE_PLAYER_SETUP;
	return 1;
}

int pp2_menu_proc_main_play_network(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	pp2_select_menu(&instance->interface, PP2_MENU_PLAY_NETWORK);
	return 1;
}

int pp2_menu_proc_main_play_online(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	if(strlen(pp2_network_id) > 0)
	{
		pp2_select_menu(&instance->interface, PP2_MENU_PLAY_ONLINE);
	}
	else
	{
		PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

		pp2_select_menu(&instance->interface, PP2_MENU_NETWORK_ID);
		strcpy(pp2_entered_text, pp2_network_id);
		pp2_entering_text = 1;
		pp2_entering_text_pos = strlen(pp2_entered_text);
		t3f_clear_keys();
	}
	return 1;
}

int pp2_menu_proc_main_play_lan(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	if(strlen(pp2_network_id) > 0)
	{
		pp2_select_menu(&instance->interface, PP2_MENU_PLAY_LAN);
	}
	else
	{
		pp2_select_menu(&instance->interface, PP2_MENU_NETWORK_ID);
		strcpy(pp2_entered_text, pp2_network_id);
		pp2_entering_text = 1;
		pp2_entering_text_pos = strlen(pp2_entered_text);
		t3f_clear_keys();
	}
	return 1;
}

int pp2_menu_proc_play_lan_host(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;
	int c;

	instance->interface.menu_joystick_disabled = false;
	pp2_entering_text = 0;
	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	al_stop_timer(t3f_timer);
	instance->lan_arg = 1;
	pp2_server_thread = al_create_thread(pp2_server_thread_proc, instance);
	if(!pp2_server_thread)
	{
		return 1;
	}
	strcpy(pp2_server_name, pp2_entered_text);
	al_start_thread(pp2_server_thread);
	pp2_client = joynet_create_client();
	if(pp2_client)
	{
		al_rest(0.25); // give server thread time to start up
		if(joynet_connect_to_game_server(pp2_client_game, pp2_client, "127.0.0.1", 5566))
		{
			for(c = 0; c < pp2_client_game->players; c++)
			{
				instance->game.player[c].step = 0;
			}
			joynet_set_client_screen_name(pp2_client, pp2_network_id);
			joynet_watch_game(pp2_client_game);
			pp2_select_menu(&instance->interface, PP2_MENU_MAIN_HOST);
			instance->interface.menu_stack_size = 0;
			joynet_set_client_chat_callback(pp2_client, pp2_chat_callback, data);
			pp2_lan_play = true;
		}
		else
		{
			joynet_destroy_client(pp2_client);
			pp2_client = NULL;
			al_join_thread(pp2_server_thread, NULL);
			al_destroy_thread(pp2_server_thread);
			pp2_server_thread = NULL;
		}
	}
	else
	{
		al_join_thread(pp2_server_thread, NULL);
		al_destroy_thread(pp2_server_thread);
		pp2_server_thread = NULL;
	}
	al_set_config_value(instance->interface.config, "Network Settings", "Server Name", pp2_server_name);
	al_start_timer(t3f_timer);
	return 1;
}

int pp2_menu_proc_play_lan_join(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	pp2_select_menu(&instance->interface, PP2_MENU_HOST_IP);
	strcpy(pp2_entered_text, "");
	pp2_entering_text = 1;
	pp2_entering_text_pos = 0;
	t3f_clear_keys();
	instance->interface.menu_joystick_disabled = true;
	return 1;
}

int pp2_menu_proc_host_ip_ok(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;
	char message[256] = {0};

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	pp2_entering_text = 0;
	al_stop_timer(t3f_timer);
	pp2_client = joynet_create_client();
	if(pp2_client)
	{
		pp2_player_setup_reset(&instance->game);
		if(joynet_connect_to_game_server(pp2_client_game, pp2_client, pp2_entered_text, 5566))
		{
			joynet_set_client_screen_name(pp2_client, pp2_network_id);
			joynet_watch_game(pp2_client_game);
			pp2_select_menu(&instance->interface, PP2_MENU_MAIN_CLIENT);
			instance->interface.menu_stack_size = 0;
			joynet_set_client_chat_callback(pp2_client, pp2_chat_callback, instance);
			joynet_set_client_global_callback(pp2_client, pp2_client_callback, instance);
			pp2_entering_text = 0;
		}
		else
		{
			sprintf(message, "Failed to connect to server.");
			pp2_add_message(pp2_messages, message, instance->resources.font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 0.0, 0.0, 1.0), 300, PP2_SCREEN_VISIBLE_WIDTH, 0.0);
			joynet_destroy_client(pp2_client);
			pp2_client = NULL;
		}
	}
	al_start_timer(t3f_timer);
	return 1;
}

int pp2_menu_proc_network_id_ok(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	pp2_select_menu(&instance->interface, PP2_MENU_PLAY_ONLINE);
	strcpy(pp2_network_id, pp2_entered_text);
	al_set_config_value(instance->interface.config, "Network Settings", "ID", pp2_network_id);
	pp2_entering_text = 0;
	return 1;
}

int pp2_menu_proc_play_online_host(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	if(strlen(pp2_server_name) > 0)
	{
		strcpy(pp2_entered_text, pp2_server_name);
	}
	else
	{
		strcpy(pp2_entered_text, "Unnamed Host");
	}
	pp2_entering_text_pos = strlen(pp2_entered_text);
	pp2_entering_text = 1;
	t3f_clear_keys();
	pp2_select_menu(&instance->interface, PP2_MENU_HOST_NAME);
	instance->interface.menu_joystick_disabled = true;
	return 1;
}

int pp2_menu_proc_host_name_ok(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;
	int c;

	instance->interface.menu_joystick_disabled = false;
	pp2_entering_text = 0;
	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	al_stop_timer(t3f_timer);
	pp2_server_thread = al_create_thread(pp2_server_thread_proc, data);
	if(!pp2_server_thread)
	{
		return 1;
	}
	strcpy(pp2_server_name, pp2_entered_text);
	al_start_thread(pp2_server_thread);
	pp2_client = joynet_create_client();
	if(pp2_client)
	{
		al_rest(0.25); // give server thread time to start up
		if(joynet_connect_to_game_server(pp2_client_game, pp2_client, "127.0.0.1", 5566))
		{
			for(c = 0; c < pp2_client_game->players; c++)
			{
				instance->game.player[c].step = 0;
			}
			joynet_set_client_screen_name(pp2_client, pp2_network_id);
			joynet_watch_game(pp2_client_game);
			pp2_select_menu(&instance->interface, PP2_MENU_MAIN_HOST);
			instance->interface.menu_stack_size = 0;
			joynet_set_client_chat_callback(pp2_client, pp2_chat_callback, data);
		}
		else
		{
			joynet_destroy_client(pp2_client);
			pp2_client = NULL;
			al_join_thread(pp2_server_thread, NULL);
			al_destroy_thread(pp2_server_thread);
			pp2_server_thread = NULL;
		}
	}
	else
	{
		al_join_thread(pp2_server_thread, NULL);
		al_destroy_thread(pp2_server_thread);
		pp2_server_thread = NULL;
	}
	al_set_config_value(instance->interface.config, "Network Settings", "Server Name", pp2_server_name);
	al_start_timer(t3f_timer);
	return 1;
}

static bool pp2_create_server_list_menu(PP2_INTERFACE * ip, PP2_RESOURCES * resources)
{
	int i;
	char text[1024] = {0};
	bool fail = false;

	if(ip->menu[PP2_MENU_PLAY_ONLINE_LIST])
	{
		t3f_destroy_gui(ip->menu[PP2_MENU_PLAY_ONLINE_LIST]);
		ip->menu[PP2_MENU_PLAY_ONLINE_LIST] = NULL;
	}
	if(!pp2_server_list)
	{
		pp2_server_list = t3net_get_server_list("www.t3-i.com/t3net2/master/query.php", "PP2", PP2_VERSION_NETWORK);
		if(!pp2_server_list)
		{
			fail = true;
		}
	}
	else
	{
		if(!t3net_update_server_list_2(pp2_server_list))
		{
			fail = true;
		}
	}
	if(!fail)
	{
		if(pp2_server_list->entries <= 0)
		{
			pp2_add_message(pp2_messages, "No public servers available.", resources->font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 0.0, 0.0, 1.0), 300, PP2_SCREEN_VISIBLE_WIDTH, 0.0);
			fail = true;
		}
	}
	else
	{
		pp2_add_message(pp2_messages, "Unable to retrieve server list.", resources->font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 0.0, 0.0, 1.0), 300, PP2_SCREEN_VISIBLE_WIDTH, 0.0);
	}
	if(pp2_server_list && !fail)
	{
		ip->menu[PP2_MENU_PLAY_ONLINE_LIST] = t3f_create_gui(0, 0);
		for(i = 0; i < pp2_server_list->entries; i++)
		{
			sprintf(text, "%s (%s)", pp2_server_list->entry[i]->name, pp2_server_list->entry[i]->capacity);
//			printf("%s\n", text);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_ONLINE_LIST], pp2_menu_proc_server_list_select, text, (void **)&resources->font[PP2_FONT_COMIC_16], 320, 240 + i * 24, al_map_rgba_f(1.0, 1.0, 1.0, 1.0), T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_COPY);
		}
	}
	t3f_center_gui(ip->menu[PP2_MENU_PLAY_STOCK], 0.0, 480.0);
	return !fail;
}

int pp2_menu_proc_play_online_join(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	al_stop_timer(t3f_timer);
	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	pp2_entered_text[0] = 0;
	pp2_entering_text = 1;
	pp2_entering_text_pos = 0;
	t3f_clear_keys();
	if(pp2_create_server_list_menu(&instance->interface, &instance->resources))
	{
		if(instance->interface.menu[PP2_MENU_PLAY_ONLINE_LIST])
		{
			pp2_select_menu(&instance->interface, PP2_MENU_PLAY_ONLINE_LIST);
		}
	}
	al_start_timer(t3f_timer);
	return 1;
}

int pp2_menu_proc_server_list_select(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;
	al_stop_timer(t3f_timer);
	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	pp2_client = joynet_create_client();
	if(pp2_client)
	{
		if(joynet_connect_to_game_server(pp2_client_game, pp2_client, pp2_server_list->entry[i]->address, 5566))
		{
			joynet_set_client_screen_name(pp2_client, pp2_network_id);
			joynet_watch_game(pp2_client_game);
			pp2_select_menu(&instance->interface, PP2_MENU_MAIN_CLIENT);
			instance->interface.menu_stack_size = 0;
			joynet_set_client_chat_callback(pp2_client, pp2_chat_callback, instance);
			joynet_set_client_global_callback(pp2_client, pp2_client_callback, instance);
			pp2_entering_text = 0;
		}
		else
		{
			joynet_destroy_client(pp2_client);
			pp2_client = NULL;
		}
	}
	al_start_timer(t3f_timer);
	return 1;
}

int pp2_menu_proc_main_disconnect(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	al_stop_timer(t3f_timer);
	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	pp2_clear_messages(pp2_messages);
	joynet_disconnect_from_game_server(pp2_client_game, pp2_client);
	joynet_destroy_client(pp2_client);
	pp2_client = NULL;
	pp2_select_menu(&instance->interface, PP2_MENU_MAIN);
	instance->interface.menu_stack_size = 0;
	pp2_player_setup_reset(&instance->game);
	al_start_timer(t3f_timer);
	return 1;
}

int pp2_menu_proc_main_close_server(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	al_stop_timer(t3f_timer);
	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	pp2_clear_messages(pp2_messages);
	joynet_disconnect_from_game_server(pp2_client_game, pp2_client);
	joynet_destroy_client(pp2_client);
	pp2_client = NULL;
	al_join_thread(pp2_server_thread, NULL);
	al_destroy_thread(pp2_server_thread);
	pp2_server_thread = NULL;
	pp2_select_menu(&instance->interface, PP2_MENU_MAIN);
	instance->interface.menu_stack_size = 0;
	pp2_player_setup_reset(&instance->game);
	al_start_timer(t3f_timer);
	return 1;
}

int pp2_menu_proc_play_online_join_connect(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;
	al_stop_timer(t3f_timer);
	pp2_client = joynet_create_client();
	if(pp2_client)
	{
		pp2_player_setup_reset(&instance->game);
		if(joynet_connect_to_game_server(pp2_client_game, pp2_client, pp2_entered_text, 5566))
		{
			joynet_set_client_screen_name(pp2_client, pp2_network_id);
			joynet_watch_game(pp2_client_game);
			pp2_select_menu(&instance->interface, PP2_MENU_MAIN_CLIENT);
			instance->interface.menu_stack_size = 0;
			joynet_set_client_chat_callback(pp2_client, pp2_chat_callback, instance);
			joynet_set_client_global_callback(pp2_client, pp2_client_callback, instance);
			pp2_entering_text = 0;
		}
		else
		{
			joynet_destroy_client(pp2_client);
			pp2_client = NULL;
		}
	}
	al_start_timer(t3f_timer);
	return 1;
}

int pp2_menu_proc_main_view_replay(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;
	char buf[1024];
	ALLEGRO_PATH * start = NULL;
	const char * rp = NULL;
	bool played = false;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	start = al_create_path(t3f_get_filename(t3f_data_path, "replays/", buf, 1024));
	instance->interface.replay_filechooser = al_create_native_file_dialog(al_path_cstr(start, '/'), "Load Replay", "*.p2r", ALLEGRO_FILECHOOSER_FILE_MUST_EXIST | ALLEGRO_FILECHOOSER_MULTIPLE);
/*	if(!file_load_dialog)
	{
		file_load_dialog = al_create_native_file_dialog(NULL, "Load Replay", "*.p2r", ALLEGRO_FILECHOOSER_FILE_MUST_EXIST);
	} */
	if(instance->interface.replay_filechooser)
	{
		al_stop_timer(t3f_timer);
		al_show_native_file_dialog(t3f_display, instance->interface.replay_filechooser);
		if(al_get_native_file_dialog_count(instance->interface.replay_filechooser) == 1)
		{
			rp = al_get_native_file_dialog_path(instance->interface.replay_filechooser, 0);
			if(rp)
			{
				if(!pp2_play_replay(&instance->game, rp, i < 0 ? PP2_REPLAY_FLAG_CAPTURE : 0, &instance->interface, &instance->resources))
				{
					pp2_add_message(pp2_messages, "Failed to play replay.", instance->resources.font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 0.0, 0.0, 1.0), 300, PP2_SCREEN_VISIBLE_WIDTH, 0.0);
				}
			}
			al_destroy_native_file_dialog(instance->interface.replay_filechooser);
			instance->interface.replay_filechooser = NULL;
		}
		else if(al_get_native_file_dialog_count(instance->interface.replay_filechooser) > 1)
		{
			instance->interface.replay_file_number = 0;
			while(instance->interface.replay_file_number < al_get_native_file_dialog_count(instance->interface.replay_filechooser) && !played)
			{
				rp = al_get_native_file_dialog_path(instance->interface.replay_filechooser, instance->interface.replay_file_number);
				if(rp)
				{
					if(pp2_play_replay(&instance->game, rp, PP2_REPLAY_FLAG_THEATER, &instance->interface, &instance->resources))
					{
						played = true;
						instance->state = PP2_STATE_THEATER;
					}
				}
				instance->interface.replay_file_number++;
			}
			if(!played)
			{
				pp2_add_message(pp2_messages, "Could not enter theater mode, no valid replays.", instance->resources.font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 0.0, 0.0, 1.0), 300, PP2_SCREEN_VISIBLE_WIDTH, 0.0);
			}
		}
		al_start_timer(t3f_timer);
	}
	al_destroy_path(start);

	return 1;
}

int pp2_menu_proc_main_capture_replay(void * data, int i, void * p)
{
	pp2_menu_proc_main_view_replay(data, -1, p);
	return 1;
}

int pp2_menu_proc_main_profiles(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	pp2_generate_profiles_menu(&instance->interface, &instance->resources);
	instance->interface.selected_profile = 0;
	pp2_select_menu(&instance->interface, PP2_MENU_PROFILES);
	return 1;
}

int pp2_menu_proc_main_options(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	pp2_select_menu(&instance->interface, PP2_MENU_OPTIONS);
	return 1;
}

int pp2_menu_proc_options_controllers(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	pp2_select_menu(&instance->interface, PP2_MENU_CONTROLLERS);
	return 1;
}

static char pp2_menu_controller_name[1024] = {0};

static const char * pp2_get_menu_controller_name(PP2_INTERFACE * ip, int controller)
{
	int i;
	char controller_name[1024] = {0};

	strcpy(controller_name, t3f_get_controller_name(ip->controller[controller], 0));
	for(i = 1; i < 8; i++)
	{
		if(strcasecmp(controller_name, t3f_get_controller_name(ip->controller[controller], i)))
		{
			strcpy(pp2_menu_controller_name, "Multiple Control Sources");
			return pp2_menu_controller_name;
		}
	}
	return t3f_get_controller_name(ip->controller[controller], 0);
}

static void pp2_menu_update_controller_text(PP2_INTERFACE * ip, int controller)
{
	int c;

	sprintf(pp2_controller_binding_text[0], "Controller %d", controller + 1);
	sprintf(pp2_controller_binding_text[10], "%s", pp2_get_menu_controller_name(ip, controller));
	for(c = 0; c < 9; c++)
	{
		sprintf(pp2_controller_binding_text[c + 1], "%s", t3f_get_controller_binding_name(ip->controller[controller], c));
	}
}

int pp2_menu_proc_options_controller_1(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	pp2_menu_update_controller_text(&instance->interface, 0);
	instance->interface.menu_selected_controller = 0;
	pp2_select_menu(&instance->interface, PP2_MENU_CONTROLLER);
	return 1;
}

int pp2_menu_proc_options_controller_2(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	pp2_menu_update_controller_text(&instance->interface, 1);
	instance->interface.menu_selected_controller = 1;
	pp2_select_menu(&instance->interface, PP2_MENU_CONTROLLER);
	return 1;
}

int pp2_menu_proc_options_controller_3(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	pp2_menu_update_controller_text(&instance->interface, 2);
	instance->interface.menu_selected_controller = 2;
	pp2_select_menu(&instance->interface, PP2_MENU_CONTROLLER);
	return 1;
}

int pp2_menu_proc_options_controller_4(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	pp2_menu_update_controller_text(&instance->interface, 3);
	instance->interface.menu_selected_controller = 3;
	pp2_select_menu(&instance->interface, PP2_MENU_CONTROLLER);
	return 1;
}

int pp2_menu_proc_options_controller_reset(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	pp2_autodetect_controllers();
	return 1;
}

int pp2_menu_proc_controller_up(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	al_stop_timer(t3f_timer);
	t3f_bind_controller(instance->interface.controller[instance->interface.menu_selected_controller], PP2_CONTROLLER_UP);
	pp2_menu_update_controller_text(&instance->interface, instance->interface.menu_selected_controller);
	al_start_timer(t3f_timer);
	pp2_set_controller_config(instance->interface.menu_selected_controller, PP2_CONTROLLER_UP);
	instance->interface.menu_joystick_skip = 1;
	return 1;
}

int pp2_menu_proc_controller_down(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	al_stop_timer(t3f_timer);
	t3f_bind_controller(instance->interface.controller[instance->interface.menu_selected_controller], PP2_CONTROLLER_DOWN);
	pp2_menu_update_controller_text(&instance->interface, instance->interface.menu_selected_controller);
	al_start_timer(t3f_timer);
	pp2_set_controller_config(instance->interface.menu_selected_controller, PP2_CONTROLLER_DOWN);
	instance->interface.menu_joystick_skip = 1;
	return 1;
}

int pp2_menu_proc_controller_left(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	al_stop_timer(t3f_timer);
	t3f_bind_controller(instance->interface.controller[instance->interface.menu_selected_controller], PP2_CONTROLLER_LEFT);
	pp2_menu_update_controller_text(&instance->interface, instance->interface.menu_selected_controller);
	al_start_timer(t3f_timer);
	pp2_set_controller_config(instance->interface.menu_selected_controller, PP2_CONTROLLER_LEFT);
	instance->interface.menu_joystick_skip = 1;
	return 1;
}

int pp2_menu_proc_controller_right(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	al_stop_timer(t3f_timer);
	t3f_bind_controller(instance->interface.controller[instance->interface.menu_selected_controller], PP2_CONTROLLER_RIGHT);
	pp2_menu_update_controller_text(&instance->interface, instance->interface.menu_selected_controller);
	al_start_timer(t3f_timer);
	pp2_set_controller_config(instance->interface.menu_selected_controller, PP2_CONTROLLER_RIGHT);
	instance->interface.menu_joystick_skip = 1;
	return 1;
}

int pp2_menu_proc_controller_jump(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	al_stop_timer(t3f_timer);
	t3f_bind_controller(instance->interface.controller[instance->interface.menu_selected_controller], PP2_CONTROLLER_JUMP);
	pp2_menu_update_controller_text(&instance->interface, instance->interface.menu_selected_controller);
	al_start_timer(t3f_timer);
	pp2_set_controller_config(instance->interface.menu_selected_controller, PP2_CONTROLLER_JUMP);
	instance->interface.menu_joystick_skip = 1;
	return 1;
}

int pp2_menu_proc_controller_fire(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	al_stop_timer(t3f_timer);
	t3f_bind_controller(instance->interface.controller[instance->interface.menu_selected_controller], PP2_CONTROLLER_FIRE);
	pp2_menu_update_controller_text(&instance->interface, instance->interface.menu_selected_controller);
	al_start_timer(t3f_timer);
	pp2_set_controller_config(instance->interface.menu_selected_controller, PP2_CONTROLLER_FIRE);
	instance->interface.menu_joystick_skip = 1;
	return 1;
}

int pp2_menu_proc_controller_select(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	al_stop_timer(t3f_timer);
	t3f_bind_controller(instance->interface.controller[instance->interface.menu_selected_controller], PP2_CONTROLLER_SELECT);
	pp2_menu_update_controller_text(&instance->interface, instance->interface.menu_selected_controller);
	al_start_timer(t3f_timer);
	pp2_set_controller_config(instance->interface.menu_selected_controller, PP2_CONTROLLER_SELECT);
	instance->interface.menu_joystick_skip = 1;
	return 1;
}

int pp2_menu_proc_controller_strafe(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	al_stop_timer(t3f_timer);
	t3f_bind_controller(instance->interface.controller[instance->interface.menu_selected_controller], PP2_CONTROLLER_STRAFE);
	pp2_menu_update_controller_text(&instance->interface, instance->interface.menu_selected_controller);
	al_start_timer(t3f_timer);
	pp2_set_controller_config(instance->interface.menu_selected_controller, PP2_CONTROLLER_STRAFE);
	instance->interface.menu_joystick_skip = 1;
	return 1;
}

int pp2_menu_proc_controller_show_scores(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	al_stop_timer(t3f_timer);
	t3f_bind_controller(instance->interface.controller[instance->interface.menu_selected_controller], PP2_CONTROLLER_SCORES);
	pp2_menu_update_controller_text(&instance->interface, instance->interface.menu_selected_controller);
	al_start_timer(t3f_timer);
	pp2_set_controller_config(instance->interface.menu_selected_controller, PP2_CONTROLLER_SCORES);
	instance->interface.menu_joystick_skip = 1;
	return 1;
}

int pp2_menu_proc_options_audio(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	pp2_select_menu(&instance->interface, PP2_MENU_AUDIO);
	sprintf(instance->interface.menu_text[0], "%d%%", (int)(t3f_get_music_volume() * 100.0 + 0.1));
	sprintf(instance->interface.menu_text[1], "%d%%", (int)(instance->interface.sound_volume * 100.0 + 0.1));
	return 1;
}

int pp2_menu_proc_music_down(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_NEXT], 1.0, 0.0, 1.0);
	if(t3f_get_music_volume() > 0.0)
	{
		pp2_set_music_volume(t3f_get_music_volume() - 0.05);
		sprintf(instance->interface.menu_text[0], "%d%%", (int)(t3f_get_music_volume() * 100.0 + 0.1));
	}
	return 1;
}

int pp2_menu_proc_music_up(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_NEXT], 1.0, 0.0, 1.0);
	if(t3f_get_music_volume() < 1.0)
	{
		pp2_set_music_volume(t3f_get_music_volume() + 0.05);
		sprintf(instance->interface.menu_text[0], "%d%%", (int)(t3f_get_music_volume() * 100.0 + 0.1));
	}
	return 1;
}

int pp2_menu_proc_sound_down(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	if(t3f_get_sound_volume() > 0.0)
	{
		t3f_set_sound_volume(t3f_get_sound_volume() - 0.05);
		sprintf(instance->interface.menu_text[1], "%d%%", (int)(t3f_get_sound_volume() * 100.0 + 0.1));
	}
	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_NEXT], 1.0, 0.0, 1.0);
	return 1;
}

int pp2_menu_proc_sound_up(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	if(t3f_get_sound_volume() < 1.0)
	{
		t3f_set_sound_volume(t3f_get_sound_volume() + 0.05);
		sprintf(instance->interface.menu_text[1], "%d%%", (int)(t3f_get_sound_volume() * 100.0 + 0.1));
	}
	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_NEXT], 1.0, 0.0, 1.0);
	return 1;
}

int pp2_menu_proc_options_video(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	pp2_select_menu(&instance->interface, PP2_MENU_VIDEO);
	sprintf(instance->interface.menu_text[0], "%s", (al_get_display_flags(t3f_display) & ALLEGRO_FULLSCREEN_WINDOW) ? "Full Screen" : "Window");
	sprintf(instance->interface.menu_text[1], "%dx%d", al_get_display_width(t3f_display), al_get_display_height(t3f_display));
	return 1;
}

static void set_window_size(int w, int h)
{
	t3f_set_gfx_mode(w, h, t3f_flags);
	#ifdef ALLEGRO_WINDOWS
		t3f_unload_resources();
		t3f_reload_resources();
	#endif
}

int pp2_menu_proc_display_toggle(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;
	const char * val = al_get_config_value(t3f_config, "T3F", "force_fullscreen");
	int fs = 0;
	char buffer[32] = {0};
	ALLEGRO_MONITOR_INFO info;
	int nw, nh;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_NEXT], 1.0, 0.0, 1.0);
	if(val)
	{
		if(!strcmp(val, "true"))
		{
			al_set_config_value(t3f_config, "T3F", "force_fullscreen", "false");
		}
		else
		{
			al_set_config_value(t3f_config, "T3F", "force_fullscreen", "true");
			fs = 1;
		}
	}
	else
	{
		al_set_config_value(t3f_config, "T3F", "force_fullscreen", "true");
		fs = 1;
	}
	if(t3f_set_gfx_mode(640, 480, fs ? T3F_USE_FULLSCREEN : 0) == 2)
	{
		pp2_add_message(pp2_messages, "Game must be restarted for change to take effect.", instance->resources.font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 0.0, 0.0, 1.0), 300, PP2_SCREEN_VISIBLE_WIDTH, 0.0);
		al_get_monitor_info(0, &info);
		nw = info.x2 - info.x1;
		nh = info.y2 - info.y1;
		sprintf(buffer, "%d", nw);
		al_set_config_value(t3f_config, "T3F", "display_width", buffer);
		sprintf(buffer, "%d", nh);
		al_set_config_value(t3f_config, "T3F", "display_height", buffer);
	}
	#ifdef ALLEGRO_WINDOWS
		t3f_unload_resources();
		t3f_reload_resources();
	#endif
	pp2_adjust_menus(&instance->interface);
	sprintf(instance->interface.menu_text[0], "%s", fs ? "Full Screen" : "Window");
	sprintf(instance->interface.menu_text[1], "%dx%d", al_get_display_width(t3f_display), al_get_display_height(t3f_display));
	return 1;
}

static bool pp2_check_fullscreen_config(void)
{
	const char * val = al_get_config_value(t3f_config, "T3F", "force_fullscreen");

	if(val)
	{
		if(!strcmp(val, "true"))
		{
			return true;
		}
	}
	return false;
}

static void pp2_get_monitor_size(int * w, int * h)
{
	ALLEGRO_MONITOR_INFO info;

	al_get_monitor_info(0, &info);
	if(w)
	{
		*w = info.x2 - info.x1;
	}
	if(h)
	{
		*h = info.y2 - info.y1;
	}
}

int pp2_menu_proc_resolution_left(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;
	int w = al_get_display_width(t3f_display);
	int mw, mh;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_NEXT], 1.0, 0.0, 1.0);
	pp2_get_monitor_size(&mw, &mh);
	if(t3f_flags & T3F_USE_FULLSCREEN)
	{
		pp2_add_message(pp2_messages, "Resolution cannot be changed in full screen mode.", instance->resources.font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 0.0, 0.0, 1.0), 300, PP2_SCREEN_VISIBLE_WIDTH, 0.0);
	}
	else if(pp2_check_fullscreen_config())
	{
		pp2_add_message(pp2_messages, "Set Display option back to Window or restart.", instance->resources.font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 0.0, 0.0, 1.0), 300, PP2_SCREEN_VISIBLE_WIDTH, 0.0);
	}
	else
	{
		if(w == 1920)
		{
			set_window_size(1440, 810);
		}
		else if(w == 1440)
		{
			set_window_size(960, 540);
		}
		else if(w == 960)
		{
			set_window_size(640, 480);
		}
		else
		{
			if(mw > 1920 && mh > 1080)
			{
				set_window_size(1920, 1080);
			}
			else if(mw > 1440 && mh > 810)
			{
				set_window_size(1440, 810);
			}
			else if(mw > 960 && mh > 540)
			{
				set_window_size(960, 540);
			}
		}
		sprintf(instance->interface.menu_text[1], "%dx%d", al_get_display_width(t3f_display), al_get_display_height(t3f_display));
		pp2_adjust_menus(&instance->interface);
	}
	return 1;
}

int pp2_menu_proc_resolution_right(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;
	int w = al_get_display_width(t3f_display);
	int mw, mh;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_NEXT], 1.0, 0.0, 1.0);
	pp2_get_monitor_size(&mw, &mh);
	if(t3f_flags & T3F_USE_FULLSCREEN)
	{
		pp2_add_message(pp2_messages, "Resolution cannot be changed in full screen mode.", instance->resources.font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 0.0, 0.0, 1.0), 300, PP2_SCREEN_VISIBLE_WIDTH, 0.0);
	}
	else if(pp2_check_fullscreen_config())
	{
		pp2_add_message(pp2_messages, "Set Display option back to Window or restart.", instance->resources.font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 0.0, 0.0, 1.0), 300, PP2_SCREEN_VISIBLE_WIDTH, 0.0);
	}
	else
	{
		if(w == 640 && mw > 960 && mh > 540)
		{
			set_window_size(960, 540);
		}
		else if(w == 960 && mw > 1440 && mh > 810)
		{
			set_window_size(1440, 810);
		}
		else if(w == 1440 && mw > 1920 && mh > 1080)
		{
			set_window_size(1920, 1080);
		}
		else
		{
			set_window_size(640, 480);
		}
		sprintf(instance->interface.menu_text[1], "%dx%d", al_get_display_width(t3f_display), al_get_display_height(t3f_display));
	}
	return 1;
}

int pp2_menu_proc_options_network(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	pp2_select_menu(&instance->interface, PP2_MENU_NETWORK);
	strcpy(pp2_entered_text, pp2_network_id);
	pp2_entering_text = 1;
	pp2_entering_text_pos = strlen(pp2_entered_text);
	t3f_clear_keys();
	instance->interface.menu_joystick_disabled = true;
	return 1;
}

int pp2_menu_proc_network_ok(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	instance->interface.menu_joystick_disabled = false;
	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	pp2_select_previous_menu(&instance->interface);
	strcpy(pp2_network_id, pp2_entered_text);
	al_set_config_value(instance->interface.config, "Network Settings", "ID", pp2_network_id);
	pp2_entering_text = 0;
	return 1;
}

int pp2_menu_proc_main_quit(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	t3f_exit();
	return 1;
}

int pp2_menu_proc_play_quick_play(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	pp2_select_menu(&instance->interface, PP2_MENU_PLAY_QUICK_PLAY);
	return 1;
}

void pp2_generate_custom_game_menu(PP2_INTERFACE * ip, PP2_GAME * gp, PP2_RESOURCES * resources);
void pp2_generate_custom_game_settings_menu(PP2_INTERFACE * ip, PP2_GAME * gp, PP2_RESOURCES * resources);

int pp2_menu_proc_game_type_left(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_NEXT], 1.0, 0.0, 1.0);
	instance->game.option[PP2_OPTION_GAME_MODE]--;
	if(instance->game.option[PP2_OPTION_GAME_MODE] < 0)
	{
		instance->game.option[PP2_OPTION_GAME_MODE] = 2;
	}
	pp2_generate_custom_game_menu(&instance->interface, &instance->game, &instance->resources);
	return 1;
}

int pp2_menu_proc_game_type_right(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_NEXT], 1.0, 0.0, 1.0);
	instance->game.option[PP2_OPTION_GAME_MODE]++;
	if(instance->game.option[PP2_OPTION_GAME_MODE] >= 3)
	{
		instance->game.option[PP2_OPTION_GAME_MODE] = 0;
	}
	pp2_generate_custom_game_menu(&instance->interface, &instance->game, &instance->resources);
	return 1;
}

int pp2_menu_proc_e_hits_left(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;
	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_NEXT], 1.0, 0.0, 1.0);
	if(instance->game.option[PP2_OPTION_ELIMINATION_HITS] > 1)
	{
		instance->game.option[PP2_OPTION_ELIMINATION_HITS]--;
	}
	pp2_generate_custom_game_settings_menu(&instance->interface, &instance->game, &instance->resources);
	return 1;
}

int pp2_menu_proc_e_hits_right(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;
	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_NEXT], 1.0, 0.0, 1.0);
	if(instance->game.option[PP2_OPTION_ELIMINATION_HITS] < 99)
	{
		instance->game.option[PP2_OPTION_ELIMINATION_HITS]++;
	}
	pp2_generate_custom_game_settings_menu(&instance->interface, &instance->game, &instance->resources);
	return 1;
}

int pp2_menu_proc_life_left(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;
	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_NEXT], 1.0, 0.0, 1.0);
	if(instance->game.option[PP2_OPTION_LIFE] > 1)
	{
		instance->game.option[PP2_OPTION_LIFE]--;
	}
	pp2_generate_custom_game_settings_menu(&instance->interface, &instance->game, &instance->resources);
	return 1;
}

int pp2_menu_proc_life_right(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;
	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_NEXT], 1.0, 0.0, 1.0);
	if(instance->game.option[PP2_OPTION_LIFE] < 99)
	{
		instance->game.option[PP2_OPTION_LIFE]++;
	}
	pp2_generate_custom_game_settings_menu(&instance->interface, &instance->game, &instance->resources);
	return 1;
}

int pp2_menu_proc_dm_frags_left(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;
	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_NEXT], 1.0, 0.0, 1.0);
	if(instance->game.option[PP2_OPTION_DEATH_MATCH_FRAGS] > 0)
	{
		instance->game.option[PP2_OPTION_DEATH_MATCH_FRAGS]--;
	}
	pp2_generate_custom_game_settings_menu(&instance->interface, &instance->game, &instance->resources);
	return 1;
}

int pp2_menu_proc_dm_frags_right(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;
	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_NEXT], 1.0, 0.0, 1.0);
	if(instance->game.option[PP2_OPTION_DEATH_MATCH_FRAGS] < 99)
	{
		instance->game.option[PP2_OPTION_DEATH_MATCH_FRAGS]++;
	}
	pp2_generate_custom_game_settings_menu(&instance->interface, &instance->game, &instance->resources);
	return 1;
}

int pp2_menu_proc_time_left(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;
	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_NEXT], 1.0, 0.0, 1.0);
	if(instance->game.option[PP2_OPTION_TIME_LIMIT] > 0)
	{
		instance->game.option[PP2_OPTION_TIME_LIMIT]--;
	}
	pp2_generate_custom_game_settings_menu(&instance->interface, &instance->game, &instance->resources);
	return 1;
}

int pp2_menu_proc_time_right(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;
	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_NEXT], 1.0, 0.0, 1.0);
	if(instance->game.option[PP2_OPTION_TIME_LIMIT] < 99)
	{
		instance->game.option[PP2_OPTION_TIME_LIMIT]++;
	}
	pp2_generate_custom_game_settings_menu(&instance->interface, &instance->game, &instance->resources);
	return 1;
}

static void pp2_menu_update_stock(PP2_INTERFACE * ip, PP2_GAME * gp)
{
	sprintf(ip->menu_text[8], "%d", gp->option[PP2_OPTION_START_AMMO_NORMAL]);
	sprintf(ip->menu_text[9], "%d", gp->option[PP2_OPTION_START_AMMO_X]);
	sprintf(ip->menu_text[10], "%d", gp->option[PP2_OPTION_START_AMMO_MINE]);
	sprintf(ip->menu_text[11], "%d", gp->option[PP2_OPTION_START_AMMO_BOUNCE]);
	sprintf(ip->menu_text[12], "%d", gp->option[PP2_OPTION_START_AMMO_SEEK]);
	sprintf(ip->menu_text[13], "%d", gp->option[PP2_OPTION_START_AMMO_REFLECTOR]);
	sprintf(ip->menu_text[14], "%d", gp->option[PP2_OPTION_START_AMMO_PMINE]);
	sprintf(ip->menu_text[15], "%d", gp->option[PP2_OPTION_START_AMMO_GHOST]);
}

static void pp2_menu_update_ammo(PP2_INTERFACE * ip, PP2_GAME * gp)
{
	sprintf(ip->menu_text[8], "%s", gp->option[PP2_OPTION_ENABLE_AMMO_NORMAL] ? "On" : "Off");
	sprintf(ip->menu_text[9], "%s", gp->option[PP2_OPTION_ENABLE_AMMO_X] ? "On" : "Off");
	sprintf(ip->menu_text[10], "%s", gp->option[PP2_OPTION_ENABLE_AMMO_MINE] ? "On" : "Off");
	sprintf(ip->menu_text[11], "%s", gp->option[PP2_OPTION_ENABLE_AMMO_BOUNCE] ? "On" : "Off");
	sprintf(ip->menu_text[12], "%s", gp->option[PP2_OPTION_ENABLE_AMMO_SEEK] ? "On" : "Off");
	sprintf(ip->menu_text[13], "%s", gp->option[PP2_OPTION_ENABLE_AMMO_REFLECTOR] ? "On" : "Off");
	sprintf(ip->menu_text[14], "%s", gp->option[PP2_OPTION_ENABLE_AMMO_PMINE] ? "On" : "Off");
	sprintf(ip->menu_text[15], "%s", gp->option[PP2_OPTION_ENABLE_AMMO_GHOST] ? "On" : "Off");
}

static void pp2_menu_update_powerups(PP2_INTERFACE * ip, PP2_GAME * gp)
{
	sprintf(ip->menu_text[8], "%s", gp->option[PP2_OPTION_ENABLE_POWERUP_CLOAK] ? "On" : "Off");
	sprintf(ip->menu_text[9], "%s", gp->option[PP2_OPTION_ENABLE_POWERUP_JUMP] ? "On" : "Off");
	sprintf(ip->menu_text[10], "%s", gp->option[PP2_OPTION_ENABLE_POWERUP_RUN] ? "On" : "Off");
	sprintf(ip->menu_text[11], "%s", gp->option[PP2_OPTION_ENABLE_POWERUP_DEFLECT] ? "On" : "Off");
	sprintf(ip->menu_text[12], "%s", gp->option[PP2_OPTION_ENABLE_POWERUP_FLY] ? "On" : "Off");
	sprintf(ip->menu_text[13], "%s", gp->option[PP2_OPTION_ENABLE_POWERUP_TURBO] ? "On" : "Off");
}

int pp2_menu_proc_stock(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	pp2_select_menu(&instance->interface, PP2_MENU_PLAY_STOCK);
	pp2_menu_update_stock(&instance->interface, &instance->game);
	return 1;
}

int pp2_menu_proc_ammo(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	pp2_select_menu(&instance->interface, PP2_MENU_PLAY_AMMO);
	pp2_menu_update_ammo(&instance->interface, &instance->game);
	return 1;
}

int pp2_menu_proc_powerups(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	pp2_select_menu(&instance->interface, PP2_MENU_PLAY_POWERUPS);
	pp2_menu_update_powerups(&instance->interface, &instance->game);
	return 1;
}

static void pp2_menu_option_left(int option, PP2_GAME * gp, PP2_RESOURCES * resources)
{
	t3f_play_sample(resources->sample[PP2_SAMPLE_MENU_NEXT], 1.0, 0.0, 1.0);
	if(gp->option[option] > 0)
	{
		gp->option[option]--;
	}
}

static void pp2_menu_option_right(int option, PP2_GAME * gp, PP2_RESOURCES * resources)
{
	t3f_play_sample(resources->sample[PP2_SAMPLE_MENU_NEXT], 1.0, 0.0, 1.0);
	if(gp->option[option] < 99)
	{
		gp->option[option]++;
	}
}

static void pp2_menu_option_toggle(int option, PP2_GAME * gp, PP2_RESOURCES * resources)
{
	t3f_play_sample(resources->sample[PP2_SAMPLE_MENU_NEXT], 1.0, 0.0, 1.0);
	if(gp->option[option])
	{
		gp->option[option] = 0;
	}
	else
	{
		gp->option[option] = 1;
	}
}

int pp2_menu_proc_normal_stock_left(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	pp2_menu_option_left(PP2_OPTION_START_AMMO_NORMAL, &instance->game, &instance->resources);
	pp2_menu_update_stock(&instance->interface, &instance->game);
	return 1;
}

int pp2_menu_proc_normal_stock_right(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	pp2_menu_option_right(PP2_OPTION_START_AMMO_NORMAL, &instance->game, &instance->resources);
	pp2_menu_update_stock(&instance->interface, &instance->game);
	return 1;
}

int pp2_menu_proc_splitter_stock_left(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	pp2_menu_option_left(PP2_OPTION_START_AMMO_X, &instance->game, &instance->resources);
	pp2_menu_update_stock(&instance->interface, &instance->game);
	return 1;
}

int pp2_menu_proc_splitter_stock_right(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	pp2_menu_option_right(PP2_OPTION_START_AMMO_X, &instance->game, &instance->resources);
	pp2_menu_update_stock(&instance->interface, &instance->game);
	return 1;
}

int pp2_menu_proc_mine_stock_left(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	pp2_menu_option_left(PP2_OPTION_START_AMMO_MINE, &instance->game, &instance->resources);
	pp2_menu_update_stock(&instance->interface, &instance->game);
	return 1;
}

int pp2_menu_proc_mine_stock_right(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	pp2_menu_option_right(PP2_OPTION_START_AMMO_MINE, &instance->game, &instance->resources);
	pp2_menu_update_stock(&instance->interface, &instance->game);
	return 1;
}

int pp2_menu_proc_bouncer_stock_left(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	pp2_menu_option_left(PP2_OPTION_START_AMMO_BOUNCE, &instance->game, &instance->resources);
	pp2_menu_update_stock(&instance->interface, &instance->game);
	return 1;
}

int pp2_menu_proc_bouncer_stock_right(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	pp2_menu_option_right(PP2_OPTION_START_AMMO_BOUNCE, &instance->game, &instance->resources);
	pp2_menu_update_stock(&instance->interface, &instance->game);
	return 1;
}

int pp2_menu_proc_seeker_stock_left(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	pp2_menu_option_left(PP2_OPTION_START_AMMO_SEEK, &instance->game, &instance->resources);
	pp2_menu_update_stock(&instance->interface, &instance->game);
	return 1;
}

int pp2_menu_proc_seeker_stock_right(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	pp2_menu_option_right(PP2_OPTION_START_AMMO_SEEK, &instance->game, &instance->resources);
	pp2_menu_update_stock(&instance->interface, &instance->game);
	return 1;
}

int pp2_menu_proc_reflector_stock_left(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	pp2_menu_option_left(PP2_OPTION_START_AMMO_REFLECTOR, &instance->game, &instance->resources);
	pp2_menu_update_stock(&instance->interface, &instance->game);
	return 1;
}

int pp2_menu_proc_reflector_stock_right(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	pp2_menu_option_right(PP2_OPTION_START_AMMO_REFLECTOR, &instance->game, &instance->resources);
	pp2_menu_update_stock(&instance->interface, &instance->game);
	return 1;
}

int pp2_menu_proc_pmine_stock_left(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	pp2_menu_option_left(PP2_OPTION_START_AMMO_PMINE, &instance->game, &instance->resources);
	pp2_menu_update_stock(&instance->interface, &instance->game);
	return 1;
}

int pp2_menu_proc_pmine_stock_right(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	pp2_menu_option_right(PP2_OPTION_START_AMMO_PMINE, &instance->game, &instance->resources);
	pp2_menu_update_stock(&instance->interface, &instance->game);
	return 1;
}

int pp2_menu_proc_ghost_stock_left(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	pp2_menu_option_left(PP2_OPTION_START_AMMO_GHOST, &instance->game, &instance->resources);
	pp2_menu_update_stock(&instance->interface, &instance->game);
	return 1;
}

int pp2_menu_proc_ghost_stock_right(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	pp2_menu_option_right(PP2_OPTION_START_AMMO_GHOST, &instance->game, &instance->resources);
	pp2_menu_update_stock(&instance->interface, &instance->game);
	return 1;
}

int pp2_menu_proc_normal_ammo_left(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	pp2_menu_option_toggle(PP2_OPTION_ENABLE_AMMO_NORMAL, &instance->game, &instance->resources);
	pp2_menu_update_ammo(&instance->interface, &instance->game);
	return 1;
}

int pp2_menu_proc_normal_ammo_right(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	pp2_menu_option_toggle(PP2_OPTION_ENABLE_AMMO_NORMAL, &instance->game, &instance->resources);
	pp2_menu_update_ammo(&instance->interface, &instance->game);
	return 1;
}

int pp2_menu_proc_splitter_ammo_left(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	pp2_menu_option_toggle(PP2_OPTION_ENABLE_AMMO_X, &instance->game, &instance->resources);
	pp2_menu_update_ammo(&instance->interface, &instance->game);
	return 1;
}

int pp2_menu_proc_splitter_ammo_right(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	pp2_menu_option_toggle(PP2_OPTION_ENABLE_AMMO_X, &instance->game, &instance->resources);
	pp2_menu_update_ammo(&instance->interface, &instance->game);
	return 1;
}

int pp2_menu_proc_mine_ammo_left(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	pp2_menu_option_toggle(PP2_OPTION_ENABLE_AMMO_MINE, &instance->game, &instance->resources);
	pp2_menu_update_ammo(&instance->interface, &instance->game);
	return 1;
}

int pp2_menu_proc_mine_ammo_right(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	pp2_menu_option_toggle(PP2_OPTION_ENABLE_AMMO_MINE, &instance->game, &instance->resources);
	pp2_menu_update_ammo(&instance->interface, &instance->game);
	return 1;
}

int pp2_menu_proc_bouncer_ammo_left(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	pp2_menu_option_toggle(PP2_OPTION_ENABLE_AMMO_BOUNCE, &instance->game, &instance->resources);
	pp2_menu_update_ammo(&instance->interface, &instance->game);
	return 1;
}

int pp2_menu_proc_bouncer_ammo_right(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	pp2_menu_option_toggle(PP2_OPTION_ENABLE_AMMO_BOUNCE, &instance->game, &instance->resources);
	pp2_menu_update_ammo(&instance->interface, &instance->game);
	return 1;
}

int pp2_menu_proc_seeker_ammo_left(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	pp2_menu_option_toggle(PP2_OPTION_ENABLE_AMMO_SEEK, &instance->game, &instance->resources);
	pp2_menu_update_ammo(&instance->interface, &instance->game);
	return 1;
}

int pp2_menu_proc_seeker_ammo_right(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	pp2_menu_option_toggle(PP2_OPTION_ENABLE_AMMO_SEEK, &instance->game, &instance->resources);
	pp2_menu_update_ammo(&instance->interface, &instance->game);
	return 1;
}

int pp2_menu_proc_reflector_ammo_left(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	pp2_menu_option_toggle(PP2_OPTION_ENABLE_AMMO_REFLECTOR, &instance->game, &instance->resources);
	pp2_menu_update_ammo(&instance->interface, &instance->game);
	return 1;
}

int pp2_menu_proc_reflector_ammo_right(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	pp2_menu_option_toggle(PP2_OPTION_ENABLE_AMMO_REFLECTOR, &instance->game, &instance->resources);
	pp2_menu_update_ammo(&instance->interface, &instance->game);
	return 1;
}

int pp2_menu_proc_pmine_ammo_left(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	pp2_menu_option_toggle(PP2_OPTION_ENABLE_AMMO_PMINE, &instance->game, &instance->resources);
	pp2_menu_update_ammo(&instance->interface, &instance->game);
	return 1;
}

int pp2_menu_proc_pmine_ammo_right(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	pp2_menu_option_toggle(PP2_OPTION_ENABLE_AMMO_PMINE, &instance->game, &instance->resources);
	pp2_menu_update_ammo(&instance->interface, &instance->game);
	return 1;
}

int pp2_menu_proc_ghost_ammo_left(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	pp2_menu_option_toggle(PP2_OPTION_ENABLE_AMMO_GHOST, &instance->game, &instance->resources);
	pp2_menu_update_ammo(&instance->interface, &instance->game);
	return 1;
}

int pp2_menu_proc_ghost_ammo_right(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	pp2_menu_option_toggle(PP2_OPTION_ENABLE_AMMO_GHOST, &instance->game, &instance->resources);
	pp2_menu_update_ammo(&instance->interface, &instance->game);
	return 1;
}

int pp2_menu_proc_cloak_power_left(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	pp2_menu_option_toggle(PP2_OPTION_ENABLE_POWERUP_CLOAK, &instance->game, &instance->resources);
	pp2_menu_update_powerups(&instance->interface, &instance->game);
	return 1;
}

int pp2_menu_proc_cloak_power_right(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	pp2_menu_option_toggle(PP2_OPTION_ENABLE_POWERUP_CLOAK, &instance->game, &instance->resources);
	pp2_menu_update_powerups(&instance->interface, &instance->game);
	return 1;
}

int pp2_menu_proc_jump_power_left(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	pp2_menu_option_toggle(PP2_OPTION_ENABLE_POWERUP_JUMP, &instance->game, &instance->resources);
	pp2_menu_update_powerups(&instance->interface, &instance->game);
	return 1;
}

int pp2_menu_proc_jump_power_right(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	pp2_menu_option_toggle(PP2_OPTION_ENABLE_POWERUP_JUMP, &instance->game, &instance->resources);
	pp2_menu_update_powerups(&instance->interface, &instance->game);
	return 1;
}

int pp2_menu_proc_run_power_left(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	pp2_menu_option_toggle(PP2_OPTION_ENABLE_POWERUP_RUN, &instance->game, &instance->resources);
	pp2_menu_update_powerups(&instance->interface, &instance->game);
	return 1;
}

int pp2_menu_proc_run_power_right(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	pp2_menu_option_toggle(PP2_OPTION_ENABLE_POWERUP_RUN, &instance->game, &instance->resources);
	pp2_menu_update_powerups(&instance->interface, &instance->game);
	return 1;
}

int pp2_menu_proc_deflect_power_left(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	pp2_menu_option_toggle(PP2_OPTION_ENABLE_POWERUP_DEFLECT, &instance->game, &instance->resources);
	pp2_menu_update_powerups(&instance->interface, &instance->game);
	return 1;
}

int pp2_menu_proc_deflect_power_right(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	pp2_menu_option_toggle(PP2_OPTION_ENABLE_POWERUP_DEFLECT, &instance->game, &instance->resources);
	pp2_menu_update_powerups(&instance->interface, &instance->game);
	return 1;
}

int pp2_menu_proc_fly_power_left(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	pp2_menu_option_toggle(PP2_OPTION_ENABLE_POWERUP_FLY, &instance->game, &instance->resources);
	pp2_menu_update_powerups(&instance->interface, &instance->game);
	return 1;
}

int pp2_menu_proc_fly_power_right(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	pp2_menu_option_toggle(PP2_OPTION_ENABLE_POWERUP_FLY, &instance->game, &instance->resources);
	pp2_menu_update_powerups(&instance->interface, &instance->game);
	return 1;
}

int pp2_menu_proc_turbo_power_left(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	pp2_menu_option_toggle(PP2_OPTION_ENABLE_POWERUP_TURBO, &instance->game, &instance->resources);
	pp2_menu_update_powerups(&instance->interface, &instance->game);
	return 1;
}

int pp2_menu_proc_turbo_power_right(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	pp2_menu_option_toggle(PP2_OPTION_ENABLE_POWERUP_TURBO, &instance->game, &instance->resources);
	pp2_menu_update_powerups(&instance->interface, &instance->game);
	return 1;
}

int pp2_menu_proc_stomp_toggle(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	pp2_menu_option_toggle(PP2_OPTION_STOMP_HITS, &instance->game, &instance->resources);
	pp2_generate_custom_game_settings_menu(&instance->interface, &instance->game, &instance->resources);
	return 1;
}

int pp2_menu_proc_random_item_toggle(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	pp2_menu_option_toggle(PP2_OPTION_RANDOMIZE_ITEMS, &instance->game, &instance->resources);
	pp2_generate_custom_game_settings_menu(&instance->interface, &instance->game, &instance->resources);
	return 1;
}

void pp2_generate_custom_game_settings_menu(PP2_INTERFACE * ip, PP2_GAME * gp, PP2_RESOURCES * resources)
{
	float ypos = 0.0;
	char text[128] = {0};
	float cx0, cx1, cx2;

	/* calculate column positions (0 = center, 1 = left, 2 = right) */
	cx0 = PP2_SCREEN_WIDTH / 2;
	cx1 = PP2_SCREEN_WIDTH / 4;
	cx2 = PP2_SCREEN_WIDTH - PP2_SCREEN_WIDTH / 4;

	sprintf(ip->menu_text[1], "%s", gp->option[PP2_OPTION_STOMP_HITS] ? "Enabled" : "Disabled");
	sprintf(ip->menu_text[5], "%s", gp->option[PP2_OPTION_RANDOMIZE_ITEMS] ? "Enabled" : "Disabled");
	switch(gp->option[PP2_OPTION_GAME_MODE])
	{
		case PP2_GAME_MODE_ELIMINATOR:
		{
			sprintf(ip->menu_text[2], "Hits: %d", gp->option[PP2_OPTION_ELIMINATION_HITS]);
			break;
		}
		case PP2_GAME_MODE_DEATH_MATCH:
		{
			sprintf(ip->menu_text[2], "Life: %d", gp->option[PP2_OPTION_LIFE]);
			sprintf(text, "%d Minutes", gp->option[PP2_OPTION_TIME_LIMIT]);
			sprintf(ip->menu_text[3], "Time Limit: %s", gp->option[PP2_OPTION_TIME_LIMIT] == 0 ? "None" : text);
			sprintf(text, "%d", gp->option[PP2_OPTION_DEATH_MATCH_FRAGS]);
			sprintf(ip->menu_text[4], "Frag Limit: %s", gp->option[PP2_OPTION_DEATH_MATCH_FRAGS] == 0 ? "None" : text);
			break;
		}
		case PP2_GAME_MODE_COIN_RUSH:
		{
			sprintf(text, "%d Minutes", gp->option[PP2_OPTION_TIME_LIMIT]);
			sprintf(ip->menu_text[3], "Time Limit: %s", gp->option[PP2_OPTION_TIME_LIMIT] == 0 ? "None" : text);
			break;
		}
	}
	if(ip->menu[PP2_MENU_PLAY_SETTINGS])
	{
		t3f_destroy_gui(ip->menu[PP2_MENU_PLAY_SETTINGS]);
	}
	ip->menu[PP2_MENU_PLAY_SETTINGS] = t3f_create_gui(0, 0);
	ip->menu[PP2_MENU_PLAY_SETTINGS]->font_margin_bottom = 6;
	t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_SETTINGS], NULL, "Stomping", (void **)&resources->font[PP2_FONT_COMIC_16], cx0, ypos, PP2_MENU_HEADER_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
	ypos += 24.0;
	t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_SETTINGS], NULL, ip->menu_text[1], (void **)&resources->font[PP2_FONT_COMIC_16], cx0, ypos, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
	t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_SETTINGS], pp2_menu_proc_stomp_toggle, "<", (void **)&resources->font[PP2_FONT_COMIC_16], cx0 - al_get_text_width(resources->font[PP2_FONT_COMIC_16], "              ") / 2 - al_get_text_width(resources->font[PP2_FONT_COMIC_16], "<"), ypos, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
	t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_SETTINGS], pp2_menu_proc_stomp_toggle, ">", (void **)&resources->font[PP2_FONT_COMIC_16], cx0 + al_get_text_width(resources->font[PP2_FONT_COMIC_16], "              ") / 2, ypos, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
	ypos += 48.0;
	t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_SETTINGS], NULL, "Randomize Items", (void **)&resources->font[PP2_FONT_COMIC_16], cx0, ypos, PP2_MENU_HEADER_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
	ypos += 24.0;
	t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_SETTINGS], NULL, ip->menu_text[5], (void **)&resources->font[PP2_FONT_COMIC_16], cx0, ypos, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
	t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_SETTINGS], pp2_menu_proc_random_item_toggle, "<", (void **)&resources->font[PP2_FONT_COMIC_16], cx0 - al_get_text_width(resources->font[PP2_FONT_COMIC_16], "              ") / 2 - al_get_text_width(resources->font[PP2_FONT_COMIC_16], "<"), ypos, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
	t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_SETTINGS], pp2_menu_proc_random_item_toggle, ">", (void **)&resources->font[PP2_FONT_COMIC_16], cx0 + al_get_text_width(resources->font[PP2_FONT_COMIC_16], "              ") / 2, ypos, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
	ypos += 48.0;
	switch(gp->option[PP2_OPTION_GAME_MODE])
	{
		case PP2_GAME_MODE_ELIMINATOR:
		{
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_SETTINGS], NULL, ip->menu_text[2], (void **)&resources->font[PP2_FONT_COMIC_16], cx0, ypos, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_SETTINGS], pp2_menu_proc_e_hits_left, "<", (void **)&resources->font[PP2_FONT_COMIC_16], cx0 - al_get_text_width(resources->font[PP2_FONT_COMIC_16], "              ") / 2 - al_get_text_width(resources->font[PP2_FONT_COMIC_16], "<"), ypos, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_SETTINGS], pp2_menu_proc_e_hits_right, ">", (void **)&resources->font[PP2_FONT_COMIC_16], cx0 + al_get_text_width(resources->font[PP2_FONT_COMIC_16], "              ") / 2, ypos, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			ypos += 48.0;
			break;
		}
		case PP2_GAME_MODE_DEATH_MATCH:
		{
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_SETTINGS], NULL, ip->menu_text[2], (void **)&resources->font[PP2_FONT_COMIC_16], cx0, ypos, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_SETTINGS], pp2_menu_proc_life_left, "<", (void **)&resources->font[PP2_FONT_COMIC_16], cx0 - al_get_text_width(resources->font[PP2_FONT_COMIC_16], "              ") / 2 - al_get_text_width(resources->font[PP2_FONT_COMIC_16], "<"), ypos, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_SETTINGS], pp2_menu_proc_life_right, ">", (void **)&resources->font[PP2_FONT_COMIC_16], cx0 + al_get_text_width(resources->font[PP2_FONT_COMIC_16], "              ") / 2, ypos, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			ypos += 24.0;
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_SETTINGS], NULL, ip->menu_text[3], (void **)&resources->font[PP2_FONT_COMIC_16], cx0, ypos, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_SETTINGS], pp2_menu_proc_time_left, "<", (void **)&resources->font[PP2_FONT_COMIC_16], cx0 - al_get_text_width(resources->font[PP2_FONT_COMIC_16], "              ") / 2 - al_get_text_width(resources->font[PP2_FONT_COMIC_16], "<"), ypos, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_SETTINGS], pp2_menu_proc_time_right, ">", (void **)&resources->font[PP2_FONT_COMIC_16], cx0 + al_get_text_width(resources->font[PP2_FONT_COMIC_16], "              ") / 2, ypos, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			ypos += 24.0;
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_SETTINGS], NULL, ip->menu_text[4], (void **)&resources->font[PP2_FONT_COMIC_16], cx0, ypos, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_SETTINGS], pp2_menu_proc_dm_frags_left, "<", (void **)&resources->font[PP2_FONT_COMIC_16], cx0 - al_get_text_width(resources->font[PP2_FONT_COMIC_16], "              ") / 2 - al_get_text_width(resources->font[PP2_FONT_COMIC_16], "<"), ypos, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_SETTINGS], pp2_menu_proc_dm_frags_right, ">", (void **)&resources->font[PP2_FONT_COMIC_16], cx0 + al_get_text_width(resources->font[PP2_FONT_COMIC_16], "              ") / 2, ypos, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			ypos += 48.0;
			break;
		}
		case PP2_GAME_MODE_COIN_RUSH:
		{
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_SETTINGS], NULL, ip->menu_text[3], (void **)&resources->font[PP2_FONT_COMIC_16], cx0, ypos, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_SETTINGS], pp2_menu_proc_time_left, "<", (void **)&resources->font[PP2_FONT_COMIC_16], cx0 - al_get_text_width(resources->font[PP2_FONT_COMIC_16], "              ") / 2 - al_get_text_width(resources->font[PP2_FONT_COMIC_16], "<"), ypos, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_SETTINGS], pp2_menu_proc_time_right, ">", (void **)&resources->font[PP2_FONT_COMIC_16], cx0 + al_get_text_width(resources->font[PP2_FONT_COMIC_16], "              ") / 2, ypos, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			ypos += 48.0;
			break;
		}
		default:
		{
//			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_CUSTOM], NULL, ip->menu_text[1], (void **)&resources->font[PP2_FONT_COMIC_16], 320, ypos, al_map_rgba_f(1.0, 1.0, 1.0, 1.0), T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
//			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_CUSTOM], pp2_menu_proc_e_hits_left, "<", (void **)&resources->font[PP2_FONT_COMIC_16], 320 - al_get_text_width(resources->font[PP2_FONT_COMIC_16], "              ") / 2 - al_get_text_width(resources->font[PP2_FONT_COMIC_16], "<"), ypos, al_map_rgba_f(1.0, 1.0, 1.0, 1.0), T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
//			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_CUSTOM], pp2_menu_proc_e_hits_right, ">", (void **)&resources->font[PP2_FONT_COMIC_16], 320 + al_get_text_width(resources->font[PP2_FONT_COMIC_16], "              ") / 2, ypos, al_map_rgba_f(1.0, 1.0, 1.0, 1.0), T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
//			ypos += 48.0;
			break;
		}
	}
	t3f_center_gui(ip->menu[PP2_MENU_PLAY_SETTINGS], 200.0, 456.0);
	t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_SETTINGS], pp2_menu_proc_overlay_back, "< Back", (void **)&resources->font[PP2_FONT_COMIC_16], t3f_default_view->left, t3f_default_view->bottom - al_get_font_line_height(resources->font[PP2_FONT_COMIC_16]) - ip->menu[PP2_MENU_PLAY_SETTINGS]->oy, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW);
}

int pp2_menu_proc_settings(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	pp2_generate_custom_game_settings_menu(&instance->interface, &instance->game, &instance->resources);
	pp2_select_menu(&instance->interface, PP2_MENU_PLAY_SETTINGS);
	return 1;
}

void pp2_generate_custom_game_menu(PP2_INTERFACE * ip, PP2_GAME * gp, PP2_RESOURCES * resources)
{
	float ypos = 0.0;
	int hover = -1;
	float cx0, cx1, cx2;

	/* calculate column positions (0 = center, 1 = left, 2 = right) */
	cx0 = PP2_SCREEN_WIDTH / 2;
	cx1 = PP2_SCREEN_WIDTH / 4;
	cx2 = PP2_SCREEN_WIDTH - PP2_SCREEN_WIDTH / 4;

	if(gp->option[PP2_OPTION_GAME_MODE] == PP2_GAME_MODE_EXPLORE)
	{
		gp->option[PP2_OPTION_GAME_MODE] = 0;
	}
	/* fill in custom text */
	sprintf(ip->menu_text[0], "%s", pp2_game_mode_text[gp->option[PP2_OPTION_GAME_MODE]]);

	/* create the menu */
	if(ip->menu[PP2_MENU_PLAY_CUSTOM])
	{
		hover = ip->menu[PP2_MENU_PLAY_CUSTOM]->hover_element;
		t3f_destroy_gui(ip->menu[PP2_MENU_PLAY_CUSTOM]);
	}
	ip->menu[PP2_MENU_PLAY_CUSTOM] = t3f_create_gui(0, 0);
	t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_CUSTOM], NULL, "Game Mode", (void **)&resources->font[PP2_FONT_COMIC_16], cx0, ypos, PP2_MENU_HEADER_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
	ypos += 24.0;
	t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_CUSTOM], NULL, ip->menu_text[0], (void **)&resources->font[PP2_FONT_COMIC_16], cx0, ypos, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
	t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_CUSTOM], pp2_menu_proc_game_type_left, "<", (void **)&resources->font[PP2_FONT_COMIC_16], cx0 - al_get_text_width(resources->font[PP2_FONT_COMIC_16], "              ") / 2 - al_get_text_width(resources->font[PP2_FONT_COMIC_16], "<"), ypos, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
	t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_CUSTOM], pp2_menu_proc_game_type_right, ">", (void **)&resources->font[PP2_FONT_COMIC_16], cx0 + al_get_text_width(resources->font[PP2_FONT_COMIC_16], "              ") / 2, ypos, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
	ypos += 48.0;
	t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_CUSTOM], pp2_menu_proc_settings, "Settings", (void **)&resources->font[PP2_FONT_COMIC_16], cx0, ypos, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
	ypos += 24.0;
	t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_CUSTOM], pp2_menu_proc_stock, "Stock", (void **)&resources->font[PP2_FONT_COMIC_16], cx0, ypos, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
	ypos += 24.0;
	t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_CUSTOM], pp2_menu_proc_ammo, "Ammo", (void **)&resources->font[PP2_FONT_COMIC_16], cx0, ypos, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
	ypos += 24.0;
	t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_CUSTOM], pp2_menu_proc_powerups, "Power-Ups", (void **)&resources->font[PP2_FONT_COMIC_16], cx0, ypos, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
	ypos += 48.0;
	t3f_center_gui(ip->menu[PP2_MENU_PLAY_CUSTOM], 200.0, 456.0);
	t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_CUSTOM], pp2_menu_proc_overlay_back, "< Back", (void **)&resources->font[PP2_FONT_COMIC_16], t3f_default_view->left, t3f_default_view->bottom - al_get_font_line_height(resources->font[PP2_FONT_COMIC_16]) - ip->menu[PP2_MENU_PLAY_CUSTOM]->oy, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW);
	t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_CUSTOM], pp2_menu_proc_overlay_next, "Next >", (void **)&resources->font[PP2_FONT_COMIC_16], t3f_default_view->right - al_get_text_width(resources->font[PP2_FONT_COMIC_16], "Next >"), t3f_default_view->bottom - al_get_font_line_height(resources->font[PP2_FONT_COMIC_16]) - ip->menu[PP2_MENU_PLAY_CUSTOM]->oy, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW);
	ip->menu[PP2_MENU_PLAY_CUSTOM]->hover_element = hover;
}

int pp2_menu_proc_play_custom(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	pp2_generate_custom_game_menu(&instance->interface, &instance->game, &instance->resources);
	pp2_select_menu(&instance->interface, PP2_MENU_PLAY_CUSTOM);
//	pp2_add_message(pp2_messages, "Custom games not available in this demo.", (void **)&resources->font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 0.0, 0.0, 1.0), 300, PP2_SCREEN_VISIBLE_WIDTH, 0.0);
	return 1;
}

static void pp2_menu_get_level_preview(PP2_INTERFACE * ip, PP2_RESOURCES * resources)
{
	int entry;
	int i;

	entry = pp2_database_find_entry(resources->level_database, ip->level_hash);
	if(entry >= 0)
	{
		ip->level_preview = pp2_load_level_preview(((PP2_LEVEL_DATABASE_EXTRA *)resources->level_database->entry[entry]->extra)->preview);
		if(!ip->level_preview)
		{
			printf("Could not load preview!\n");
			return;
		}
		for(i = 0; i < pp2_client_game->content_list[PP2_CONTENT_LEVELS]->count; i++)
		{
			if(pp2_client_game->content_list[PP2_CONTENT_LEVELS]->hash[i] == ip->level_hash)
			{
				ip->level_choosing = i;
				break;
			}
		}
	}
	else
	{
		ip->level_choosing = 0;
		ip->level_preview = pp2_load_level_preview(((PP2_LEVEL_DATABASE_EXTRA *)resources->level_database->entry[0]->extra)->preview);
	}
}

int pp2_menu_proc_play_1_hit(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	instance->game.option[PP2_OPTION_GAME_MODE] = PP2_GAME_MODE_ELIMINATOR;
	instance->game.option[PP2_OPTION_RANDOMIZE_ITEMS] = 0;
	instance->game.option[PP2_OPTION_ELIMINATION_HITS] = 1;
	instance->game.option[PP2_OPTION_ENABLE_AMMO_NORMAL] = 0;
	instance->game.option[PP2_OPTION_ENABLE_AMMO_X] = 0;
	instance->game.option[PP2_OPTION_ENABLE_AMMO_MINE] = 0;
	instance->game.option[PP2_OPTION_ENABLE_AMMO_BOUNCE] = 0;
	instance->game.option[PP2_OPTION_ENABLE_AMMO_SEEK] = 0;
	instance->game.option[PP2_OPTION_ENABLE_AMMO_REFLECTOR] = 0;
	instance->game.option[PP2_OPTION_ENABLE_AMMO_PMINE] = 0;
	instance->game.option[PP2_OPTION_ENABLE_AMMO_GHOST] = 0;
	instance->game.option[PP2_OPTION_ENABLE_POWERUP_CLOAK] = 1;
	instance->game.option[PP2_OPTION_ENABLE_POWERUP_RUN] = 1;
	instance->game.option[PP2_OPTION_ENABLE_POWERUP_JUMP] = 1;
	instance->game.option[PP2_OPTION_ENABLE_POWERUP_DEFLECT] = 1;
	instance->game.option[PP2_OPTION_ENABLE_POWERUP_FLY] = 1;
	instance->game.option[PP2_OPTION_ENABLE_POWERUP_TURBO] = 1;
	instance->game.option[PP2_OPTION_START_AMMO_NORMAL] = 1;
	instance->game.option[PP2_OPTION_START_AMMO_X] = 1;
	instance->game.option[PP2_OPTION_START_AMMO_MINE] = 1;
	instance->game.option[PP2_OPTION_START_AMMO_BOUNCE] = 1;
	instance->game.option[PP2_OPTION_START_AMMO_SEEK] = 1;
	instance->game.option[PP2_OPTION_START_AMMO_REFLECTOR] = 1;
	instance->game.option[PP2_OPTION_START_AMMO_PMINE] = 1;
	instance->game.option[PP2_OPTION_START_AMMO_GHOST] = 1;
	instance->game.option[PP2_OPTION_STOMP_HITS] = 0;
	pp2_menu_get_level_preview(&instance->interface, &instance->resources);
	instance->state = PP2_STATE_LEVEL_SETUP;
	return 1;
}

int pp2_menu_proc_play_21_stomp(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	instance->game.option[PP2_OPTION_GAME_MODE] = PP2_GAME_MODE_DEATH_MATCH;
	instance->game.option[PP2_OPTION_RANDOMIZE_ITEMS] = 0;
	instance->game.option[PP2_OPTION_TIME_LIMIT] = 2;
	instance->game.option[PP2_OPTION_LIFE] = 1;
	instance->game.option[PP2_OPTION_DEATH_MATCH_FRAGS] = 21;
	instance->game.option[PP2_OPTION_ENABLE_AMMO_NORMAL] = 0;
	instance->game.option[PP2_OPTION_ENABLE_AMMO_X] = 0;
	instance->game.option[PP2_OPTION_ENABLE_AMMO_MINE] = 0;
	instance->game.option[PP2_OPTION_ENABLE_AMMO_BOUNCE] = 0;
	instance->game.option[PP2_OPTION_ENABLE_AMMO_SEEK] = 0;
	instance->game.option[PP2_OPTION_ENABLE_AMMO_REFLECTOR] = 0;
	instance->game.option[PP2_OPTION_ENABLE_AMMO_PMINE] = 0;
	instance->game.option[PP2_OPTION_ENABLE_AMMO_GHOST] = 0;
	instance->game.option[PP2_OPTION_ENABLE_POWERUP_CLOAK] = 1;
	instance->game.option[PP2_OPTION_ENABLE_POWERUP_RUN] = 1;
	instance->game.option[PP2_OPTION_ENABLE_POWERUP_JUMP] = 1;
	instance->game.option[PP2_OPTION_ENABLE_POWERUP_DEFLECT] = 0;
	instance->game.option[PP2_OPTION_ENABLE_POWERUP_FLY] = 1;
	instance->game.option[PP2_OPTION_ENABLE_POWERUP_TURBO] = 0;
	instance->game.option[PP2_OPTION_START_AMMO_NORMAL] = 0;
	instance->game.option[PP2_OPTION_START_AMMO_X] = 0;
	instance->game.option[PP2_OPTION_START_AMMO_MINE] = 0;
	instance->game.option[PP2_OPTION_START_AMMO_BOUNCE] = 0;
	instance->game.option[PP2_OPTION_START_AMMO_SEEK] = 0;
	instance->game.option[PP2_OPTION_START_AMMO_REFLECTOR] = 0;
	instance->game.option[PP2_OPTION_START_AMMO_PMINE] = 0;
	instance->game.option[PP2_OPTION_START_AMMO_GHOST] = 0;
	instance->game.option[PP2_OPTION_STOMP_HITS] = 1;
	pp2_menu_get_level_preview(&instance->interface, &instance->resources);
	instance->state = PP2_STATE_LEVEL_SETUP;
	return 1;
}

int pp2_menu_proc_play_death_match(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	instance->game.option[PP2_OPTION_GAME_MODE] = PP2_GAME_MODE_DEATH_MATCH;
	instance->game.option[PP2_OPTION_RANDOMIZE_ITEMS] = 0;
	instance->game.option[PP2_OPTION_TIME_LIMIT] = 2;
	instance->game.option[PP2_OPTION_LIFE] = 1;
	instance->game.option[PP2_OPTION_DEATH_MATCH_FRAGS] = 20;
	instance->game.option[PP2_OPTION_ENABLE_AMMO_NORMAL] = 1;
	instance->game.option[PP2_OPTION_ENABLE_AMMO_X] = 1;
	instance->game.option[PP2_OPTION_ENABLE_AMMO_MINE] = 1;
	instance->game.option[PP2_OPTION_ENABLE_AMMO_BOUNCE] = 1;
	instance->game.option[PP2_OPTION_ENABLE_AMMO_SEEK] = 1;
	instance->game.option[PP2_OPTION_ENABLE_AMMO_REFLECTOR] = 1;
	instance->game.option[PP2_OPTION_ENABLE_AMMO_PMINE] = 1;
	instance->game.option[PP2_OPTION_ENABLE_AMMO_GHOST] = 1;
	instance->game.option[PP2_OPTION_ENABLE_POWERUP_CLOAK] = 1;
	instance->game.option[PP2_OPTION_ENABLE_POWERUP_RUN] = 1;
	instance->game.option[PP2_OPTION_ENABLE_POWERUP_JUMP] = 1;
	instance->game.option[PP2_OPTION_ENABLE_POWERUP_DEFLECT] = 1;
	instance->game.option[PP2_OPTION_ENABLE_POWERUP_FLY] = 1;
	instance->game.option[PP2_OPTION_ENABLE_POWERUP_TURBO] = 1;
	instance->game.option[PP2_OPTION_START_AMMO_NORMAL] = 1;
	instance->game.option[PP2_OPTION_START_AMMO_X] = 1;
	instance->game.option[PP2_OPTION_START_AMMO_MINE] = 1;
	instance->game.option[PP2_OPTION_START_AMMO_BOUNCE] = 1;
	instance->game.option[PP2_OPTION_START_AMMO_SEEK] = 1;
	instance->game.option[PP2_OPTION_START_AMMO_REFLECTOR] = 1;
	instance->game.option[PP2_OPTION_START_AMMO_PMINE] = 1;
	instance->game.option[PP2_OPTION_START_AMMO_GHOST] = 1;
	instance->game.option[PP2_OPTION_STOMP_HITS] = 0;
	instance->game.option[PP2_OPTION_AMMO_WORTH] = 1;
	pp2_menu_get_level_preview(&instance->interface, &instance->resources);
	instance->state = PP2_STATE_LEVEL_SETUP;
	return 1;
}

int pp2_menu_proc_play_coin_rush(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	instance->game.option[PP2_OPTION_GAME_MODE] = PP2_GAME_MODE_COIN_RUSH;
	instance->game.option[PP2_OPTION_RANDOMIZE_ITEMS] = 0;
	instance->game.option[PP2_OPTION_TIME_LIMIT] = 2;
	instance->game.option[PP2_OPTION_LIFE] = 1;
	instance->game.option[PP2_OPTION_ENABLE_AMMO_NORMAL] = 1;
	instance->game.option[PP2_OPTION_ENABLE_AMMO_X] = 1;
	instance->game.option[PP2_OPTION_ENABLE_AMMO_MINE] = 1;
	instance->game.option[PP2_OPTION_ENABLE_AMMO_BOUNCE] = 1;
	instance->game.option[PP2_OPTION_ENABLE_AMMO_SEEK] = 1;
	instance->game.option[PP2_OPTION_ENABLE_AMMO_REFLECTOR] = 1;
	instance->game.option[PP2_OPTION_ENABLE_AMMO_PMINE] = 1;
	instance->game.option[PP2_OPTION_ENABLE_AMMO_GHOST] = 1;
	instance->game.option[PP2_OPTION_ENABLE_POWERUP_CLOAK] = 1;
	instance->game.option[PP2_OPTION_ENABLE_POWERUP_RUN] = 1;
	instance->game.option[PP2_OPTION_ENABLE_POWERUP_JUMP] = 1;
	instance->game.option[PP2_OPTION_ENABLE_POWERUP_DEFLECT] = 1;
	instance->game.option[PP2_OPTION_ENABLE_POWERUP_FLY] = 1;
	instance->game.option[PP2_OPTION_ENABLE_POWERUP_TURBO] = 1;
	instance->game.option[PP2_OPTION_START_AMMO_NORMAL] = 1;
	instance->game.option[PP2_OPTION_START_AMMO_X] = 1;
	instance->game.option[PP2_OPTION_START_AMMO_MINE] = 1;
	instance->game.option[PP2_OPTION_START_AMMO_BOUNCE] = 1;
	instance->game.option[PP2_OPTION_START_AMMO_SEEK] = 1;
	instance->game.option[PP2_OPTION_START_AMMO_REFLECTOR] = 1;
	instance->game.option[PP2_OPTION_START_AMMO_PMINE] = 1;
	instance->game.option[PP2_OPTION_START_AMMO_GHOST] = 1;
	instance->game.option[PP2_OPTION_STOMP_HITS] = 1;
	pp2_menu_get_level_preview(&instance->interface, &instance->resources);
	instance->state = PP2_STATE_LEVEL_SETUP;
	return 1;
}

int pp2_menu_proc_play_battle_royale(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	instance->game.option[PP2_OPTION_GAME_MODE] = PP2_GAME_MODE_ELIMINATOR;
	instance->game.option[PP2_OPTION_RANDOMIZE_ITEMS] = 1;
	instance->game.option[PP2_OPTION_ELIMINATION_HITS] = 1;
	instance->game.option[PP2_OPTION_ENABLE_AMMO_NORMAL] = 1;
	instance->game.option[PP2_OPTION_ENABLE_AMMO_X] = 1;
	instance->game.option[PP2_OPTION_ENABLE_AMMO_MINE] = 1;
	instance->game.option[PP2_OPTION_ENABLE_AMMO_BOUNCE] = 1;
	instance->game.option[PP2_OPTION_ENABLE_AMMO_SEEK] = 1;
	instance->game.option[PP2_OPTION_ENABLE_AMMO_REFLECTOR] = 1;
	instance->game.option[PP2_OPTION_ENABLE_AMMO_PMINE] = 1;
	instance->game.option[PP2_OPTION_ENABLE_AMMO_GHOST] = 1;
	instance->game.option[PP2_OPTION_ENABLE_POWERUP_CLOAK] = 1;
	instance->game.option[PP2_OPTION_ENABLE_POWERUP_RUN] = 1;
	instance->game.option[PP2_OPTION_ENABLE_POWERUP_JUMP] = 1;
	instance->game.option[PP2_OPTION_ENABLE_POWERUP_DEFLECT] = 1;
	instance->game.option[PP2_OPTION_ENABLE_POWERUP_FLY] = 1;
	instance->game.option[PP2_OPTION_ENABLE_POWERUP_TURBO] = 1;
	instance->game.option[PP2_OPTION_START_AMMO_NORMAL] = 0;
	instance->game.option[PP2_OPTION_START_AMMO_X] = 0;
	instance->game.option[PP2_OPTION_START_AMMO_MINE] = 0;
	instance->game.option[PP2_OPTION_START_AMMO_BOUNCE] = 0;
	instance->game.option[PP2_OPTION_START_AMMO_SEEK] = 0;
	instance->game.option[PP2_OPTION_START_AMMO_REFLECTOR] = 0;
	instance->game.option[PP2_OPTION_START_AMMO_PMINE] = 0;
	instance->game.option[PP2_OPTION_START_AMMO_GHOST] = 0;
	instance->game.option[PP2_OPTION_STOMP_HITS] = 0;
	pp2_menu_get_level_preview(&instance->interface, &instance->resources);
	instance->state = PP2_STATE_LEVEL_SETUP;
	return 1;
}

int pp2_menu_proc_play_explore(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	instance->game.option[PP2_OPTION_GAME_MODE] = PP2_GAME_MODE_EXPLORE;
	instance->game.option[PP2_OPTION_RANDOMIZE_ITEMS] = 0;
	instance->game.option[PP2_OPTION_ENABLE_AMMO_NORMAL] = 1;
	instance->game.option[PP2_OPTION_ENABLE_AMMO_X] = 1;
	instance->game.option[PP2_OPTION_ENABLE_AMMO_MINE] = 1;
	instance->game.option[PP2_OPTION_ENABLE_AMMO_BOUNCE] = 1;
	instance->game.option[PP2_OPTION_ENABLE_AMMO_SEEK] = 1;
	instance->game.option[PP2_OPTION_ENABLE_AMMO_REFLECTOR] = 1;
	instance->game.option[PP2_OPTION_ENABLE_AMMO_PMINE] = 1;
	instance->game.option[PP2_OPTION_ENABLE_AMMO_GHOST] = 1;
	instance->game.option[PP2_OPTION_ENABLE_POWERUP_CLOAK] = 1;
	instance->game.option[PP2_OPTION_ENABLE_POWERUP_RUN] = 1;
	instance->game.option[PP2_OPTION_ENABLE_POWERUP_JUMP] = 1;
	instance->game.option[PP2_OPTION_ENABLE_POWERUP_DEFLECT] = 1;
	instance->game.option[PP2_OPTION_ENABLE_POWERUP_FLY] = 1;
	instance->game.option[PP2_OPTION_ENABLE_POWERUP_TURBO] = 1;
	instance->game.option[PP2_OPTION_START_AMMO_NORMAL] = 99;
	instance->game.option[PP2_OPTION_START_AMMO_X] = 99;
	instance->game.option[PP2_OPTION_START_AMMO_MINE] = 99;
	instance->game.option[PP2_OPTION_START_AMMO_BOUNCE] = 99;
	instance->game.option[PP2_OPTION_START_AMMO_SEEK] = 99;
	instance->game.option[PP2_OPTION_START_AMMO_REFLECTOR] = 99;
	instance->game.option[PP2_OPTION_START_AMMO_PMINE] = 99;
	instance->game.option[PP2_OPTION_START_AMMO_GHOST] = 99;
	pp2_menu_get_level_preview(&instance->interface, &instance->resources);
	instance->state = PP2_STATE_LEVEL_SETUP;
	return 1;
}

/* in-game menus */
int pp2_menu_proc_game_resume(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	joynet_resume_game(pp2_client_game);
	return 1;
}

int pp2_menu_proc_game_end(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	instance->game.end_game_option = PP2_END_GAME_QUIT;
	joynet_update_game_option(pp2_client_game, &instance->game.end_game_option);
	joynet_end_game(pp2_client_game);
	return 1;
}

int pp2_menu_proc_game_rematch(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	instance->game.end_game_option = PP2_END_GAME_REMATCH;
	joynet_update_game_option(pp2_client_game, &instance->game.end_game_option);
	joynet_end_game(pp2_client_game);
	return 1;
}

int pp2_menu_proc_game_play_again(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	instance->game.end_game_option = PP2_END_GAME_PLAY_AGAIN;
	joynet_update_game_option(pp2_client_game, &instance->game.end_game_option);
	joynet_end_game(pp2_client_game);
	return 1;
}

int pp2_menu_proc_game_new_game(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	instance->game.end_game_option = PP2_END_GAME_NEW;
	joynet_update_game_option(pp2_client_game, &instance->game.end_game_option);
	joynet_end_game(pp2_client_game);
	return 1;
}

int pp2_menu_proc_game_quit(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	instance->game.end_game_option = PP2_END_GAME_QUIT;
	joynet_update_game_option(pp2_client_game, &instance->game.end_game_option);
	joynet_end_game(pp2_client_game);
	return 1;
}

int pp2_menu_proc_new_profile_ok(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	pp2_entering_text = 0;
	instance->interface.menu_joystick_disabled = false;
	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	pp2_add_profile(&instance->interface.profiles, pp2_entered_text);
	instance->state = PP2_STATE_PLAYER_SETUP;
	pp2_select_previous_menu(&instance->interface);
	return 1;
}

int pp2_menu_proc_overlay_back(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;

	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	switch(instance->state)
	{
		case PP2_STATE_MENU:
		{
			switch(instance->interface.current_menu)
			{
				case PP2_MENU_PLAY:
				case PP2_MENU_PLAY_SINGLE:
				{
					instance->state = PP2_STATE_PLAYER_SETUP;
					break;
				}
				case PP2_MENU_NEW_PROFILE:
				{
					instance->state = PP2_STATE_PLAYER_SETUP;
					pp2_select_previous_menu(&instance->interface);
					break;
				}
				default:
				{
					pp2_select_previous_menu(&instance->interface);
					break;
				}
			}
			break;
		}
		case PP2_STATE_PLAYER_SETUP:
		{
			while(instance->interface.menu_stack_size > 0)
			{
				pp2_select_previous_menu(&instance->interface);
			}
			instance->state = PP2_STATE_MENU;
			break;
		}
		case PP2_STATE_LEVEL_SETUP:
		{
			instance->state = PP2_STATE_MENU;
			break;
		}
	}
	return 1;
}

int pp2_menu_proc_overlay_next(void * data, int i, void * p)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;
	t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
	switch(instance->state)
	{
		case PP2_STATE_PLAYER_SETUP:
		{
			if(pp2_client_game->player_count > 1)
			{
				if(pp2_client || pp2_level_setup_players_ready(&instance->game))
				{
					pp2_select_menu(&instance->interface, PP2_MENU_PLAY);
					instance->state = PP2_STATE_MENU;
				}
				else
				{
					pp2_add_message(pp2_messages, "Players still making selections.", instance->resources.font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 0.0, 0.0, 1.0), 300, PP2_SCREEN_VISIBLE_WIDTH, 0.0);
				}
			}
			else
			{
				if(pp2_client || pp2_level_setup_players_ready(&instance->game))
				{
					pp2_select_menu(&instance->interface, PP2_MENU_PLAY_SINGLE);
					instance->state = PP2_STATE_MENU;
				}
				else
				{
					pp2_add_message(pp2_messages, "Players still making selections.", instance->resources.font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 0.0, 0.0, 1.0), 300, PP2_SCREEN_VISIBLE_WIDTH, 0.0);
				}
			}
			break;
		}
		case PP2_STATE_LEVEL_SETUP:
		{
			/* level_choosing is -1 until the hash goes through the network and makes the new selection */
			if(instance->interface.level_chosen && pp2_level_setup_players_ready(&instance->game))
			{
				if(instance->interface.level_preview->players < pp2_client_game->player_count)
				{
					pp2_add_message(pp2_messages, "Too many players for the selected level.", instance->resources.font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 0.0, 0.0, 1.0), 300, PP2_SCREEN_VISIBLE_WIDTH, 0.0);
				}
				else
				{
					joynet_select_game_content(pp2_client_game, 0, PP2_CONTENT_LEVELS, pp2_client_game->content_list[PP2_CONTENT_LEVELS]->hash[instance->interface.level_choosing]);
					instance->game.seed = time(0);
					joynet_start_game(pp2_client_game);
				}
			}
			else
			{
				pp2_add_message(pp2_messages, "Players still making selections.", instance->resources.font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 0.0, 0.0, 1.0), 300, PP2_SCREEN_VISIBLE_WIDTH, 0.0);
			}
			break;
		}
		case PP2_STATE_MENU:
		{
			if(instance->game.option[PP2_OPTION_GAME_MODE] != PP2_GAME_MODE_ELIMINATOR && instance->game.option[PP2_OPTION_GAME_MODE] != PP2_GAME_MODE_DEATH_MATCH && instance->game.option[PP2_OPTION_GAME_MODE] != PP2_GAME_MODE_COIN_RUSH)
			{
				pp2_add_message(pp2_messages, "Game mode not available in this demo.", instance->resources.font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 0.0, 0.0, 1.0), 300, PP2_SCREEN_VISIBLE_WIDTH, 0.0);
			}
			else
			{
				instance->state = PP2_STATE_LEVEL_SETUP;
			}
			break;
		}
	}
	return 1;
}
