#include "t3f/t3f.h"
#include "t3f/draw.h"
#include "../file/wav.h"
#include "../file/character.h"
#include "../file/level.h"
#include "../legacy/character.h"
#include "../legacy/level.h"
#include "../data.h"
#include "../init.h"
#include "preview.h"

PP2_CHARACTER_PREVIEW * pp2_load_character_preview(const char * fn)
{
	ALLEGRO_FILE * fp;
	PP2_CHARACTER_PREVIEW * cp = NULL;
	char header[16] = {0};
	int i;
	
	fp = al_fopen(fn, "rb");
	if(fp)
	{
		cp = malloc(sizeof(PP2_CHARACTER_PREVIEW));
		if(cp)
		{
			al_fread(fp, header, 16);
			
			/* incorrect file format */
			if(strcmp(header, "P2CP"))
			{
				al_fclose(fp);
				return NULL;
			}
			
			switch(header[15])
			{
				case 0:
				{
					al_fread(fp, cp->name, 256);
					al_fread(fp, cp->author, 256);
					al_fread(fp, cp->comment, 1024);
					cp->animations = al_fread16le(fp);
					for(i = 0; i < cp->animations; i++)
					{
						cp->animation[i] = t3f_load_animation_f(fp, fn);
						if(!cp->animation[i])
						{
							al_fclose(fp);
							return NULL;
						}
					}
					cp->pieces = al_fread16le(fp);
					for(i = 0; i < cp->pieces; i++)
					{
						cp->piece[i].animation = al_fread16le(fp);
						cp->piece[i].x = t3f_fread_float(fp);
						cp->piece[i].y = t3f_fread_float(fp);
						cp->piece[i].cx = t3f_fread_float(fp);
						cp->piece[i].cy = t3f_fread_float(fp);
						cp->piece[i].angle = t3f_fread_float(fp);
						cp->piece[i].flags = al_fread32le(fp);
					}
					if(al_fgetc(fp))
					{
						cp->sound = pp2_load_wav_f(fp);
					}
					else
					{
						cp->sound = NULL;
					}
					break;
				}
				case 1:
				{
					al_fread(fp, cp->name, 256);
					al_fread(fp, cp->author, 256);
					al_fread(fp, cp->comment, 1024);
					cp->animations = al_fread16le(fp);
					for(i = 0; i < cp->animations; i++)
					{
						cp->animation[i] = t3f_load_animation_f(fp, fn);
						if(!cp->animation[i])
						{
							al_fclose(fp);
							return NULL;
						}
					}
					cp->pieces = al_fread16le(fp);
					for(i = 0; i < cp->pieces; i++)
					{
						cp->piece[i].animation = al_fread16le(fp);
						cp->piece[i].x = t3f_fread_float(fp);
						cp->piece[i].y = t3f_fread_float(fp);
						cp->piece[i].cx = t3f_fread_float(fp);
						cp->piece[i].cy = t3f_fread_float(fp);
						cp->piece[i].angle = t3f_fread_float(fp);
						cp->piece[i].flags = al_fread32le(fp);
					}
					cp->cx = t3f_fread_float(fp);
					cp->cy = t3f_fread_float(fp);
					if(al_fgetc(fp))
					{
						cp->sound = pp2_load_wav_f(fp);
					}
					else
					{
						cp->sound = NULL;
					}
					break;
				}
			}
		}
		al_fclose(fp);
	}
	return cp;
}

bool pp2_create_character_preview_from_character(const char * fn, const char * outfn)
{
	PP2_CHARACTER * cp = NULL;
	PP2_CHARACTER_PREVIEW * pp = NULL;
	int i;
	ALLEGRO_PATH * new_path = NULL;
	char text[1024] = {0};
	
	new_path = al_create_path(fn);
//	al_set_path_extension(new_path, ".preview");
	
	cp = pp2_load_character(fn, 0);
	if(!cp)
	{
		cp = pp2_load_legacy_character(fn, 0);
		if(cp)
		{
			strcpy(cp->info.name, al_get_path_basename(new_path));
			strcpy(cp->info.author, "N/A");
			strcpy(cp->info.comment, "N/A");
		}
	}
	if(cp)
	{
		sprintf(text, "Generating preview: %s", fn);
		pp2_show_load_screen(text);
		pp = malloc(sizeof(PP2_CHARACTER_PREVIEW));
		if(pp)
		{
			pp->animations = 0;
			pp->pieces = 0;
			strcpy(pp->name, cp->info.name);
			strcpy(pp->author, cp->info.author);
			strcpy(pp->comment, cp->info.comment);
			for(i = 0; i < cp->state[PP2_CHARACTER_STATE_WALK_R_R].pieces; i++)
			{
				pp->animation[pp->animations] = t3f_clone_animation(cp->animation[cp->state[PP2_CHARACTER_STATE_WALK_R_R].piece[i].animation]);
				if(!pp->animation[pp->animations])
				{
					return false;
				}
				memcpy(&pp->piece[pp->pieces], &cp->state[PP2_CHARACTER_STATE_WALK_R_R].piece[i], sizeof(PP2_CHARACTER_PIECE));
				pp->piece[pp->pieces].animation = pp->animations;
				pp->animations++;
				pp->pieces++;
			}
			pp->cx = cp->state[0].collision_x + cp->state[0].collision_w / 2;
			pp->cy = cp->state[0].collision_y + cp->state[0].collision_h / 2;
			pp->sound = cp->sample[PP2_SAMPLE_ENTER];
			pp2_save_character_preview(pp, outfn);
			al_destroy_path(new_path);
			pp->sound = NULL;
			pp2_destroy_character(cp);
			pp2_destroy_character_preview(pp);
		}
		return true;
	}
	return false;
}

bool pp2_save_character_preview(PP2_CHARACTER_PREVIEW * cp, const char * fn)
{
	ALLEGRO_FILE * fp;
	char header[16] = {'P', '2', 'C', 'P'};
	int i;
	
	header[15] = 1;
	fp = al_fopen(fn, "wb");
	if(fp)
	{
		al_fwrite(fp, header, 16);
			
		al_fwrite(fp, cp->name, 256);
		al_fwrite(fp, cp->author, 256);
		al_fwrite(fp, cp->comment, 1024);
		al_fwrite16le(fp, cp->animations);
		for(i = 0; i < cp->animations; i++)
		{
			t3f_save_animation_f(cp->animation[i], fp);
		}
		al_fwrite16le(fp, cp->pieces);
		for(i = 0; i < cp->pieces; i++)
		{
			al_fwrite16le(fp, cp->piece[i].animation);
			t3f_fwrite_float(fp, cp->piece[i].x);
			t3f_fwrite_float(fp, cp->piece[i].y);
			t3f_fwrite_float(fp, cp->piece[i].cx);
			t3f_fwrite_float(fp, cp->piece[i].cy);
			t3f_fwrite_float(fp, cp->piece[i].angle);
			al_fwrite32le(fp, cp->piece[i].flags);
		}
		t3f_fwrite_float(fp, cp->cx);
		t3f_fwrite_float(fp, cp->cy);
		if(cp->sound)
		{
			al_fputc(fp, 1);
			pp2_save_wav_f(cp->sound, fp);
		}
		else
		{
			al_fputc(fp, 0);
		}
		al_fclose(fp);
		return true;
	}
	return false;
}

void pp2_destroy_character_preview(PP2_CHARACTER_PREVIEW * pp)
{
	int i;
	
	for(i = 0; i < pp->animations; i++)
	{
		t3f_destroy_animation(pp->animation[i]);
	}
	al_destroy_sample(pp->sound);
	free(pp);
}

void pp2_render_character_preview(PP2_CHARACTER_PREVIEW * pp, ALLEGRO_COLOR tint, float x, float y, float z)
{
	int i;
	for(i = 0; i < pp->pieces; i++)
	{
		t3f_draw_rotated_animation(pp->animation[pp->piece[i].animation], tint, pp2_tick, pp->piece[i].cx, pp->piece[i].cy, x + pp->piece[i].x, y + pp->piece[i].y, z, pp->piece[i].angle, pp->piece[i].flags);
	}
}

PP2_LEVEL_PREVIEW * pp2_load_level_preview(const char * fn)
{
	ALLEGRO_FILE * fp;
	PP2_LEVEL_PREVIEW * cp = NULL;
	char header[16] = {0};
	
	fp = al_fopen(fn, "rb");
	if(fp)
	{
		cp = malloc(sizeof(PP2_LEVEL_PREVIEW));
		if(cp)
		{
			al_fread(fp, header, 16);
			
			/* incorrect file format */
			if(strcmp(header, "P2LP"))
			{
				al_fclose(fp);
				return NULL;
			}
			
			switch(header[15])
			{
				case 0:
				{
					al_fread(fp, cp->name, 256);
					al_fread(fp, cp->author, 256);
					al_fread(fp, cp->comment, 1024);
					cp->players = al_fread16le(fp);
					cp->bitmap = al_load_bitmap_f(fp, ".png");
					break;
				}
			}
		}
		al_fclose(fp);
	}
	return cp;
}

static int pp2_count_level_portals(PP2_LEVEL * lp)
{
	int i;
	int c = 0;
	
	for(i = 0; i < lp->objects; i++)
	{
		if(lp->object[i].type == PP2_OBJECT_PORTAL)
		{
			c++;
		}
	}
	return c;
}

static float pp2_preview_get_speed(T3F_TILEMAP * tmp, int layer, float oz)
{
	return (t3f_project_x(1.0, tmp->layer[layer]->z - oz) - t3f_project_x(0.0, tmp->layer[layer]->z - oz));
}

static float pp2_get_layer_speed(T3F_TILEMAP * tmp, int layer, float oz)
{
	return (t3f_project_x(1.0, tmp->layer[layer]->z - oz) - t3f_project_x(0.0, tmp->layer[layer]->z - oz));
}

static void pp2_preview_render_tilemap(T3F_TILEMAP * tmp, T3F_TILESET * tsp, int layer, int tick, float ox, float oy, float oz, ALLEGRO_COLOR color)
{
	int startx, ostartx;
	int starty, ostarty;
	float fox, foy;
	float tw;
	float th;
	int tx, px;
	int ty, py;
	float zsp = pp2_preview_get_speed(tmp, layer, oz);
	float ziw = (float)tsp->width * tmp->layer[layer]->scale;
	float zih = (float)tsp->height * tmp->layer[layer]->scale;
	float ztp = zsp * ziw;
	float zhp = zsp * zih;
	float sw;
	float sh;
	float cx = (ox * tmp->layer[layer]->speed_x) - tmp->layer[layer]->x;
	float cy = (oy * tmp->layer[layer]->speed_y) - tmp->layer[layer]->y;
	ALLEGRO_STATE old_blender;
	int revert = 0;
	
	sw = t3f_virtual_display_width;
	sh = t3f_virtual_display_height;
	
	/* calculate total visible tiles */
	tw = tmp->layer[layer]->width;
	th = tmp->layer[layer]->height;
	
	/* calculate first visible horizontal tile */
	fox = (cx * zsp) / ztp - ((t3f_project_x(0.0, tmp->layer[layer]->z - oz)) / ztp);
	ostartx = fox;
	if(fox < 0.0)
	{
		ostartx--;
	}
	ostartx--;
	startx = ostartx;
	while(startx < 0)
	{
		startx += tmp->layer[layer]->width;
	}
	while(startx >= tmp->layer[layer]->width)
	{
		startx -= tmp->layer[layer]->width;
	}
	
	/* calculate first visible vertical tile */
	foy = (cy * zsp) / zhp - ((t3f_project_y(0.0, tmp->layer[layer]->z - oz)) / zhp);
	ostarty = foy;
	if(foy < 0.0)
	{
		ostarty--;
	}
	ostarty--;
	starty = ostarty;
	while(starty < 0)
	{
		starty += tmp->layer[layer]->height;
	}
	while(starty >= tmp->layer[layer]->height)
	{
		starty -= tmp->layer[layer]->height;
	}
	
	/* render the tiles */
	ty = ostarty;
	py = starty;
	
	al_store_state(&old_blender, ALLEGRO_STATE_BLENDER);
	if(tmp->layer[layer]->flags & T3F_TILEMAP_LAYER_SOLID)
	{
		al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ZERO);
	}
	if(al_is_bitmap_drawing_held())
	{
		revert = 1;
	}
	else
	{
		al_hold_bitmap_drawing(true);
	}
	while(ty < ostarty + (int)th + 3)
	{
		tx = ostartx;
		px = startx;
		while(tx < ostartx + (int)tw + 3)
		{
			if((tmp->layer[layer]->flags & T3F_TILEMAP_LAYER_SOLID) || tmp->layer[layer]->data[py][px] != 0)
			{
				t3f_draw_scaled_animation(tsp->tile[t3f_get_tile(tsp, tmp->layer[layer]->data[py][px], tick)]->ap, color, tick, tmp->layer[layer]->x + (float)tx * ziw - ox * tmp->layer[layer]->speed_x, tmp->layer[layer]->y + (float)ty * zih - oy * tmp->layer[layer]->speed_y, tmp->layer[layer]->z - oz, tmp->layer[layer]->scale, 0);
			}
			tx++;
			px++;
			if(px >= tmp->layer[layer]->width)
			{
				px = 0;
			}
		}
		ty++;
		py++;
		if(py >= tmp->layer[layer]->height)
		{
			py = 0;
		}
	}
	if(!revert)
	{
		al_hold_bitmap_drawing(false);
	}
	al_restore_state(&old_blender);
}

bool pp2_create_level_preview_from_level(const char * fn, const char * outfn)
{
	PP2_LEVEL * lp;
	PP2_LEVEL_PREVIEW * pp = NULL;
	ALLEGRO_PATH * new_path = NULL;
	char text[1024] = {0};
	
	new_path = al_create_path(fn);
	al_set_path_extension(new_path, ".preview");
	
	lp = pp2_load_level(fn, 0);
	if(!lp)
	{
		lp = pp2_load_legacy_level(fn, 0);
		if(lp)
		{
			strcpy(lp->info.name, al_get_path_basename(new_path));
			strcpy(lp->info.author, "N/A");
			strcpy(lp->info.comment, "N/A");
		}
	}
	if(lp)
	{
		int pw, ph, i, tw, th;
		float vw, vh, tvw, tvh;
		float sx, sy;
		float ox, oy;
		float zs;
		ALLEGRO_STATE old_state;
		ALLEGRO_TRANSFORM transform;
		
		sprintf(text, "Generating preview: %s", fn);
		pp2_show_load_screen(text);
		pp = malloc(sizeof(PP2_LEVEL_PREVIEW));
		if(pp)
		{
			if(lp->info.preview)
			{
				pp->bitmap = al_clone_bitmap(lp->info.preview);
			}
			else
			{
				/* generate thumbnail */
				al_store_state(&old_state, ALLEGRO_STATE_TARGET_BITMAP | ALLEGRO_STATE_BLENDER | ALLEGRO_STATE_TRANSFORM);
				t3f_select_view(t3f_default_view);
				pw = (lp->room.bx - lp->room.x + 1) * lp->tileset->width;
				ph = (lp->room.by - lp->room.y + 1) * lp->tileset->height;
				tw = ((float)pw * (240.0 / (float)ph));
				if(tw > 600)
				{
					tw = 600;
					th = ((float)ph * (600.0 / (float)pw));
				}
				else
				{
					th = 240;
				}
//				al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE);
				pp->bitmap = al_create_bitmap(tw, th);
				if(!pp->bitmap)
				{
					return NULL;
				}
				if(lp->bg)
				{
					al_identity_transform(&transform);
					if((float)tw / (float)th > (float)t3f_virtual_display_width / (float)t3f_virtual_display_height)
					{
						sx = (float)tw / (float)t3f_virtual_display_width;
						ox = 0.0;
						oy = (float)(th / 2) / sx - (float)t3f_virtual_display_height / 2.0;
					}
					else
					{
						sx = (float)th / (float)t3f_virtual_display_height;
						ox = (float)(tw / 2) / sx - (float)t3f_virtual_display_width / 2.0;
						oy = 0.0;
					}
					al_scale_transform(&transform, sx, sx);
					al_use_transform(&transform);
					t3f_draw_animation(lp->bg, t3f_color_white, 0, ox, oy, 0, 0);
				}
				for(i = 0; i < lp->tilemap->layers; i++)
				{
					al_identity_transform(&transform);
					vw = 640.0 + (float)(pw - 640) * lp->tilemap->layer[i]->speed_x;
					if(vw < 640.0)
					{
						vw = 640.0;
					}
					vh = 480.0 + (float)(ph - 480) * lp->tilemap->layer[i]->speed_y;
					if(vh < 480.0)
					{
						vh = 480.0;
					}
					tvw = (float)tw;
					tvh = (float)th;
					zs = pp2_get_layer_speed(lp->tilemap, i, 0.0);
					sx = (tvw / vw) / zs;
					sy = (tvh / vh) / zs;
					al_scale_transform(&transform, sx, sy);
					al_use_transform(&transform);
					pp2_preview_render_tilemap(lp->tilemap, lp->tileset, i, 0, lp->room.x * lp->tileset->width, lp->room.y * lp->tileset->height, 0, t3f_color_white);
				}
				if(lp->fg)
				{
					al_identity_transform(&transform);
					if((float)tw / (float)th > (float)t3f_virtual_display_width / (float)t3f_virtual_display_height)
					{
						sx = (float)tw / (float)t3f_virtual_display_width;
						ox = 0.0;
						oy = (float)(th / 2) / sx - (float)t3f_virtual_display_height / 2.0;
					}
					else
					{
						sx = (float)th / (float)t3f_virtual_display_height;
						ox = (float)(tw / 2) / sx - (float)t3f_virtual_display_width / 2.0;
						oy = 0.0;
					}
					al_scale_transform(&transform, sx, sx);
					al_use_transform(&transform);
					t3f_draw_animation(lp->fg, t3f_color_white, 0, ox, oy, 0, 0);
				}
				al_set_target_bitmap(pp->bitmap);
				al_clear_to_color(al_map_rgba_f(0.0, 0.0, 0.0, 1.0));
				al_draw_bitmap_region(al_get_backbuffer(t3f_display), 0.0, 0.0, tw, th, 0.0, 0.0, 0);
				al_restore_state(&old_state);
			}
			strcpy(pp->name, lp->info.name);
			strcpy(pp->author, lp->info.author);
			strcpy(pp->comment, lp->info.comment);
			pp->players = pp2_count_level_portals(lp);
			pp2_save_level_preview(pp, outfn);
			al_destroy_path(new_path);
			pp2_destroy_level(lp);
			pp2_destroy_level_preview(pp);
		}
		return true;
	}
	return false;
}

bool pp2_save_level_preview(PP2_LEVEL_PREVIEW * pp, const char * fn)
{
	ALLEGRO_FILE * fp;
	char header[16] = {'P', '2', 'L', 'P'};
	
	fp = al_fopen(fn, "wb");
	if(fp)
	{
		al_fwrite(fp, header, 16);
			
		al_fwrite(fp, pp->name, 256);
		al_fwrite(fp, pp->author, 256);
		al_fwrite(fp, pp->comment, 1024);
		al_fwrite16le(fp, pp->players);
		al_save_bitmap_f(fp, ".png", pp->bitmap);
		al_fclose(fp);
		return true;
	}
	return false;
}

void pp2_destroy_level_preview(PP2_LEVEL_PREVIEW * pp)
{
	al_destroy_bitmap(pp->bitmap);
	free(pp);
}

void pp2_render_level_preview(PP2_LEVEL_PREVIEW * pp, ALLEGRO_COLOR tint, float x, float y, float z)
{
	t3f_draw_bitmap(pp->bitmap, tint, x, y, z, 0);
}
