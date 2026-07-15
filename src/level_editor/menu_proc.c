#include "t3f/t3f.h"
#include "instance.h"
#include "legacy/level.h"

const char * levedit_get_extension(const char * fn)
{
	int i;

	for(i = strlen(fn) - 1; i >= 0; i++)
	{
		if(fn[i] == '.')
		{
			break;
		}
	}
	return &fn[i];
}

int levedit_menu_update_proc_loaded(ALLEGRO_MENU * mp, int i, void * data)
{
	PP2_LEVEL_EDITOR_INSTANCE * level_editor = (PP2_LEVEL_EDITOR_INSTANCE *)data;

	if(level_editor->level)
	{
		al_set_menu_item_flags(mp, i, 0);
	}
	else
	{
		al_set_menu_item_flags(mp, i, ALLEGRO_MENU_ITEM_DISABLED);
	}
	return 0;
}

int levedit_menu_proc_new(int i, void * data)
{
	PP2_LEVEL_EDITOR_INSTANCE * level_editor = (PP2_LEVEL_EDITOR_INSTANCE *)data;

	if(level_editor->level)
	{
		pp2_destroy_level(level_editor->level);
		level_editor->level = NULL;
	}
	level_editor->level = pp2_create_level();
	if(level_editor->level)
	{
		level_editor->level->objects = 0;
		level_editor->level->object = malloc(sizeof(PP2_LEVEL_OBJECT) * 256);
		if(!level_editor->level->object)
		{
			pp2_destroy_level(level_editor->level);
			level_editor->level = NULL;
		}
	}
	t3f_refresh_menus();
	return 0;
}

int levedit_menu_proc_open(int i, void * data)
{
	PP2_LEVEL_EDITOR_INSTANCE * level_editor = (PP2_LEVEL_EDITOR_INSTANCE *)data;
	al_stop_timer(t3f_timer);
	level_editor->file_load_dialog = al_create_native_file_dialog(level_editor->path, "Load Level", "*.ppl;*.p2c", 0);
	al_show_native_file_dialog(t3f_display, level_editor->file_load_dialog);
	if(al_get_native_file_dialog_count(level_editor->file_load_dialog) > 0)
	{
		level_editor->path = al_get_native_file_dialog_path(level_editor->file_load_dialog, 0);
		if(!strcasecmp(levedit_get_extension(level_editor->path), ".p2l"))
		{
			if(t3f_key_held(ALLEGRO_KEY_LSHIFT))
			{
				level_editor->level = pp2_load_old_level(level_editor->path, 0);
			}
			else
			{
				level_editor->level = pp2_load_level(level_editor->path, 0);
			}
		}
		else
		{
			level_editor->level = pp2_load_legacy_level(level_editor->path, 0);
		}
	}
	al_destroy_native_file_dialog(level_editor->file_load_dialog);
	al_resume_timer(t3f_timer);
	t3f_refresh_menus();
	return 0;
}

int levedit_menu_proc_save(int i, void * data)
{
	PP2_LEVEL_EDITOR_INSTANCE * level_editor = (PP2_LEVEL_EDITOR_INSTANCE *)data;

	ALLEGRO_PATH * temp_path = NULL;
	al_stop_timer(t3f_timer);
	level_editor->file_save_dialog = al_create_native_file_dialog(level_editor->path, "Save Level", "*.ppl;*.p2c", ALLEGRO_FILECHOOSER_SAVE);
	al_show_native_file_dialog(t3f_display, level_editor->file_save_dialog);
	if(al_get_native_file_dialog_count(level_editor->file_save_dialog) > 0)
	{
		level_editor->path = al_get_native_file_dialog_path(level_editor->file_save_dialog, 0);
		temp_path = al_create_path(level_editor->path);
		if(temp_path)
		{
			al_set_path_extension(temp_path, ".p2l");
			level_editor->level->flags = 0;
			pp2_save_level(level_editor->level, al_path_cstr(temp_path, '/'));
			al_destroy_path(temp_path);
		}
	}
	al_destroy_native_file_dialog(level_editor->file_save_dialog);
	al_resume_timer(t3f_timer);
	return 0;
}

int levedit_menu_proc_save_as(int i, void * data)
{
	PP2_LEVEL_EDITOR_INSTANCE * level_editor = (PP2_LEVEL_EDITOR_INSTANCE *)data;

	return 0;
}

int levedit_menu_proc_import_tileset(int i, void * data)
{
	PP2_LEVEL_EDITOR_INSTANCE * level_editor = (PP2_LEVEL_EDITOR_INSTANCE *)data;

	al_stop_timer(t3f_timer);
	level_editor->file_load_dialog = al_create_native_file_dialog(level_editor->path, "Import Tileset", "*.t3t", 0);
	al_show_native_file_dialog(t3f_display, level_editor->file_load_dialog);
	if(al_get_native_file_dialog_count(level_editor->file_load_dialog) > 0)
	{
		level_editor->path = al_get_native_file_dialog_path(level_editor->file_load_dialog, 0);
		if(level_editor->level->tileset)
		{
			t3f_destroy_tileset(level_editor->level->tileset);
		}
		level_editor->level->tileset = t3f_load_tileset(level_editor->path);
	}
	al_destroy_native_file_dialog(level_editor->file_load_dialog);
	al_resume_timer(t3f_timer);
	return 0;
}

int levedit_menu_proc_import_tilemap(int i, void * data)
{
	PP2_LEVEL_EDITOR_INSTANCE * level_editor = (PP2_LEVEL_EDITOR_INSTANCE *)data;

	al_stop_timer(t3f_timer);
	level_editor->file_load_dialog = al_create_native_file_dialog(level_editor->path, "Import Tilemap", "*.t3m", 0);
	al_show_native_file_dialog(t3f_display, level_editor->file_load_dialog);
	if(al_get_native_file_dialog_count(level_editor->file_load_dialog) > 0)
	{
		level_editor->path = al_get_native_file_dialog_path(level_editor->file_load_dialog, 0);
		if(level_editor->level->tilemap)
		{
			t3f_destroy_tilemap(level_editor->level->tilemap);
		}
		level_editor->level->tilemap = t3f_load_tilemap(level_editor->path);
	}
	al_destroy_native_file_dialog(level_editor->file_load_dialog);
	al_resume_timer(t3f_timer);
	return 0;
}

int levedit_menu_proc_export_tileset(int i, void * data)
{
	PP2_LEVEL_EDITOR_INSTANCE * level_editor = (PP2_LEVEL_EDITOR_INSTANCE *)data;
	ALLEGRO_PATH * temp_path = NULL;
	al_stop_timer(t3f_timer);
	level_editor->file_save_dialog = al_create_native_file_dialog(level_editor->path, "Export Tileset", "*.t3t", ALLEGRO_FILECHOOSER_SAVE);
	al_show_native_file_dialog(t3f_display, level_editor->file_save_dialog);
	if(al_get_native_file_dialog_count(level_editor->file_save_dialog) > 0)
	{
		level_editor->path = al_get_native_file_dialog_path(level_editor->file_save_dialog, 0);
		temp_path = al_create_path(level_editor->path);
		if(temp_path)
		{
			al_set_path_extension(temp_path, ".t3t");
			t3f_save_tileset(level_editor->level->tileset, al_path_cstr(temp_path, '/'));
			al_destroy_path(temp_path);
		}
	}
	al_destroy_native_file_dialog(level_editor->file_save_dialog);
	al_resume_timer(t3f_timer);
	return 0;
}

int levedit_menu_proc_export_tilemap(int i, void * data)
{
	PP2_LEVEL_EDITOR_INSTANCE * level_editor = (PP2_LEVEL_EDITOR_INSTANCE *)data;
	ALLEGRO_PATH * temp_path = NULL;
	al_stop_timer(t3f_timer);
	level_editor->file_save_dialog = al_create_native_file_dialog(level_editor->path, "Export Tilemap", "*.t3m", ALLEGRO_FILECHOOSER_SAVE);
	al_show_native_file_dialog(t3f_display, level_editor->file_save_dialog);
	if(al_get_native_file_dialog_count(level_editor->file_save_dialog) > 0)
	{
		level_editor->path = al_get_native_file_dialog_path(level_editor->file_save_dialog, 0);
		temp_path = al_create_path(level_editor->path);
		if(temp_path)
		{
			al_set_path_extension(temp_path, ".t3m");
			t3f_save_tilemap(level_editor->level->tilemap, al_path_cstr(temp_path, '/'));
			al_destroy_path(temp_path);
		}
	}
	al_destroy_native_file_dialog(level_editor->file_save_dialog);
	al_resume_timer(t3f_timer);
	return 0;
}

int levedit_menu_proc_quit(int i, void * data)
{
	PP2_LEVEL_EDITOR_INSTANCE * level_editor = (PP2_LEVEL_EDITOR_INSTANCE *)data;
	t3f_exit();
	return 0;
}

