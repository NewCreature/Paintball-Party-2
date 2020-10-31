#include "t3f/t3f.h"
#include "palette.h"

static ALLEGRO_COLOR legacy_palette[256];

static int pp2_legacy_color_type = PP2_LEGACY_COLOR_SOLID;

void pp2_legacy_set_color_type(int type)
{
  pp2_legacy_color_type = type;
}

void pp2_legacy_set_color(int color, ALLEGRO_COLOR acolor)
{
  legacy_palette[color] = acolor;
}

ALLEGRO_COLOR pp2_legacy_get_color(int color, int type)
{
	float r, g, b, a, l;

  if(type < 0)
  {
    type = pp2_legacy_color_type;
  }
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

bool pp2_legacy_load_palette(const char * fn)
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
