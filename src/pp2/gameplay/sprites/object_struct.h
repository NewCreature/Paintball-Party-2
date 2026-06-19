#ifndef PP2_OBJECT_STRUCT_H
#define PP2_OBJECT_STRUCT_H

typedef struct
{

	T3F_COLLISION_OBJECT * object;

	float x, y, z;
	float vx, vy;

	int type, layer, extra;
	unsigned long tick;
	int counter;
	int flags;

} PP2_OBJECT;

#endif
