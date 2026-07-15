#include "t3f/t3f.h"
#include "instance.h"
#include "state.h"
#include "legacy/palette.h"
#include "legacy/animation.h"
#include "pp2/gameplay/sprites/object_defines.h"
#include "menu_proc.h"

static bool _levedit_load_animations(PP2_LEVEL_EDITOR_INSTANCE * level_editor)
{
	int i;

	if(!pp2_legacy_load_palette("data/graphics/legacy_palette.png"))
	{
		return false;
	}
	level_editor->object_animation[PP2_OBJECT_PORTAL] = pp2_legacy_load_animation("data/graphics/portal.ani");
	if(!level_editor->object_animation[PP2_OBJECT_PORTAL])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_PORTAL);
		return false;
	}
	level_editor->object_animation[PP2_OBJECT_AMMO_NORMAL] = t3f_load_animation("data/graphics/ammo_normal.t3a", 0, false);
	if(!level_editor->object_animation[PP2_OBJECT_AMMO_NORMAL])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_AMMO_NORMAL);
		return false;
	}
	level_editor->object_animation[PP2_OBJECT_AMMO_X] = t3f_load_animation("data/graphics/ammo_splitter.t3a", 0, false);
	if(!level_editor->object_animation[PP2_OBJECT_AMMO_X])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_AMMO_X);
		return false;
	}
	level_editor->object_animation[PP2_OBJECT_AMMO_MINE] = t3f_load_animation("data/graphics/ammo_mine.t3a", 0, false);
	if(!level_editor->object_animation[PP2_OBJECT_AMMO_MINE])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_AMMO_MINE);
		return false;
	}
	level_editor->object_animation[PP2_OBJECT_AMMO_BOUNCE] = t3f_load_animation("data/graphics/ammo_bouncer.t3a", 0, false);
	if(!level_editor->object_animation[PP2_OBJECT_AMMO_BOUNCE])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_AMMO_BOUNCE);
		return false;
	}
	level_editor->object_animation[PP2_OBJECT_AMMO_SEEK] = t3f_load_animation("data/graphics/ammo_seeker.t3a", 0, false);
	if(!level_editor->object_animation[PP2_OBJECT_AMMO_SEEK])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_AMMO_SEEK);
		return false;
	}
	level_editor->object_animation[PP2_OBJECT_AMMO_REFLECTOR] = t3f_load_animation("data/graphics/ammo_reflector.t3a", 0, false);
	if(!level_editor->object_animation[PP2_OBJECT_AMMO_REFLECTOR])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_AMMO_REFLECTOR);
		return false;
	}
	level_editor->object_animation[PP2_OBJECT_AMMO_PMINE] = t3f_load_animation("data/graphics/ammo_pmine.t3a", 0, false);
	if(!level_editor->object_animation[PP2_OBJECT_AMMO_PMINE])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_AMMO_PMINE);
		return false;
	}
	level_editor->object_animation[PP2_OBJECT_AMMO_GHOST] = t3f_load_animation("data/graphics/ammo_ghost.t3a", 0, false);
	if(!level_editor->object_animation[PP2_OBJECT_AMMO_GHOST])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_AMMO_GHOST);
		return false;
	}
	level_editor->object_animation[PP2_OBJECT_POWER_CLOAK] = t3f_load_animation("data/graphics/powerup_cloak.t3a", 0, false);
	if(!level_editor->object_animation[PP2_OBJECT_POWER_CLOAK])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_POWER_CLOAK);
		return false;
	}
	level_editor->object_animation[PP2_OBJECT_POWER_JUMP] = t3f_load_animation("data/graphics/powerup_jump.t3a", 0, false);
	if(!level_editor->object_animation[PP2_OBJECT_POWER_JUMP])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_POWER_JUMP);
		return false;
	}
	level_editor->object_animation[PP2_OBJECT_POWER_RUN] = t3f_load_animation("data/graphics/powerup_run.t3a", 0, false);
	if(!level_editor->object_animation[PP2_OBJECT_POWER_RUN])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_POWER_RUN);
		return false;
	}
	level_editor->object_animation[PP2_OBJECT_POWER_DEFLECT] = t3f_load_animation("data/graphics/powerup_deflect.t3a", 0, false);
	if(!level_editor->object_animation[PP2_OBJECT_POWER_DEFLECT])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_POWER_DEFLECT);
		return false;
	}
	level_editor->object_animation[PP2_OBJECT_POWER_FLY] = t3f_load_animation("data/graphics/powerup_fly.t3a", 0, false);
	if(!level_editor->object_animation[PP2_OBJECT_POWER_FLY])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_POWER_FLY);
		return false;
	}
	level_editor->object_animation[PP2_OBJECT_POWER_TURBO] = t3f_load_animation("data/graphics/powerup_turbo.t3a", 0, false);
	if(!level_editor->object_animation[PP2_OBJECT_POWER_TURBO])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_POWER_TURBO);
		return false;
	}
	level_editor->object_animation[PP2_OBJECT_SPRING_UP] = pp2_legacy_load_animation("data/graphics/spring_up.ani");
	if(!level_editor->object_animation[PP2_OBJECT_SPRING_UP])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_SPRING_UP);
		return false;
	}
	level_editor->object_animation[PP2_OBJECT_SPRING_DOWN] = pp2_legacy_load_animation("data/graphics/spring_down.ani");
	if(!level_editor->object_animation[PP2_OBJECT_SPRING_DOWN])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_SPRING_DOWN);
		return false;
	}
	level_editor->object_animation[PP2_OBJECT_SPRING_LEFT] = pp2_legacy_load_animation("data/graphics/spring_left.ani");
	if(!level_editor->object_animation[PP2_OBJECT_SPRING_LEFT])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_SPRING_LEFT);
		return false;
	}
	level_editor->object_animation[PP2_OBJECT_SPRING_RIGHT] = pp2_legacy_load_animation("data/graphics/spring_right.ani");
	if(!level_editor->object_animation[PP2_OBJECT_SPRING_RIGHT])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_SPRING_RIGHT);
		return false;
	}
	level_editor->object_animation[PP2_OBJECT_TILE_READER] = t3f_load_animation("data/graphics/tile_reader.png", 0, false);
	if(!level_editor->object_animation[PP2_OBJECT_TILE_READER])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_TILE_READER);
		return false;
	}
	level_editor->object_atlas = t3f_create_atlas(1024, 1024);
	if(level_editor->object_atlas)
	{
		for(i = 0; i < 256; i++)
		{
			if(level_editor->object_animation[i])
			{
				t3f_add_animation_to_atlas(level_editor->object_atlas, level_editor->object_animation[i], T3F_ATLAS_SPRITE);
			}
		}
	}
	return true;
}

PP2_LEVEL_EDITOR_INSTANCE * pp2_create_level_editor_instance(int argc, char * argv[])
{
  PP2_LEVEL_EDITOR_INSTANCE * level_editor = NULL;

  level_editor = malloc(sizeof(PP2_LEVEL_EDITOR_INSTANCE));
  if(!level_editor)
  {
    goto fail;
  }
  memset(level_editor, 0, sizeof(PP2_LEVEL_EDITOR_INSTANCE));

	if(!t3f_initialize("Level Builder", 960, 540, 60.0, levedit_logic, levedit_render, T3F_DEFAULT | T3F_USE_MENU, level_editor))
	{
		return false;
	}
	al_set_new_bitmap_flags(ALLEGRO_NO_PREMULTIPLIED_ALPHA);
	al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
	level_editor->font = t3f_load_font("data/fonts/chared_font.png", T3F_FONT_TYPE_AUTO, 0, 0, false);
	if(!level_editor->font)
	{
		return false;
	}
	if(!_levedit_load_animations(level_editor))
	{
		return false;
	}
	level_editor->menu[LEVEDIT_MENU_FILE_IMPORT] = al_create_menu();
	if(level_editor->menu[LEVEDIT_MENU_FILE_IMPORT])
	{
		t3f_add_menu_item(level_editor->menu[LEVEDIT_MENU_FILE_IMPORT], "Import Tile&set", 0, NULL, levedit_menu_proc_import_tileset, levedit_menu_update_proc_loaded);
		t3f_add_menu_item(level_editor->menu[LEVEDIT_MENU_FILE_IMPORT], "Import Tile&map", 0, NULL, levedit_menu_proc_import_tilemap, levedit_menu_update_proc_loaded);
	}
	level_editor->menu[LEVEDIT_MENU_FILE_EXPORT] = al_create_menu();
	if(level_editor->menu[LEVEDIT_MENU_FILE_EXPORT])
	{
		t3f_add_menu_item(level_editor->menu[LEVEDIT_MENU_FILE_EXPORT], "Export Tile&set", 0, NULL, levedit_menu_proc_export_tileset, levedit_menu_update_proc_loaded);
		t3f_add_menu_item(level_editor->menu[LEVEDIT_MENU_FILE_EXPORT], "Export Tile&map", 0, NULL, levedit_menu_proc_export_tilemap, levedit_menu_update_proc_loaded);
	}
	level_editor->menu[LEVEDIT_MENU_FILE] = al_create_menu();
	if(level_editor->menu[LEVEDIT_MENU_FILE])
	{
		t3f_add_menu_item(level_editor->menu[LEVEDIT_MENU_FILE], "&New", 0, NULL, levedit_menu_proc_new, NULL);
		t3f_add_menu_item(level_editor->menu[LEVEDIT_MENU_FILE], "&Open", 0, NULL, levedit_menu_proc_open, NULL);
		t3f_add_menu_item(level_editor->menu[LEVEDIT_MENU_FILE], "", 0, NULL, NULL, NULL);
		t3f_add_menu_item(level_editor->menu[LEVEDIT_MENU_FILE], "&Save", 0, NULL, levedit_menu_proc_save, levedit_menu_update_proc_loaded);
		t3f_add_menu_item(level_editor->menu[LEVEDIT_MENU_FILE], "Save &As", 0, NULL, levedit_menu_proc_save_as, levedit_menu_update_proc_loaded);
		t3f_add_menu_item(level_editor->menu[LEVEDIT_MENU_FILE], "", 0, NULL, NULL, NULL);
		t3f_add_menu_item(level_editor->menu[LEVEDIT_MENU_FILE], "Import", 0, level_editor->menu[LEVEDIT_MENU_FILE_IMPORT], NULL, NULL);
		t3f_add_menu_item(level_editor->menu[LEVEDIT_MENU_FILE], "Export", 0, level_editor->menu[LEVEDIT_MENU_FILE_EXPORT], NULL, NULL);
		t3f_add_menu_item(level_editor->menu[LEVEDIT_MENU_FILE], "", 0, NULL, NULL, NULL);
		t3f_add_menu_item(level_editor->menu[LEVEDIT_MENU_FILE], "Quit", 0, NULL, levedit_menu_proc_quit, NULL);
	}
	level_editor->menu[LEVEDIT_MENU_MAIN] = al_create_menu();
	if(level_editor->menu[LEVEDIT_MENU_MAIN])
	{
		t3f_add_menu_item(level_editor->menu[LEVEDIT_MENU_MAIN], "File", 0, level_editor->menu[LEVEDIT_MENU_FILE], NULL, NULL);
	}
	t3f_attach_menu(level_editor->menu[LEVEDIT_MENU_MAIN]);
	t3f_refresh_menus();

	return level_editor;

  fail:
  {
    pp2_destroy_level_editor_instance(level_editor);
		return NULL;
  }
}

void pp2_destroy_level_editor_instance(PP2_LEVEL_EDITOR_INSTANCE * level_editor)
{
  if(level_editor)
  {
    free(level_editor);
  }
}
