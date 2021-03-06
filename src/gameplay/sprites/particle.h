#ifndef PP2_PARTICLE_H
#define PP2_PARTICLE_H

#include "../camera.h"
#include "../../resource.h"
#include "particle_struct.h"
#include "../game_struct.h"

void pp2_particle_logic(PP2_GAME * gp, PP2_PARTICLE * pp);
void pp2_particle_render(PP2_GAME * gp, PP2_PARTICLE * pp, PP2_CAMERA * cp, PP2_RESOURCES * resources);

#endif
