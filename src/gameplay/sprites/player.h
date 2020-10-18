#ifndef PP2_PLAYER_H
#define PP2_PLAYER_H

#include "t3f/t3f.h"
#include "t3f/tilemap.h"
#include "t3f/collision.h"
#include "t3f/view.h"
#include "../../file/character.h"
#include "../../defines.h"
#include "../camera.h"
#include "paintball.h"
#include "particle.h"
#include "../../resource.h"
#include "player_struct.h"
#include "../game_struct.h"

void pp2_player_next_weapon(PP2_PLAYER * pp);
void pp2_player_move_object_x(PP2_PLAYER * pp);
void pp2_player_move_object_y(PP2_PLAYER * pp);
void pp2_player_receive_hit(PP2_GAME * gp, PP2_PLAYER * pp, int dealer, PP2_RESOURCES * resources);
void pp2_player_logic(PP2_GAME * gp, PP2_PLAYER * pp, PP2_RESOURCES * resources);
void pp2_player_render(PP2_PLAYER * pp, PP2_CAMERA * cp);

#endif
