#ifndef PP2_H
#define PP2_H

#include "defines.h"
#include "theme.h"
#include "resource.h"
#include "gameplay/game_struct.h"

typedef struct
{

	PP2_THEME * theme;
	PP2_RESOURCES resources;

	int lan_arg;

	PP2_GAME game;

} PP2_INSTANCE;

#endif
