#ifndef PP2_PREVIEW_H
#define PP2_PREVIEW_H

#include <allegro5/allegro5.h>
#include "../t3f/t3f.h"
#include "../t3f/animation.h"
#include "../file/character.h"
#include "../resource.h"

typedef struct
{

	T3F_ANIMATION * animation[PP2_CHARACTER_MAX_ANIMATIONS];
	int animations;
	PP2_CHARACTER_PIECE piece[PP2_CHARACTER_MAX_PIECES];
	int pieces;
	float cx, cy;
	ALLEGRO_SAMPLE * sound;
	ALLEGRO_SAMPLE_ID sound_id;
	char name[256];
	char author[256];
	char comment[1024];

} PP2_CHARACTER_PREVIEW;

typedef struct
{

	ALLEGRO_BITMAP * bitmap;
	int players;

	char name[256];
	char author[256];
	char comment[1024];

} PP2_LEVEL_PREVIEW;

PP2_CHARACTER_PREVIEW * pp2_load_character_preview(const char * fn);
bool pp2_create_character_preview_from_character(const char * fn, const char * outfn, PP2_RESOURCES * resources);
bool pp2_save_character_preview(PP2_CHARACTER_PREVIEW * cp, const char * fn);
void pp2_destroy_character_preview(PP2_CHARACTER_PREVIEW * pp);
void pp2_render_character_preview(PP2_CHARACTER_PREVIEW * pp, int tick, ALLEGRO_COLOR tint, float x, float y, float z);
PP2_LEVEL_PREVIEW * pp2_load_level_preview(const char * fn);
bool pp2_create_level_preview_from_level(const char * fn, const char * outfn, PP2_RESOURCES * resources);
bool pp2_save_level_preview(PP2_LEVEL_PREVIEW * pp, const char * fn);
void pp2_destroy_level_preview(PP2_LEVEL_PREVIEW * pp);
void pp2_render_level_preview(PP2_LEVEL_PREVIEW * pp, ALLEGRO_COLOR tint, float x, float y, float z);

#endif
