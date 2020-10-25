#include "theme.h"
#include "gameplay/sprites/object_defines.h"

const char * get_val_fallback(PP2_THEME * base_theme, PP2_THEME * theme, const char * key)
{
	const char * val = NULL;

	if(theme && theme->config)
	{
		val = al_get_config_value(theme->config, "Paintball Party 2 Theme", key);
	}
	if(!val && base_theme && base_theme->config)
	{
		val = al_get_config_value(base_theme->config, "Paintball Party 2 Theme", key);
	}
	return val;
}

static ALLEGRO_COLOR get_color(const char * buf)
{
	char cbuf[3] = {0};
	int i;
	int ce[4] = {0, 0, 0, 255};
	int l = strlen(buf);

	for(i = 0; i < l / 2; i++)
	{
		cbuf[0] = buf[i * 2 + 0];
		cbuf[1] = buf[i * 2 + 1];
		ce[i] = strtol(cbuf, NULL, 16);
	}
	return al_map_rgba(ce[0], ce[1], ce[2], ce[3]);
}

PP2_THEME * pp2_load_theme(PP2_THEME * base_theme, const char * fn)
{
	PP2_THEME * tp;
	const char * val;

	tp = malloc(sizeof(PP2_THEME));
	if(!tp)
	{
		goto fail;
	}
	memset(tp, 0, sizeof(PP2_THEME));
	if(fn)
	{
		tp->config = al_load_config_file(fn);
		if(!tp->config)
		{
			if(base_theme)
			{
				free(tp);
				tp = NULL;
			}
			else
			{
				goto fail;
			}
		}
	}
	val = get_val_fallback(base_theme, tp, "BITMAP_T3_LOGO");
	if(val)
	{
		tp->bitmap_fn[PP2_BITMAP_T3_LOGO] = val;
	}
	val = get_val_fallback(base_theme, tp, "BITMAP_TITLE_SPLAT");
	if(val)
	{
		tp->bitmap_fn[PP2_BITMAP_TITLE_SPLAT] = val;
	}
	val = get_val_fallback(base_theme, tp, "BITMAP_MENU_BG");
	if(val)
	{
		tp->bitmap_fn[PP2_BITMAP_MENU_BG] = val;
	}
	val = get_val_fallback(base_theme, tp, "BITMAP_T3_LOGO_MEMORY");
	if(val)
	{
		tp->bitmap_fn[PP2_BITMAP_T3_LOGO_MEMORY] = val;
	}
	val = get_val_fallback(base_theme, tp, "BITMAP_MENU_LOGO");
	if(val)
	{
		tp->bitmap_fn[PP2_BITMAP_MENU_LOGO] = val;
	}
	val = get_val_fallback(base_theme, tp, "BITMAP_TITLE_LOGO");
	if(val)
	{
		tp->bitmap_fn[PP2_BITMAP_TITLE_LOGO] = val;
	}
	val = get_val_fallback(base_theme, tp, "BITMAP_TARGET");
	if(val)
	{
		tp->bitmap_fn[PP2_BITMAP_TARGET] = val;
	}
	val = get_val_fallback(base_theme, tp, "BITMAP_EMPTY_PLAYER");
	if(val)
	{
		tp->bitmap_fn[PP2_BITMAP_EMPTY_PLAYER] = val;
	}
	val = get_val_fallback(base_theme, tp, "BITMAP_RADAR_BLIP");
	if(val)
	{
		tp->bitmap_fn[PP2_BITMAP_RADAR_BLIP] = val;
	}
	val = get_val_fallback(base_theme, tp, "BITMAP_TYPING");
	if(val)
	{
		tp->bitmap_fn[PP2_BITMAP_TYPING] = val;
	}
	val = get_val_fallback(base_theme, tp, "BITMAP_HIGHLIGHT");
	if(val)
	{
		tp->bitmap_fn[PP2_BITMAP_HIGHLIGHT] = val;
	}
	val = get_val_fallback(base_theme, tp, "BITMAP_LOADING");
	if(val)
	{
		tp->bitmap_fn[PP2_BITMAP_LOADING] = val;
	}
	val = get_val_fallback(base_theme, tp, "FONT_SMALL");
	if(val)
	{
		tp->font_fn[PP2_FONT_SMALL] = val;
	}
	val = get_val_fallback(base_theme, tp, "FONT_COMIC_10");
	if(val)
	{
		tp->font_fn[PP2_FONT_COMIC_10] = val;
	}
	val = get_val_fallback(base_theme, tp, "FONT_COMIC_12");
	if(val)
	{
		tp->font_fn[PP2_FONT_COMIC_12] = val;
	}
	val = get_val_fallback(base_theme, tp, "FONT_COMIC_14");
	if(val)
	{
		tp->font_fn[PP2_FONT_COMIC_14] = val;
	}
	val = get_val_fallback(base_theme, tp, "FONT_COMIC_16");
	if(val)
	{
		tp->font_fn[PP2_FONT_COMIC_16] = val;
	}
	val = get_val_fallback(base_theme, tp, "FONT_HUD");
	if(val)
	{
		tp->font_fn[PP2_FONT_HUD] = val;
	}
	val = get_val_fallback(base_theme, tp, "FONT_WEAPON_INFO");
	if(val)
	{
		tp->font_fn[PP2_FONT_WEAPON_INFO] = val;
	}
	val = get_val_fallback(base_theme, tp, "OBJECT_PORTAL");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_PORTAL] = val;
	}
	val = get_val_fallback(base_theme, tp, "OBJECT_GENERATOR");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_GENERATOR] = val;
	}
	val = get_val_fallback(base_theme, tp, "OBJECT_AMMO_NORMAL");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_AMMO_NORMAL] = val;
	}
	val = get_val_fallback(base_theme, tp, "OBJECT_AMMO_X");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_AMMO_X] = val;
	}
	val = get_val_fallback(base_theme, tp, "OBJECT_AMMO_MINE");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_AMMO_MINE] = val;
	}
	val = get_val_fallback(base_theme, tp, "OBJECT_AMMO_BOUNCE");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_AMMO_BOUNCE] = val;
	}
	val = get_val_fallback(base_theme, tp, "OBJECT_AMMO_SEEK");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_AMMO_SEEK] = val;
	}
	val = get_val_fallback(base_theme, tp, "OBJECT_POWER_CLOAK");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_POWER_CLOAK] = val;
	}
	val = get_val_fallback(base_theme, tp, "OBJECT_POWER_JUMP");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_POWER_JUMP] = val;
	}
	val = get_val_fallback(base_theme, tp, "OBJECT_POWER_RUN");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_POWER_RUN] = val;
	}
	val = get_val_fallback(base_theme, tp, "OBJECT_POWER_DEFLECT");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_POWER_DEFLECT] = val;
	}
	val = get_val_fallback(base_theme, tp, "OBJECT_FLAG");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_FLAG] = val;
	}
	val = get_val_fallback(base_theme, tp, "OBJECT_FLAG_PORTAL");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_FLAG_PORTAL] = val;
	}
	val = get_val_fallback(base_theme, tp, "OBJECT_BASE");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_BASE] = val;
	}
	val = get_val_fallback(base_theme, tp, "OBJECT_BASE_PORTAL");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_BASE_PORTAL] = val;
	}
	val = get_val_fallback(base_theme, tp, "OBJECT_BANK");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_BANK] = val;
	}
	val = get_val_fallback(base_theme, tp, "OBJECT_GEM_1");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_GEM_1] = val;
	}
	val = get_val_fallback(base_theme, tp, "OBJECT_GEM_2");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_GEM_2] = val;
	}
	val = get_val_fallback(base_theme, tp, "OBJECT_GEM_3");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_GEM_3] = val;
	}
	val = get_val_fallback(base_theme, tp, "OBJECT_GEM_4");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_GEM_4] = val;
	}
	val = get_val_fallback(base_theme, tp, "OBJECT_GEM_5");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_GEM_5] = val;
	}
	val = get_val_fallback(base_theme, tp, "OBJECT_HUNT_PORTAL");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_HUNT_PORTAL] = val;
	}
	val = get_val_fallback(base_theme, tp, "OBJECT_HUNT_BASE");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_HUNT_BASE] = val;
	}
	val = get_val_fallback(base_theme, tp, "OBJECT_TARGET");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_TARGET] = val;
	}
	val = get_val_fallback(base_theme, tp, "OBJECT_TARGET_PORTAL");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_TARGET_PORTAL] = val;
	}
	val = get_val_fallback(base_theme, tp, "OBJECT_POWER_POOF");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_POWER_POOF] = val;
	}
	val = get_val_fallback(base_theme, tp, "OBJECT_POWER_FLY");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_POWER_FLY] = val;
	}
	val = get_val_fallback(base_theme, tp, "OBJECT_JET");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_JET] = val;
	}
	val = get_val_fallback(base_theme, tp, "OBJECT_SPRING_UP");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_SPRING_UP] = val;
	}
	val = get_val_fallback(base_theme, tp, "OBJECT_SPRING_DOWN");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_SPRING_DOWN] = val;
	}
	val = get_val_fallback(base_theme, tp, "OBJECT_SPRING_LEFT");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_SPRING_LEFT] = val;
	}
	val = get_val_fallback(base_theme, tp, "OBJECT_SPRING_RIGHT");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_SPRING_RIGHT] = val;
	}
	val = get_val_fallback(base_theme, tp, "OBJECT_SPRING_BAR_V");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_SPRING_BAR_V] = val;
	}
	val = get_val_fallback(base_theme, tp, "OBJECT_SPRING_BAR_H");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_SPRING_BAR_H] = val;
	}
	val = get_val_fallback(base_theme, tp, "OBJECT_POWER_TURBO");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_POWER_TURBO] = val;
	}
	val = get_val_fallback(base_theme, tp, "OBJECT_AMMO_REFLECTOR");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_AMMO_REFLECTOR] = val;
	}
	val = get_val_fallback(base_theme, tp, "OBJECT_AMMO_PMINE");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_AMMO_PMINE] = val;
	}
	val = get_val_fallback(base_theme, tp, "OBJECT_AMMO_GHOST");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_AMMO_GHOST] = val;
	}
	val = get_val_fallback(base_theme, tp, "OBJECT_COIN");
	if(val)
	{
		tp->object_animation_fn[PP2_OBJECT_COIN] = val;
	}
	val = get_val_fallback(base_theme, tp, "ANIMATION_HUD_AMMO_NORMAL");
	if(val)
	{
		tp->animation_fn[PP2_ANIMATION_HUD_AMMO_NORMAL] = val;
	}
	val = get_val_fallback(base_theme, tp, "ANIMATION_HUD_AMMO_SPLITTER");
	if(val)
	{
		tp->animation_fn[PP2_ANIMATION_HUD_AMMO_SPLITTER] = val;
	}
	val = get_val_fallback(base_theme, tp, "ANIMATION_HUD_AMMO_MINE");
	if(val)
	{
		tp->animation_fn[PP2_ANIMATION_HUD_AMMO_MINE] = val;
	}
	val = get_val_fallback(base_theme, tp, "ANIMATION_HUD_AMMO_BOUNCER");
	if(val)
	{
		tp->animation_fn[PP2_ANIMATION_HUD_AMMO_BOUNCER] = val;
	}
	val = get_val_fallback(base_theme, tp, "ANIMATION_HUD_AMMO_SEEKER");
	if(val)
	{
		tp->animation_fn[PP2_ANIMATION_HUD_AMMO_SEEKER] = val;
	}
	val = get_val_fallback(base_theme, tp, "ANIMATION_HUD_AMMO_REFLECTOR");
	if(val)
	{
		tp->animation_fn[PP2_ANIMATION_HUD_AMMO_REFLECTOR] = val;
	}
	val = get_val_fallback(base_theme, tp, "ANIMATION_HUD_AMMO_PMINE");
	if(val)
	{
		tp->animation_fn[PP2_ANIMATION_HUD_AMMO_PMINE] = val;
	}
	val = get_val_fallback(base_theme, tp, "ANIMATION_HUD_AMMO_GHOST");
	if(val)
	{
		tp->animation_fn[PP2_ANIMATION_HUD_AMMO_GHOST] = val;
	}
	val = get_val_fallback(base_theme, tp, "SAMPLE_FIRE");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_FIRE] = val;
	}
	val = get_val_fallback(base_theme, tp, "SAMPLE_SPLAT");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_SPLAT] = val;
	}
	val = get_val_fallback(base_theme, tp, "SAMPLE_RELOAD_A");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_RELOAD_A] = val;
	}
	val = get_val_fallback(base_theme, tp, "SAMPLE_RELOAD_B");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_RELOAD_B] = val;
	}
	val = get_val_fallback(base_theme, tp, "SAMPLE_AMMO");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_AMMO] = val;
	}
	val = get_val_fallback(base_theme, tp, "SAMPLE_CLOAK");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_CLOAK] = val;
	}
	val = get_val_fallback(base_theme, tp, "SAMPLE_GEM");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_GEM] = val;
	}
	val = get_val_fallback(base_theme, tp, "SAMPLE_JUMP");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_JUMP] = val;
	}
	val = get_val_fallback(base_theme, tp, "SAMPLE_LAND");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_LAND] = val;
	}
	val = get_val_fallback(base_theme, tp, "SAMPLE_HIT");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_HIT] = val;
	}
	val = get_val_fallback(base_theme, tp, "SAMPLE_BUMP");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_BUMP] = val;
	}
	val = get_val_fallback(base_theme, tp, "SAMPLE_TIME");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_TIME] = val;
	}
	val = get_val_fallback(base_theme, tp, "SAMPLE_MENU_PICK");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_MENU_PICK] = val;
	}
	val = get_val_fallback(base_theme, tp, "SAMPLE_MENU_NEXT");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_MENU_NEXT] = val;
	}
	val = get_val_fallback(base_theme, tp, "SAMPLE_FIRE");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_FIRE] = val;
	}
	val = get_val_fallback(base_theme, tp, "SAMPLE_TELE_IN");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_TELE_IN] = val;
	}
	val = get_val_fallback(base_theme, tp, "SAMPLE_TELE_OUT");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_TELE_OUT] = val;
	}
	val = get_val_fallback(base_theme, tp, "SAMPLE_RUN");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_RUN] = val;
	}
	val = get_val_fallback(base_theme, tp, "SAMPLE_DEFLECT");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_DEFLECT] = val;
	}
	val = get_val_fallback(base_theme, tp, "SAMPLE_FLAG");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_FLAG] = val;
	}
	val = get_val_fallback(base_theme, tp, "SAMPLE_BANK");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_BANK] = val;
	}
	val = get_val_fallback(base_theme, tp, "SAMPLE_BASE");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_BASE] = val;
	}
	val = get_val_fallback(base_theme, tp, "SAMPLE_RESPAWN");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_RESPAWN] = val;
	}
	val = get_val_fallback(base_theme, tp, "SAMPLE_PJUMP");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_PJUMP] = val;
	}
	val = get_val_fallback(base_theme, tp, "SAMPLE_CRUSH");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_CRUSH] = val;
	}
	val = get_val_fallback(base_theme, tp, "SAMPLE_TARGET");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_TARGET] = val;
	}
	val = get_val_fallback(base_theme, tp, "SAMPLE_BOUNCE");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_BOUNCE] = val;
	}
	val = get_val_fallback(base_theme, tp, "SAMPLE_RICOCHET");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_RICOCHET] = val;
	}
	val = get_val_fallback(base_theme, tp, "SAMPLE_START");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_START] = val;
	}
	val = get_val_fallback(base_theme, tp, "SAMPLE_WIN");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_WIN] = val;
	}
	val = get_val_fallback(base_theme, tp, "SAMPLE_ENTER");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_ENTER] = val;
	}
	val = get_val_fallback(base_theme, tp, "SAMPLE_FLY");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_FLY] = val;
	}
	val = get_val_fallback(base_theme, tp, "SAMPLE_PFLY");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_PFLY] = val;
	}
	val = get_val_fallback(base_theme, tp, "SAMPLE_SPRING");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_SPRING] = val;
	}
	val = get_val_fallback(base_theme, tp, "SAMPLE_PTURBO");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_PTURBO] = val;
	}
	val = get_val_fallback(base_theme, tp, "SAMPLE_COIN_LAND");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_COIN_LAND] = val;
	}
	val = get_val_fallback(base_theme, tp, "SAMPLE_COIN_PICKUP");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_COIN_PICKUP] = val;
	}
	val = get_val_fallback(base_theme, tp, "SAMPLE_LOGO_TICK");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_LOGO_TICK] = val;
	}
	val = get_val_fallback(base_theme, tp, "SAMPLE_LOGO");
	if(val)
	{
		tp->sample_fn[PP2_SAMPLE_LOGO] = val;
	}
	tp->theme_music_fn = get_val_fallback(base_theme, tp, "theme_music_fn");
	tp->menu_music_fn = get_val_fallback(base_theme, tp, "menu_music_fn");
	tp->menu_bg_color = t3f_color_white;
	val = get_val_fallback(base_theme, tp, "menu_bg_color");
	if(val)
	{
		tp->menu_bg_color = get_color(val);
	}
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
		free(tp);
	}
}
