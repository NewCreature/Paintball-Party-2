#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include "../t3f/animation.h"
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

T3F_ANIMATION * pp2_legacy_load_ani_fp(ALLEGRO_FILE * fp, void * pal)
{
    T3F_ANIMATION * ap;
    char header[4];
    int i, j, k;
    int w, h, f, d;
    ALLEGRO_STATE old_state;

    /* check format */
    al_fread(fp, header, 4);
    if(header[0] != 'A' || header[1] != 'N' || header[2] != 'I' || header[3] != 21)
    {
        return NULL;
    }

    ap = t3f_create_animation();

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
	al_store_state(&old_state, ALLEGRO_STATE_TARGET_BITMAP);
	ap->bitmaps->count = f;
    for(i = 0; i < f; i++)
    {
        ap->bitmaps->bitmap[i] = al_create_bitmap(w, h);
		if(ap->bitmaps->bitmap[i])
		{
	        al_lock_bitmap(ap->bitmaps->bitmap[i], al_get_bitmap_format(ap->bitmaps->bitmap[i]), ALLEGRO_LOCK_WRITEONLY);
	        al_set_target_bitmap(ap->bitmaps->bitmap[i]);
	        for(j = 0; j < h; j++)
	        {
	            for(k = 0; k < w; k++)
	            {
		            al_put_pixel(k, j, pp2_legacy_get_color(al_fgetc(fp), pp2_legacy_color_type));
	            }
	        }
	        al_unlock_bitmap(ap->bitmaps->bitmap[i]);
	        t3f_animation_add_frame(ap, i, 0, 0, 0, w * 2, h * 2, 0, d > 0 ? d : 1, 0);
		}
    }
	al_restore_state(&old_state);

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
	ap = pp2_legacy_load_ani_fp(fp, NULL);
	al_fclose(fp);
	return ap;
}
