#ifndef PP2_LEVEL_SETUP_H
#define PP2_LEVEL_SETUP_H

#include "../gameplay/game_struct.h"

bool pp2_level_setup_players_ready(PP2_GAME * gp);
void pp2_level_setup_logic(PP2_INSTANCE * instance);
void pp2_level_setup_render(PP2_INTERFACE * ip, PP2_GAME * gp, PP2_RESOURCES * resources);

#endif
