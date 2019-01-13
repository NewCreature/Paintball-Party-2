#ifndef PP2_DATA_H
#define PP2_DATA_H

#include <allegro5/allegro_native_dialog.h>
#include "t3f/t3f.h"
#include "t3f/controller.h"
#include "t3f/gui.h"
#include "joynet/joynet.h"
#include "t3net/server_list.h"
#include "file/database/database.h"
#include "file/profile.h"
#include "file/level.h"
#include "interface/preview.h"
#include "interface/title.h"
#include "interface/message.h"
#include "gameplay/sprites/player.h"
#include "gameplay/sprites/objects.h"
#include "gameplay/sprites/radar.h"

extern char pp2_string[1024];
extern char pp2_menu_text[32][64];
extern float pp2_sound_volume;
extern bool pp2_joystick_menu_activation;
extern bool pp2_regenerate_cache;

extern ALLEGRO_TRANSFORM pp2_identity_transform;
extern PP2_CREDITS pp2_credits;
extern PP2_PROFILE_LIST pp2_profiles;
extern int pp2_selected_profile;
extern PP2_PLAYER pp2_player[PP2_MAX_PLAYERS];
extern PP2_LEVEL * pp2_level;
extern PP2_PARTICLE pp2_particle[PP2_MAX_PARTICLES];
extern int pp2_current_particle;
extern T3F_ANIMATION * pp2_object_animation[PP2_MAX_OBJECT_TYPES];
extern PP2_CHARACTER_PREVIEW * pp2_player_preview[PP2_MAX_PLAYERS];
extern ALLEGRO_CONFIG * pp2_player_preview_config[4];
extern ALLEGRO_BITMAP * pp2_bitmap[PP2_MAX_BITMAPS];
extern T3F_ANIMATION * pp2_animation[PP2_MAX_ANIMATIONS];
extern PP2_LEVEL_PREVIEW * pp2_level_preview;
extern T3F_ATLAS * pp2_object_atlas;
extern ALLEGRO_CONFIG * pp2_level_preview_config;
extern ALLEGRO_FONT * pp2_font[PP2_MAX_FONTS];
extern ALLEGRO_BITMAP * pp2_radar_bitmap[PP2_LEVEL_MAX_LAYERS];
extern PP2_OBJECT pp2_object[PP2_MAX_OBJECTS];
extern PP2_RADAR_OBJECT pp2_radar_object[PP2_MAX_RADAR_OBJECTS];
extern int pp2_radar_objects;
extern unsigned long pp2_tick;
extern float pp2_menu_offset;
extern ALLEGRO_FILECHOOSER * pp2_replay_filechooser;
extern int pp2_replay_file_number;
extern bool pp2_viewport_used[4];

extern int pp2_state;
extern int pp2_old_state;
extern int pp2_level_choice, pp2_level_choosing, pp2_level_chosen;
extern unsigned long pp2_level_hash;
extern int pp2_end_game_option;
extern int pp2_option[PP2_MAX_OPTIONS];
extern int pp2_setting[PP2_MAX_SETTINGS];
extern int pp2_winner;
extern int pp2_award_accuracy;
extern int pp2_award_fodder;
extern int pp2_award_trigger;
extern int pp2_time_left;
extern int pp2_coins_needed;
extern int pp2_radar_offset_x;
extern int pp2_radar_offset_y;
extern unsigned int pp2_seed;
extern T3F_CONTROLLER * pp2_controller[PP2_MAX_PLAYERS]; // up to four local controllers
extern ALLEGRO_CONFIG * pp2_config;
extern ALLEGRO_CONFIG * pp2_character_config;
extern ALLEGRO_CONFIG * pp2_level_config;
extern ALLEGRO_SAMPLE * pp2_sample[PP2_MAX_SAMPLES];
extern int pp2_local_player; // if only one player is local, set this to that player

extern PP2_FILE_DATABASE * pp2_level_database;
extern PP2_FILE_DATABASE * pp2_character_database;
extern PP2_FILE_DATABASE * pp2_music_database;
extern PP2_FILE_DATABASE * pp2_demo_database;

/* menu data */
extern T3F_GUI * pp2_menu[PP2_MAX_MENUS];
extern int pp2_current_menu;
extern int pp2_previous_menu[PP2_MAX_MENU_STACK];
extern int pp2_menu_stack_size;
extern int pp2_overlay_menu;
extern int pp2_menu_selected_controller;
extern bool pp2_menu_joystick_disabled;
extern int pp2_menu_joystick_skip;

/* networking */
extern ALLEGRO_THREAD * pp2_server_thread;
extern JOYNET_CLIENT * pp2_client;
extern JOYNET_GAME * pp2_client_game;
extern char pp2_network_id[256];
extern char pp2_network_ip[256];
extern T3NET_SERVER_LIST * pp2_server_list;
extern char pp2_server_name[256];
extern bool pp2_client_disconnected;
extern bool pp2_lan_play;

extern PP2_MESSAGE_LIST * pp2_messages;

extern ALLEGRO_FILE * pp2_replay_file;
extern int pp2_replay_player;
extern bool pp2_replay_rewind;

#endif
