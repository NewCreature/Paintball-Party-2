#include "t3f/t3f.h"
#include "t3f/animation.h"
#include "t3f/file.h"
#include "../file/wav.h"
#include "character.h"

PP2_CHARACTER * pp2_create_character(void)
{
	PP2_CHARACTER * cp;

	cp = malloc(sizeof(PP2_CHARACTER));
	if(!cp)
	{
		return NULL;
	}
	memset(cp, 0, sizeof(PP2_CHARACTER));
	return cp;
}

void pp2_destroy_character(PP2_CHARACTER * cp)
{
	int i;

	for(i = 0; i < cp->animations; i++)
	{
		if(cp->animation[i])
		{
			t3f_destroy_animation(cp->animation[i]);
		}
	}
	if(cp->atlas)
	{
		t3f_destroy_atlas(cp->atlas);
	}
	for(i = 0; i < 64; i++)
	{
		if(cp->sample[i])
		{
			al_destroy_sample(cp->sample[i]);
		}
	}
	free(cp);
}

PP2_CHARACTER * pp2_load_character_f(ALLEGRO_FILE * fp, const char * fn, int flags)
{
	int i, j;
	PP2_CHARACTER * cp;
	char header[16];

	cp = pp2_create_character();
	if(!cp)
	{
		return NULL;
	}
	al_fread(fp, header, 16);
	if(strcmp(header, "PP2CHAR"))
	{
		pp2_destroy_character(cp);
		return NULL;
	}
	switch(header[15])
	{
		case 0:
		{

			/* read meta data */
			al_fread(fp, cp->info.name, 128);
			al_fread(fp, cp->info.author, 128);
			al_fread(fp, cp->info.comment, 256);

			/* load animation data */
			cp->animations = al_fread16le(fp);
			for(i = 0; i < cp->animations; i++)
			{
				cp->animation[i] = t3f_load_animation_f(fp, fn);
			}

			/* load state data */
			for(i = 0; i < PP2_CHARACTER_MAX_STATES; i++)
			{
				cp->state[i].pieces = al_fread16le(fp);
				for(j = 0; j < cp->state[i].pieces; j++)
				{
					cp->state[i].piece[j].animation = al_fread16le(fp);
					cp->state[i].piece[j].x = t3f_fread_float(fp);
					cp->state[i].piece[j].y = t3f_fread_float(fp);
					cp->state[i].piece[j].angle = t3f_fread_float(fp);
					cp->state[i].piece[j].flags = al_fread32le(fp);
				}
				cp->state[i].paintball.animation = al_fread16le(fp);
				cp->state[i].paintball.x = t3f_fread_float(fp);
				cp->state[i].paintball.y = t3f_fread_float(fp);
				cp->state[i].paintball.angle = t3f_fread_float(fp);
				cp->state[i].paintball.cx = cp->animation[cp->state[i].paintball.animation]->frame[0]->width / 2;
				cp->state[i].paintball.cy = cp->animation[cp->state[i].paintball.animation]->frame[0]->height / 2;
				cp->state[i].particle.animation = al_fread16le(fp);
				cp->state[i].particle.x = t3f_fread_float(fp);
				cp->state[i].particle.y = t3f_fread_float(fp);
				cp->state[i].particle.angle = t3f_fread_float(fp);
				cp->state[i].collision_x = al_fread16le(fp);
				cp->state[i].collision_y = al_fread16le(fp);
				cp->state[i].collision_w = al_fread16le(fp);
				cp->state[i].collision_h = al_fread16le(fp);
				cp->state[i].flags = al_fread32le(fp);
			}
			cp->paintball_size = al_fread16le(fp);
			cp->flags = al_fread32le(fp);
			for(i = 0; i < 64; i++)
			{
				if(al_fgetc(fp))
				{
					cp->sample[i] = pp2_load_wav_f(fp);
					if(!cp->sample[i])
					{
//						return NULL;
					}
				}
				else
				{
					cp->sample[i] = NULL;
				}
			}
			break;
		}
	}
	/* create sprite sheet */
	if(flags)
	{
		cp->atlas = t3f_create_atlas(1024, 1024);
		if(cp->atlas)
		{
			for(i = 0; i < cp->animations; i++)
			{
				t3f_add_animation_to_atlas(cp->atlas, cp->animation[i], T3F_ATLAS_SPRITE);
			}
		}
	}
	return cp;
}

PP2_CHARACTER * pp2_load_character(const char * fn, int flags)
{
	ALLEGRO_FILE * fp;
	PP2_CHARACTER * cp;

	fp = al_fopen(fn, "rb");
	if(!fp)
	{
		return NULL;
	}
	cp = pp2_load_character_f(fp, fn, flags);
	al_fclose(fp);
	return cp;
}

int pp2_save_character_f(PP2_CHARACTER * cp, ALLEGRO_FILE * fp)
{
	int i, j;
	char header[16] = {0};

	strcpy(header, "PP2CHAR");
	header[15] = 0;

	al_fwrite(fp, header, 16);

	/* read meta data */
	al_fwrite(fp, cp->info.name, 128);
	al_fwrite(fp, cp->info.author, 128);
	al_fwrite(fp, cp->info.comment, 256);

	/* save animation data */
	al_fwrite16le(fp, cp->animations);
	for(i = 0; i < cp->animations; i++)
	{
		t3f_save_animation_f(cp->animation[i], fp);
	}

	/* save state data */
	for(i = 0; i < PP2_CHARACTER_MAX_STATES; i++)
	{
		al_fwrite16le(fp, cp->state[i].pieces);
		for(j = 0; j < cp->state[i].pieces; j++)
		{
			al_fwrite16le(fp, cp->state[i].piece[j].animation);
			t3f_fwrite_float(fp, cp->state[i].piece[j].x);
			t3f_fwrite_float(fp, cp->state[i].piece[j].y);
			t3f_fwrite_float(fp, cp->state[i].piece[j].angle);
			al_fwrite32le(fp, cp->state[i].piece[j].flags);
		}
		al_fwrite16le(fp, cp->state[i].paintball.animation);
		t3f_fwrite_float(fp, cp->state[i].paintball.x);
		t3f_fwrite_float(fp, cp->state[i].paintball.y);
		t3f_fwrite_float(fp, cp->state[i].paintball.angle);
		al_fwrite16le(fp, cp->state[i].particle.animation);
		t3f_fwrite_float(fp, cp->state[i].particle.x);
		t3f_fwrite_float(fp, cp->state[i].particle.y);
		t3f_fwrite_float(fp, cp->state[i].particle.angle);
		al_fwrite16le(fp, cp->state[i].collision_x);
		al_fwrite16le(fp, cp->state[i].collision_y);
		al_fwrite16le(fp, cp->state[i].collision_w);
		al_fwrite16le(fp, cp->state[i].collision_h);
		al_fwrite32le(fp, cp->state[i].flags);
	}
	al_fwrite16le(fp, cp->paintball_size);
	al_fwrite32le(fp, cp->flags);
	for(i = 0; i < 64; i++)
	{
		if(cp->sample[i])
		{
			al_fputc(fp, 1);
			pp2_save_wav_f(cp->sample[i], fp);
		}
		else
		{
			al_fputc(fp, 0);
		}
	}
	return 1;
}

int pp2_save_character(PP2_CHARACTER * cp, const char * fn)
{
	ALLEGRO_FILE * fp;

	fp = al_fopen(fn, "wb");
	if(!fp)
	{
		return 0;
	}
	pp2_save_character_f(cp, fp);
	al_fclose(fp);
	return 1;
}
