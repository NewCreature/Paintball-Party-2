#include "t3f/t3f.h"
#include "t3f/animation.h"
#include "t3f/controller.h"
#include "t3f/gui.h"
#include "t3net/server_list.h"
#include <allegro5/allegro_native_dialog.h>
#include "file/database/database.h"
#include "file/database/character.h"
#include "file/database/level.h"
#include "file/profile.h"
#include "file/config.h"
#include "file/wav.h"
#include "file/level.h"
#include "interface/title.h"
#include "interface/message.h"
#include "interface/preview.h"
#include "gameplay/game.h"
#include "gameplay/network.h"
#include "gameplay/replay.h"
#include "gameplay/sprites/player.h"
#include "gameplay/sprites/paintball.h"
#include "gameplay/sprites/objects.h"
#include "gameplay/sprites/particle.h"
#include "gameplay/sprites/radar_struct.h"

bool pp2_regenerate_cache = false;
bool pp2_use_ffmpeg = false;

ALLEGRO_TRANSFORM pp2_identity_transform;
PP2_CREDITS pp2_credits;
unsigned long pp2_tick = 0;

int pp2_state = PP2_STATE_GAME;
int pp2_old_state = PP2_STATE_GAME;
int pp2_level_choice, pp2_level_choosing, pp2_level_chosen;
unsigned long pp2_level_hash = 0;
int pp2_end_game_option = PP2_END_GAME_QUIT;
int pp2_option[PP2_MAX_OPTIONS] = {0};
int pp2_setting[PP2_MAX_SETTINGS] = {0};
T3F_CONTROLLER * pp2_controller[PP2_MAX_PLAYERS] = {NULL}; // up to four local controllers
ALLEGRO_CONFIG * pp2_config = NULL;
ALLEGRO_CONFIG * pp2_character_config = NULL;
ALLEGRO_CONFIG * pp2_level_config = NULL;
ALLEGRO_SAMPLE * pp2_sample[PP2_MAX_SAMPLES] = {NULL};

PP2_FILE_DATABASE * pp2_level_database = NULL;
PP2_FILE_DATABASE * pp2_character_database = NULL;
PP2_FILE_DATABASE * pp2_music_database = NULL;
PP2_FILE_DATABASE * pp2_demo_database = NULL;

/* menu data */
T3F_GUI * pp2_menu[PP2_MAX_MENUS] = {NULL};
int pp2_current_menu = PP2_MENU_MAIN;
int pp2_previous_menu[PP2_MAX_MENU_STACK] = {PP2_MENU_MAIN};
int pp2_menu_stack_size = 0;
int pp2_overlay_menu = PP2_MENU_OVERLAY;
int pp2_menu_selected_controller = 0;
bool pp2_menu_joystick_disabled = false;
int pp2_menu_joystick_skip = 0;

/* networking */
ALLEGRO_THREAD * pp2_server_thread = NULL;
JOYNET_CLIENT * pp2_client = NULL;
JOYNET_GAME * pp2_client_game = NULL;
char pp2_network_id[256] = {0};
char pp2_network_ip[256] = {0};
T3NET_SERVER_LIST * pp2_server_list = NULL;
char pp2_server_name[256] = {0};
bool pp2_client_disconnected = false;
bool pp2_lan_play = false;

PP2_MESSAGE_LIST * pp2_messages = NULL;
