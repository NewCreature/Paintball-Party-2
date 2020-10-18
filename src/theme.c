#include "theme.h"

PP2_THEME * pp2_load_theme(const char * fn)
{
	PP2_THEME * tp;
	const char * val;

	tp = malloc(sizeof(PP2_THEME));
	if(!tp)
	{
		goto fail;
	}
	memset(tp, 0, sizeof(PP2_THEME));
	tp->config = al_load_config_file(fn);
	if(!tp->config)
	{
		goto fail;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "bitmap_bg_tile");
	if(val)
	{
		tp->bitmap_bg_tile_fn = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "bitmap_load");
	if(val)
	{
		tp->bitmap_load_fn = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "font_load_info");
	if(val)
	{
		tp->font_load_info_fn = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "font_menu_header");
	if(val)
	{
		tp->font_menu_header_fn = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "font_menu_item");
	if(val)
	{
		tp->font_menu_item_fn = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "font_hud");
	if(val)
	{
		tp->font_hud_fn = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "font_weapon_info");
	if(val)
	{
		tp->font_weapon_info_fn = val;
	}
	tp->theme_music_fn = al_get_config_value(tp->config, "Paintball Party 2 Theme", "theme_music_fn");
	tp->menu_music_fn = al_get_config_value(tp->config, "Paintball Party 2 Theme", "menu_music_fn");
	return tp;

	fail:
	{
		pp2_destroy_theme(tp);
	}
	return NULL;
}

void pp2_destroy_theme(PP2_THEME * tp)
{
	if(tp)
	{
		if(tp->config)
		{
			al_destroy_config(tp->config);
		}
	}
}
