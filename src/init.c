#include "t3f/resource.h"
#include "t3f/file_utils.h"
#include "pp2.h"
#include "version.h"
#include "legacy/animation.h"
#include "gameplay/network.h"
#include "file/database/database.h"
#include "file/database/character.h"
#include "file/database/level.h"
#include "resource.h"
#include "gameplay/sprites/object_defines.h"
#include "legacy/palette.h"
#include "legacy/font.h"

static int pp2_default_keys[4][9] =
{
	{	ALLEGRO_KEY_UP, ALLEGRO_KEY_DOWN, ALLEGRO_KEY_LEFT, ALLEGRO_KEY_RIGHT, ALLEGRO_KEY_FULLSTOP, ALLEGRO_KEY_SLASH, ALLEGRO_KEY_QUOTE, ALLEGRO_KEY_SPACE, 0},
	{	ALLEGRO_KEY_W,     ALLEGRO_KEY_S,    ALLEGRO_KEY_A,     ALLEGRO_KEY_D,        ALLEGRO_KEY_Q,     ALLEGRO_KEY_1,     ALLEGRO_KEY_3,     ALLEGRO_KEY_2, 0},
	{	ALLEGRO_KEY_T,     ALLEGRO_KEY_G,    ALLEGRO_KEY_F,     ALLEGRO_KEY_H,        ALLEGRO_KEY_R,     ALLEGRO_KEY_4,     ALLEGRO_KEY_6,     ALLEGRO_KEY_5, 0},
	{	ALLEGRO_KEY_I,     ALLEGRO_KEY_K,    ALLEGRO_KEY_J,     ALLEGRO_KEY_L,        ALLEGRO_KEY_U,     ALLEGRO_KEY_7,     ALLEGRO_KEY_9,     ALLEGRO_KEY_8, 0}
};

static char * pp2_button_name[9] = {"Up", "Down", "Left", "Right", "Jump", "Fire", "Select", "Strafe", "Show Scores"};

static char pp2_load_text[1024] = {0};

static int pp2_load_counter = 0;
static char pp2_itoa_string[1024] = {0};

void pp2_show_load_screen(const char * text, PP2_RESOURCES * resources)
{
	ALLEGRO_STATE old_state;
	ALLEGRO_TRANSFORM transform;

	al_store_state(&old_state, ALLEGRO_STATE_TRANSFORM);
	al_identity_transform(&transform);
	al_use_transform(&transform);
	al_clear_to_color(al_map_rgba_f(0.0, 0.0, 0.0, 1.0));
	if(resources->bitmap[PP2_BITMAP_LOADING])
	{
		al_draw_tinted_bitmap(resources->bitmap[PP2_BITMAP_LOADING], al_map_rgba(255 - pp2_load_counter % 256, 255 - pp2_load_counter % 256, 255 - pp2_load_counter % 256, 255 - pp2_load_counter % 256), al_get_display_width(t3f_display) / 2 - al_get_bitmap_width(resources->bitmap[PP2_BITMAP_LOADING]) / 2, al_get_display_height(t3f_display) / 2 - al_get_bitmap_height(resources->bitmap[PP2_BITMAP_LOADING]) / 2, 0);
	}
	if(text)
	{
		strcpy(pp2_load_text, text);
	}
	if(resources->font[PP2_FONT_SMALL])
	{
		t3f_draw_text(resources->font[PP2_FONT_SMALL], al_map_rgba(255, 255, 255, 255), al_get_display_width(t3f_display) / 2, al_get_display_height(t3f_display) / 2 + al_get_bitmap_height(resources->bitmap[PP2_BITMAP_LOADING]) / 2, 0, ALLEGRO_ALIGN_CENTRE, pp2_load_text);
	}
	pp2_load_counter++;
	al_flip_display();
	al_restore_state(&old_state);
}

void pp2_database_callback(const ALLEGRO_PATH * pp)
{
//	pp2_show_load_screen();
}

void pp2_setup_directories(PP2_RESOURCES * resources)
{
	ALLEGRO_PATH * temp_path = NULL;

	pp2_show_load_screen("Creating directory structure", resources);
	temp_path = al_clone_path(t3f_data_path);
	al_append_path_component(temp_path, "replays");
	t3f_setup_directories(temp_path);
	al_destroy_path(temp_path);
	temp_path = al_clone_path(t3f_data_path);
	al_append_path_component(temp_path, "levels");
	t3f_setup_directories(temp_path);
	al_destroy_path(temp_path);
	temp_path = al_clone_path(t3f_data_path);
	al_append_path_component(temp_path, "characters");
	t3f_setup_directories(temp_path);
	al_destroy_path(temp_path);
	temp_path = al_clone_path(t3f_data_path);
	al_append_path_component(temp_path, "preview_cache");
	al_append_path_component(temp_path, "levels");
	t3f_setup_directories(temp_path);
	al_destroy_path(temp_path);
	temp_path = al_clone_path(t3f_data_path);
	al_append_path_component(temp_path, "preview_cache");
	al_append_path_component(temp_path, "characters");
	t3f_setup_directories(temp_path);
	al_destroy_path(temp_path);
	temp_path = al_clone_path(t3f_data_path);
	al_append_path_component(temp_path, "music");
	t3f_setup_directories(temp_path);
	al_destroy_path(temp_path);
}

static char * pp2_itoa(int i)
{
	sprintf(pp2_itoa_string, "%d", i);
	return pp2_itoa_string;
}

void pp2_set_controller_config(PP2_INTERFACE * ip, int controller, int binding)
{
	char temp_string[256];
	char temp_string2[256];

	sprintf(temp_string, "Controller %d", controller);
	sprintf(temp_string2, "%s Type", pp2_button_name[binding]);
	al_set_config_value(ip->config, temp_string, temp_string2, pp2_itoa(ip->controller[controller]->binding[binding].type));
	sprintf(temp_string2, "%s SubType", pp2_button_name[binding]);
	al_set_config_value(ip->config, temp_string, temp_string2, pp2_itoa(ip->controller[controller]->binding[binding].sub_type));
	sprintf(temp_string2, "%s Joystick", pp2_button_name[binding]);
	al_set_config_value(ip->config, temp_string, temp_string2, pp2_itoa(ip->controller[controller]->binding[binding].joystick));
	sprintf(temp_string2, "%s Button", pp2_button_name[binding]);
	al_set_config_value(ip->config, temp_string, temp_string2, pp2_itoa(ip->controller[controller]->binding[binding].button));
	sprintf(temp_string2, "%s Stick", pp2_button_name[binding]);
	al_set_config_value(ip->config, temp_string, temp_string2, pp2_itoa(ip->controller[controller]->binding[binding].stick));
	sprintf(temp_string2, "%s Axis", pp2_button_name[binding]);
	al_set_config_value(ip->config, temp_string, temp_string2, pp2_itoa(ip->controller[controller]->binding[binding].axis));
	sprintf(temp_string2, "%s Flags", pp2_button_name[binding]);
	al_set_config_value(ip->config, temp_string, temp_string2, pp2_itoa(ip->controller[controller]->binding[binding].flags));
}

void pp2_autodetect_controllers(PP2_INTERFACE * ip)
{
	int i, c;

	/* use all joysticks */
	c = al_get_num_joysticks();
	for(i = 0; i < c; i++)
	{
		ip->controller[i]->binding[0].type = T3F_CONTROLLER_BINDING_JOYSTICK_AXIS;
		ip->controller[i]->binding[0].joystick = i;
		ip->controller[i]->binding[0].stick = 0;
		ip->controller[i]->binding[0].axis = 1;
		ip->controller[i]->binding[0].flags = T3F_CONTROLLER_FLAG_AXIS_NEGATIVE;
		pp2_set_controller_config(ip, i, 0);
		ip->controller[i]->binding[1].type = T3F_CONTROLLER_BINDING_JOYSTICK_AXIS;
		ip->controller[i]->binding[1].joystick = i;
		ip->controller[i]->binding[1].stick = 0;
		ip->controller[i]->binding[1].axis = 1;
		ip->controller[i]->binding[1].flags = T3F_CONTROLLER_FLAG_AXIS_POSITIVE;
		pp2_set_controller_config(ip, i, 1);
		ip->controller[i]->binding[2].type = T3F_CONTROLLER_BINDING_JOYSTICK_AXIS;
		ip->controller[i]->binding[2].joystick = i;
		ip->controller[i]->binding[2].stick = 0;
		ip->controller[i]->binding[2].axis = 0;
		ip->controller[i]->binding[2].flags = T3F_CONTROLLER_FLAG_AXIS_NEGATIVE;
		pp2_set_controller_config(ip, i, 2);
		ip->controller[i]->binding[3].type = T3F_CONTROLLER_BINDING_JOYSTICK_AXIS;
		ip->controller[i]->binding[3].joystick = i;
		ip->controller[i]->binding[3].stick = 0;
		ip->controller[i]->binding[3].axis = 0;
		ip->controller[i]->binding[3].flags = T3F_CONTROLLER_FLAG_AXIS_POSITIVE;
		pp2_set_controller_config(ip, i, 3);
		ip->controller[i]->binding[4].type = T3F_CONTROLLER_BINDING_JOYSTICK_BUTTON;
		ip->controller[i]->binding[4].joystick = i;
		ip->controller[i]->binding[4].stick = 0;
		ip->controller[i]->binding[4].button = 0;
		ip->controller[i]->binding[4].flags = 0;
		pp2_set_controller_config(ip, i, 4);
		ip->controller[i]->binding[5].type = T3F_CONTROLLER_BINDING_JOYSTICK_BUTTON;
		ip->controller[i]->binding[5].joystick = i;
		ip->controller[i]->binding[5].stick = 0;
		ip->controller[i]->binding[5].button = 1;
		ip->controller[i]->binding[5].flags = 0;
		pp2_set_controller_config(ip, i, 5);
		ip->controller[i]->binding[6].type = T3F_CONTROLLER_BINDING_JOYSTICK_BUTTON;
		ip->controller[i]->binding[6].joystick = i;
		ip->controller[i]->binding[6].stick = 0;
		ip->controller[i]->binding[6].button = 2;
		ip->controller[i]->binding[6].flags = 0;
		pp2_set_controller_config(ip, i, 6);
		ip->controller[i]->binding[7].type = T3F_CONTROLLER_BINDING_JOYSTICK_BUTTON;
		ip->controller[i]->binding[7].joystick = i;
		ip->controller[i]->binding[7].stick = 0;
		ip->controller[i]->binding[7].button = 3;
		ip->controller[i]->binding[7].flags = 0;
		pp2_set_controller_config(ip, i, 7);
		ip->controller[i]->binding[8].type = T3F_CONTROLLER_BINDING_JOYSTICK_BUTTON;
		ip->controller[i]->binding[8].joystick = i;
		ip->controller[i]->binding[8].stick = 0;
		ip->controller[i]->binding[8].button = 4;
		ip->controller[i]->binding[8].flags = 0;
		pp2_set_controller_config(ip, i, 8);
	}

	/* fill remaining controllers with default keys */
	for(i = c; i < 4; i++)
	{
		ip->controller[i]->binding[0].type = T3F_CONTROLLER_BINDING_KEY;
		ip->controller[i]->binding[0].button = pp2_default_keys[i - c][0];
		pp2_set_controller_config(ip, i, 0);
		ip->controller[i]->binding[1].type = T3F_CONTROLLER_BINDING_KEY;
		ip->controller[i]->binding[1].button = pp2_default_keys[i - c][1];
		pp2_set_controller_config(ip, i, 1);
		ip->controller[i]->binding[2].type = T3F_CONTROLLER_BINDING_KEY;
		ip->controller[i]->binding[2].button = pp2_default_keys[i - c][2];
		pp2_set_controller_config(ip, i, 2);
		ip->controller[i]->binding[3].type = T3F_CONTROLLER_BINDING_KEY;
		ip->controller[i]->binding[3].button = pp2_default_keys[i - c][3];
		pp2_set_controller_config(ip, i, 3);
		ip->controller[i]->binding[4].type = T3F_CONTROLLER_BINDING_KEY;
		ip->controller[i]->binding[4].button = pp2_default_keys[i - c][4];
		pp2_set_controller_config(ip, i, 4);
		ip->controller[i]->binding[5].type = T3F_CONTROLLER_BINDING_KEY;
		ip->controller[i]->binding[5].button = pp2_default_keys[i - c][5];
		pp2_set_controller_config(ip, i, 5);
		ip->controller[i]->binding[6].type = T3F_CONTROLLER_BINDING_KEY;
		ip->controller[i]->binding[6].button = pp2_default_keys[i - c][6];
		pp2_set_controller_config(ip, i, 6);
		ip->controller[i]->binding[7].type = T3F_CONTROLLER_BINDING_KEY;
		ip->controller[i]->binding[7].button = pp2_default_keys[i - c][7];
		pp2_set_controller_config(ip, i, 7);
		ip->controller[i]->binding[8].type = T3F_CONTROLLER_BINDING_KEY;
		ip->controller[i]->binding[8].button = pp2_default_keys[i - c][8];
		pp2_set_controller_config(ip, i, 8);
	}
}

static bool load_font(PP2_THEME * theme, PP2_RESOURCES * resources, int font)
{
	const char * extension;

	if(theme->font_fn[font])
	{
		extension = t3f_get_path_extension(theme->font_fn[font]);
		if(!strcmp(extension, ".mft"))
		{
			t3f_load_resource((void **)&resources->font[font], pp2_legacy_font_resource_handler_proc, theme->font_fn[font], 1, 0, 0);
		}
		else
		{
			t3f_load_resource((void **)&resources->font[font], t3f_font_resource_handler_proc, theme->font_fn[font], 1, 0, 0);
		}
		if(!resources->font[font])
		{
			printf("Failed to load font %d (%s)!\n", font, theme->font_fn[font]);
			return false;
		}
		return true;
	}
	else
	{
		printf("No path for font %d!\n", font);
	}
	return false;
}

bool pp2_load_fonts(PP2_THEME * theme, PP2_RESOURCES * resources)
{
	if(!load_font(theme, resources, PP2_FONT_COMIC_16))
	{
		return false;
	}
	if(!load_font(theme, resources, PP2_FONT_HUD))
	{
		return false;
	}
	if(!load_font(theme, resources, PP2_FONT_COMIC_10))
	{
		return false;
	}
	if(!load_font(theme, resources, PP2_FONT_COMIC_12))
	{
		return false;
	}
	if(!load_font(theme, resources, PP2_FONT_COMIC_14))
	{
		return false;
	}
	return true;
}

/* make "magic pink" transparent and grays different levels of alpha */
static void convert_pink_to_alpha(ALLEGRO_BITMAP * bitmap)
{
	int x, y;
	unsigned char ir, ig, ib, ia;
	ALLEGRO_COLOR pixel;
	ALLEGRO_STATE old_state;

	if(!al_lock_bitmap(bitmap, al_get_bitmap_format(bitmap), 0))
	{
		return;
	}

	al_store_state(&old_state, ALLEGRO_STATE_TARGET_BITMAP);
	al_set_target_bitmap(bitmap);

	for(y = 0; y < al_get_bitmap_height(bitmap); y++)
	{
		for(x = 0; x < al_get_bitmap_width(bitmap); x++)
		{
			pixel = al_get_pixel(bitmap, x, y);
			al_unmap_rgba(pixel, &ir, &ig, &ib, &ia);
			if(ir == 255 && ig == 0 && ib == 255)
			{
				pixel = al_map_rgba(0, 0, 0, 0);
				al_put_pixel(x, y, pixel);
			}
		}
	}

	al_restore_state(&old_state);
	al_unlock_bitmap(bitmap);
}

static bool load_bitmap(PP2_THEME * theme, PP2_RESOURCES * resources, int bitmap, bool optional)
{
	ALLEGRO_STATE old_state;
	bool ret = optional;
	const char * extension;
	bool legacy = false;

	if(theme->bitmap_fn[bitmap])
	{
		extension = t3f_get_path_extension(theme->bitmap_fn[bitmap]);
		if(!strcmp(extension, ".pcx"))
		{
			legacy = true;
		}
		if(legacy)
		{
			al_store_state(&old_state, ALLEGRO_STATE_NEW_BITMAP_PARAMETERS);
			al_set_new_bitmap_flags(0);
		}
		t3f_load_resource((void **)&resources->bitmap[bitmap], t3f_bitmap_resource_handler_proc, theme->bitmap_fn[bitmap], 0, 0, 0);
		if(legacy)
		{
			al_restore_state(&old_state);
		}
		if(!resources->bitmap[bitmap])
		{
			printf("Failed to load bitmap %d (%s)!\n", bitmap, theme->bitmap_fn[bitmap]);
			ret = optional;
		}
		else
		{
			if(legacy && resources->bitmap[bitmap])
			{
				convert_pink_to_alpha(resources->bitmap[bitmap]);
				t3f_resize_bitmap(&resources->bitmap[bitmap], al_get_bitmap_width(resources->bitmap[bitmap]) * 2, al_get_bitmap_height(resources->bitmap[bitmap]) * 2, false, al_get_new_bitmap_flags() | ALLEGRO_NO_PRESERVE_TEXTURE);
			}
		}
		return true;
	}
	else
	{
		printf("No path for bitmap %d!\n", bitmap);
	}
	return ret;
}

bool pp2_load_images(PP2_THEME * theme, PP2_RESOURCES * resources)
{
	ALLEGRO_STATE old_state;

	if(!load_bitmap(theme, resources, PP2_BITMAP_T3_LOGO, false))
	{
		return false;
	}
	if(!load_bitmap(theme, resources, PP2_BITMAP_TITLE_SPLAT, false))
	{
		return false;
	}
	if(!load_bitmap(theme, resources, PP2_BITMAP_TITLE_LOGO, false))
	{
		return false;
	}
	if(!load_bitmap(theme, resources, PP2_BITMAP_EMPTY_PLAYER, false))
	{
		return false;
	}
	if(!load_bitmap(theme, resources, PP2_BITMAP_MENU_BG, false))
	{
		return false;
	}
	if(!load_bitmap(theme, resources, PP2_BITMAP_MENU_LOGO, false))
	{
		return false;
	}
	if(!load_bitmap(theme, resources, PP2_BITMAP_TARGET, false))
	{
		return false;
	}
	if(!load_bitmap(theme, resources, PP2_BITMAP_RADAR_BLIP, false))
	{
		return false;
	}
	if(!load_bitmap(theme, resources, PP2_BITMAP_TYPING, false))
	{
		return false;
	}
	if(!load_bitmap(theme, resources, PP2_BITMAP_HIGHLIGHT, false))
	{
		return false;
	}
	if(!load_bitmap(theme, resources, PP2_BITMAP_HUD_SCORE, true))
	{
		return false;
	}
	if(!load_bitmap(theme, resources, PP2_BITMAP_HUD_LIVES, true))
	{
		return false;
	}
	if(!load_bitmap(theme, resources, PP2_BITMAP_HUD_TIMER, true))
	{
		return false;
	}
	if(!load_bitmap(theme, resources, PP2_BITMAP_HUD_AMMO, true))
	{
		return false;
	}
	al_store_state(&old_state, ALLEGRO_STATE_NEW_BITMAP_PARAMETERS);
	al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
	if(!load_bitmap(theme, resources, PP2_BITMAP_T3_LOGO_MEMORY, false))
	{
		return false;
	}
	al_restore_state(&old_state);
	return true;
}

static bool load_sample(PP2_THEME * theme, PP2_RESOURCES * resources, int sample)
{
	if(theme->sample_fn[sample])
	{
		resources->sample[sample] = al_load_sample(theme->sample_fn[sample]);
		if(!resources->sample[sample])
		{
			printf("Failed to load sample %d (%s)!\n", sample, theme->sample_fn[sample]);
			return false;
		}
		return true;
	}
	else
	{
		printf("No path for sample %d!\n", sample);
	}
	return false;
}

bool pp2_load_sounds(PP2_THEME * theme, PP2_RESOURCES * resources)
{
	if(!load_sample(theme, resources, PP2_SAMPLE_MENU_PICK))
	{
		return false;
	}
	if(!load_sample(theme, resources, PP2_SAMPLE_MENU_NEXT))
	{
		return false;
	}
	if(!load_sample(theme, resources, PP2_SAMPLE_LOGO))
	{
		return false;
	}
	if(!load_sample(theme, resources, PP2_SAMPLE_LOGO_TICK))
	{
		return false;
	}
	if(!load_sample(theme, resources, PP2_SAMPLE_FIRE))
	{
		return false;
	}
	if(!load_sample(theme, resources, PP2_SAMPLE_SPLAT))
	{
		return false;
	}
	if(!load_sample(theme, resources, PP2_SAMPLE_RELOAD_A))
	{
		return false;
	}
	if(!load_sample(theme, resources, PP2_SAMPLE_RELOAD_B))
	{
		return false;
	}
	if(!load_sample(theme, resources, PP2_SAMPLE_AMMO))
	{
		return false;
	}
	if(!load_sample(theme, resources, PP2_SAMPLE_CLOAK))
	{
		return false;
	}
	if(!load_sample(theme, resources, PP2_SAMPLE_GEM))
	{
		return false;
	}
	if(!load_sample(theme, resources, PP2_SAMPLE_JUMP))
	{
		return false;
	}
	if(!load_sample(theme, resources, PP2_SAMPLE_LAND))
	{
		return false;
	}
	if(!load_sample(theme, resources, PP2_SAMPLE_HIT))
	{
		return false;
	}
	if(!load_sample(theme, resources, PP2_SAMPLE_BUMP))
	{
		return false;
	}
	if(!load_sample(theme, resources, PP2_SAMPLE_TIME))
	{
		return false;
	}
	if(!load_sample(theme, resources, PP2_SAMPLE_TELE_IN))
	{
		return false;
	}
	if(!load_sample(theme, resources, PP2_SAMPLE_TELE_OUT))
	{
		return false;
	}
	if(!load_sample(theme, resources, PP2_SAMPLE_RUN))
	{
		return false;
	}
	if(!load_sample(theme, resources, PP2_SAMPLE_DEFLECT))
	{
		return false;
	}
	if(!load_sample(theme, resources, PP2_SAMPLE_FLAG))
	{
		return false;
	}
	if(!load_sample(theme, resources, PP2_SAMPLE_BANK))
	{
		return false;
	}
	if(!load_sample(theme, resources, PP2_SAMPLE_BASE))
	{
		return false;
	}
	if(!load_sample(theme, resources, PP2_SAMPLE_RESPAWN))
	{
		return false;
	}
	if(!load_sample(theme, resources, PP2_SAMPLE_PJUMP))
	{
		return false;
	}
	if(!load_sample(theme, resources, PP2_SAMPLE_FLY))
	{
		return false;
	}
	if(!load_sample(theme, resources, PP2_SAMPLE_PFLY))
	{
		return false;
	}
	if(!load_sample(theme, resources, PP2_SAMPLE_PTURBO))
	{
		return false;
	}
	if(!load_sample(theme, resources, PP2_SAMPLE_CRUSH))
	{
		return false;
	}
	if(!load_sample(theme, resources, PP2_SAMPLE_TARGET))
	{
		return false;
	}
	if(!load_sample(theme, resources, PP2_SAMPLE_BOUNCE))
	{
		return false;
	}
	if(!load_sample(theme, resources, PP2_SAMPLE_RICOCHET))
	{
		return false;
	}
	if(!load_sample(theme, resources, PP2_SAMPLE_SPRING))
	{
		return false;
	}
	if(!load_sample(theme, resources, PP2_SAMPLE_COIN_LAND))
	{
		return false;
	}
	if(!load_sample(theme, resources, PP2_SAMPLE_COIN_PICKUP))
	{
		return false;
	}
	return true;
}

static bool load_animation(PP2_THEME * theme, PP2_RESOURCES * resources, int object)
{
	const char * extension;

	if(theme->animation_fn[object])
	{
		extension = t3f_get_path_extension(theme->animation_fn[object]);
		if(!strcmp(extension, ".t3a"))
		{
			resources->animation[object] = t3f_load_animation(theme->animation_fn[object]);
		}
		else
		{
			resources->animation[object] = pp2_legacy_load_animation(theme->animation_fn[object]);
		}
		if(!resources->animation[object])
		{
			printf("Error loading animation %d (%s)!\n", object, theme->animation_fn[object]);
			return false;
		}
	}
	else
	{
		printf("No path for animation %d!\n", object);
	}
	return true;
}

static bool load_object_animation(PP2_THEME * theme, PP2_RESOURCES * resources, int object)
{
	const char * extension;

	if(theme->object_animation_fn[object])
	{
		extension = t3f_get_path_extension(theme->object_animation_fn[object]);
		if(!strcmp(extension, ".t3a"))
		{
			resources->object_animation[object] = t3f_load_animation(theme->object_animation_fn[object]);
		}
		else
		{
			resources->object_animation[object] = pp2_legacy_load_animation(theme->object_animation_fn[object]);
		}
		if(!resources->object_animation[object])
		{
			printf("Error loading object animation %d (%s)!\n", object, theme->object_animation_fn[object]);
			return false;
		}
	}
	else
	{
		printf("No path for object animation %d!\n", object);
	}
	return true;
}

bool pp2_load_animations(PP2_THEME * theme, PP2_RESOURCES * resources)
{
	int i;

	if(!load_object_animation(theme, resources, PP2_OBJECT_PORTAL))
	{
		return false;
	}
	if(!load_object_animation(theme, resources, PP2_OBJECT_AMMO_NORMAL))
	{
		return false;
	}
	if(!load_object_animation(theme, resources, PP2_OBJECT_AMMO_X))
	{
		return false;
	}
	if(!load_object_animation(theme, resources, PP2_OBJECT_AMMO_MINE))
	{
		return false;
	}
	if(!load_object_animation(theme, resources, PP2_OBJECT_AMMO_BOUNCE))
	{
		return false;
	}
	if(!load_object_animation(theme, resources, PP2_OBJECT_AMMO_SEEK))
	{
		return false;
	}
	if(!load_object_animation(theme, resources, PP2_OBJECT_AMMO_REFLECTOR))
	{
		return false;
	}
	if(!load_object_animation(theme, resources, PP2_OBJECT_AMMO_PMINE))
	{
		return false;
	}
	if(!load_object_animation(theme, resources, PP2_OBJECT_AMMO_GHOST))
	{
		return false;
	}
	if(!load_object_animation(theme, resources, PP2_OBJECT_POWER_CLOAK))
	{
		return false;
	}
	if(!load_object_animation(theme, resources, PP2_OBJECT_POWER_JUMP))
	{
		return false;
	}
	if(!load_object_animation(theme, resources, PP2_OBJECT_POWER_RUN))
	{
		return false;
	}
	if(!load_object_animation(theme, resources, PP2_OBJECT_POWER_DEFLECT))
	{
		return false;
	}
	if(!load_object_animation(theme, resources, PP2_OBJECT_POWER_FLY))
	{
		return false;
	}
	if(!load_object_animation(theme, resources, PP2_OBJECT_POWER_TURBO))
	{
		return false;
	}
	if(!load_object_animation(theme, resources, PP2_OBJECT_POWER_FLY))
	{
		return false;
	}
	if(!load_object_animation(theme, resources, PP2_OBJECT_SPRING_UP))
	{
		return false;
	}
	if(!load_object_animation(theme, resources, PP2_OBJECT_SPRING_DOWN))
	{
		return false;
	}
	if(!load_object_animation(theme, resources, PP2_OBJECT_SPRING_LEFT))
	{
		return false;
	}
	if(!load_object_animation(theme, resources, PP2_OBJECT_SPRING_RIGHT))
	{
		return false;
	}
	if(!load_object_animation(theme, resources, PP2_OBJECT_SPRING_BAR_V))
	{
		return false;
	}
	if(!load_object_animation(theme, resources, PP2_OBJECT_SPRING_BAR_H))
	{
		return false;
	}
	if(!load_object_animation(theme, resources, PP2_OBJECT_COIN))
	{
		return false;
	}
	if(!load_object_animation(theme, resources, PP2_OBJECT_JET))
	{
		return false;
	}
	if(!load_animation(theme, resources, PP2_ANIMATION_HUD_AMMO_NORMAL))
	{
		return false;
	}
	if(!load_animation(theme, resources, PP2_ANIMATION_HUD_AMMO_SPLITTER))
	{
		return false;
	}
	if(!load_animation(theme, resources, PP2_ANIMATION_HUD_AMMO_MINE))
	{
		return false;
	}
	if(!load_animation(theme, resources, PP2_ANIMATION_HUD_AMMO_BOUNCER))
	{
		return false;
	}
	if(!load_animation(theme, resources, PP2_ANIMATION_HUD_AMMO_SEEKER))
	{
		return false;
	}
	if(!load_animation(theme, resources, PP2_ANIMATION_HUD_AMMO_REFLECTOR))
	{
		return false;
	}
	if(!load_animation(theme, resources, PP2_ANIMATION_HUD_AMMO_PMINE))
	{
		return false;
	}
	if(!load_animation(theme, resources, PP2_ANIMATION_HUD_AMMO_GHOST))
	{
		return false;
	}
	resources->object_atlas = t3f_create_atlas(1024, 1024);
	if(resources->object_atlas)
	{
		for(i = 0; i < PP2_MAX_OBJECT_TYPES; i++)
		{
			if(resources->object_animation[i])
			{
				t3f_add_animation_to_atlas(resources->object_atlas, resources->object_animation[i], T3F_ATLAS_SPRITE);
			}
		}
		t3f_add_bitmap_to_atlas(resources->object_atlas, &resources->bitmap[PP2_BITMAP_RADAR_BLIP], T3F_ATLAS_SPRITE);
		t3f_add_bitmap_to_atlas(resources->object_atlas, &resources->bitmap[PP2_BITMAP_TYPING], T3F_ATLAS_SPRITE);
		t3f_add_bitmap_to_atlas(resources->object_atlas, &resources->bitmap[PP2_BITMAP_HIGHLIGHT], T3F_ATLAS_SPRITE);
	}
	return true;
}

bool pp2_load_resources(PP2_THEME * theme, PP2_RESOURCES * resources)
{
	if(!pp2_legacy_load_palette("data/graphics/legacy_palette.png"))
	{
		return false;
	}
	if(!load_bitmap(theme, resources, PP2_BITMAP_LOADING, false))
	{
		return false;
	}
	if(!load_font(theme, resources, PP2_FONT_SMALL))
	{
		return false;
	}
	pp2_show_load_screen("Loading fonts...", resources);
	if(!pp2_load_fonts(theme, resources))
	{
		return false;
	}
	pp2_show_load_screen("Loading bitmaps...", resources);
	if(!pp2_load_images(theme, resources))
	{
		return false;
	}
	pp2_show_load_screen("Loading animations...", resources);
	if(!pp2_load_animations(theme, resources))
	{
		return false;
	}
	pp2_show_load_screen("Loading audio samples...", resources);
	if(t3f_flags & T3F_USE_SOUND)
	{
		if(!pp2_load_sounds(theme, resources))
		{
			return false;
		}
	}
	return true;
}

bool pp2_setup_joynet(PP2_INSTANCE * instance)
{
	int i;

	if(!joynet_init())
	{
		return false;
	}
	instance->game.client_game = joynet_create_game(PP2_VERSION_STRING, JOYNET_GAME_TYPE_CONTROLLERS, 16, 4, pp2_game_channel_callback, instance);
	if(!instance->game.client_game)
	{
		return false;
	}
	joynet_setup_game_controllers(instance->game.client_game, 8, 0, 256);
	joynet_add_game_option(instance->game.client_game, (int *)(&instance->game.seed));
	joynet_add_game_option(instance->game.client_game, &instance->game.end_game_option);
	joynet_add_game_option(instance->game.client_game, &instance->game.option[PP2_OPTION_GAME_MODE]);
	joynet_add_game_option(instance->game.client_game, &instance->game.option[PP2_OPTION_ELIMINATION_HITS]);
	joynet_add_game_option(instance->game.client_game, &instance->game.option[PP2_OPTION_CTF_FLAGS]);
	joynet_add_game_option(instance->game.client_game, &instance->game.option[PP2_OPTION_TIME_LIMIT]);
	joynet_add_game_option(instance->game.client_game, &instance->game.option[PP2_OPTION_ENABLE_AMMO_NORMAL]);
	joynet_add_game_option(instance->game.client_game, &instance->game.option[PP2_OPTION_ENABLE_AMMO_X]);
	joynet_add_game_option(instance->game.client_game, &instance->game.option[PP2_OPTION_ENABLE_AMMO_MINE]);
	joynet_add_game_option(instance->game.client_game, &instance->game.option[PP2_OPTION_ENABLE_AMMO_BOUNCE]);
	joynet_add_game_option(instance->game.client_game, &instance->game.option[PP2_OPTION_ENABLE_AMMO_SEEK]);
	joynet_add_game_option(instance->game.client_game, &instance->game.option[PP2_OPTION_ENABLE_AMMO_REFLECTOR]);
	joynet_add_game_option(instance->game.client_game, &instance->game.option[PP2_OPTION_ENABLE_AMMO_PMINE]);
	joynet_add_game_option(instance->game.client_game, &instance->game.option[PP2_OPTION_ENABLE_AMMO_GHOST]);
	joynet_add_game_option(instance->game.client_game, &instance->game.option[PP2_OPTION_ENABLE_POWERUP_CLOAK]);
	joynet_add_game_option(instance->game.client_game, &instance->game.option[PP2_OPTION_ENABLE_POWERUP_JUMP]);
	joynet_add_game_option(instance->game.client_game, &instance->game.option[PP2_OPTION_ENABLE_POWERUP_RUN]);
	joynet_add_game_option(instance->game.client_game, &instance->game.option[PP2_OPTION_ENABLE_POWERUP_DEFLECT]);
	joynet_add_game_option(instance->game.client_game, &instance->game.option[PP2_OPTION_ENABLE_POWERUP_FLY]);
	joynet_add_game_option(instance->game.client_game, &instance->game.option[PP2_OPTION_ENABLE_POWERUP_TURBO]);
	joynet_add_game_option(instance->game.client_game, &instance->game.option[PP2_OPTION_START_AMMO_NORMAL]);
	joynet_add_game_option(instance->game.client_game, &instance->game.option[PP2_OPTION_START_AMMO_X]);
	joynet_add_game_option(instance->game.client_game, &instance->game.option[PP2_OPTION_START_AMMO_MINE]);
	joynet_add_game_option(instance->game.client_game, &instance->game.option[PP2_OPTION_START_AMMO_BOUNCE]);
	joynet_add_game_option(instance->game.client_game, &instance->game.option[PP2_OPTION_START_AMMO_SEEK]);
	joynet_add_game_option(instance->game.client_game, &instance->game.option[PP2_OPTION_START_AMMO_REFLECTOR]);
	joynet_add_game_option(instance->game.client_game, &instance->game.option[PP2_OPTION_START_AMMO_PMINE]);
	joynet_add_game_option(instance->game.client_game, &instance->game.option[PP2_OPTION_START_AMMO_GHOST]);
	joynet_add_game_option(instance->game.client_game, &instance->game.option[PP2_OPTION_DEATH_MATCH_FRAGS]);
	joynet_add_game_option(instance->game.client_game, &instance->game.option[PP2_OPTION_LIFE]);
	joynet_add_game_option(instance->game.client_game, &instance->game.option[PP2_OPTION_RANDOMIZE_ITEMS]);
	joynet_add_game_option(instance->game.client_game, &instance->game.option[PP2_OPTION_STOMP_HITS]);
	joynet_add_game_option(instance->game.client_game, &instance->game.option[PP2_OPTION_AMMO_WORTH]);
	for(i = 0; i < instance->resources.level_database->entries; i++)
	{
		if(instance->resources.level_database->entry[i]->extra)
		{
			joynet_add_game_content(instance->game.client_game, PP2_CONTENT_LEVELS, instance->resources.level_database->entry[i]->checksum);
		}
	}
	for(i = 0; i < instance->resources.character_database->entries; i++)
	{
		if(instance->resources.character_database->entry[i]->extra)
		{
			joynet_add_game_content(instance->game.client_game, PP2_CONTENT_CHARACTERS, instance->resources.character_database->entry[i]->checksum);
		}
	}
	for(i = 0; i < PP2_MAX_PLAYERS; i++)
	{
		joynet_add_player_option(instance->game.client_game, i, &instance->game.player[i].step);
	}
	return true;
}

bool pp2_build_character_database(PP2_INTERFACE * ip, PP2_RESOURCES * resources)
{
	char buf[1024];
	ALLEGRO_PATH * temp_path = NULL;
	unsigned long count = 0;
	char * search_path[64] = {NULL};
	const char * path = NULL;
	int search_paths = 0;
	const char * search_type[16] = {".p2c", ".ppc"};
	int search_types = 2;
	int i, j;

	search_path[0] = "data/characters";
	path = t3f_get_filename(t3f_data_path, "characters", buf, 1024);
	search_path[1] = malloc(strlen(path) + 1);
	strcpy((char *)search_path[1], path);
	search_paths = 2;

	for(i = 0; i < search_paths; i++)
	{
		for(j = 0; j < search_types; j++)
		{
			count += pp2_database_count_files(search_path[i], search_type[j], 0);
		}
	}

	resources->character_database = pp2_create_database(t3f_get_filename(t3f_data_path, "characters.ini", buf, 1024), count, ip->regenerate_cache ? PP2_DATABASE_FLAG_REGENERATE : 0);
	if(!resources->character_database)
	{
		return false;
	}
	for(i = 0; i < search_paths; i++)
	{
		for(j = 0; j < search_types; j++)
		{
			temp_path = al_create_path_for_directory(search_path[i]);
			if(temp_path)
			{
				pp2_database_add_files(resources->character_database, temp_path, search_type[j], 0);
				al_destroy_path(temp_path);
			}
		}
	}
	for(i = 0; i < resources->character_database->entries; i++)
	{
		resources->character_database->entry[i]->extra = pp2_character_database_create(resources->character_database, i, ip->regenerate_cache ? PP2_DATABASE_FLAG_REGENERATE : 0, resources);
		if(!resources->character_database->entry[i]->extra)
		{
			return false;
		}
	}
	free(search_path[1]);
	return true;
}

bool pp2_build_level_database(PP2_INTERFACE * ip, PP2_RESOURCES * resources)
{
	char buf[1024];
	ALLEGRO_PATH * temp_path = NULL;
	unsigned long count = 0;
	char * search_path[64] = {NULL};
	const char * path = NULL;
	int search_paths = 0;
	const char * search_type[16] = {".p2l", ".ppl"};
	int search_types = 2;
	int i, j;

	search_path[0] = "data/levels";
	path = t3f_get_filename(t3f_data_path, "levels", buf, 1024);
	search_path[1] = malloc(strlen(path) + 1);
	strcpy((char *)search_path[1], path);
	search_paths = 2;

	for(i = 0; i < search_paths; i++)
	{
		for(j = 0; j < search_types; j++)
		{
			count += pp2_database_count_files(search_path[i], search_type[j], 0);
		}
	}

	resources->level_database = pp2_create_database(t3f_get_filename(t3f_data_path, "levels.ini", buf, 1024), count, ip->regenerate_cache ? PP2_DATABASE_FLAG_REGENERATE : 0);
	if(!resources->level_database)
	{
		return false;
	}
	for(i = 0; i < search_paths; i++)
	{
		for(j = 0; j < search_types; j++)
		{
			temp_path = al_create_path_for_directory(search_path[i]);
			if(temp_path)
			{
				pp2_database_add_files(resources->level_database, temp_path, search_type[j], 0);
				al_destroy_path(temp_path);
			}
		}
	}
	for(i = 0; i < resources->level_database->entries; i++)
	{
		resources->level_database->entry[i]->extra = pp2_level_database_create(resources->level_database, i, ip->regenerate_cache ? PP2_DATABASE_FLAG_REGENERATE : 0, resources);
		if(!resources->level_database->entry[i]->extra)
		{
			return false;
		}
	}
	free(search_path[1]);
	return true;
}

bool pp2_build_music_database(PP2_INTERFACE * ip, PP2_RESOURCES * resources)
{
	char buf[1024];
	ALLEGRO_PATH * temp_path = NULL;
	unsigned long count = 0;
	char * search_path[64] = {NULL};
	const char * path = NULL;
	const char * val = NULL;
	int search_paths = 0;
	const char * search_type[16] = {".ogg", ".flac", ".it", ".s3m", ".mod"};
	int search_types = 5;
	int locations = 0;
	char location_text[64] = {0};
	int i, j;

	search_path[0] = "data/music/bgm"; // search music that comes in the game's package
	search_path[1] = "data/levels"; // search music that comes with the game's levels
	path = t3f_get_filename(t3f_data_path, "music", buf, 1024);
	search_path[2] = malloc(strlen(path) + 1);
	strcpy((char *)search_path[2], path);
	path = t3f_get_filename(t3f_data_path, "levels", buf, 1024);
	search_path[3] = malloc(strlen(path) + 1);
	strcpy((char *)search_path[3], path);
	search_paths = 4;

	/* read custom locations from config file */
	val = al_get_config_value(ip->config, "Music", "Locations");
	if(val)
	{
		locations = atoi(val);
		for(i = 0; i < locations; i++)
		{
			sprintf(location_text, "Location %d", i + 1);
			search_path[search_paths] = (char *)al_get_config_value(ip->config, "Music", location_text);
			if(search_path[search_paths])
			{
				search_paths++;
			}
		}
	}

	for(i = 0; i < search_paths; i++)
	{
		for(j = 0; j < search_types; j++)
		{
			count += pp2_database_count_files(search_path[i], search_type[j], 0);
		}
	}
	resources->music_database = pp2_create_database(NULL, count, 0);
	if(!resources->music_database)
	{
		return false;
	}
	for(i = 0; i < search_paths; i++)
	{
		for(j = 0; j < search_types; j++)
		{
			temp_path = al_create_path_for_directory(search_path[i]);
			if(temp_path)
			{
				pp2_database_add_files(resources->music_database, temp_path, search_type[j], PP2_DATABASE_FLAG_NO_CHECKSUM);
				al_destroy_path(temp_path);
			}
		}
	}
	free(search_path[2]);
	free(search_path[3]);
	return true;
}

bool pp2_build_demo_database(PP2_RESOURCES * resources)
{
	char buf[1024];
	ALLEGRO_PATH * temp_path = NULL;
	unsigned long count = 0;

	count = pp2_database_count_files("data/demos", ".p2r", 0);
	count += pp2_database_count_files(t3f_get_filename(t3f_data_path, "replays", buf, 1024), ".p2r", 0);
	resources->demo_database = pp2_create_database(NULL, count, 0);
	if(!resources->demo_database)
	{
		return false;
	}
	temp_path = al_create_path_for_directory("data/demos");
	if(!temp_path)
	{
		return false;
	}
	pp2_database_add_files(resources->demo_database, temp_path, ".p2r", PP2_DATABASE_FLAG_NO_CHECKSUM);
	al_destroy_path(temp_path);
	temp_path = al_clone_path(t3f_data_path);
	if(!temp_path)
	{
		return false;
	}
	al_append_path_component(temp_path, "replays");
	pp2_database_add_files(resources->demo_database, temp_path, ".p2r", PP2_DATABASE_FLAG_NO_CHECKSUM);
	return true;
}
