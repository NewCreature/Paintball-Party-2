#include "t3f/t3f.h"
#include "t3f/gui.h"
#include "t3f/sound.h"
#include "t3f/draw.h"
#include "title.h"
#include "../data.h"
#include "../file/music.h"
#include "../gameplay/replay.h"

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
	float y = 480.0;
	
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

void pp2_t_title_menu_logic(void)
{
	pp2_tick++;
	pp2_title_y += pp2_title_vy;
	if(pp2_title_vy >= 0.0)
	{
		pp2_menu_logo_y += pp2_menu_vy;
		if(pp2_menu_logo_y > 0.0)
		{
			pp2_menu_logo_y = 0.0;
			pp2_state = PP2_STATE_MENU;
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
	pp2_menu_offset -= 0.25;
	if(pp2_menu_offset <= -64.0)
	{
		pp2_menu_offset = 0.0;
	}
}

void pp2_t_title_menu_render(void)
{
	int tw = 640 / al_get_bitmap_width(pp2_bitmap[PP2_BITMAP_MENU_BG]) + 1;
	int th = 480 / al_get_bitmap_height(pp2_bitmap[PP2_BITMAP_MENU_BG]) + 2;
	int i, j;
	
	t3f_select_view(t3f_default_view);
	al_clear_to_color(al_map_rgba_f(0.75, 0.75, 1.0, 1.0));
	for(i = 0; i < th; i++)
	{
		for(j = 0; j < tw; j++)
		{
			al_draw_tinted_bitmap(pp2_bitmap[PP2_BITMAP_MENU_BG], al_map_rgba_f(0.75 * pp2_menu_bg_alpha, 0.75 * pp2_menu_bg_alpha, pp2_menu_bg_alpha, pp2_menu_bg_alpha), (float)(j * al_get_bitmap_width(pp2_bitmap[PP2_BITMAP_MENU_BG])) + pp2_menu_offset, (float)(i * al_get_bitmap_height(pp2_bitmap[PP2_BITMAP_MENU_BG])) + pp2_menu_offset, 0);
		}
	}
	al_draw_tinted_bitmap(pp2_bitmap[PP2_BITMAP_TITLE_SPLAT], al_map_rgba_f(pp2_title_alpha, pp2_title_alpha, pp2_title_alpha, pp2_title_alpha), 0.0, 0.0, 0);
	
	t3f_draw_bitmap(pp2_bitmap[PP2_BITMAP_TITLE_LOGO], al_map_rgba_f(0.0, 0.0, 0.0, 0.5), 0.0 + 4.0, pp2_title_y + 4.0, pp2_title_z, 0);
	t3f_draw_bitmap(pp2_bitmap[PP2_BITMAP_TITLE_LOGO], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), 0.0 + 4.0 - pp2_title_float, pp2_title_y + 4.0 - pp2_title_float, pp2_title_z, 0);
	
	al_draw_tinted_bitmap(pp2_bitmap[PP2_BITMAP_MENU_LOGO], al_map_rgba_f(0.0, 0.0, 0.0, 0.5), 160.0 + 2, pp2_menu_logo_y + 2, 0);
	al_draw_tinted_bitmap(pp2_bitmap[PP2_BITMAP_MENU_LOGO], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), 160.0, pp2_menu_logo_y, 0);
}

void pp2_title_setup(void)
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
	pp2_tick = 0;
}

void pp2_title_logic(void)
{
	int i;
	bool fired = false;
	
	pp2_tick++;
	for(i = 0; i < 4; i++)
	{
		t3f_read_controller(pp2_controller[i]);
		t3f_update_controller(pp2_controller[i]);
		if(pp2_controller[i]->state[PP2_CONTROLLER_FIRE].pressed)
		{
			fired = true;
		}
	}
	if(t3f_key[ALLEGRO_KEY_ENTER] || fired || t3f_mouse_button[0])
	{
		t3f_play_sample(pp2_sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
		pp2_state = PP2_STATE_T_TITLE_MENU;
		if(fired)
		{
			pp2_menu[pp2_current_menu]->hover_element = 0;
		}
		t3f_key[ALLEGRO_KEY_ENTER] = 0;
	}
	if(pp2_tick >= 60)
	{
		pp2_title_float += 0.2;
		if(pp2_title_float > 4.0)
		{
			pp2_title_float = 4.0;
		}
	}
	if(pp2_tick > 600)
	{
		if(t3f_key[ALLEGRO_KEY_EQUALS])
		{
			pp2_title_y -= 10.0;
		}
		else
		{
			pp2_title_y -= 1.0;
		}
		if(pp2_title_y + pp2_credits.credit[pp2_credits.credits - 1].y + PP2_CREDIT_SPACE < 0.0)
		{
			pp2_title_fade += 1.0 / 30.0;
			if(pp2_title_fade >= 1.0)
			{
				if(pp2_demo_database->entries > 0)
				{
					pp2_play_replay(al_path_cstr(pp2_demo_database->entry[rand() % pp2_demo_database->entries]->path, '/'), PP2_REPLAY_FLAG_DEMO);
				}
				else
				{
					pp2_title_setup();
				}
			}
		}
	}
}

void pp2_title_render(void)
{
//	int tw = 640 / al_get_bitmap_width(pp2_bitmap[PP2_BITMAP_MENU_BG]) + 1;
//	int th = 480 / al_get_bitmap_height(pp2_bitmap[PP2_BITMAP_MENU_BG]) + 2;
	int i;
	float alpha;
	
	if(pp2_tick < 30)
	{
		alpha = (float)pp2_tick / 30.0;
	}
	else
	{
		alpha = 1.0;
	}

	t3f_select_view(t3f_default_view);
	al_clear_to_color(al_map_rgba_f(0.75, 0.75, 1.0, 1.0));
	al_draw_tinted_bitmap(pp2_bitmap[PP2_BITMAP_TITLE_SPLAT], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), 0.0, 0.0, 0);
	al_draw_tinted_bitmap(pp2_bitmap[PP2_BITMAP_TITLE_LOGO], al_map_rgba_f(0.0, 0.0, 0.0, 0.5), 0.0 + 4.0, pp2_title_y + 4.0, 0);
	al_draw_tinted_bitmap(pp2_bitmap[PP2_BITMAP_TITLE_LOGO], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), 0.0 + 4.0 - pp2_title_float, pp2_title_y + 4.0 - pp2_title_float, 0);
	
	/* render credits */
	for(i = 0; i < pp2_credits.credits; i++)
	{
		al_draw_text(pp2_font[PP2_FONT_COMIC_16], al_map_rgba_f(0.0, 0.0, 0.0, 0.5), 320.0 + 2, pp2_credits.credit[i].y + pp2_title_y + 2, ALLEGRO_ALIGN_CENTRE, pp2_credits.credit[i].name);
		al_draw_text(pp2_font[PP2_FONT_COMIC_16], pp2_credits.credit[i].color, 320.0, pp2_credits.credit[i].y + pp2_title_y, ALLEGRO_ALIGN_CENTRE, pp2_credits.credit[i].name);
	}
	
	if(pp2_tick < 30)
	{
		al_draw_filled_rectangle(0, 0, 640, 480, al_map_rgba_f(0.0, 0.0, 0.0, 1.0 - (float)(pp2_tick) / 30.0));
	}
	else if(pp2_title_fade > 0.0)
	{
		al_draw_filled_rectangle(0, 0, 640, 480, al_map_rgba_f(0.0, 0.0, 0.0, pp2_title_fade));
	}
}
