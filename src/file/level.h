#ifndef PP2_LEVEL_H
#define PP2_LEVEL_H

#include "../t3f/animation.h"
#include "../t3f/tilemap.h"
#include "../t3f/collision.h"

#define PP2_LEVEL_MAX_SCRIPT_OBJECTS      16
#define PP2_LEVEL_MAX_PLATFORMS           64
#define PP2_LEVEL_MAX_PLATFORM_ANIMATIONS 32
#define PP2_LEVEL_MAX_LAYERS              32

#define PP2_LEVEL_COLLISION_FLAG_USER   (T3F_COLLISION_FLAG_USER)
#define PP2_LEVEL_COLLISION_FLAG_SECRET (T3F_COLLISION_FLAG_USER << 1)

#define PP2_LEVEL_FLAG_LEGACY 1

typedef struct
{

	int x, y, layer;
	int type;
	int flags;

} PP2_LEVEL_OBJECT;

typedef struct
{

	int x, y, layer;
	int type;
	int flags;
	char script[1024];

} PP2_LEVEL_SCRIPT_OBJECT;

typedef struct
{

	char name[128];
	char author[128];
	char comment[256];
	ALLEGRO_BITMAP * preview;

} PP2_LEVEL_META_DATA;

typedef struct
{

	int x, y, bx, by;

} PP2_LEVEL_ROOM;

typedef struct
{

	int x, y;
	int type;
	int animation;
	int flags;

} PP2_LEVEL_PLATFORM;

typedef struct
{

	T3F_ANIMATION * animation[PP2_LEVEL_MAX_PLATFORM_ANIMATIONS];
	int animations;

	PP2_LEVEL_PLATFORM platform[PP2_LEVEL_MAX_PLATFORMS];
	int platforms;

} PP2_LEVEL_PLATFORM_DATA;

typedef struct
{

	PP2_LEVEL_META_DATA info;

	T3F_TILESET * tileset;
	T3F_TILEMAP * tilemap;
	T3F_COLLISION_TILEMAP * collision_tilemap[PP2_LEVEL_MAX_LAYERS];
	T3F_ANIMATION * bg;
	T3F_ANIMATION * fg;

	PP2_LEVEL_OBJECT * object; // not the actual objects, merely placement info
	int objects;
	PP2_LEVEL_SCRIPT_OBJECT script_object[PP2_LEVEL_MAX_SCRIPT_OBJECTS];
	int script_objects;
	PP2_LEVEL_PLATFORM_DATA platforms;

//	short sprite_layer;
	PP2_LEVEL_ROOM room;
	int flags;

} PP2_LEVEL;

PP2_LEVEL * pp2_create_level(void);
void pp2_destroy_level(PP2_LEVEL * lp);

PP2_LEVEL * pp2_load_old_level(const char * fn, int flags);
PP2_LEVEL * pp2_load_level(const char * fn, int flags);
int pp2_save_level(PP2_LEVEL * lp, const char * fn);

void pp2_fortify_level(PP2_LEVEL * lp);

#endif
