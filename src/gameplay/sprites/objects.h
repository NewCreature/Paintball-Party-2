#ifndef PP2_OBJECTS_H
#define PP2_OBJECTS_H

#include "../../t3f/tilemap.h"
#include "../../t3f/collision.h"
#include "../camera.h"
#include "../../resource.h"
#include "object_struct.h"
#include "../game_struct.h"

int pp2_generate_object(PP2_GAME * gp, float x, float y, int layer, int type, int flags, int extra);
void pp2_object_logic(PP2_GAME * gp, PP2_OBJECT * op, PP2_RESOURCES * resources);
void pp2_object_render(PP2_OBJECT * op, PP2_CAMERA * cp, PP2_RESOURCES * resources);

#endif
