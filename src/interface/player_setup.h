#ifndef PP2_PLAYER_SETUP_H
#define PP2_PLAYER_SETUP_H

#include "../resource.h"
#include "../pp2.h"
#include "../gameplay/game_struct.h"

void pp2_player_setup_reset(PP2_GAME * gp);
void pp2_player_setup_logic(PP2_INTERFACE * ip, PP2_GAME * gp, PP2_INSTANCE * instance);
void pp2_player_setup_render(PP2_INTERFACE * ip, PP2_GAME * gp, PP2_RESOURCES * resources);

#endif
