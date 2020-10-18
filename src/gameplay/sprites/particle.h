#ifndef PP2_PARTICLE_H
#define PP2_PARTICLE_H

#include "../camera.h"
#include "../../resource.h"

#define PP2_PARTICLE_FLAG_ACTIVE     1

typedef struct
{

	float x;
	float y;
	float z;
	float vx;
	float vy;
	float vz;

	int type;
	int owner;
	int state;
	int total_life, life;
	unsigned long tick;
	int flags;

} PP2_PARTICLE;

void pp2_particle_logic(PP2_PARTICLE * pp);
void pp2_particle_render(PP2_PARTICLE * pp, PP2_CAMERA * cp, PP2_RESOURCES * resources);

#endif
