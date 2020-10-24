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
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "BITMAP_MENU_BG");
	if(val)
	{
		tp->bitmap_fn[PP2_BITMAP_MENU_BG] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "BITMAP_LOADING");
	if(val)
	{
		tp->bitmap_fn[PP2_BITMAP_LOADING] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "FONT_SMALL");
	if(val)
	{
		tp->font_fn[PP2_FONT_SMALL] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "FONT_COMIC_10");
	if(val)
	{
		tp->font_fn[PP2_FONT_COMIC_10] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "FONT_COMIC_12");
	if(val)
	{
		tp->font_fn[PP2_FONT_COMIC_12] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "FONT_COMIC_14");
	if(val)
	{
		tp->font_fn[PP2_FONT_COMIC_14] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "FONT_COMIC_16");
	if(val)
	{
		tp->font_fn[PP2_FONT_COMIC_16] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "FONT_HUD");
	if(val)
	{
		tp->font_fn[PP2_FONT_HUD] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "FONT_WEAPON_INFO");
	if(val)
	{
		tp->font_fn[PP2_FONT_WEAPON_INFO] = val;
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
