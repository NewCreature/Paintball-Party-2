#include "../t3f/t3f.h"
#include "../t3f/gui.h"
#include "title.h"
#include "../data.h"
#include "../text_entry.h"
#include "../tables.h"
#include "../file/music.h"
#include "menu.h"
#include "menu_proc.h"

void pp2_menu_initialize(void)
{
	t3f_set_gui_driver(NULL);
	pp2_menu[PP2_MENU_MAIN] = t3f_create_gui(0, 0);
	t3f_add_gui_text_element(pp2_menu[PP2_MENU_MAIN], pp2_menu_proc_main_play, "Play", pp2_font[PP2_FONT_COMIC_16], 320, 240, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
	t3f_add_gui_text_element(pp2_menu[PP2_MENU_MAIN], pp2_menu_proc_main_play_network, "Netplay", pp2_font[PP2_FONT_COMIC_16], 320, 240 + 24, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
	t3f_add_gui_text_element(pp2_menu[PP2_MENU_MAIN], pp2_menu_proc_main_profiles, "Player Stats", pp2_font[PP2_FONT_COMIC_16], 320, 240 + 24 * 2, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
	t3f_add_gui_text_element(pp2_menu[PP2_MENU_MAIN], pp2_menu_proc_main_view_replay, "View Replay", pp2_font[PP2_FONT_COMIC_16], 320, 240 + 24 * 3, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
	t3f_add_gui_text_element(pp2_menu[PP2_MENU_MAIN], pp2_menu_proc_main_options, "Options", pp2_font[PP2_FONT_COMIC_16], 320, 240 + 24 * 4, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
	t3f_add_gui_text_element(pp2_menu[PP2_MENU_MAIN], pp2_menu_proc_main_quit, "Quit", pp2_font[PP2_FONT_COMIC_16], 320, 240 + 24 * 5, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
	t3f_center_gui(pp2_menu[PP2_MENU_MAIN], 200.0, 480.0);
	
		pp2_menu[PP2_MENU_PLAY] = t3f_create_gui(0, 0);
		t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY], pp2_menu_proc_play_quick_play, "Quick Play", pp2_font[PP2_FONT_COMIC_16], 320, 240, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
		t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY], pp2_menu_proc_play_custom, "Custom", pp2_font[PP2_FONT_COMIC_16], 320, 240 + 24, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
		t3f_center_gui(pp2_menu[PP2_MENU_PLAY], 200.0, 456.0);
		t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY], pp2_menu_proc_overlay_back, "< Back", pp2_font[PP2_FONT_COMIC_16], 0, 480 - al_get_font_line_height(pp2_font[PP2_FONT_COMIC_16]) - pp2_menu[PP2_MENU_PLAY]->oy, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW);
	
			pp2_menu[PP2_MENU_PLAY_QUICK_PLAY] = t3f_create_gui(0, 0);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_QUICK_PLAY], pp2_menu_proc_play_1_hit, "1-Hit Elimination", pp2_font[PP2_FONT_COMIC_16], 320, 240, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_QUICK_PLAY], pp2_menu_proc_play_21_stomp, "21 Stomp", pp2_font[PP2_FONT_COMIC_16], 320, 240 + 24, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_QUICK_PLAY], pp2_menu_proc_play_death_match, "2 Minute Death Match", pp2_font[PP2_FONT_COMIC_16], 320, 240 + 24 * 2, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_QUICK_PLAY], pp2_menu_proc_play_coin_rush, "Coin Rush", pp2_font[PP2_FONT_COMIC_16], 320, 240 + 24 * 3, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
			t3f_center_gui(pp2_menu[PP2_MENU_PLAY_QUICK_PLAY], 200.0, 456.0);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_QUICK_PLAY], pp2_menu_proc_overlay_back, "< Back", pp2_font[PP2_FONT_COMIC_16], 0, 480 - al_get_font_line_height(pp2_font[PP2_FONT_COMIC_16]) - pp2_menu[PP2_MENU_PLAY_QUICK_PLAY]->oy, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW);
			
			pp2_menu[PP2_MENU_PLAY_SINGLE] = t3f_create_gui(0, 0);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_SINGLE], pp2_menu_proc_play_explore, "Explore", pp2_font[PP2_FONT_COMIC_16], 320, 240, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
			t3f_center_gui(pp2_menu[PP2_MENU_PLAY_SINGLE], 200.0, 456.0);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_SINGLE], pp2_menu_proc_overlay_back, "< Back", pp2_font[PP2_FONT_COMIC_16], 0, 480 - al_get_font_line_height(pp2_font[PP2_FONT_COMIC_16]) - pp2_menu[PP2_MENU_PLAY_SINGLE]->oy, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW);
			
			pp2_menu[PP2_MENU_PLAY_STOCK] = t3f_create_gui(0, 0);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_STOCK], NULL, "Standard", pp2_font[PP2_FONT_COMIC_16], 160, 240, PP2_MENU_HEADER_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_STOCK], NULL, pp2_menu_text[8], pp2_font[PP2_FONT_COMIC_16], 160, 240 + 24, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_STOCK], pp2_menu_proc_normal_stock_left, "<", pp2_font[PP2_FONT_COMIC_16], 160 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "100") / 2 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "<"), 240 + 24, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_STOCK], pp2_menu_proc_normal_stock_right, ">", pp2_font[PP2_FONT_COMIC_16], 160 + al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "100") / 2, 240 + 24, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_STOCK], NULL, "Splitter", pp2_font[PP2_FONT_COMIC_16], 160, 240 + 24 * 2, PP2_MENU_HEADER_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_STOCK], NULL, pp2_menu_text[9], pp2_font[PP2_FONT_COMIC_16], 160, 240 + 24 * 3, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_STOCK], pp2_menu_proc_splitter_stock_left, "<", pp2_font[PP2_FONT_COMIC_16], 160 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "100") / 2 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "<"), 240 + 24 * 3, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_STOCK], pp2_menu_proc_splitter_stock_right, ">", pp2_font[PP2_FONT_COMIC_16], 160 + al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "100") / 2, 240 + 24 * 3, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_STOCK], NULL, "Mine", pp2_font[PP2_FONT_COMIC_16], 160, 240 + 24 * 4, PP2_MENU_HEADER_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_STOCK], NULL, pp2_menu_text[10], pp2_font[PP2_FONT_COMIC_16], 160, 240 + 24 * 5, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_STOCK], pp2_menu_proc_mine_stock_left, "<", pp2_font[PP2_FONT_COMIC_16], 160 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "100") / 2 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "<"), 240 + 24 * 5, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_STOCK], pp2_menu_proc_mine_stock_right, ">", pp2_font[PP2_FONT_COMIC_16], 160 + al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "100") / 2, 240 + 24 * 5, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_STOCK], NULL, "Bouncer", pp2_font[PP2_FONT_COMIC_16], 160, 240 + 24 * 6, PP2_MENU_HEADER_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_STOCK], NULL, pp2_menu_text[11], pp2_font[PP2_FONT_COMIC_16], 160, 240 + 24 * 7, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_STOCK], pp2_menu_proc_bouncer_stock_left, "<", pp2_font[PP2_FONT_COMIC_16], 160 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "100") / 2 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "<"), 240 + 24 * 7, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_STOCK], pp2_menu_proc_bouncer_stock_right, ">", pp2_font[PP2_FONT_COMIC_16], 160 + al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "100") / 2, 240 + 24 * 7, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_STOCK], NULL, "Seeker", pp2_font[PP2_FONT_COMIC_16], 480, 240, PP2_MENU_HEADER_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_STOCK], NULL, pp2_menu_text[12], pp2_font[PP2_FONT_COMIC_16], 480, 240 + 24, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_STOCK], pp2_menu_proc_seeker_stock_left, "<", pp2_font[PP2_FONT_COMIC_16], 480 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "100") / 2 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "<"), 240 + 24, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_STOCK], pp2_menu_proc_seeker_stock_right, ">", pp2_font[PP2_FONT_COMIC_16], 480 + al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "100") / 2, 240 + 24, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_STOCK], NULL, "Reflector", pp2_font[PP2_FONT_COMIC_16], 480, 240 + 24 * 2, PP2_MENU_HEADER_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_STOCK], NULL, pp2_menu_text[13], pp2_font[PP2_FONT_COMIC_16], 480, 240 + 24 * 3, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_STOCK], pp2_menu_proc_reflector_stock_left, "<", pp2_font[PP2_FONT_COMIC_16], 480 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "100") / 2 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "<"), 240 + 24 * 3, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_STOCK], pp2_menu_proc_reflector_stock_right, ">", pp2_font[PP2_FONT_COMIC_16], 480 + al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "100") / 2, 240 + 24 * 3, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_STOCK], NULL, "Proximity Mine", pp2_font[PP2_FONT_COMIC_16], 480, 240 + 24 * 4, PP2_MENU_HEADER_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_STOCK], NULL, pp2_menu_text[14], pp2_font[PP2_FONT_COMIC_16], 480, 240 + 24 * 5, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_STOCK], pp2_menu_proc_pmine_stock_left, "<", pp2_font[PP2_FONT_COMIC_16], 480 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "100") / 2 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "<"), 240 + 24 * 5, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_STOCK], pp2_menu_proc_pmine_stock_right, ">", pp2_font[PP2_FONT_COMIC_16], 480 + al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "100") / 2, 240 + 24 * 5, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_STOCK], NULL, "Ghost", pp2_font[PP2_FONT_COMIC_16], 480, 240 + 24 * 6, PP2_MENU_HEADER_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_STOCK], NULL, pp2_menu_text[15], pp2_font[PP2_FONT_COMIC_16], 480, 240 + 24 * 7, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_STOCK], pp2_menu_proc_ghost_stock_left, "<", pp2_font[PP2_FONT_COMIC_16], 480 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "100") / 2 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "<"), 240 + 24 * 7, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_STOCK], pp2_menu_proc_ghost_stock_right, ">", pp2_font[PP2_FONT_COMIC_16], 480 + al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "100") / 2, 240 + 24 * 7, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_center_gui(pp2_menu[PP2_MENU_PLAY_STOCK], 200.0, 456.0);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_STOCK], pp2_menu_proc_overlay_back, "< Back", pp2_font[PP2_FONT_COMIC_16], 0, 480 - al_get_font_line_height(pp2_font[PP2_FONT_COMIC_16]) - pp2_menu[PP2_MENU_PLAY_STOCK]->oy, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW);
			
			pp2_menu[PP2_MENU_PLAY_AMMO] = t3f_create_gui(0, 0);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_AMMO], NULL, "Standard", pp2_font[PP2_FONT_COMIC_16], 160, 240, PP2_MENU_HEADER_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_AMMO], NULL, pp2_menu_text[8], pp2_font[PP2_FONT_COMIC_16], 160, 240 + 24, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_AMMO], pp2_menu_proc_normal_ammo_left, "<", pp2_font[PP2_FONT_COMIC_16], 160 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "Off ") / 2 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "<"), 240 + 24, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_AMMO], pp2_menu_proc_normal_ammo_right, ">", pp2_font[PP2_FONT_COMIC_16], 160 + al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "Off ") / 2, 240 + 24, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_AMMO], NULL, "Splitter", pp2_font[PP2_FONT_COMIC_16], 160, 240 + 24 * 2, PP2_MENU_HEADER_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_AMMO], NULL, pp2_menu_text[9], pp2_font[PP2_FONT_COMIC_16], 160, 240 + 24 * 3, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_AMMO], pp2_menu_proc_splitter_ammo_left, "<", pp2_font[PP2_FONT_COMIC_16], 160 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "Off ") / 2 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "<"), 240 + 24 * 3, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_AMMO], pp2_menu_proc_splitter_ammo_right, ">", pp2_font[PP2_FONT_COMIC_16], 160 + al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "Off ") / 2, 240 + 24 * 3, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_AMMO], NULL, "Mine", pp2_font[PP2_FONT_COMIC_16], 160, 240 + 24 * 4, PP2_MENU_HEADER_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_AMMO], NULL, pp2_menu_text[10], pp2_font[PP2_FONT_COMIC_16], 160, 240 + 24 * 5, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_AMMO], pp2_menu_proc_mine_ammo_left, "<", pp2_font[PP2_FONT_COMIC_16], 160 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "Off ") / 2 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "<"), 240 + 24 * 5, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_AMMO], pp2_menu_proc_mine_ammo_right, ">", pp2_font[PP2_FONT_COMIC_16], 160 + al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "Off ") / 2, 240 + 24 * 5, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_AMMO], NULL, "Bouncer", pp2_font[PP2_FONT_COMIC_16], 160, 240 + 24 * 6, PP2_MENU_HEADER_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_AMMO], NULL, pp2_menu_text[11], pp2_font[PP2_FONT_COMIC_16], 160, 240 + 24 * 7, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_AMMO], pp2_menu_proc_bouncer_ammo_left, "<", pp2_font[PP2_FONT_COMIC_16], 160 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "Off ") / 2 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "<"), 240 + 24 * 7, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_AMMO], pp2_menu_proc_bouncer_ammo_right, ">", pp2_font[PP2_FONT_COMIC_16], 160 + al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "Off ") / 2, 240 + 24 * 7, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_AMMO], NULL, "Seeker", pp2_font[PP2_FONT_COMIC_16], 480, 240, PP2_MENU_HEADER_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_AMMO], NULL, pp2_menu_text[12], pp2_font[PP2_FONT_COMIC_16], 480, 240 + 24, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_AMMO], pp2_menu_proc_seeker_ammo_left, "<", pp2_font[PP2_FONT_COMIC_16], 480 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "Off ") / 2 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "<"), 240 + 24, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_AMMO], pp2_menu_proc_seeker_ammo_right, ">", pp2_font[PP2_FONT_COMIC_16], 480 + al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "Off ") / 2, 240 + 24, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_AMMO], NULL, "Reflector", pp2_font[PP2_FONT_COMIC_16], 480, 240 + 24 * 2, PP2_MENU_HEADER_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_AMMO], NULL, pp2_menu_text[13], pp2_font[PP2_FONT_COMIC_16], 480, 240 + 24 * 3, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_AMMO], pp2_menu_proc_reflector_ammo_left, "<", pp2_font[PP2_FONT_COMIC_16], 480 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "Off ") / 2 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "<"), 240 + 24 * 3, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_AMMO], pp2_menu_proc_reflector_ammo_right, ">", pp2_font[PP2_FONT_COMIC_16], 480 + al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "Off ") / 2, 240 + 24 * 3, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_AMMO], NULL, "Proximity Mine", pp2_font[PP2_FONT_COMIC_16], 480, 240 + 24 * 4, PP2_MENU_HEADER_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_AMMO], NULL, pp2_menu_text[14], pp2_font[PP2_FONT_COMIC_16], 480, 240 + 24 * 5, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_AMMO], pp2_menu_proc_pmine_ammo_left, "<", pp2_font[PP2_FONT_COMIC_16], 480 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "Off ") / 2 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "<"), 240 + 24 * 5, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_AMMO], pp2_menu_proc_pmine_ammo_right, ">", pp2_font[PP2_FONT_COMIC_16], 480 + al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "Off ") / 2, 240 + 24 * 5, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_AMMO], NULL, "Ghost", pp2_font[PP2_FONT_COMIC_16], 480, 240 + 24 * 6, PP2_MENU_HEADER_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_AMMO], NULL, pp2_menu_text[15], pp2_font[PP2_FONT_COMIC_16], 480, 240 + 24 * 7, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_AMMO], pp2_menu_proc_ghost_ammo_left, "<", pp2_font[PP2_FONT_COMIC_16], 480 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "Off ") / 2 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "<"), 240 + 24 * 7, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_AMMO], pp2_menu_proc_ghost_ammo_right, ">", pp2_font[PP2_FONT_COMIC_16], 480 + al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "Off ") / 2, 240 + 24 * 7, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_center_gui(pp2_menu[PP2_MENU_PLAY_AMMO], 200.0, 456.0);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_AMMO], pp2_menu_proc_overlay_back, "< Back", pp2_font[PP2_FONT_COMIC_16], 0, 480 - al_get_font_line_height(pp2_font[PP2_FONT_COMIC_16]) - pp2_menu[PP2_MENU_PLAY_AMMO]->oy, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW);
		
			pp2_menu[PP2_MENU_PLAY_POWERUPS] = t3f_create_gui(0, 0);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_POWERUPS], NULL, "Cloak", pp2_font[PP2_FONT_COMIC_16], 160, 240, PP2_MENU_HEADER_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_POWERUPS], NULL, pp2_menu_text[8], pp2_font[PP2_FONT_COMIC_16], 160, 240 + 24, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_POWERUPS], pp2_menu_proc_cloak_power_left, "<", pp2_font[PP2_FONT_COMIC_16], 160 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "Off ") / 2 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "<"), 240 + 24, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_POWERUPS], pp2_menu_proc_cloak_power_right, ">", pp2_font[PP2_FONT_COMIC_16], 160 + al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "Off ") / 2, 240 + 24, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_POWERUPS], NULL, "Jump", pp2_font[PP2_FONT_COMIC_16], 160, 240 + 24 * 2, PP2_MENU_HEADER_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_POWERUPS], NULL, pp2_menu_text[9], pp2_font[PP2_FONT_COMIC_16], 160, 240 + 24 * 3, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_POWERUPS], pp2_menu_proc_jump_power_left, "<", pp2_font[PP2_FONT_COMIC_16], 160 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "Off ") / 2 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "<"), 240 + 24 * 3, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_POWERUPS], pp2_menu_proc_jump_power_right, ">", pp2_font[PP2_FONT_COMIC_16], 160 + al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "Off ") / 2, 240 + 24 * 3, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_POWERUPS], NULL, "Run", pp2_font[PP2_FONT_COMIC_16], 160, 240 + 24 * 4, PP2_MENU_HEADER_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_POWERUPS], NULL, pp2_menu_text[10], pp2_font[PP2_FONT_COMIC_16], 160, 240 + 24 * 5, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_POWERUPS], pp2_menu_proc_run_power_left, "<", pp2_font[PP2_FONT_COMIC_16], 160 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "Off ") / 2 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "<"), 240 + 24 * 5, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_POWERUPS], pp2_menu_proc_run_power_right, ">", pp2_font[PP2_FONT_COMIC_16], 160 + al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "Off ") / 2, 240 + 24 * 5, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_POWERUPS], NULL, "Deflect", pp2_font[PP2_FONT_COMIC_16], 480, 240, PP2_MENU_HEADER_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_POWERUPS], NULL, pp2_menu_text[11], pp2_font[PP2_FONT_COMIC_16], 480, 240 + 24, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_POWERUPS], pp2_menu_proc_deflect_power_left, "<", pp2_font[PP2_FONT_COMIC_16], 480 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "Off ") / 2 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "<"), 240 + 24, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_POWERUPS], pp2_menu_proc_deflect_power_right, ">", pp2_font[PP2_FONT_COMIC_16], 480 + al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "Off ") / 2, 240 + 24, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_POWERUPS], NULL, "Fly", pp2_font[PP2_FONT_COMIC_16], 480, 240 + 24 * 2, PP2_MENU_HEADER_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_POWERUPS], NULL, pp2_menu_text[12], pp2_font[PP2_FONT_COMIC_16], 480, 240 + 24 * 3, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_POWERUPS], pp2_menu_proc_fly_power_left, "<", pp2_font[PP2_FONT_COMIC_16], 480 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "Off ") / 2 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "<"), 240 + 24 * 3, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_POWERUPS], pp2_menu_proc_fly_power_right, ">", pp2_font[PP2_FONT_COMIC_16], 480 + al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "Off ") / 2, 240 + 24 * 3, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_POWERUPS], NULL, "Turbo", pp2_font[PP2_FONT_COMIC_16], 480, 240 + 24 * 4, PP2_MENU_HEADER_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_POWERUPS], NULL, pp2_menu_text[13], pp2_font[PP2_FONT_COMIC_16], 480, 240 + 24 * 5, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_POWERUPS], pp2_menu_proc_turbo_power_left, "<", pp2_font[PP2_FONT_COMIC_16], 480 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "Off ") / 2 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "<"), 240 + 24 * 5, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_POWERUPS], pp2_menu_proc_turbo_power_right, ">", pp2_font[PP2_FONT_COMIC_16], 480 + al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "Off ") / 2, 240 + 24 * 5, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_center_gui(pp2_menu[PP2_MENU_PLAY_POWERUPS], 200.0, 456.0);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_POWERUPS], pp2_menu_proc_overlay_back, "< Back", pp2_font[PP2_FONT_COMIC_16], 0, 480 - al_get_font_line_height(pp2_font[PP2_FONT_COMIC_16]) - pp2_menu[PP2_MENU_PLAY_POWERUPS]->oy, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW);
			
		pp2_menu[PP2_MENU_PLAY_NETWORK] = t3f_create_gui(0, 0);
		t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_NETWORK], pp2_menu_proc_main_play_online, "Play Online", pp2_font[PP2_FONT_COMIC_16], 320, 240 + 24, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
		t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_NETWORK], pp2_menu_proc_main_play_lan, "LAN Play", pp2_font[PP2_FONT_COMIC_16], 320, 240 + 24 * 2, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
		t3f_center_gui(pp2_menu[PP2_MENU_PLAY_NETWORK], 200.0, 456.0);
		t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_NETWORK], pp2_menu_proc_overlay_back, "< Back", pp2_font[PP2_FONT_COMIC_16], 0, 480 - al_get_font_line_height(pp2_font[PP2_FONT_COMIC_16]) - pp2_menu[PP2_MENU_PLAY_NETWORK]->oy, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW);
	
			pp2_menu[PP2_MENU_PLAY_ONLINE] = t3f_create_gui(0, 0);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_ONLINE], pp2_menu_proc_play_online_host, "Host Game", pp2_font[PP2_FONT_COMIC_16], 320, 240, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_ONLINE], pp2_menu_proc_play_online_join, "Join Game", pp2_font[PP2_FONT_COMIC_16], 320, 240 + 24, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
			t3f_center_gui(pp2_menu[PP2_MENU_PLAY_ONLINE], 200.0, 456.0);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_ONLINE], pp2_menu_proc_overlay_back, "< Back", pp2_font[PP2_FONT_COMIC_16], 0, 480 - al_get_font_line_height(pp2_font[PP2_FONT_COMIC_16]) - pp2_menu[PP2_MENU_PLAY_ONLINE]->oy, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW);

				pp2_menu[PP2_MENU_NETWORK_ID] = t3f_create_gui(0, 0);
				t3f_add_gui_text_element(pp2_menu[PP2_MENU_NETWORK_ID], NULL, "Enter Network ID", pp2_font[PP2_FONT_COMIC_16], 320, 240, PP2_MENU_HEADER_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
				t3f_add_gui_text_element(pp2_menu[PP2_MENU_NETWORK_ID], NULL, pp2_entered_text, pp2_font[PP2_FONT_COMIC_16], 320, 240 + 24, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
				t3f_add_gui_text_element(pp2_menu[PP2_MENU_NETWORK_ID], pp2_menu_proc_network_id_ok, "OK", pp2_font[PP2_FONT_COMIC_16], 320, 240 + 24 * 2, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
				t3f_center_gui(pp2_menu[PP2_MENU_NETWORK_ID], 200.0, 456.0);
				t3f_add_gui_text_element(pp2_menu[PP2_MENU_NETWORK_ID], pp2_menu_proc_overlay_back, "< Back", pp2_font[PP2_FONT_COMIC_16], 0, 480 - al_get_font_line_height(pp2_font[PP2_FONT_COMIC_16]) - pp2_menu[PP2_MENU_NETWORK_ID]->oy, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW);
				
				pp2_menu[PP2_MENU_HOST_NAME] = t3f_create_gui(0, 0);
				t3f_add_gui_text_element(pp2_menu[PP2_MENU_HOST_NAME], NULL, "Enter Server Name", pp2_font[PP2_FONT_COMIC_16], 320, 240, PP2_MENU_HEADER_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
				t3f_add_gui_text_element(pp2_menu[PP2_MENU_HOST_NAME], NULL, pp2_entered_text, pp2_font[PP2_FONT_COMIC_16], 320, 240 + 24, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
				t3f_add_gui_text_element(pp2_menu[PP2_MENU_HOST_NAME], pp2_menu_proc_host_name_ok, "Begin Hosting", pp2_font[PP2_FONT_COMIC_16], 320, 240 + 24 * 2, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
				t3f_center_gui(pp2_menu[PP2_MENU_HOST_NAME], 200.0, 456.0);
				t3f_add_gui_text_element(pp2_menu[PP2_MENU_HOST_NAME], pp2_menu_proc_overlay_back, "< Back", pp2_font[PP2_FONT_COMIC_16], 0, 480 - al_get_font_line_height(pp2_font[PP2_FONT_COMIC_16]) - pp2_menu[PP2_MENU_HOST_NAME]->oy, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW);
				
			pp2_menu[PP2_MENU_PLAY_LAN] = t3f_create_gui(0, 0);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_LAN], pp2_menu_proc_play_lan_host, "Host Game", pp2_font[PP2_FONT_COMIC_16], 320, 240, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_LAN], pp2_menu_proc_play_lan_join, "Join Game", pp2_font[PP2_FONT_COMIC_16], 320, 240 + 24, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
			t3f_center_gui(pp2_menu[PP2_MENU_PLAY_LAN], 200.0, 456.0);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAY_LAN], pp2_menu_proc_overlay_back, "< Back", pp2_font[PP2_FONT_COMIC_16], 0, 480 - al_get_font_line_height(pp2_font[PP2_FONT_COMIC_16]) - pp2_menu[PP2_MENU_PLAY_ONLINE]->oy, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW);

				pp2_menu[PP2_MENU_HOST_IP] = t3f_create_gui(0, 0);
				t3f_add_gui_text_element(pp2_menu[PP2_MENU_HOST_IP], NULL, "Enter IP Address", pp2_font[PP2_FONT_COMIC_16], 320, 240, PP2_MENU_HEADER_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
				t3f_add_gui_text_element(pp2_menu[PP2_MENU_HOST_IP], NULL, pp2_entered_text, pp2_font[PP2_FONT_COMIC_16], 320, 240 + 24, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
				t3f_add_gui_text_element(pp2_menu[PP2_MENU_HOST_IP], pp2_menu_proc_host_ip_ok, "Connect to Server", pp2_font[PP2_FONT_COMIC_16], 320, 240 + 24 * 2, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
				t3f_center_gui(pp2_menu[PP2_MENU_HOST_IP], 200.0, 456.0);
				t3f_add_gui_text_element(pp2_menu[PP2_MENU_HOST_IP], pp2_menu_proc_overlay_back, "< Back", pp2_font[PP2_FONT_COMIC_16], 0, 480 - al_get_font_line_height(pp2_font[PP2_FONT_COMIC_16]) - pp2_menu[PP2_MENU_HOST_NAME]->oy, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW);

		pp2_menu[PP2_MENU_OPTIONS] = t3f_create_gui(0, 0);
		t3f_add_gui_text_element(pp2_menu[PP2_MENU_OPTIONS], pp2_menu_proc_options_controllers, "Controllers", pp2_font[PP2_FONT_COMIC_16], 320, 240, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
		t3f_add_gui_text_element(pp2_menu[PP2_MENU_OPTIONS], pp2_menu_proc_options_audio, "Audio", pp2_font[PP2_FONT_COMIC_16], 320, 240 + 24, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
		t3f_add_gui_text_element(pp2_menu[PP2_MENU_OPTIONS], pp2_menu_proc_options_video, "Video", pp2_font[PP2_FONT_COMIC_16], 320, 240 + 24 * 2, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
		t3f_add_gui_text_element(pp2_menu[PP2_MENU_OPTIONS], pp2_menu_proc_options_network, "Network", pp2_font[PP2_FONT_COMIC_16], 320, 240 + 24 * 3, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
		t3f_center_gui(pp2_menu[PP2_MENU_OPTIONS], 200.0, 456.0);
		t3f_add_gui_text_element(pp2_menu[PP2_MENU_OPTIONS], pp2_menu_proc_overlay_back, "< Back", pp2_font[PP2_FONT_COMIC_16], 0, 480 - al_get_font_line_height(pp2_font[PP2_FONT_COMIC_16]) - pp2_menu[PP2_MENU_OPTIONS]->oy, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW);

			pp2_menu[PP2_MENU_CONTROLLERS] = t3f_create_gui(0, 0);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_CONTROLLERS], pp2_menu_proc_options_controller_1, "Controller 1", pp2_font[PP2_FONT_COMIC_16], 320, 240, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_CONTROLLERS], pp2_menu_proc_options_controller_2, "Controller 2", pp2_font[PP2_FONT_COMIC_16], 320, 240 + 24, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_CONTROLLERS], pp2_menu_proc_options_controller_3, "Controller 3", pp2_font[PP2_FONT_COMIC_16], 320, 240 + 24 * 2, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_CONTROLLERS], pp2_menu_proc_options_controller_4, "Controller 4", pp2_font[PP2_FONT_COMIC_16], 320, 240 + 24 * 3, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_CONTROLLERS], pp2_menu_proc_options_controller_reset, "Autodetect", pp2_font[PP2_FONT_COMIC_16], 320, 240 + 24 * 4, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
			t3f_center_gui(pp2_menu[PP2_MENU_CONTROLLERS], 200.0, 456.0);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_CONTROLLERS], pp2_menu_proc_overlay_back, "< Back", pp2_font[PP2_FONT_COMIC_16], 0, 480 - al_get_font_line_height(pp2_font[PP2_FONT_COMIC_16]) - pp2_menu[PP2_MENU_CONTROLLERS]->oy, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW);

				pp2_menu[PP2_MENU_CONTROLLER] = t3f_create_gui(0, 0);
				t3f_add_gui_text_element(pp2_menu[PP2_MENU_CONTROLLER], NULL, pp2_controller_binding_text[0], pp2_font[PP2_FONT_COMIC_16], 320, 0, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
				t3f_add_gui_text_element(pp2_menu[PP2_MENU_CONTROLLER], NULL, pp2_controller_binding_text[10], pp2_font[PP2_FONT_COMIC_10], 320, 0 + 24 * 1, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
				t3f_add_gui_text_element(pp2_menu[PP2_MENU_CONTROLLER], NULL, "Up", pp2_font[PP2_FONT_COMIC_12], 160, 0 + 16 * 2, PP2_MENU_HEADER_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
				t3f_add_gui_text_element(pp2_menu[PP2_MENU_CONTROLLER], pp2_menu_proc_controller_up, pp2_controller_binding_text[1], pp2_font[PP2_FONT_COMIC_12], 160, 0 + 16 * 3, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
				t3f_add_gui_text_element(pp2_menu[PP2_MENU_CONTROLLER], NULL, "Down", pp2_font[PP2_FONT_COMIC_12], 160, 0 + 16 * 4, PP2_MENU_HEADER_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
				t3f_add_gui_text_element(pp2_menu[PP2_MENU_CONTROLLER], pp2_menu_proc_controller_down, pp2_controller_binding_text[2], pp2_font[PP2_FONT_COMIC_12], 160, 0 + 16 * 5, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
				t3f_add_gui_text_element(pp2_menu[PP2_MENU_CONTROLLER], NULL, "Left", pp2_font[PP2_FONT_COMIC_12], 160, 0 + 16 * 6, PP2_MENU_HEADER_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
				t3f_add_gui_text_element(pp2_menu[PP2_MENU_CONTROLLER], pp2_menu_proc_controller_left, pp2_controller_binding_text[3], pp2_font[PP2_FONT_COMIC_12], 160, 0 + 16 * 7, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
				t3f_add_gui_text_element(pp2_menu[PP2_MENU_CONTROLLER], NULL, "Right", pp2_font[PP2_FONT_COMIC_12], 160, 0 + 16 * 8, PP2_MENU_HEADER_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
				t3f_add_gui_text_element(pp2_menu[PP2_MENU_CONTROLLER], pp2_menu_proc_controller_right, pp2_controller_binding_text[4], pp2_font[PP2_FONT_COMIC_12], 160, 0 + 16 * 9, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
				t3f_add_gui_text_element(pp2_menu[PP2_MENU_CONTROLLER], NULL, "Jump", pp2_font[PP2_FONT_COMIC_12], 480, 0 + 16 * 2, PP2_MENU_HEADER_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
				t3f_add_gui_text_element(pp2_menu[PP2_MENU_CONTROLLER], pp2_menu_proc_controller_jump, pp2_controller_binding_text[5], pp2_font[PP2_FONT_COMIC_12], 480, 0 + 16 * 3, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
				t3f_add_gui_text_element(pp2_menu[PP2_MENU_CONTROLLER], NULL, "Fire", pp2_font[PP2_FONT_COMIC_12], 480, 0 + 16 * 4, PP2_MENU_HEADER_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
				t3f_add_gui_text_element(pp2_menu[PP2_MENU_CONTROLLER], pp2_menu_proc_controller_fire, pp2_controller_binding_text[6], pp2_font[PP2_FONT_COMIC_12], 480, 0 + 16 * 5, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
				t3f_add_gui_text_element(pp2_menu[PP2_MENU_CONTROLLER], NULL, "Select", pp2_font[PP2_FONT_COMIC_12], 480, 0 + 16 * 6, PP2_MENU_HEADER_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
				t3f_add_gui_text_element(pp2_menu[PP2_MENU_CONTROLLER], pp2_menu_proc_controller_select, pp2_controller_binding_text[7], pp2_font[PP2_FONT_COMIC_12], 480, 0 + 16 * 7, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
				t3f_add_gui_text_element(pp2_menu[PP2_MENU_CONTROLLER], NULL, "Strafe", pp2_font[PP2_FONT_COMIC_12], 480, 0 + 16 * 8, PP2_MENU_HEADER_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
				t3f_add_gui_text_element(pp2_menu[PP2_MENU_CONTROLLER], pp2_menu_proc_controller_strafe, pp2_controller_binding_text[8], pp2_font[PP2_FONT_COMIC_12], 480, 0 + 16 * 9, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
				t3f_add_gui_text_element(pp2_menu[PP2_MENU_CONTROLLER], NULL, "Show Scores", pp2_font[PP2_FONT_COMIC_12], 320, 0 + 16 * 10, PP2_MENU_HEADER_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
				t3f_add_gui_text_element(pp2_menu[PP2_MENU_CONTROLLER], pp2_menu_proc_controller_show_scores, pp2_controller_binding_text[9], pp2_font[PP2_FONT_COMIC_12], 320, 0 + 16 * 11, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
				t3f_center_gui(pp2_menu[PP2_MENU_CONTROLLER], 200.0, 456.0);
				t3f_add_gui_text_element(pp2_menu[PP2_MENU_CONTROLLER], pp2_menu_proc_overlay_back, "< Back", pp2_font[PP2_FONT_COMIC_16], 0, 480 - al_get_font_line_height(pp2_font[PP2_FONT_COMIC_16]) - pp2_menu[PP2_MENU_CONTROLLER]->oy, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW);

			pp2_menu[PP2_MENU_AUDIO] = t3f_create_gui(0, 0);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_AUDIO], NULL, "Music Volume", pp2_font[PP2_FONT_COMIC_16], 320, 240, PP2_MENU_HEADER_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_AUDIO], NULL, pp2_menu_text[0], pp2_font[PP2_FONT_COMIC_16], 320, 240 + 24, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_AUDIO], pp2_menu_proc_music_down, "<", pp2_font[PP2_FONT_COMIC_16], 320 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "100%%") / 2 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "<"), 240 + 24, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_AUDIO], pp2_menu_proc_music_up, ">", pp2_font[PP2_FONT_COMIC_16], 320 + al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "100%%") / 2, 240 + 24, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_AUDIO], NULL, "Sound Volume", pp2_font[PP2_FONT_COMIC_16], 320, 240 + 24 * 2, PP2_MENU_HEADER_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_AUDIO], NULL, pp2_menu_text[1], pp2_font[PP2_FONT_COMIC_16], 320, 240 + 24 * 3, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_AUDIO], pp2_menu_proc_sound_down, "<", pp2_font[PP2_FONT_COMIC_16], 320 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "100%%") / 2 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "<"), 240 + 24 * 3, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_AUDIO], pp2_menu_proc_sound_up, ">", pp2_font[PP2_FONT_COMIC_16], 320 + al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "100%%") / 2, 240 + 24 * 3, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_center_gui(pp2_menu[PP2_MENU_AUDIO], 200.0, 456.0);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_AUDIO], pp2_menu_proc_overlay_back, "< Back", pp2_font[PP2_FONT_COMIC_16], 0, 480 - al_get_font_line_height(pp2_font[PP2_FONT_COMIC_16]) - pp2_menu[PP2_MENU_AUDIO]->oy, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW);
			
			pp2_menu[PP2_MENU_VIDEO] = t3f_create_gui(0, 0);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_VIDEO], NULL, "Display", pp2_font[PP2_FONT_COMIC_16], 320, 240, PP2_MENU_HEADER_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_VIDEO], NULL, pp2_menu_text[0], pp2_font[PP2_FONT_COMIC_16], 320, 240 + 24, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_VIDEO], pp2_menu_proc_display_toggle, "<", pp2_font[PP2_FONT_COMIC_16], 320 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], " Full Screen ") / 2 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "<"), 240 + 24, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_VIDEO], pp2_menu_proc_display_toggle, ">", pp2_font[PP2_FONT_COMIC_16], 320 + al_get_text_width(pp2_font[PP2_FONT_COMIC_16], " Full Screen ") / 2, 240 + 24, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_VIDEO], NULL, "Resolution", pp2_font[PP2_FONT_COMIC_16], 320, 240 + 24 * 2, PP2_MENU_HEADER_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_VIDEO], NULL, pp2_menu_text[1], pp2_font[PP2_FONT_COMIC_16], 320, 240 + 24 * 3, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_VIDEO], pp2_menu_proc_resolution_left, "<", pp2_font[PP2_FONT_COMIC_16], 320 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], " 1024x768 ") / 2 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "<"), 240 + 24 * 3, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_VIDEO], pp2_menu_proc_resolution_right, ">", pp2_font[PP2_FONT_COMIC_16], 320 + al_get_text_width(pp2_font[PP2_FONT_COMIC_16], " 1024x768 ") / 2, 240 + 24 * 3, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_AUTOHIDE);
			t3f_center_gui(pp2_menu[PP2_MENU_VIDEO], 200.0, 456.0);
			t3f_add_gui_text_element(pp2_menu[PP2_MENU_VIDEO], pp2_menu_proc_overlay_back, "< Back", pp2_font[PP2_FONT_COMIC_16], 0, 480 - al_get_font_line_height(pp2_font[PP2_FONT_COMIC_16]) - pp2_menu[PP2_MENU_VIDEO]->oy, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW);
			
		pp2_menu[PP2_MENU_NETWORK] = t3f_create_gui(0, 0);
		t3f_add_gui_text_element(pp2_menu[PP2_MENU_NETWORK], NULL, "Enter Network ID", pp2_font[PP2_FONT_COMIC_16], 320, 240, PP2_MENU_HEADER_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
		t3f_add_gui_text_element(pp2_menu[PP2_MENU_NETWORK], NULL, pp2_entered_text, pp2_font[PP2_FONT_COMIC_16], 320, 240 + 24, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
		t3f_add_gui_text_element(pp2_menu[PP2_MENU_NETWORK], pp2_menu_proc_network_ok, "OK", pp2_font[PP2_FONT_COMIC_16], 320, 240 + 24 * 2, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
		t3f_center_gui(pp2_menu[PP2_MENU_NETWORK], 200.0, 456.0);
		t3f_add_gui_text_element(pp2_menu[PP2_MENU_NETWORK], pp2_menu_proc_overlay_back, "< Back", pp2_font[PP2_FONT_COMIC_16], 0, 480 - al_get_font_line_height(pp2_font[PP2_FONT_COMIC_16]) - pp2_menu[PP2_MENU_NETWORK]->oy, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW);

	pp2_menu[PP2_MENU_MAIN_HOST] = t3f_create_gui(0, 0);
	t3f_add_gui_text_element(pp2_menu[PP2_MENU_MAIN_HOST], pp2_menu_proc_main_play, "Play", pp2_font[PP2_FONT_COMIC_16], 320, 240, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
	t3f_add_gui_text_element(pp2_menu[PP2_MENU_MAIN_HOST], pp2_menu_proc_main_close_server, "Close Server", pp2_font[PP2_FONT_COMIC_16], 320, 240 + 24, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
	t3f_center_gui(pp2_menu[PP2_MENU_MAIN_HOST], 200.0, 480.0);
	
	pp2_menu[PP2_MENU_MAIN_CLIENT] = t3f_create_gui(0, 0);
	t3f_add_gui_text_element(pp2_menu[PP2_MENU_MAIN_CLIENT], pp2_menu_proc_main_play, "Play", pp2_font[PP2_FONT_COMIC_16], 320, 240, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
	t3f_add_gui_text_element(pp2_menu[PP2_MENU_MAIN_CLIENT], pp2_menu_proc_main_disconnect, "Disconnect", pp2_font[PP2_FONT_COMIC_16], 320, 240 + 24, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
	t3f_center_gui(pp2_menu[PP2_MENU_MAIN_CLIENT], 200.0, 480.0);

	pp2_menu[PP2_MENU_PAUSE] = t3f_create_gui(0, 0);
	t3f_add_gui_text_element(pp2_menu[PP2_MENU_PAUSE], pp2_menu_proc_game_resume, "Resume Game", pp2_font[PP2_FONT_COMIC_16], 320, 240, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
	t3f_add_gui_text_element(pp2_menu[PP2_MENU_PAUSE], pp2_menu_proc_game_end, "End Game", pp2_font[PP2_FONT_COMIC_16], 320, 240 + 24, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
	t3f_center_gui(pp2_menu[PP2_MENU_PAUSE], 200.0, 480.0);

	pp2_menu[PP2_MENU_GAME_OVER] = t3f_create_gui(0, 0);
	t3f_add_gui_text_element(pp2_menu[PP2_MENU_GAME_OVER], pp2_menu_proc_game_rematch, "Rematch", pp2_font[PP2_FONT_COMIC_16], 320, 240, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
	t3f_add_gui_text_element(pp2_menu[PP2_MENU_GAME_OVER], pp2_menu_proc_game_play_again, "Play Again", pp2_font[PP2_FONT_COMIC_16], 320, 240 + 24, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
	t3f_add_gui_text_element(pp2_menu[PP2_MENU_GAME_OVER], pp2_menu_proc_game_new_game, "New Game", pp2_font[PP2_FONT_COMIC_16], 320, 240 + 24 * 2, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
	t3f_add_gui_text_element(pp2_menu[PP2_MENU_GAME_OVER], pp2_menu_proc_game_quit, "Quit", pp2_font[PP2_FONT_COMIC_16], 320, 240 + 24 * 3, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
	t3f_center_gui(pp2_menu[PP2_MENU_GAME_OVER], 200.0, 480.0);
	
	pp2_menu[PP2_MENU_PAUSE_CLIENT] = t3f_create_gui(0, 0);
	t3f_add_gui_text_element(pp2_menu[PP2_MENU_PAUSE_CLIENT], NULL, "Paused", pp2_font[PP2_FONT_COMIC_16], 320, 240, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
	t3f_center_gui(pp2_menu[PP2_MENU_PAUSE_CLIENT], 200.0, 480.0);
	
	pp2_menu[PP2_MENU_NEW_PROFILE] = t3f_create_gui(0, 0);
	t3f_add_gui_text_element(pp2_menu[PP2_MENU_NEW_PROFILE], NULL, "Enter Your Name", pp2_font[PP2_FONT_COMIC_16], 320, 240, PP2_MENU_HEADER_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
	t3f_add_gui_text_element(pp2_menu[PP2_MENU_NEW_PROFILE], NULL, pp2_entered_text, pp2_font[PP2_FONT_COMIC_16], 320, 240 + 24, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW | T3F_GUI_ELEMENT_STATIC);
	t3f_add_gui_text_element(pp2_menu[PP2_MENU_NEW_PROFILE], pp2_menu_proc_new_profile_ok, "OK", pp2_font[PP2_FONT_COMIC_16], 320, 240 + 24 * 2, PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_CENTRE | T3F_GUI_ELEMENT_SHADOW);
	t3f_center_gui(pp2_menu[PP2_MENU_NEW_PROFILE], 200.0, 456.0);
	
	pp2_menu[PP2_MENU_OVERLAY] = t3f_create_gui(0, 0);
	t3f_add_gui_text_element(pp2_menu[PP2_MENU_OVERLAY], pp2_menu_proc_overlay_back, "< Back", pp2_font[PP2_FONT_COMIC_16], 0, 480 - al_get_font_line_height(pp2_font[PP2_FONT_COMIC_16]), PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW);
	
	pp2_menu[PP2_MENU_PLAYER_SETUP_OVERLAY] = t3f_create_gui(0, 0);
	t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAYER_SETUP_OVERLAY], pp2_menu_proc_overlay_back, "< Back", pp2_font[PP2_FONT_COMIC_16], 0, 480 - al_get_font_line_height(pp2_font[PP2_FONT_COMIC_16]), PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW);
	t3f_add_gui_text_element(pp2_menu[PP2_MENU_PLAYER_SETUP_OVERLAY], pp2_menu_proc_overlay_next, "Next >", pp2_font[PP2_FONT_COMIC_16], 640 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "Next >"), 480 - al_get_font_line_height(pp2_font[PP2_FONT_COMIC_16]), PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW);
	
	pp2_menu[PP2_MENU_CLIENT_PLAYER_SETUP_OVERLAY] = t3f_create_gui(0, 0);
	t3f_add_gui_text_element(pp2_menu[PP2_MENU_CLIENT_PLAYER_SETUP_OVERLAY], pp2_menu_proc_overlay_back, "< Back", pp2_font[PP2_FONT_COMIC_16], 0, 480 - al_get_font_line_height(pp2_font[PP2_FONT_COMIC_16]), PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW);
//	t3f_add_gui_text_element(pp2_menu[PP2_MENU_CLIENT_PLAYER_SETUP_OVERLAY], pp2_menu_proc_overlay_next, "Next >", pp2_font[PP2_FONT_COMIC_16], 640 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "Next >"), 480 - al_get_font_line_height(pp2_font[PP2_FONT_COMIC_16]), PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW);
	
	pp2_menu[PP2_MENU_LEVEL_SETUP_OVERLAY] = t3f_create_gui(0, 0);
	t3f_add_gui_text_element(pp2_menu[PP2_MENU_LEVEL_SETUP_OVERLAY], pp2_menu_proc_overlay_back, "< Back", pp2_font[PP2_FONT_COMIC_16], 0, 480 - al_get_font_line_height(pp2_font[PP2_FONT_COMIC_16]), PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW);
	t3f_add_gui_text_element(pp2_menu[PP2_MENU_LEVEL_SETUP_OVERLAY], pp2_menu_proc_overlay_next, "Start Game >", pp2_font[PP2_FONT_COMIC_16], 640 - al_get_text_width(pp2_font[PP2_FONT_COMIC_16], "Start Game >"), 480 - al_get_font_line_height(pp2_font[PP2_FONT_COMIC_16]), PP2_MENU_OPTION_COLOR, T3F_GUI_ELEMENT_SHADOW);
}

void pp2_process_menu(T3F_GUI * menu)
{
	int i;
	
	for(i = 0; i < 4; i++)
	{
		t3f_read_controller(pp2_controller[i]);
		t3f_update_controller(pp2_controller[i]);
		if(!pp2_menu_joystick_disabled && !pp2_entering_text && pp2_menu_joystick_skip == 0)
		{
			if(pp2_controller[i]->state[PP2_CONTROLLER_UP].pressed || pp2_controller[i]->state[PP2_CONTROLLER_LEFT].pressed)
			{
				t3f_select_previous_gui_element(pp2_menu[pp2_current_menu]);
			}
			if(pp2_controller[i]->state[PP2_CONTROLLER_DOWN].pressed || pp2_controller[i]->state[PP2_CONTROLLER_RIGHT].pressed)
			{
				t3f_select_next_gui_element(pp2_menu[pp2_current_menu]);
			}
			if(pp2_controller[i]->state[PP2_CONTROLLER_FIRE].pressed)
			{
				pp2_joystick_menu_activation = true;
				t3f_activate_selected_gui_element(pp2_menu[pp2_current_menu], NULL);
				pp2_joystick_menu_activation = false;
			}
			else if(pp2_controller[i]->state[PP2_CONTROLLER_JUMP].pressed)
			{
				pp2_menu_proc_overlay_back(NULL, 0, NULL);
			}
		}
	}
	if(pp2_menu_joystick_skip > 0)
	{
		pp2_menu_joystick_skip--;
	}
	t3f_process_gui(pp2_menu[pp2_current_menu], NULL);
}

void pp2_menu_logic(void)
{
	pp2_menu_offset -= 0.25;
	if(pp2_menu_offset <= -64.0)
	{
		pp2_menu_offset = 0.0;
	}
	pp2_tick++;
	pp2_process_menu(pp2_menu[pp2_current_menu]);
	if(pp2_menu_stack_size > 0)
	{
		if(pp2_current_menu == PP2_MENU_PLAY_CUSTOM)
		{
			if(pp2_client && !pp2_client->master)
			{
				t3f_process_gui(pp2_menu[PP2_MENU_CLIENT_PLAYER_SETUP_OVERLAY], NULL);
			}
/*			else
			{
				t3f_process_gui(pp2_menu[PP2_MENU_PLAYER_SETUP_OVERLAY]);
			} */
		}
/*		else
		{
			t3f_process_gui(pp2_menu[PP2_MENU_OVERLAY]);
		} */
	}
}

void pp2_menu_render(void)
{
	int tw = 640 / al_get_bitmap_width(pp2_bitmap[PP2_BITMAP_MENU_BG]) + 1;
	int th = 480 / al_get_bitmap_height(pp2_bitmap[PP2_BITMAP_MENU_BG]) + 2;
	int i, j;
	
	al_hold_bitmap_drawing(true);
	for(i = 0; i < th; i++)
	{
		for(j = 0; j < tw; j++)
		{
			al_draw_tinted_bitmap(pp2_bitmap[PP2_BITMAP_MENU_BG], al_map_rgba_f(0.75, 0.75, 1.0, 1.0), (float)(j * al_get_bitmap_width(pp2_bitmap[PP2_BITMAP_MENU_BG])) + pp2_menu_offset, (float)(i * al_get_bitmap_height(pp2_bitmap[PP2_BITMAP_MENU_BG])) + pp2_menu_offset, 0);
		}
	}
	al_draw_tinted_bitmap(pp2_bitmap[PP2_BITMAP_MENU_LOGO], al_map_rgba_f(0.0, 0.0, 0.0, 0.5), 160.0 + 2, 0.0 + 2, 0);
	al_draw_bitmap(pp2_bitmap[PP2_BITMAP_MENU_LOGO], 160.0, 0.0, 0);
	t3f_render_gui(pp2_menu[pp2_current_menu]);
	if(pp2_menu_stack_size > 0)
	{
		if(pp2_current_menu == PP2_MENU_PLAY_CUSTOM)
		{
			if(pp2_client && !pp2_client->master)
			{
				t3f_render_gui(pp2_menu[PP2_MENU_CLIENT_PLAYER_SETUP_OVERLAY]);
			}
/*			else
			{
				t3f_render_gui(pp2_menu[PP2_MENU_PLAYER_SETUP_OVERLAY]);
			} */
		}
/*		else
		{
			t3f_render_gui(pp2_menu[PP2_MENU_OVERLAY]);
		} */
	}
	if(pp2_lan_play)
	{
	}
	al_hold_bitmap_drawing(false);
}
