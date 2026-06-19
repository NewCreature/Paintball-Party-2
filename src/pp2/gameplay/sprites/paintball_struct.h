#ifndef PP2_PAINTBALL_STRUCT_H
#define PP2_PAINTBALL_STRUCT_H

typedef struct
{

	float x, y, z;
	float cx, cy;
	float angle;
	int layer;
	int owner;
	unsigned long tick;
	int counter;
	bool active;

} PP2_PAINTBALL_TRAIL;

typedef struct
{

	T3F_COLLISION_OBJECT * object;

	float x, y, z;
	float cx, cy;
	float vx, vy;
	float angle;
	int layer;

	int type;
	int owner;
	int state;
	int leaving;
	int deflect;
	int target;
	int counter;
	unsigned long tick;
	int flags;

} PP2_PAINTBALL;

#endif
