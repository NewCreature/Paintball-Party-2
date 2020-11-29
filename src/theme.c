#include "theme.h"
#include "gameplay/sprites/object_defines.h"

static char * bitmap_name_table[PP2_MAX_BITMAPS] = {NULL};
static char * animation_name_table[PP2_MAX_ANIMATIONS] = {NULL};
static char * font_name_table[PP2_MAX_FONTS] = {NULL};
static char * object_name_table[PP2_MAX_OBJECT_TYPES] = {NULL};
static char * sample_name_table[PP2_MAX_SAMPLES] = {NULL};
static char * theme_object_name_table[PP2_THEME_MAX_OBJECTS] = {NULL};

static void init_name_tables(void)
{
	bitmap_name_table[PP2_BITMAP_T3_LOGO] = "T3_LOGO";
	bitmap_name_table[PP2_BITMAP_TITLE_SPLAT] = "TITLE_SPLAT";
	bitmap_name_table[PP2_BITMAP_HUD] = "HUD";
	bitmap_name_table[PP2_BITMAP_MENU_BG] = "MENU_BG";
	bitmap_name_table[PP2_BITMAP_T3_LOGO_MEMORY] = "T3_LOGO_MEMORY";
	bitmap_name_table[PP2_BITMAP_MENU_LOGO] = "MENU_LOGO";
	bitmap_name_table[PP2_BITMAP_TITLE_LOGO] = "TITLE_LOGO";
	bitmap_name_table[PP2_BITMAP_TARGET] = "TARGET";
	bitmap_name_table[PP2_BITMAP_EMPTY_PLAYER] = "EMPTY_PLAYER";
	bitmap_name_table[PP2_BITMAP_RADAR_BLIP] = "RADAR_BLIP";
	bitmap_name_table[PP2_BITMAP_TYPING] = "TYPING";
	bitmap_name_table[PP2_BITMAP_HIGHLIGHT] = "HIGHLIGHT";
	bitmap_name_table[PP2_BITMAP_HUD_SCORE] = "HUD_SCORE";
	bitmap_name_table[PP2_BITMAP_HUD_LIVES] = "HUD_LIVES";
	bitmap_name_table[PP2_BITMAP_HUD_TIMER] = "HUD_TIMER";
	bitmap_name_table[PP2_BITMAP_HUD_AMMO] = "HUD_AMMO";
	bitmap_name_table[PP2_BITMAP_SCREEN_COPY] = "SCREEN_COPY";
	bitmap_name_table[PP2_BITMAP_LOADING] = "LOADING";

	animation_name_table[PP2_ANIMATION_HUD_AMMO_NORMAL] = "HUD_AMMO_NORMAL";
	animation_name_table[PP2_ANIMATION_HUD_AMMO_SPLITTER] = "HUD_AMMO_SPLITTER";
	animation_name_table[PP2_ANIMATION_HUD_AMMO_MINE] = "HUD_AMMO_MINE";
	animation_name_table[PP2_ANIMATION_HUD_AMMO_BOUNCER] = "HUD_AMMO_BOUNCER";
	animation_name_table[PP2_ANIMATION_HUD_AMMO_SEEKER] = "HUD_AMMO_SEEKER";
	animation_name_table[PP2_ANIMATION_HUD_AMMO_REFLECTOR] = "HUD_AMMO_REFLECTOR";
	animation_name_table[PP2_ANIMATION_HUD_AMMO_PMINE] = "HUD_AMMO_PMINE";
	animation_name_table[PP2_ANIMATION_HUD_AMMO_GHOST] = "HUD_AMMO_GHOST";

	font_name_table[PP2_FONT_SMALL] = "MESSAGE";
	font_name_table[PP2_FONT_COMIC_16] = "UI_LARGE";
	font_name_table[PP2_FONT_HUD] = "HUD";
	font_name_table[PP2_FONT_COMIC_10] = "UI_VERY_SMALL";
	font_name_table[PP2_FONT_COMIC_12] = "UI_SMALL";
	font_name_table[PP2_FONT_COMIC_14] = "UI_MEDIUM";
	font_name_table[PP2_FONT_WEAPON_INFO] = "WEAPON_INFO";

	object_name_table[PP2_OBJECT_PORTAL] = "PORTAL";
	object_name_table[PP2_OBJECT_GENERATOR] = "GENERATOR";
	object_name_table[PP2_OBJECT_AMMO_NORMAL] = "AMMO_NORMAL";
	object_name_table[PP2_OBJECT_AMMO_X] = "AMMO_X";
	object_name_table[PP2_OBJECT_AMMO_MINE] = "AMMO_MINE";
	object_name_table[PP2_OBJECT_AMMO_BOUNCE] = "AMMO_BOUNCE";
	object_name_table[PP2_OBJECT_AMMO_SEEK] = "AMMO_SEEK";
	object_name_table[PP2_OBJECT_POWER_CLOAK] = "POWER_CLOAK";
	object_name_table[PP2_OBJECT_POWER_JUMP] = "POWER_JUMP";
	object_name_table[PP2_OBJECT_POWER_RUN] = "POWER_RUN";
	object_name_table[PP2_OBJECT_POWER_DEFLECT] = "POWER_DEFLECT";
	object_name_table[PP2_OBJECT_FLAG] = "FLAG";
	object_name_table[PP2_OBJECT_FLAG_PORTAL] = "FLAG_PORTAL";
	object_name_table[PP2_OBJECT_BASE] = "BASE";
	object_name_table[PP2_OBJECT_BASE_PORTAL] = "BASE_PORTAL";
	object_name_table[PP2_OBJECT_BANK] = "BANK";
	object_name_table[PP2_OBJECT_GEM_1] = "GEM_1";
	object_name_table[PP2_OBJECT_GEM_2] = "GEM_2";
	object_name_table[PP2_OBJECT_GEM_3] = "GEM_3";
	object_name_table[PP2_OBJECT_GEM_4] = "GEM_4";
	object_name_table[PP2_OBJECT_GEM_5] = "GEM_5";
	object_name_table[PP2_OBJECT_HUNT_PORTAL] = "HUNT_PORTAL";
	object_name_table[PP2_OBJECT_HUNT_BASE] = "HUNT_BASE";
	object_name_table[PP2_OBJECT_TARGET] = "TARGET";
	object_name_table[PP2_OBJECT_TARGET_PORTAL] = "TARGET_PORTAL";
	object_name_table[PP2_OBJECT_POWER_POOF] = "POWER_POOF";
	object_name_table[PP2_OBJECT_GEM_POOF] = "GEM_POOF";
	object_name_table[PP2_OBJECT_POOF] = "POOF";
	object_name_table[PP2_OBJECT_POWER_FLY] = "POWER_FLY";
	object_name_table[PP2_OBJECT_JET] = "JET";
	object_name_table[PP2_OBJECT_SPRING_UP] = "SPRING_UP";
	object_name_table[PP2_OBJECT_SPRING_DOWN] = "SPRING_DOWN";
	object_name_table[PP2_OBJECT_SPRING_LEFT] = "SPRING_LEFT";
	object_name_table[PP2_OBJECT_SPRING_RIGHT] = "SPRING_RIGHT";
	object_name_table[PP2_OBJECT_SPRING_BAR_V] = "SPRING_BAR_V";
	object_name_table[PP2_OBJECT_SPRING_BAR_H] = "SPRING_BAR_H";
	object_name_table[PP2_OBJECT_POWER_TURBO] = "POWER_TURBO";
	object_name_table[PP2_OBJECT_AMMO_REFLECTOR] = "AMMO_REFLECTOR";
	object_name_table[PP2_OBJECT_AMMO_PMINE] = "AMMO_PMINE";
	object_name_table[PP2_OBJECT_AMMO_GHOST] = "AMMO_GHOST";
	object_name_table[PP2_OBJECT_COIN] = "COIN";

	sample_name_table[PP2_SAMPLE_FIRE] = "FIRE";
	sample_name_table[PP2_SAMPLE_SPLAT] = "SPLAT";
	sample_name_table[PP2_SAMPLE_RELOAD_A] = "RELOAD_A";
	sample_name_table[PP2_SAMPLE_RELOAD_B] = "RELOAD_B";
	sample_name_table[PP2_SAMPLE_AMMO] = "AMMO";
	sample_name_table[PP2_SAMPLE_CLOAK] = "CLOAK";
	sample_name_table[PP2_SAMPLE_GEM] = "GEM";
	sample_name_table[PP2_SAMPLE_JUMP] = "JUMP";
	sample_name_table[PP2_SAMPLE_LAND] = "LAND";
	sample_name_table[PP2_SAMPLE_HIT] = "HIT";
	sample_name_table[PP2_SAMPLE_BUMP] = "BUMP";
	sample_name_table[PP2_SAMPLE_TIME] = "TIME";
	sample_name_table[PP2_SAMPLE_MENU_PICK] = "MENU_PICK";
	sample_name_table[PP2_SAMPLE_MENU_NEXT] = "MENU_NEXT";
	sample_name_table[PP2_SAMPLE_TELE_IN] = "TELE_IN";
	sample_name_table[PP2_SAMPLE_TELE_OUT] = "TELE_OUT";
	sample_name_table[PP2_SAMPLE_RUN] = "RUN";
	sample_name_table[PP2_SAMPLE_DEFLECT] = "DEFLECT";
	sample_name_table[PP2_SAMPLE_FLAG] = "FLAG";
	sample_name_table[PP2_SAMPLE_BANK] = "BANK";
	sample_name_table[PP2_SAMPLE_BASE] = "BASE";
	sample_name_table[PP2_SAMPLE_RESPAWN] = "RESPAWN";
	sample_name_table[PP2_SAMPLE_PJUMP] = "PJUMP";
	sample_name_table[PP2_SAMPLE_CRUSH] = "CRUSH";
	sample_name_table[PP2_SAMPLE_TARGET] = "TARGET";
	sample_name_table[PP2_SAMPLE_BOUNCE] = "BOUNCE";
	sample_name_table[PP2_SAMPLE_RICOCHET] = "RICOCHET";
	sample_name_table[PP2_SAMPLE_START] = "START";
	sample_name_table[PP2_SAMPLE_WIN] = "WIN";
	sample_name_table[PP2_SAMPLE_ENTER] = "ENTER";
	sample_name_table[PP2_SAMPLE_FLY] = "FLY";
	sample_name_table[PP2_SAMPLE_PFLY] = "PFLY";
	sample_name_table[PP2_SAMPLE_SPRING] = "SPRING";
	sample_name_table[PP2_SAMPLE_PTURBO] = "PTURBO";
	sample_name_table[PP2_SAMPLE_COIN_LAND] = "COIN_LAND";
	sample_name_table[PP2_SAMPLE_COIN_PICKUP] = "COIN_PICKUP";
	sample_name_table[PP2_SAMPLE_LOGO_TICK] = "LOGO_TICK";
	sample_name_table[PP2_SAMPLE_LOGO] = "LOGO";

	theme_object_name_table[PP2_THEME_OBJECT_HUD_SCORE] = "HUD_SCORE";
	theme_object_name_table[PP2_THEME_OBJECT_HUD_SCORE_TEXT] = "HUD_SCORE_TEXT";
	theme_object_name_table[PP2_THEME_OBJECT_HUD_AMMO] = "HUD_AMMO";
	theme_object_name_table[PP2_THEME_OBJECT_HUD_AMMO_TYPE] = "HUD_AMMO_TYPE";
	theme_object_name_table[PP2_THEME_OBJECT_HUD_AMMO_TEXT] = "HUD_AMMO_TEXT";
	theme_object_name_table[PP2_THEME_OBJECT_HUD_TIMER] = "HUD_TIMER";
	theme_object_name_table[PP2_THEME_OBJECT_HUD_TIMER_TEXT] = "PP2_THEME_OBJECT_HUD_TIMER_TEXT";
}

const char * get_val_fallback(PP2_THEME * base_theme, PP2_THEME * theme, const char * section, const char * key)
{
	const char * val = NULL;

	if(theme && theme->config)
	{
		val = al_get_config_value(theme->config, section, key);
	}
	if(!val && base_theme && base_theme->config)
	{
		val = al_get_config_value(base_theme->config, section, key);
	}
	if(val && !strlen(val))
	{
		return NULL;
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

static bool pp2_load_theme_object(PP2_THEME * base_theme, PP2_THEME * theme, int theme_object)
{
	const char * val;
	char buf[256];

	sprintf(buf, "%s_X", theme_object_name_table[theme_object]);
	val = get_val_fallback(base_theme, theme, "Theme Objects", buf);
	if(val)
	{
		theme->object[theme_object].x = atof(val);
	}
	sprintf(buf, "%s_Y", theme_object_name_table[theme_object]);
	val = get_val_fallback(base_theme, theme, "Theme Objects", buf);
	if(val)
	{
		theme->object[theme_object].y = atof(val);
	}
	return true;
}

PP2_THEME * pp2_load_theme(PP2_THEME * base_theme, const char * fn)
{
	PP2_THEME * tp;
	const char * val;
	char buf[256];
	int i;

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
			if(!base_theme)
			{
				goto fail;
			}
		}
	}
	init_name_tables();
	for(i = 0; i < PP2_MAX_BITMAPS; i++)
	{
		if(bitmap_name_table[i])
		{
			val = get_val_fallback(base_theme, tp, "Bitmaps", bitmap_name_table[i]);
			if(val)
			{
				tp->bitmap_fn[i] = val;
				tp->bitmap_option[i] = 1;
				sprintf(buf, "%s_OPTION", bitmap_name_table[i]);
				val = get_val_fallback(base_theme, tp, "Bitmaps", buf);
				if(val)
				{
					tp->bitmap_option[i] = atoi(val);
				}
			}
		}
	}
	for(i = 0; i < PP2_MAX_ANIMATIONS; i++)
	{
		if(animation_name_table[i])
		{
			val = get_val_fallback(base_theme, tp, "Animations", animation_name_table[i]);
			if(val)
			{
				tp->animation_fn[i] = val;
			}
		}
	}
	for(i = 0; i < PP2_MAX_FONTS; i++)
	{
		if(font_name_table[i])
		{
			val = get_val_fallback(base_theme, tp, "Fonts", font_name_table[i]);
			if(val)
			{
				tp->font_fn[i] = val;
			}
		}
	}
	for(i = 0; i < PP2_MAX_OBJECT_TYPES; i++)
	{
		if(object_name_table[i])
		{
			val = get_val_fallback(base_theme, tp, "Objects", object_name_table[i]);
			if(val)
			{
				tp->object_animation_fn[i] = val;
			}
		}
	}
	for(i = 0; i < PP2_MAX_SAMPLES; i++)
	{
		if(sample_name_table[i])
		{
			val = get_val_fallback(base_theme, tp, "Samples", sample_name_table[i]);
			if(val)
			{
				tp->sample_fn[i] = val;
			}
		}
	}
	for(i = 0; i < PP2_THEME_MAX_OBJECTS; i++)
	{
		if(theme_object_name_table[i])
		{
			pp2_load_theme_object(base_theme, tp, i);
		}
	}
	tp->theme_music_fn = get_val_fallback(base_theme, tp, "Music", "theme");
	tp->menu_music_fn = get_val_fallback(base_theme, tp, "Music", "menu");
	tp->victory_music_fn = get_val_fallback(base_theme, tp, "Music", "victory");
	tp->defeat_music_fn = get_val_fallback(base_theme, tp, "Music", "defeat");
	tp->menu_bg_color = t3f_color_white;
	val = get_val_fallback(base_theme, tp, "UI", "menu_bg_color");
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
