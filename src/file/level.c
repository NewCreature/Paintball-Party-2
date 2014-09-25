#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include "../t3f/animation.h"
#include "../t3f/tilemap.h"
#include "../t3f/collision.h"
#include "level.h"

PP2_LEVEL * pp2_create_level(void)
{
	PP2_LEVEL * lp;
	
	lp = malloc(sizeof(PP2_LEVEL));
	if(!lp)
	{
		return NULL;
	}
	memset(lp, 0, sizeof(PP2_LEVEL));
	return lp;
}

void pp2_destroy_level(PP2_LEVEL * lp)
{
	int i;
	
	if(lp->tileset)
	{
		t3f_destroy_tileset(lp->tileset);
	}
	if(lp->tilemap)
	{
		for(i = 0; i < lp->tilemap->layers; i++)
		{
			if(lp->collision_tilemap[i])
			{
				t3f_destroy_collision_tilemap(lp->collision_tilemap[i]);
			}
		}
		t3f_destroy_tilemap(lp->tilemap);
	}
	if(lp->info.preview)
	{
		al_destroy_bitmap(lp->info.preview);
	}
	free(lp);
}

PP2_LEVEL * pp2_load_old_level_f(ALLEGRO_FILE * fp, const char * fn, int flags)
{
	int i, sl;
	PP2_LEVEL * lp;
	T3F_COLLISION_TILEMAP * ctp = NULL;
	char header[16];
	
	lp = pp2_create_level();
	if(!lp)
	{
		return NULL;
	}
	al_fread(fp, header, 16);
	if(strcmp(header, "PP2LEVEL"))
	{
		pp2_destroy_level(lp);
		return NULL;
	}
	switch(header[15])
	{
		case 0:
		{
			
			/* read meta data */
			al_fread(fp, lp->info.name, 128);
			al_fread(fp, lp->info.author, 128);
			al_fread(fp, lp->info.comment, 256);
			if(al_fgetc(fp))
			{
				lp->info.preview = al_load_bitmap_f(fp, ".png");
			}
			else
			{
				lp->info.preview = NULL;
			}
			
			lp->tileset = t3f_load_tileset_f(fp, fn);
			lp->tilemap = t3f_load_tilemap_f(fp);
			
			ctp = t3f_load_collision_tilemap_f(fp);
			if(al_fgetc(fp))
			{
				lp->bg = t3f_load_animation_f(fp, fn);
			}
			else
			{
				lp->bg = NULL;
			}
			if(al_fgetc(fp))
			{
				lp->fg = t3f_load_animation_f(fp, fn);
			}
			else
			{
				lp->fg = NULL;
			}
			
			lp->objects = al_fread32le(fp);
			for(i = 0; i < lp->objects; i++)
			{
				lp->object[i].x = al_fread32le(fp);
				lp->object[i].y = al_fread32le(fp);
				lp->object[i].type = al_fread32le(fp);
				lp->object[i].flags = al_fread32le(fp);
			}
			
			lp->platforms.animations = al_fread32le(fp);
			for(i = 0; i < lp->platforms.animations; i++)
			{
				lp->platforms.animation[i] = t3f_load_animation_f(fp, fn);
			}
			
			lp->platforms.platforms = al_fread32le(fp);
			for(i = 0; i < lp->platforms.platforms; i++)
			{
				lp->platforms.platform[i].x = al_fread32le(fp);
				lp->platforms.platform[i].y = al_fread32le(fp);
				lp->platforms.platform[i].type = al_fread32le(fp);
				lp->platforms.platform[i].animation = al_fread32le(fp);
				lp->platforms.platform[i].flags = al_fread32le(fp);
			}
			
			sl = al_fgetc(fp);
			
			/* temporary code, remove once all levels have been resaved */
			for(i = 0; i < lp->objects; i++)
			{
				lp->object[i].layer = sl;
			}
			lp->collision_tilemap[sl] = ctp;
			
			lp->room.x = al_fread32le(fp);
			lp->room.y = al_fread32le(fp);
			lp->room.bx = al_fread32le(fp);
			lp->room.by = al_fread32le(fp);
			
			lp->flags = al_fread32le(fp);
			
			break;
		}
	}
	if(flags)
	{
		t3f_atlas_tileset(lp->tileset);
	}
	return lp;
}

PP2_LEVEL * pp2_load_old_level(const char * fn, int flags)
{
	ALLEGRO_FILE * fp;
	PP2_LEVEL * lp;
	
	fp = al_fopen(fn, "rb");
	if(!fp)
	{
		return NULL;
	}
	lp = pp2_load_old_level_f(fp, fn, flags);
	al_fclose(fp);
	return lp;
}

PP2_LEVEL * pp2_load_level_f(ALLEGRO_FILE * fp, const char * fn, int flags)
{
	int i, l;
	PP2_LEVEL * lp;
	char header[16];
	
	lp = pp2_create_level();
	if(!lp)
	{
		return NULL;
	}
	al_fread(fp, header, 16);
	if(strcmp(header, "PP2LEVEL"))
	{
		pp2_destroy_level(lp);
		return NULL;
	}
	switch(header[15])
	{
		case 0:
		{
			
			/* read meta data */
			al_fread(fp, lp->info.name, 128);
			al_fread(fp, lp->info.author, 128);
			al_fread(fp, lp->info.comment, 256);
			if(al_fgetc(fp))
			{
				lp->info.preview = al_load_bitmap_f(fp, ".png");
			}
			else
			{
				lp->info.preview = NULL;
			}
			
			lp->tileset = t3f_load_tileset_f(fp, fn);
			lp->tilemap = t3f_load_tilemap_f(fp);
			
			for(i = 0; i < lp->tilemap->layers; i++)
			{
				if(al_fgetc(fp))
				{
					lp->collision_tilemap[i] = t3f_load_collision_tilemap_f(fp);
				}
			}
			
			if(al_fgetc(fp))
			{
				lp->bg = t3f_load_animation_f(fp, fn);
			}
			else
			{
				lp->bg = NULL;
			}
			if(al_fgetc(fp))
			{
				lp->fg = t3f_load_animation_f(fp, fn);
			}
			else
			{
				lp->fg = NULL;
			}
			
			lp->objects = al_fread32le(fp);
			for(i = 0; i < lp->objects; i++)
			{
				lp->object[i].x = al_fread32le(fp);
				lp->object[i].y = al_fread32le(fp);
				lp->object[i].layer = al_fread32le(fp);
				lp->object[i].type = al_fread32le(fp);
				lp->object[i].flags = al_fread32le(fp);
			}
			
			lp->script_objects = al_fread32le(fp);
			for(i = 0; i < lp->script_objects; i++)
			{
				lp->script_object[i].x = al_fread32le(fp);
				lp->script_object[i].y = al_fread32le(fp);
				lp->script_object[i].layer = al_fread32le(fp);
				lp->script_object[i].type = al_fread32le(fp);
				lp->script_object[i].flags = al_fread32le(fp);
				l = al_fread32le(fp);
				al_fread(fp, lp->script_object[i].script, l);
			}

			lp->platforms.animations = al_fread32le(fp);
			for(i = 0; i < lp->platforms.animations; i++)
			{
				lp->platforms.animation[i] = t3f_load_animation_f(fp, fn);
			}
			
			lp->platforms.platforms = al_fread32le(fp);
			for(i = 0; i < lp->platforms.platforms; i++)
			{
				lp->platforms.platform[i].x = al_fread32le(fp);
				lp->platforms.platform[i].y = al_fread32le(fp);
				lp->platforms.platform[i].type = al_fread32le(fp);
				lp->platforms.platform[i].animation = al_fread32le(fp);
				lp->platforms.platform[i].flags = al_fread32le(fp);
			}
			
			lp->room.x = al_fread32le(fp);
			lp->room.y = al_fread32le(fp);
			lp->room.bx = al_fread32le(fp);
			lp->room.by = al_fread32le(fp);
			
			lp->flags = al_fread32le(fp);
			
			break;
		}
	}
	if(flags)
	{
		t3f_atlas_tileset(lp->tileset);
	}
	return lp;
}

PP2_LEVEL * pp2_load_level(const char * fn, int flags)
{
	ALLEGRO_FILE * fp;
	PP2_LEVEL * lp;
	
	fp = al_fopen(fn, "rb");
	if(!fp)
	{
		return NULL;
	}
	lp = pp2_load_level_f(fp, fn, flags);
	al_fclose(fp);
	return lp;
}

int pp2_save_level_f(PP2_LEVEL * lp, ALLEGRO_FILE * fp)
{
	int i, l;
	char header[16] = {0};
	
	strcpy(header, "PP2LEVEL");
	header[15] = 0;
	
	al_fwrite(fp, header, 16);
	
	/* read meta data */
	al_fwrite(fp, lp->info.name, 128);
	al_fwrite(fp, lp->info.author, 128);
	al_fwrite(fp, lp->info.comment, 256);
	if(lp->info.preview)
	{
		al_fputc(fp, 1);
		al_save_bitmap_f(fp, ".png", lp->info.preview);
	}
	else
	{
		al_fputc(fp, 0);
	}
			
	t3f_save_tileset_f(lp->tileset, fp);
	t3f_save_tilemap_f(lp->tilemap, fp);
	for(i = 0; i < lp->tilemap->layers; i++)
	{
		if(lp->collision_tilemap[i])
		{
			al_fputc(fp, 1);
			t3f_save_collision_tilemap_f(lp->collision_tilemap[i], fp);
		}
		else
		{
			al_fputc(fp, 0);
		}
	}
	if(lp->bg)
	{
		al_fputc(fp, 1);
		t3f_save_animation_f(lp->bg, fp);
	}
	else
	{
		al_fputc(fp, 0);
	}
	if(lp->fg)
	{
		al_fputc(fp, 1);
		t3f_save_animation_f(lp->fg, fp);
	}
	else
	{
		al_fputc(fp, 0);
	}
	
	al_fwrite32le(fp, lp->objects);
	for(i = 0; i < lp->objects; i++)
	{
		al_fwrite32le(fp, lp->object[i].x);
		al_fwrite32le(fp, lp->object[i].y);
		al_fwrite32le(fp, lp->object[i].layer);
		al_fwrite32le(fp, lp->object[i].type);
		al_fwrite32le(fp, lp->object[i].flags);
	}
	
	al_fwrite32le(fp, lp->script_objects);
	for(i = 0; i < lp->script_objects; i++)
	{
		al_fwrite32le(fp, lp->script_object[i].x);
		al_fwrite32le(fp, lp->script_object[i].y);
		al_fwrite32le(fp, lp->script_object[i].layer);
		al_fwrite32le(fp, lp->script_object[i].type);
		al_fwrite32le(fp, lp->script_object[i].flags);
		l = strlen(lp->script_object[i].script);
		al_fwrite32le(fp, l);
		al_fwrite(fp, lp->script_object[i].script, l);
	}
	
	al_fwrite32le(fp, lp->platforms.animations);
	for(i = 0; i < lp->platforms.platforms; i++)
	{
		t3f_save_animation_f(lp->platforms.animation[i], fp);
	}
	
	al_fwrite32le(fp, lp->platforms.platforms);
	for(i = 0; i < lp->platforms.platforms; i++)
	{
		al_fwrite32le(fp, lp->platforms.platform[i].x);
		al_fwrite32le(fp, lp->platforms.platform[i].y);
		al_fwrite32le(fp, lp->platforms.platform[i].type);
		al_fwrite32le(fp, lp->platforms.platform[i].animation);
		al_fwrite32le(fp, lp->platforms.platform[i].flags);
	}
	
	al_fwrite32le(fp, lp->room.x);
	al_fwrite32le(fp, lp->room.y);
	al_fwrite32le(fp, lp->room.bx);
	al_fwrite32le(fp, lp->room.by);
	
	al_fwrite32le(fp, lp->flags);
	return 1;
}

int pp2_save_level(PP2_LEVEL * lp, const char * fn)
{
	ALLEGRO_FILE * fp;
	
	fp = al_fopen(fn, "wb");
	if(!fp)
	{
		return 0;
	}
	pp2_save_level_f(lp, fp);
	al_fclose(fp);
	return 1;
}

/* ensure the player has now way to "escape" the level by going past the edge,
 * used for fixing legacy levels */
void pp2_fortify_level(PP2_LEVEL * lp)
{
	int i, j;
	
	for(i = 0; i < lp->collision_tilemap[lp->object[0].layer]->width; i++)
	{
		for(j = lp->collision_tilemap[lp->object[0].layer]->height - 1; j >= 0; j--)
		{
			if(lp->collision_tilemap[lp->object[0].layer]->data[j][i].flags & T3F_COLLISION_FLAG_SOLID_TOP)
			{
				lp->collision_tilemap[lp->object[0].layer]->data[j][i].flags |= T3F_COLLISION_FLAG_SOLID_BOTTOM;
				break;
			}
		}
	}
}
