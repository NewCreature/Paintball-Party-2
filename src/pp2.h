#ifndef PP2_H
#define PP2_H

#include "joynet/joynet.h"
#include "t3net/server_list.h"
#include "defines.h"
#include "theme.h"
#include "resource.h"
#include "interface/interface.h"
#include "gameplay/game_struct.h"

typedef struct
{

	PP2_THEME * default_theme;
	PP2_THEME * theme;
	PP2_RESOURCES resources;

	PP2_INTERFACE ui;
	PP2_GAME game;

	ALLEGRO_THREAD * server_thread;
	JOYNET_CLIENT * client;
	T3NET_SERVER_LIST * server_list;

	int state;
	int old_state;
	int logic_state;

} PP2_INSTANCE;

#endif
