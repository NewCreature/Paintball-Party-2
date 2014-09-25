#include "../t3f/t3f.h"
#include "../t3f/sound.h"
#include "../data.h"
#include "../text_entry.h"
#include "../gameplay/game.h"
#include "../file/database/character.h"
#include "../file/database/level.h"
#include "menu_proc.h"
#include "player_setup.h"

void pp2_player_setup_reset(void)
{
	int i;
	
	for(i = 0; i < PP2_MAX_PLAYERS; i++)
	{
		pp2_player[i].step = PP2_PLAYER_STEP_SELECT_PROFILE;
	}
}

void pp2_player_setup_logic(void)
{
	int i;
	
	pp2_menu_offset -= 0.25;
	if(pp2_menu_offset <= -64.0)
	{
		pp2_menu_offset = 0.0;
	}
	
	/* do not process local controls when chatting */
	if(!pp2_entering_text)
	{
		for(i = 0; i < 4; i++)
		{
			if(pp2_client_game->controller[i]->port < 0)
			{
				t3f_read_controller(pp2_controller[i]);
				t3f_update_controller(pp2_controller[i]);
				if(pp2_controller[i]->state[PP2_CONTROLLER_FIRE].pressed)
				{
					joynet_connect_to_game(pp2_client_game, i, -1);
				}
			}
		}
		
		for(i = 0; i < 4; i++)
		{
			if(pp2_client_game->controller[i]->port >= 0)
			{
				t3f_read_controller(pp2_controller[i]);
				t3f_update_controller(pp2_controller[i]);
				if(pp2_controller[i]->state[PP2_CONTROLLER_FIRE].pressed)
				{
					switch(pp2_player[pp2_client_game->controller[i]->port].step)
					{
						case PP2_PLAYER_STEP_SELECT_PROFILE:
						{
							if(pp2_player[pp2_client_game->controller[i]->port].profile_choice == pp2_profiles.items)
							{
								pp2_entered_text[0] = 0;
								pp2_entering_text = 1;
								pp2_entering_text_pos = 0;
								t3f_clear_keys();
								pp2_state = PP2_STATE_MENU;
								pp2_select_menu(PP2_MENU_NEW_PROFILE);
								pp2_menu_joystick_disabled = true;
							}
							else
							{
								strcpy(pp2_client_game->player[pp2_client_game->controller[i]->port]->name, pp2_profiles.item[pp2_player[pp2_client_game->controller[i]->port].profile_choice].name);
								t3f_play_sample(pp2_sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
								pp2_player[pp2_client_game->controller[i]->port].step = PP2_PLAYER_STEP_SELECTED_PROFILE;
								joynet_update_player_options(pp2_client_game, pp2_client_game->controller[i]->port);
								
								/* select the level if this client is the master */
								if(!pp2_client || pp2_client->master)
								{
									joynet_select_game_content(pp2_client_game, pp2_client_game->controller[i]->port, PP2_CONTENT_LEVELS, pp2_level_hash);
								}
							}
							break;
						}
						case PP2_PLAYER_STEP_CHARACTER_FOUND:
						{
	//						joynet_select_game_content(pp2_client_game, pp2_client_game->controller[i]->port, PP2_CONTENT_CHARACTERS, pp2_client_game->content_list[PP2_CONTENT_CHARACTERS]->hash[pp2_player[pp2_client_game->controller[i]->port].character_choosing]);
							pp2_player[pp2_client_game->controller[i]->port].step = PP2_PLAYER_STEP_DONE;
							joynet_update_player_options(pp2_client_game, pp2_client_game->controller[i]->port);
							break;
						}
						case PP2_PLAYER_STEP_DONE:
						{
							
							/* only master can proceed to next menu */
							if(!pp2_client || pp2_client->master)
							{
								pp2_menu_proc_overlay_next(NULL, 0, NULL);
								pp2_menu[pp2_current_menu]->hover_element = 0;
							}
						}
					}
				}
				else if(pp2_controller[i]->state[PP2_CONTROLLER_JUMP].pressed)
				{
					switch(pp2_player[pp2_client_game->controller[i]->port].step)
					{
						case PP2_PLAYER_STEP_SELECT_PROFILE:
						{
							joynet_disconnect_from_game(pp2_client_game, i, pp2_client_game->controller[i]->port);
							break;
						}
						case PP2_PLAYER_STEP_SELECT_CHARACTER:
						case PP2_PLAYER_STEP_SELECTED_CHARACTER:
						case PP2_PLAYER_STEP_CHARACTER_WAIT:
						case PP2_PLAYER_STEP_CHARACTER_FOUND:
						{
							pp2_player[pp2_client_game->controller[i]->port].step = PP2_PLAYER_STEP_SELECT_PROFILE;
							joynet_update_player_options(pp2_client_game, pp2_client_game->controller[i]->port);
							break;
						}
						case PP2_PLAYER_STEP_DONE:
						{
							if(pp2_player_preview[pp2_client_game->controller[i]->port]->sound)
							{
								al_stop_sample(&pp2_player_preview[pp2_client_game->controller[i]->port]->sound_id);
							}
							pp2_player[pp2_client_game->controller[i]->port].step = PP2_PLAYER_STEP_CHARACTER_FOUND;
							joynet_update_player_options(pp2_client_game, pp2_client_game->controller[i]->port);
							break;
						}
					}
				}
				else if(pp2_controller[i]->state[PP2_CONTROLLER_LEFT].pressed && pp2_player[pp2_client_game->controller[i]->port].step == PP2_PLAYER_STEP_SELECT_PROFILE)
				{
					t3f_play_sample(pp2_sample[PP2_SAMPLE_MENU_NEXT], 1.0, 0.0, 1.0);
					pp2_player[pp2_client_game->controller[i]->port].profile_choice--;
					if(pp2_player[pp2_client_game->controller[i]->port].profile_choice < 0)
					{
						pp2_player[pp2_client_game->controller[i]->port].profile_choice = pp2_profiles.items;
					}
				}
				else if(pp2_controller[i]->state[PP2_CONTROLLER_RIGHT].pressed && pp2_player[pp2_client_game->controller[i]->port].step == PP2_PLAYER_STEP_SELECT_PROFILE)
				{
					t3f_play_sample(pp2_sample[PP2_SAMPLE_MENU_NEXT], 1.0, 0.0, 1.0);
					pp2_player[pp2_client_game->controller[i]->port].profile_choice++;
					if(pp2_player[pp2_client_game->controller[i]->port].profile_choice > pp2_profiles.items)
					{
						pp2_player[pp2_client_game->controller[i]->port].profile_choice = 0;
					}
				}
				else if(pp2_controller[i]->state[PP2_CONTROLLER_LEFT].pressed && (pp2_player[pp2_client_game->controller[i]->port].step == PP2_PLAYER_STEP_SELECT_CHARACTER || pp2_player[pp2_client_game->controller[i]->port].step == PP2_PLAYER_STEP_SELECTED_CHARACTER || pp2_player[pp2_client_game->controller[i]->port].step == PP2_PLAYER_STEP_CHARACTER_WAIT || pp2_player[pp2_client_game->controller[i]->port].step == PP2_PLAYER_STEP_CHARACTER_FOUND))
				{
					t3f_play_sample(pp2_sample[PP2_SAMPLE_MENU_NEXT], 1.0, 0.0, 1.0);
					pp2_player[pp2_client_game->controller[i]->port].character_choosing--;
					if(pp2_player[pp2_client_game->controller[i]->port].character_choosing < 0)
					{
						pp2_player[pp2_client_game->controller[i]->port].character_choosing = pp2_client_game->content_list[PP2_CONTENT_CHARACTERS]->count - 1;
					}
					pp2_player[pp2_client_game->controller[i]->port].step = PP2_PLAYER_STEP_CHARACTER_WAIT;
					joynet_update_player_options(pp2_client_game, pp2_client_game->controller[i]->port);
					joynet_select_game_content(pp2_client_game, pp2_client_game->controller[i]->port, PP2_CONTENT_CHARACTERS, pp2_client_game->content_list[PP2_CONTENT_CHARACTERS]->hash[pp2_player[pp2_client_game->controller[i]->port].character_choosing]);
				}
				else if(pp2_controller[i]->state[PP2_CONTROLLER_RIGHT].pressed && (pp2_player[pp2_client_game->controller[i]->port].step == PP2_PLAYER_STEP_SELECT_CHARACTER || pp2_player[pp2_client_game->controller[i]->port].step == PP2_PLAYER_STEP_SELECTED_CHARACTER || pp2_player[pp2_client_game->controller[i]->port].step == PP2_PLAYER_STEP_CHARACTER_WAIT || pp2_player[pp2_client_game->controller[i]->port].step == PP2_PLAYER_STEP_CHARACTER_FOUND))
				{
					t3f_play_sample(pp2_sample[PP2_SAMPLE_MENU_NEXT], 1.0, 0.0, 1.0);
					pp2_player[pp2_client_game->controller[i]->port].character_choosing++;
					if(pp2_player[pp2_client_game->controller[i]->port].character_choosing >= pp2_client_game->content_list[PP2_CONTENT_CHARACTERS]->count)
					{
						pp2_player[pp2_client_game->controller[i]->port].character_choosing = 0;
					}
					pp2_player[pp2_client_game->controller[i]->port].step = PP2_PLAYER_STEP_CHARACTER_WAIT;
					joynet_update_player_options(pp2_client_game, pp2_client_game->controller[i]->port);
					joynet_select_game_content(pp2_client_game, pp2_client_game->controller[i]->port, PP2_CONTENT_CHARACTERS, pp2_client_game->content_list[PP2_CONTENT_CHARACTERS]->hash[pp2_player[pp2_client_game->controller[i]->port].character_choosing]);
				}
				else if(pp2_controller[i]->state[PP2_CONTROLLER_UP].pressed && (pp2_player[pp2_client_game->controller[i]->port].step == PP2_PLAYER_STEP_SELECT_CHARACTER || pp2_player[pp2_client_game->controller[i]->port].step == PP2_PLAYER_STEP_SELECTED_CHARACTER || pp2_player[pp2_client_game->controller[i]->port].step == PP2_PLAYER_STEP_CHARACTER_WAIT || pp2_player[pp2_client_game->controller[i]->port].step == PP2_PLAYER_STEP_CHARACTER_FOUND))
				{
					t3f_play_sample(pp2_sample[PP2_SAMPLE_MENU_NEXT], 1.0, 0.0, 1.0);
					pp2_player[pp2_client_game->controller[i]->port].character_choosing = rand() % pp2_client_game->content_list[PP2_CONTENT_CHARACTERS]->count;
					pp2_player[pp2_client_game->controller[i]->port].step = PP2_PLAYER_STEP_CHARACTER_WAIT;
					joynet_update_player_options(pp2_client_game, pp2_client_game->controller[i]->port);
					joynet_select_game_content(pp2_client_game, pp2_client_game->controller[i]->port, PP2_CONTENT_CHARACTERS, pp2_client_game->content_list[PP2_CONTENT_CHARACTERS]->hash[pp2_player[pp2_client_game->controller[i]->port].character_choosing]);
				}
			}
			else if(pp2_controller[i]->state[PP2_CONTROLLER_JUMP].pressed)
			{
				pp2_menu_proc_overlay_back(NULL, 0, NULL);
			}
		}
	}
	if(!pp2_client || pp2_client->master)
	{
		t3f_process_gui(pp2_menu[PP2_MENU_PLAYER_SETUP_OVERLAY], NULL);
	}
	else
	{
		t3f_process_gui(pp2_menu[PP2_MENU_CLIENT_PLAYER_SETUP_OVERLAY], NULL);
	}
}

void pp2_player_setup_render(void)
{
	int cx[4] = {0, 320, 320, 0};
	int cy[4] = {0, 240, 0, 240};
	int ix[2] = {0, 320};
	int tw = 640 / al_get_bitmap_width(pp2_bitmap[PP2_BITMAP_MENU_BG]) + 1;
	int th = 480 / al_get_bitmap_height(pp2_bitmap[PP2_BITMAP_MENU_BG]) + 2;
	int i, j;
	
	al_hold_bitmap_drawing(true);
	for(i = 0; i < th; i++)
	{
		for(j = 0; j < tw; j++)
		{
			al_draw_tinted_bitmap(pp2_bitmap[PP2_BITMAP_MENU_BG], al_map_rgba_f(0.1, 0.1, 0.25, 1.0), (float)(j * al_get_bitmap_width(pp2_bitmap[PP2_BITMAP_MENU_BG])) + pp2_menu_offset, (float)(i * al_get_bitmap_height(pp2_bitmap[PP2_BITMAP_MENU_BG])) + pp2_menu_offset, 0);
		}
	}
	
	al_draw_text(pp2_font[PP2_FONT_COMIC_16], al_map_rgba_f(0.0, 0.0, 0.0, 1.0), 320 + 2, 0 + 2, ALLEGRO_ALIGN_CENTRE, "Player Setup");
	al_draw_text(pp2_font[PP2_FONT_COMIC_16], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), 320, 0, ALLEGRO_ALIGN_CENTRE, "Player Setup");
	al_hold_bitmap_drawing(false);
	if(pp2_client_game->client)
	{
		for(i = 0; i < PP2_MAX_PLAYERS; i++)
		{
			if(pp2_client_game->player[i]->playing)
			{
				switch(pp2_player[i].step)
				{
					case PP2_PLAYER_STEP_SELECT_PROFILE:
					{
						if(pp2_client_game->player[i]->local)
						{
							al_draw_textf(pp2_font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), ix[i / 8], (i % 8) * 48 + 64, 0, "%02d - < %s > - ", i + 1, pp2_profiles.item[pp2_player[i].profile_choice].name);
						}
						else
						{
							al_draw_textf(pp2_font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), ix[i / 8], (i % 8) * 48 + 64, 0, "%02d - < %s > - ", i + 1, pp2_client_game->player[i]->name);
						}
						break;
					}
					case PP2_PLAYER_STEP_SELECT_CHARACTER:
					case PP2_PLAYER_STEP_SELECTED_CHARACTER:
					case PP2_PLAYER_STEP_CHARACTER_FOUND:
					{
						pp2_render_character_preview(pp2_player_preview[i], al_map_rgba_f(0.5, 0.5, 0.5, 1.0), ix[i / 8] + 240 - pp2_player_preview[i]->cx, (i % 8) * 48 + 76 - pp2_player_preview[i]->cy, 0.0);
						al_draw_textf(pp2_font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), ix[i / 8], (i % 8) * 48 + 64, 0, "%02d - %s - < %s >", i + 1, pp2_client_game->player[i]->name, pp2_player_preview[i]->name);
						break;
					}
					case PP2_PLAYER_STEP_CHARACTER_WAIT:
					{
						pp2_render_character_preview(pp2_player_preview[i], al_map_rgba_f(0.5, 0.5, 0.5, 0.5), ix[i / 8] + 240 - pp2_player_preview[i]->cx, (i % 8) * 48 + 76 - pp2_player_preview[i]->cy, 0.0);
						al_draw_textf(pp2_font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), ix[i / 8], (i % 8) * 48 + 64, 0, "%02d - %s - < %s >", i + 1, pp2_client_game->player[i]->name, pp2_player_preview[i]->name);
						break;
					}
					case PP2_PLAYER_STEP_DONE:
					{
						if(pp2_player_preview[i])
						{
							pp2_render_character_preview(pp2_player_preview[i], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), ix[i / 8] + 240 - pp2_player_preview[i]->cx, (i % 8) * 48 + 76 - pp2_player_preview[i]->cy, 0.0);
						}
						al_draw_textf(pp2_font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), ix[i / 8], (i % 8) * 48 + 64, 0, "%02d - %s - %s", i + 1, pp2_client_game->player[i]->name, pp2_player_preview[i]->name);
						break;
					}
				}
			}
			else
			{
				al_draw_textf(pp2_font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), ix[i / 8], (i % 8) * 48 + 64, 0, "%02d - N/A", i + 1);
			}
		}
	}
	else
	{
		for(i = 0; i < 4; i++)
		{
			if(pp2_client_game->player[i]->playing)
			{
				switch(pp2_player[i].step)
				{
					case PP2_PLAYER_STEP_SELECT_PROFILE:
					{
						al_draw_textf(pp2_font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), cx[i] + 160, cy[i] + 64, ALLEGRO_ALIGN_CENTRE, "< %s >", pp2_profiles.item[pp2_player[i].profile_choice].name);
						break;
					}
					case PP2_PLAYER_STEP_SELECT_CHARACTER:
					case PP2_PLAYER_STEP_SELECTED_CHARACTER:
					case PP2_PLAYER_STEP_CHARACTER_FOUND:
					{
						pp2_render_character_preview(pp2_player_preview[i], al_map_rgba_f(0.5, 0.5, 0.5, 1.0), cx[i] + 160 - pp2_player_preview[i]->cx, cy[i] + 128 - pp2_player_preview[i]->cy, 0.0);
						al_draw_textf(pp2_font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), cx[i] + 160, cy[i] + 64, ALLEGRO_ALIGN_CENTRE, "%s", pp2_profiles.item[pp2_player[i].profile_choice].name);
						al_draw_textf(pp2_font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), cx[i] + 160, cy[i] + 176, ALLEGRO_ALIGN_CENTRE, "< %s >", pp2_player_preview[i]->name);
						break;
					}
					case PP2_PLAYER_STEP_DONE:
					{
						pp2_render_character_preview(pp2_player_preview[i], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), cx[i] + 160 - pp2_player_preview[i]->cx, cy[i] + 128 - pp2_player_preview[i]->cy, 0.0);
						al_draw_textf(pp2_font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), cx[i] + 160, cy[i] + 64, ALLEGRO_ALIGN_CENTRE, "%s", pp2_profiles.item[pp2_player[i].profile_choice].name);
						al_draw_textf(pp2_font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), cx[i] + 160, cy[i] + 176, ALLEGRO_ALIGN_CENTRE, "%s", pp2_player_preview[i]->name);
						break;
					}
				}
			}
			else
			{
				if(pp2_tick % 30 < 15)
				{
					al_draw_textf(pp2_font[PP2_FONT_COMIC_16], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), t3f_project_x(cx[i] + 160, 0), t3f_project_y(cy[i] + 120, 0) - al_get_font_line_height(pp2_font[PP2_FONT_SMALL]) / 2, ALLEGRO_ALIGN_CENTRE, "Press Fire");
				}
			}
		}
	}
	if(!pp2_client || pp2_client->master)
	{
		t3f_render_gui(pp2_menu[PP2_MENU_PLAYER_SETUP_OVERLAY]);
	}
	else
	{
		t3f_render_gui(pp2_menu[PP2_MENU_CLIENT_PLAYER_SETUP_OVERLAY]);
	}
}
