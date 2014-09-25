#include <allegro5/allegro5.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_font.h>
#include "t3f/t3f.h"
#include "file/level.h"
#include "legacy/animation.h"
#include "legacy/level.h"
#include "gameplay/sprites/objects.h"

#define LEVEDIT_VIEW_LEVEL 0
#define LEVEDIT_VIEW_META  1

ALLEGRO_FILECHOOSER * levedit_file_load_dialog = NULL;
ALLEGRO_FILECHOOSER * levedit_file_save_dialog = NULL;
ALLEGRO_FILECHOOSER * levedit_file_load_ani_dialog = NULL;
const char * levedit_path = NULL;

ALLEGRO_FONT * levedit_font = NULL;
char levedit_temp_string[1024] = {0};

PP2_LEVEL * levedit_level = NULL;
T3F_ANIMATION * levedit_object_animation[256] = {NULL};
T3F_ATLAS * levedit_object_atlas = NULL;
float levedit_cx = 0.0;
float levedit_cy = 0.0;
float levedit_cz = 0.0;
int levedit_view = LEVEDIT_VIEW_LEVEL;
int levedit_tick = 0;
int levedit_hover_object = -1;
int levedit_selected_object = -1;
int levedit_hover_tile = -1;
int levedit_selected_tile = 0;
int levedit_object_type = 0;
int levedit_selected_layer = 0;
int levedit_place_x, levedit_place_y;
bool levedit_clicked = false;
int levedit_tilemap_hover_x = 0;
int levedit_tilemap_hover_y = 0;
bool levedit_tilemap_hover_fail = false;
char * levedit_entry_pointer = NULL;
int levedit_entry_pos = 0;
int levedit_selected_meta = 0;

const char * levedit_get_extension(const char * fn)
{
	ALLEGRO_PATH * path;
	
	path = al_create_path(fn);
	if(path)
	{
		strcpy(levedit_temp_string, al_get_path_extension(path));
		al_destroy_path(path);
	}
	else
	{
		strcpy(levedit_temp_string, "");
	}
	return levedit_temp_string;
}

void levedit_get_entry_pos(void)
{
	switch(levedit_selected_meta)
	{
		case 0:
		{
			levedit_entry_pos = strlen(levedit_level->info.name);
			levedit_entry_pointer = levedit_level->info.name;
			break;
		}
		case 1:
		{
			levedit_entry_pos = strlen(levedit_level->info.author);
			levedit_entry_pointer = levedit_level->info.author;
			break;
		}
		case 2:
		{
			levedit_entry_pos = strlen(levedit_level->info.comment);
			levedit_entry_pointer = levedit_level->info.comment;
			break;
		}
	}
}

void levedit_add_object(int type, int x, int y, int flags)
{
	if(levedit_level->objects < 256)
	{
		levedit_level->object[levedit_level->objects].type = type;
		levedit_level->object[levedit_level->objects].x = x;
		levedit_level->object[levedit_level->objects].y = y;
		levedit_level->object[levedit_level->objects].layer = levedit_selected_layer;
		levedit_level->object[levedit_level->objects].flags = flags;
		levedit_selected_object = levedit_level->objects;
		levedit_level->objects++;
	}
}

void levedit_previous_object(void)
{
	while(1)
	{
		levedit_object_type--;
		if(levedit_object_type < 0)
		{
			levedit_object_type = 255;
		}
		if(levedit_object_animation[levedit_object_type])
		{
			break;
		}
	}
}

void levedit_next_object(void)
{
	while(1)
	{
		levedit_object_type++;
		if(levedit_object_type > 255)
		{
			levedit_object_type = 0;
		}
		if(levedit_object_animation[levedit_object_type])
		{
			break;
		}
	}
}

void levedit_delete_object(void)
{
	int i;
	
	if(levedit_selected_object >= 0 && levedit_selected_object < levedit_level->objects)
	{
		for(i = levedit_selected_object; i < levedit_level->objects - 1; i++)
		{
			memcpy(&levedit_level->object[i], &levedit_level->object[i + 1], sizeof(PP2_LEVEL_OBJECT));
		}
		levedit_level->objects--;
		if(levedit_selected_object >= levedit_level->objects)
		{
			levedit_selected_object = 0;
		}
	}
}

void levedit_move_logic(void)
{
	if(t3f_key[ALLEGRO_KEY_LEFT])
	{
		if(t3f_key[ALLEGRO_KEY_LSHIFT])
		{
			levedit_cx -= levedit_level->tileset->width;
			levedit_cx = (int)(levedit_cx / levedit_level->tileset->width) * levedit_level->tileset->width;
			t3f_key[ALLEGRO_KEY_LEFT] = 0;
		}
		else
		{
			levedit_cx -= 1.0;
		}
	}
	if(t3f_key[ALLEGRO_KEY_RIGHT])
	{
		if(t3f_key[ALLEGRO_KEY_LSHIFT])
		{
			levedit_cx += levedit_level->tileset->width;
			levedit_cx = (int)(levedit_cx / levedit_level->tileset->width) * levedit_level->tileset->width;
			t3f_key[ALLEGRO_KEY_RIGHT] = 0;
		}
		else
		{
			levedit_cx += 1.0;
		}
	}
	if(t3f_key[ALLEGRO_KEY_UP])
	{
		if(t3f_key[ALLEGRO_KEY_LSHIFT])
		{
			levedit_cy -= levedit_level->tileset->height;
			levedit_cy = (int)(levedit_cy / levedit_level->tileset->height) * levedit_level->tileset->height;
			t3f_key[ALLEGRO_KEY_UP] = 0;
		}
		else
		{
			levedit_cy -= 1.0;
		}
	}
	if(t3f_key[ALLEGRO_KEY_DOWN])
	{
		if(t3f_key[ALLEGRO_KEY_LSHIFT])
		{
			levedit_cy += levedit_level->tileset->height;
			levedit_cy = (int)(levedit_cy / levedit_level->tileset->height) * levedit_level->tileset->height;
			t3f_key[ALLEGRO_KEY_DOWN] = 0;
		}
		else
		{
			levedit_cy += 1.0;
		}
	}
	levedit_cz = levedit_level->tilemap->layer[levedit_selected_layer]->z;
}

void levedit_create_collision_tilemap_if_needed(void)
{
	if(!levedit_level->collision_tilemap[levedit_selected_layer])
	{
		levedit_level->collision_tilemap[levedit_selected_layer] = t3f_create_collision_tilemap(levedit_level->tilemap->layer[levedit_selected_layer]->width, levedit_level->tilemap->layer[levedit_selected_layer]->height, 32, 32);
	}
}

void levedit_level_logic(void)
{
	int mx, my, mz;
	int i;
	
	if(t3f_key[ALLEGRO_KEY_MINUS])
	{
		if(t3f_key[ALLEGRO_KEY_LCTRL])
		{
			levedit_selected_tile--;
			if(levedit_selected_tile < 0)
			{
				levedit_selected_tile = levedit_level->tileset->tiles - 1;
			}
		}
		else
		{
			levedit_previous_object();
		}
		t3f_key[ALLEGRO_KEY_MINUS] = 0;
	}
	if(t3f_key[ALLEGRO_KEY_EQUALS])
	{
		if(t3f_key[ALLEGRO_KEY_LCTRL])
		{
			levedit_selected_tile++;
			if(levedit_selected_tile >= levedit_level->tileset->tiles)
			{
				levedit_selected_tile = 0;
			}
		}
		else
		{
			levedit_next_object();
		}
		t3f_key[ALLEGRO_KEY_EQUALS] = 0;
	}
	if(t3f_key[ALLEGRO_KEY_DELETE])
	{
		levedit_delete_object();
		t3f_key[ALLEGRO_KEY_DELETE] = 0;
	}
	if(t3f_key[ALLEGRO_KEY_PGUP])
	{
		levedit_selected_layer++;
		if(levedit_selected_layer >= levedit_level->tilemap->layers)
		{
			levedit_selected_layer = 0;
		}
		t3f_key[ALLEGRO_KEY_PGUP] = 0;
	}
	if(t3f_key[ALLEGRO_KEY_PGDN])
	{
		levedit_selected_layer--;
		if(levedit_selected_layer < 0)
		{
			levedit_selected_layer = levedit_level->tilemap->layers - 1;
		}
		t3f_key[ALLEGRO_KEY_PGDN] = 0;
	}
	levedit_hover_object = -1;
	levedit_place_x = t3f_mouse_x - levedit_object_animation[levedit_object_type]->frame[0]->width / 2 + levedit_cx;
	levedit_place_y = t3f_mouse_y - levedit_object_animation[levedit_object_type]->frame[0]->height / 2 + levedit_cy;
	if(t3f_key[ALLEGRO_KEY_LCTRL])
	{
		levedit_place_x = (levedit_place_x / 16) * 16;
		levedit_place_y = (levedit_place_y / 16) * 16;
	}
	for(i = 0; i < levedit_level->objects; i++)
	{
		if(levedit_level->object[i].type >= 0 && levedit_level->object[i].type < 256 && levedit_level->object[i].layer == levedit_selected_layer && levedit_object_animation[levedit_level->object[i].type])
		{
			if(t3f_mouse_x + levedit_cx >= levedit_level->object[i].x && t3f_mouse_x + levedit_cx < levedit_level->object[i].x + levedit_object_animation[levedit_level->object[i].type]->frame[0]->width && t3f_mouse_y + levedit_cy >= levedit_level->object[i].y && t3f_mouse_y + levedit_cy < levedit_level->object[i].y + levedit_object_animation[levedit_level->object[i].type]->frame[0]->height)
			{
				levedit_hover_object = i;
				break;
			}
		}
	}
	t3f_get_mouse_mickeys(&mx, &my, &mz);
	levedit_hover_tile = -1;
	levedit_tilemap_hover_x = (int)(t3f_mouse_x + levedit_cx) / levedit_level->tileset->width;
	levedit_tilemap_hover_y = (int)(t3f_mouse_y + levedit_cy) / levedit_level->tileset->height;
	if(levedit_level->collision_tilemap[levedit_selected_layer])
	{
		if(levedit_tilemap_hover_x < 0 || levedit_tilemap_hover_x >= levedit_level->collision_tilemap[levedit_selected_layer]->width || levedit_tilemap_hover_y < 0 || levedit_tilemap_hover_y >= levedit_level->collision_tilemap[levedit_selected_layer]->height)
		{
			levedit_tilemap_hover_fail = true;
		}
		else
		{
			levedit_tilemap_hover_fail = false;
			levedit_hover_tile = levedit_level->tilemap->layer[levedit_selected_layer]->data[levedit_tilemap_hover_y][levedit_tilemap_hover_x];
		}
	}
	if(t3f_key[ALLEGRO_KEY_LCTRL])
	{
		if(t3f_key[ALLEGRO_KEY_W])
		{
			levedit_level->room.y = levedit_tilemap_hover_y;
			t3f_key[ALLEGRO_KEY_W] = 0;
		}
		if(t3f_key[ALLEGRO_KEY_A])
		{
			levedit_level->room.x = levedit_tilemap_hover_x;
			t3f_key[ALLEGRO_KEY_A] = 0;
		}
		if(t3f_key[ALLEGRO_KEY_S])
		{
			levedit_level->room.by = levedit_tilemap_hover_y;
			t3f_key[ALLEGRO_KEY_S] = 0;
		}
		if(t3f_key[ALLEGRO_KEY_D])
		{
			levedit_level->room.bx = levedit_tilemap_hover_x;
			t3f_key[ALLEGRO_KEY_D] = 0;
		}
	}
	else if(t3f_key[ALLEGRO_KEY_LSHIFT])
	{
		if(t3f_key[ALLEGRO_KEY_W])
		{
			levedit_level->tileset->tile[levedit_selected_tile]->user_data[15] ^= T3F_COLLISION_FLAG_SOLID_TOP;
			t3f_key[ALLEGRO_KEY_W] = 0;
		}
		if(t3f_key[ALLEGRO_KEY_S])
		{
			levedit_level->tileset->tile[levedit_selected_tile]->user_data[15] ^= T3F_COLLISION_FLAG_SOLID_BOTTOM;
			t3f_key[ALLEGRO_KEY_S] = 0;
		}
		if(t3f_key[ALLEGRO_KEY_A])
		{
			levedit_level->tileset->tile[levedit_selected_tile]->user_data[15] ^= T3F_COLLISION_FLAG_SOLID_LEFT;
			t3f_key[ALLEGRO_KEY_A] = 0;
		}
		if(t3f_key[ALLEGRO_KEY_D])
		{
			levedit_level->tileset->tile[levedit_selected_tile]->user_data[15] ^= T3F_COLLISION_FLAG_SOLID_RIGHT;
			t3f_key[ALLEGRO_KEY_D] = 0;
		}
		if(levedit_level->tileset->tile[levedit_selected_tile]->user_data[15])
		{
			levedit_level->tileset->tile[levedit_selected_tile]->flags |= T3F_TILE_FLAG_USER_DATA;
		}
		else
		{
			levedit_level->tileset->tile[levedit_selected_tile]->flags &= ~T3F_TILE_FLAG_USER_DATA;
		}
	}
	else
	{
		if(t3f_key[ALLEGRO_KEY_W])
		{
			levedit_create_collision_tilemap_if_needed();
			levedit_level->collision_tilemap[levedit_selected_layer]->data[levedit_tilemap_hover_y][levedit_tilemap_hover_x].flags ^= T3F_COLLISION_FLAG_SOLID_TOP;
			if((levedit_level->collision_tilemap[levedit_selected_layer]->data[levedit_tilemap_hover_y][levedit_tilemap_hover_x].flags & T3F_COLLISION_FLAG_SOLID_TOP) && t3f_key[ALLEGRO_KEY_ALT])
			{
				levedit_level->collision_tilemap[levedit_selected_layer]->data[levedit_tilemap_hover_y][levedit_tilemap_hover_x].flags |= T3F_COLLISION_FLAG_USER;
			}
			else
			{
				levedit_level->collision_tilemap[levedit_selected_layer]->data[levedit_tilemap_hover_y][levedit_tilemap_hover_x].flags &= ~T3F_COLLISION_FLAG_USER;
			}
			t3f_key[ALLEGRO_KEY_W] = 0;
		}
		if(t3f_key[ALLEGRO_KEY_S])
		{
			levedit_create_collision_tilemap_if_needed();
			levedit_level->collision_tilemap[levedit_selected_layer]->data[levedit_tilemap_hover_y][levedit_tilemap_hover_x].flags ^= T3F_COLLISION_FLAG_SOLID_BOTTOM;
			t3f_key[ALLEGRO_KEY_S] = 0;
		}
		if(t3f_key[ALLEGRO_KEY_A])
		{
			levedit_create_collision_tilemap_if_needed();
			levedit_level->collision_tilemap[levedit_selected_layer]->data[levedit_tilemap_hover_y][levedit_tilemap_hover_x].flags ^= T3F_COLLISION_FLAG_SOLID_LEFT;
			t3f_key[ALLEGRO_KEY_A] = 0;
		}
		if(t3f_key[ALLEGRO_KEY_D])
		{
			levedit_create_collision_tilemap_if_needed();
			levedit_level->collision_tilemap[levedit_selected_layer]->data[levedit_tilemap_hover_y][levedit_tilemap_hover_x].flags ^= T3F_COLLISION_FLAG_SOLID_RIGHT;
			t3f_key[ALLEGRO_KEY_D] = 0;
		}
		if(t3f_key[ALLEGRO_KEY_Q])
		{
			levedit_create_collision_tilemap_if_needed();
			levedit_level->collision_tilemap[levedit_selected_layer]->data[levedit_tilemap_hover_y][levedit_tilemap_hover_x].flags ^= T3F_COLLISION_FLAG_SOLID_RIGHT | T3F_COLLISION_FLAG_SOLID_LEFT | T3F_COLLISION_FLAG_SOLID_TOP | T3F_COLLISION_FLAG_SOLID_BOTTOM;
			t3f_key[ALLEGRO_KEY_Q] = 0;
		}
		if(t3f_key[ALLEGRO_KEY_E])
		{
			levedit_create_collision_tilemap_if_needed();
			levedit_level->collision_tilemap[levedit_selected_layer]->data[levedit_tilemap_hover_y][levedit_tilemap_hover_x].flags ^= PP2_LEVEL_COLLISION_FLAG_SECRET;
			t3f_key[ALLEGRO_KEY_E] = 0;
		}
	}
	if(t3f_key[ALLEGRO_KEY_T] && levedit_hover_tile >= 0)
	{
		levedit_selected_tile = levedit_hover_tile;
	}
	levedit_move_logic();
	if(levedit_hover_object >= 0)
	{
		if(t3f_mouse_button[0])
		{
			levedit_selected_object = levedit_hover_object;
			if(t3f_key[ALLEGRO_KEY_LCTRL])
			{
				levedit_level->object[levedit_hover_object].x = ((int)(t3f_mouse_x + levedit_cx) / 8) * 8 - levedit_object_animation[levedit_level->object[levedit_hover_object].type]->frame[0]->width / 2;
				levedit_level->object[levedit_hover_object].y = ((int)(t3f_mouse_y + levedit_cy) / 8) * 8 - levedit_object_animation[levedit_level->object[levedit_hover_object].type]->frame[0]->height / 2;
			}
			else
			{
				levedit_level->object[levedit_hover_object].x -= mx;
				levedit_level->object[levedit_hover_object].y -= my;
			}
		}
	}
	else
	{
		if(t3f_mouse_button[1])
		{
			if(!levedit_clicked)
			{
				levedit_add_object(levedit_object_type, levedit_place_x, levedit_place_y, 0);
				levedit_clicked = true;
			}
		}
		else
		{
			levedit_clicked = false;
		}
	}
	levedit_cz += mz * 16;
	
	/* fix camera */
	if(levedit_cx < 0)
	{
		levedit_cx = 0;
	}
	else if(levedit_cx >= 32 * levedit_level->tilemap->layer[0]->width - 640)
	{
		levedit_cx = 32 * levedit_level->tilemap->layer[0]->width - 640 - 1;
	}
	if(levedit_cy < 0)
	{
		levedit_cy = 0;
	}
	else if(levedit_cy >= 32 * levedit_level->tilemap->layer[0]->height - 480)
	{
		levedit_cy = 32 * levedit_level->tilemap->layer[0]->height - 480 - 1;
	}
}

void levedit_meta_logic(void)
{
	int input = 0;
	
	if(t3f_key[ALLEGRO_KEY_LEFT])
	{
		levedit_selected_meta--;
		if(levedit_selected_meta < 0)
		{
			levedit_selected_meta = 2;
		}
		levedit_get_entry_pos();
		t3f_key[ALLEGRO_KEY_LEFT] = 0;
	}
	if(t3f_key[ALLEGRO_KEY_RIGHT])
	{
		levedit_selected_meta++;
		if(levedit_selected_meta > 2)
		{
			levedit_selected_meta = 2;
		}
		levedit_get_entry_pos();
		t3f_key[ALLEGRO_KEY_RIGHT] = 0;
	}
	
	input = t3f_read_key(0);
	if(input)
	{
		if(input == '\b' || input == 127)
		{
			if(levedit_entry_pos > 0)
			{
				levedit_entry_pos--;
				levedit_entry_pointer[levedit_entry_pos] = '\0';
			}
		}
		else if(input >= 32 && input < 127)
		{
			levedit_entry_pointer[levedit_entry_pos] = input;
			levedit_entry_pos++;
			levedit_entry_pointer[levedit_entry_pos] = '\0';
		}
	}
}

void levedit_logic(void)
{
	ALLEGRO_PATH * temp_path = NULL;
	
	if(t3f_key[ALLEGRO_KEY_F3])
	{
		levedit_file_load_dialog = al_create_native_file_dialog(levedit_path, "Load Level", "*.ppl;*.p2c", 0);
		al_show_native_file_dialog(t3f_display, levedit_file_load_dialog);
		if(al_get_native_file_dialog_count(levedit_file_load_dialog) > 0)
		{
			levedit_path = al_get_native_file_dialog_path(levedit_file_load_dialog, 0);
			if(!strcasecmp(levedit_get_extension(levedit_path), ".p2l"))
			{
				if(t3f_key[ALLEGRO_KEY_LSHIFT])
				{
					levedit_level = pp2_load_old_level(levedit_path, 0);
				}
				else
				{
					levedit_level = pp2_load_level(levedit_path, 0);
				}
			}
			else
			{
				levedit_level = pp2_load_legacy_level(levedit_path, 0);
			}
		}
		al_destroy_native_file_dialog(levedit_file_load_dialog);
		t3f_key[ALLEGRO_KEY_F3] = 0;
	}
	if(t3f_key[ALLEGRO_KEY_F4])
	{
		if(levedit_level)
		{
			pp2_destroy_level(levedit_level);
			levedit_level = NULL;
		}
		levedit_level = pp2_create_level();
		t3f_key[ALLEGRO_KEY_F4] = 0;
	}
	if(levedit_level)
	{
		if(t3f_key[ALLEGRO_KEY_F2])
		{
			levedit_file_save_dialog = al_create_native_file_dialog(levedit_path, "Save Level", "*.ppl;*.p2c", ALLEGRO_FILECHOOSER_SAVE);
			al_show_native_file_dialog(t3f_display, levedit_file_save_dialog);
			if(al_get_native_file_dialog_count(levedit_file_save_dialog) > 0)
			{
				levedit_path = al_get_native_file_dialog_path(levedit_file_save_dialog, 0);
				temp_path = al_create_path(levedit_path);
				if(temp_path)
				{
					al_set_path_extension(temp_path, ".p2l");
					pp2_save_level(levedit_level, al_path_cstr(temp_path, '/'));
					al_destroy_path(temp_path);
				}
			}
			al_destroy_native_file_dialog(levedit_file_save_dialog);
			t3f_key[ALLEGRO_KEY_F2] = 0;
		}
		if(t3f_key[ALLEGRO_KEY_F6])
		{
			levedit_file_save_dialog = al_create_native_file_dialog(levedit_path, "Save Level Parts", "*.t3t;*.t3m", ALLEGRO_FILECHOOSER_SAVE);
			al_show_native_file_dialog(t3f_display, levedit_file_save_dialog);
			if(al_get_native_file_dialog_count(levedit_file_save_dialog) > 0)
			{
				levedit_path = al_get_native_file_dialog_path(levedit_file_save_dialog, 0);
				temp_path = al_create_path(levedit_path);
				if(temp_path)
				{
					al_set_path_extension(temp_path, ".t3t");
					t3f_save_tileset(levedit_level->tileset, al_path_cstr(temp_path, '/'));
					al_set_path_extension(temp_path, ".t3m");
					t3f_save_tilemap(levedit_level->tilemap, al_path_cstr(temp_path, '/'));
					al_destroy_path(temp_path);
				}
			}
			al_destroy_native_file_dialog(levedit_file_save_dialog);
			t3f_key[ALLEGRO_KEY_F6] = 0;
		}
		if(t3f_key[ALLEGRO_KEY_F7])
		{
			if(t3f_key[ALLEGRO_KEY_LCTRL])
			{
				if(levedit_level->bg)
				{
					t3f_destroy_animation(levedit_level->bg);
					levedit_level->bg = NULL;
				}
			}
			else
			{
				levedit_file_load_dialog = al_create_native_file_dialog(levedit_path, "Load BG Image", "*.png;*.pcx;*.tga;*.bmp;*.jpg", 0);
				al_show_native_file_dialog(t3f_display, levedit_file_load_dialog);
				if(al_get_native_file_dialog_count(levedit_file_load_dialog) > 0)
				{
					levedit_path = al_get_native_file_dialog_path(levedit_file_load_dialog, 0);
					if(!strcasecmp(levedit_get_extension(levedit_path), ".t3a"))
					{
						levedit_level->bg = t3f_load_animation(levedit_path);
					}
					else
					{
						levedit_level->bg = t3f_load_animation_from_bitmap(levedit_path);
					}
				}
				al_destroy_native_file_dialog(levedit_file_load_dialog);
			}
			t3f_key[ALLEGRO_KEY_F7] = 0;
		}
		if(t3f_key[ALLEGRO_KEY_F8])
		{
			if(t3f_key[ALLEGRO_KEY_LCTRL])
			{
				if(levedit_level->fg)
				{
					t3f_destroy_animation(levedit_level->fg);
					levedit_level->bg = NULL;
				}
			}
			else
			{
				levedit_file_load_dialog = al_create_native_file_dialog(levedit_path, "Load FG Image", "*.png;*.pcx;*.tga;*.bmp", 0);
				al_show_native_file_dialog(t3f_display, levedit_file_load_dialog);
				if(al_get_native_file_dialog_count(levedit_file_load_dialog) > 0)
				{
					levedit_path = al_get_native_file_dialog_path(levedit_file_load_dialog, 0);
					if(!strcasecmp(levedit_get_extension(levedit_path), ".t3a"))
					{
						levedit_level->fg = t3f_load_animation(levedit_path);
					}
					else
					{
						levedit_level->fg = t3f_load_animation_from_bitmap(levedit_path);
					}
				}
				al_destroy_native_file_dialog(levedit_file_load_dialog);
			}
			t3f_key[ALLEGRO_KEY_F8] = 0;
		}
		if(t3f_key[ALLEGRO_KEY_9])
		{
			levedit_file_load_dialog = al_create_native_file_dialog(levedit_path, "Load Tileset", "*.t3t", 0);
			al_show_native_file_dialog(t3f_display, levedit_file_load_dialog);
			if(al_get_native_file_dialog_count(levedit_file_load_dialog) > 0)
			{
				levedit_path = al_get_native_file_dialog_path(levedit_file_load_dialog, 0);
				if(levedit_level->tileset)
				{
					t3f_destroy_tileset(levedit_level->tileset);
				}
				levedit_level->tileset = t3f_load_tileset(levedit_path);
			}
			al_destroy_native_file_dialog(levedit_file_load_dialog);
			t3f_key[ALLEGRO_KEY_9] = 0;
		}
		if(t3f_key[ALLEGRO_KEY_F10] || t3f_key[ALLEGRO_KEY_M])
		{
			al_stop_timer(t3f_timer);
			levedit_file_load_dialog = al_create_native_file_dialog(levedit_path, "Load Tilemap", "*.t3m", 0);
			al_show_native_file_dialog(t3f_display, levedit_file_load_dialog);
			if(al_get_native_file_dialog_count(levedit_file_load_dialog) > 0)
			{
				levedit_path = al_get_native_file_dialog_path(levedit_file_load_dialog, 0);
				if(levedit_level->tilemap)
				{
					t3f_destroy_tilemap(levedit_level->tilemap);
				}
				levedit_level->tilemap = t3f_load_tilemap(levedit_path);
			}
			al_destroy_native_file_dialog(levedit_file_load_dialog);
			al_start_timer(t3f_timer);
			t3f_key[ALLEGRO_KEY_F10] = 0;
			t3f_key[ALLEGRO_KEY_M] = 0;
		}
		if(t3f_key[ALLEGRO_KEY_TAB])
		{
			if(levedit_view == LEVEDIT_VIEW_LEVEL)
			{
				levedit_get_entry_pos();
				levedit_view = LEVEDIT_VIEW_META;
			}
			else
			{
				levedit_view = LEVEDIT_VIEW_LEVEL;
			}
			t3f_key[ALLEGRO_KEY_TAB] = 0;
		}
		switch(levedit_view)
		{
			case LEVEDIT_VIEW_LEVEL:
			{
				if(levedit_level->tileset && levedit_level->tilemap)
				{
					levedit_level_logic();
				}
				break;
			}
			case LEVEDIT_VIEW_META:
			{
				levedit_meta_logic();
				break;
			}
		}
		levedit_tick++;
	}
}

void levedit_render_collision_tile(int flags, float x, float y)
{
	int si, sj;
	int tw, th, tw2, th2, tw4, th4;
	ALLEGRO_COLOR red = al_map_rgba_f(1.0, 0.0, 0.0, 0.5);
	ALLEGRO_COLOR yellow = al_map_rgba_f(1.0, 1.0, 0.0, 0.5);
	ALLEGRO_COLOR blue = al_map_rgba_f(0.0, 0.0, 1.0, 0.5);
	
	tw = levedit_level->collision_tilemap[levedit_selected_layer]->tile_width;
	th = levedit_level->collision_tilemap[levedit_selected_layer]->tile_height;
	tw2 = tw >> 1;
	th2 = th >> 1;
	tw4 = tw >> 2;
	th4 = th >> 2;
	si = (int)levedit_cy / tw;
	sj = (int)levedit_cx / th;
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
		if(flags & T3F_COLLISION_FLAG_USER)
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
}

void levedit_render_collision_tilemap(void)
{
	int i, j, si, sj;
	int tw, th, tw2, th2, tw4, th4;
	
	al_hold_bitmap_drawing(false);
	tw = levedit_level->collision_tilemap[levedit_selected_layer]->tile_width;
	th = levedit_level->collision_tilemap[levedit_selected_layer]->tile_height;
	tw2 = tw >> 1;
	th2 = th >> 1;
	tw4 = tw >> 2;
	th4 = th >> 2;
	si = (int)levedit_cy / tw;
	sj = (int)levedit_cx / th;
	if(si < 0)
	{
		si = 0;
	}
	if(sj < 0)
	{
		sj = 0;
	}
	for(i = si; i < si + t3f_virtual_display_height / th + 1 && i < levedit_level->collision_tilemap[levedit_selected_layer]->height; i++)
	{
		for(j = sj; j < sj + t3f_virtual_display_width / tw + 1 && j < levedit_level->collision_tilemap[levedit_selected_layer]->width; j++)
		{
			levedit_render_collision_tile(levedit_level->collision_tilemap[levedit_selected_layer]->data[i][j].flags, j * tw - levedit_cx, i * th - levedit_cy);
		}
	}
	if(!levedit_tilemap_hover_fail)
	{
		al_draw_rectangle(levedit_tilemap_hover_x * levedit_level->tileset->width + 0.5 - levedit_cx, levedit_tilemap_hover_y * levedit_level->tileset->height + 0.5 - levedit_cy, levedit_tilemap_hover_x * levedit_level->tileset->width + levedit_level->tileset->width - 1 + 0.5 - levedit_cx, levedit_tilemap_hover_y * levedit_level->tileset->height + levedit_level->tileset->height - 1 + 0.5 - levedit_cy, al_map_rgba_f(0.0, 1.0, 0.0, 0.5), 1.0);
	}
}

void levedit_level_render(void)
{
	int i;
	float tx, ty;
	
	if(levedit_level)
	{
		if(!levedit_level->tileset)
		{
			al_clear_to_color(al_map_rgb(0, 0, 0));
			al_draw_textf(levedit_font, t3f_color_white, 0.0, 0.0, 0, "No tileset loaded...");
		}
		else if(!levedit_level->tilemap)
		{
			al_clear_to_color(al_map_rgb(0, 0, 0));
			al_draw_textf(levedit_font, t3f_color_white, 0.0, 0.0, 0, "No tilemap loaded...");
		}
		else
		{
			al_hold_bitmap_drawing(true);
			if(levedit_level->bg)
			{
				t3f_draw_animation(levedit_level->bg, t3f_color_white, levedit_tick, 0, 0, 0, 0);
			}
			for(i = 0; i <= levedit_selected_layer; i++)
			{
				t3f_render_tilemap(levedit_level->tilemap, levedit_level->tileset, i, levedit_tick, levedit_cx, levedit_cy, levedit_cz, t3f_color_white);
			}
			for(i = 0; i < levedit_level->objects; i++)
			{
				if(levedit_level->object[i].type >= 0 && levedit_object_animation[levedit_level->object[i].type])
				{
					t3f_draw_animation(levedit_object_animation[levedit_level->object[i].type], t3f_color_white, levedit_tick, levedit_level->object[i].x - levedit_cx, levedit_level->object[i].y - levedit_cy, levedit_level->tilemap->layer[levedit_level->object[i].layer]->z - levedit_cz, 0);
				}
			}
			if(!t3f_key[ALLEGRO_KEY_L])
			{
				for(i = levedit_selected_layer + 1; i < levedit_level->tilemap->layers; i++)
				{
					t3f_render_tilemap(levedit_level->tilemap, levedit_level->tileset, i, levedit_tick, levedit_cx, levedit_cy, levedit_cz, t3f_color_white);
				}
				if(levedit_level->fg)
				{
					t3f_draw_animation(levedit_level->fg, t3f_color_white, levedit_tick, 0, 0, 0, 0);
				}
			}
			al_hold_bitmap_drawing(false);
			if(levedit_level->collision_tilemap[levedit_selected_layer])
			{
				levedit_render_collision_tilemap();
			}
			al_draw_rectangle(levedit_level->room.x * levedit_level->tileset->width - levedit_cx + 0.5,
							  levedit_level->room.y * levedit_level->tileset->height - levedit_cy + 0.5,
							  levedit_level->room.bx * levedit_level->tileset->width + levedit_level->tileset->width - 1.0 - levedit_cx + 0.5,
							  levedit_level->room.by * levedit_level->tileset->height + levedit_level->tileset->height - 1.0 - levedit_cy + 0.5,
							  al_map_rgba_f(0.0, 1.0, 0.0, 1.0), 1.0);
			if(levedit_hover_object >= 0 && levedit_hover_object < levedit_level->objects)
			{
				al_draw_rectangle(t3f_project_x(levedit_level->object[levedit_hover_object].x - levedit_cx, levedit_level->tilemap->layer[levedit_level->object[levedit_hover_object].layer]->z - levedit_cz),
								  t3f_project_y(levedit_level->object[levedit_hover_object].y - levedit_cy, levedit_level->tilemap->layer[levedit_level->object[levedit_hover_object].layer]->z - levedit_cz),
								  t3f_project_x(levedit_level->object[levedit_hover_object].x + levedit_object_animation[levedit_level->object[levedit_hover_object].type]->frame[0]->width - levedit_cx, levedit_level->tilemap->layer[levedit_level->object[levedit_hover_object].layer]->z - levedit_cz),
								  t3f_project_y(levedit_level->object[levedit_hover_object].y + levedit_object_animation[levedit_level->object[levedit_hover_object].type]->frame[0]->height - levedit_cy, levedit_level->tilemap->layer[levedit_level->object[levedit_hover_object].layer]->z - levedit_cz),
								  al_map_rgba_f(0.0, 0.5, 0.0, 0.5), 1.0);
			}
			else
			{
				if(levedit_object_animation[levedit_object_type])
				{
					t3f_draw_animation(levedit_object_animation[levedit_object_type], al_map_rgba_f(0.5, 0.5, 0.5, 0.5), levedit_tick, levedit_place_x - levedit_cx, levedit_place_y - levedit_cy, levedit_level->tilemap->layer[levedit_selected_layer]->z - levedit_cz, 0);
				}
			}
			if(levedit_selected_object >= 0 && levedit_selected_object < levedit_level->objects && levedit_object_animation[levedit_level->object[levedit_selected_object].type])
			{
				al_draw_rectangle(t3f_project_x(levedit_level->object[levedit_selected_object].x - levedit_cx, levedit_level->tilemap->layer[levedit_level->object[levedit_selected_object].layer]->z - levedit_cz),
								  t3f_project_y(levedit_level->object[levedit_selected_object].y - levedit_cy, levedit_level->tilemap->layer[levedit_level->object[levedit_selected_object].layer]->z - levedit_cz),
								  t3f_project_x(levedit_level->object[levedit_selected_object].x + levedit_object_animation[levedit_level->object[levedit_selected_object].type]->frame[0]->width - levedit_cx, levedit_level->tilemap->layer[levedit_level->object[levedit_selected_object].layer]->z - levedit_cz),
								  t3f_project_y(levedit_level->object[levedit_selected_object].y + levedit_object_animation[levedit_level->object[levedit_selected_object].type]->frame[0]->height - levedit_cy, levedit_level->tilemap->layer[levedit_level->object[levedit_selected_object].layer]->z - levedit_cz),
								  al_map_rgba_f(0.0, 0.5, 0.5, 0.5), 1.0);
			}
			if(levedit_hover_tile >= 0)
			{
				t3f_draw_animation(levedit_level->tileset->tile[levedit_hover_tile]->ap, al_map_rgba_f(0.5, 0.5, 0.5, 0.5), levedit_tick, 640 - 32, 480 - 32, 0, 0);
			}
			tx = 640 - 32 - 32;
			ty = 480 - 32;
			t3f_draw_animation(levedit_level->tileset->tile[levedit_selected_tile]->ap, al_map_rgba_f(1.0, 1.0, 1.0, 1.0), levedit_tick, tx, ty, 0, 0);
			if(levedit_level->collision_tilemap[levedit_selected_layer])
			{
				levedit_render_collision_tile(levedit_level->tileset->tile[levedit_selected_tile]->user_data[15], tx, ty);
			}
			al_draw_textf(levedit_font, t3f_color_white, 0.0, 0.0, 0, "(%4.1f, %4.1f, %4.1f) Layer: %d", levedit_cx, levedit_cy, levedit_cz, levedit_selected_layer);
		}
	}
	else
	{
		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_draw_textf(levedit_font, t3f_color_white, 0.0, 0.0, 0, "No level loaded...");
	}
}

void levedit_meta_render(void)
{
	al_clear_to_color(al_map_rgb(0.0, 0.0, 0.5));
	switch(levedit_selected_meta)
	{
		case 0:
		{
			al_draw_textf(levedit_font, t3f_color_white, 0, 0, 0, "Name: %s", levedit_level->info.name);
			break;
		}
		case 1:
		{
			al_draw_textf(levedit_font, t3f_color_white, 0, 0, 0, "Author: %s", levedit_level->info.author);
			break;
		}
		case 2:
		{
			al_draw_textf(levedit_font, t3f_color_white, 0, 0, 0, "Comment: %s", levedit_level->info.comment);
			break;
		}
	}
}

void levedit_render(void)
{
	switch(levedit_view)
	{
		case LEVEDIT_VIEW_LEVEL:
		{
			levedit_level_render();
			break;
		}
		case LEVEDIT_VIEW_META:
		{
			levedit_meta_render();
			break;
		}
	}
}

bool levedit_load_animations(void)
{
	int i;
	
	if(!pp2_legacy_load_palette("data/graphics/legacy_palette.png"))
	{
		return false;
	}
	levedit_object_animation[PP2_OBJECT_PORTAL] = pp2_legacy_load_animation("data/graphics/portal.ani");
	if(!levedit_object_animation[PP2_OBJECT_PORTAL])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_PORTAL);
		return false;
	}
	levedit_object_animation[PP2_OBJECT_AMMO_NORMAL] = t3f_load_animation("data/graphics/ammo_normal.t3a");
	if(!levedit_object_animation[PP2_OBJECT_AMMO_NORMAL])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_AMMO_NORMAL);
		return false;
	}
	levedit_object_animation[PP2_OBJECT_AMMO_X] = t3f_load_animation("data/graphics/ammo_splitter.t3a");
	if(!levedit_object_animation[PP2_OBJECT_AMMO_X])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_AMMO_X);
		return false;
	}
	levedit_object_animation[PP2_OBJECT_AMMO_MINE] = t3f_load_animation("data/graphics/ammo_mine.t3a");
	if(!levedit_object_animation[PP2_OBJECT_AMMO_MINE])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_AMMO_MINE);
		return false;
	}
	levedit_object_animation[PP2_OBJECT_AMMO_BOUNCE] = t3f_load_animation("data/graphics/ammo_bouncer.t3a");
	if(!levedit_object_animation[PP2_OBJECT_AMMO_BOUNCE])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_AMMO_BOUNCE);
		return false;
	}
	levedit_object_animation[PP2_OBJECT_AMMO_SEEK] = t3f_load_animation("data/graphics/ammo_seeker.t3a");
	if(!levedit_object_animation[PP2_OBJECT_AMMO_SEEK])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_AMMO_SEEK);
		return false;
	}
	levedit_object_animation[PP2_OBJECT_AMMO_REFLECTOR] = t3f_load_animation("data/graphics/ammo_reflector.t3a");
	if(!levedit_object_animation[PP2_OBJECT_AMMO_REFLECTOR])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_AMMO_REFLECTOR);
		return false;
	}
	levedit_object_animation[PP2_OBJECT_AMMO_PMINE] = t3f_load_animation("data/graphics/ammo_pmine.t3a");
	if(!levedit_object_animation[PP2_OBJECT_AMMO_PMINE])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_AMMO_PMINE);
		return false;
	}
	levedit_object_animation[PP2_OBJECT_AMMO_GHOST] = t3f_load_animation("data/graphics/ammo_ghost.t3a");
	if(!levedit_object_animation[PP2_OBJECT_AMMO_GHOST])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_AMMO_GHOST);
		return false;
	}
	levedit_object_animation[PP2_OBJECT_POWER_CLOAK] = t3f_load_animation("data/graphics/powerup_cloak.t3a");
	if(!levedit_object_animation[PP2_OBJECT_POWER_CLOAK])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_POWER_CLOAK);
		return false;
	}
	levedit_object_animation[PP2_OBJECT_POWER_JUMP] = t3f_load_animation("data/graphics/powerup_jump.t3a");
	if(!levedit_object_animation[PP2_OBJECT_POWER_JUMP])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_POWER_JUMP);
		return false;
	}
	levedit_object_animation[PP2_OBJECT_POWER_RUN] = t3f_load_animation("data/graphics/powerup_run.t3a");
	if(!levedit_object_animation[PP2_OBJECT_POWER_RUN])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_POWER_RUN);
		return false;
	}
	levedit_object_animation[PP2_OBJECT_POWER_DEFLECT] = t3f_load_animation("data/graphics/powerup_deflect.t3a");
	if(!levedit_object_animation[PP2_OBJECT_POWER_DEFLECT])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_POWER_DEFLECT);
		return false;
	}
	levedit_object_animation[PP2_OBJECT_POWER_FLY] = t3f_load_animation("data/graphics/powerup_fly.t3a");
	if(!levedit_object_animation[PP2_OBJECT_POWER_FLY])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_POWER_FLY);
		return false;
	}
	levedit_object_animation[PP2_OBJECT_POWER_TURBO] = t3f_load_animation("data/graphics/powerup_turbo.t3a");
	if(!levedit_object_animation[PP2_OBJECT_POWER_TURBO])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_POWER_TURBO);
		return false;
	}
	levedit_object_animation[PP2_OBJECT_SPRING_UP] = pp2_legacy_load_animation("data/graphics/spring_up.ani");
	if(!levedit_object_animation[PP2_OBJECT_SPRING_UP])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_SPRING_UP);
		return false;
	}
	levedit_object_animation[PP2_OBJECT_SPRING_DOWN] = pp2_legacy_load_animation("data/graphics/spring_down.ani");
	if(!levedit_object_animation[PP2_OBJECT_SPRING_DOWN])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_SPRING_DOWN);
		return false;
	}
	levedit_object_animation[PP2_OBJECT_SPRING_LEFT] = pp2_legacy_load_animation("data/graphics/spring_left.ani");
	if(!levedit_object_animation[PP2_OBJECT_SPRING_LEFT])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_SPRING_LEFT);
		return false;
	}
	levedit_object_animation[PP2_OBJECT_SPRING_RIGHT] = pp2_legacy_load_animation("data/graphics/spring_right.ani");
	if(!levedit_object_animation[PP2_OBJECT_SPRING_RIGHT])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_SPRING_RIGHT);
		return false;
	}
	levedit_object_animation[PP2_OBJECT_TILE_READER] = t3f_load_animation_from_bitmap("data/graphics/tile_reader.png");
	if(!levedit_object_animation[PP2_OBJECT_TILE_READER])
	{
		printf("Error loading animation %d!\n", PP2_OBJECT_TILE_READER);
		return false;
	}
	levedit_object_atlas = t3f_create_atlas(T3F_ATLAS_SPRITES, 1024, 1024);
	if(levedit_object_atlas)
	{
		for(i = 0; i < 256; i++)
		{
			if(levedit_object_animation[i])
			{
				t3f_add_animation_to_atlas(levedit_object_atlas, levedit_object_animation[i]);
			}
		}
	}
	return true;
}

bool levedit_initialize(int argc, char * argv[])
{
	if(!t3f_initialize("Level Builder", 640, 480, 60.0, levedit_logic, levedit_render, T3F_DEFAULT | T3F_USE_MOUSE))
	{
		return false;
	}
	if(!t3f_locate_resource("data/fonts/chared_font.png"))
	{
		return false;
	}
	al_set_new_bitmap_flags(ALLEGRO_NO_PREMULTIPLIED_ALPHA);
	al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
	levedit_font = al_load_bitmap_font("data/fonts/chared_font.png");
	if(!levedit_font)
	{
		return false;
	}
	if(!levedit_load_animations())
	{
		return false;
	}
	return true;
}

int main(int argc, char * argv[])
{
	if(!levedit_initialize(argc, argv))
	{
		printf("Initialization failed!\n");
		return -1;
	}
	t3f_run();
	return 0;
}
