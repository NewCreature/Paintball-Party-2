#ifndef PP2_THEME_H
#define PP2_THEME_H

#include "t3f/font.h"
#include "defines.h"

#define PP2_THEME_MAX_OBJECTS           32
#define PP2_THEME_OBJECT_HUD_SCORE       0
#define PP2_THEME_OBJECT_HUD_SCORE_TEXT  1
#define PP2_THEME_OBJECT_HUD_AMMO        2
#define PP2_THEME_OBJECT_HUD_AMMO_TYPE   3
#define PP2_THEME_OBJECT_HUD_AMMO_TEXT   4
#define PP2_THEME_OBJECT_HUD_TIMER       5
#define PP2_THEME_OBJECT_HUD_TIMER_TEXT  6

typedef struct
{

	float x;
	float y;

} PP2_THEME_OBJECT;

typedef struct
{

	ALLEGRO_CONFIG * config;
	const char * font_fn[PP2_MAX_FONTS];
	const char * bitmap_fn[PP2_MAX_BITMAPS];
	int bitmap_option[PP2_MAX_BITMAPS];
	const char * animation_fn[PP2_MAX_ANIMATIONS];
	const char * object_animation_fn[PP2_MAX_OBJECT_TYPES];
	const char * sample_fn[PP2_MAX_SAMPLES];
	const char * theme_music_fn;
	const char * menu_music_fn;
	const char * victory_music_fn;
	const char * defeat_music_fn;
	ALLEGRO_COLOR menu_bg_color;

	int load_screen_type;
	PP2_THEME_OBJECT object[PP2_THEME_MAX_OBJECTS];

} PP2_THEME;

PP2_THEME * pp2_load_theme(PP2_THEME * base_theme, const char * fn);
void pp2_destroy_theme(PP2_THEME * tp);

#endif
