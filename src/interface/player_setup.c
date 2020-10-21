#include "../t3f/t3f.h"
#include "../t3f/sound.h"
#include "../data.h"
#include "../text_entry.h"
#include "../gameplay/game.h"
#include "../gameplay/game_struct.h"
#include "../file/database/character.h"
#include "../file/database/level.h"
#include "menu_proc.h"
#include "player_setup.h"
#include "../resource.h"
#include "../pp2.h"

void pp2_player_setup_reset(PP2_GAME * gp)
{
	int i;

	for(i = 0; i < PP2_MAX_PLAYERS; i++)
	{
		gp->player[i].step = PP2_PLAYER_STEP_SELECT_PROFILE;
	}
}

void pp2_player_setup_logic(PP2_INTERFACE * ip, PP2_GAME * gp, PP2_INSTANCE * instance)
{
	int i;

	ip->menu_offset -= 0.25;
	if(ip->menu_offset <= -64.0)
	{
		ip->menu_offset = 0.0;
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
					switch(gp->player[pp2_client_game->controller[i]->port].step)
					{
						case PP2_PLAYER_STEP_SELECT_PROFILE:
						{
							if(gp->player[pp2_client_game->controller[i]->port].profile_choice == ip->profiles.items)
							{
								pp2_entered_text[0] = 0;
								pp2_entering_text = 1;
								pp2_entering_text_pos = 0;
								t3f_clear_keys();
								pp2_state = PP2_STATE_MENU;
								pp2_select_menu(ip, PP2_MENU_NEW_PROFILE);
								pp2_menu_joystick_disabled = true;
							}
							else
							{
								gp->player[pp2_client_game->controller[i]->port].profile = &ip->profiles.item[gp->player[pp2_client_game->controller[i]->port].profile_choice];
								strcpy(pp2_client_game->player[pp2_client_game->controller[i]->port]->name, ip->profiles.item[gp->player[pp2_client_game->controller[i]->port].profile_choice].name);
								t3f_play_sample(pp2_sample[PP2_SAMPLE_MENU_PICK], 1.0, 0.0, 1.0);
								gp->player[pp2_client_game->controller[i]->port].step = PP2_PLAYER_STEP_SELECTED_PROFILE;
								joynet_update_player_options(pp2_client_game, pp2_client_game->controller[i]->port);

								/* select the level if this client is the master */
								if(!pp2_client || pp2_client->master)
								{
									joynet_select_game_content(pp2_client_game, pp2_client_game->controller[i]->port, PP2_CONTENT_LEVELS, ip->level_hash);
								}
							}
							break;
						}
						case PP2_PLAYER_STEP_CHARACTER_FOUND:
						{
	//						joynet_select_game_content(pp2_client_game, pp2_client_game->controller[i]->port, PP2_CONTENT_CHARACTERS, pp2_client_game->content_list[PP2_CONTENT_CHARACTERS]->hash[gp->player[pp2_client_game->controller[i]->port].character_choosing]);
							gp->player[pp2_client_game->controller[i]->port].step = PP2_PLAYER_STEP_DONE;
							joynet_update_player_options(pp2_client_game, pp2_client_game->controller[i]->port);
							break;
						}
						case PP2_PLAYER_STEP_DONE:
						{

							/* only master can proceed to next menu */
							if(!pp2_client || pp2_client->master)
							{
								pp2_menu_proc_overlay_next(instance, 0, NULL);
								pp2_menu[pp2_current_menu]->hover_element = 0;
							}
						}
					}
				}
				else if(pp2_controller[i]->state[PP2_CONTROLLER_JUMP].pressed)
				{
					switch(gp->player[pp2_client_game->controller[i]->port].step)
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
							gp->player[pp2_client_game->controller[i]->port].step = PP2_PLAYER_STEP_SELECT_PROFILE;
							joynet_update_player_options(pp2_client_game, pp2_client_game->controller[i]->port);
							break;
						}
						case PP2_PLAYER_STEP_DONE:
						{
							if(ip->player_preview[pp2_client_game->controller[i]->port]->sound)
							{
								al_stop_sample(&ip->player_preview[pp2_client_game->controller[i]->port]->sound_id);
							}
							gp->player[pp2_client_game->controller[i]->port].step = PP2_PLAYER_STEP_CHARACTER_FOUND;
							joynet_update_player_options(pp2_client_game, pp2_client_game->controller[i]->port);
							break;
						}
					}
				}
				else if(pp2_controller[i]->state[PP2_CONTROLLER_LEFT].pressed && gp->player[pp2_client_game->controller[i]->port].step == PP2_PLAYER_STEP_SELECT_PROFILE)
				{
					t3f_play_sample(pp2_sample[PP2_SAMPLE_MENU_NEXT], 1.0, 0.0, 1.0);
					gp->player[pp2_client_game->controller[i]->port].profile_choice--;
					if(gp->player[pp2_client_game->controller[i]->port].profile_choice < 0)
					{
						gp->player[pp2_client_game->controller[i]->port].profile_choice = ip->profiles.items;
					}
					gp->player[pp2_client_game->controller[i]->port].profile = &ip->profiles.item[gp->player[pp2_client_game->controller[i]->port].profile_choice];
				}
				else if(pp2_controller[i]->state[PP2_CONTROLLER_RIGHT].pressed && gp->player[pp2_client_game->controller[i]->port].step == PP2_PLAYER_STEP_SELECT_PROFILE)
				{
					t3f_play_sample(pp2_sample[PP2_SAMPLE_MENU_NEXT], 1.0, 0.0, 1.0);
					gp->player[pp2_client_game->controller[i]->port].profile_choice++;
					if(gp->player[pp2_client_game->controller[i]->port].profile_choice > ip->profiles.items)
					{
						gp->player[pp2_client_game->controller[i]->port].profile_choice = 0;
					}
					gp->player[pp2_client_game->controller[i]->port].profile = &ip->profiles.item[gp->player[pp2_client_game->controller[i]->port].profile_choice];
				}
				else if(pp2_controller[i]->state[PP2_CONTROLLER_LEFT].pressed && (gp->player[pp2_client_game->controller[i]->port].step == PP2_PLAYER_STEP_SELECT_CHARACTER || gp->player[pp2_client_game->controller[i]->port].step == PP2_PLAYER_STEP_SELECTED_CHARACTER || gp->player[pp2_client_game->controller[i]->port].step == PP2_PLAYER_STEP_CHARACTER_WAIT || gp->player[pp2_client_game->controller[i]->port].step == PP2_PLAYER_STEP_CHARACTER_FOUND))
				{
					t3f_play_sample(pp2_sample[PP2_SAMPLE_MENU_NEXT], 1.0, 0.0, 1.0);
					gp->player[pp2_client_game->controller[i]->port].character_choosing--;
					if(gp->player[pp2_client_game->controller[i]->port].character_choosing < 0)
					{
						gp->player[pp2_client_game->controller[i]->port].character_choosing = pp2_client_game->content_list[PP2_CONTENT_CHARACTERS]->count - 1;
					}
					gp->player[pp2_client_game->controller[i]->port].step = PP2_PLAYER_STEP_CHARACTER_WAIT;
					joynet_update_player_options(pp2_client_game, pp2_client_game->controller[i]->port);
					joynet_select_game_content(pp2_client_game, pp2_client_game->controller[i]->port, PP2_CONTENT_CHARACTERS, pp2_client_game->content_list[PP2_CONTENT_CHARACTERS]->hash[gp->player[pp2_client_game->controller[i]->port].character_choosing]);
				}
				else if(pp2_controller[i]->state[PP2_CONTROLLER_RIGHT].pressed && (gp->player[pp2_client_game->controller[i]->port].step == PP2_PLAYER_STEP_SELECT_CHARACTER || gp->player[pp2_client_game->controller[i]->port].step == PP2_PLAYER_STEP_SELECTED_CHARACTER || gp->player[pp2_client_game->controller[i]->port].step == PP2_PLAYER_STEP_CHARACTER_WAIT || gp->player[pp2_client_game->controller[i]->port].step == PP2_PLAYER_STEP_CHARACTER_FOUND))
				{
					t3f_play_sample(pp2_sample[PP2_SAMPLE_MENU_NEXT], 1.0, 0.0, 1.0);
					gp->player[pp2_client_game->controller[i]->port].character_choosing++;
					if(gp->player[pp2_client_game->controller[i]->port].character_choosing >= pp2_client_game->content_list[PP2_CONTENT_CHARACTERS]->count)
					{
						gp->player[pp2_client_game->controller[i]->port].character_choosing = 0;
					}
					gp->player[pp2_client_game->controller[i]->port].step = PP2_PLAYER_STEP_CHARACTER_WAIT;
					joynet_update_player_options(pp2_client_game, pp2_client_game->controller[i]->port);
					joynet_select_game_content(pp2_client_game, pp2_client_game->controller[i]->port, PP2_CONTENT_CHARACTERS, pp2_client_game->content_list[PP2_CONTENT_CHARACTERS]->hash[gp->player[pp2_client_game->controller[i]->port].character_choosing]);
				}
				else if(pp2_controller[i]->state[PP2_CONTROLLER_UP].pressed && (gp->player[pp2_client_game->controller[i]->port].step == PP2_PLAYER_STEP_SELECT_CHARACTER || gp->player[pp2_client_game->controller[i]->port].step == PP2_PLAYER_STEP_SELECTED_CHARACTER || gp->player[pp2_client_game->controller[i]->port].step == PP2_PLAYER_STEP_CHARACTER_WAIT || gp->player[pp2_client_game->controller[i]->port].step == PP2_PLAYER_STEP_CHARACTER_FOUND))
				{
					t3f_play_sample(pp2_sample[PP2_SAMPLE_MENU_NEXT], 1.0, 0.0, 1.0);
					gp->player[pp2_client_game->controller[i]->port].character_choosing = rand() % pp2_client_game->content_list[PP2_CONTENT_CHARACTERS]->count;
					gp->player[pp2_client_game->controller[i]->port].step = PP2_PLAYER_STEP_CHARACTER_WAIT;
					joynet_update_player_options(pp2_client_game, pp2_client_game->controller[i]->port);
					joynet_select_game_content(pp2_client_game, pp2_client_game->controller[i]->port, PP2_CONTENT_CHARACTERS, pp2_client_game->content_list[PP2_CONTENT_CHARACTERS]->hash[gp->player[pp2_client_game->controller[i]->port].character_choosing]);
				}
			}
			else if(pp2_controller[i]->state[PP2_CONTROLLER_JUMP].pressed)
			{
				pp2_menu_proc_overlay_back(instance, 0, NULL);
			}
		}
	}
	if(!pp2_client || pp2_client->master)
	{
		t3f_process_gui(pp2_menu[PP2_MENU_PLAYER_SETUP_OVERLAY], instance);
	}
	else
	{
		t3f_process_gui(pp2_menu[PP2_MENU_CLIENT_PLAYER_SETUP_OVERLAY], instance);
	}
}

void pp2_player_setup_render(PP2_INTERFACE * ip, PP2_GAME * gp, PP2_RESOURCES * resources)
{
	int cx[4] = {0, PP2_SCREEN_WIDTH / 2, PP2_SCREEN_WIDTH / 2, 0};
	int cy[4] = {0, PP2_SCREEN_HEIGHT / 2, 0, PP2_SCREEN_HEIGHT / 2};
	int ix[2] = {0, PP2_SCREEN_WIDTH / 2};
	float cw = PP2_SCREEN_WIDTH / 4.0;
	float ch = PP2_SCREEN_HEIGHT / 4.0;
	int tw = PP2_SCREEN_WIDTH / al_get_bitmap_width(resources->bitmap[PP2_BITMAP_MENU_BG]) + 1;
	int th = PP2_SCREEN_HEIGHT / al_get_bitmap_height(resources->bitmap[PP2_BITMAP_MENU_BG]) + 2;
	int i, j;

	al_hold_bitmap_drawing(true);
	for(i = 0; i < th; i++)
	{
		for(j = 0; j < tw; j++)
		{
			al_draw_tinted_bitmap(resources->bitmap[PP2_BITMAP_MENU_BG], al_map_rgba_f(0.1, 0.1, 0.25, 1.0), (float)(j * al_get_bitmap_width(resources->bitmap[PP2_BITMAP_MENU_BG])) + ip->menu_offset, (float)(i * al_get_bitmap_height(resources->bitmap[PP2_BITMAP_MENU_BG])) + ip->menu_offset, 0);
		}
	}

	al_draw_text(resources->font[PP2_FONT_COMIC_16], al_map_rgba_f(0.0, 0.0, 0.0, 1.0), PP2_SCREEN_WIDTH / 2 + 2, 0 + 2, ALLEGRO_ALIGN_CENTRE, "Player Setup");
	al_draw_text(resources->font[PP2_FONT_COMIC_16], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), PP2_SCREEN_WIDTH / 2, 0, ALLEGRO_ALIGN_CENTRE, "Player Setup");
	al_hold_bitmap_drawing(false);
	if(pp2_client_game->client)
	{
		for(i = 0; i < PP2_MAX_PLAYERS; i++)
		{
			if(pp2_client_game->player[i]->playing)
			{
				switch(gp->player[i].step)
				{
					case PP2_PLAYER_STEP_SELECT_PROFILE:
					{
						if(pp2_client_game->player[i]->local)
						{
							al_draw_textf(resources->font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), ix[i / 8], (i % 8) * 48 + 64, 0, "%02d - < %s > - ", i + 1, ip->profiles.item[gp->player[i].profile_choice].name);
						}
						else
						{
							al_draw_textf(resources->font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), ix[i / 8], (i % 8) * 48 + 64, 0, "%02d - < %s > - ", i + 1, pp2_client_game->player[i]->name);
						}
						break;
					}
					case PP2_PLAYER_STEP_SELECT_CHARACTER:
					case PP2_PLAYER_STEP_SELECTED_CHARACTER:
					case PP2_PLAYER_STEP_CHARACTER_FOUND:
					{
						pp2_render_character_preview(ip->player_preview[i], al_map_rgba_f(0.5, 0.5, 0.5, 1.0), ix[i / 8] + 240 - ip->player_preview[i]->cx, (i % 8) * 48 + 76 - ip->player_preview[i]->cy, 0.0);
						al_draw_textf(resources->font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), ix[i / 8], (i % 8) * 48 + 64, 0, "%02d - %s - < %s >", i + 1, pp2_client_game->player[i]->name, ip->player_preview[i]->name);
						break;
					}
					case PP2_PLAYER_STEP_CHARACTER_WAIT:
					{
						pp2_render_character_preview(ip->player_preview[i], al_map_rgba_f(0.5, 0.5, 0.5, 0.5), ix[i / 8] + 240 - ip->player_preview[i]->cx, (i % 8) * 48 + 76 - ip->player_preview[i]->cy, 0.0);
						al_draw_textf(resources->font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), ix[i / 8], (i % 8) * 48 + 64, 0, "%02d - %s - < %s >", i + 1, pp2_client_game->player[i]->name, ip->player_preview[i]->name);
						break;
					}
					case PP2_PLAYER_STEP_DONE:
					{
						if(ip->player_preview[i])
						{
							pp2_render_character_preview(ip->player_preview[i], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), ix[i / 8] + 240 - ip->player_preview[i]->cx, (i % 8) * 48 + 76 - ip->player_preview[i]->cy, 0.0);
						}
						al_draw_textf(resources->font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), ix[i / 8], (i % 8) * 48 + 64, 0, "%02d - %s - %s", i + 1, pp2_client_game->player[i]->name, ip->player_preview[i]->name);
						break;
					}
				}
			}
			else
			{
				al_draw_textf(resources->font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), ix[i / 8], (i % 8) * 48 + 64, 0, "%02d - N/A", i + 1);
			}
		}
	}
	else
	{
		for(i = 0; i < 4; i++)
		{
			if(pp2_client_game->player[i]->playing)
			{
				switch(gp->player[i].step)
				{
					case PP2_PLAYER_STEP_SELECT_PROFILE:
					{
						al_draw_textf(resources->font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), cx[i] + cw, cy[i] + 64, ALLEGRO_ALIGN_CENTRE, "< %s >", ip->profiles.item[gp->player[i].profile_choice].name);
						break;
					}
					case PP2_PLAYER_STEP_SELECT_CHARACTER:
					case PP2_PLAYER_STEP_SELECTED_CHARACTER:
					case PP2_PLAYER_STEP_CHARACTER_FOUND:
					{
						pp2_render_character_preview(ip->player_preview[i], al_map_rgba_f(0.5, 0.5, 0.5, 1.0), cx[i] + cw - ip->player_preview[i]->cx, cy[i] + 128 - ip->player_preview[i]->cy, 0.0);
						al_draw_textf(resources->font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), cx[i] + cw, cy[i] + 64, ALLEGRO_ALIGN_CENTRE, "%s", ip->profiles.item[gp->player[i].profile_choice].name);
						al_draw_textf(resources->font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), cx[i] + cw, cy[i] + 176, ALLEGRO_ALIGN_CENTRE, "< %s >", ip->player_preview[i]->name);
						break;
					}
					case PP2_PLAYER_STEP_DONE:
					{
						pp2_render_character_preview(ip->player_preview[i], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), cx[i] + cw - ip->player_preview[i]->cx, cy[i] + 128 - ip->player_preview[i]->cy, 0.0);
						al_draw_textf(resources->font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), cx[i] + cw, cy[i] + 64, ALLEGRO_ALIGN_CENTRE, "%s", ip->profiles.item[gp->player[i].profile_choice].name);
						al_draw_textf(resources->font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), cx[i] + cw, cy[i] + 176, ALLEGRO_ALIGN_CENTRE, "%s", ip->player_preview[i]->name);
						break;
					}
				}
			}
			else
			{
				if(pp2_tick % 30 < 15)
				{
					al_draw_textf(resources->font[PP2_FONT_COMIC_16], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), t3f_project_x(cx[i] + cw, 0), t3f_project_y(cy[i] + ch, 0) - al_get_font_line_height(resources->font[PP2_FONT_SMALL]) / 2, ALLEGRO_ALIGN_CENTRE, "Press Fire");
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
