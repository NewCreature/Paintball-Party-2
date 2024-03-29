#ifndef PP2_INTERFACE_H
#define PP2_INTERFACE_H

#include "../file/profile.h"
#include "preview.h"
#include "credits_struct.h"
#include "message.h"

typedef struct
{

	ALLEGRO_CONFIG * config;
	T3F_INPUT_HANDLER * input_handler[PP2_MAX_PLAYERS]; // up to four local controllers
	PP2_CREDITS credits;
	PP2_MESSAGE_LIST * messages;
	T3F_GUI * menu[PP2_MAX_MENUS];
	char menu_text[32][64];
	float menu_offset;
	char controller_binding_text[11][128];
	int current_menu;
	int previous_menu[PP2_MAX_MENU_STACK];
	int menu_stack_size;
	int menu_selected_controller;
	bool menu_joystick_disabled;
	int menu_joystick_skip;
	bool joystick_menu_activation;

	char network_id[256];
	char network_ip[256];
	char server_name[256];
	bool client_disconnected;
	int lan_arg;
	float sound_volume;
	PP2_PROFILE_LIST profiles;
	int selected_profile;
	PP2_CHARACTER_PREVIEW * player_preview[PP2_MAX_PLAYERS];
	PP2_LEVEL_PREVIEW * level_preview;
	ALLEGRO_FILECHOOSER * replay_filechooser;
	int replay_file_number;
	int level_choice;
	int level_choosing;
	int level_chosen;
	unsigned long level_hash;
	int setting[PP2_MAX_SETTINGS];
	bool regenerate_cache;
	bool use_ffmpeg;
	unsigned long tick;
	int next_state;

} PP2_INTERFACE;

#endif
