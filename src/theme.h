#ifndef PP2_THEME_H
#define PP2_THEME_H

#include "t3f/font.h"

/*#define PP2_THEME_MAX_BITMAPS           8
#define PP2_THEME_BITMAP_BG_TILE        0
#define PP2_THEME_BITMAP_LOGO           1
#define PP2_THEME_BITMAP_HUD_SPLAT_FEST 2
#define PP2_THEME_BITMAP_HUD_ELIMINATOR 3
#define PP2_THEME_BITMAP_HUD_TIMED      4

#define PP2_THEME_MAX_FONTS        8
#define PP2_THEME_FONT_LOAD_INFO   0
#define PP2_THEME_FONT_MENU_HEADER 1
#define PP2_THEME_FONT_MENU_ITEM   2
#define PP2_THEME_FONT_HUD         3
#define PP2_THEME_FONT_WEAPON_INFO 4

#define PP2_THEME_LOAD_SCREEN_TYPE_DEFAULT 0
#define PP2_THEME_LOAD_SCREEN_TYPE_CLASSIC 1 */

typedef struct
{

	ALLEGRO_CONFIG * config;
//	ALLEGRO_BITMAP * bitmap[PP2_THEME_MAX_BITMAPS];
//	T3F_FONT * font[PP2_THEME_MAX_FONTS];
	const char * bitmap_load_fn;
	const char * bitmap_bg_tile_fn;
	const char * font_load_info_fn;
	const char * font_menu_header_fn;
	const char * font_menu_item_fn;
	const char * font_hud_fn;
	const char * font_weapon_info_fn;
	const char * theme_music_fn;
	const char * menu_music_fn;

	int load_screen_type;

} PP2_THEME;

PP2_THEME * pp2_load_theme(const char * fn);
void pp2_destroy_theme(PP2_THEME * tp);

#endif
