#ifndef PP2_PAINTBALL_H
#define PP2_PAINTBALL_H

#include "../camera.h"
#include "../../resource.h"
#include "paintball_struct.h"

int pp2_create_paintball(int owner, int type, float x, float y, float angle);
void pp2_paintball_logic(PP2_PAINTBALL * pp, PP2_RESOURCES * resources);
void pp2_paintball_render(PP2_PAINTBALL * pp, PP2_CAMERA * cp, PP2_RESOURCES * resources);
void pp2_paintball_trail_logic(PP2_PAINTBALL_TRAIL * pp);
void pp2_paintball_trail_render(PP2_PAINTBALL_TRAIL * pp, PP2_CAMERA * cp);

#endif
