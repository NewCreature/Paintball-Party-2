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
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "SAMPLE_FIRE");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_FIRE] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "SAMPLE_SPLAT");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_SPLAT] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "SAMPLE_RELOAD_A");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_RELOAD_A] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "SAMPLE_RELOAD_B");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_RELOAD_B] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "SAMPLE_AMMO");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_AMMO] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "SAMPLE_CLOAK");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_CLOAK] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "SAMPLE_GEM");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_GEM] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "SAMPLE_JUMP");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_JUMP] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "SAMPLE_LAND");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_LAND] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "SAMPLE_HIT");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_HIT] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "SAMPLE_BUMP");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_BUMP] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "SAMPLE_TIME");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_TIME] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "SAMPLE_MENU_PICK");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_MENU_PICK] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "SAMPLE_MENU_NEXT");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_MENU_NEXT] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "SAMPLE_FIRE");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_FIRE] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "SAMPLE_TELE_IN");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_TELE_IN] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "SAMPLE_TELE_OUT");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_TELE_OUT] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "SAMPLE_RUN");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_RUN] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "SAMPLE_DEFLECT");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_DEFLECT] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "SAMPLE_FLAG");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_FLAG] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "SAMPLE_BANK");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_BANK] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "SAMPLE_BASE");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_BASE] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "SAMPLE_RESPAWN");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_RESPAWN] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "SAMPLE_PJUMP");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_PJUMP] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "SAMPLE_CRUSH");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_CRUSH] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "SAMPLE_TARGET");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_TARGET] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "SAMPLE_BOUNCE");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_BOUNCE] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "SAMPLE_RICOCHET");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_RICOCHET] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "SAMPLE_START");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_START] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "SAMPLE_WIN");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_WIN] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "SAMPLE_ENTER");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_ENTER] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "SAMPLE_FLY");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_FLY] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "SAMPLE_PFLY");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_PFLY] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "SAMPLE_SPRING");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_SPRING] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "SAMPLE_PTURBO");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_PTURBO] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "SAMPLE_COIN_LAND");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_COIN_LAND] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "SAMPLE_COIN_PICKUP");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_COIN_PICKUP] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "SAMPLE_LOGO_TICK");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_LOGO_TICK] = val;
	}
	val = al_get_config_value(tp->config, "Paintball Party 2 Theme", "SAMPLE_LOGO");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_LOGO] = val;
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
