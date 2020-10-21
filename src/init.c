#include "t3f/resource.h"
#include "pp2.h"
#include "tables.h"
#include "data.h"
#include "version.h"
#include "legacy/animation.h"
#include "gameplay/network.h"
#include "file/database/database.h"
#include "file/database/character.h"
#include "file/database/level.h"
#include "resource.h"
#include "gameplay/sprites/object_defines.h"

char pp2_load_text[1024] = {0};

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
		al_draw_text(resources->font[PP2_FONT_SMALL], al_map_rgba(255, 255, 255, 255), al_get_display_width(t3f_display) / 2, al_get_display_height(t3f_display) / 2 + al_get_bitmap_height(resources->bitmap[PP2_BITMAP_LOADING]) / 2, ALLEGRO_ALIGN_CENTRE, pp2_load_text);
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

void pp2_set_controller_config(int controller, int binding)
{
	char temp_string[256];
	char temp_string2[256];

	sprintf(temp_string, "Controller %d", controller);
	sprintf(temp_string2, "%s Type", pp2_button_name[binding]);
	al_set_config_value(pp2_config, temp_string, temp_string2, pp2_itoa(pp2_controller[controller]->binding[binding].type));
	sprintf(temp_string2, "%s SubType", pp2_button_name[binding]);
	al_set_config_value(pp2_config, temp_string, temp_string2, pp2_itoa(pp2_controller[controller]->binding[binding].sub_type));
	sprintf(temp_string2, "%s Joystick", pp2_button_name[binding]);
	al_set_config_value(pp2_config, temp_string, temp_string2, pp2_itoa(pp2_controller[controller]->binding[binding].joystick));
	sprintf(temp_string2, "%s Button", pp2_button_name[binding]);
	al_set_config_value(pp2_config, temp_string, temp_string2, pp2_itoa(pp2_controller[controller]->binding[binding].button));
	sprintf(temp_string2, "%s Stick", pp2_button_name[binding]);
	al_set_config_value(pp2_config, temp_string, temp_string2, pp2_itoa(pp2_controller[controller]->binding[binding].stick));
	sprintf(temp_string2, "%s Axis", pp2_button_name[binding]);
	al_set_config_value(pp2_config, temp_string, temp_string2, pp2_itoa(pp2_controller[controller]->binding[binding].axis));
	sprintf(temp_string2, "%s Flags", pp2_button_name[binding]);
	al_set_config_value(pp2_config, temp_string, temp_string2, pp2_itoa(pp2_controller[controller]->binding[binding].flags));
}

void pp2_autodetect_controllers(void)
{
	int i, c;

	/* use all joysticks */
	c = al_get_num_joysticks();
	for(i = 0; i < c; i++)
	{
		pp2_controller[i]->binding[0].type = T3F_CONTROLLER_BINDING_JOYSTICK_AXIS;
		pp2_controller[i]->binding[0].joystick = i;
		pp2_controller[i]->binding[0].stick = 0;
		pp2_controller[i]->binding[0].axis = 1;
		pp2_controller[i]->binding[0].flags = T3F_CONTROLLER_FLAG_AXIS_NEGATIVE;
		pp2_set_controller_config(i, 0);
		pp2_controller[i]->binding[1].type = T3F_CONTROLLER_BINDING_JOYSTICK_AXIS;
		pp2_controller[i]->binding[1].joystick = i;
		pp2_controller[i]->binding[1].stick = 0;
		pp2_controller[i]->binding[1].axis = 1;
		pp2_controller[i]->binding[1].flags = T3F_CONTROLLER_FLAG_AXIS_POSITIVE;
		pp2_set_controller_config(i, 1);
		pp2_controller[i]->binding[2].type = T3F_CONTROLLER_BINDING_JOYSTICK_AXIS;
		pp2_controller[i]->binding[2].joystick = i;
		pp2_controller[i]->binding[2].stick = 0;
		pp2_controller[i]->binding[2].axis = 0;
		pp2_controller[i]->binding[2].flags = T3F_CONTROLLER_FLAG_AXIS_NEGATIVE;
		pp2_set_controller_config(i, 2);
		pp2_controller[i]->binding[3].type = T3F_CONTROLLER_BINDING_JOYSTICK_AXIS;
		pp2_controller[i]->binding[3].joystick = i;
		pp2_controller[i]->binding[3].stick = 0;
		pp2_controller[i]->binding[3].axis = 0;
		pp2_controller[i]->binding[3].flags = T3F_CONTROLLER_FLAG_AXIS_POSITIVE;
		pp2_set_controller_config(i, 3);
		pp2_controller[i]->binding[4].type = T3F_CONTROLLER_BINDING_JOYSTICK_BUTTON;
		pp2_controller[i]->binding[4].joystick = i;
		pp2_controller[i]->binding[4].stick = 0;
		pp2_controller[i]->binding[4].button = 0;
		pp2_controller[i]->binding[4].flags = 0;
		pp2_set_controller_config(i, 4);
		pp2_controller[i]->binding[5].type = T3F_CONTROLLER_BINDING_JOYSTICK_BUTTON;
		pp2_controller[i]->binding[5].joystick = i;
		pp2_controller[i]->binding[5].stick = 0;
		pp2_controller[i]->binding[5].button = 1;
		pp2_controller[i]->binding[5].flags = 0;
		pp2_set_controller_config(i, 5);
		pp2_controller[i]->binding[6].type = T3F_CONTROLLER_BINDING_JOYSTICK_BUTTON;
		pp2_controller[i]->binding[6].joystick = i;
		pp2_controller[i]->binding[6].stick = 0;
		pp2_controller[i]->binding[6].button = 2;
		pp2_controller[i]->binding[6].flags = 0;
		pp2_set_controller_config(i, 6);
		pp2_controller[i]->binding[7].type = T3F_CONTROLLER_BINDING_JOYSTICK_BUTTON;
		pp2_controller[i]->binding[7].joystick = i;
		pp2_controller[i]->binding[7].stick = 0;
		pp2_controller[i]->binding[7].button = 3;
		pp2_controller[i]->binding[7].flags = 0;
		pp2_set_controller_config(i, 7);
		pp2_controller[i]->binding[8].type = T3F_CONTROLLER_BINDING_JOYSTICK_BUTTON;
		pp2_controller[i]->binding[8].joystick = i;
		pp2_controller[i]->binding[8].stick = 0;
		pp2_controller[i]->binding[8].button = 4;
		pp2_controller[i]->binding[8].flags = 0;
		pp2_set_controller_config(i, 8);
	}

	/* fill remaining controllers with default keys */
	for(i = c; i < 4; i++)
	{
		pp2_controller[i]->binding[0].type = T3F_CONTROLLER_BINDING_KEY;
		pp2_controller[i]->binding[0].button = pp2_default_keys[i - c][0];
		pp2_set_controller_config(i, 0);
		pp2_controller[i]->binding[1].type = T3F_CONTROLLER_BINDING_KEY;
		pp2_controller[i]->binding[1].button = pp2_default_keys[i - c][1];
		pp2_set_controller_config(i, 1);
		pp2_controller[i]->binding[2].type = T3F_CONTROLLER_BINDING_KEY;
		pp2_controller[i]->binding[2].button = pp2_default_keys[i - c][2];
		pp2_set_controller_config(i, 2);
		pp2_controller[i]->binding[3].type = T3F_CONTROLLER_BINDING_KEY;
		pp2_controller[i]->binding[3].button = pp2_default_keys[i - c][3];
		pp2_set_controller_config(i, 3);
		pp2_controller[i]->binding[4].type = T3F_CONTROLLER_BINDING_KEY;
		pp2_controller[i]->binding[4].button = pp2_default_keys[i - c][4];
		pp2_set_controller_config(i, 4);
		pp2_controller[i]->binding[5].type = T3F_CONTROLLER_BINDING_KEY;
		pp2_controller[i]->binding[5].button = pp2_default_keys[i - c][5];
		pp2_set_controller_config(i, 5);
		pp2_controller[i]->binding[6].type = T3F_CONTROLLER_BINDING_KEY;
		pp2_controller[i]->binding[6].button = pp2_default_keys[i - c][6];
		pp2_set_controller_config(i, 6);
		pp2_controller[i]->binding[7].type = T3F_CONTROLLER_BINDING_KEY;
		pp2_controller[i]->binding[7].button = pp2_default_keys[i - c][7];
		pp2_set_controller_config(i, 7);
		pp2_controller[i]->binding[8].type = T3F_CONTROLLER_BINDING_KEY;
		pp2_controller[i]->binding[8].button = pp2_default_keys[i - c][8];
		pp2_set_controller_config(i, 8);
	}
}

bool pp2_load_images(PP2_RESOURCES * resources)
{
	ALLEGRO_STATE old_state;

	t3f_load_resource((void **)&resources->bitmap[PP2_BITMAP_T3_LOGO], T3F_RESOURCE_TYPE_BITMAP, "data/graphics/t3_logo.png", 0, 0, 0);
	if(!resources->bitmap[PP2_BITMAP_T3_LOGO])
	{
		printf("Error loading image %d.\n", PP2_BITMAP_T3_LOGO);
		return false;
	}
	t3f_load_resource((void **)&resources->bitmap[PP2_BITMAP_TITLE_SPLAT], T3F_RESOURCE_TYPE_BITMAP, "data/graphics/title_splat.png", 0, 0, 0);
	if(!resources->bitmap[PP2_BITMAP_TITLE_SPLAT])
	{
		printf("Error loading image %d.\n", PP2_BITMAP_TITLE_SPLAT);
		return false;
	}
	t3f_load_resource((void **)&resources->bitmap[PP2_BITMAP_TITLE_LOGO], T3F_RESOURCE_TYPE_BITMAP, "data/graphics/title_logo.png", 0, 0, 0);
	if(!resources->bitmap[PP2_BITMAP_TITLE_LOGO])
	{
		printf("Error loading image %d.\n", PP2_BITMAP_TITLE_LOGO);
		return false;
	}
	t3f_load_resource((void **)&resources->bitmap[PP2_BITMAP_EMPTY_PLAYER], T3F_RESOURCE_TYPE_BITMAP, "data/graphics/empty_player.png", 0, 0, 0);
	if(!resources->bitmap[PP2_BITMAP_EMPTY_PLAYER])
	{
		printf("Error loading image %d.\n", PP2_BITMAP_EMPTY_PLAYER);
		return false;
	}
	t3f_load_resource((void **)&resources->bitmap[PP2_BITMAP_MENU_BG], T3F_RESOURCE_TYPE_BITMAP, "data/graphics/menubg.png", 0, 0, 0);
	if(!resources->bitmap[PP2_BITMAP_MENU_BG])
	{
		printf("Error loading image %d.\n", PP2_BITMAP_MENU_BG);
		return false;
	}
	t3f_load_resource((void **)&resources->bitmap[PP2_BITMAP_MENU_LOGO], T3F_RESOURCE_TYPE_BITMAP, "data/graphics/menu_logo.png", 0, 0, 0);
	if(!resources->bitmap[PP2_BITMAP_MENU_LOGO])
	{
		printf("Error loading image %d.\n", PP2_BITMAP_MENU_LOGO);
		return false;
	}
	t3f_load_resource((void **)&resources->bitmap[PP2_BITMAP_TARGET], T3F_RESOURCE_TYPE_BITMAP, "data/graphics/target.png", 0, 0, 0);
	if(!resources->bitmap[PP2_BITMAP_TARGET])
	{
		printf("Error loading image %d.\n", PP2_BITMAP_TARGET);
		return false;
	}
	t3f_load_resource((void **)&resources->bitmap[PP2_BITMAP_RADAR_BLIP], T3F_RESOURCE_TYPE_BITMAP, "data/graphics/radar_blip.png", 0, 0, 0);
	if(!resources->bitmap[PP2_BITMAP_RADAR_BLIP])
	{
		printf("Error loading image %d.\n", PP2_BITMAP_RADAR_BLIP);
		return false;
	}
	t3f_load_resource((void **)&resources->bitmap[PP2_BITMAP_TYPING], T3F_RESOURCE_TYPE_BITMAP, "data/graphics/typing.png", 0, 0, 0);
	if(!resources->bitmap[PP2_BITMAP_TYPING])
	{
		printf("Error loading image %d.\n", PP2_BITMAP_TYPING);
		return false;
	}
	t3f_load_resource((void **)&resources->bitmap[PP2_BITMAP_HIGHLIGHT], T3F_RESOURCE_TYPE_BITMAP, "data/graphics/paintball_highlight.png", 0, 0, 0);
	if(!resources->bitmap[PP2_BITMAP_HIGHLIGHT])
	{
		printf("Error loading image %d.\n", PP2_BITMAP_HIGHLIGHT);
		return false;
	}
	al_store_state(&old_state, ALLEGRO_STATE_NEW_BITMAP_PARAMETERS);
	al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
	resources->bitmap[PP2_BITMAP_T3_LOGO_MEMORY] = al_load_bitmap("data/graphics/t3_logo.png");
	if(!resources->bitmap[PP2_BITMAP_T3_LOGO_MEMORY])
	{
		printf("Error loading image %d.\n", PP2_BITMAP_T3_LOGO_MEMORY);
		return false;
	}
	al_restore_state(&old_state);
	return true;
}

bool pp2_load_sounds(void)
{
	pp2_sample[PP2_SAMPLE_MENU_PICK] = al_load_sample("data/sounds/menu_enter.wav");
	if(!pp2_sample[PP2_SAMPLE_MENU_PICK])
	{
		printf("Error loading sample %d!\n", PP2_SAMPLE_MENU_PICK);
		return false;
	}
	pp2_sample[PP2_SAMPLE_MENU_NEXT] = al_load_sample("data/sounds/menu_right.wav");
	if(!pp2_sample[PP2_SAMPLE_MENU_NEXT])
	{
		printf("Error loading sample %d!\n", PP2_SAMPLE_MENU_NEXT);
		return false;
	}
	pp2_sample[PP2_SAMPLE_LOGO] = al_load_sample("data/sounds/logo.ogg");
	if(!pp2_sample[PP2_SAMPLE_LOGO])
	{
		printf("Error loading sample %d!\n", PP2_SAMPLE_LOGO);
		return false;
	}
	pp2_sample[PP2_SAMPLE_LOGO_TICK] = al_load_sample("data/sounds/logotick.wav");
	if(!pp2_sample[PP2_SAMPLE_LOGO_TICK])
	{
		printf("Error loading sample %d!\n", PP2_SAMPLE_LOGO_TICK);
		return false;
	}

	pp2_sample[PP2_SAMPLE_FIRE] = al_load_sample("data/sounds/shoot.wav");
	if(!pp2_sample[PP2_SAMPLE_FIRE])
	{
		printf("Error loading sample %d!\n", PP2_SAMPLE_FIRE);
		return false;
	}
	pp2_sample[PP2_SAMPLE_SPLAT] = al_load_sample("data/sounds/splat.wav");
	if(!pp2_sample[PP2_SAMPLE_SPLAT])
	{
		printf("Error loading sample %d!\n", PP2_SAMPLE_SPLAT);
		return false;
	}
	pp2_sample[PP2_SAMPLE_RELOAD_A] = al_load_sample("data/sounds/reload1.wav");
	if(!pp2_sample[PP2_SAMPLE_RELOAD_A])
	{
		printf("Error loading sample %d!\n", PP2_SAMPLE_RELOAD_A);
		return false;
	}
	pp2_sample[PP2_SAMPLE_RELOAD_B] = al_load_sample("data/sounds/reload2.wav");
	if(!pp2_sample[PP2_SAMPLE_RELOAD_B])
	{
		printf("Error loading sample %d!\n", PP2_SAMPLE_RELOAD_B);
		return false;
	}
	pp2_sample[PP2_SAMPLE_AMMO] = al_load_sample("data/sounds/ammo.wav");
	if(!pp2_sample[PP2_SAMPLE_AMMO])
	{
		printf("Error loading sample %d!\n", PP2_SAMPLE_AMMO);
		return false;
	}
	pp2_sample[PP2_SAMPLE_CLOAK] = al_load_sample("data/sounds/cloak.wav");
	if(!pp2_sample[PP2_SAMPLE_CLOAK])
	{
		printf("Error loading sample %d!\n", PP2_SAMPLE_CLOAK);
		return false;
	}
	pp2_sample[PP2_SAMPLE_GEM] = al_load_sample("data/sounds/gem.wav");
	if(!pp2_sample[PP2_SAMPLE_GEM])
	{
		printf("Error loading sample %d!\n", PP2_SAMPLE_GEM);
		return false;
	}
	pp2_sample[PP2_SAMPLE_JUMP] = al_load_sample("data/sounds/jump.wav");
	if(!pp2_sample[PP2_SAMPLE_JUMP])
	{
		printf("Error loading sample %d!\n", PP2_SAMPLE_JUMP);
		return false;
	}
	pp2_sample[PP2_SAMPLE_LAND] = al_load_sample("data/sounds/land.wav");
	if(!pp2_sample[PP2_SAMPLE_LAND])
	{
		printf("Error loading sample %d!\n", PP2_SAMPLE_LAND);
		return false;
	}
	pp2_sample[PP2_SAMPLE_HIT] = al_load_sample("data/sounds/hit.wav");
	if(!pp2_sample[PP2_SAMPLE_HIT])
	{
		printf("Error loading sample %d!\n", PP2_SAMPLE_HIT);
		return false;
	}
	pp2_sample[PP2_SAMPLE_BUMP] = al_load_sample("data/sounds/bump.wav");
	if(!pp2_sample[PP2_SAMPLE_BUMP])
	{
		printf("Error loading sample %d!\n", PP2_SAMPLE_BUMP);
		return false;
	}
	pp2_sample[PP2_SAMPLE_TIME] = al_load_sample("data/sounds/time.wav");
	if(!pp2_sample[PP2_SAMPLE_TIME])
	{
		printf("Error loading sample %d!\n", PP2_SAMPLE_TIME);
		return false;
	}
	pp2_sample[PP2_SAMPLE_TELE_IN] = al_load_sample("data/sounds/telein.wav");
	if(!pp2_sample[PP2_SAMPLE_TELE_IN])
	{
		printf("Error loading sample %d!\n", PP2_SAMPLE_TELE_IN);
		return false;
	}
	pp2_sample[PP2_SAMPLE_TELE_OUT] = al_load_sample("data/sounds/telein.wav");
	if(!pp2_sample[PP2_SAMPLE_TELE_OUT])
	{
		printf("Error loading sample %d!\n", PP2_SAMPLE_TELE_OUT);
		return false;
	}
	pp2_sample[PP2_SAMPLE_RUN] = al_load_sample("data/sounds/run.wav");
	if(!pp2_sample[PP2_SAMPLE_RUN])
	{
		printf("Error loading sample %d!\n", PP2_SAMPLE_RUN);
		return false;
	}
	pp2_sample[PP2_SAMPLE_DEFLECT] = al_load_sample("data/sounds/deflect.wav");
	if(!pp2_sample[PP2_SAMPLE_DEFLECT])
	{
		printf("Error loading sample %d!\n", PP2_SAMPLE_DEFLECT);
		return false;
	}
	pp2_sample[PP2_SAMPLE_FLAG] = al_load_sample("data/sounds/flag.wav");
	if(!pp2_sample[PP2_SAMPLE_FLAG])
	{
		printf("Error loading sample %d!\n", PP2_SAMPLE_FLAG);
		return false;
	}
	pp2_sample[PP2_SAMPLE_BANK] = al_load_sample("data/sounds/bank.wav");
	if(!pp2_sample[PP2_SAMPLE_BANK])
	{
		printf("Error loading sample %d!\n", PP2_SAMPLE_BANK);
		return false;
	}
	pp2_sample[PP2_SAMPLE_BASE] = al_load_sample("data/sounds/base.wav");
	if(!pp2_sample[PP2_SAMPLE_BASE])
	{
		printf("Error loading sample %d!\n", PP2_SAMPLE_BASE);
		return false;
	}
	pp2_sample[PP2_SAMPLE_RESPAWN] = al_load_sample("data/sounds/respawn.wav");
	if(!pp2_sample[PP2_SAMPLE_RESPAWN])
	{
		printf("Error loading sample %d!\n", PP2_SAMPLE_RESPAWN);
		return false;
	}
	pp2_sample[PP2_SAMPLE_PJUMP] = al_load_sample("data/sounds/pjump.wav");
	if(!pp2_sample[PP2_SAMPLE_PJUMP])
	{
		printf("Error loading sample %d!\n", PP2_SAMPLE_PJUMP);
		return false;
	}
	pp2_sample[PP2_SAMPLE_FLY] = al_load_sample("data/sounds/fly.wav");
	if(!pp2_sample[PP2_SAMPLE_FLY])
	{
		printf("Error loading sample %d!\n", PP2_SAMPLE_FLY);
		return false;
	}
	pp2_sample[PP2_SAMPLE_PFLY] = al_load_sample("data/sounds/pfly.wav");
	if(!pp2_sample[PP2_SAMPLE_PFLY])
	{
		printf("Error loading sample %d!\n", PP2_SAMPLE_PFLY);
		return false;
	}
	pp2_sample[PP2_SAMPLE_PTURBO] = al_load_sample("data/sounds/pturbo.wav");
	if(!pp2_sample[PP2_SAMPLE_PTURBO])
	{
		printf("Error loading sample %d!\n", PP2_SAMPLE_PTURBO);
		return false;
	}
	pp2_sample[PP2_SAMPLE_CRUSH] = al_load_sample("data/sounds/crush.wav");
	if(!pp2_sample[PP2_SAMPLE_CRUSH])
	{
		printf("Error loading sample %d!\n", PP2_SAMPLE_CRUSH);
		return false;
	}
	pp2_sample[PP2_SAMPLE_TARGET] = al_load_sample("data/sounds/target.wav");
	if(!pp2_sample[PP2_SAMPLE_TARGET])
	{
		printf("Error loading sample %d!\n", PP2_SAMPLE_TARGET);
		return false;
	}
	pp2_sample[PP2_SAMPLE_BOUNCE] = al_load_sample("data/sounds/bounce.wav");
	if(!pp2_sample[PP2_SAMPLE_BOUNCE])
	{
		printf("Error loading sample %d!\n", PP2_SAMPLE_BOUNCE);
		return false;
	}
	pp2_sample[PP2_SAMPLE_RICOCHET] = al_load_sample("data/sounds/deflects.wav");
	if(!pp2_sample[PP2_SAMPLE_RICOCHET])
	{
		printf("Error loading sample %d!\n", PP2_SAMPLE_RICOCHET);
		return false;
	}
	pp2_sample[PP2_SAMPLE_SPRING] = al_load_sample("data/sounds/spring.wav");
	if(!pp2_sample[PP2_SAMPLE_SPRING])
	{
		printf("Error loading sample %d!\n", PP2_SAMPLE_SPRING);
		return false;
	}
	pp2_sample[PP2_SAMPLE_COIN_LAND] = al_load_sample("data/sounds/coin_land.wav");
	if(!pp2_sample[PP2_SAMPLE_COIN_LAND])
	{
		printf("Error loading sample %d!\n", PP2_SAMPLE_COIN_LAND);
		return false;
	}
	pp2_sample[PP2_SAMPLE_COIN_PICKUP] = al_load_sample("data/sounds/coin_pickup.wav");
	if(!pp2_sample[PP2_SAMPLE_COIN_PICKUP])
	{
		printf("Error loading sample %d!\n", PP2_SAMPLE_COIN_PICKUP);
		return false;
	}
	return true;
}

bool pp2_load_animations(PP2_RESOURCES * resources)
{
	int i;

	if(!pp2_legacy_load_palette("data/graphics/legacy_palette.png"))
	{
		return false;
	}
	resources->object_animation[PP2_OBJECT_PORTAL] = pp2_legacy_load_animation("data/graphics/portal.ani");
	if(!resources->object_animation[PP2_OBJECT_PORTAL])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_PORTAL);
		return false;
	}
	resources->object_animation[PP2_OBJECT_AMMO_NORMAL] = t3f_load_animation("data/graphics/ammo_normal.t3a");
	if(!resources->object_animation[PP2_OBJECT_AMMO_NORMAL])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_AMMO_NORMAL);
		return false;
	}
	resources->object_animation[PP2_OBJECT_AMMO_X] = t3f_load_animation("data/graphics/ammo_splitter.t3a");
	if(!resources->object_animation[PP2_OBJECT_AMMO_X])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_AMMO_X);
		return false;
	}
	resources->object_animation[PP2_OBJECT_AMMO_MINE] = t3f_load_animation("data/graphics/ammo_mine.t3a");
	if(!resources->object_animation[PP2_OBJECT_AMMO_MINE])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_AMMO_MINE);
		return false;
	}
	resources->object_animation[PP2_OBJECT_AMMO_BOUNCE] = t3f_load_animation("data/graphics/ammo_bouncer.t3a");
	if(!resources->object_animation[PP2_OBJECT_AMMO_BOUNCE])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_AMMO_BOUNCE);
		return false;
	}
	resources->object_animation[PP2_OBJECT_AMMO_SEEK] = t3f_load_animation("data/graphics/ammo_seeker.t3a");
	if(!resources->object_animation[PP2_OBJECT_AMMO_SEEK])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_AMMO_SEEK);
		return false;
	}
	resources->object_animation[PP2_OBJECT_AMMO_REFLECTOR] = t3f_load_animation("data/graphics/ammo_reflector.t3a");
	if(!resources->object_animation[PP2_OBJECT_AMMO_REFLECTOR])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_AMMO_REFLECTOR);
		return false;
	}
	resources->object_animation[PP2_OBJECT_AMMO_PMINE] = t3f_load_animation("data/graphics/ammo_pmine.t3a");
	if(!resources->object_animation[PP2_OBJECT_AMMO_PMINE])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_AMMO_PMINE);
		return false;
	}
	resources->object_animation[PP2_OBJECT_AMMO_GHOST] = t3f_load_animation("data/graphics/ammo_ghost.t3a");
	if(!resources->object_animation[PP2_OBJECT_AMMO_GHOST])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_AMMO_GHOST);
		return false;
	}
	resources->object_animation[PP2_OBJECT_POWER_CLOAK] = t3f_load_animation("data/graphics/powerup_cloak.t3a");
	if(!resources->object_animation[PP2_OBJECT_POWER_CLOAK])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_POWER_CLOAK);
		return false;
	}
	resources->object_animation[PP2_OBJECT_POWER_JUMP] = t3f_load_animation("data/graphics/powerup_jump.t3a");
	if(!resources->object_animation[PP2_OBJECT_POWER_JUMP])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_POWER_JUMP);
		return false;
	}
	resources->object_animation[PP2_OBJECT_POWER_RUN] = t3f_load_animation("data/graphics/powerup_run.t3a");
	if(!resources->object_animation[PP2_OBJECT_POWER_RUN])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_POWER_RUN);
		return false;
	}
	resources->object_animation[PP2_OBJECT_POWER_DEFLECT] = t3f_load_animation("data/graphics/powerup_deflect.t3a");
	if(!resources->object_animation[PP2_OBJECT_POWER_DEFLECT])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_POWER_DEFLECT);
		return false;
	}
	resources->object_animation[PP2_OBJECT_POWER_FLY] = t3f_load_animation("data/graphics/powerup_fly.t3a");
	if(!resources->object_animation[PP2_OBJECT_POWER_FLY])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_POWER_FLY);
		return false;
	}
	resources->object_animation[PP2_OBJECT_POWER_TURBO] = t3f_load_animation("data/graphics/powerup_turbo.t3a");
	if(!resources->object_animation[PP2_OBJECT_POWER_TURBO])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_POWER_TURBO);
		return false;
	}
	resources->object_animation[PP2_OBJECT_JET] = pp2_legacy_load_animation("data/graphics/jet.ani");
	if(!resources->object_animation[PP2_OBJECT_JET])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_JET);
		return false;
	}
	resources->object_animation[PP2_OBJECT_SPRING_UP] = pp2_legacy_load_animation("data/graphics/spring_up.ani");
	if(!resources->object_animation[PP2_OBJECT_SPRING_UP])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_SPRING_UP);
		return false;
	}
	resources->object_animation[PP2_OBJECT_SPRING_DOWN] = pp2_legacy_load_animation("data/graphics/spring_down.ani");
	if(!resources->object_animation[PP2_OBJECT_SPRING_DOWN])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_SPRING_DOWN);
		return false;
	}
	resources->object_animation[PP2_OBJECT_SPRING_LEFT] = pp2_legacy_load_animation("data/graphics/spring_left.ani");
	if(!resources->object_animation[PP2_OBJECT_SPRING_LEFT])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_SPRING_LEFT);
		return false;
	}
	resources->object_animation[PP2_OBJECT_SPRING_RIGHT] = pp2_legacy_load_animation("data/graphics/spring_right.ani");
	if(!resources->object_animation[PP2_OBJECT_SPRING_RIGHT])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_SPRING_RIGHT);
		return false;
	}
	resources->object_animation[PP2_OBJECT_SPRING_BAR_V] = pp2_legacy_load_animation("data/graphics/spring_bar_v.ani");
	if(!resources->object_animation[PP2_OBJECT_SPRING_BAR_V])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_SPRING_BAR_V);
		return false;
	}
	resources->object_animation[PP2_OBJECT_SPRING_BAR_H] = pp2_legacy_load_animation("data/graphics/spring_bar_h.ani");
	if(!resources->object_animation[PP2_OBJECT_SPRING_BAR_H])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_SPRING_BAR_H);
		return false;
	}
	resources->object_animation[PP2_OBJECT_COIN] = t3f_load_animation("data/graphics/coin.t3a");
	if(!resources->object_animation[PP2_OBJECT_COIN])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_COIN);
		return false;
	}
	resources->animation[PP2_ANIMATION_HUD_AMMO_NORMAL] = t3f_load_animation("data/graphics/hud_normal.t3a");
	if(!resources->animation[PP2_ANIMATION_HUD_AMMO_NORMAL])
	{
		printf("Error loading HUD animation %d!\n", PP2_ANIMATION_HUD_AMMO_NORMAL);
		return false;
	}
	resources->animation[PP2_ANIMATION_HUD_AMMO_SPLITTER] = t3f_load_animation("data/graphics/hud_splitter.t3a");
	if(!resources->animation[PP2_ANIMATION_HUD_AMMO_SPLITTER])
	{
		printf("Error loading HUD animation %d!\n", PP2_ANIMATION_HUD_AMMO_SPLITTER);
		return false;
	}
	resources->animation[PP2_ANIMATION_HUD_AMMO_MINE] = t3f_load_animation("data/graphics/hud_mine.t3a");
	if(!resources->animation[PP2_ANIMATION_HUD_AMMO_MINE])
	{
		printf("Error loading HUD animation %d!\n", PP2_ANIMATION_HUD_AMMO_MINE);
		return false;
	}
	resources->animation[PP2_ANIMATION_HUD_AMMO_BOUNCER] = t3f_load_animation("data/graphics/hud_bouncer.t3a");
	if(!resources->animation[PP2_ANIMATION_HUD_AMMO_BOUNCER])
	{
		printf("Error loading HUD animation %d!\n", PP2_ANIMATION_HUD_AMMO_BOUNCER);
		return false;
	}
	resources->animation[PP2_ANIMATION_HUD_AMMO_SEEKER] = t3f_load_animation("data/graphics/hud_seeker.t3a");
	if(!resources->animation[PP2_ANIMATION_HUD_AMMO_SEEKER])
	{
		printf("Error loading HUD animation %d!\n", PP2_ANIMATION_HUD_AMMO_SEEKER);
		return false;
	}
	resources->animation[PP2_ANIMATION_HUD_AMMO_REFLECTOR] = t3f_load_animation("data/graphics/hud_reflector.t3a");
	if(!resources->animation[PP2_ANIMATION_HUD_AMMO_REFLECTOR])
	{
		printf("Error loading HUD animation %d!\n", PP2_ANIMATION_HUD_AMMO_REFLECTOR);
		return false;
	}
	resources->animation[PP2_ANIMATION_HUD_AMMO_PMINE] = t3f_load_animation("data/graphics/hud_pmine.t3a");
	if(!resources->animation[PP2_ANIMATION_HUD_AMMO_PMINE])
	{
		printf("Error loading HUD animation %d!\n", PP2_ANIMATION_HUD_AMMO_PMINE);
		return false;
	}
	resources->animation[PP2_ANIMATION_HUD_AMMO_GHOST] = t3f_load_animation("data/graphics/hud_ghost.t3a");
	if(!resources->animation[PP2_ANIMATION_HUD_AMMO_GHOST])
	{
		printf("Error loading HUD animation %d!\n", PP2_ANIMATION_HUD_AMMO_GHOST);
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

bool pp2_setup_joynet(PP2_INSTANCE * instance)
{
	int i;

	if(!joynet_init())
	{
		return false;
	}
	pp2_client_game = joynet_create_game(PP2_VERSION_STRING, JOYNET_GAME_TYPE_CONTROLLERS, 16, 4, pp2_game_channel_callback, instance);
	if(!pp2_client_game)
	{
		return false;
	}
	joynet_setup_game_controllers(pp2_client_game, 8, 0, 256);
	joynet_add_game_option(pp2_client_game, (int *)(&instance->game.seed));
	joynet_add_game_option(pp2_client_game, &pp2_end_game_option);
	joynet_add_game_option(pp2_client_game, &instance->game.option[PP2_OPTION_GAME_MODE]);
	joynet_add_game_option(pp2_client_game, &instance->game.option[PP2_OPTION_ELIMINATION_HITS]);
	joynet_add_game_option(pp2_client_game, &instance->game.option[PP2_OPTION_CTF_FLAGS]);
	joynet_add_game_option(pp2_client_game, &instance->game.option[PP2_OPTION_TIME_LIMIT]);
	joynet_add_game_option(pp2_client_game, &instance->game.option[PP2_OPTION_ENABLE_AMMO_NORMAL]);
	joynet_add_game_option(pp2_client_game, &instance->game.option[PP2_OPTION_ENABLE_AMMO_X]);
	joynet_add_game_option(pp2_client_game, &instance->game.option[PP2_OPTION_ENABLE_AMMO_MINE]);
	joynet_add_game_option(pp2_client_game, &instance->game.option[PP2_OPTION_ENABLE_AMMO_BOUNCE]);
	joynet_add_game_option(pp2_client_game, &instance->game.option[PP2_OPTION_ENABLE_AMMO_SEEK]);
	joynet_add_game_option(pp2_client_game, &instance->game.option[PP2_OPTION_ENABLE_AMMO_REFLECTOR]);
	joynet_add_game_option(pp2_client_game, &instance->game.option[PP2_OPTION_ENABLE_AMMO_PMINE]);
	joynet_add_game_option(pp2_client_game, &instance->game.option[PP2_OPTION_ENABLE_AMMO_GHOST]);
	joynet_add_game_option(pp2_client_game, &instance->game.option[PP2_OPTION_ENABLE_POWERUP_CLOAK]);
	joynet_add_game_option(pp2_client_game, &instance->game.option[PP2_OPTION_ENABLE_POWERUP_JUMP]);
	joynet_add_game_option(pp2_client_game, &instance->game.option[PP2_OPTION_ENABLE_POWERUP_RUN]);
	joynet_add_game_option(pp2_client_game, &instance->game.option[PP2_OPTION_ENABLE_POWERUP_DEFLECT]);
	joynet_add_game_option(pp2_client_game, &instance->game.option[PP2_OPTION_ENABLE_POWERUP_FLY]);
	joynet_add_game_option(pp2_client_game, &instance->game.option[PP2_OPTION_ENABLE_POWERUP_TURBO]);
	joynet_add_game_option(pp2_client_game, &instance->game.option[PP2_OPTION_START_AMMO_NORMAL]);
	joynet_add_game_option(pp2_client_game, &instance->game.option[PP2_OPTION_START_AMMO_X]);
	joynet_add_game_option(pp2_client_game, &instance->game.option[PP2_OPTION_START_AMMO_MINE]);
	joynet_add_game_option(pp2_client_game, &instance->game.option[PP2_OPTION_START_AMMO_BOUNCE]);
	joynet_add_game_option(pp2_client_game, &instance->game.option[PP2_OPTION_START_AMMO_SEEK]);
	joynet_add_game_option(pp2_client_game, &instance->game.option[PP2_OPTION_START_AMMO_REFLECTOR]);
	joynet_add_game_option(pp2_client_game, &instance->game.option[PP2_OPTION_START_AMMO_PMINE]);
	joynet_add_game_option(pp2_client_game, &instance->game.option[PP2_OPTION_START_AMMO_GHOST]);
	joynet_add_game_option(pp2_client_game, &instance->game.option[PP2_OPTION_DEATH_MATCH_FRAGS]);
	joynet_add_game_option(pp2_client_game, &instance->game.option[PP2_OPTION_LIFE]);
	joynet_add_game_option(pp2_client_game, &instance->game.option[PP2_OPTION_RANDOMIZE_ITEMS]);
	joynet_add_game_option(pp2_client_game, &instance->game.option[PP2_OPTION_STOMP_HITS]);
	joynet_add_game_option(pp2_client_game, &instance->game.option[PP2_OPTION_AMMO_WORTH]);
	for(i = 0; i < pp2_level_database->entries; i++)
	{
		if(pp2_level_database->entry[i]->extra)
		{
			joynet_add_game_content(pp2_client_game, PP2_CONTENT_LEVELS, pp2_level_database->entry[i]->checksum);
		}
	}
	for(i = 0; i < pp2_character_database->entries; i++)
	{
		if(pp2_character_database->entry[i]->extra)
		{
			joynet_add_game_content(pp2_client_game, PP2_CONTENT_CHARACTERS, pp2_character_database->entry[i]->checksum);
		}
	}
	for(i = 0; i < PP2_MAX_PLAYERS; i++)
	{
		joynet_add_player_option(pp2_client_game, i, &instance->game.player[i].step);
	}
	return true;
}

bool pp2_build_character_database(PP2_RESOURCES * resources)
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

	pp2_character_database = pp2_create_database(t3f_get_filename(t3f_data_path, "characters.ini", buf, 1024), count, pp2_regenerate_cache ? PP2_DATABASE_FLAG_REGENERATE : 0);
	if(!pp2_character_database)
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
				pp2_database_add_files(pp2_character_database, temp_path, search_type[j], 0);
				al_destroy_path(temp_path);
			}
		}
	}
	for(i = 0; i < pp2_character_database->entries; i++)
	{
		pp2_character_database->entry[i]->extra = pp2_character_database_create(pp2_character_database, i, pp2_regenerate_cache ? PP2_DATABASE_FLAG_REGENERATE : 0, resources);
		if(!pp2_character_database->entry[i]->extra)
		{
			return false;
		}
	}
	free(search_path[1]);
	return true;
}

bool pp2_build_level_database(PP2_RESOURCES * resources)
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

	pp2_level_database = pp2_create_database(t3f_get_filename(t3f_data_path, "levels.ini", buf, 1024), count, pp2_regenerate_cache ? PP2_DATABASE_FLAG_REGENERATE : 0);
	if(!pp2_level_database)
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
				pp2_database_add_files(pp2_level_database, temp_path, search_type[j], 0);
				al_destroy_path(temp_path);
			}
		}
	}
	for(i = 0; i < pp2_level_database->entries; i++)
	{
		pp2_level_database->entry[i]->extra = pp2_level_database_create(pp2_level_database, i, pp2_regenerate_cache ? PP2_DATABASE_FLAG_REGENERATE : 0, resources);
		if(!pp2_level_database->entry[i]->extra)
		{
			return false;
		}
	}
	free(search_path[1]);
	return true;
}

bool pp2_build_music_database(void)
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
	val = al_get_config_value(pp2_config, "Music", "Locations");
	if(val)
	{
		locations = atoi(val);
		for(i = 0; i < locations; i++)
		{
			sprintf(location_text, "Location %d", i + 1);
			search_path[search_paths] = (char *)al_get_config_value(pp2_config, "Music", location_text);
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
	pp2_music_database = pp2_create_database(NULL, count, 0);
	if(!pp2_music_database)
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
				pp2_database_add_files(pp2_music_database, temp_path, search_type[j], PP2_DATABASE_FLAG_NO_CHECKSUM);
				al_destroy_path(temp_path);
			}
		}
	}
	free(search_path[2]);
	free(search_path[3]);
	return true;
}

bool pp2_build_demo_database(void)
{
	char buf[1024];
	ALLEGRO_PATH * temp_path = NULL;
	unsigned long count = 0;

	count = pp2_database_count_files("data/demos", ".p2r", 0);
	count += pp2_database_count_files(t3f_get_filename(t3f_data_path, "replays", buf, 1024), ".p2r", 0);
	pp2_demo_database = pp2_create_database(NULL, count, 0);
	if(!pp2_demo_database)
	{
		return false;
	}
	temp_path = al_create_path_for_directory("data/demos");
	if(!temp_path)
	{
		return false;
	}
	pp2_database_add_files(pp2_demo_database, temp_path, ".p2r", PP2_DATABASE_FLAG_NO_CHECKSUM);
	al_destroy_path(temp_path);
	temp_path = al_clone_path(t3f_data_path);
	if(!temp_path)
	{
		return false;
	}
	al_append_path_component(temp_path, "replays");
	pp2_database_add_files(pp2_demo_database, temp_path, ".p2r", PP2_DATABASE_FLAG_NO_CHECKSUM);
	return true;
}
