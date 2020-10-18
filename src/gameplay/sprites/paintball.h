#ifndef PP2_PAINTBALL_H
#define PP2_PAINTBALL_H

#include "../camera.h"
#include "../../resource.h"

#define PP2_PAINTBALL_TYPE_NORMAL     0
#define PP2_PAINTBALL_TYPE_SPLITTER   1
#define PP2_PAINTBALL_TYPE_MINE       2
#define PP2_PAINTBALL_TYPE_BOUNCER    3
#define PP2_PAINTBALL_TYPE_SEEKER     4
#define PP2_PAINTBALL_TYPE_REFLECTOR  5
#define PP2_PAINTBALL_TYPE_PMINE      6
#define PP2_PAINTBALL_TYPE_GHOST      7

#define PP2_PAINTBALL_FLAG_ACTIVE     1
#define PP2_PAINTBALL_FLAG_LANDB      4
#define PP2_PAINTBALL_FLAG_LANDT      8
#define PP2_PAINTBALL_FLAG_LANDL     16
#define PP2_PAINTBALL_FLAG_LANDR     32

#define PP2_PAINTBALL_TRAIL_TIME      6
#define PP2_PAINTBALL_HIGHLIGHT_SIZE  4

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

int pp2_create_paintball(int owner, int type, float x, float y, float angle);
void pp2_paintball_logic(PP2_PAINTBALL * pp);
void pp2_paintball_render(PP2_PAINTBALL * pp, PP2_CAMERA * cp, PP2_RESOURCES * resources);
void pp2_paintball_trail_logic(PP2_PAINTBALL_TRAIL * pp);
void pp2_paintball_trail_render(PP2_PAINTBALL_TRAIL * pp, PP2_CAMERA * cp);

#endif
