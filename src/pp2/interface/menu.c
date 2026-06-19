#include "t3f/t3f.h"
#include "title.h"
#include "text_entry.h"
#include "file/music.h"
#include "menu.h"
#include "menu_proc.h"
#include "pp2/resource.h"
#include "pp2/pp2.h"
#include "interface.h"

static bool element_is_flush_left(T3F_GUI_ELEMENT * ep)
{
	char * text = (char *)ep->data;

	if(strlen(text) > 1 && text[0] == '<')
	{
		return true;
	}
	return false;
}

static bool element_is_flush_right(T3F_GUI_ELEMENT * ep)
{
	char * text = (char *)ep->data;

	if(strlen(text) > 1 && text[strlen(text) - 1] == '>')
	{
		return true;
	}
	return false;
}

static void pp2_adjust_menu(T3F_GUI * gp)
{
	int i;

	for(i = 0; i < gp->elements; i++)
	{
		if(element_is_flush_left(&gp->element[i]))
		{
			gp->element[i].ox = t3f_default_view->left;
		}
		else if(element_is_flush_right(&gp->element[i]))
		{
			gp->element[i].ox = t3f_default_view->right - t3f_get_text_width((T3F_FONT *)gp->element[i].resource, (char *)gp->element[i].data);
		}
	}
}

void pp2_adjust_menus(PP2_INTERFACE * ip)
{
	int i;

	for(i = 0; i < PP2_MAX_MENUS; i++)
	{
		if(ip->menu[i])
		{
			pp2_adjust_menu(ip->menu[i]);
		}
	}
}

void pp2_menu_initialize(PP2_INSTANCE * instance)
{
	PP2_INTERFACE * ip = &instance->ui;
	PP2_RESOURCES * resources = &instance->resources;
	float cx0, cx1, cx2;
	int pos;

	/* calculate column positions (0 = center, 1 = left, 2 = right) */
	cx0 = PP2_SCREEN_WIDTH / 2;
	cx1 = PP2_SCREEN_WIDTH / 4;
	cx2 = PP2_SCREEN_WIDTH - PP2_SCREEN_WIDTH / 4;

	t3f_set_gui_driver(NULL);

	pos = 0;
	ip->menu[PP2_MENU_MAIN] = t3f_create_gui(0, 0, instance);
	t3f_add_gui_text_element(ip->menu[PP2_MENU_MAIN], pp2_menu_proc_main_play, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "Play", cx0, 240 + 24 * pos, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
	pos++;
	t3f_add_gui_text_element(ip->menu[PP2_MENU_MAIN], pp2_menu_proc_main_play_network, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "Netplay", cx0, 240 + 24 * pos, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
	pos++;
	t3f_add_gui_text_element(ip->menu[PP2_MENU_MAIN], pp2_menu_proc_main_profiles, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "Player Stats", cx0, 240 + 24 * pos, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
	pos++;
	t3f_add_gui_text_element(ip->menu[PP2_MENU_MAIN], pp2_menu_proc_main_view_replay, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "View Replay", cx0, 240 + 24 * pos, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
	pos++;
	if(ip->use_ffmpeg)
	{
		t3f_add_gui_text_element(ip->menu[PP2_MENU_MAIN], pp2_menu_proc_main_capture_replay, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "Capture Replay", cx0, 240 + 24 * pos, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
		pos++;
	}
	t3f_add_gui_text_element(ip->menu[PP2_MENU_MAIN], pp2_menu_proc_main_options, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "Options", cx0, 240 + 24 * pos, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
	pos++;
	t3f_add_gui_text_element(ip->menu[PP2_MENU_MAIN], pp2_menu_proc_main_quit, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "Quit", cx0, 240 + 24 * pos, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
	t3f_center_gui(ip->menu[PP2_MENU_MAIN], 200.0, PP2_SCREEN_HEIGHT);

		ip->menu[PP2_MENU_PLAY] = t3f_create_gui(0, 0, instance);
		t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY], pp2_menu_proc_play_quick_play, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "Quick Play", cx0, 240, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
		t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY], pp2_menu_proc_play_custom, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "Custom", cx0, 240 + 24, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
		t3f_center_gui(ip->menu[PP2_MENU_PLAY], 200.0, 456.0);
		t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY], pp2_menu_proc_overlay_back, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "< Back", 0, PP2_SCREEN_HEIGHT - t3f_get_font_line_height(resources->font[PP2_FONT_COMIC_16]) - ip->menu[PP2_MENU_PLAY]->oy, T3F_GUI_ELEMENT_SHADOW);

			ip->menu[PP2_MENU_PLAY_QUICK_PLAY] = t3f_create_gui(0, 0, instance);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_QUICK_PLAY], pp2_menu_proc_play_1_hit, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "1-Hit Elimination", cx0, 240, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_QUICK_PLAY], pp2_menu_proc_play_21_stomp, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "21 Stomp", cx0, 240 + 24, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_QUICK_PLAY], pp2_menu_proc_play_death_match, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "2 Minute Death Match", cx0, 240 + 24 * 2, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_QUICK_PLAY], pp2_menu_proc_play_coin_rush, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "Coin Rush", cx0, 240 + 24 * 3, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_QUICK_PLAY], pp2_menu_proc_play_battle_royale, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "Battle Royale", cx0, 240 + 24 * 4, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
			t3f_center_gui(ip->menu[PP2_MENU_PLAY_QUICK_PLAY], 200.0, 456.0);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_QUICK_PLAY], pp2_menu_proc_overlay_back, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "< Back", 0, PP2_SCREEN_HEIGHT - t3f_get_font_line_height(resources->font[PP2_FONT_COMIC_16]) - ip->menu[PP2_MENU_PLAY_QUICK_PLAY]->oy, T3F_GUI_ELEMENT_SHADOW);

			ip->menu[PP2_MENU_PLAY_SINGLE] = t3f_create_gui(0, 0, instance);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_SINGLE], pp2_menu_proc_play_explore, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "Explore", cx0, 240, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
			t3f_center_gui(ip->menu[PP2_MENU_PLAY_SINGLE], 200.0, 456.0);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_SINGLE], pp2_menu_proc_overlay_back, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "< Back", 0, PP2_SCREEN_HEIGHT - t3f_get_font_line_height(resources->font[PP2_FONT_COMIC_16]) - ip->menu[PP2_MENU_PLAY_SINGLE]->oy, T3F_GUI_ELEMENT_SHADOW);

			ip->menu[PP2_MENU_PLAY_STOCK] = t3f_create_gui(0, 0, instance);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_STOCK], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_HEADER_COLOR, "Standard", cx1, 240, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_STOCK], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ip->menu_text[8], cx1, 240 + 24, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_STOCK], pp2_menu_proc_normal_stock_left, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "<", cx1 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "100") / 2 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "<"), 240 + 24, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_STOCK], pp2_menu_proc_normal_stock_right, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ">", cx1 + t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "100") / 2, 240 + 24, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_STOCK], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_HEADER_COLOR, "Splitter", cx1, 240 + 24 * 2, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_STOCK], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ip->menu_text[9], cx1, 240 + 24 * 3, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_STOCK], pp2_menu_proc_splitter_stock_left, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "<", cx1 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "100") / 2 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "<"), 240 + 24 * 3, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_STOCK], pp2_menu_proc_splitter_stock_right, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ">", cx1 + t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "100") / 2, 240 + 24 * 3, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_STOCK], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_HEADER_COLOR, "Mine", cx1, 240 + 24 * 4, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_STOCK], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ip->menu_text[10], cx1, 240 + 24 * 5, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_STOCK], pp2_menu_proc_mine_stock_left, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "<", cx1 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "100") / 2 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "<"), 240 + 24 * 5, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_STOCK], pp2_menu_proc_mine_stock_right, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ">", cx1 + t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "100") / 2, 240 + 24 * 5, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_STOCK], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_HEADER_COLOR, "Bouncer", cx1, 240 + 24 * 6, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_STOCK], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ip->menu_text[11], cx1, 240 + 24 * 7, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_STOCK], pp2_menu_proc_bouncer_stock_left, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "<", cx1 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "100") / 2 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "<"), 240 + 24 * 7, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_STOCK], pp2_menu_proc_bouncer_stock_right, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ">", cx1 + t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "100") / 2, 240 + 24 * 7, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_STOCK], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_HEADER_COLOR, "Seeker", cx2, 240, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_STOCK], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ip->menu_text[12], cx2, 240 + 24, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_STOCK], pp2_menu_proc_seeker_stock_left, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "<", cx2 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "100") / 2 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "<"), 240 + 24, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_STOCK], pp2_menu_proc_seeker_stock_right, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ">", cx2 + t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "100") / 2, 240 + 24, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_STOCK], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_HEADER_COLOR, "Reflector", cx2, 240 + 24 * 2, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_STOCK], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ip->menu_text[13], cx2, 240 + 24 * 3, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_STOCK], pp2_menu_proc_reflector_stock_left, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "<", cx2 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "100") / 2 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "<"), 240 + 24 * 3, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_STOCK], pp2_menu_proc_reflector_stock_right, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ">", cx2 + t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "100") / 2, 240 + 24 * 3, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_STOCK], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_HEADER_COLOR, "Proximity Mine", cx2, 240 + 24 * 4, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_STOCK], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ip->menu_text[14], cx2, 240 + 24 * 5, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_STOCK], pp2_menu_proc_pmine_stock_left, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "<", cx2 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "100") / 2 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "<"), 240 + 24 * 5, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_STOCK], pp2_menu_proc_pmine_stock_right, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ">", cx2 + t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "100") / 2, 240 + 24 * 5, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_STOCK], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_HEADER_COLOR, "Ghost", cx2, 240 + 24 * 6, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_STOCK], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ip->menu_text[15], cx2, 240 + 24 * 7, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_STOCK], pp2_menu_proc_ghost_stock_left, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "<", cx2 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "100") / 2 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "<"), 240 + 24 * 7, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_STOCK], pp2_menu_proc_ghost_stock_right, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ">", cx2 + t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "100") / 2, 240 + 24 * 7, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_center_gui(ip->menu[PP2_MENU_PLAY_STOCK], 200.0, 456.0);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_STOCK], pp2_menu_proc_overlay_back, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "< Back", 0, PP2_SCREEN_HEIGHT - t3f_get_font_line_height(resources->font[PP2_FONT_COMIC_16]) - ip->menu[PP2_MENU_PLAY_STOCK]->oy, T3F_GUI_ELEMENT_SHADOW);

			ip->menu[PP2_MENU_PLAY_AMMO] = t3f_create_gui(0, 0, instance);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_AMMO], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_HEADER_COLOR, "Standard", cx1, 240, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_AMMO], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ip->menu_text[8], cx1, 240 + 24, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_AMMO], pp2_menu_proc_normal_ammo_left, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "<", cx1 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "Off ") / 2 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "<"), 240 + 24, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_AMMO], pp2_menu_proc_normal_ammo_right, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ">", cx1 + t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "Off ") / 2, 240 + 24, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_AMMO], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_HEADER_COLOR, "Splitter", cx1, 240 + 24 * 2, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_AMMO], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ip->menu_text[9], cx1, 240 + 24 * 3, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_AMMO], pp2_menu_proc_splitter_ammo_left, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "<", cx1 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "Off ") / 2 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "<"), 240 + 24 * 3, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_AMMO], pp2_menu_proc_splitter_ammo_right, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ">", cx1 + t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "Off ") / 2, 240 + 24 * 3, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_AMMO], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_HEADER_COLOR, "Mine", cx1, 240 + 24 * 4, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_AMMO], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ip->menu_text[10], cx1, 240 + 24 * 5, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_AMMO], pp2_menu_proc_mine_ammo_left, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "<", cx1 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "Off ") / 2 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "<"), 240 + 24 * 5, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_AMMO], pp2_menu_proc_mine_ammo_right, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ">", cx1 + t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "Off ") / 2, 240 + 24 * 5, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_AMMO], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_HEADER_COLOR, "Bouncer", cx1, 240 + 24 * 6, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_AMMO], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ip->menu_text[11], cx1, 240 + 24 * 7, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_AMMO], pp2_menu_proc_bouncer_ammo_left, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "<", cx1 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "Off ") / 2 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "<"), 240 + 24 * 7, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_AMMO], pp2_menu_proc_bouncer_ammo_right, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ">", cx1 + t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "Off ") / 2, 240 + 24 * 7, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_AMMO], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_HEADER_COLOR, "Seeker", cx2, 240, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_AMMO], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ip->menu_text[12], cx2, 240 + 24, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_AMMO], pp2_menu_proc_seeker_ammo_left, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "<", cx2 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "Off ") / 2 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "<"), 240 + 24, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_AMMO], pp2_menu_proc_seeker_ammo_right, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ">", cx2 + t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "Off ") / 2, 240 + 24, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_AMMO], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_HEADER_COLOR, "Reflector", cx2, 240 + 24 * 2, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_AMMO], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ip->menu_text[13], cx2, 240 + 24 * 3, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_AMMO], pp2_menu_proc_reflector_ammo_left, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "<", cx2 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "Off ") / 2 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "<"), 240 + 24 * 3, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_AMMO], pp2_menu_proc_reflector_ammo_right, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ">", cx2 + t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "Off ") / 2, 240 + 24 * 3, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_AMMO], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_HEADER_COLOR, "Proximity Mine", cx2, 240 + 24 * 4, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_AMMO], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ip->menu_text[14], cx2, 240 + 24 * 5, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_AMMO], pp2_menu_proc_pmine_ammo_left, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "<", cx2 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "Off ") / 2 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "<"), 240 + 24 * 5, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_AMMO], pp2_menu_proc_pmine_ammo_right, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ">", cx2 + t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "Off ") / 2, 240 + 24 * 5, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_AMMO], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_HEADER_COLOR, "Ghost", cx2, 240 + 24 * 6, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_AMMO], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ip->menu_text[15], cx2, 240 + 24 * 7, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_AMMO], pp2_menu_proc_ghost_ammo_left, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "<", cx2 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "Off ") / 2 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "<"), 240 + 24 * 7, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_AMMO], pp2_menu_proc_ghost_ammo_right, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ">", cx2 + t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "Off ") / 2, 240 + 24 * 7, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_center_gui(ip->menu[PP2_MENU_PLAY_AMMO], 200.0, 456.0);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_AMMO], pp2_menu_proc_overlay_back, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "< Back", 0, PP2_SCREEN_HEIGHT - t3f_get_font_line_height(resources->font[PP2_FONT_COMIC_16]) - ip->menu[PP2_MENU_PLAY_AMMO]->oy, T3F_GUI_ELEMENT_SHADOW);

			ip->menu[PP2_MENU_PLAY_POWERUPS] = t3f_create_gui(0, 0, instance);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_POWERUPS], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_HEADER_COLOR, "Cloak", cx1, 240, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_POWERUPS], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ip->menu_text[8], cx1, 240 + 24, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_POWERUPS], pp2_menu_proc_cloak_power_left, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "<", cx1 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "Off ") / 2 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "<"), 240 + 24, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_POWERUPS], pp2_menu_proc_cloak_power_right, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ">", cx1 + t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "Off ") / 2, 240 + 24, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_POWERUPS], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_HEADER_COLOR, "Jump", cx1, 240 + 24 * 2, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_POWERUPS], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ip->menu_text[9], cx1, 240 + 24 * 3, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_POWERUPS], pp2_menu_proc_jump_power_left, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "<", cx1 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "Off ") / 2 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "<"), 240 + 24 * 3, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_POWERUPS], pp2_menu_proc_jump_power_right, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ">", cx1 + t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "Off ") / 2, 240 + 24 * 3, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_POWERUPS], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_HEADER_COLOR, "Run", cx1, 240 + 24 * 4, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_POWERUPS], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ip->menu_text[10], cx1, 240 + 24 * 5, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_POWERUPS], pp2_menu_proc_run_power_left, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "<", cx1 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "Off ") / 2 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "<"), 240 + 24 * 5, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_POWERUPS], pp2_menu_proc_run_power_right, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ">", cx1 + t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "Off ") / 2, 240 + 24 * 5, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_POWERUPS], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_HEADER_COLOR, "Deflect", cx2, 240, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_POWERUPS], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ip->menu_text[11], cx2, 240 + 24, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_POWERUPS], pp2_menu_proc_deflect_power_left, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "<", cx2 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "Off ") / 2 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "<"), 240 + 24, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_POWERUPS], pp2_menu_proc_deflect_power_right, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ">", cx2 + t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "Off ") / 2, 240 + 24, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_POWERUPS], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_HEADER_COLOR, "Fly", cx2, 240 + 24 * 2, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_POWERUPS], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ip->menu_text[12], cx2, 240 + 24 * 3, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_POWERUPS], pp2_menu_proc_fly_power_left, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "<", cx2 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "Off ") / 2 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "<"), 240 + 24 * 3, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_POWERUPS], pp2_menu_proc_fly_power_right, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ">", cx2 + t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "Off ") / 2, 240 + 24 * 3, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_POWERUPS], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_HEADER_COLOR, "Turbo", cx2, 240 + 24 * 4, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_POWERUPS], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ip->menu_text[13], cx2, 240 + 24 * 5, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_POWERUPS], pp2_menu_proc_turbo_power_left, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "<", cx2 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "Off ") / 2 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "<"), 240 + 24 * 5, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_POWERUPS], pp2_menu_proc_turbo_power_right, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ">", cx2 + t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "Off ") / 2, 240 + 24 * 5, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_center_gui(ip->menu[PP2_MENU_PLAY_POWERUPS], 200.0, 456.0);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_POWERUPS], pp2_menu_proc_overlay_back, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "< Back", 0, PP2_SCREEN_HEIGHT - t3f_get_font_line_height(resources->font[PP2_FONT_COMIC_16]) - ip->menu[PP2_MENU_PLAY_POWERUPS]->oy, T3F_GUI_ELEMENT_SHADOW);

		ip->menu[PP2_MENU_PLAY_NETWORK] = t3f_create_gui(0, 0, instance);
		t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_NETWORK], pp2_menu_proc_main_play_online, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "Play Online", cx0, 240 + 24, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
		t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_NETWORK], pp2_menu_proc_main_play_lan, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "LAN Play", cx0, 240 + 24 * 2, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
		t3f_center_gui(ip->menu[PP2_MENU_PLAY_NETWORK], 200.0, 456.0);
		t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_NETWORK], pp2_menu_proc_overlay_back, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "< Back", 0, PP2_SCREEN_HEIGHT - t3f_get_font_line_height(resources->font[PP2_FONT_COMIC_16]) - ip->menu[PP2_MENU_PLAY_NETWORK]->oy, T3F_GUI_ELEMENT_SHADOW);

			ip->menu[PP2_MENU_PLAY_ONLINE] = t3f_create_gui(0, 0, instance);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_ONLINE], pp2_menu_proc_play_online_host, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "Host Game", cx0, 240, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_ONLINE], pp2_menu_proc_play_online_join, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "Join Game", cx0, 240 + 24, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
			t3f_center_gui(ip->menu[PP2_MENU_PLAY_ONLINE], 200.0, 456.0);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_ONLINE], pp2_menu_proc_overlay_back, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "< Back", 0, PP2_SCREEN_HEIGHT - t3f_get_font_line_height(resources->font[PP2_FONT_COMIC_16]) - ip->menu[PP2_MENU_PLAY_ONLINE]->oy, T3F_GUI_ELEMENT_SHADOW);

				ip->menu[PP2_MENU_NETWORK_ID] = t3f_create_gui(0, 0, instance);
				t3f_add_gui_text_element(ip->menu[PP2_MENU_NETWORK_ID], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_HEADER_COLOR, "Enter Network ID", cx0, 240, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
				t3f_add_gui_text_element(ip->menu[PP2_MENU_NETWORK_ID], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, pp2_get_entered_text(), cx0, 240 + 24, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
				t3f_add_gui_text_element(ip->menu[PP2_MENU_NETWORK_ID], pp2_menu_proc_network_id_ok, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "OK", cx0, 240 + 24 * 2, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
				t3f_center_gui(ip->menu[PP2_MENU_NETWORK_ID], 200.0, 456.0);
				t3f_add_gui_text_element(ip->menu[PP2_MENU_NETWORK_ID], pp2_menu_proc_overlay_back, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "< Back", 0, PP2_SCREEN_HEIGHT - t3f_get_font_line_height(resources->font[PP2_FONT_COMIC_16]) - ip->menu[PP2_MENU_NETWORK_ID]->oy, T3F_GUI_ELEMENT_SHADOW);

				ip->menu[PP2_MENU_HOST_NAME] = t3f_create_gui(0, 0, instance);
				t3f_add_gui_text_element(ip->menu[PP2_MENU_HOST_NAME], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_HEADER_COLOR, "Enter Server Name", cx0, 240, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
				t3f_add_gui_text_element(ip->menu[PP2_MENU_HOST_NAME], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, pp2_get_entered_text(), cx0, 240 + 24, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
				t3f_add_gui_text_element(ip->menu[PP2_MENU_HOST_NAME], pp2_menu_proc_host_name_ok, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "Begin Hosting", cx0, 240 + 24 * 2, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
				t3f_center_gui(ip->menu[PP2_MENU_HOST_NAME], 200.0, 456.0);
				t3f_add_gui_text_element(ip->menu[PP2_MENU_HOST_NAME], pp2_menu_proc_overlay_back, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "< Back", 0, PP2_SCREEN_HEIGHT - t3f_get_font_line_height(resources->font[PP2_FONT_COMIC_16]) - ip->menu[PP2_MENU_HOST_NAME]->oy, T3F_GUI_ELEMENT_SHADOW);

			ip->menu[PP2_MENU_PLAY_LAN] = t3f_create_gui(0, 0, instance);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_LAN], pp2_menu_proc_play_lan_host, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "Host Game", cx0, 240, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_LAN], pp2_menu_proc_play_lan_join, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "Join Game", cx0, 240 + 24, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
			t3f_center_gui(ip->menu[PP2_MENU_PLAY_LAN], 200.0, 456.0);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAY_LAN], pp2_menu_proc_overlay_back, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "< Back", 0, PP2_SCREEN_HEIGHT - t3f_get_font_line_height(resources->font[PP2_FONT_COMIC_16]) - ip->menu[PP2_MENU_PLAY_ONLINE]->oy, T3F_GUI_ELEMENT_SHADOW);

				ip->menu[PP2_MENU_HOST_IP] = t3f_create_gui(0, 0, instance);
				t3f_add_gui_text_element(ip->menu[PP2_MENU_HOST_IP], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_HEADER_COLOR, "Enter IP Address", cx0, 240, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
				t3f_add_gui_text_element(ip->menu[PP2_MENU_HOST_IP], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, pp2_get_entered_text(), cx0, 240 + 24, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
				t3f_add_gui_text_element(ip->menu[PP2_MENU_HOST_IP], pp2_menu_proc_host_ip_ok, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "Connect to Server", cx0, 240 + 24 * 2, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
				t3f_center_gui(ip->menu[PP2_MENU_HOST_IP], 200.0, 456.0);
				t3f_add_gui_text_element(ip->menu[PP2_MENU_HOST_IP], pp2_menu_proc_overlay_back, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "< Back", 0, PP2_SCREEN_HEIGHT - t3f_get_font_line_height(resources->font[PP2_FONT_COMIC_16]) - ip->menu[PP2_MENU_HOST_NAME]->oy, T3F_GUI_ELEMENT_SHADOW);

		ip->menu[PP2_MENU_OPTIONS] = t3f_create_gui(0, 0, instance);
		t3f_add_gui_text_element(ip->menu[PP2_MENU_OPTIONS], pp2_menu_proc_options_controllers, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "Controllers", cx0, 240, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
		t3f_add_gui_text_element(ip->menu[PP2_MENU_OPTIONS], pp2_menu_proc_options_audio, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "Audio", cx0, 240 + 24, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
		t3f_add_gui_text_element(ip->menu[PP2_MENU_OPTIONS], pp2_menu_proc_options_video, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "Video", cx0, 240 + 24 * 2, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
		t3f_add_gui_text_element(ip->menu[PP2_MENU_OPTIONS], pp2_menu_proc_options_network, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "Network", cx0, 240 + 24 * 3, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
		t3f_center_gui(ip->menu[PP2_MENU_OPTIONS], 200.0, 456.0);
		t3f_add_gui_text_element(ip->menu[PP2_MENU_OPTIONS], pp2_menu_proc_overlay_back, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "< Back", 0, PP2_SCREEN_HEIGHT - t3f_get_font_line_height(resources->font[PP2_FONT_COMIC_16]) - ip->menu[PP2_MENU_OPTIONS]->oy, T3F_GUI_ELEMENT_SHADOW);

			ip->menu[PP2_MENU_CONTROLLERS] = t3f_create_gui(0, 0, instance);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_CONTROLLERS], pp2_menu_proc_options_controller_1, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "Controller 1", cx0, 240, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_CONTROLLERS], pp2_menu_proc_options_controller_2, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "Controller 2", cx0, 240 + 24, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_CONTROLLERS], pp2_menu_proc_options_controller_3, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "Controller 3", cx0, 240 + 24 * 2, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_CONTROLLERS], pp2_menu_proc_options_controller_4, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "Controller 4", cx0, 240 + 24 * 3, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_CONTROLLERS], pp2_menu_proc_options_controller_reset, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "Autodetect", cx0, 240 + 24 * 4, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
			t3f_center_gui(ip->menu[PP2_MENU_CONTROLLERS], 200.0, 456.0);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_CONTROLLERS], pp2_menu_proc_overlay_back, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "< Back", 0, PP2_SCREEN_HEIGHT - t3f_get_font_line_height(resources->font[PP2_FONT_COMIC_16]) - ip->menu[PP2_MENU_CONTROLLERS]->oy, T3F_GUI_ELEMENT_SHADOW);

				ip->menu[PP2_MENU_CONTROLLER] = t3f_create_gui(0, 0, instance);
				t3f_add_gui_text_element(ip->menu[PP2_MENU_CONTROLLER], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ip->controller_binding_text[0], cx0, 0, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
				t3f_add_gui_text_element(ip->menu[PP2_MENU_CONTROLLER], NULL, resources->font[PP2_FONT_COMIC_10], PP2_MENU_OPTION_COLOR, ip->controller_binding_text[10], cx0, 0 + 24 * 1, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
				t3f_add_gui_text_element(ip->menu[PP2_MENU_CONTROLLER], NULL, resources->font[PP2_FONT_COMIC_12], PP2_MENU_HEADER_COLOR, "Up", cx1, 0 + 16 * 2, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
				t3f_add_gui_text_element(ip->menu[PP2_MENU_CONTROLLER], pp2_menu_proc_controller_up, resources->font[PP2_FONT_COMIC_12], PP2_MENU_OPTION_COLOR, ip->controller_binding_text[1], cx1, 0 + 16 * 3, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
				t3f_add_gui_text_element(ip->menu[PP2_MENU_CONTROLLER], NULL, resources->font[PP2_FONT_COMIC_12], PP2_MENU_HEADER_COLOR, "Down", cx1, 0 + 16 * 4, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
				t3f_add_gui_text_element(ip->menu[PP2_MENU_CONTROLLER], pp2_menu_proc_controller_down, resources->font[PP2_FONT_COMIC_12], PP2_MENU_OPTION_COLOR, ip->controller_binding_text[2], cx1, 0 + 16 * 5, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
				t3f_add_gui_text_element(ip->menu[PP2_MENU_CONTROLLER], NULL, resources->font[PP2_FONT_COMIC_12], PP2_MENU_HEADER_COLOR, "Left", cx1, 0 + 16 * 6, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
				t3f_add_gui_text_element(ip->menu[PP2_MENU_CONTROLLER], pp2_menu_proc_controller_left, resources->font[PP2_FONT_COMIC_12], PP2_MENU_OPTION_COLOR, ip->controller_binding_text[3], cx1, 0 + 16 * 7, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
				t3f_add_gui_text_element(ip->menu[PP2_MENU_CONTROLLER], NULL, resources->font[PP2_FONT_COMIC_12], PP2_MENU_HEADER_COLOR, "Right", cx1, 0 + 16 * 8, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
				t3f_add_gui_text_element(ip->menu[PP2_MENU_CONTROLLER], pp2_menu_proc_controller_right, resources->font[PP2_FONT_COMIC_12], PP2_MENU_OPTION_COLOR, ip->controller_binding_text[4], cx1, 0 + 16 * 9, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
				t3f_add_gui_text_element(ip->menu[PP2_MENU_CONTROLLER], NULL, resources->font[PP2_FONT_COMIC_12], PP2_MENU_HEADER_COLOR, "Jump", cx2, 0 + 16 * 2, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
				t3f_add_gui_text_element(ip->menu[PP2_MENU_CONTROLLER], pp2_menu_proc_controller_jump, resources->font[PP2_FONT_COMIC_12], PP2_MENU_OPTION_COLOR, ip->controller_binding_text[5], cx2, 0 + 16 * 3, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
				t3f_add_gui_text_element(ip->menu[PP2_MENU_CONTROLLER], NULL, resources->font[PP2_FONT_COMIC_12], PP2_MENU_HEADER_COLOR, "Fire", cx2, 0 + 16 * 4, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
				t3f_add_gui_text_element(ip->menu[PP2_MENU_CONTROLLER], pp2_menu_proc_controller_fire, resources->font[PP2_FONT_COMIC_12], PP2_MENU_OPTION_COLOR, ip->controller_binding_text[6], cx2, 0 + 16 * 5, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
				t3f_add_gui_text_element(ip->menu[PP2_MENU_CONTROLLER], NULL, resources->font[PP2_FONT_COMIC_12], PP2_MENU_HEADER_COLOR, "Select", cx2, 0 + 16 * 6, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
				t3f_add_gui_text_element(ip->menu[PP2_MENU_CONTROLLER], pp2_menu_proc_controller_select, resources->font[PP2_FONT_COMIC_12], PP2_MENU_OPTION_COLOR, ip->controller_binding_text[7], cx2, 0 + 16 * 7, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
				t3f_add_gui_text_element(ip->menu[PP2_MENU_CONTROLLER], NULL, resources->font[PP2_FONT_COMIC_12], PP2_MENU_HEADER_COLOR, "Strafe", cx2, 0 + 16 * 8, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
				t3f_add_gui_text_element(ip->menu[PP2_MENU_CONTROLLER], pp2_menu_proc_controller_strafe, resources->font[PP2_FONT_COMIC_12], PP2_MENU_OPTION_COLOR, ip->controller_binding_text[8], cx2, 0 + 16 * 9, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
				t3f_add_gui_text_element(ip->menu[PP2_MENU_CONTROLLER], NULL, resources->font[PP2_FONT_COMIC_12], PP2_MENU_HEADER_COLOR, "Show Scores", cx0, 0 + 16 * 10, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
				t3f_add_gui_text_element(ip->menu[PP2_MENU_CONTROLLER], pp2_menu_proc_controller_show_scores, resources->font[PP2_FONT_COMIC_12], PP2_MENU_OPTION_COLOR, ip->controller_binding_text[9], cx0, 0 + 16 * 11, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
				t3f_center_gui(ip->menu[PP2_MENU_CONTROLLER], 200.0, 456.0);
				t3f_add_gui_text_element(ip->menu[PP2_MENU_CONTROLLER], pp2_menu_proc_overlay_back, resources->font[PP2_FONT_COMIC_16],PP2_MENU_OPTION_COLOR, "< Back", 0, PP2_SCREEN_HEIGHT - t3f_get_font_line_height(resources->font[PP2_FONT_COMIC_16]) - ip->menu[PP2_MENU_CONTROLLER]->oy, T3F_GUI_ELEMENT_SHADOW);

			ip->menu[PP2_MENU_AUDIO] = t3f_create_gui(0, 0, instance);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_AUDIO], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_HEADER_COLOR, "Music Volume", cx0, 240, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_AUDIO], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ip->menu_text[0], cx0, 240 + 24, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_AUDIO], pp2_menu_proc_music_down, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "<", cx0 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "100%%") / 2 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "<"), 240 + 24, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_AUDIO], pp2_menu_proc_music_up, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ">", cx0 + t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "100%%") / 2, 240 + 24, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_AUDIO], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_HEADER_COLOR, "Sound Volume", cx0, 240 + 24 * 2, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_AUDIO], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ip->menu_text[1], cx0, 240 + 24 * 3, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_AUDIO], pp2_menu_proc_sound_down, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "<", cx0 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "100%%") / 2 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "<"), 240 + 24 * 3, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_AUDIO], pp2_menu_proc_sound_up, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ">", cx0 + t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "100%%") / 2, 240 + 24 * 3, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_center_gui(ip->menu[PP2_MENU_AUDIO], 200.0, 456.0);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_AUDIO], pp2_menu_proc_overlay_back, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "< Back", 0, PP2_SCREEN_HEIGHT - t3f_get_font_line_height(resources->font[PP2_FONT_COMIC_16]) - ip->menu[PP2_MENU_AUDIO]->oy, T3F_GUI_ELEMENT_SHADOW);

			ip->menu[PP2_MENU_VIDEO] = t3f_create_gui(0, 0, instance);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_VIDEO], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_HEADER_COLOR, "Display", cx0, 240, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_VIDEO], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ip->menu_text[0], cx0, 240 + 24, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_VIDEO], pp2_menu_proc_display_toggle, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "<", cx0 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], " Full Screen ") / 2 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "<"), 240 + 24, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_VIDEO], pp2_menu_proc_display_toggle, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ">", cx0 + t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], " Full Screen ") / 2, 240 + 24, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_VIDEO], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_HEADER_COLOR, "Resolution", cx0, 240 + 24 * 2, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_VIDEO], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ip->menu_text[1], cx0, 240 + 24 * 3, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_VIDEO], pp2_menu_proc_resolution_left, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "<", cx0 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], " 1024x768 ") / 2 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "<"), 240 + 24 * 3, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_VIDEO], pp2_menu_proc_resolution_right, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, ">", cx0 + t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], " 1024x768 ") / 2, 240 + 24 * 3, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_center_gui(ip->menu[PP2_MENU_VIDEO], 200.0, 456.0);
			t3f_add_gui_text_element(ip->menu[PP2_MENU_VIDEO], pp2_menu_proc_overlay_back, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "< Back", 0, PP2_SCREEN_HEIGHT - t3f_get_font_line_height(resources->font[PP2_FONT_COMIC_16]) - ip->menu[PP2_MENU_VIDEO]->oy, T3F_GUI_ELEMENT_SHADOW);

		ip->menu[PP2_MENU_NETWORK] = t3f_create_gui(0, 0, instance);
		t3f_add_gui_text_element(ip->menu[PP2_MENU_NETWORK], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_HEADER_COLOR, "Enter Network ID", cx0, 240, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
		t3f_add_gui_text_element(ip->menu[PP2_MENU_NETWORK], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, pp2_get_entered_text(), cx0, 240 + 24, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
		t3f_add_gui_text_element(ip->menu[PP2_MENU_NETWORK], pp2_menu_proc_network_ok, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "OK", cx0, 240 + 24 * 2, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
		t3f_center_gui(ip->menu[PP2_MENU_NETWORK], 200.0, 456.0);
		t3f_add_gui_text_element(ip->menu[PP2_MENU_NETWORK], pp2_menu_proc_overlay_back, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "< Back", 0, PP2_SCREEN_HEIGHT - t3f_get_font_line_height(resources->font[PP2_FONT_COMIC_16]) - ip->menu[PP2_MENU_NETWORK]->oy, T3F_GUI_ELEMENT_SHADOW);

	ip->menu[PP2_MENU_MAIN_HOST] = t3f_create_gui(0, 0, instance);
	t3f_add_gui_text_element(ip->menu[PP2_MENU_MAIN_HOST], pp2_menu_proc_main_play, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "Play", cx0, 240, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
	t3f_add_gui_text_element(ip->menu[PP2_MENU_MAIN_HOST], pp2_menu_proc_main_close_server, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "Close Server", cx0, 240 + 24, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
	t3f_center_gui(ip->menu[PP2_MENU_MAIN_HOST], 200.0, PP2_SCREEN_HEIGHT);

	ip->menu[PP2_MENU_MAIN_CLIENT] = t3f_create_gui(0, 0, instance);
	t3f_add_gui_text_element(ip->menu[PP2_MENU_MAIN_CLIENT], pp2_menu_proc_main_play, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "Play", cx0, 240, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
	t3f_add_gui_text_element(ip->menu[PP2_MENU_MAIN_CLIENT], pp2_menu_proc_main_disconnect, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "Disconnect", cx0, 240 + 24, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
	t3f_center_gui(ip->menu[PP2_MENU_MAIN_CLIENT], 200.0, PP2_SCREEN_HEIGHT);

	ip->menu[PP2_MENU_PAUSE] = t3f_create_gui(0, 0, instance);
	t3f_add_gui_text_element(ip->menu[PP2_MENU_PAUSE], pp2_menu_proc_game_resume, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "Resume Game", cx0, 240, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
	t3f_add_gui_text_element(ip->menu[PP2_MENU_PAUSE], pp2_menu_proc_game_end, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "End Game", cx0, 240 + 24, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
	t3f_center_gui(ip->menu[PP2_MENU_PAUSE], 200.0, PP2_SCREEN_HEIGHT);

	ip->menu[PP2_MENU_GAME_OVER] = t3f_create_gui(0, 0, instance);
	t3f_add_gui_text_element(ip->menu[PP2_MENU_GAME_OVER], pp2_menu_proc_game_rematch, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "Rematch", cx0, 240, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
	t3f_add_gui_text_element(ip->menu[PP2_MENU_GAME_OVER], pp2_menu_proc_game_play_again, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "Play Again", cx0, 240 + 24, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
	t3f_add_gui_text_element(ip->menu[PP2_MENU_GAME_OVER], pp2_menu_proc_game_new_game, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "New Game", cx0, 240 + 24 * 2, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
	t3f_add_gui_text_element(ip->menu[PP2_MENU_GAME_OVER], pp2_menu_proc_game_quit, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "Quit", cx0, 240 + 24 * 3, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
	t3f_center_gui(ip->menu[PP2_MENU_GAME_OVER], 200.0, PP2_SCREEN_HEIGHT);

	ip->menu[PP2_MENU_PAUSE_CLIENT] = t3f_create_gui(0, 0, instance);
	t3f_add_gui_text_element(ip->menu[PP2_MENU_PAUSE_CLIENT], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "Paused", cx0, 240, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
	t3f_center_gui(ip->menu[PP2_MENU_PAUSE_CLIENT], 200.0, PP2_SCREEN_HEIGHT);

	ip->menu[PP2_MENU_NEW_PROFILE] = t3f_create_gui(0, 0, instance);
	t3f_add_gui_text_element(ip->menu[PP2_MENU_NEW_PROFILE], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_HEADER_COLOR, "Enter Your Name", cx0, 240, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
	t3f_add_gui_text_element(ip->menu[PP2_MENU_NEW_PROFILE], NULL, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, pp2_get_entered_text(), cx0, 240 + 24, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
	t3f_add_gui_text_element(ip->menu[PP2_MENU_NEW_PROFILE], pp2_menu_proc_new_profile_ok, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "OK", cx0, 240 + 24 * 2, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
	t3f_center_gui(ip->menu[PP2_MENU_NEW_PROFILE], 200.0, 456.0);

	ip->menu[PP2_MENU_OVERLAY] = t3f_create_gui(0, 0, instance);
	t3f_add_gui_text_element(ip->menu[PP2_MENU_OVERLAY], pp2_menu_proc_overlay_back, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "< Back", 0, t3f_default_view->bottom - t3f_get_font_line_height(resources->font[PP2_FONT_COMIC_16]), T3F_GUI_ELEMENT_SHADOW);

	ip->menu[PP2_MENU_PLAYER_SETUP_OVERLAY] = t3f_create_gui(0, 0, instance);
	t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAYER_SETUP_OVERLAY], pp2_menu_proc_overlay_back, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "< Back", 0, PP2_SCREEN_HEIGHT - t3f_get_font_line_height(resources->font[PP2_FONT_COMIC_16]), T3F_GUI_ELEMENT_SHADOW);
	t3f_add_gui_text_element(ip->menu[PP2_MENU_PLAYER_SETUP_OVERLAY], pp2_menu_proc_overlay_next, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "Next >", PP2_SCREEN_WIDTH - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "Next >"), t3f_default_view->bottom - t3f_get_font_line_height(resources->font[PP2_FONT_COMIC_16]), T3F_GUI_ELEMENT_SHADOW);

	ip->menu[PP2_MENU_CLIENT_PLAYER_SETUP_OVERLAY] = t3f_create_gui(0, 0, instance);
	t3f_add_gui_text_element(ip->menu[PP2_MENU_CLIENT_PLAYER_SETUP_OVERLAY], pp2_menu_proc_overlay_back, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "< Back", 0, t3f_default_view->bottom - t3f_get_font_line_height(resources->font[PP2_FONT_COMIC_16]), T3F_GUI_ELEMENT_SHADOW);
//	t3f_add_gui_text_element(ip->menu[PP2_MENU_CLIENT_PLAYER_SETUP_OVERLAY], pp2_menu_proc_overlay_next, "Next >", (void **)&resources->font[PP2_FONT_COMIC_16], 640 - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "Next >"), cx2 - t3f_get_font_line_height(resources->font[PP2_FONT_COMIC_16]), PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW);

	ip->menu[PP2_MENU_LEVEL_SETUP_OVERLAY] = t3f_create_gui(0, 0, instance);
	t3f_add_gui_text_element(ip->menu[PP2_MENU_LEVEL_SETUP_OVERLAY], pp2_menu_proc_overlay_back, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "< Back", 0, PP2_SCREEN_HEIGHT - t3f_get_font_line_height(resources->font[PP2_FONT_COMIC_16]), T3F_GUI_ELEMENT_SHADOW);
	t3f_add_gui_text_element(ip->menu[PP2_MENU_LEVEL_SETUP_OVERLAY], pp2_menu_proc_overlay_next, resources->font[PP2_FONT_COMIC_16], PP2_MENU_OPTION_COLOR, "Start Game >", PP2_SCREEN_WIDTH - t3f_get_text_width(resources->font[PP2_FONT_COMIC_16], "Start Game >"), t3f_default_view->bottom - t3f_get_font_line_height(resources->font[PP2_FONT_COMIC_16]), T3F_GUI_ELEMENT_SHADOW);
	pp2_adjust_menus(ip);
}

void pp2_process_menu(T3F_GUI * menu, PP2_INSTANCE * instance)
{
	if(instance->ui.menu_input_source >= PP2_INPUT_BASE_MOUSE || instance->ui.menu_joystick_disabled)
	{
		t3f_select_hover_gui_element(menu, t3f_get_input_val(instance->ui.input_handler[PP2_INPUT_BASE_MOUSE], T3F_MOUSE_X), t3f_get_input_val(instance->ui.input_handler[PP2_INPUT_BASE_MOUSE], T3F_MOUSE_Y));
		if(t3f_input_pressed(instance->ui.input_handler[PP2_INPUT_BASE_MOUSE], T3F_MOUSE_BUTTON_1))
		{
			t3f_activate_selected_gui_element(menu);
		}
	}
	else if(!pp2_get_text_entry_state() && instance->ui.menu_joystick_skip == 0)
	{
		if(instance->ui.input_handler[instance->ui.menu_input_source]->element[PP2_CONTROLLER_UP].pressed || instance->ui.input_handler[instance->ui.menu_input_source]->element[PP2_CONTROLLER_LEFT].pressed)
		{
			t3f_select_previous_gui_element(instance->ui.menu[instance->ui.current_menu]);
		}
		if(instance->ui.input_handler[instance->ui.menu_input_source]->element[PP2_CONTROLLER_DOWN].pressed || instance->ui.input_handler[instance->ui.menu_input_source]->element[PP2_CONTROLLER_RIGHT].pressed)
		{
			t3f_select_next_gui_element(instance->ui.menu[instance->ui.current_menu]);
		}
		if(instance->ui.input_handler[instance->ui.menu_input_source]->element[PP2_CONTROLLER_FIRE].pressed)
		{
			instance->ui.joystick_menu_activation = true;
			t3f_activate_selected_gui_element(instance->ui.menu[instance->ui.current_menu]);
			instance->ui.joystick_menu_activation = false;
		}
		else if(instance->ui.input_handler[instance->ui.menu_input_source]->element[PP2_CONTROLLER_JUMP].pressed)
		{
			pp2_menu_proc_overlay_back(instance, 0, NULL);
		}
	}
	if(instance->ui.menu_joystick_skip > 0)
	{
		instance->ui.menu_joystick_skip--;
	}
}

void pp2_menu_logic(PP2_INSTANCE * instance)
{
	int i;

	instance->ui.menu_offset -= 0.25;
	if(instance->ui.menu_offset <= -64.0)
	{
		instance->ui.menu_offset = 0.0;
	}
	instance->ui.tick++;

	/* update all input devices */
	for(i = 0; i < PP2_MAX_PLAYERS; i++)
	{
		if(instance->ui.input_handler[i])
		{
			t3f_update_input_handler_state(instance->ui.input_handler[i]);
		}
	}

	/* determine which input device is being used to control the menus */
	if(instance->ui.menu_input_source >= PP2_INPUT_BASE_MOUSE)
	{
		for(i = 0; i < PP2_INPUT_BASE_MOUSE; i++)
		{
			if(instance->ui.input_handler[i])
			{
				if(t3f_input_pressed(instance->ui.input_handler[i], T3F_GAMEPAD_DPAD_UP) || t3f_input_pressed(instance->ui.input_handler[i], T3F_GAMEPAD_DPAD_DOWN) || t3f_input_pressed(instance->ui.input_handler[i], T3F_GAMEPAD_DPAD_LEFT) || t3f_input_pressed(instance->ui.input_handler[i], T3F_GAMEPAD_DPAD_RIGHT))
				{
					instance->ui.menu_input_source = i;
					break;
				}
			}
		}
	}
	else
	{
		if(t3f_get_input_diff(instance->ui.input_handler[PP2_INPUT_BASE_MOUSE], T3F_MOUSE_X) != 0.0 || t3f_get_input_diff(instance->ui.input_handler[PP2_INPUT_BASE_MOUSE], T3F_MOUSE_Y) != 0.0)
		{
			instance->ui.menu_input_source = PP2_INPUT_BASE_MOUSE;
		}
	}

	pp2_process_menu(instance->ui.menu[instance->ui.current_menu], instance);
	if(instance->ui.menu_stack_size > 0)
	{
		if(instance->ui.current_menu == PP2_MENU_PLAY_CUSTOM)
		{
			if(instance->client && !instance->client->master)
			{
				t3f_process_gui(instance->ui.menu[PP2_MENU_CLIENT_PLAYER_SETUP_OVERLAY], 0);
			}
/*			else
			{
				t3f_process_gui(ip->menu[PP2_MENU_PLAYER_SETUP_OVERLAY]);
			} */
		}
/*		else
		{
			t3f_process_gui(ip->menu[PP2_MENU_OVERLAY]);
		} */
	}
}

void pp2_menu_render(PP2_INSTANCE * instance, PP2_INTERFACE * ip, PP2_RESOURCES * resources)
{
	int tw = PP2_SCREEN_WIDTH / resources->bitmap[PP2_BITMAP_MENU_BG]->target_width + 1;
	int th = PP2_SCREEN_HEIGHT / resources->bitmap[PP2_BITMAP_MENU_BG]->target_height + 2;
	int i, j;
	float cx;

	al_hold_bitmap_drawing(true);
	for(i = 0; i < th; i++)
	{
		for(j = 0; j < tw; j++)
		{
			t3f_draw_bitmap(resources->bitmap[PP2_BITMAP_MENU_BG], instance->theme->menu_bg_color, (float)(j * resources->bitmap[PP2_BITMAP_MENU_BG]->target_width) + ip->menu_offset, (float)(i * resources->bitmap[PP2_BITMAP_MENU_BG]->target_height) + ip->menu_offset, 0, 0);
		}
	}
	cx = PP2_SCREEN_WIDTH / 2 - resources->bitmap[PP2_BITMAP_MENU_LOGO]->target_width / 2;
	t3f_draw_bitmap(resources->bitmap[PP2_BITMAP_MENU_LOGO], al_map_rgba_f(0.0, 0.0, 0.0, 0.5), cx + 2, 0.0 + 2, 0, 0);
	t3f_draw_bitmap(resources->bitmap[PP2_BITMAP_MENU_LOGO], t3f_color_white, cx, 0.0, 0, 0);
	t3f_render_gui(ip->menu[ip->current_menu], 0);
	if(ip->menu_stack_size > 0)
	{
		if(ip->current_menu == PP2_MENU_PLAY_CUSTOM)
		{
			if(instance->client && !instance->client->master)
			{
				t3f_render_gui(ip->menu[PP2_MENU_CLIENT_PLAYER_SETUP_OVERLAY], 0);
			}
		}
	}
	al_hold_bitmap_drawing(false);
}
