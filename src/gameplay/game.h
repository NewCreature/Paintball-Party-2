#ifndef PP2_GAME_H
#define PP2_GAME_H

#include "sprites/player.h"
#include "../resource.h"
#include "../pp2.h"
#include "game_struct.h"

void pp2_game_render(PP2_GAME * gp, PP2_THEME * theme, PP2_RESOURCES * resources);
void pp2_game_over_logic(PP2_INSTANCE * instance, PP2_GAME * gp, PP2_INTERFACE * ip, PP2_RESOURCES * resources);
void pp2_game_over_render(PP2_GAME * gp, PP2_THEME * theme, PP2_RESOURCES * resources);
void pp2_camera_logic(PP2_GAME * gp, int i);
void pp2_game_logic(PP2_GAME * gp, PP2_INTERFACE * ip, PP2_RESOURCES * resources, PP2_THEME * tp);
void pp2_game_paused_logic(PP2_INSTANCE * instance);
void pp2_game_paused_render(PP2_INSTANCE * instance, PP2_INTERFACE * ip, PP2_RESOURCES * resources);

#endif
