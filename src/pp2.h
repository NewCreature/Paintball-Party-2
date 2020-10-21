#ifndef PP2_H
#define PP2_H

#include "defines.h"
#include "theme.h"
#include "resource.h"
#include "interface/interface.h"
#include "gameplay/game_struct.h"

typedef struct
{

	PP2_THEME * theme;
	PP2_RESOURCES resources;

	int lan_arg;

	PP2_INTERFACE interface;
	PP2_GAME game;

	int state;
	int old_state;

} PP2_INSTANCE;

#endif
