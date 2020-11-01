#ifndef PP2_THEME_H
#define PP2_THEME_H

#include "t3f/font.h"
#include "defines.h"

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
	ALLEGRO_COLOR menu_bg_color;

	int load_screen_type;
	float hud_x;
	float hud_y;

} PP2_THEME;

PP2_THEME * pp2_load_theme(PP2_THEME * base_theme, const char * fn);
void pp2_destroy_theme(PP2_THEME * tp);

#endif
