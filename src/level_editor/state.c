#include "t3f/t3f.h"
#include "instance.h"

#define LEVEDIT_VIEW_LEVEL 0
#define LEVEDIT_VIEW_META  1

void levedit_get_entry_pos(PP2_LEVEL_EDITOR_INSTANCE * level_editor)
{
	switch(level_editor->selected_meta)
	{
		case 0:
		{
			level_editor->entry_pos = strlen(level_editor->level->info.name);
			level_editor->entry_pointer = level_editor->level->info.name;
			break;
		}
		case 1:
		{
			level_editor->entry_pos = strlen(level_editor->level->info.author);
			level_editor->entry_pointer = level_editor->level->info.author;
			break;
		}
		case 2:
		{
			level_editor->entry_pos = strlen(level_editor->level->info.comment);
			level_editor->entry_pointer = level_editor->level->info.comment;
			break;
		}
	}
}

void levedit_previous_object(PP2_LEVEL_EDITOR_INSTANCE * level_editor)
{
	while(1)
	{
		level_editor->object_type--;
		if(level_editor->object_type < 0)
		{
			level_editor->object_type = 255;
		}
		if(level_editor->object_animation[level_editor->object_type])
		{
			break;
		}
	}
}

void levedit_next_object(PP2_LEVEL_EDITOR_INSTANCE * level_editor)
{
	while(1)
	{
		level_editor->object_type++;
		if(level_editor->object_type > 255)
		{
			level_editor->object_type = 0;
		}
		if(level_editor->object_animation[level_editor->object_type])
		{
			break;
		}
	}
}

void levedit_delete_object(PP2_LEVEL_EDITOR_INSTANCE * level_editor)
{
	int i;

	if(level_editor->selected_object >= 0 && level_editor->selected_object < level_editor->level->objects)
	{
		for(i = level_editor->selected_object; i < level_editor->level->objects - 1; i++)
		{
			memcpy(&level_editor->level->object[i], &level_editor->level->object[i + 1], sizeof(PP2_LEVEL_OBJECT));
		}
		level_editor->level->objects--;
		if(level_editor->selected_object >= level_editor->level->objects)
		{
			level_editor->selected_object = 0;
		}
	}
}

void levedit_create_collision_tilemap_if_needed(PP2_LEVEL_EDITOR_INSTANCE * level_editor)
{
	if(!level_editor->level->collision_tilemap[level_editor->selected_layer])
	{
		level_editor->level->collision_tilemap[level_editor->selected_layer] = t3f_create_collision_tilemap(level_editor->level->tilemap->layer[level_editor->selected_layer]->width, level_editor->level->tilemap->layer[level_editor->selected_layer]->height, 32, 32);
	}
}

void levedit_move_logic(PP2_LEVEL_EDITOR_INSTANCE * level_editor)
{
	if(t3f_key_pressed(ALLEGRO_KEY_LEFT))
	{
		if(t3f_key_held(ALLEGRO_KEY_LSHIFT))
		{
			level_editor->cx -= level_editor->level->tileset->width;
			level_editor->cx = (int)(level_editor->cx / level_editor->level->tileset->width) * level_editor->level->tileset->width;
			t3f_use_key_press(ALLEGRO_KEY_LEFT);
		}
		else
		{
			level_editor->cx -= 1.0;
		}
	}
	if(t3f_key_pressed(ALLEGRO_KEY_RIGHT))
	{
		if(t3f_key_held(ALLEGRO_KEY_LSHIFT))
		{
			level_editor->cx += level_editor->level->tileset->width;
			level_editor->cx = (int)(level_editor->cx / level_editor->level->tileset->width) * level_editor->level->tileset->width;
			t3f_use_key_press(ALLEGRO_KEY_RIGHT);
		}
		else
		{
			level_editor->cx += 1.0;
		}
	}
	if(t3f_key_pressed(ALLEGRO_KEY_UP))
	{
		if(t3f_key_held(ALLEGRO_KEY_LSHIFT))
		{
			level_editor->cy -= level_editor->level->tileset->height;
			level_editor->cy = (int)(level_editor->cy / level_editor->level->tileset->height) * level_editor->level->tileset->height;
			t3f_use_key_press(ALLEGRO_KEY_UP);
		}
		else
		{
			level_editor->cy -= 1.0;
		}
	}
	if(t3f_key_pressed(ALLEGRO_KEY_DOWN))
	{
		if(t3f_key_held(ALLEGRO_KEY_LSHIFT))
		{
			level_editor->cy += level_editor->level->tileset->height;
			level_editor->cy = (int)(level_editor->cy / level_editor->level->tileset->height) * level_editor->level->tileset->height;
			t3f_use_key_press(ALLEGRO_KEY_DOWN);
		}
		else
		{
			level_editor->cy += 1.0;
		}
	}
	level_editor->cz = level_editor->level->tilemap->layer[level_editor->selected_layer]->z;
}

void levedit_add_object(PP2_LEVEL_EDITOR_INSTANCE * level_editor, int type, int x, int y, int flags)
{
	if(level_editor->level->objects < 256)
	{
		level_editor->level->object[level_editor->level->objects].type = type;
		level_editor->level->object[level_editor->level->objects].x = x;
		level_editor->level->object[level_editor->level->objects].y = y;
		level_editor->level->object[level_editor->level->objects].layer = level_editor->selected_layer;
		level_editor->level->object[level_editor->level->objects].flags = flags;
		level_editor->selected_object = level_editor->level->objects;
		level_editor->level->objects++;
	}
}

void levedit_level_logic(PP2_LEVEL_EDITOR_INSTANCE * level_editor)
{
	int mx, my, mz;
	int i;

	if(t3f_key_pressed(ALLEGRO_KEY_MINUS))
	{
		if(t3f_key_held(ALLEGRO_KEY_LCTRL))
		{
			level_editor->selected_tile--;
			if(level_editor->selected_tile < 0)
			{
				level_editor->selected_tile = level_editor->level->tileset->tiles - 1;
			}
		}
		else
		{
			levedit_previous_object(level_editor);
		}
		t3f_use_key_press(ALLEGRO_KEY_MINUS);
	}
	if(t3f_key_pressed(ALLEGRO_KEY_EQUALS))
	{
		if(t3f_key_held(ALLEGRO_KEY_LCTRL))
		{
			level_editor->selected_tile++;
			if(level_editor->selected_tile >= level_editor->level->tileset->tiles)
			{
				level_editor->selected_tile = 0;
			}
		}
		else
		{
			levedit_next_object(level_editor);
		}
		t3f_use_key_press(ALLEGRO_KEY_EQUALS);
	}
	if(t3f_key_pressed(ALLEGRO_KEY_DELETE))
	{
		levedit_delete_object(level_editor);
		t3f_use_key_press(ALLEGRO_KEY_DELETE);
	}
	if(t3f_key_pressed(ALLEGRO_KEY_PGUP))
	{
		level_editor->selected_layer++;
		if(level_editor->selected_layer >= level_editor->level->tilemap->layers)
		{
			level_editor->selected_layer = 0;
		}
		t3f_use_key_press(ALLEGRO_KEY_PGUP);
	}
	if(t3f_key_pressed(ALLEGRO_KEY_PGDN))
	{
		level_editor->selected_layer--;
		if(level_editor->selected_layer < 0)
		{
			level_editor->selected_layer = level_editor->level->tilemap->layers - 1;
		}
		t3f_use_key_press(ALLEGRO_KEY_PGDN);
	}
	level_editor->hover_object = -1;
	level_editor->place_x = t3f_get_mouse_x() - level_editor->object_animation[level_editor->object_type]->data->frame[0]->width / 2 + level_editor->cx;
	level_editor->place_y = t3f_get_mouse_y() - level_editor->object_animation[level_editor->object_type]->data->frame[0]->height / 2 + level_editor->cy;
	if(t3f_key_held(ALLEGRO_KEY_LCTRL))
	{
		level_editor->place_x = (level_editor->place_x / 16) * 16;
		level_editor->place_y = (level_editor->place_y / 16) * 16;
	}
	for(i = 0; i < level_editor->level->objects; i++)
	{
		if(level_editor->level->object[i].type >= 0 && level_editor->level->object[i].type < 256 && level_editor->level->object[i].layer == level_editor->selected_layer && level_editor->object_animation[level_editor->level->object[i].type])
		{
			if(t3f_get_mouse_x() + level_editor->cx >= level_editor->level->object[i].x && t3f_get_mouse_x() + level_editor->cx < level_editor->level->object[i].x + level_editor->object_animation[level_editor->level->object[i].type]->data->frame[0]->width && t3f_get_mouse_y() + level_editor->cy >= level_editor->level->object[i].y && t3f_get_mouse_y() + level_editor->cy < level_editor->level->object[i].y + level_editor->object_animation[level_editor->level->object[i].type]->data->frame[0]->height)
			{
				level_editor->hover_object = i;
				break;
			}
		}
	}
	t3f_get_mouse_mickeys(&mx, &my, &mz);
	level_editor->hover_tile = -1;
	level_editor->tilemap_hover_x = (int)(t3f_get_mouse_x() + level_editor->cx) / level_editor->level->tileset->width;
	level_editor->tilemap_hover_y = (int)(t3f_get_mouse_y() + level_editor->cy) / level_editor->level->tileset->height;
	if(level_editor->level->collision_tilemap[level_editor->selected_layer])
	{
		if(level_editor->tilemap_hover_x < 0 || level_editor->tilemap_hover_x >= level_editor->level->collision_tilemap[level_editor->selected_layer]->width || level_editor->tilemap_hover_y < 0 || level_editor->tilemap_hover_y >= level_editor->level->collision_tilemap[level_editor->selected_layer]->height)
		{
			level_editor->tilemap_hover_fail = true;
		}
		else
		{
			level_editor->tilemap_hover_fail = false;
			level_editor->hover_tile = level_editor->level->tilemap->layer[level_editor->selected_layer]->data[level_editor->tilemap_hover_y][level_editor->tilemap_hover_x];
		}
	}
	if(t3f_key_held(ALLEGRO_KEY_LCTRL))
	{
		if(t3f_key_pressed(ALLEGRO_KEY_W))
		{
			level_editor->level->room.y = level_editor->tilemap_hover_y;
			t3f_use_key_press(ALLEGRO_KEY_W);
		}
		if(t3f_key_pressed(ALLEGRO_KEY_A))
		{
			level_editor->level->room.x = level_editor->tilemap_hover_x;
			t3f_use_key_press(ALLEGRO_KEY_A);
		}
		if(t3f_key_pressed(ALLEGRO_KEY_S))
		{
			level_editor->level->room.by = level_editor->tilemap_hover_y;
			t3f_use_key_press(ALLEGRO_KEY_S);
		}
		if(t3f_key_pressed(ALLEGRO_KEY_D))
		{
			level_editor->level->room.bx = level_editor->tilemap_hover_x;
			t3f_use_key_press(ALLEGRO_KEY_D);
		}
	}
	else if(t3f_key_held(ALLEGRO_KEY_LSHIFT))
	{
		if(t3f_key_pressed(ALLEGRO_KEY_W))
		{
			level_editor->level->tileset->tile[level_editor->selected_tile]->user_data[15] ^= T3F_COLLISION_FLAG_SOLID_TOP;
			t3f_use_key_press(ALLEGRO_KEY_W);
		}
		if(t3f_key_pressed(ALLEGRO_KEY_S))
		{
			level_editor->level->tileset->tile[level_editor->selected_tile]->user_data[15] ^= T3F_COLLISION_FLAG_SOLID_BOTTOM;
			t3f_use_key_press(ALLEGRO_KEY_S);
		}
		if(t3f_key_pressed(ALLEGRO_KEY_A))
		{
			level_editor->level->tileset->tile[level_editor->selected_tile]->user_data[15] ^= T3F_COLLISION_FLAG_SOLID_LEFT;
			t3f_use_key_press(ALLEGRO_KEY_A);
		}
		if(t3f_key_pressed(ALLEGRO_KEY_D))
		{
			level_editor->level->tileset->tile[level_editor->selected_tile]->user_data[15] ^= T3F_COLLISION_FLAG_SOLID_RIGHT;
			t3f_use_key_press(ALLEGRO_KEY_D);
		}
		if(level_editor->level->tileset->tile[level_editor->selected_tile]->user_data[15])
		{
			level_editor->level->tileset->tile[level_editor->selected_tile]->flags |= T3F_TILE_FLAG_USER_DATA;
		}
		else
		{
			level_editor->level->tileset->tile[level_editor->selected_tile]->flags &= ~T3F_TILE_FLAG_USER_DATA;
		}
	}
	else
	{
		if(t3f_key_pressed(ALLEGRO_KEY_W))
		{
			levedit_create_collision_tilemap_if_needed(level_editor);
			level_editor->level->collision_tilemap[level_editor->selected_layer]->data[level_editor->tilemap_hover_y][level_editor->tilemap_hover_x].flags ^= T3F_COLLISION_FLAG_SOLID_TOP;
			if((level_editor->level->collision_tilemap[level_editor->selected_layer]->data[level_editor->tilemap_hover_y][level_editor->tilemap_hover_x].flags & T3F_COLLISION_FLAG_SOLID_TOP) && t3f_key_held(ALLEGRO_KEY_ALT))
			{
				level_editor->level->collision_tilemap[level_editor->selected_layer]->data[level_editor->tilemap_hover_y][level_editor->tilemap_hover_x].flags |= PP2_LEVEL_COLLISION_FLAG_SECRET;
			}
			else
			{
				level_editor->level->collision_tilemap[level_editor->selected_layer]->data[level_editor->tilemap_hover_y][level_editor->tilemap_hover_x].flags &= ~PP2_LEVEL_COLLISION_FLAG_SECRET;
			}
			t3f_use_key_press(ALLEGRO_KEY_W);
		}
		if(t3f_key_pressed(ALLEGRO_KEY_S))
		{
			levedit_create_collision_tilemap_if_needed(level_editor);
			level_editor->level->collision_tilemap[level_editor->selected_layer]->data[level_editor->tilemap_hover_y][level_editor->tilemap_hover_x].flags ^= T3F_COLLISION_FLAG_SOLID_BOTTOM;
			t3f_use_key_press(ALLEGRO_KEY_S);
		}
		if(t3f_key_pressed(ALLEGRO_KEY_A))
		{
			levedit_create_collision_tilemap_if_needed(level_editor);
			level_editor->level->collision_tilemap[level_editor->selected_layer]->data[level_editor->tilemap_hover_y][level_editor->tilemap_hover_x].flags ^= T3F_COLLISION_FLAG_SOLID_LEFT;
			t3f_use_key_press(ALLEGRO_KEY_A);
		}
		if(t3f_key_pressed(ALLEGRO_KEY_D))
		{
			levedit_create_collision_tilemap_if_needed(level_editor);
			level_editor->level->collision_tilemap[level_editor->selected_layer]->data[level_editor->tilemap_hover_y][level_editor->tilemap_hover_x].flags ^= T3F_COLLISION_FLAG_SOLID_RIGHT;
			t3f_use_key_press(ALLEGRO_KEY_D);
		}
		if(t3f_key_pressed(ALLEGRO_KEY_Q))
		{
			levedit_create_collision_tilemap_if_needed(level_editor);
			level_editor->level->collision_tilemap[level_editor->selected_layer]->data[level_editor->tilemap_hover_y][level_editor->tilemap_hover_x].flags ^= T3F_COLLISION_FLAG_SOLID_RIGHT | T3F_COLLISION_FLAG_SOLID_LEFT | T3F_COLLISION_FLAG_SOLID_TOP | T3F_COLLISION_FLAG_SOLID_BOTTOM;
			t3f_use_key_press(ALLEGRO_KEY_Q);
		}
		if(t3f_key_pressed(ALLEGRO_KEY_E))
		{
			levedit_create_collision_tilemap_if_needed(level_editor);
			level_editor->level->collision_tilemap[level_editor->selected_layer]->data[level_editor->tilemap_hover_y][level_editor->tilemap_hover_x].flags ^= PP2_LEVEL_COLLISION_FLAG_SECRET;
			t3f_use_key_press(ALLEGRO_KEY_E);
		}
		if(t3f_key_pressed(ALLEGRO_KEY_I))
		{
			levedit_create_collision_tilemap_if_needed(level_editor);
			level_editor->level->collision_tilemap[level_editor->selected_layer]->data[level_editor->tilemap_hover_y][level_editor->tilemap_hover_x].flags ^= PP2_LEVEL_COLLISION_FLAG_ICE;
			t3f_use_key_press(ALLEGRO_KEY_I);
		}
	}
	if(t3f_key_held(ALLEGRO_KEY_T) && level_editor->hover_tile >= 0)
	{
		level_editor->selected_tile = level_editor->hover_tile;
	}
	levedit_move_logic(level_editor);
	if(level_editor->hover_object >= 0)
	{
		if(t3f_mouse_button_pressed(0))
		{
			level_editor->selected_object = level_editor->hover_object;
			if(t3f_key_held(ALLEGRO_KEY_LCTRL))
			{
				level_editor->level->object[level_editor->hover_object].x = ((int)(t3f_get_mouse_x() + level_editor->cx) / 8) * 8 - level_editor->object_animation[level_editor->level->object[level_editor->hover_object].type]->data->frame[0]->width / 2;
				level_editor->level->object[level_editor->hover_object].y = ((int)(t3f_get_mouse_y() + level_editor->cy) / 8) * 8 - level_editor->object_animation[level_editor->level->object[level_editor->hover_object].type]->data->frame[0]->height / 2;
			}
			else
			{
				level_editor->level->object[level_editor->hover_object].x -= mx;
				level_editor->level->object[level_editor->hover_object].y -= my;
			}
		}
		t3f_use_mouse_button_press(0);
	}
	else
	{
		if(t3f_mouse_button_pressed(1))
		{
			if(!level_editor->clicked)
			{
				levedit_add_object(level_editor, level_editor->object_type, level_editor->place_x, level_editor->place_y, 0);
				level_editor->clicked = true;
			}
		}
		else
		{
			level_editor->clicked = false;
		}
		t3f_use_mouse_button_press(1);
	}
	level_editor->cz += mz * 16;

	/* fix camera */
	if(level_editor->cx < 0)
	{
		level_editor->cx = 0;
	}
	else if(level_editor->cx >= 32 * level_editor->level->tilemap->layer[0]->width - 640)
	{
		level_editor->cx = 32 * level_editor->level->tilemap->layer[0]->width - 640 - 1;
	}
	if(level_editor->cy < 0)
	{
		level_editor->cy = 0;
	}
	else if(level_editor->cy >= 32 * level_editor->level->tilemap->layer[0]->height - 480)
	{
		level_editor->cy = 32 * level_editor->level->tilemap->layer[0]->height - 480 - 1;
	}
}

void levedit_meta_logic(PP2_LEVEL_EDITOR_INSTANCE * level_editor)
{
	int input = 0;

	if(t3f_key_pressed(ALLEGRO_KEY_LEFT))
	{
		level_editor->selected_meta--;
		if(level_editor->selected_meta < 0)
		{
			level_editor->selected_meta = 2;
		}
		levedit_get_entry_pos(level_editor);
		t3f_use_key_press(ALLEGRO_KEY_LEFT);
	}
	if(t3f_key_pressed(ALLEGRO_KEY_RIGHT))
	{
		level_editor->selected_meta++;
		if(level_editor->selected_meta > 2)
		{
			level_editor->selected_meta = 2;
		}
		levedit_get_entry_pos(level_editor);
		t3f_use_key_press(ALLEGRO_KEY_RIGHT);
	}

	input = t3f_get_char(0);
	if(input)
	{
		if(input == '\b' || input == 127)
		{
			if(level_editor->entry_pos > 0)
			{
				level_editor->entry_pos--;
				level_editor->entry_pointer[level_editor->entry_pos] = '\0';
			}
		}
		else if(input >= 32 && input < 127)
		{
			level_editor->entry_pointer[level_editor->entry_pos] = input;
			level_editor->entry_pos++;
			level_editor->entry_pointer[level_editor->entry_pos] = '\0';
		}
	}
}

void levedit_logic(void * data)
{
  PP2_LEVEL_EDITOR_INSTANCE * level_editor = (PP2_LEVEL_EDITOR_INSTANCE *)data;

	if(t3f_key_pressed(ALLEGRO_KEY_F3))
	{
		t3f_use_key_press(ALLEGRO_KEY_F3);
	}
	if(t3f_key_pressed(ALLEGRO_KEY_F4))
	{
		t3f_use_key_press(ALLEGRO_KEY_F4);
	}
	if(level_editor->level)
	{
		if(t3f_key_pressed(ALLEGRO_KEY_F2))
		{
			t3f_use_key_press(ALLEGRO_KEY_F2);
		}
		if(t3f_key_pressed(ALLEGRO_KEY_F6))
		{
			t3f_use_key_press(ALLEGRO_KEY_F6);
		}
		if(t3f_key_pressed(ALLEGRO_KEY_F7))
		{
			if(t3f_key_held(ALLEGRO_KEY_LCTRL))
			{
				if(level_editor->level->bg)
				{
					t3f_destroy_animation(level_editor->level->bg);
					level_editor->level->bg = NULL;
				}
			}
			else
			{
				level_editor->file_load_dialog = al_create_native_file_dialog(level_editor->path, "Load BG Image", "*.png;*.pcx;*.tga;*.bmp;*.jpg", 0);
				al_show_native_file_dialog(t3f_display, level_editor->file_load_dialog);
				if(al_get_native_file_dialog_count(level_editor->file_load_dialog) > 0)
				{
					level_editor->path = al_get_native_file_dialog_path(level_editor->file_load_dialog, 0);
					level_editor->level->bg = t3f_load_animation(level_editor->path, 0, false);
				}
				al_destroy_native_file_dialog(level_editor->file_load_dialog);
			}
			t3f_use_key_press(ALLEGRO_KEY_F7);
		}
		if(t3f_key_pressed(ALLEGRO_KEY_F8))
		{
			if(t3f_key_held(ALLEGRO_KEY_LCTRL))
			{
				if(level_editor->level->fg)
				{
					t3f_destroy_animation(level_editor->level->fg);
					level_editor->level->bg = NULL;
				}
			}
			else
			{
				level_editor->file_load_dialog = al_create_native_file_dialog(level_editor->path, "Load FG Image", "*.png;*.pcx;*.tga;*.bmp", 0);
				al_show_native_file_dialog(t3f_display, level_editor->file_load_dialog);
				if(al_get_native_file_dialog_count(level_editor->file_load_dialog) > 0)
				{
					level_editor->path = al_get_native_file_dialog_path(level_editor->file_load_dialog, 0);
					level_editor->level->fg = t3f_load_animation(level_editor->path, 0, false);
				}
				al_destroy_native_file_dialog(level_editor->file_load_dialog);
			}
			t3f_use_key_press(ALLEGRO_KEY_F8);
		}
		if(t3f_key_pressed(ALLEGRO_KEY_9))
		{
			t3f_use_key_press(ALLEGRO_KEY_9);
		}
		if(t3f_key_pressed(ALLEGRO_KEY_F10) || t3f_key_pressed(ALLEGRO_KEY_M))
		{
			t3f_use_key_press(ALLEGRO_KEY_F10);
			t3f_use_key_press(ALLEGRO_KEY_M);
		}
		if(t3f_key_pressed(ALLEGRO_KEY_TAB))
		{
			if(level_editor->view == LEVEDIT_VIEW_LEVEL)
			{
				t3f_clear_key_states();
				levedit_get_entry_pos(level_editor);
				level_editor->view = LEVEDIT_VIEW_META;
			}
			else
			{
				level_editor->view = LEVEDIT_VIEW_LEVEL;
			}
			t3f_use_key_press(ALLEGRO_KEY_TAB);
		}
		switch(level_editor->view)
		{
			case LEVEDIT_VIEW_LEVEL:
			{
				if(level_editor->level->tileset && level_editor->level->tilemap)
				{
					levedit_level_logic(level_editor);
				}
				break;
			}
			case LEVEDIT_VIEW_META:
			{
				levedit_meta_logic(level_editor);
				break;
			}
		}
		level_editor->tick++;
	}
}

void levedit_render_collision_tile(PP2_LEVEL_EDITOR_INSTANCE * level_editor, int flags, float x, float y)
{
	int si, sj;
	int tw, th, tw2, th2, tw4, th4;
	ALLEGRO_COLOR red = al_map_rgba_f(1.0, 0.0, 0.0, 0.5);
	ALLEGRO_COLOR yellow = al_map_rgba_f(1.0, 1.0, 0.0, 0.5);
	ALLEGRO_COLOR blue = al_map_rgba_f(0.0, 0.0, 1.0, 0.5);
	ALLEGRO_COLOR light_blue = al_map_rgba_f(0.5, 0.5, 1.0, 0.5);

	tw = level_editor->level->collision_tilemap[level_editor->selected_layer]->tile_width;
	th = level_editor->level->collision_tilemap[level_editor->selected_layer]->tile_height;
	tw2 = tw >> 1;
	th2 = th >> 1;
	tw4 = tw >> 2;
	th4 = th >> 2;
	si = (int)level_editor->cy / tw;
	sj = (int)level_editor->cx / th;
	if(si < 0)
	{
		si = 0;
	}
	if(sj < 0)
	{
		sj = 0;
	}
	if(flags & T3F_COLLISION_FLAG_SOLID_TOP)
	{
		if(flags & PP2_LEVEL_COLLISION_FLAG_SECRET)
		{
			al_draw_line(x + 0.5, y + 0.5, x + tw - 1 + 0.5, y + 0.5, yellow, 1.0);
			al_draw_line(x + tw2 + 0.5, y + 0.5, x + tw2 + 0.5, y + th4 + 0.5, yellow, 1.0);
		}
		else
		{
			al_draw_line(x + 0.5, y + 0.5, x + tw - 1 + 0.5, y + 0.5, red, 1.0);
			al_draw_line(x + tw2 + 0.5, y + 0.5, x + tw2 + 0.5, y + th4 + 0.5, red, 1.0);
		}
	}
	if(flags & T3F_COLLISION_FLAG_SOLID_BOTTOM)
	{
		al_draw_line(x + 0.5, y + th - 1 + 0.5, x + tw - 1 + 0.5, y + th - 1 + 0.5, red, 1.0);
		al_draw_line(x + tw2 + 0.5, y + th - 1 + 0.5, x + tw2 + 0.5, y + th - 1 - th4 + 0.5, red, 1.0);
	}
	if(flags & T3F_COLLISION_FLAG_SOLID_LEFT)
	{
		al_draw_line(x + 0.5, y + 0.5, x + 0.5, y + th - 1 + 0.5, red, 1.0);
		al_draw_line(x + 0.5, y + th2 + 0.5, x + tw4 + 0.5, y + th2 + 0.5, red, 1.0);
	}
	if(flags & T3F_COLLISION_FLAG_SOLID_RIGHT)
	{
		al_draw_line(x + tw - 1 + 0.5, y + 0.5, x + tw - 1 + 0.5, y + th - 1 + 0.5, red, 1.0);
		al_draw_line(x + tw - 1 + 0.5, y + th2 - 1 + 0.5, x + tw - 1 - tw4 + 0.5, y + th2 - 1 + 0.5, red, 1.0);
	}
	if(flags & PP2_LEVEL_COLLISION_FLAG_SECRET)
	{
		al_draw_line(x + 0.5, y + 0.5, x + tw - 1 + 0.5, y + th - 1 + 0.5, blue, 1.0);
	}
	if(flags & PP2_LEVEL_COLLISION_FLAG_ICE)
	{
		al_draw_line(x + 0.5, y + 1.0 + 0.5, x + tw - 1 + 0.5, y + 1.0 + 0.5, light_blue, 1.0);
	}
}

void levedit_render_collision_tilemap(PP2_LEVEL_EDITOR_INSTANCE * level_editor)
{
	int i, j, si, sj;
	int tw, th, tw2, th2, tw4, th4;

	al_hold_bitmap_drawing(false);
	tw = level_editor->level->collision_tilemap[level_editor->selected_layer]->tile_width;
	th = level_editor->level->collision_tilemap[level_editor->selected_layer]->tile_height;
	tw2 = tw >> 1;
	th2 = th >> 1;
	tw4 = tw >> 2;
	th4 = th >> 2;
	si = (int)level_editor->cy / tw;
	sj = (int)level_editor->cx / th;
	if(si < 0)
	{
		si = 0;
	}
	if(sj < 0)
	{
		sj = 0;
	}
	for(i = si; i < si + t3f_virtual_display_height / th + 1 && i < level_editor->level->collision_tilemap[level_editor->selected_layer]->height; i++)
	{
		for(j = sj; j < sj + t3f_virtual_display_width / tw + 1 && j < level_editor->level->collision_tilemap[level_editor->selected_layer]->width; j++)
		{
			levedit_render_collision_tile(level_editor, level_editor->level->collision_tilemap[level_editor->selected_layer]->data[i][j].flags, j * tw - level_editor->cx, i * th - level_editor->cy);
		}
	}
	if(!level_editor->tilemap_hover_fail)
	{
		al_draw_rectangle(level_editor->tilemap_hover_x * level_editor->level->tileset->width + 0.5 - level_editor->cx, level_editor->tilemap_hover_y * level_editor->level->tileset->height + 0.5 - level_editor->cy, level_editor->tilemap_hover_x * level_editor->level->tileset->width + level_editor->level->tileset->width - 1 + 0.5 - level_editor->cx, level_editor->tilemap_hover_y * level_editor->level->tileset->height + level_editor->level->tileset->height - 1 + 0.5 - level_editor->cy, al_map_rgba_f(0.0, 1.0, 0.0, 0.5), 1.0);
	}
}

void levedit_level_render(PP2_LEVEL_EDITOR_INSTANCE * level_editor)
{
	int i;
	float tx, ty;

	if(level_editor->level)
	{
		if(!level_editor->level->tileset)
		{
			al_clear_to_color(al_map_rgb(0, 0, 0));
			t3f_draw_textf(level_editor->font, t3f_color_white, 0.0, 0.0, 0, 0, "No tileset loaded...");
		}
		else if(!level_editor->level->tilemap)
		{
			al_clear_to_color(al_map_rgb(0, 0, 0));
			t3f_draw_textf(level_editor->font, t3f_color_white, 0.0, 0.0, 0, 0, "No tilemap loaded...");
		}
		else
		{
			al_hold_bitmap_drawing(true);
			if(level_editor->level->bg)
			{
				t3f_draw_animation(level_editor->level->bg, t3f_color_white, level_editor->tick, 0, 0, 0, 0);
			}
			for(i = 0; i <= level_editor->selected_layer; i++)
			{
				t3f_render_tilemap(level_editor->level->tilemap, level_editor->level->tileset, i, level_editor->tick, level_editor->cx, level_editor->cy, level_editor->cz, t3f_color_white);
			}
			for(i = 0; i < level_editor->level->objects; i++)
			{
				if(level_editor->level->object[i].type >= 0 && level_editor->object_animation[level_editor->level->object[i].type])
				{
					t3f_draw_animation(level_editor->object_animation[level_editor->level->object[i].type], t3f_color_white, level_editor->tick, level_editor->level->object[i].x - level_editor->cx, level_editor->level->object[i].y - level_editor->cy, level_editor->level->tilemap->layer[level_editor->level->object[i].layer]->z - level_editor->cz, 0);
				}
			}
			if(!t3f_key_held(ALLEGRO_KEY_L))
			{
				for(i = level_editor->selected_layer + 1; i < level_editor->level->tilemap->layers; i++)
				{
					t3f_render_tilemap(level_editor->level->tilemap, level_editor->level->tileset, i, level_editor->tick, level_editor->cx, level_editor->cy, level_editor->cz, t3f_color_white);
				}
				if(level_editor->level->fg)
				{
					t3f_draw_animation(level_editor->level->fg, t3f_color_white, level_editor->tick, 0, 0, 0, 0);
				}
			}
			al_hold_bitmap_drawing(false);
			if(level_editor->level->collision_tilemap[level_editor->selected_layer])
			{
				levedit_render_collision_tilemap(level_editor);
			}
			al_draw_rectangle(level_editor->level->room.x * level_editor->level->tileset->width - level_editor->cx + 0.5,
							  level_editor->level->room.y * level_editor->level->tileset->height - level_editor->cy + 0.5,
							  level_editor->level->room.bx * level_editor->level->tileset->width + level_editor->level->tileset->width - 1.0 - level_editor->cx + 0.5,
							  level_editor->level->room.by * level_editor->level->tileset->height + level_editor->level->tileset->height - 1.0 - level_editor->cy + 0.5,
							  al_map_rgba_f(0.0, 1.0, 0.0, 1.0), 1.0);
			if(level_editor->hover_object >= 0 && level_editor->hover_object < level_editor->level->objects)
			{
				al_draw_rectangle(t3f_project_x(level_editor->level->object[level_editor->hover_object].x - level_editor->cx, level_editor->level->tilemap->layer[level_editor->level->object[level_editor->hover_object].layer]->z - level_editor->cz),
								  t3f_project_y(level_editor->level->object[level_editor->hover_object].y - level_editor->cy, level_editor->level->tilemap->layer[level_editor->level->object[level_editor->hover_object].layer]->z - level_editor->cz),
								  t3f_project_x(level_editor->level->object[level_editor->hover_object].x + level_editor->object_animation[level_editor->level->object[level_editor->hover_object].type]->data->frame[0]->width - level_editor->cx, level_editor->level->tilemap->layer[level_editor->level->object[level_editor->hover_object].layer]->z - level_editor->cz),
								  t3f_project_y(level_editor->level->object[level_editor->hover_object].y + level_editor->object_animation[level_editor->level->object[level_editor->hover_object].type]->data->frame[0]->height - level_editor->cy, level_editor->level->tilemap->layer[level_editor->level->object[level_editor->hover_object].layer]->z - level_editor->cz),
								  al_map_rgba_f(0.0, 0.5, 0.0, 0.5), 1.0);
			}
			else
			{
				if(level_editor->object_animation[level_editor->object_type])
				{
					t3f_draw_animation(level_editor->object_animation[level_editor->object_type], al_map_rgba_f(0.5, 0.5, 0.5, 0.5), level_editor->tick, level_editor->place_x - level_editor->cx, level_editor->place_y - level_editor->cy, level_editor->level->tilemap->layer[level_editor->selected_layer]->z - level_editor->cz, 0);
				}
			}
			if(level_editor->selected_object >= 0 && level_editor->selected_object < level_editor->level->objects && level_editor->object_animation[level_editor->level->object[level_editor->selected_object].type])
			{
				al_draw_rectangle(t3f_project_x(level_editor->level->object[level_editor->selected_object].x - level_editor->cx, level_editor->level->tilemap->layer[level_editor->level->object[level_editor->selected_object].layer]->z - level_editor->cz),
								  t3f_project_y(level_editor->level->object[level_editor->selected_object].y - level_editor->cy, level_editor->level->tilemap->layer[level_editor->level->object[level_editor->selected_object].layer]->z - level_editor->cz),
								  t3f_project_x(level_editor->level->object[level_editor->selected_object].x + level_editor->object_animation[level_editor->level->object[level_editor->selected_object].type]->data->frame[0]->width - level_editor->cx, level_editor->level->tilemap->layer[level_editor->level->object[level_editor->selected_object].layer]->z - level_editor->cz),
								  t3f_project_y(level_editor->level->object[level_editor->selected_object].y + level_editor->object_animation[level_editor->level->object[level_editor->selected_object].type]->data->frame[0]->height - level_editor->cy, level_editor->level->tilemap->layer[level_editor->level->object[level_editor->selected_object].layer]->z - level_editor->cz),
								  al_map_rgba_f(0.0, 0.5, 0.5, 0.5), 1.0);
			}
			if(level_editor->hover_tile >= 0)
			{
				t3f_draw_animation(level_editor->level->tileset->tile[level_editor->hover_tile]->ap, al_map_rgba_f(0.5, 0.5, 0.5, 0.5), level_editor->tick, 640 - 32, 480 - 32, 0, 0);
			}
			tx = 640 - 32 - 32;
			ty = 480 - 32;
			t3f_draw_animation(level_editor->level->tileset->tile[level_editor->selected_tile]->ap, al_map_rgba_f(1.0, 1.0, 1.0, 1.0), level_editor->tick, tx, ty, 0, 0);
			if(level_editor->level->collision_tilemap[level_editor->selected_layer])
			{
				levedit_render_collision_tile(level_editor, level_editor->level->tileset->tile[level_editor->selected_tile]->user_data[15], tx, ty);
			}
			t3f_draw_textf(level_editor->font, t3f_color_white, 0.0, 0.0, 0, 0, "(%4.1f, %4.1f, %4.1f) Layer: %d", level_editor->cx, level_editor->cy, level_editor->cz, level_editor->selected_layer);
		}
	}
	else
	{
		al_clear_to_color(al_map_rgb(0, 0, 0));
		t3f_draw_textf(level_editor->font, t3f_color_white, 0.0, 0.0, 0, 0, "No level loaded...");
	}
}

void levedit_meta_render(PP2_LEVEL_EDITOR_INSTANCE * level_editor)
{
	al_clear_to_color(al_map_rgb_f(0.0, 0.0, 0.5));
	switch(level_editor->selected_meta)
	{
		case 0:
		{
			t3f_draw_textf(level_editor->font, t3f_color_white, 0, 0, 0, 0, "Name: %s", level_editor->level->info.name);
			break;
		}
		case 1:
		{
			t3f_draw_textf(level_editor->font, t3f_color_white, 0, 0, 0, 0, "Author: %s", level_editor->level->info.author);
			break;
		}
		case 2:
		{
			t3f_draw_textf(level_editor->font, t3f_color_white, 0, 0, 0, 0, "Comment: %s", level_editor->level->info.comment);
			break;
		}
	}
}

void levedit_render(void * data)
{
  PP2_LEVEL_EDITOR_INSTANCE * level_editor = (PP2_LEVEL_EDITOR_INSTANCE *)data;

  switch(level_editor->view)
	{
		case LEVEDIT_VIEW_LEVEL:
		{
			levedit_level_render(level_editor);
			break;
		}
		case LEVEDIT_VIEW_META:
		{
			levedit_meta_render(level_editor);
			break;
		}
	}
}
