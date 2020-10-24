#include "theme.h"
#include "gameplay/sprites/object_defines.h"

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
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "OBJECT_PORTAL");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_PORTAL] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "OBJECT_GENERATOR");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_GENERATOR] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "OBJECT_AMMO_NORMAL");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_AMMO_NORMAL] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "OBJECT_AMMO_X");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_AMMO_X] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "OBJECT_AMMO_MINE");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_AMMO_MINE] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "OBJECT_AMMO_BOUNCE");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_AMMO_BOUNCE] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "OBJECT_AMMO_SEEK");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_AMMO_SEEK] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "OBJECT_POWER_CLOAK");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_POWER_CLOAK] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "OBJECT_POWER_JUMP");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_POWER_JUMP] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "OBJECT_POWER_RUN");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_POWER_RUN] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "OBJECT_POWER_DEFLECT");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_POWER_DEFLECT] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "OBJECT_FLAG");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_FLAG] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "OBJECT_FLAG_PORTAL");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_FLAG_PORTAL] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "OBJECT_BASE");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_BASE] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "OBJECT_BASE_PORTAL");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_BASE_PORTAL] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "OBJECT_BANK");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_BANK] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "OBJECT_GEM_1");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_GEM_1] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "OBJECT_GEM_2");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_GEM_2] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "OBJECT_GEM_3");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_GEM_3] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "OBJECT_GEM_4");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_GEM_4] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "OBJECT_GEM_5");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_GEM_5] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "OBJECT_HUNT_PORTAL");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_HUNT_PORTAL] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "OBJECT_HUNT_BASE");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_HUNT_BASE] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "OBJECT_TARGET");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_TARGET] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "OBJECT_TARGET_PORTAL");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_TARGET_PORTAL] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "OBJECT_POWER_POOF");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_POWER_POOF] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "OBJECT_POWER_FLY");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_POWER_FLY] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "OBJECT_JET");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_JET] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "OBJECT_SPRING_UP");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_SPRING_UP] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "OBJECT_SPRING_DOWN");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_SPRING_DOWN] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "OBJECT_SPRING_LEFT");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_SPRING_LEFT] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "OBJECT_SPRING_RIGHT");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_SPRING_RIGHT] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "OBJECT_SPRING_BAR_V");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_SPRING_BAR_V] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "OBJECT_SPRING_BAR_H");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_SPRING_BAR_H] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "OBJECT_POWER_TURBO");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_POWER_TURBO] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "OBJECT_AMMO_REFLECTOR");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_AMMO_REFLECTOR] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "OBJECT_AMMO_PMINE");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_AMMO_PMINE] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "OBJECT_AMMO_GHOST");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_AMMO_GHOST] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "OBJECT_COIN");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_COIN] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "ANIMATION_HUD_AMMO_NORMAL");
	if(val)
	{
		tp->animation_fn[PP2_ANIMATION_HUD_AMMO_NORMAL] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "ANIMATION_HUD_AMMO_SPLITTER");
	if(val)
	{
		tp->animation_fn[PP2_ANIMATION_HUD_AMMO_SPLITTER] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "ANIMATION_HUD_AMMO_MINE");
	if(val)
	{
		tp->animation_fn[PP2_ANIMATION_HUD_AMMO_MINE] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "ANIMATION_HUD_AMMO_BOUNCER");
	if(val)
	{
		tp->animation_fn[PP2_ANIMATION_HUD_AMMO_BOUNCER] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "ANIMATION_HUD_AMMO_SEEKER");
	if(val)
	{
		tp->animation_fn[PP2_ANIMATION_HUD_AMMO_SEEKER] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "ANIMATION_HUD_AMMO_REFLECTOR");
	if(val)
	{
		tp->animation_fn[PP2_ANIMATION_HUD_AMMO_REFLECTOR] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "ANIMATION_HUD_AMMO_PMINE");
	if(val)
	{
		tp->animation_fn[PP2_ANIMATION_HUD_AMMO_PMINE] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "ANIMATION_HUD_AMMO_GHOST");
	if(val)
	{
		tp->animation_fn[PP2_ANIMATION_HUD_AMMO_GHOST] = val;
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
