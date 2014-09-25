#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include "t3f/t3f.h"
#include "file/character.h"
#include "legacy/animation.h"
#include "legacy/character.h"

#define CHARED_VIEW_ANIMATIONS 0
#define CHARED_VIEW_CHARACTER  1
#define CHARED_VIEW_BITMAP     2
#define CHARED_VIEW_META       3

char * chared_state_name[PP2_CHARACTER_MAX_STATES] = 
{
	"PP2_CHARACTER_STATE_STAND_R_R",
	"PP2_CHARACTER_STATE_STAND_R_DR",
	"PP2_CHARACTER_STATE_STAND_R_D",
	"PP2_CHARACTER_STATE_STAND_R_DL",
	"PP2_CHARACTER_STATE_STAND_R_L",
	"PP2_CHARACTER_STATE_STAND_R_UL",
	"PP2_CHARACTER_STATE_STAND_R_U",
	"PP2_CHARACTER_STATE_STAND_R_UR",
	"PP2_CHARACTER_STATE_STAND_L_R",
	"PP2_CHARACTER_STATE_STAND_L_DR",
	"PP2_CHARACTER_STATE_STAND_L_D",
	"PP2_CHARACTER_STATE_STAND_L_DL",
	"PP2_CHARACTER_STATE_STAND_L_L",
	"PP2_CHARACTER_STATE_STAND_L_UL",
	"PP2_CHARACTER_STATE_STAND_L_U",
	"PP2_CHARACTER_STATE_STAND_L_UR",
	"PP2_CHARACTER_STATE_WALK_R_R",
	"PP2_CHARACTER_STATE_WALK_R_DR",
	"PP2_CHARACTER_STATE_WALK_R_D",
	"PP2_CHARACTER_STATE_WALK_R_DL",
	"PP2_CHARACTER_STATE_WALK_R_L",
	"PP2_CHARACTER_STATE_WALK_R_UL",
	"PP2_CHARACTER_STATE_WALK_R_U",
	"PP2_CHARACTER_STATE_WALK_R_UR",
	"PP2_CHARACTER_STATE_WALK_L_R",
	"PP2_CHARACTER_STATE_WALK_L_DR",
	"PP2_CHARACTER_STATE_WALK_L_D",
	"PP2_CHARACTER_STATE_WALK_L_DL",
	"PP2_CHARACTER_STATE_WALK_L_L",
	"PP2_CHARACTER_STATE_WALK_L_UL",
	"PP2_CHARACTER_STATE_WALK_L_U",
	"PP2_CHARACTER_STATE_WALK_L_UR",
	"PP2_CHARACTER_STATE_JUMP_R_R",
	"PP2_CHARACTER_STATE_JUMP_R_DR",
	"PP2_CHARACTER_STATE_JUMP_R_D",
	"PP2_CHARACTER_STATE_JUMP_R_DL",
	"PP2_CHARACTER_STATE_JUMP_R_L",
	"PP2_CHARACTER_STATE_JUMP_R_UL",
	"PP2_CHARACTER_STATE_JUMP_R_U",
	"PP2_CHARACTER_STATE_JUMP_R_UR",
	"PP2_CHARACTER_STATE_JUMP_L_R",
	"PP2_CHARACTER_STATE_JUMP_L_DR",
	"PP2_CHARACTER_STATE_JUMP_L_D",
	"PP2_CHARACTER_STATE_JUMP_L_DL",
	"PP2_CHARACTER_STATE_JUMP_L_L",
	"PP2_CHARACTER_STATE_JUMP_L_UL",
	"PP2_CHARACTER_STATE_JUMP_L_U",
	"PP2_CHARACTER_STATE_JUMP_L_UR",
	"PP2_CHARACTER_STATE_FALL_R_R",
	"PP2_CHARACTER_STATE_FALL_R_DR",
	"PP2_CHARACTER_STATE_FALL_R_D",
	"PP2_CHARACTER_STATE_FALL_R_DL",
	"PP2_CHARACTER_STATE_FALL_R_L",
	"PP2_CHARACTER_STATE_FALL_R_UL",
	"PP2_CHARACTER_STATE_FALL_R_U",
	"PP2_CHARACTER_STATE_FALL_R_UR",
	"PP2_CHARACTER_STATE_FALL_L_R",
	"PP2_CHARACTER_STATE_FALL_L_DR",
	"PP2_CHARACTER_STATE_FALL_L_D",
	"PP2_CHARACTER_STATE_FALL_L_DL",
	"PP2_CHARACTER_STATE_FALL_L_L",
	"PP2_CHARACTER_STATE_FALL_L_UL",
	"PP2_CHARACTER_STATE_FALL_L_U",
	"PP2_CHARACTER_STATE_FALL_L_UR",
	"PP2_CHARACTER_STATE_DUCK_R_R",
	"PP2_CHARACTER_STATE_DUCK_R_DR",
	"PP2_CHARACTER_STATE_DUCK_R_D",
	"PP2_CHARACTER_STATE_DUCK_R_DL",
	"PP2_CHARACTER_STATE_DUCK_R_L",
	"PP2_CHARACTER_STATE_DUCK_R_UL",
	"PP2_CHARACTER_STATE_DUCK_R_U",
	"PP2_CHARACTER_STATE_DUCK_R_UR",
	"PP2_CHARACTER_STATE_DUCK_L_R",
	"PP2_CHARACTER_STATE_DUCK_L_DR",
	"PP2_CHARACTER_STATE_DUCK_L_D",
	"PP2_CHARACTER_STATE_DUCK_L_DL",
	"PP2_CHARACTER_STATE_DUCK_L_L",
	"PP2_CHARACTER_STATE_DUCK_L_UL",
	"PP2_CHARACTER_STATE_DUCK_L_U",
	"PP2_CHARACTER_STATE_DUCK_L_UR"
};

float chared_angle_table[PP2_CHARACTER_MAX_STATES] = 
{
	0.0, 0.25 * ALLEGRO_PI, 0.5 * ALLEGRO_PI, 0.75 * ALLEGRO_PI, ALLEGRO_PI, 1.25 * ALLEGRO_PI, 1.5 * ALLEGRO_PI, 1.75 * ALLEGRO_PI,
	0.0, 0.25 * ALLEGRO_PI, 0.5 * ALLEGRO_PI, 0.75 * ALLEGRO_PI, ALLEGRO_PI, 1.25 * ALLEGRO_PI, 1.5 * ALLEGRO_PI, 1.75 * ALLEGRO_PI,
	0.0, 0.25 * ALLEGRO_PI, 0.5 * ALLEGRO_PI, 0.75 * ALLEGRO_PI, ALLEGRO_PI, 1.25 * ALLEGRO_PI, 1.5 * ALLEGRO_PI, 1.75 * ALLEGRO_PI,
	0.0, 0.25 * ALLEGRO_PI, 0.5 * ALLEGRO_PI, 0.75 * ALLEGRO_PI, ALLEGRO_PI, 1.25 * ALLEGRO_PI, 1.5 * ALLEGRO_PI, 1.75 * ALLEGRO_PI,
	0.0, 0.25 * ALLEGRO_PI, 0.5 * ALLEGRO_PI, 0.75 * ALLEGRO_PI, ALLEGRO_PI, 1.25 * ALLEGRO_PI, 1.5 * ALLEGRO_PI, 1.75 * ALLEGRO_PI,
	0.0, 0.25 * ALLEGRO_PI, 0.5 * ALLEGRO_PI, 0.75 * ALLEGRO_PI, ALLEGRO_PI, 1.25 * ALLEGRO_PI, 1.5 * ALLEGRO_PI, 1.75 * ALLEGRO_PI,
	0.0, 0.25 * ALLEGRO_PI, 0.5 * ALLEGRO_PI, 0.75 * ALLEGRO_PI, ALLEGRO_PI, 1.25 * ALLEGRO_PI, 1.5 * ALLEGRO_PI, 1.75 * ALLEGRO_PI,
	0.0, 0.25 * ALLEGRO_PI, 0.5 * ALLEGRO_PI, 0.75 * ALLEGRO_PI, ALLEGRO_PI, 1.25 * ALLEGRO_PI, 1.5 * ALLEGRO_PI, 1.75 * ALLEGRO_PI,
	0.0, 0.25 * ALLEGRO_PI, 0.5 * ALLEGRO_PI, 0.75 * ALLEGRO_PI, ALLEGRO_PI, 1.25 * ALLEGRO_PI, 1.5 * ALLEGRO_PI, 1.75 * ALLEGRO_PI,
	0.0, 0.25 * ALLEGRO_PI, 0.5 * ALLEGRO_PI, 0.75 * ALLEGRO_PI, ALLEGRO_PI, 1.25 * ALLEGRO_PI, 1.5 * ALLEGRO_PI, 1.75 * ALLEGRO_PI
};

PP2_CHARACTER * chared_character = NULL;
ALLEGRO_FILECHOOSER * chared_file_load_dialog = NULL;
ALLEGRO_FILECHOOSER * chared_file_save_dialog = NULL;
ALLEGRO_FILECHOOSER * chared_file_load_ani_dialog = NULL;
ALLEGRO_PATH * chared_path = NULL;
ALLEGRO_PATH * chared_return_path = NULL;
const char * chared_ani_path = NULL;
const char * chared_return_path_cstr = NULL;
char chared_start_path[PATH_MAX] = {0};
ALLEGRO_FONT * chared_font = NULL;
unsigned long chared_tick = 0;
int chared_view = CHARED_VIEW_ANIMATIONS;

/* import data */
ALLEGRO_CONFIG * chared_import_config = NULL;
ALLEGRO_BITMAP * chared_import_bitmap = NULL;
T3F_ANIMATION * chared_import_animation[PP2_CHARACTER_MAX_ANIMATIONS] = {NULL};
int chared_import_ox = 0;
int chared_import_oy = 0;
int chared_import_ow = 0;
int chared_import_oh = 0;
int chared_import_vx = 0;
int chared_import_vy = 0;
int chared_import_step = 0;

int chared_selected_state = 0;
int chared_selected_piece = 0;
int chared_selected_animation = 0;
int chared_selected_bitmap = 0;
char chared_temp_string[1024] = {0};
char * chared_entry_pointer = NULL;
int chared_entry_pos = 0;
int chared_selected_meta = 0;

bool pegged = false;
int pegx, pegy;

const ALLEGRO_PATH * chared_replace_path(const char * fn)
{
	if(chared_return_path)
	{
		al_destroy_path(chared_return_path);
	}
	return al_create_path(fn);
}

const char * chared_get_extension(const char * fn)
{
	ALLEGRO_PATH * path;
	
	path = al_create_path(fn);
	if(path)
	{
		strcpy(chared_temp_string, al_get_path_extension(path));
		al_destroy_path(path);
		return chared_temp_string;
	}
	strcpy(chared_temp_string, "");
	return chared_temp_string;
}

void chared_remove_filename(char * fn)
{
	int i;
	
	i = strlen(fn);
	while(i >= 0)
	{
		if(fn[i] == '/')
		{
			fn[i + 1] = 0;
			return;
		}
	}
}

void chared_get_entry_pos(void)
{
	switch(chared_selected_meta)
	{
		case 0:
		{
			chared_entry_pos = strlen(chared_character->info.name);
			chared_entry_pointer = chared_character->info.name;
			break;
		}
		case 1:
		{
			chared_entry_pos = strlen(chared_character->info.author);
			chared_entry_pointer = chared_character->info.author;
			break;
		}
		case 2:
		{
			chared_entry_pos = strlen(chared_character->info.comment);
			chared_entry_pointer = chared_character->info.comment;
			break;
		}
	}
}

void chared_animations_logic(void)
{
	T3F_ANIMATION * ap = NULL;
	int i;
	
	if(t3f_key[ALLEGRO_KEY_INSERT] || t3f_key[ALLEGRO_KEY_EQUALS])
	{
		if(chared_file_load_ani_dialog)
		{
			al_destroy_native_file_dialog(chared_file_load_ani_dialog);
		}
		if(chared_ani_path)
		{
//			chared_remove_filename(chared_ani_path);
		}
		chared_file_load_ani_dialog = al_create_native_file_dialog(chared_ani_path, "Load Animation", NULL, ALLEGRO_FILECHOOSER_MULTIPLE);
		if(chared_file_load_ani_dialog)
		{
			al_show_native_file_dialog(t3f_display, chared_file_load_ani_dialog);
			if(al_get_native_file_dialog_count(chared_file_load_ani_dialog) > 0)
			{
				for(i = 0; i < al_get_native_file_dialog_count(chared_file_load_ani_dialog); i++)
				{
					chared_ani_path = al_get_native_file_dialog_path(chared_file_load_ani_dialog, i);
					if(!strcasecmp(chared_get_extension(chared_ani_path), ".t3a"))
					{
						chared_character->animation[chared_character->animations] = t3f_load_animation(chared_ani_path);
					}
					else
					{
						chared_character->animation[chared_character->animations] = t3f_load_animation_from_bitmap(chared_ani_path);
					}
					if(chared_character->animation[chared_character->animations])
					{
						chared_selected_animation = chared_character->animations;
						chared_character->animations++;
					}
				}
			}
			al_destroy_native_file_dialog(chared_file_load_ani_dialog);
			chared_file_load_ani_dialog = NULL;
		}
		t3f_key[ALLEGRO_KEY_INSERT] = 0;
		t3f_key[ALLEGRO_KEY_EQUALS] = 0;
	}
	if(t3f_key[ALLEGRO_KEY_DELETE])
	{
		if(chared_character->animation[chared_selected_animation])
		{
			t3f_destroy_animation(chared_character->animation[chared_selected_animation]);
			for(i = chared_selected_animation; i < chared_character->animations - 1; i++)
			{
				chared_character->animation[i] = chared_character->animation[i + 1];
			}
		}
		t3f_key[ALLEGRO_KEY_DELETE] = 0;
	}
	if(t3f_key[ALLEGRO_KEY_ENTER])
	{
		chared_file_load_ani_dialog = al_create_native_file_dialog(chared_ani_path, "Load Animation", NULL, ALLEGRO_FILECHOOSER_MULTIPLE);
		al_show_native_file_dialog(t3f_display, chared_file_load_ani_dialog);
		if(al_get_native_file_dialog_count(chared_file_load_ani_dialog) > 0)
		{
			chared_return_path_cstr = al_get_native_file_dialog_path(chared_file_load_ani_dialog, 0);
			if(!strcmp(al_get_path_extension(chared_replace_path(chared_return_path_cstr)), ".t3a"))
			{
				ap = t3f_load_animation(al_path_cstr(chared_replace_path(chared_return_path_cstr), '/'));
			}
			else
			{
				ap = t3f_load_animation_from_bitmap(al_path_cstr(chared_replace_path(chared_return_path_cstr), '/'));
			}
			if(ap)
			{
				t3f_destroy_animation(chared_character->animation[chared_selected_animation]);
				chared_character->animation[chared_selected_animation] = ap;
				printf("new animation loaded\n");
			}
		}
		al_destroy_native_file_dialog(chared_file_load_ani_dialog);
		t3f_key[ALLEGRO_KEY_ENTER] = 0;
	}
	if(t3f_key[ALLEGRO_KEY_LEFT])
	{
		chared_selected_animation--;
		if(chared_selected_animation < 0)
		{
			chared_selected_animation = chared_character->animations - 1;
		}
		t3f_key[ALLEGRO_KEY_LEFT] = 0;
	}
	if(t3f_key[ALLEGRO_KEY_RIGHT])
	{
		chared_selected_animation++;
		if(chared_selected_animation >= chared_character->animations)
		{
			chared_selected_animation = 0;
		}
		t3f_key[ALLEGRO_KEY_RIGHT] = 0;
	}
	if(t3f_key[ALLEGRO_KEY_PAD_ASTERISK])
	{
		for(i = 0; i < chared_character->animation[chared_selected_animation]->frames; i++)
		{
			chared_character->animation[chared_selected_animation]->frame[i]->x *= 2;
			chared_character->animation[chared_selected_animation]->frame[i]->y *= 2;
			chared_character->animation[chared_selected_animation]->frame[i]->width *= 2;
			chared_character->animation[chared_selected_animation]->frame[i]->height *= 2;
		}
		t3f_key[ALLEGRO_KEY_PAD_ASTERISK] = 0;
	}
	if(t3f_key[ALLEGRO_KEY_PAD_SLASH])
	{
		for(i = 0; i < chared_character->animation[chared_selected_animation]->frames; i++)
		{
			chared_character->animation[chared_selected_animation]->frame[i]->x /= 2;
			chared_character->animation[chared_selected_animation]->frame[i]->y /= 2;
			chared_character->animation[chared_selected_animation]->frame[i]->width /= 2;
			chared_character->animation[chared_selected_animation]->frame[i]->height /= 2;
		}
		t3f_key[ALLEGRO_KEY_PAD_SLASH] = 0;
	}
	if(t3f_key[ALLEGRO_KEY_P])
	{
		for(i = 0; i < PP2_CHARACTER_MAX_STATES; i++)
		{
			chared_character->state[i].particle.animation = chared_selected_animation;
			chared_character->state[i].particle.cx = chared_character->animation[chared_selected_animation]->frame[0]->width / 2;
			chared_character->state[i].particle.cy = chared_character->animation[chared_selected_animation]->frame[0]->height / 2;
		}
		t3f_key[ALLEGRO_KEY_P] = 0;
	}
	if(t3f_key[ALLEGRO_KEY_B])
	{
		chared_view = CHARED_VIEW_BITMAP;
		t3f_key[ALLEGRO_KEY_B] = 0;
	}
}

void chared_character_logic(void)
{
	int i, j, k, l;
	int facing;
	
	if(t3f_key[ALLEGRO_KEY_LEFT])
	{
		if(t3f_key[ALLEGRO_KEY_LCTRL])
		{
			chared_character->state[chared_selected_state].piece[chared_selected_piece].x -= 1.0;
		}
		else if(t3f_key[ALLEGRO_KEY_ALT])
		{
			if(t3f_key[ALLEGRO_KEY_S])
			{
				chared_character->state[chared_selected_state].collision_w--;
			}
			else if(t3f_key[ALLEGRO_KEY_X])
			{
				chared_character->state[chared_selected_state].paintball.x--;
			}
			else
			{
				chared_character->state[chared_selected_state].collision_x--;
			}
		}
		else
		{
			chared_selected_state--;
			if(chared_selected_state < 0)
			{
				chared_selected_state = PP2_CHARACTER_MAX_STATES - 1;
			}
		}
		t3f_key[ALLEGRO_KEY_LEFT] = 0;
	}
	if(t3f_key[ALLEGRO_KEY_RIGHT])
	{
		if(t3f_key[ALLEGRO_KEY_LCTRL])
		{
			chared_character->state[chared_selected_state].piece[chared_selected_piece].x += 1.0;
		}
		else if(t3f_key[ALLEGRO_KEY_ALT])
		{
			if(t3f_key[ALLEGRO_KEY_S])
			{
				chared_character->state[chared_selected_state].collision_w++;
			}
			else if(t3f_key[ALLEGRO_KEY_X])
			{
				chared_character->state[chared_selected_state].paintball.x++;
			}
			else
			{
				chared_character->state[chared_selected_state].collision_x++;
			}
		}
		else
		{
			chared_selected_state++;
			if(chared_selected_state >= PP2_CHARACTER_MAX_STATES)
			{
				chared_selected_state = 0;
			}
		}
		t3f_key[ALLEGRO_KEY_RIGHT] = 0;
	}
	if(t3f_key[ALLEGRO_KEY_UP])
	{
		chared_character->state[chared_selected_state].piece[chared_selected_piece].animation--;
		if(chared_character->state[chared_selected_state].piece[chared_selected_piece].animation < 0)
		{
			chared_character->state[chared_selected_state].piece[chared_selected_piece].animation = chared_character->animations - 1;
		}
		t3f_key[ALLEGRO_KEY_UP] = 0;
	}
	if(t3f_key[ALLEGRO_KEY_DOWN])
	{
		chared_character->state[chared_selected_state].piece[chared_selected_piece].animation++;
		if(chared_character->state[chared_selected_state].piece[chared_selected_piece].animation >= chared_character->animations)
		{
			chared_character->state[chared_selected_state].piece[chared_selected_piece].animation = 0;
		}
		t3f_key[ALLEGRO_KEY_DOWN] = 0;
	}
	if(t3f_key[ALLEGRO_KEY_PGUP])
	{
		chared_selected_piece--;
		if(chared_selected_piece < 0)
		{
			chared_selected_piece = chared_character->state[chared_selected_state].pieces - 1;
		}
		t3f_key[ALLEGRO_KEY_PGUP] = 0;
	}
	if(t3f_key[ALLEGRO_KEY_PGDN])
	{
		chared_selected_piece++;
		if(chared_selected_piece >= chared_character->state[chared_selected_state].pieces)
		{
			chared_selected_piece = 0;
		}
		t3f_key[ALLEGRO_KEY_PGDN] = 0;
	}
	if(t3f_key[ALLEGRO_KEY_EQUALS])
	{
		chared_character->state[chared_selected_state].piece[chared_character->state[chared_selected_state].pieces].animation = chared_selected_animation;
		chared_character->state[chared_selected_state].pieces++;
		t3f_key[ALLEGRO_KEY_EQUALS] = 0;
	}
	if(t3f_key[ALLEGRO_KEY_MINUS])
	{
		chared_character->state[chared_selected_state].pieces--;
		t3f_key[ALLEGRO_KEY_MINUS] = 0;
	}
	if(t3f_key[ALLEGRO_KEY_COMMA])
	{
		chared_character->state[chared_selected_state].piece[chared_selected_piece].angle -= 0.01;
		t3f_key[ALLEGRO_KEY_COMMA] = 0;
	}
	if(t3f_key[ALLEGRO_KEY_FULLSTOP])
	{
		chared_character->state[chared_selected_state].piece[chared_selected_piece].angle += 0.01;
		t3f_key[ALLEGRO_KEY_FULLSTOP] = 0;
	}
	if(t3f_key[ALLEGRO_KEY_H])
	{
		chared_character->state[chared_selected_state].piece[chared_selected_piece].flags ^= ALLEGRO_FLIP_HORIZONTAL;
		t3f_key[ALLEGRO_KEY_H] = 0;
	}
	if(t3f_key[ALLEGRO_KEY_D])
	{
		for(i = 0; i < PP2_CHARACTER_MAX_STATES; i++)
		{
			if(i / 8 == chared_selected_state / 8)
			{
			}
			else
			{
				memcpy(&chared_character->state[i], &chared_character->state[chared_selected_state / 8 + i % 8], sizeof(PP2_CHARACTER_STATE));
			}
		}
		t3f_key[ALLEGRO_KEY_D] = 0;
	}
	
	/* make all states in the current group of states the same */
	if(t3f_key[ALLEGRO_KEY_B])
	{
		for(i = (chared_selected_state / 8) * 8; i < (chared_selected_state / 8) * 8 + 8; i++)
		{
			if(i != chared_selected_state)
			{
				memcpy(&chared_character->state[i], &chared_character->state[chared_selected_state], sizeof(PP2_CHARACTER_STATE));
			}
		}
		t3f_key[ALLEGRO_KEY_B] = 0;
	}
	
	/* create mirror of current state group */
	if(t3f_key[ALLEGRO_KEY_M])
	{
		j = (chared_selected_state / 8);
		k = j % 2 ? -1 : 1;
		for(i = (j + k) * 8; i < (j + k) * 8 + 8; i++)
		{
			if(i != chared_selected_state)
			{
				memcpy(&chared_character->state[i], &chared_character->state[i + 8 * -k], sizeof(PP2_CHARACTER_STATE));
				for(l = 0; l < chared_character->state[i].pieces; l++)
				{
					chared_character->state[i].piece[l].flags ^= ALLEGRO_FLIP_HORIZONTAL;
				}
			}
		}
		t3f_key[ALLEGRO_KEY_M] = 0;
	}
	
	/* duplicate current collision box */
	if(t3f_key[ALLEGRO_KEY_C])
	{
		if(chared_selected_state >= PP2_CHARACTER_STATE_DUCK_R_R)
		{
			for(i = PP2_CHARACTER_STATE_DUCK_R_R; i < PP2_CHARACTER_MAX_STATES; i++)
			{
				if(i != chared_selected_state)
				{
					chared_character->state[i].collision_x = chared_character->state[chared_selected_state].collision_x;
					chared_character->state[i].collision_y = chared_character->state[chared_selected_state].collision_y;
					chared_character->state[i].collision_w = chared_character->state[chared_selected_state].collision_w;
					chared_character->state[i].collision_h = chared_character->state[chared_selected_state].collision_h;
				}
			}
		}
		else
		{
			for(i = 0; i < PP2_CHARACTER_STATE_DUCK_R_R; i++)
			{
				if(i != chared_selected_state)
				{
					chared_character->state[i].collision_x = chared_character->state[chared_selected_state].collision_x;
					chared_character->state[i].collision_y = chared_character->state[chared_selected_state].collision_y;
					chared_character->state[i].collision_w = chared_character->state[chared_selected_state].collision_w;
					chared_character->state[i].collision_h = chared_character->state[chared_selected_state].collision_h;
				}
			}
		}
		t3f_key[ALLEGRO_KEY_C] = 0;
	}
	
	/* make current piece the same for all similar states */
	if(t3f_key[ALLEGRO_KEY_S])
	{
		facing = (chared_selected_state / 8) % 2;
		for(i = chared_selected_state % 8; i < PP2_CHARACTER_MAX_STATES; i += 8)
		{
			if(i != chared_selected_state && facing == (i / 8) % 2)
			{
				memcpy(&chared_character->state[i].piece[chared_selected_piece], &chared_character->state[chared_selected_state].piece[chared_selected_piece], sizeof(PP2_CHARACTER_PIECE));
				chared_character->state[i].pieces = chared_character->state[chared_selected_state].pieces;
			}
		}
		t3f_key[ALLEGRO_KEY_S] = 0;
	}
	
	if(t3f_key[ALLEGRO_KEY_LSHIFT] && t3f_key[ALLEGRO_KEY_P])
	{
		facing = (chared_selected_state / 8) % 2;
		for(i = chared_selected_state % 8; i < PP2_CHARACTER_MAX_STATES; i += 8)
		{
			if(i != chared_selected_state && facing == (i / 8) % 2)
			{
				memcpy(&chared_character->state[i].paintball, &chared_character->state[chared_selected_state].paintball, sizeof(PP2_CHARACTER_PIECE));
			}
		}
		t3f_key[ALLEGRO_KEY_P] = 0;
	}
	if(t3f_key[ALLEGRO_KEY_O])
	{
		chared_character->state[chared_selected_state].paintball.animation++;
		if(chared_character->state[chared_selected_state].paintball.animation >= chared_character->animations)
		{
			chared_character->state[chared_selected_state].paintball.animation = 0;
		}
		if(t3f_key[ALLEGRO_KEY_LCTRL])
		{
			for(i = 0; i < PP2_CHARACTER_MAX_STATES; i++)
			{
				if(i != chared_selected_state)
				{
					chared_character->state[i].paintball.animation = chared_character->state[chared_selected_state].paintball.animation;
				}
			}
		}
		if(t3f_key[ALLEGRO_KEY_LSHIFT])
		{
			for(i = 0; i < PP2_CHARACTER_MAX_STATES; i++)
			{
				chared_character->state[i].particle.animation = chared_character->state[chared_selected_state].paintball.animation;
			}
		}
		t3f_key[ALLEGRO_KEY_O] = 0;
	}
	
	if(t3f_mouse_button[0])
	{
		if(!pegged)
		{
			pegx = t3f_mouse_x;
			pegy = t3f_mouse_y;
			pegged = true;
		}
		if(t3f_key[ALLEGRO_KEY_P])
		{
			if(t3f_key[ALLEGRO_KEY_OPENBRACE])
			{
				chared_character->paintball_size--;
				t3f_key[ALLEGRO_KEY_P] = 0;
			}
			else
			{
				chared_character->state[chared_selected_state].paintball.x += (t3f_mouse_x - pegx);
				chared_character->state[chared_selected_state].paintball.y += (t3f_mouse_y - pegy);
				chared_character->paintball_size = chared_character->animation[chared_character->state[chared_selected_state].paintball.animation]->frame[0]->width / 2;
			}
		}
		else if(t3f_key[ALLEGRO_KEY_LSHIFT])
		{
			if(t3f_key[ALLEGRO_KEY_LCTRL])
			{
				chared_character->state[chared_selected_state].collision_w += (t3f_mouse_x - pegx);
				chared_character->state[chared_selected_state].collision_h += (t3f_mouse_y - pegy);
			}
			else
			{
				chared_character->state[chared_selected_state].collision_x += (t3f_mouse_x - pegx);
				chared_character->state[chared_selected_state].collision_y += (t3f_mouse_y - pegy);
			}
		}
		else
		{
			chared_character->state[chared_selected_state].piece[chared_selected_piece].x += (t3f_mouse_x - pegx);
			chared_character->state[chared_selected_state].piece[chared_selected_piece].y += (t3f_mouse_y - pegy);
		}
		pegx = t3f_mouse_x;
		pegy = t3f_mouse_y;
	}
	else
	{
		pegged = false;
	}
}

void chared_read_import_config(void)
{
	char buffer[256] = {0};
	const char * val;
	
	sprintf(buffer, "Animation %d Bitmap %d", chared_selected_animation, chared_selected_bitmap);
	val = al_get_config_value(chared_import_config, buffer, "x");
	if(val)
	{
		chared_import_ox = atoi(val);
	}
	val = al_get_config_value(chared_import_config, buffer, "y");
	if(val)
	{
		chared_import_oy = atoi(val);
	}
	val = al_get_config_value(chared_import_config, buffer, "w");
	if(val)
	{
		chared_import_ow = atoi(val);
	}
	val = al_get_config_value(chared_import_config, buffer, "h");
	if(val)
	{
		chared_import_oh = atoi(val);
	}
}

void chared_write_import_config(void)
{
	char buffer[256] = {0};
	char bufval[256] = {0};
	
	sprintf(buffer, "Animation %d Bitmap %d", chared_selected_animation, chared_selected_bitmap);
	sprintf(bufval, "%d", chared_import_ox);
	al_set_config_value(chared_import_config, buffer, "x", bufval);
	sprintf(bufval, "%d", chared_import_oy);
	al_set_config_value(chared_import_config, buffer, "y", bufval);
	sprintf(bufval, "%d", chared_import_ow);
	al_set_config_value(chared_import_config, buffer, "w", bufval);
	sprintf(bufval, "%d", chared_import_oh);
	al_set_config_value(chared_import_config, buffer, "h", bufval);
}

void chared_move_bitmap(int bitmap, float ox, float oy)
{
	int i;
	
	for(i = 0; i < chared_character->animation[chared_selected_animation]->frames; i++)
	{
		if(chared_character->animation[chared_selected_animation]->frame[i]->bitmap == bitmap)
		{
			chared_character->animation[chared_selected_animation]->frame[i]->x += ox;
			chared_character->animation[chared_selected_animation]->frame[i]->y += oy;
		}
	}
}

void chared_grab(void)
{
	ALLEGRO_BITMAP * sub_bitmap = NULL;
	int i;
	
	sub_bitmap = al_create_sub_bitmap(chared_import_bitmap, chared_import_ox, chared_import_oy, chared_import_ow, chared_import_oh);
	if(sub_bitmap)
	{
		if(!chared_import_animation[chared_selected_animation])
		{
			chared_import_animation[chared_selected_animation] = t3f_clone_animation(chared_character->animation[chared_selected_animation]);
		}
		al_destroy_bitmap(chared_character->animation[chared_selected_animation]->bitmap[chared_selected_bitmap]);
		chared_character->animation[chared_selected_animation]->bitmap[chared_selected_bitmap] = al_clone_bitmap(sub_bitmap);
		al_destroy_bitmap(sub_bitmap);
		for(i = 0; i < chared_character->animation[chared_selected_animation]->frames; i++)
		{
			if(chared_character->animation[chared_selected_animation]->frame[i]->bitmap == chared_selected_bitmap)
			{
				chared_character->animation[chared_selected_animation]->frame[i]->width = al_get_bitmap_width(chared_character->animation[chared_selected_animation]->bitmap[chared_selected_bitmap]);
				chared_character->animation[chared_selected_animation]->frame[i]->height = al_get_bitmap_height(chared_character->animation[chared_selected_animation]->bitmap[chared_selected_bitmap]);
			}
		}
		chared_write_import_config();
	}
}

void chared_bitmap_logic(void)
{
	int old_animation = chared_selected_animation;
	int old_bitmap = chared_selected_bitmap;
	
	if(t3f_key[ALLEGRO_KEY_ENTER])
	{
		chared_file_load_dialog = al_create_native_file_dialog(chared_start_path, "Load Bitmap", NULL, 0);
		al_show_native_file_dialog(t3f_display, chared_file_load_dialog);
		if(al_get_native_file_dialog_count(chared_file_load_dialog) > 0)
		{
			chared_return_path_cstr = al_get_native_file_dialog_path(chared_file_load_dialog, 0);
			chared_import_bitmap = al_load_bitmap(chared_return_path_cstr);
			strcpy(chared_start_path, chared_return_path_cstr);
		}
		al_destroy_native_file_dialog(chared_file_load_dialog);
		t3f_key[ALLEGRO_KEY_ENTER] = 0;
	}
	
	/* this should be used to "grab" the current selection into the character */
	if(t3f_key[ALLEGRO_KEY_G])
	{
		chared_grab();
		t3f_key[ALLEGRO_KEY_G] = 0;
	}
	
	if(t3f_key[ALLEGRO_KEY_I])
	{
		for(chared_selected_animation = 0; chared_selected_animation < chared_character->animations; chared_selected_animation++)
		{
			for(chared_selected_bitmap = 0; chared_selected_bitmap < chared_character->animation[chared_selected_animation]->bitmaps; chared_selected_bitmap++)
			{
				chared_read_import_config();
				chared_grab();
			}
		}
		chared_selected_animation = old_animation;
		chared_selected_bitmap = old_bitmap;
		t3f_key[ALLEGRO_KEY_I] = 0;
	}
	if(t3f_key[ALLEGRO_KEY_LEFT])
	{
		chared_selected_animation--;
		if(chared_selected_animation < 0)
		{
			chared_selected_animation = chared_character->animations - 1;
		}
		chared_selected_bitmap = 0;
		chared_read_import_config();
		t3f_key[ALLEGRO_KEY_LEFT] = 0;
	}
	if(t3f_key[ALLEGRO_KEY_RIGHT])
	{
		chared_selected_animation++;
		if(chared_selected_animation >= chared_character->animations)
		{
			chared_selected_animation = 0;
		}
		chared_selected_bitmap = 0;
		chared_read_import_config();
		t3f_key[ALLEGRO_KEY_RIGHT] = 0;
	}
	if(t3f_key[ALLEGRO_KEY_UP])
	{
		chared_selected_bitmap--;
		if(chared_selected_bitmap < 0)
		{
			chared_selected_bitmap = chared_character->animation[chared_selected_animation]->bitmaps - 1;
		}
		chared_read_import_config();
		t3f_key[ALLEGRO_KEY_UP] = 0;
	}
	if(t3f_key[ALLEGRO_KEY_DOWN])
	{
		chared_selected_bitmap++;
		if(chared_selected_bitmap >= chared_character->animation[chared_selected_animation]->bitmaps)
		{
			chared_selected_bitmap = 0;
		}
		chared_read_import_config();
		t3f_key[ALLEGRO_KEY_DOWN] = 0;
	}
	if(t3f_key[ALLEGRO_KEY_ESCAPE])
	{
		chared_view = CHARED_VIEW_ANIMATIONS;
		t3f_key[ALLEGRO_KEY_ESCAPE] = 0;
	}
	if(t3f_key[ALLEGRO_KEY_LSHIFT])
	{
		if(t3f_key[ALLEGRO_KEY_W])
		{
			chared_move_bitmap(chared_selected_bitmap, 0.0, -1.0);
			t3f_key[ALLEGRO_KEY_W] = 0;
		}
		if(t3f_key[ALLEGRO_KEY_S])
		{
			chared_move_bitmap(chared_selected_bitmap, 0.0, 1.0);
			t3f_key[ALLEGRO_KEY_S] = 0;
		}
		if(t3f_key[ALLEGRO_KEY_A])
		{
			chared_move_bitmap(chared_selected_bitmap, -1.0, 0.0);
			t3f_key[ALLEGRO_KEY_A] = 0;
		}
		if(t3f_key[ALLEGRO_KEY_D])
		{
			chared_move_bitmap(chared_selected_bitmap, 1.0, 0.0);
			t3f_key[ALLEGRO_KEY_D] = 0;
		}
	}
	else if(t3f_key[ALLEGRO_KEY_LCTRL])
	{
		if(t3f_key[ALLEGRO_KEY_W])
		{
			chared_import_oy--;
			t3f_key[ALLEGRO_KEY_W] = 0;
		}
		if(t3f_key[ALLEGRO_KEY_S])
		{
			chared_import_oy++;
			t3f_key[ALLEGRO_KEY_S] = 0;
		}
		if(t3f_key[ALLEGRO_KEY_A])
		{
			chared_import_ox--;
			t3f_key[ALLEGRO_KEY_A] = 0;
		}
		if(t3f_key[ALLEGRO_KEY_D])
		{
			chared_import_ox++;
			t3f_key[ALLEGRO_KEY_D] = 0;
		}
	}
	else
	{
		if(t3f_key[ALLEGRO_KEY_W])
		{
			chared_import_vy--;
		}
		if(t3f_key[ALLEGRO_KEY_S])
		{
			chared_import_vy++;
		}
		if(t3f_key[ALLEGRO_KEY_A])
		{
			chared_import_vx--;
		}
		if(t3f_key[ALLEGRO_KEY_D])
		{
			chared_import_vx++;
		}
	}
	if(t3f_mouse_button[0])
	{
		if(chared_import_step == 0)
		{
			chared_import_ox = t3f_mouse_x + chared_import_vx;
			chared_import_oy = t3f_mouse_y + chared_import_vy;
			chared_import_step = 1;
		}
		else if(chared_import_step == 1)
		{
			chared_import_ow = t3f_mouse_x - chared_import_ox + chared_import_vx;
			chared_import_oh = t3f_mouse_y - chared_import_oy + chared_import_vy;
		}
	}
	else
	{
		if(chared_import_step == 1)
		{
			if(chared_import_ow < 0)
			{
				chared_import_ox += chared_import_ow;
				chared_import_ow = -chared_import_ow;
			}
			if(chared_import_oh < 0)
			{
				chared_import_oy += chared_import_oh;
				chared_import_oh = -chared_import_oh;
			}
		}
		chared_import_step = 0;
	}
}

void chared_meta_logic(void)
{
	int input = 0;
	
	if(t3f_key[ALLEGRO_KEY_LEFT])
	{
		chared_selected_meta--;
		if(chared_selected_meta < 0)
		{
			chared_selected_meta = 2;
		}
		chared_get_entry_pos();
		t3f_key[ALLEGRO_KEY_LEFT] = 0;
	}
	if(t3f_key[ALLEGRO_KEY_RIGHT])
	{
		chared_selected_meta++;
		if(chared_selected_meta > 2)
		{
			chared_selected_meta = 2;
		}
		chared_get_entry_pos();
		t3f_key[ALLEGRO_KEY_RIGHT] = 0;
	}
	
	input = t3f_read_key(0);
	if(input)
	{
		if(input == '\b' || input == 127)
		{
			if(chared_entry_pos > 0)
			{
				chared_entry_pos--;
				chared_entry_pointer[chared_entry_pos] = '\0';
			}
		}
		else if(input >= 32 && input < 127)
		{
			chared_entry_pointer[chared_entry_pos] = input;
			chared_entry_pos++;
			chared_entry_pointer[chared_entry_pos] = '\0';
		}
	}
}

void chared_logic(void)
{
	if(t3f_key[ALLEGRO_KEY_F2])
	{
		chared_file_save_dialog = al_create_native_file_dialog(chared_start_path, "Save Character", "*.p2c;*.ppc", ALLEGRO_FILECHOOSER_SAVE);
		al_show_native_file_dialog(t3f_display, chared_file_save_dialog);
		if(al_get_native_file_dialog_count(chared_file_save_dialog) > 0)
		{
			chared_return_path_cstr = al_get_native_file_dialog_path(chared_file_save_dialog, 0);
			pp2_save_character(chared_character, chared_return_path_cstr);
			strcpy(chared_start_path, chared_return_path_cstr);
		}
		al_destroy_native_file_dialog(chared_file_save_dialog);
		t3f_key[ALLEGRO_KEY_F2] = 0;
	}
	if(t3f_key[ALLEGRO_KEY_F3])
	{
		chared_file_load_dialog = al_create_native_file_dialog(chared_start_path, "Load Character", "*.p2c;*.ppc", 0);
		al_show_native_file_dialog(t3f_display, chared_file_load_dialog);
		if(al_get_native_file_dialog_count(chared_file_load_dialog) > 0)
		{
			chared_return_path_cstr = al_get_native_file_dialog_path(chared_file_load_dialog, 0);
			if(!strcmp(al_get_path_extension(chared_replace_path(chared_return_path_cstr)), ".ppc"))
			{
				chared_character = pp2_load_legacy_character((char *)al_path_cstr(chared_replace_path(chared_return_path_cstr), '/'), 0);
			}
			else if(!strcmp(al_get_path_extension(chared_replace_path(chared_return_path_cstr)), ".p2c"))
			{
				chared_character = pp2_load_character((char *)al_path_cstr(chared_replace_path(chared_return_path_cstr), '/'), 0);
			}
			strcpy(chared_start_path, chared_return_path_cstr);
		}
		al_destroy_native_file_dialog(chared_file_load_dialog);
		t3f_key[ALLEGRO_KEY_F3] = 0;
	}
	if(t3f_key[ALLEGRO_KEY_F4])
	{
		if(chared_character)
		{
			pp2_destroy_character(chared_character);
		}
		chared_character = pp2_create_character();
		t3f_key[ALLEGRO_KEY_F4] = 0;
	}
	if(t3f_key[ALLEGRO_KEY_F10])
	{
		int i;
		
		chared_character->flags = 0;
		for(i = 0; i < 80; i++)
		{
			chared_character->state[i].paintball.cx = chared_character->animation[chared_character->state[chared_selected_state].paintball.animation]->frame[0]->width / 2;
			chared_character->state[i].paintball.cy = chared_character->animation[chared_character->state[chared_selected_state].paintball.animation]->frame[0]->height / 2;
		}
		t3f_key[ALLEGRO_KEY_F10] = 0;
	}
	if(t3f_key[ALLEGRO_KEY_F11])
	{
		int i;
		
		for(i = 1; i < 80; i++)
		{
			chared_character->state[i].collision_x = chared_character->state[chared_selected_state].collision_x;
			chared_character->state[i].collision_y = chared_character->state[chared_selected_state].collision_y;
			chared_character->state[i].collision_w = chared_character->state[chared_selected_state].collision_w;
			chared_character->state[i].collision_h = chared_character->state[chared_selected_state].collision_h;
		}
		t3f_key[ALLEGRO_KEY_F11] = 0;
	}
	if(t3f_key[ALLEGRO_KEY_F12])
	{
		int i, j, k;
		
		for(i = 1; i < 10; i++)
		{
			for(j = 0; j < 8; j++)
			{
				for(k = 0; k < chared_character->state[j].pieces; k++)
				{
					memcpy(&chared_character->state[i * 8 + j].piece[k], &chared_character->state[j].piece[k], sizeof(PP2_CHARACTER_PIECE));
				}
				chared_character->state[i * 8 + j].pieces = chared_character->state[j].pieces;
				memcpy(&chared_character->state[i * 8 + j].paintball, &chared_character->state[j].paintball, sizeof(PP2_CHARACTER_PIECE));
			}
		}
		t3f_key[ALLEGRO_KEY_F12] = 0;
	}
	if(t3f_key[ALLEGRO_KEY_TAB])
	{
		chared_view++;
		if(chared_view > 3)
		{
			chared_view = 0;
		}
		if(chared_view == CHARED_VIEW_META)
		{
			chared_get_entry_pos();
		}
		t3f_key[ALLEGRO_KEY_TAB] = 0;
	}
	switch(chared_view)
	{
		case CHARED_VIEW_ANIMATIONS:
		{
			chared_animations_logic();
			break;
		}
		case CHARED_VIEW_CHARACTER:
		{
			chared_character_logic();
			break;
		}
		case CHARED_VIEW_BITMAP:
		{
			chared_bitmap_logic();
			break;
		}
		case CHARED_VIEW_META:
		{
			chared_meta_logic();
			break;
		}
	}
	chared_tick++;
}

void chared_animations_render(void)
{
	al_draw_textf(chared_font, t3f_color_white, 0.0, 0.0, 0, "View: Animations");
	if(chared_selected_animation < chared_character->animations)
	{
		t3f_draw_animation(chared_character->animation[chared_selected_animation], al_map_rgba(255, 255, 255, 255), chared_tick, 32.0, 32.0, 0.0, 0);
		al_draw_textf(chared_font, al_map_rgba(255, 255, 255, 255), 0.0, 216.0, 0, "Animation: %d of %d", chared_selected_animation + 1, chared_character->animations);
	}
}

void chared_character_render(void)
{
	int i;
	float ox, oy;
	
	/* render the main character view */
	al_draw_textf(chared_font, t3f_color_white, 0.0, 0.0, 0, "View: Character");
	al_draw_line(0.5, 128.5, 128.5, 128.5, al_map_rgba_f(1.0, 0.0, 0.0, 0.5), 1.0);
	al_draw_line(64.5, 0.5, 64.5, 128.5, al_map_rgba_f(1.0, 0.0, 0.0, 0.5), 1.0);
	for(i = 0; i < chared_character->state[chared_selected_state].pieces; i++)
	{
		if(i == chared_selected_piece)
		{
			t3f_draw_rotated_animation(chared_character->animation[chared_character->state[chared_selected_state].piece[i].animation], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), chared_tick, chared_character->state[chared_selected_state].piece[i].cx, chared_character->state[chared_selected_state].piece[i].cy, chared_character->state[chared_selected_state].piece[i].x, chared_character->state[chared_selected_state].piece[i].y, 0.0, chared_character->state[chared_selected_state].piece[i].angle, chared_character->state[chared_selected_state].piece[i].flags);
		}
		else
		{
			t3f_draw_rotated_animation(chared_character->animation[chared_character->state[chared_selected_state].piece[i].animation], al_map_rgba(255, 255, 255, 128), chared_tick, chared_character->state[chared_selected_state].piece[i].cx, chared_character->state[chared_selected_state].piece[i].cy, chared_character->state[chared_selected_state].piece[i].x, chared_character->state[chared_selected_state].piece[i].y, 0.0, chared_character->state[chared_selected_state].piece[i].angle, chared_character->state[chared_selected_state].piece[i].flags);
		}
	}
	t3f_draw_rotated_animation(chared_character->animation[chared_character->state[chared_selected_state].paintball.animation], al_map_rgba(255, 255, 255, 128), chared_tick, chared_character->state[chared_selected_state].paintball.cx, chared_character->state[chared_selected_state].paintball.cy, chared_character->state[chared_selected_state].paintball.x + chared_character->state[chared_selected_state].paintball.cx, chared_character->state[chared_selected_state].paintball.y + chared_character->state[chared_selected_state].paintball.cy, 0.0, chared_angle_table[chared_selected_state], 0);
	al_draw_rectangle(chared_character->state[chared_selected_state].collision_x + 0.5, chared_character->state[chared_selected_state].collision_y + 0.5, chared_character->state[chared_selected_state].collision_x + chared_character->state[chared_selected_state].collision_w - 1.0 + 0.5, chared_character->state[chared_selected_state].collision_y + chared_character->state[chared_selected_state].collision_h - 1.0 + 0.5, al_map_rgba_f(0.0, 0.5, 0.0, 0.5), 1.0);
	ox = chared_character->animation[chared_character->state[chared_selected_state].paintball.animation]->frame[0]->width / 2 - chared_character->paintball_size;
	oy = chared_character->animation[chared_character->state[chared_selected_state].paintball.animation]->frame[0]->height / 2 - chared_character->paintball_size;
	al_draw_rectangle(chared_character->state[chared_selected_state].paintball.x + ox + 0.5, chared_character->state[chared_selected_state].paintball.y + oy + 1.0 + 0.5, chared_character->state[chared_selected_state].paintball.x + ox + chared_character->paintball_size * 2.0 - 1.0 + 0.5, chared_character->state[chared_selected_state].paintball.y + oy + chared_character->paintball_size * 2.0 - 1.0 + 0.5, al_map_rgba_f(0.0, 0.5, 0.0, 0.5), 1.0);
	al_draw_pixel(chared_character->state[chared_selected_state].paintball.x + chared_character->state[chared_selected_state].paintball.cx + 0.5, chared_character->state[chared_selected_state].paintball.y + chared_character->state[chared_selected_state].paintball.cy + 0.5, al_map_rgba_f(0.5, 0.0, 0.0, 0.5));
	al_draw_textf(chared_font, al_map_rgba(255, 255, 255, 255), 0.0, 216.0, 0, "%s", chared_state_name[chared_selected_state]);
}

void chared_bitmap_render(void)
{
	int i;
	
	if(chared_import_bitmap)
	{
		al_draw_bitmap(chared_import_bitmap, -chared_import_vx,  -chared_import_vy, 0);
		if(chared_import_ow > 0)
		{
			al_draw_rectangle(chared_import_ox - chared_import_vx + 0.5, chared_import_oy - chared_import_vy + 0.5, chared_import_ox + chared_import_ow - chared_import_vx - 1 + 0.5, chared_import_oy + chared_import_oh - chared_import_vy - 1 + 0.5, al_map_rgba_f(0.0, 0.5, 0.0, 0.5), 1.0);
		}
	}
	for(i = 0; i < chared_character->animation[chared_selected_animation]->frames; i++)
	{
		if(chared_character->animation[chared_selected_animation]->frame[i]->bitmap == chared_selected_bitmap)
		{
			al_draw_line(640.0 - 64.0 + 0.5, 480.0 - 64.0 + 0.5, 640.0 - 64.0 + 32.0 + 0.5, 480.0 - 64.0 + 0.5, al_map_rgba_f(1.0, 0.0, 0.0, 1.0), 1.0);
			al_draw_line(640.0 - 64.0 + 0.5, 480.0 - 64.0 + 0.5, 640.0 - 64.0 + 0.5, 480.0 - 64.0 + 16.0 + 0.5, al_map_rgba_f(1.0, 0.0, 0.0, 1.0), 1.0);
			al_draw_bitmap(chared_character->animation[chared_selected_animation]->bitmap[chared_selected_bitmap], 640 - 64 + chared_character->animation[chared_selected_animation]->frame[i]->x, 480 - 64 + chared_character->animation[chared_selected_animation]->frame[i]->y, 0);
			break;
		}
	}
	if(chared_import_animation[chared_selected_animation])
	{
		t3f_draw_animation(chared_import_animation[chared_selected_animation], al_map_rgba_f(1.0, 1.0, 1.0, 0.5), chared_tick, 640 - 128, 480 - 64, 0, 0);
	}
	al_draw_line(640.0 - 128.0 + 0.5, 480.0 - 64.0 + 0.5, 640.0 - 128.0 + 32.0 + 0.5, 480.0 - 64.0 + 0.5, al_map_rgba_f(1.0, 0.0, 0.0, 1.0), 1.0);
	al_draw_line(640.0 - 128.0 + 0.5, 480.0 - 64.0 + 0.5, 640.0 - 128.0 + 0.5, 480.0 - 64.0 + 16.0 + 0.5, al_map_rgba_f(1.0, 0.0, 0.0, 1.0), 1.0);
	t3f_draw_animation(chared_character->animation[chared_selected_animation], t3f_color_white, chared_tick, 640 - 128, 480 - 64, 0, 0);
}

void chared_meta_render(void)
{
	switch(chared_selected_meta)
	{
		case 0:
		{
			al_draw_textf(chared_font, t3f_color_white, 0, 0, 0, "Name: %s", chared_character->info.name);
			break;
		}
		case 1:
		{
			al_draw_textf(chared_font, t3f_color_white, 0, 0, 0, "Author: %s", chared_character->info.author);
			break;
		}
		case 2:
		{
			al_draw_textf(chared_font, t3f_color_white, 0, 0, 0, "Comment: %s", chared_character->info.comment);
			break;
		}
	}
}

void chared_render(void)
{
	if(chared_character)
	{
		al_clear_to_color(al_map_rgb(0, 0, 64));
		switch(chared_view)
		{
			case CHARED_VIEW_ANIMATIONS:
			{
				chared_animations_render();
				break;
			}
			case CHARED_VIEW_CHARACTER:
			{
				chared_character_render();
				break;
			}
			case CHARED_VIEW_BITMAP:
			{
				chared_bitmap_render();
				break;
			}
			case CHARED_VIEW_META:
			{
				chared_meta_render();
				break;
			}
		}
	}
	else
	{
		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_draw_textf(chared_font, t3f_color_white, 0.0, 0.0, 0, "No character loaded...");
	}
}

bool chared_initialize(int argc, char * argv[])
{
	if(!t3f_initialize("Character Builder", 640, 480, 60.0, chared_logic, chared_render, T3F_DEFAULT | T3F_USE_MOUSE))
	{
		return false;
	}
	al_set_new_bitmap_flags(ALLEGRO_NO_PREMULTIPLIED_ALPHA);
	al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
	pp2_legacy_load_palette("data/graphics/legacy_palette.png");
	chared_font = al_load_bitmap_font("data/fonts/chared_font.png");
	chared_path = al_create_path(NULL);
	chared_import_config = al_load_config_file("import.ini");
	if(!chared_import_config)
	{
		chared_import_config = al_create_config();
	}
	return true;
}

int main(int argc, char * argv[])
{
	if(!chared_initialize(argc, argv))
	{
		printf("Initialization failed!\n");
		return -1;
	}
	t3f_run();
	al_save_config_file("import.ini", chared_import_config);
	return 0;
}
