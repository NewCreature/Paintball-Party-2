#include "../t3f/t3f.h"
#include "../t3f/animation.h"
#include "../t3f/resource.h"
#include "../defines.h"
#include "animation.h"


ALLEGRO_COLOR legacy_palette[256];

int pp2_legacy_color_type = PP2_LEGACY_COLOR_SOLID;

static ALLEGRO_COLOR pp2_legacy_get_color(int color, int type)
{
	float r, g, b, a, l;

	switch(type)
	{
		case PP2_LEGACY_COLOR_SOLID:
		{
			return legacy_palette[color];
		}
		case PP2_LEGACY_COLOR_TRANS:
		{
			al_unmap_rgba_f(legacy_palette[color], &r, &g, &b, &a);
			if(color > 0)
			{
				a = 0.5;
			}
			return al_map_rgba_f(r * a, g * a, b * a, a);
		}
		case PP2_LEGACY_COLOR_LIGHT:
		{
			al_unmap_rgba_f(legacy_palette[color], &r, &g, &b, &a);
			l = (r + g + b) / 3.0;
			if(l < 0.5)
			{
				return al_map_rgba_f(0.0, 0.0, 0.0, (0.5 - l) / 0.5);
			}
			else if(l > 0.5)
			{
				return al_map_rgba_f(1.0, 1.0, 1.0, (l - 0.5) / 0.5);
			}
			return legacy_palette[color];
		}
	}
	return legacy_palette[color];
}

bool pp2_legacy_load_palette(char * fn)
{
	ALLEGRO_BITMAP * bp;
	ALLEGRO_STATE old_state;
	int i;
	unsigned char r, g, b;

	al_store_state(&old_state, ALLEGRO_STATE_ALL);
	al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
	bp = al_load_bitmap(fn);
	if(bp)
	{
		for(i = 0; i < 256; i++)
		{
			legacy_palette[i] = al_get_pixel(bp, i % 16, i / 16);
		}
		al_destroy_bitmap(bp);
		al_unmap_rgb(legacy_palette[0], &r, &g, &b);
		legacy_palette[0] = al_map_rgba(r, g, b, 0);
		al_restore_state(&old_state);
		return true;
	}
	al_restore_state(&old_state);
	return false;
}

bool pp2_legacy_character_bitmap_resource_handler_proc(void ** ptr, ALLEGRO_FILE * fp, const char * filename, int option, int flags, unsigned long offset, bool destroy)
{
	ALLEGRO_STATE old_state;
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
				al_lock_bitmap(*ptr, al_get_bitmap_format(*ptr), ALLEGRO_LOCK_WRITEONLY);
				al_set_target_bitmap(*ptr);
				for(j = 0; j < h; j++)
				{
					for(k = 0; k < w; k++)
					{
						al_put_pixel(k, j, pp2_legacy_get_color(al_fgetc(fp), pp2_legacy_color_type));
					}
				}
				al_unlock_bitmap(*ptr);
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
	ap->bitmaps->count = f;
    for(i = 0; i < f; i++)
    {
		t3f_load_resource_f((void **)&ap->bitmaps->bitmap[i], pp2_legacy_character_bitmap_resource_handler_proc, fp, fn, w | (h << 16), 0);
		if(ap->bitmaps->bitmap[i])
		{
	        t3f_animation_add_frame(ap, i, 0, 0, 0, w * 2, h * 2, 0, d > 0 ? d : 1, 0);
		}
		else
		{
			printf("failed to load legacy character bitmap resource\n");
			return NULL;
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
