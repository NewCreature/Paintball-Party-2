#ifndef PP2_OBJECTS_H
#define PP2_OBJECTS_H

#include "../../t3f/tilemap.h"
#include "../../t3f/collision.h"
#include "../camera.h"
#include "../../resource.h"

#define PP2_OBJECT_FLAG_ACTIVE         1
#define PP2_OBJECT_FLAG_GROUND         2

/* main objects */
#define PP2_OBJECT_PORTAL              0
#define PP2_OBJECT_GENERATOR           1

/* ammo power ups */
#define PP2_OBJECT_AMMO_NORMAL         2
#define PP2_OBJECT_AMMO_X              3
#define PP2_OBJECT_AMMO_MINE           4
#define PP2_OBJECT_AMMO_BOUNCE         5
#define PP2_OBJECT_AMMO_SEEK           6

/* special power ups */
#define PP2_OBJECT_POWER_CLOAK         7
#define PP2_OBJECT_POWER_JUMP          8
#define PP2_OBJECT_POWER_RUN           9
#define PP2_OBJECT_POWER_DEFLECT      10

/* capture the flag objects */
#define PP2_OBJECT_FLAG               11
#define PP2_OBJECT_FLAG_PORTAL        12
#define PP2_OBJECT_BASE               13
#define PP2_OBJECT_BASE_PORTAL        14
#define PP2_OBJECT_BANK               15

/* scavenger hunt objects */
#define PP2_OBJECT_GEM_1              16
#define PP2_OBJECT_GEM_2              17
#define PP2_OBJECT_GEM_3              18
#define PP2_OBJECT_GEM_4              19
#define PP2_OBJECT_GEM_5              20
#define PP2_OBJECT_HUNT_PORTAL        21
#define PP2_OBJECT_HUNT_BASE          22

/* target practice objects */
#define PP2_OBJECT_TARGET             23
#define PP2_OBJECT_TARGET_PORTAL      24

/* extra objects */
#define PP2_OBJECT_POWER_POOF         25
#define PP2_OBJECT_GEM_POOF           26
#define PP2_OBJECT_POOF               27

/* new objects for v1.1 */
#define PP2_OBJECT_POWER_FLY          28
#define PP2_OBJECT_JET                29
#define PP2_OBJECT_SPRING_UP          30
#define PP2_OBJECT_SPRING_DOWN        31
#define PP2_OBJECT_SPRING_LEFT        32
#define PP2_OBJECT_SPRING_RIGHT       33
#define PP2_OBJECT_SPRING_BAR_V       34
#define PP2_OBJECT_SPRING_BAR_H       35
#define PP2_OBJECT_POWER_TURBO        36

#define PP2_OBJECT_AMMO_REFLECTOR     37
#define PP2_OBJECT_AMMO_PMINE         38
#define PP2_OBJECT_AMMO_GHOST         39

#define PP2_OBJECT_COIN               40

#define PP2_OBJECT_TILE_READER       255

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

int pp2_generate_object(float x, float y, int layer, int type, int flags, int extra);
void pp2_object_logic(PP2_OBJECT * op);
void pp2_object_render(PP2_OBJECT * op, PP2_CAMERA * cp, PP2_RESOURCES * resources);

#endif
