#ifndef PP2_OBJECTS_H
#define PP2_OBJECTS_H

#include "t3f/t3f.h"
#include "pp2/gameplay/camera.h"
#include "pp2/resource.h"
#include "object_struct.h"
#include "pp2/gameplay/game_struct.h"

int pp2_generate_object(PP2_GAME * gp, float x, float y, int layer, int type, int flags, int extra);
void pp2_object_logic(PP2_GAME * gp, PP2_OBJECT * op, PP2_RESOURCES * resources);
void pp2_object_render(PP2_OBJECT * op, PP2_CAMERA * cp, PP2_RESOURCES * resources);

#endif
