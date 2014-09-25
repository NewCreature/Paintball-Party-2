#include "../t3f/t3f.h"
#include "../t3f/sound.h"
#include "../data.h"
#include "../text_entry.h"
#include "../gameplay/game.h"
#include "../file/database/level.h"
#include "menu_proc.h"
#include "level_setup.h"

bool pp2_level_setup_players_ready(void)
{
	int i;
	int count = 0;
	
	for(i = 0; i < PP2_MAX_PLAYERS; i++)
	{
		if(pp2_client_game->player[i]->playing)
		{
			count++;
			if(pp2_player[i].step < 3)
			{
				return false;
			}
		}
	}
	if(count > 0)
	{
		return true;
	}
	return false;
}

int pp2_level_setup_player_count(void)
{
	int i;
	int count = 0;
	
	for(i = 0; i < PP2_MAX_PLAYERS; i++)
	{
		if(pp2_client_game->player[i]->playing)
		{
			count++;
		}
	}
	return count;
}

void pp2_level_setup_logic(void)
{
	int i;
	
	pp2_menu_offset -= 0.25;
	if(pp2_menu_offset <= -64.0)
	{
		pp2_menu_offset = 0.0;
	}
	if(!pp2_entering_text)
	{
		for(i = 0; i < 4; i++)
		{
			if(pp2_client_game->controller[i]->port >= 0)
			{
				t3f_read_controller(pp2_controller[i]);
				t3f_update_controller(pp2_controller[i]);
				if(pp2_controller[i]->state[PP2_CONTROLLER_LEFT].pressed)
				{
					t3f_play_sample(pp2_sample[PP2_SAMPLE_MENU_NEXT], 1.0, 0.0, 1.0);
					pp2_level_choosing--;
					if(pp2_level_choosing < 0)
					{
						pp2_level_choosing = pp2_client_game->content_list[PP2_CONTENT_LEVELS]->count - 1;
					}
					pp2_level_chosen = 0;
					joynet_select_game_content(pp2_client_game, 0, PP2_CONTENT_LEVELS, pp2_client_game->content_list[PP2_CONTENT_LEVELS]->hash[pp2_level_choosing]);
				}
				else if(pp2_controller[i]->state[PP2_CONTROLLER_RIGHT].pressed)
				{
					t3f_play_sample(pp2_sample[PP2_SAMPLE_MENU_NEXT], 1.0, 0.0, 1.0);
					pp2_level_choosing++;
					if(pp2_level_choosing >= pp2_client_game->content_list[PP2_CONTENT_LEVELS]->count)
					{
						pp2_level_choosing = 0;
					}
					pp2_level_chosen = 0;
					joynet_select_game_content(pp2_client_game, 0, PP2_CONTENT_LEVELS, pp2_client_game->content_list[PP2_CONTENT_LEVELS]->hash[pp2_level_choosing]);
				}
				else if(pp2_controller[i]->state[PP2_CONTROLLER_UP].pressed)
				{
					t3f_play_sample(pp2_sample[PP2_SAMPLE_MENU_NEXT], 1.0, 0.0, 1.0);
					pp2_level_choosing = rand() % pp2_client_game->content_list[PP2_CONTENT_LEVELS]->count;
					pp2_level_chosen = 0;
					joynet_select_game_content(pp2_client_game, 0, PP2_CONTENT_LEVELS, pp2_client_game->content_list[PP2_CONTENT_LEVELS]->hash[pp2_level_choosing]);
				}
				else if(pp2_controller[i]->state[PP2_CONTROLLER_FIRE].pressed)
				{
					pp2_menu_proc_overlay_next(NULL, 0, NULL);
				}
				else if(pp2_controller[i]->state[PP2_CONTROLLER_JUMP].pressed)
				{
					pp2_menu_proc_overlay_back(NULL, 0, NULL);
				}
			}
		}
	}
	t3f_process_gui(pp2_menu[PP2_MENU_LEVEL_SETUP_OVERLAY], NULL);
}

void pp2_level_setup_render(void)
{
	int tw = 640 / al_get_bitmap_width(pp2_bitmap[PP2_BITMAP_MENU_BG]) + 1;
	int th = 480 / al_get_bitmap_height(pp2_bitmap[PP2_BITMAP_MENU_BG]) + 2;
	int i, j;
	ALLEGRO_COLOR tint;
	
	al_hold_bitmap_drawing(true);
	for(i = 0; i < th; i++)
	{
		for(j = 0; j < tw; j++)
		{
			al_draw_tinted_bitmap(pp2_bitmap[PP2_BITMAP_MENU_BG], al_map_rgba_f(0.1, 0.1, 0.25, 1.0), (float)(j * al_get_bitmap_width(pp2_bitmap[PP2_BITMAP_MENU_BG])) + pp2_menu_offset, (float)(i * al_get_bitmap_height(pp2_bitmap[PP2_BITMAP_MENU_BG])) + pp2_menu_offset, 0);
		}
	}
	
	al_draw_text(pp2_font[PP2_FONT_COMIC_16], al_map_rgba_f(0.0, 0.0, 0.0, 1.0), 320 + 2, 0 + 2, ALLEGRO_ALIGN_CENTRE, "Level Setup");
	al_draw_text(pp2_font[PP2_FONT_COMIC_16], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), 320, 0, ALLEGRO_ALIGN_CENTRE, "Level Setup");
	al_hold_bitmap_drawing(false);
	if(!pp2_level_chosen)
	{
		tint = al_map_rgba_f(0.5, 0.5, 0.5, 0.5);
	}
	else if(pp2_client_game->player_count > pp2_level_preview->players)
	{
		tint = al_map_rgba_f(0.5, 0.5, 0.5, 1.0);
	}
	else
	{
		tint = al_map_rgba_f(1.0, 1.0, 1.0, 1.0);
	}
	if(!pp2_level_preview)
	{
	}
	else
	{
		pp2_render_level_preview(pp2_level_preview, tint, 320 - al_get_bitmap_width(pp2_level_preview->bitmap) / 2, 240 - al_get_bitmap_height(pp2_level_preview->bitmap) / 2, 0);
		al_draw_textf(pp2_font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), 320, 240 + al_get_bitmap_height(pp2_level_preview->bitmap) / 2 + 16.0, ALLEGRO_ALIGN_CENTRE, "< %s (%d/%d) >", pp2_level_preview->name, pp2_client_game->player_count, pp2_level_preview->players);
	}
	t3f_render_gui(pp2_menu[PP2_MENU_LEVEL_SETUP_OVERLAY]);
}
