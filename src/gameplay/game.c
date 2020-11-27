#include "t3f/tilemap.h"
#include "t3f/t3f.h"
#include "t3f/sound.h"
#include "t3f/draw.h"
#include "../joynet/joynet.h"
#include "../legacy/level.h"
#include "../legacy/character.h"
#include "../file/music.h"
#include "../init.h"
#include "../misc/date.h"
#include "../interface/menu.h"
#include "sprites/player.h"
#include "sprites/paintball.h"
#include "sprites/paintball_defines.h"
#include "sprites/particle.h"
#include "sprites/object_defines.h"
#include "game.h"
#include "game_defines.h"
#include "rules.h"
#include "scoreboard.h"
#include "replay.h"
#include "../resource.h"
#include "../pp2.h"
#include "sprites/objects.h"

char * chared_state_name[PP2_CHARACTER_MAX_STATES] =
{
	"PP2_CHARACTER_STATE_STAND_R_R",
	"PP2_CHARACTER_STATE_STAND_R_DR",
	"PP2_CHARACTER_STATE_STAND_R_D",
	"PP2_CHARACTER_STATE_STAND_R_DL",
	"PP2_CHARACTER_STATE_STAND_R_L",
	"PP2_CHARACTER_STATE_STAND_R_UL",
	"PP2_CHARACTER_STATE_STAND_R_U",
	"PP2_CHARACTER_STATE_STAND_R_UR",
	"PP2_CHARACTER_STATE_STAND_L_R",
	"PP2_CHARACTER_STATE_STAND_L_DR",
	"PP2_CHARACTER_STATE_STAND_L_D",
	"PP2_CHARACTER_STATE_STAND_L_DL",
	"PP2_CHARACTER_STATE_STAND_L_L",
	"PP2_CHARACTER_STATE_STAND_L_UL",
	"PP2_CHARACTER_STATE_STAND_L_U",
	"PP2_CHARACTER_STATE_STAND_L_UR",
	"PP2_CHARACTER_STATE_WALK_R_R",
	"PP2_CHARACTER_STATE_WALK_R_DR",
	"PP2_CHARACTER_STATE_WALK_R_D",
	"PP2_CHARACTER_STATE_WALK_R_DL",
	"PP2_CHARACTER_STATE_WALK_R_L",
	"PP2_CHARACTER_STATE_WALK_R_UL",
	"PP2_CHARACTER_STATE_WALK_R_U",
	"PP2_CHARACTER_STATE_WALK_R_UR",
	"PP2_CHARACTER_STATE_WALK_L_R",
	"PP2_CHARACTER_STATE_WALK_L_DR",
	"PP2_CHARACTER_STATE_WALK_L_D",
	"PP2_CHARACTER_STATE_WALK_L_DL",
	"PP2_CHARACTER_STATE_WALK_L_L",
	"PP2_CHARACTER_STATE_WALK_L_UL",
	"PP2_CHARACTER_STATE_WALK_L_U",
	"PP2_CHARACTER_STATE_WALK_L_UR",
	"PP2_CHARACTER_STATE_JUMP_R_R",
	"PP2_CHARACTER_STATE_JUMP_R_DR",
	"PP2_CHARACTER_STATE_JUMP_R_D",
	"PP2_CHARACTER_STATE_JUMP_R_DL",
	"PP2_CHARACTER_STATE_JUMP_R_L",
	"PP2_CHARACTER_STATE_JUMP_R_UL",
	"PP2_CHARACTER_STATE_JUMP_R_U",
	"PP2_CHARACTER_STATE_JUMP_R_UR",
	"PP2_CHARACTER_STATE_JUMP_L_R",
	"PP2_CHARACTER_STATE_JUMP_L_DR",
	"PP2_CHARACTER_STATE_JUMP_L_D",
	"PP2_CHARACTER_STATE_JUMP_L_DL",
	"PP2_CHARACTER_STATE_JUMP_L_L",
	"PP2_CHARACTER_STATE_JUMP_L_UL",
	"PP2_CHARACTER_STATE_JUMP_L_U",
	"PP2_CHARACTER_STATE_JUMP_L_UR",
	"PP2_CHARACTER_STATE_FALL_R_R",
	"PP2_CHARACTER_STATE_FALL_R_DR",
	"PP2_CHARACTER_STATE_FALL_R_D",
	"PP2_CHARACTER_STATE_FALL_R_DL",
	"PP2_CHARACTER_STATE_FALL_R_L",
	"PP2_CHARACTER_STATE_FALL_R_UL",
	"PP2_CHARACTER_STATE_FALL_R_U",
	"PP2_CHARACTER_STATE_FALL_R_UR",
	"PP2_CHARACTER_STATE_FALL_L_R",
	"PP2_CHARACTER_STATE_FALL_L_DR",
	"PP2_CHARACTER_STATE_FALL_L_D",
	"PP2_CHARACTER_STATE_FALL_L_DL",
	"PP2_CHARACTER_STATE_FALL_L_L",
	"PP2_CHARACTER_STATE_FALL_L_UL",
	"PP2_CHARACTER_STATE_FALL_L_U",
	"PP2_CHARACTER_STATE_FALL_L_UR",
	"PP2_CHARACTER_STATE_DUCK_R_R",
	"PP2_CHARACTER_STATE_DUCK_R_DR",
	"PP2_CHARACTER_STATE_DUCK_R_D",
	"PP2_CHARACTER_STATE_DUCK_R_DL",
	"PP2_CHARACTER_STATE_DUCK_R_L",
	"PP2_CHARACTER_STATE_DUCK_R_UL",
	"PP2_CHARACTER_STATE_DUCK_R_U",
	"PP2_CHARACTER_STATE_DUCK_R_UR",
	"PP2_CHARACTER_STATE_DUCK_L_R",
	"PP2_CHARACTER_STATE_DUCK_L_DR",
	"PP2_CHARACTER_STATE_DUCK_L_D",
	"PP2_CHARACTER_STATE_DUCK_L_DL",
	"PP2_CHARACTER_STATE_DUCK_L_L",
	"PP2_CHARACTER_STATE_DUCK_L_UL",
	"PP2_CHARACTER_STATE_DUCK_L_U",
	"PP2_CHARACTER_STATE_DUCK_L_UR"
};

static bool pp2_camera_clamp_left(PP2_GAME * gp, int i)
{
	if(gp->player[i].camera.x < gp->level->room.x * 32 - gp->player[i].view->left)
	{
		gp->player[i].camera.x = gp->level->room.x * 32 - gp->player[i].view->left;
		return true;
	}
	return false;
}

static bool pp2_camera_clamp_right(PP2_GAME * gp, int i)
{
	float right_offset = gp->player[i].view->virtual_width - (gp->player[i].view->virtual_width - (gp->player[i].view->right - gp->player[i].view->left)) / 2;

	if(gp->player[i].camera.x + right_offset > gp->level->room.bx * 32 + 32)
	{
		gp->player[i].camera.x = gp->level->room.bx * 32 + 32 - right_offset;
		return true;
	}
	return false;
}

static bool pp2_camera_clamp_top(PP2_GAME * gp, int i)
{
	if(gp->player[i].camera.y < gp->level->room.y * 32 - gp->player[i].view->top)
	{
		gp->player[i].camera.y = gp->level->room.y * 32 - gp->player[i].view->top;
		return true;
	}
	return false;
}

static bool pp2_camera_clamp_bottom(PP2_GAME * gp, int i)
{
	float bottom_offset = gp->player[i].view->virtual_height - (gp->player[i].view->virtual_height - (gp->player[i].view->bottom - gp->player[i].view->top)) / 2;
	if(gp->player[i].camera.y + bottom_offset > gp->level->room.by * 32 + 32)
	{
		gp->player[i].camera.y = gp->level->room.by * 32 + 32 - bottom_offset;
		return true;
	}
	return false;
}

void pp2_camera_logic(PP2_GAME * gp, int i)
{
	gp->player[i].camera.x = gp->player[i].x - gp->player[i].view->virtual_width / 2 + gp->player[i].object[0]->map.top.point[0].x;
	if(!pp2_camera_clamp_left(gp, i))
	{
		if(pp2_camera_clamp_right(gp, i))
		{
			pp2_camera_clamp_left(gp, i);
		}
	}
	gp->player[i].camera.y = gp->player[i].y - gp->player[i].view->virtual_height / 2 + gp->player[i].object[0]->map.left.point[0].y;
	if(!pp2_camera_clamp_top(gp, i))
	{
		if(pp2_camera_clamp_bottom(gp, i))
		{
			pp2_camera_clamp_top(gp, i);
		}
	}
}

static void pp2_game_logic_tick(PP2_GAME * gp, PP2_INTERFACE * ip, PP2_RESOURCES * resources, PP2_THEME * tp)
{
	int i, j;

	joynet_game_logic(gp->client_game);
	gp->radar_objects = 0;
	if(gp->winner < 0 && gp->time_left > 0)
	{
		gp->time_left--;
	}
	for(i = 0; i < PP2_MAX_PLAYERS; i++)
	{
		if(gp->client_game->player[i]->playing)
		{
			if(gp->replay_file)
			{
				al_fputc(gp->replay_file, gp->client_game->player_controller[i]->bits[0]);
			}
			for(j = 0; j < 8; j++)
			{
				ip->controller[i]->state[j].down = gp->client_game->player_controller[i]->button[j];
			}
			t3f_update_controller(ip->controller[i]);
			pp2_player_logic(gp, &gp->player[i], resources);
			if(gp->option[PP2_OPTION_TRAILS])
			{
				for(j = 0; j < PP2_MAX_TRAILS; j++)
				{
					pp2_paintball_trail_logic(&gp->player[i].trail[j]);
				}
			}
			for(j = 0; j < PP2_MAX_PAINTBALLS; j++)
			{
				pp2_paintball_logic(gp, &gp->player[i].paintball[j], resources);
			}
			for(j = 0; j < PP2_MAX_PARTICLES; j++)
			{
				pp2_particle_logic(gp, &gp->particle[j]);
				pp2_particle_logic(gp, &gp->player[i].particle[j]);
			}

			if(gp->player[i].fade_time != 0 && gp->player[i].fade_type == 0)
			{
				gp->player[i].camera.z += 4.0;
			}
			else if(gp->player[i].camera.z > 0.0)
			{
				gp->player[i].camera.z -= 4.0;
			}

			/* camera logic */
			if(gp->winner < 0)
			{
				pp2_camera_logic(gp, i);
			}
		}
	}
	for(i = 0; i < gp->object_size; i++)
	{
		pp2_object_logic(gp, &gp->object[i], resources);
	}
	if(gp->winner < 0)
	{
		pp2_process_rules(gp, tp);
		if(gp->winner >= 0 && gp->player[gp->winner].character->sample[PP2_SAMPLE_WIN])
		{
			t3f_play_sample(gp->player[gp->winner].character->sample[PP2_SAMPLE_WIN], 1.0, 0.0, 1.0);
		}
	}
	gp->tick++;
}

void pp2_game_logic(PP2_GAME * gp, PP2_INTERFACE * ip, PP2_RESOURCES * resources, PP2_THEME * tp)
{
	int i, j, c;

	gp->show_scores = false;
	/* fill in local controller data and send it off */
	for(i = 0; i < 4; i++)
	{
		t3f_read_controller(ip->controller[i]);
		for(j = 0; j < 8; j++)
		{
			gp->client_game->controller[i]->button[j] = ip->controller[i]->state[j].down;
		}

		/* see if a player wants to see the scores */
		if(ip->controller[i]->state[PP2_CONTROLLER_SCORES].down)
		{
			gp->show_scores = true;
		}
	}
	joynet_update_game(gp->client_game, 1);

	c = joynet_get_input_buffer_frames(gp->client_game->input_buffer);
	if(c > 0 && c < ip->setting[PP2_SETTING_NETWORK_MAX_FRAMES])
	{
		c = 1;
	}
	for(i = 0; i < c; i++)
	{
		pp2_game_logic_tick(gp, ip, resources, tp);
	}
}

ALLEGRO_COLOR pp2_game_get_ammo_color(PP2_GAME * gp, int player, int weapon, float a)
{
	if(gp->player[player].ammo[weapon] > 5)
	{
		return al_map_rgba_f(0.0, 1.0 * a, 0.0, a);
	}
	else if(gp->player[player].ammo[weapon] > 1)
	{
		return al_map_rgba_f(1.0 * a, 1.0 * a, 0.0, a);
	}
	else
	{
		return al_map_rgba_f(1.0 * a, 0.0, 0.0, a);
	}
}

/* renders scoreboard */
void pp2_game_render_scoreboard(PP2_GAME * gp, const char * title, PP2_RESOURCES * resources)
{
	PP2_SCOREBOARD_ENTRY score[PP2_MAX_PLAYERS];
	ALLEGRO_COLOR color;
	int scores = 0;
	int i;
	float y;

	switch(gp->option[PP2_OPTION_GAME_MODE])
	{
		case PP2_GAME_MODE_ELIMINATOR:
		{
			for(i = 0; i < PP2_MAX_PLAYERS; i++)
			{
				if(gp->player[i].playing)
				{
					score[scores].player = i;
					if(gp->player[i].flags & PP2_PLAYER_FLAG_ACTIVE)
					{
						score[scores].score = 1;
					}
					else
					{
						score[scores].score = 0;
					}
					scores++;
				}
			}
			qsort(score, scores, sizeof(PP2_SCOREBOARD_ENTRY), pp2_scoreboard_sorter);
			y = 240.0 - (float)((scores + 2) * t3f_get_font_line_height(resources->font[PP2_FONT_SMALL])) / 2.0;
			break;
		}
		case PP2_GAME_MODE_DEATH_MATCH:
		{
			for(i = 0; i < PP2_MAX_PLAYERS; i++)
			{
				if(gp->player[i].playing)
				{
					score[scores].player = i;
					score[scores].score = gp->player[i].frags;
					scores++;
				}
			}
			qsort(score, scores, sizeof(PP2_SCOREBOARD_ENTRY), pp2_scoreboard_sorter);
			y = 240.0 - (float)((scores + 2) * t3f_get_font_line_height(resources->font[PP2_FONT_SMALL])) / 2.0;
			break;
		}
		case PP2_GAME_MODE_COIN_RUSH:
		{
			for(i = 0; i < PP2_MAX_PLAYERS; i++)
			{
				if(gp->player[i].playing)
				{
					score[scores].player = i;
					score[scores].score = gp->player[i].coins;
					scores++;
				}
			}
			qsort(score, scores, sizeof(PP2_SCOREBOARD_ENTRY), pp2_scoreboard_sorter);
			y = 240.0 - (float)((scores + 2) * t3f_get_font_line_height(resources->font[PP2_FONT_SMALL])) / 2.0;
			break;
		}
		default:
		{
			scores = 0;
			y = 240.0;
		}
	}

	/* render the scores */
	if(scores > 0)
	{
		t3f_select_view(NULL);
		al_hold_bitmap_drawing(false);
		al_draw_filled_rectangle(0.0, 0.0, PP2_SCREEN_WIDTH, PP2_SCREEN_HEIGHT, al_map_rgba_f(0.0, 0.0, 0.0, 0.5));
		al_hold_bitmap_drawing(true);
		t3f_draw_textf(resources->font[PP2_FONT_SMALL], al_map_rgba_f(0.0, 0.0, 0.0, 0.5), PP2_SCREEN_WIDTH / 2 + 2, y + 2, 0, ALLEGRO_ALIGN_CENTRE, "%s", title);
		t3f_draw_textf(resources->font[PP2_FONT_SMALL], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), PP2_SCREEN_WIDTH / 2, y, 0, ALLEGRO_ALIGN_CENTRE, "%s", title);
		y += t3f_get_font_line_height(resources->font[PP2_FONT_SMALL]) * 2;
		for(i = 0; i < scores; i++)
		{
			if(i == 0)
			{
				if(gp->tick % 2)
				{
					color = al_map_rgba_f(1.0, 1.0, 0.0, 1.0);
				}
				else
				{
					color = al_map_rgba_f(1.0, 1.0, 1.0, 1.0);
				}
			}
			else
			{
				color = al_map_rgba_f(0.5, 0.5, 0.5, 1.0);
			}
			t3f_draw_textf(resources->font[PP2_FONT_SMALL], al_map_rgba_f(0.0, 0.0, 0.0, 0.5), 200 + 2, y + i * t3f_get_font_line_height(resources->font[PP2_FONT_SMALL]) + 2, 0, 0, "%s", gp->player[score[i].player].name);
			t3f_draw_textf(resources->font[PP2_FONT_SMALL], al_map_rgba_f(0.0, 0.0, 0.0, 0.5), PP2_SCREEN_WIDTH / 2 + 200 + 2, y + i * t3f_get_font_line_height(resources->font[PP2_FONT_SMALL]) + 2, 0, ALLEGRO_ALIGN_RIGHT, "%d", score[i].score);
			t3f_draw_textf(resources->font[PP2_FONT_SMALL], color, 200, y + i * t3f_get_font_line_height(resources->font[PP2_FONT_SMALL]), 0, 0, "%s", gp->player[score[i].player].name);
			t3f_draw_textf(resources->font[PP2_FONT_SMALL], color, PP2_SCREEN_WIDTH / 2 + 200, y + i * t3f_get_font_line_height(resources->font[PP2_FONT_SMALL]), 0, ALLEGRO_ALIGN_RIGHT, "%d", score[i].score);
		}
		al_hold_bitmap_drawing(false);
	}
}

static void pp2_game_render_hud_weapon_type(PP2_GAME * gp, int i, int j, ALLEGRO_COLOR color, PP2_RESOURCES * resources)
{
	float start_angle = -ALLEGRO_PI / 2.0;
	float angle_step = (ALLEGRO_PI * 2.0) / 8.0;
	float cx, cy, angle;

	angle = start_angle + angle_step * (float)j;
	cx = gp->player[i].x + gp->player[i].object[0]->map.top.point[0].x;
	cy = gp->player[i].y + gp->player[i].object[0]->map.left.point[0].y;
	t3f_draw_animation(resources->animation[PP2_ANIMATION_HUD_AMMO_NORMAL + j], color, gp->tick, cx - 16.0 + 64.0 * cos(angle) - gp->player[i].camera.x, cy - 16.0 + 64.0 * sin(angle) - gp->player[i].camera.y, -gp->player[i].camera.z, 0);
}

/* renders one player's view */
void pp2_game_render_player_view(PP2_GAME * gp, int i, PP2_THEME * theme, PP2_RESOURCES * resources)
{
	int j, k;
	float cx, cy, a, s, ox = 6.0, oy = 0.0, sx = 2.0, sy = 2.0;

	t3f_select_view(gp->player[i].view);

	al_hold_bitmap_drawing(true);

	/* render the background */
	if(gp->level->bg)
	{
		t3f_draw_animation(gp->level->bg, t3f_color_white, gp->tick, 0, 0, 0, 0);
	}
	for(j = 0; j <= gp->player[i].layer; j++)
	{
		t3f_render_tilemap(gp->level->tilemap, gp->level->tileset, j, gp->tick, gp->player[i].camera.x, gp->player[i].camera.y, gp->player[i].camera.z, t3f_color_white);
	}

	/* draw game objects over background */
	for(j = 0; j < gp->object_size; j++)
	{
		pp2_object_render(&gp->object[j], &gp->player[i].camera, resources);
	}
	for(j = 0; j < PP2_MAX_PARTICLES; j++)
	{
		pp2_particle_render(gp, &gp->particle[j], &gp->player[i].camera, resources);
	}
	for(j = 0; j < PP2_MAX_PLAYERS; j++)
	{
		if(gp->option[PP2_OPTION_TRAILS])
		{
			for(k = 0; k < PP2_MAX_TRAILS; k++)
			{
				pp2_paintball_trail_render(gp, &gp->player[j].trail[k], &gp->player[i].camera);
			}
		}
		for(k = 0; k < PP2_MAX_PAINTBALLS; k++)
		{
			pp2_paintball_render(gp, &gp->player[j].paintball[k], &gp->player[i].camera, resources);
		}
	}
	for(j = 0; j < PP2_MAX_PLAYERS; j++)
	{
		pp2_player_render(&gp->player[j], &gp->player[i].camera);
	}
	for(j = 0; j < PP2_MAX_PLAYERS; j++)
	{
		for(k = 0; k < PP2_MAX_PARTICLES; k++)
		{
			pp2_particle_render(gp, &gp->player[j].particle[k], &gp->player[i].camera, resources);
		}
	}

	/* draw foreground */
	for(j = gp->player[i].layer + 1; j < gp->level->tilemap->layers; j++)
	{
		t3f_render_tilemap(gp->level->tilemap, gp->level->tileset, j, gp->tick, gp->player[i].camera.x, gp->player[i].camera.y, gp->player[i].camera.z, t3f_color_white);
	}
	if(gp->level->fg)
	{
		t3f_draw_animation(gp->level->fg, t3f_color_white, gp->tick, 0, 0, 0, 0);
	}

	/* draw the HUD */
	if(gp->player[i].target >= 0)
	{
		cx = gp->player[i].x + gp->player[i].object[0]->map.top.point[0].x;
		cy = gp->player[i].y + gp->player[i].object[0]->map.left.point[0].y;
		a = atan2((gp->player[gp->player[i].target].y + gp->player[gp->player[i].target].object[gp->player[gp->player[i].target].current_object]->map.left.point[0].y) - cy, gp->player[gp->player[i].target].x + gp->player[gp->player[i].target].object[gp->player[gp->player[i].target].current_object]->map.top.point[0].x - cx);
		al_draw_tinted_rotated_bitmap(resources->bitmap[PP2_BITMAP_TARGET], al_map_rgba_f(1.0, 0.0, 0.0, 1.0), 8 - 64, 8, cx - gp->player[i].camera.x, cy - gp->player[i].camera.y, a, 0);
	}
	if(gp->option[PP2_OPTION_GAME_MODE] == PP2_GAME_MODE_COIN_RUSH)
	{
		if(gp->player[i].coin_target)
		{
			cx = gp->player[i].x + gp->player[i].object[0]->map.top.point[0].x;
			cy = gp->player[i].y + gp->player[i].object[0]->map.left.point[0].y;
			a = atan2(gp->player[i].coin_target_y - cy, gp->player[i].coin_target_x - cx);
			al_draw_tinted_rotated_bitmap(resources->bitmap[PP2_BITMAP_TARGET], al_map_rgba_f(1.0, 1.0, 0.0, 1.0), 8 - 64, 8, cx - gp->player[i].camera.x, cy - gp->player[i].camera.y, a, 0);
		}
	}
	if(gp->player[i].choose_weapon)
	{
		ALLEGRO_COLOR wcolor;

		for(j = 0; j < PP2_PLAYER_MAX_WEAPONS; j++)
		{
			if(!gp->player[i].ammo[j])
			{
				wcolor = al_map_rgba_f(0.25, 0.25, 0.25, 0.5);
			}
			else
			{
				wcolor = t3f_color_white;
			}
			pp2_game_render_hud_weapon_type(gp, i, j, j == gp->player[i].want_weapon ? al_map_rgba_f(0.0, 1.0, 0.0, 1.0) : wcolor, resources);
		}
	}
	else
	{
		if(gp->player[i].timer[PP2_PLAYER_TIMER_WEAPON_SELECT])
		{
			pp2_game_render_hud_weapon_type(gp, i, gp->player[i].weapon, ((gp->player[i].timer[PP2_PLAYER_TIMER_WEAPON_SELECT] / 4) % 2 == 0) ? al_map_rgba_f(0.0, 1.0, 0.0, 1.0) : t3f_color_white, resources);

		}
	}
	for(j = 0; j < PP2_MAX_PLAYERS; j++)
	{
		if(gp->player[j].flags & PP2_PLAYER_FLAG_TYPING)
		{
			al_draw_bitmap(resources->bitmap[PP2_BITMAP_TYPING], gp->player[j].x + gp->player[j].object[0]->map.top.point[0].x - al_get_bitmap_width(resources->bitmap[PP2_BITMAP_TYPING]) / 2 - gp->player[i].camera.x, gp->player[j].y + gp->player[j].object[0]->map.top.point[0].y - al_get_bitmap_height(resources->bitmap[PP2_BITMAP_TYPING]) - gp->player[i].camera.y, 0);
		}
	}
	/* highlight target */
	al_hold_bitmap_drawing(false);
	if(gp->player[i].target >= 0)
	{
		j = gp->player[i].target;
		k = gp->player[gp->player[i].target].current_object;
		al_draw_rectangle(t3f_project_x(gp->player[j].x + gp->player[j].object[k]->map.left.point[0].x - 8 - gp->player[i].camera.x, -gp->player[i].camera.z), t3f_project_y(gp->player[j].y + gp->player[j].object[k]->map.top.point[0].y - 8 - gp->player[i].camera.y, -gp->player[i].camera.z), t3f_project_x(gp->player[j].x + gp->player[j].object[k]->map.right.point[0].x + 8 - gp->player[i].camera.x, -gp->player[i].camera.z), t3f_project_y(gp->player[j].y + gp->player[j].object[k]->map.bottom.point[0].y + 8 - gp->player[i].camera.y, -gp->player[i].camera.z), al_map_rgba_f(0.5, 0.0, 0.0, 0.5), 2.0);
	}
	al_hold_bitmap_drawing(true);

	/* draw player names */
	for(j = 0; j < PP2_MAX_PLAYERS; j++)
	{
		if((gp->player[j].flags & PP2_PLAYER_FLAG_ACTIVE) && ((j != i && !((gp->player[j].flags & PP2_PLAYER_FLAG_POWER_CLOAK))) || gp->replay_player >= 0))
		{
			t3f_draw_text(resources->font[PP2_FONT_SMALL], al_map_rgba_f(0.0, 0.0, 0.0, 0.5), t3f_project_x(gp->player[j].x + gp->player[j].object[0]->map.top.point[0].x - gp->player[i].camera.x, -gp->player[i].camera.z) + 2, t3f_project_y(gp->player[j].y + gp->player[j].object[0]->map.top.point[0].y - t3f_get_font_line_height(resources->font[PP2_FONT_SMALL]) - gp->player[i].camera.y, -gp->player[i].camera.z) + 2, 0, ALLEGRO_ALIGN_CENTRE, gp->player[j].name);
			t3f_draw_text(resources->font[PP2_FONT_SMALL], al_map_rgba_f(0.0, 1.0, 0.0, 1.0), t3f_project_x(gp->player[j].x + gp->player[j].object[0]->map.top.point[0].x - gp->player[i].camera.x, -gp->player[i].camera.z), t3f_project_y(gp->player[j].y + gp->player[j].object[0]->map.top.point[0].y - t3f_get_font_line_height(resources->font[PP2_FONT_SMALL]) - gp->player[i].camera.y, -gp->player[i].camera.z), 0, ALLEGRO_ALIGN_CENTRE, gp->player[j].name);
		}
	}

	switch(gp->option[PP2_OPTION_GAME_MODE])
	{
		case PP2_GAME_MODE_ELIMINATOR:
		{
			if(resources->bitmap[PP2_BITMAP_HUD_LIVES])
			{
				al_draw_bitmap(resources->bitmap[PP2_BITMAP_HUD_LIVES], gp->player[i].view->left + theme->object[PP2_THEME_OBJECT_HUD_SCORE].x, gp->player[i].view->top + theme->object[PP2_THEME_OBJECT_HUD_SCORE].y, 0);
			}
			t3f_draw_textf(resources->font[PP2_FONT_HUD], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), gp->player[i].view->left + theme->object[PP2_THEME_OBJECT_HUD_SCORE_TEXT].x, gp->player[i].view->top + theme->object[PP2_THEME_OBJECT_HUD_SCORE_TEXT].y, 0, 0, "%04d", gp->player[i].life);
//			t3f_draw_textf(resources->font[PP2_FONT_HUD], al_map_rgba_f(0.0, 0.0, 0.0, 0.5), gp->player[i].view->left + ox + sx, gp->player[i].view->top + oy + sy, 0, 0, "Ammo: %02d", gp->player[i].ammo[gp->player[i].weapon]);
			if(resources->bitmap[PP2_BITMAP_HUD_AMMO])
			{
				al_draw_bitmap(resources->bitmap[PP2_BITMAP_HUD_AMMO], gp->player[i].view->left + theme->object[PP2_THEME_OBJECT_HUD_AMMO].x, gp->player[i].view->top + theme->object[PP2_THEME_OBJECT_HUD_AMMO].y, 0);
			}
			t3f_draw_textf(resources->font[PP2_FONT_HUD], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), gp->player[i].view->left + theme->object[PP2_THEME_OBJECT_HUD_AMMO_TEXT].x, gp->player[i].view->top + theme->object[PP2_THEME_OBJECT_HUD_AMMO_TEXT].y, 0, 0, "%02d", gp->player[i].ammo[gp->player[i].weapon]);
			oy += t3f_get_font_line_height(resources->font[PP2_FONT_HUD]);
			if(gp->option[PP2_OPTION_LIFE] > 1)
			{
//				t3f_draw_textf(resources->font[PP2_FONT_HUD], al_map_rgba_f(0.0, 0.0, 0.0, 0.5), gp->player[i].view->left + ox + sx, gp->player[i].view->top + oy + sy, 0, 0, "Life: %02d", gp->player[i].life);
//				t3f_draw_textf(resources->font[PP2_FONT_HUD], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), gp->player[i].view->left + ox, gp->player[i].view->top + oy, 0, 0, "Life: %02d", gp->player[i].life);
//				oy += t3f_get_font_line_height(resources->font[PP2_FONT_HUD]);
			}
			break;
		}
		case PP2_GAME_MODE_DEATH_MATCH:
		{
			t3f_draw_textf(resources->font[PP2_FONT_HUD], al_map_rgba_f(0.0, 0.0, 0.0, 0.5), gp->player[i].view->left + ox + sx, gp->player[i].view->top + oy + sy, 0, 0, "Ammo: %02d", gp->player[i].ammo[gp->player[i].weapon]);
			t3f_draw_textf(resources->font[PP2_FONT_HUD], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), gp->player[i].view->left + ox, gp->player[i].view->top + oy, 0, 0, "Ammo: %02d", gp->player[i].ammo[gp->player[i].weapon]);
			oy += t3f_get_font_line_height(resources->font[PP2_FONT_HUD]);
			if(gp->option[PP2_OPTION_LIFE] > 1)
			{
				t3f_draw_textf(resources->font[PP2_FONT_HUD], al_map_rgba_f(0.0, 0.0, 0.0, 0.5), gp->player[i].view->left + ox + sx, gp->player[i].view->top + oy + sy, 0, 0, "Life: %02d", gp->player[i].life);
				t3f_draw_textf(resources->font[PP2_FONT_HUD], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), gp->player[i].view->left + ox, gp->player[i].view->top + oy, 0, 0, "Life: %02d", gp->player[i].life);
				oy += t3f_get_font_line_height(resources->font[PP2_FONT_HUD]);
			}
			t3f_draw_textf(resources->font[PP2_FONT_HUD], al_map_rgba_f(0.0, 0.0, 0.0, 0.5), gp->player[i].view->left + ox + sx, gp->player[i].view->top + oy + sy, 0, 0, "Frags: %02d", gp->player[i].frags);
			t3f_draw_textf(resources->font[PP2_FONT_HUD], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), gp->player[i].view->left + ox, gp->player[i].view->top + oy, 0, 0, "Frags: %02d", gp->player[i].frags);
			oy += t3f_get_font_line_height(resources->font[PP2_FONT_HUD]);
			if(gp->option[PP2_OPTION_TIME_LIMIT] > 0)
			{
				t3f_draw_textf(resources->font[PP2_FONT_HUD], al_map_rgba_f(0.0, 0.0, 0.0, 0.5), gp->player[i].view->virtual_width / 2 + sx, gp->player[i].view->top + sy, 0, ALLEGRO_ALIGN_CENTRE, "%02d:%02d", (gp->time_left + 59) / 3600, ((gp->time_left + 59) / 60) % 60);
				t3f_draw_textf(resources->font[PP2_FONT_HUD], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), gp->player[i].view->virtual_width / 2, gp->player[i].view->top, 0, ALLEGRO_ALIGN_CENTRE, "%02d:%02d", (gp->time_left + 59) / 3600, ((gp->time_left + 59) / 60) % 60);
			}
			break;
		}
		case PP2_GAME_MODE_COIN_RUSH:
		{
			t3f_draw_textf(resources->font[PP2_FONT_HUD], al_map_rgba_f(0.0, 0.0, 0.0, 0.5), gp->player[i].view->left + ox + sx, gp->player[i].view->top + oy + sy, 0, 0, "Ammo: %02d", gp->player[i].ammo[gp->player[i].weapon]);
			t3f_draw_textf(resources->font[PP2_FONT_HUD], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), gp->player[i].view->left + ox, gp->player[i].view->top + oy, 0, 0, "Ammo: %02d", gp->player[i].ammo[gp->player[i].weapon]);
			oy += t3f_get_font_line_height(resources->font[PP2_FONT_HUD]);
			if(gp->option[PP2_OPTION_LIFE] > 1)
			{
				t3f_draw_textf(resources->font[PP2_FONT_HUD], al_map_rgba_f(0.0, 0.0, 0.0, 0.5), gp->player[i].view->left + ox + sx, gp->player[i].view->top + oy + sy, 0, 0, "Life: %02d", gp->player[i].life);
				t3f_draw_textf(resources->font[PP2_FONT_HUD], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), gp->player[i].view->left + ox, gp->player[i].view->top + oy, 0, 0, "Life: %02d", gp->player[i].life);
				oy += t3f_get_font_line_height(resources->font[PP2_FONT_HUD]);
			}
			t3f_draw_textf(resources->font[PP2_FONT_HUD], al_map_rgba_f(0.0, 0.0, 0.0, 0.5), gp->player[i].view->left + ox + sx, gp->player[i].view->top + oy + sy, 0, 0, "Coins: %d of %d", gp->player[i].coins, gp->coins_needed);
			t3f_draw_textf(resources->font[PP2_FONT_HUD], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), gp->player[i].view->left + ox, gp->player[i].view->top + oy, 0, 0, "Coins: %d of %d", gp->player[i].coins, gp->coins_needed);
			oy += t3f_get_font_line_height(resources->font[PP2_FONT_HUD]);
			if(gp->option[PP2_OPTION_TIME_LIMIT] > 0)
			{
				t3f_draw_textf(resources->font[PP2_FONT_HUD], al_map_rgba_f(0.0, 0.0, 0.0, 0.5), gp->player[i].view->virtual_width / 2 + sx, gp->player[i].view->top + sy, 0, ALLEGRO_ALIGN_CENTRE, "%02d:%02d", (gp->time_left + 59) / 3600, ((gp->time_left + 59) / 60) % 60);
				t3f_draw_textf(resources->font[PP2_FONT_HUD], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), gp->player[i].view->virtual_width / 2, gp->player[i].view->top, 0, ALLEGRO_ALIGN_CENTRE, "%02d:%02d", (gp->time_left + 59) / 3600, ((gp->time_left + 59) / 60) % 60);
			}
			break;
		}
	}

	/* draw radar */
	a = 0.5;
	al_draw_tinted_scaled_bitmap(gp->radar_bitmap[gp->player[i].layer], al_map_rgba_f(a, a, a, a), 0, 0, al_get_bitmap_width(gp->radar_bitmap[gp->player[i].layer]), al_get_bitmap_height(gp->radar_bitmap[gp->player[i].layer]), gp->player[i].view->right - 96 - 8 - 1, gp->player[i].view->top + 8, 96, 96, 0);
	s = (float)96.0 / (float)(al_get_bitmap_width(gp->radar_bitmap[gp->player[i].layer]));
	for(j = 0; j < gp->radar_objects; j++)
	{
		cx = gp->radar_object[j].x - (float)(gp->level->tileset->width * gp->level->room.x);
		cx /= (float)gp->level->tileset->width;
		cx += (float)gp->radar_offset_x;
		cx *= s;
		cy = gp->radar_object[j].y  - (float)(gp->level->tileset->height * gp->level->room.y);
		cy /= (float)gp->level->tileset->height;
		cy += (float)gp->radar_offset_y;
		cy *= s;
		if(gp->radar_object[j].player < 0)
		{
			al_draw_tinted_bitmap(resources->bitmap[PP2_BITMAP_RADAR_BLIP], al_map_rgba_f(0.0, 1.0, 1.0, 1.0), gp->player[i].view->right - 96 - 8 - 1 + cx - 1.0, gp->player[i].view->top + 8 + cy - 1.0, 0);
		}
		else
		{
			if(gp->radar_object[j].player != i)
			{
				if(gp->player[gp->radar_object[j].player].flags & PP2_PLAYER_FLAG_POWER_CLOAK)
				{
				}
				else
				{
					al_draw_tinted_bitmap(resources->bitmap[PP2_BITMAP_RADAR_BLIP], al_map_rgba_f(1.0, 0.0, 0.0, 1.0), gp->player[i].view->right - 96 - 8 - 1 + cx - 1.0, gp->player[i].view->top + 8 + cy - 1.0, 0);
				}
			}
			else
			{
				al_draw_tinted_bitmap(resources->bitmap[PP2_BITMAP_RADAR_BLIP], al_map_rgba_f(1.0, 1.0, 0.0, 1.0), gp->player[i].view->right - 96 - 8 - 1 + cx - 1.0, gp->player[i].view->top + 8 + cy - 1.0, 0);
			}
		}
	}
	al_hold_bitmap_drawing(false);
}

static void render_single_viewport_backdrop(int i, ALLEGRO_COLOR color, PP2_RESOURCES * resources)
{
	switch(i)
	{
		case 0:
		{
			t3f_draw_scaled_bitmap(resources->bitmap[PP2_BITMAP_EMPTY_PLAYER], color, t3f_default_view->left, t3f_default_view->top, 0.0, PP2_SCREEN_VISIBLE_WIDTH / 2, PP2_SCREEN_VISIBLE_HEIGHT / 2, 0);
			break;
		}
		case 1:
		{
			t3f_draw_scaled_bitmap(resources->bitmap[PP2_BITMAP_EMPTY_PLAYER], color, PP2_SCREEN_WIDTH / 2, PP2_SCREEN_HEIGHT / 2, 0.0, PP2_SCREEN_VISIBLE_WIDTH / 2, PP2_SCREEN_VISIBLE_HEIGHT / 2, 0);
			break;
		}
		case 2:
		{
			t3f_draw_scaled_bitmap(resources->bitmap[PP2_BITMAP_EMPTY_PLAYER], color, PP2_SCREEN_WIDTH / 2, t3f_default_view->top, 0.0, PP2_SCREEN_VISIBLE_WIDTH / 2, PP2_SCREEN_VISIBLE_HEIGHT / 2, 0);
			break;
		}
		case 3:
		{
			t3f_draw_scaled_bitmap(resources->bitmap[PP2_BITMAP_EMPTY_PLAYER], al_map_rgba_f(1.0, 1.0, 1.0, 1.0), t3f_default_view->left, PP2_SCREEN_HEIGHT / 2, 0.0, PP2_SCREEN_VISIBLE_WIDTH / 2, PP2_SCREEN_VISIBLE_HEIGHT / 2, 0);
			break;
		}
	}
}

static void render_viewport_backdrop(PP2_GAME * gp, PP2_RESOURCES * resources)
{
	float x, y, w, h;
	bool used[4] = {false};
	int i, c, p;

	al_clear_to_color(t3f_color_black);
	al_hold_bitmap_drawing(true);
	c = 0;
	p = -1;
	for(i = 0; i < PP2_MAX_PLAYERS; i++)
	{
		if(gp->player[i].flags & PP2_PLAYER_FLAG_ACTIVE)
		{
			if(gp->player[i].view_port >= 0)
			{
				used[gp->player[i].view_port] = true;
				c++;
			}
			p = i;
		}
	}
	if(!c && p >= 0)
	{
		x = gp->player[p].view->offset_x;
		y = gp->player[p].view->offset_y;
		w = gp->player[p].view->width;
		h = gp->player[p].view->height;
		t3f_draw_scaled_bitmap(resources->bitmap[PP2_BITMAP_EMPTY_PLAYER], al_map_rgba_f(0.0, 0.0, 0.0, 1.0), x, y, 0.0, w, h, 0);
	}
	else
	{
		for(i = 0; i < 4; i++)
		{
			if(used[i])
			{
				render_single_viewport_backdrop(i, t3f_color_black, resources);
			}
			else
			{
				render_single_viewport_backdrop(i, t3f_color_white, resources);
			}
		}
	}
	if(gp->winner >= 0)
	{
		x = gp->player[gp->winner].view->offset_x;
		y = gp->player[gp->winner].view->offset_y;
		w = gp->player[gp->winner].view->width;
		h = gp->player[gp->winner].view->height;
		t3f_draw_scaled_bitmap(resources->bitmap[PP2_BITMAP_EMPTY_PLAYER], al_map_rgba_f(0.0, 0.0, 0.0, 1.0), x, y, 0.0, w, h, 0);
	}
	al_hold_bitmap_drawing(false);
}

void pp2_game_render(PP2_GAME * gp, PP2_THEME * theme, PP2_RESOURCES * resources)
{
	int i;

	/* draw empty players */
	t3f_select_view(t3f_default_view);
	render_viewport_backdrop(gp, resources);

	/* render player cameras */
	for(i = 0; i < PP2_MAX_PLAYERS; i++)
	{
		if(gp->player[i].flags & PP2_PLAYER_FLAG_ACTIVE && ((gp->client_game->player[i]->local && gp->replay_player < 0) || (i == gp->replay_player) || (i == gp->winner)))
		{
			pp2_game_render_player_view(gp, i, theme, resources);
		}
	}

	if(t3f_key[ALLEGRO_KEY_TILDE] || t3f_key[104] || gp->show_scores)
	{
		pp2_game_render_scoreboard(gp, "Current Scores", resources);
	}
}

void pp2_game_over_logic(PP2_INSTANCE * instance, PP2_GAME * gp, PP2_INTERFACE * ip, PP2_RESOURCES * resources)
{
	bool scale_done = false;
	int i;

	pp2_game_logic(gp, ip, resources, instance->theme);
	if(gp->client_game->player[gp->winner]->local)
	{
		if(gp->player[gp->winner].view->offset_x > t3f_default_view->left)
		{
			gp->player[gp->winner].view->offset_x -= 8.0;
		}
		if(gp->player[gp->winner].view->offset_y > t3f_default_view->top)
		{
			gp->player[gp->winner].view->offset_y -= 4.5;
		}
		if(gp->player[gp->winner].view->width < PP2_SCREEN_VISIBLE_WIDTH)
		{
			gp->player[gp->winner].view->width += 8.0;
	//		gp->player[gp->winner].camera.z++;
		}
		else
		{
			scale_done = true;
		}
		if(gp->player[gp->winner].view->height < PP2_SCREEN_VISIBLE_HEIGHT)
		{
			gp->player[gp->winner].view->height += 4.5;
		}
		gp->player[gp->winner].view->need_update = true;
		t3f_select_view(gp->player[gp->winner].view);
		if(!pp2_camera_clamp_left(gp, gp->winner))
		{
			if(pp2_camera_clamp_right(gp, gp->winner))
			{
				pp2_camera_clamp_left(gp, gp->winner);
			}
		}
		if(!pp2_camera_clamp_top(gp, gp->winner))
		{
			if(pp2_camera_clamp_bottom(gp, gp->winner))
			{
				pp2_camera_clamp_top(gp, gp->winner);
			}
		}
	}
	if(scale_done)
	{
		/* only the master can bring up the menu */
		if(!instance->client || instance->client->master)
		{
			for(i = 0; i < PP2_MAX_PLAYERS; i++)
			{
				if(gp->client_game->player[i]->playing && gp->client_game->player[i]->local)
				{
					if(ip->controller[i]->state[PP2_CONTROLLER_FIRE].pressed)
					{
						ip->joystick_menu_activation = true;
						joynet_pause_game(gp->client_game);
						break;
					}
				}
			}
		}
	}
}

void pp2_game_over_render(PP2_GAME * gp, PP2_THEME * theme, PP2_RESOURCES * resources)
{
	t3f_select_view(t3f_default_view);
	render_viewport_backdrop(gp, resources);
	pp2_game_render_player_view(gp, gp->winner, theme, resources);
	t3f_select_view(t3f_default_view);
	al_hold_bitmap_drawing(true);
	pp2_game_render_scoreboard(gp, "Final Scores", resources);
	al_hold_bitmap_drawing(false);
}

void pp2_game_paused_logic(PP2_INSTANCE * instance)
{
	if(!instance->client || instance->client->master)
	{
		pp2_process_menu(instance->ui.menu[instance->ui.current_menu], instance);
//		t3f_process_gui(instance->ui.menu[pp2_current_menu]);
	}
}

void pp2_game_paused_render(PP2_INSTANCE * instance, PP2_INTERFACE * ip, PP2_RESOURCES * resources)
{
	ALLEGRO_STATE old_state;
	ALLEGRO_TRANSFORM identity;
//	pp2_game_render();
	if(!instance->client || instance->client->master)
	{
		t3f_select_view(t3f_default_view);
		if(resources->bitmap[PP2_BITMAP_SCREEN_COPY])
		{
			al_store_state(&old_state, ALLEGRO_STATE_TRANSFORM);
			al_identity_transform(&identity);
			al_use_transform(&identity);
			al_draw_bitmap(resources->bitmap[PP2_BITMAP_SCREEN_COPY], 0, 0, 0);
			al_restore_state(&old_state);
		}
		al_draw_filled_rectangle(0.0, 0.0, PP2_SCREEN_WIDTH, PP2_SCREEN_HEIGHT, al_map_rgba_f(0.0, 0.0, 0.0, 0.5));
		al_hold_bitmap_drawing(true);
		al_draw_tinted_bitmap(resources->bitmap[PP2_BITMAP_MENU_LOGO], al_map_rgba_f(0.0, 0.0, 0.0, 0.5), PP2_SCREEN_WIDTH / 2 - al_get_bitmap_width(resources->bitmap[PP2_BITMAP_MENU_LOGO]) / 2 + 2, 0.0 + 2, 0);
		al_draw_bitmap(resources->bitmap[PP2_BITMAP_MENU_LOGO], PP2_SCREEN_WIDTH / 2 - al_get_bitmap_width(resources->bitmap[PP2_BITMAP_MENU_LOGO]) / 2, 0.0, 0);
		t3f_render_gui(ip->menu[ip->current_menu]);
		al_hold_bitmap_drawing(false);
	}
	else
	{
		t3f_select_view(t3f_default_view);
		if(resources->bitmap[PP2_BITMAP_SCREEN_COPY])
		{
			al_store_state(&old_state, ALLEGRO_STATE_TRANSFORM);
			al_identity_transform(&identity);
			al_use_transform(&identity);
			al_draw_bitmap(resources->bitmap[PP2_BITMAP_SCREEN_COPY], 0, 0, 0);
			al_restore_state(&old_state);
		}
		al_draw_filled_rectangle(0.0, 0.0, PP2_SCREEN_WIDTH, PP2_SCREEN_HEIGHT, al_map_rgba_f(0.0, 0.0, 0.0, 0.5));
		al_hold_bitmap_drawing(true);
		al_draw_tinted_bitmap(resources->bitmap[PP2_BITMAP_MENU_LOGO], al_map_rgba_f(0.0, 0.0, 0.0, 0.5), PP2_SCREEN_WIDTH / 2 - al_get_bitmap_width(resources->bitmap[PP2_BITMAP_MENU_LOGO]) / 2 + 2, 0.0 + 2, 0);
		al_draw_bitmap(resources->bitmap[PP2_BITMAP_MENU_LOGO], PP2_SCREEN_WIDTH / 2 - al_get_bitmap_width(resources->bitmap[PP2_BITMAP_MENU_LOGO]) / 2, 0.0, 0);
		al_hold_bitmap_drawing(false);
	}
}
