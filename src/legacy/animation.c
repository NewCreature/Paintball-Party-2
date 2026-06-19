#include "t3f/t3f.h"
#include "pp2/defines.h"
#include "animation.h"
#include "palette.h"
#include "bitmap.h"


bool pp2_legacy_character_bitmap_resource_handler_proc(void ** ptr, ALLEGRO_FILE * fp, const char * filename, int option, int flags, unsigned long offset, bool destroy)
{
	ALLEGRO_STATE old_state;
	ALLEGRO_BITMAP * bitmap = (ALLEGRO_BITMAP *)(*ptr);
	bool openfp = false; // operating on already open file
	int w, h;
	int j, k;


	if(destroy)
	{
		al_destroy_bitmap(*ptr);
		return true;
	}

	w = option & 0xFFFF;
	h = (option >> 16) & 0xFFFF;
	al_store_state(&old_state, ALLEGRO_STATE_NEW_BITMAP_PARAMETERS | ALLEGRO_STATE_TARGET_BITMAP);
	al_set_new_bitmap_flags(al_get_new_bitmap_flags() | ALLEGRO_NO_PRESERVE_TEXTURE);
	if(fp)
	{
		openfp = true;
	}
	if(!openfp && offset == 0)
	{
		fp = al_fopen(filename, "rb");
	}
	else
	{
		if(!openfp)
		{
			fp = al_fopen(filename, "rb");
			al_fseek(fp, offset, ALLEGRO_SEEK_SET);
		}
		if(fp)
		{
			*ptr = al_create_bitmap(w, h);
			if(*ptr)
			{
				bitmap = *ptr;
				al_lock_bitmap(bitmap, al_get_bitmap_format(bitmap), ALLEGRO_LOCK_WRITEONLY);
				al_set_target_bitmap(bitmap);
				for(j = 0; j < h; j++)
				{
					for(k = 0; k < w; k++)
					{
						pp2_legacy_put_pixel(k, j, pp2_legacy_get_color(al_fgetc(fp), -1), 2);
					}
				}
				al_unlock_bitmap(bitmap);
			}
			if(!openfp)
			{
				al_fclose(fp);
			}
		}
	}
	al_restore_state(&old_state);
	return *ptr;
}

T3F_ANIMATION * pp2_legacy_load_ani_fp(ALLEGRO_FILE * fp, const char * fn, void * pal)
{
	T3F_ANIMATION * ap;
	T3F_BITMAP * bitmap = NULL;
	char header[4];
	int i, j;
	int w, h, f, d;

	/* check format */
	al_fread(fp, header, 4);
	if(header[0] != 'A' || header[1] != 'N' || header[2] != 'I' || header[3] != 21)
	{
			return NULL;
	}

	ap = t3f_create_animation();
	if(!ap)
	{
		return NULL;
	}

	/* load header */
	w = al_fread32le(fp);
	h = al_fread32le(fp);
	f = al_fread32le(fp);
	d = al_fread32le(fp);

	/* load palette data */
	j = al_fgetc(fp);
	if(j)
	{
		if(pal != NULL)
		{
			for(i = 0; i < 256; i++)
			{
				al_fgetc(fp);
				al_fgetc(fp);
				al_fgetc(fp);
			}
		}
		else
		{
			for(i = 0; i < 256 * 3; i++)
			{
				al_fgetc(fp);
			}
		}
	}

	/* load animation data */
	ap->data->bitmaps->count = f;
	for(i = 0; i < f; i++)
	{
		ap->data->bitmaps->bitmap[i] = malloc(sizeof(T3F_BITMAP));
		if(ap->data->bitmaps->bitmap[i])
		{
			memset(ap->data->bitmaps->bitmap[i], 0, sizeof(T3F_BITMAP));
			t3f_load_resource_f((void **)&ap->data->bitmaps->bitmap[i]->bitmap, pp2_legacy_character_bitmap_resource_handler_proc, fp, fn, w | (h << 16), 0);
			if(ap->data->bitmaps->bitmap[i])
			{
				t3f_animation_add_frame(ap, i, 0, 0, 0, w * 2, h * 2, 0, d > 0 ? d : 1, 0);
			}
			else
			{
				printf("failed to load legacy character bitmap resource\n");
				return NULL;
			}
		}
	}

	return ap;
}

T3F_ANIMATION * pp2_legacy_load_animation(const char * fn)
{
	T3F_ANIMATION * ap;
	ALLEGRO_FILE * fp;

	fp = al_fopen(fn, "rb");
	if(!fp)
	{
		return NULL;
	}
	ap = pp2_legacy_load_ani_fp(fp, fn, NULL);
	al_fclose(fp);
	return ap;
}
