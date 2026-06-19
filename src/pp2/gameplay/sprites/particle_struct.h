#ifndef PP2_PARTICLE_STRUCT_H
#define PP2_PARTICLE_STRUCT_H

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

#endif
