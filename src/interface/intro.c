#include "t3f/t3f.h"
#include "t3f/sound.h"
#include "t3f/draw.h"
#include "../pp2.h"
#include "intro.h"
#include "../data.h"
#include "../file/music.h"
#include "../resource.h"

typedef struct
{

	float x, y, z;
	float vz;
	ALLEGRO_COLOR color;

} PP2_INTRO_PIXEL;

static PP2_INTRO_PIXEL pp2_intro_pixel[1024];
static int pp2_intro_pixels = 0;
static int pp2_intro_pixel_list[1024];
static int pp2_intro_pixel_list_size = 0;
static unsigned long logo_tick = 0;

static ALLEGRO_COLOR alpha_color(ALLEGRO_COLOR color, float alpha)
{
	float r, g, b;

	al_unmap_rgb_f(color, &r, &g, &b);
	return al_map_rgba_f(r * alpha, g * alpha, b * alpha, alpha);
}

static int pixel_sorter(const void * item_1, const void * item_2)
{
	PP2_INTRO_PIXEL * pixel_1 = (PP2_INTRO_PIXEL *)item_1;
	PP2_INTRO_PIXEL * pixel_2 = (PP2_INTRO_PIXEL *)item_2;

	if(pixel_1->z < pixel_2->z)
	{
		return 1;
	}
	else if(pixel_2->z < pixel_1->z)
	{
		return -1;
	}
	return 0;
}

void pp2_intro_setup(PP2_RESOURCES * resources)
{
	int i, j, r;
	ALLEGRO_COLOR color;
	unsigned char cr, cg, cb, ca;

	/* find all pixels */
	for(i = 0; i < 40; i++)
	{
		for(j = 0; j < 40; j++)
		{
			color = al_get_pixel(resources->bitmap[PP2_BITMAP_T3_LOGO_MEMORY], j * 10 + 5, i * 10 + 5);
			al_unmap_rgba(color, &cr, &cg, &cb, &ca);
			if(ca == 255)
			{
				pp2_intro_pixel[pp2_intro_pixels].x = j * 10 + 120;
				pp2_intro_pixel[pp2_intro_pixels].y = i * 10 + 40;
				pp2_intro_pixel[pp2_intro_pixels].color = color;
				pp2_intro_pixel_list[pp2_intro_pixel_list_size] = pp2_intro_pixels;
				pp2_intro_pixel_list_size++;
				pp2_intro_pixels++;
			}
		}
	}

	/* place pixels */
	for(i = 0; i < pp2_intro_pixels; i++)
	{
		r = rand() % pp2_intro_pixel_list_size;
		pp2_intro_pixel[pp2_intro_pixel_list[r]].vz = 5.0 + (i * 0.05);
		pp2_intro_pixel[pp2_intro_pixel_list[r]].z = 0.0;
		for(j = 0; j < 120 + i / 10; j++)
		{
			pp2_intro_pixel[pp2_intro_pixel_list[r]].z -= pp2_intro_pixel[pp2_intro_pixel_list[r]].vz;
		}

		for(j = r; j < pp2_intro_pixel_list_size - 1; j++)
		{
			pp2_intro_pixel_list[j] = pp2_intro_pixel_list[j + 1];
		}
		pp2_intro_pixel_list_size--;
	}
	qsort(pp2_intro_pixel, pp2_intro_pixels, sizeof(PP2_INTRO_PIXEL), pixel_sorter);
}

bool pp2_intro_logic(void * data)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;
	int i;

	for(i = 0; i < pp2_intro_pixels; i++)
	{
		if(pp2_intro_pixel[i].z < 0.0)
		{
			pp2_intro_pixel[i].z += pp2_intro_pixel[i].vz;
			if(pp2_intro_pixel[i].z > 0.0)
			{
				pp2_intro_pixel[i].z = 0.0;
			}
		}
	}

	logo_tick++;
	if(logo_tick == 180)
	{
		t3f_play_sample(instance->resources.sample[PP2_SAMPLE_LOGO], 1.0, 0.0, 1.0);
	}
	if(logo_tick >= 330)
	{
		if(instance->theme->menu_music_fn)
		{
			pp2_play_music(instance->theme->menu_music_fn);
		}
		else if(instance->theme->theme_music_fn)
		{
			pp2_play_music(instance->theme->theme_music_fn);
		}
		else
		{
			pp2_stop_music();
		}
		return false;
	}
	return true;
}

void pp2_intro_render(PP2_RESOURCES * resources)
{
	int i;
	ALLEGRO_COLOR tint_color;
	float alpha;

	t3f_select_view(t3f_default_view);
	al_hold_bitmap_drawing(false);
	al_clear_to_color(al_map_rgb(255, 255, 255));
	if(logo_tick < 210)
	{
		for(i = 0; i < pp2_intro_pixels; i++)
		{
			al_draw_filled_rectangle(t3f_project_x(pp2_intro_pixel[i].x, pp2_intro_pixel[i].z), t3f_project_y(pp2_intro_pixel[i].y, pp2_intro_pixel[i].z), t3f_project_x(pp2_intro_pixel[i].x + 10, pp2_intro_pixel[i].z), t3f_project_y(pp2_intro_pixel[i].y + 10, pp2_intro_pixel[i].z), alpha_color(pp2_intro_pixel[i].color, 1.0 - fabs(pp2_intro_pixel[i].z / 640.0)));
		}
	}
	if(logo_tick < 180)
	{
		tint_color = al_map_rgba_f(0.0, 0.0, 0.0, 0.0);
	}
	else if(logo_tick >= 180 && logo_tick < 210)
	{
		alpha = (float)(logo_tick - 180) / 30.0;
		tint_color = al_map_rgba_f(alpha, alpha, alpha, alpha);
	}
	else
	{
		tint_color = al_map_rgba_f(1.0, 1.0, 1.0, 1.0);
	}
	t3f_draw_bitmap(resources->bitmap[PP2_BITMAP_T3_LOGO], tint_color, 120.0, 40.0, 0.0, 0);
	if(logo_tick > 270)
	{
		alpha = (float)(logo_tick - 270) / 60.0;
		al_draw_filled_rectangle(0, 0, PP2_SCREEN_WIDTH, PP2_SCREEN_HEIGHT, al_map_rgba_f(0.0, 0.0, 0.0, alpha));
	}
}
