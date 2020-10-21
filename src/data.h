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
#include "gameplay/sprites/radar_struct.h"

extern int pp2_state;
extern int pp2_old_state;
extern int pp2_end_game_option;
extern ALLEGRO_CONFIG * pp2_config;
extern ALLEGRO_CONFIG * pp2_character_config;
extern ALLEGRO_CONFIG * pp2_level_config;

extern PP2_FILE_DATABASE * pp2_level_database;
extern PP2_FILE_DATABASE * pp2_character_database;
extern PP2_FILE_DATABASE * pp2_music_database;
extern PP2_FILE_DATABASE * pp2_demo_database;

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

#endif
