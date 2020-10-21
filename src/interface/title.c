#include "t3f/t3f.h"
#include "t3f/gui.h"
#include "t3f/sound.h"
#include "t3f/draw.h"
#include "../pp2.h"
#include "title.h"
#include "../file/music.h"
#include "../gameplay/replay.h"
#include "../resource.h"

static bool pp2_title_music_started = false;
static float pp2_title_float = 0.0;
static float pp2_title_y = 0.0;
static float pp2_title_z = 0.0;
static float pp2_title_vy = -22.0;
static float pp2_title_alpha = 1.0;
static float pp2_menu_logo_y = -240;
static float pp2_menu_bg_alpha = 0.0;
static float pp2_menu_vy = 0.0;
static float pp2_title_fade = 0.0;

void pp2_title_add_credit(PP2_CREDITS * cp, char * name, float y, ALLEGRO_COLOR color)
{
	strcpy(cp->credit[cp->credits].name, name);
	cp->credit[cp->credits].y = y;
	cp->credit[cp->credits].color = color;
	cp->credits++;
}

void pp2_title_build_credits(PP2_CREDITS * cp)
{
	cp->credits = 0;
	float y = 540.0;

	pp2_title_add_credit(cp, "Lead Producer", y, PP2_CREDIT_COLOR_TITLE);
	y += PP2_CREDIT_TITLE_SPACE;
	pp2_title_add_credit(cp, "Todd Cope", y, PP2_CREDIT_COLOR_NAME);
	y += PP2_CREDIT_SPACE;

	pp2_title_add_credit(cp, "Game Design", y, PP2_CREDIT_COLOR_TITLE);
	y += PP2_CREDIT_TITLE_SPACE;
	pp2_title_add_credit(cp, "Todd Cope", y, PP2_CREDIT_COLOR_NAME);
	y += PP2_CREDIT_SPACE;

	pp2_title_add_credit(cp, "Programming", y, PP2_CREDIT_COLOR_TITLE);
	y += PP2_CREDIT_TITLE_SPACE;
	pp2_title_add_credit(cp, "Todd Cope", y, PP2_CREDIT_COLOR_NAME);
	y += PP2_CREDIT_SPACE;

	pp2_title_add_credit(cp, "Artwork", y, PP2_CREDIT_COLOR_TITLE);
	y += PP2_CREDIT_TITLE_SPACE;
	pp2_title_add_credit(cp, "Travis Cope", y, PP2_CREDIT_COLOR_NAME);
	y += PP2_CREDIT_TITLE_SPACE;
	pp2_title_add_credit(cp, "Todd Cope", y, PP2_CREDIT_COLOR_NAME);
	y += PP2_CREDIT_SPACE;

	pp2_title_add_credit(cp, "Character Art", y, PP2_CREDIT_COLOR_TITLE);
	y += PP2_CREDIT_TITLE_SPACE;
	pp2_title_add_credit(cp, "Travis Cope", y, PP2_CREDIT_COLOR_NAME);
	y += PP2_CREDIT_SPACE;

	pp2_title_add_credit(cp, "Character Animation", y, PP2_CREDIT_COLOR_TITLE);
	y += PP2_CREDIT_TITLE_SPACE;
	pp2_title_add_credit(cp, "Todd Cope", y, PP2_CREDIT_COLOR_NAME);
	y += PP2_CREDIT_SPACE;

	pp2_title_add_credit(cp, "Level Design", y, PP2_CREDIT_COLOR_TITLE);
	y += PP2_CREDIT_TITLE_SPACE;
	pp2_title_add_credit(cp, "Todd Cope", y, PP2_CREDIT_COLOR_NAME);
	y += PP2_CREDIT_TITLE_SPACE;
	pp2_title_add_credit(cp, "Tripp (Castle Tops)", y, PP2_CREDIT_COLOR_NAME);
	y += PP2_CREDIT_TITLE_SPACE;
	pp2_title_add_credit(cp, "Mike Herring (Mike Two)", y, PP2_CREDIT_COLOR_NAME);
	y += PP2_CREDIT_TITLE_SPACE;
	pp2_title_add_credit(cp, "Jesse Nelson (Jesse's Subamarine)", y, PP2_CREDIT_COLOR_NAME);
	y += PP2_CREDIT_SPACE;

	pp2_title_add_credit(cp, "Music", y, PP2_CREDIT_COLOR_TITLE);
	y += PP2_CREDIT_TITLE_SPACE;
	pp2_title_add_credit(cp, "Grady O'Connell", y, PP2_CREDIT_COLOR_NAME);
	y += PP2_CREDIT_SPACE;

	pp2_title_add_credit(cp, "PHP Programming", y, PP2_CREDIT_COLOR_TITLE);
	y += PP2_CREDIT_TITLE_SPACE;
	pp2_title_add_credit(cp, "Grady O'Connell", y, PP2_CREDIT_COLOR_NAME);
	y += PP2_CREDIT_SPACE;

	pp2_title_add_credit(cp, "Testing", y, PP2_CREDIT_COLOR_TITLE);
	y += PP2_CREDIT_TITLE_SPACE;
	pp2_title_add_credit(cp, "Todd Cope", y, PP2_CREDIT_COLOR_NAME);
	y += PP2_CREDIT_TITLE_SPACE;
	pp2_title_add_credit(cp, "Dario_ff", y, PP2_CREDIT_COLOR_NAME);
	y += PP2_CREDIT_TITLE_SPACE;
	pp2_title_add_credit(cp, "pmprog", y, PP2_CREDIT_COLOR_NAME);
	y += PP2_CREDIT_TITLE_SPACE;
	pp2_title_add_credit(cp, "Vanneto", y, PP2_CREDIT_COLOR_NAME);
	y += PP2_CREDIT_TITLE_SPACE;
	pp2_title_add_credit(cp, "Trent Gamblin", y, PP2_CREDIT_COLOR_NAME);
	y += PP2_CREDIT_TITLE_SPACE;
	pp2_title_add_credit(cp, "Grady O'Connell", y, PP2_CREDIT_COLOR_NAME);
	y += PP2_CREDIT_TITLE_SPACE;
	pp2_title_add_credit(cp, "Alex Cope", y, PP2_CREDIT_COLOR_NAME);
	y += PP2_CREDIT_TITLE_SPACE;
	pp2_title_add_credit(cp, "Jesse Nelson", y, PP2_CREDIT_COLOR_NAME);
	y += PP2_CREDIT_TITLE_SPACE;
	pp2_title_add_credit(cp, "Travis Cope", y, PP2_CREDIT_COLOR_NAME);
	y += PP2_CREDIT_TITLE_SPACE;
	pp2_title_add_credit(cp, "Candace Cope", y, PP2_CREDIT_COLOR_NAME);
	y += PP2_CREDIT_TITLE_SPACE;
	pp2_title_add_credit(cp, "Matheus Garcia Pereira", y, PP2_CREDIT_COLOR_NAME);
	y += PP2_CREDIT_TITLE_SPACE;
	pp2_title_add_credit(cp, "Michael Cope", y, PP2_CREDIT_COLOR_NAME);
	y += PP2_CREDIT_TITLE_SPACE;
	pp2_title_add_credit(cp, "Mason Cope", y, PP2_CREDIT_COLOR_NAME);
	y += PP2_CREDIT_SPACE;

	pp2_title_add_credit(cp, "Pitch Team", y, PP2_CREDIT_COLOR_TITLE);
	y += PP2_CREDIT_TITLE_SPACE;
	pp2_title_add_credit(cp, "Todd Cope", y, PP2_CREDIT_COLOR_NAME);
	y += PP2_CREDIT_TITLE_SPACE;
	pp2_title_add_credit(cp, "Candace Cope", y, PP2_CREDIT_COLOR_NAME);
	y += PP2_CREDIT_TITLE_SPACE;
	pp2_title_add_credit(cp, "Travis Cope", y, PP2_CREDIT_COLOR_NAME);
	y += PP2_CREDIT_TITLE_SPACE;
	pp2_title_add_credit(cp, "Michael Cope", y, PP2_CREDIT_COLOR_NAME);
	y += PP2_CREDIT_TITLE_SPACE;
	pp2_title_add_credit(cp, "Wes", y, PP2_CREDIT_COLOR_NAME);
	y += PP2_CREDIT_SPACE;

	pp2_title_add_credit(cp, "Special Thanks", y, PP2_CREDIT_COLOR_TITLE);
	y += PP2_CREDIT_TITLE_SPACE;
	pp2_title_add_credit(cp, "Candace Cope", y, PP2_CREDIT_COLOR_NAME);
	y += PP2_CREDIT_TITLE_SPACE;
	pp2_title_add_credit(cp, "Alex Cope", y, PP2_CREDIT_COLOR_NAME);
	y += PP2_CREDIT_TITLE_SPACE;
	pp2_title_add_credit(cp, "Jesse Nelson", y, PP2_CREDIT_COLOR_NAME);
	y += PP2_CREDIT_TITLE_SPACE;
	pp2_title_add_credit(cp, "Dawson Irvine", y, PP2_CREDIT_COLOR_NAME);
	y += PP2_CREDIT_TITLE_SPACE;
	pp2_title_add_credit(cp, "Allegro 5 Developers", y, PP2_CREDIT_COLOR_NAME);
	y += PP2_CREDIT_SPACE;
}

bool pp2_t_title_menu_logic(void * data)
{
	PP2_INSTANCE * instance = (PP2_INSTANCE *)data;
	bool ret = true;

	instance->interface.tick++;
	pp2_title_y += pp2_title_vy;
	if(pp2_title_vy >= 0.0)
	{
		if(!pp2_title_music_started)
		{
			if(instance->theme->menu_music_fn)
			{
				pp2_play_music(instance->theme->menu_music_fn);
			}
			pp2_title_music_started = true;
		}
		pp2_menu_logo_y += pp2_menu_vy;
		if(pp2_menu_logo_y > 0.0)
		{
			pp2_menu_logo_y = 0.0;
			ret = false;
		}
		pp2_menu_vy += 0.3;
		pp2_menu_bg_alpha += 1.0 / 30.0;
		if(pp2_menu_bg_alpha > 1.0)
		{
			pp2_menu_bg_alpha = 1.0;
		}
	}
	pp2_title_vy += 0.5;
	if(pp2_title_vy > 0.0)
	{
		pp2_title_vy = 0.0;
	}
	if(pp2_title_vy < 0)
	{
		pp2_title_z += 3.0;
	}
	pp2_title_alpha -= 1.0 / 30.0;
	if(pp2_title_alpha < 0.0)
	{
		pp2_title_alpha = 0.0;
	}

	/* scroll menu bg */
	instance->interface.menu_offset -= 0.25;
	if(instance->interface.menu_offset <= -64.0)
	{
		instance->interface.menu_offset = 0.0;
	}

	return ret;
}

void pp2_t_title_menu_render(PP2_INTERFACE * ip, PP2_RESOURCES * resources)
{
	int tw = PP2_SCREEN_WIDTH / al_get_bitmap_width(resources->bitmap[PP2_BITMAP_MENU_BG]) + 1;
	int th = PP2_SCREEN_HEIGHT / al_get_bitmap_height(resources->bitmap[PP2_BITMAP_MENU_BG]) + 2;
	int i, j;
	float cx;
	float lx, ly;

	t3f_select_view(t3f_default_view);
	al_clear_to_color(PP2_TITLE_BG_COLOR);
	for(i = 0; i < th; i++)
	{
		for(j = 0; j < tw; j++)
		{
			al_draw_tinted_bitmap(resources->bitmap[PP2_BITMAP_MENU_BG], al_map_rgba_f(0.75 * pp2_menu_bg_alpha, 0.75 * pp2_menu_bg_alpha, pp2_menu_bg_alpha, pp2_menu_bg_alpha), (float)(j * al_get_bitmap_width(resources->bitmap[PP2_BITMAP_MENU_BG])) + ip->menu_offset, (float)(i * al_get_bitmap_height(resources->bitmap[PP2_BITMAP_MENU_BG])) + ip->menu_offset, 0);
		}
	}
	lx = PP2_SCREEN_WIDTH / 2 - al_get_bitmap_width(resources->bitmap[PP2_BITMAP_TITLE_SPLAT]) / 2;
	ly = PP2_SCREEN_HEIGHT / 2 - al_get_bitmap_height(resources->bitmap[PP2_BITMAP_TITLE_SPLAT]) / 2;
	al_draw_tinted_bitmap(resources->bitmap[PP2_BITMAP_TITLE_SPLAT], al_map_rgba_f(pp2_title_alpha, pp2_title_alpha, pp2_title_alpha, pp2_title_alpha), lx, ly, 0);

	t3f_draw_bitmap(resources->bitmap[PP2_BITMAP_TITLE_LOGO], al_map_rgba_f(0.0, 0.0, 0.0, 0.5), lx + 4.0, ly +  pp2_title_y + 4.0, pp2_title_z, 0);
	t3f_draw_bitmap(resources->bitmap[PP2_BITMAP_TITLE_LOGO], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), lx + 4.0 - pp2_title_float, ly + pp2_title_y + 4.0 - pp2_title_float, pp2_title_z, 0);

	cx = PP2_SCREEN_WIDTH / 2 - al_get_bitmap_width(resources->bitmap[PP2_BITMAP_MENU_LOGO]) / 2;
	al_draw_tinted_bitmap(resources->bitmap[PP2_BITMAP_MENU_LOGO], al_map_rgba_f(0.0, 0.0, 0.0, 0.5), cx + 2, pp2_menu_logo_y + 2, 0);
	al_draw_tinted_bitmap(resources->bitmap[PP2_BITMAP_MENU_LOGO], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), cx, pp2_menu_logo_y, 0);
}

void pp2_title_setup(PP2_INTERFACE * ip)
{
	pp2_title_float = 0.0;
	pp2_title_y = 0.0;
	pp2_title_z = 0.0;
	pp2_title_vy = -22.0;
	pp2_title_alpha = 1.0;
	pp2_menu_logo_y = -240;
	pp2_menu_bg_alpha = 0.0;
	pp2_menu_vy = 0.0;
	pp2_title_fade = 0.0;
	ip->tick = 0;
}

bool pp2_title_logic(PP2_INTERFACE * ip, PP2_GAME * gp, PP2_RESOURCES * resources)
{
	int i;
	bool fired = false;
	bool ret = true;

	ip->tick++;
	for(i = 0; i < 4; i++)
	{
		t3f_read_controller(ip->controller[i]);
		t3f_update_controller(ip->controller[i]);
		if(ip->controller[i]->state[PP2_CONTROLLER_FIRE].pressed)
		{
			fired = true;
		}
	}
	if(t3f_key[ALLEGRO_KEY_ENTER] || fired || t3f_mouse_button[0])
	{
		t3f_play_sample(resources->sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
		pp2_title_music_started = false;
		ret = false;
		if(fired)
		{
			ip->menu[ip->current_menu]->hover_element = 0;
		}
		t3f_key[ALLEGRO_KEY_ENTER] = 0;
	}
	if(ip->tick >= 60)
	{
		pp2_title_float += 0.2;
		if(pp2_title_float > 4.0)
		{
			pp2_title_float = 4.0;
		}
	}
	if(ip->tick > 600)
	{
		if(t3f_key[ALLEGRO_KEY_EQUALS])
		{
			pp2_title_y -= 10.0;
		}
		else
		{
			pp2_title_y -= 1.0;
		}
		if(pp2_title_y + ip->credits.credit[ip->credits.credits - 1].y + PP2_CREDIT_SPACE < 0.0)
		{
			pp2_title_fade += 1.0 / 30.0;
			if(pp2_title_fade >= 1.0)
			{
				if(resources->demo_database->entries > 0)
				{
					if(!pp2_play_replay(gp, al_path_cstr(resources->demo_database->entry[rand() % resources->demo_database->entries]->path, '/'), PP2_REPLAY_FLAG_DEMO, ip, resources))
					{
						pp2_title_setup(ip);
					}
				}
				else
				{
					pp2_title_setup(ip);
				}
			}
		}
	}
	return ret;
}

void pp2_title_render(PP2_INTERFACE * ip, PP2_RESOURCES * resources)
{
//	int tw = 640 / al_get_bitmap_width(resources->bitmap[PP2_BITMAP_MENU_BG]) + 1;
//	int th = 480 / al_get_bitmap_height(resources->bitmap[PP2_BITMAP_MENU_BG]) + 2;
	int i;
	float alpha;
	float lx, ly;

	if(ip->tick < 30)
	{
		alpha = (float)ip->tick / 30.0;
	}
	else
	{
		alpha = 1.0;
	}

	t3f_select_view(t3f_default_view);
	al_clear_to_color(PP2_TITLE_BG_COLOR);
	lx = PP2_SCREEN_WIDTH / 2 - al_get_bitmap_width(resources->bitmap[PP2_BITMAP_TITLE_SPLAT]) / 2;
	ly = PP2_SCREEN_HEIGHT / 2 - al_get_bitmap_height(resources->bitmap[PP2_BITMAP_TITLE_SPLAT]) / 2;
	al_draw_tinted_bitmap(resources->bitmap[PP2_BITMAP_TITLE_SPLAT], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), lx, ly, 0);
	al_draw_tinted_bitmap(resources->bitmap[PP2_BITMAP_TITLE_LOGO], al_map_rgba_f(0.0, 0.0, 0.0, 0.5), lx + 4.0, ly + pp2_title_y + 4.0, 0);
	al_draw_tinted_bitmap(resources->bitmap[PP2_BITMAP_TITLE_LOGO], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), lx + 4.0 - pp2_title_float, ly + pp2_title_y + 4.0 - pp2_title_float, 0);

	/* render credits */
	for(i = 0; i < ip->credits.credits; i++)
	{
		al_draw_text(resources->font[PP2_FONT_COMIC_16], al_map_rgba_f(0.0, 0.0, 0.0, 0.5), PP2_SCREEN_WIDTH / 2 + 2, ip->credits.credit[i].y + pp2_title_y + 2, ALLEGRO_ALIGN_CENTRE, ip->credits.credit[i].name);
		al_draw_text(resources->font[PP2_FONT_COMIC_16], ip->credits.credit[i].color, PP2_SCREEN_WIDTH / 2, ip->credits.credit[i].y + pp2_title_y, ALLEGRO_ALIGN_CENTRE, ip->credits.credit[i].name);
	}

	if(ip->tick < 30)
	{
		al_draw_filled_rectangle(0, 0, PP2_SCREEN_WIDTH, PP2_SCREEN_HEIGHT, al_map_rgba_f(0.0, 0.0, 0.0, 1.0 - (float)(ip->tick) / 30.0));
	}
	else if(pp2_title_fade > 0.0)
	{
		al_draw_filled_rectangle(0, 0, PP2_SCREEN_WIDTH, PP2_SCREEN_HEIGHT, al_map_rgba_f(0.0, 0.0, 0.0, pp2_title_fade));
	}
}
