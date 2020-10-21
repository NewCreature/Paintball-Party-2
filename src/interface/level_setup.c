#include "../t3f/t3f.h"
#include "../t3f/sound.h"
#include "../text_entry.h"
#include "../gameplay/game.h"
#include "../gameplay/game_struct.h"
#include "../file/database/level.h"
#include "menu_proc.h"
#include "level_setup.h"

bool pp2_level_setup_players_ready(PP2_GAME * gp)
{
	int i;
	int count = 0;

	for(i = 0; i < PP2_MAX_PLAYERS; i++)
	{
		if(gp->client_game->player[i]->playing)
		{
			count++;
			if(gp->player[i].step < 3)
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

int pp2_level_setup_player_count(PP2_GAME * gp)
{
	int i;
	int count = 0;

	for(i = 0; i < PP2_MAX_PLAYERS; i++)
	{
		if(gp->client_game->player[i]->playing)
		{
			count++;
		}
	}
	return count;
}

void pp2_level_setup_logic(PP2_INSTANCE * instance)
{
	int i;

	instance->interface.menu_offset -= 0.25;
	if(instance->interface.menu_offset <= -64.0)
	{
		instance->interface.menu_offset = 0.0;
	}
	if(!pp2_entering_text)
	{
		for(i = 0; i < 4; i++)
		{
			if(instance->game.client_game->controller[i]->port >= 0)
			{
				t3f_read_controller(instance->interface.controller[i]);
				t3f_update_controller(instance->interface.controller[i]);
				if(instance->interface.controller[i]->state[PP2_CONTROLLER_LEFT].pressed)
				{
					t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_NEXT], 1.0, 0.0, 1.0);
					instance->interface.level_choosing--;
					if(instance->interface.level_choosing < 0)
					{
						instance->interface.level_choosing = instance->game.client_game->content_list[PP2_CONTENT_LEVELS]->count - 1;
					}
					instance->interface.level_chosen = 0;
					joynet_select_game_content(instance->game.client_game, 0, PP2_CONTENT_LEVELS, instance->game.client_game->content_list[PP2_CONTENT_LEVELS]->hash[instance->interface.level_choosing]);
				}
				else if(instance->interface.controller[i]->state[PP2_CONTROLLER_RIGHT].pressed)
				{
					t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_NEXT], 1.0, 0.0, 1.0);
					instance->interface.level_choosing++;
					if(instance->interface.level_choosing >= instance->game.client_game->content_list[PP2_CONTENT_LEVELS]->count)
					{
						instance->interface.level_choosing = 0;
					}
					instance->interface.level_chosen = 0;
					joynet_select_game_content(instance->game.client_game, 0, PP2_CONTENT_LEVELS, instance->game.client_game->content_list[PP2_CONTENT_LEVELS]->hash[instance->interface.level_choosing]);
				}
				else if(instance->interface.controller[i]->state[PP2_CONTROLLER_UP].pressed)
				{
					t3f_play_sample(instance->resources.sample[PP2_SAMPLE_MENU_NEXT], 1.0, 0.0, 1.0);
					instance->interface.level_choosing = rand() % instance->game.client_game->content_list[PP2_CONTENT_LEVELS]->count;
					instance->interface.level_chosen = 0;
					joynet_select_game_content(instance->game.client_game, 0, PP2_CONTENT_LEVELS, instance->game.client_game->content_list[PP2_CONTENT_LEVELS]->hash[instance->interface.level_choosing]);
				}
				else if(instance->interface.controller[i]->state[PP2_CONTROLLER_FIRE].pressed)
				{
					pp2_menu_proc_overlay_next(instance, 0, NULL);
				}
				else if(instance->interface.controller[i]->state[PP2_CONTROLLER_JUMP].pressed)
				{
					pp2_menu_proc_overlay_back(instance, 0, NULL);
				}
			}
		}
	}
	t3f_process_gui(instance->interface.menu[PP2_MENU_LEVEL_SETUP_OVERLAY], NULL);
}

void pp2_level_setup_render(PP2_INTERFACE * ip, PP2_GAME * gp, PP2_RESOURCES * resources)
{
	int tw = PP2_SCREEN_WIDTH / al_get_bitmap_width(resources->bitmap[PP2_BITMAP_MENU_BG]) + 1;
	int th = PP2_SCREEN_HEIGHT / al_get_bitmap_height(resources->bitmap[PP2_BITMAP_MENU_BG]) + 2;
	int i, j;
	ALLEGRO_COLOR tint;

	al_hold_bitmap_drawing(true);
	for(i = 0; i < th; i++)
	{
		for(j = 0; j < tw; j++)
		{
			al_draw_tinted_bitmap(resources->bitmap[PP2_BITMAP_MENU_BG], al_map_rgba_f(0.1, 0.1, 0.25, 1.0), (float)(j * al_get_bitmap_width(resources->bitmap[PP2_BITMAP_MENU_BG])) + ip->menu_offset, (float)(i * al_get_bitmap_height(resources->bitmap[PP2_BITMAP_MENU_BG])) + ip->menu_offset, 0);
		}
	}

	al_draw_text(resources->font[PP2_FONT_COMIC_16], al_map_rgba_f(0.0, 0.0, 0.0, 1.0), PP2_SCREEN_WIDTH / 2 + 2, 0 + 2, ALLEGRO_ALIGN_CENTRE, "Level Setup");
	al_draw_text(resources->font[PP2_FONT_COMIC_16], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), PP2_SCREEN_WIDTH / 2, 0, ALLEGRO_ALIGN_CENTRE, "Level Setup");
	al_hold_bitmap_drawing(false);
	if(!ip->level_chosen)
	{
		tint = al_map_rgba_f(0.5, 0.5, 0.5, 0.5);
	}
	else if(gp->client_game->player_count > ip->level_preview->players)
	{
		tint = al_map_rgba_f(0.5, 0.5, 0.5, 1.0);
	}
	else
	{
		tint = al_map_rgba_f(1.0, 1.0, 1.0, 1.0);
	}
	if(!ip->level_preview)
	{
	}
	else
	{
		pp2_render_level_preview(ip->level_preview, tint, PP2_SCREEN_WIDTH / 2 - al_get_bitmap_width(ip->level_preview->bitmap) / 2, PP2_SCREEN_HEIGHT / 2 - al_get_bitmap_height(ip->level_preview->bitmap) / 2, 0);
		al_draw_textf(resources->font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), PP2_SCREEN_WIDTH / 2, PP2_SCREEN_HEIGHT / 2 + al_get_bitmap_height(ip->level_preview->bitmap) / 2 + 16.0, ALLEGRO_ALIGN_CENTRE, "< %s (%d/%d) >", ip->level_preview->name, gp->client_game->player_count, ip->level_preview->players);
	}
	t3f_render_gui(ip->menu[PP2_MENU_LEVEL_SETUP_OVERLAY]);
}
