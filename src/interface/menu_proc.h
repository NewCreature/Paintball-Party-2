#ifndef PP2_MENU_PROC_H
#define PP2_MENU_PROC_H

#include "interface.h"

void pp2_select_menu(PP2_INTERFACE * ip, int menu);
void pp2_select_previous_menu(void);

void pp2_generate_profiles_menu(PP2_INTERFACE * ip, PP2_RESOURCES * resources);

int pp2_menu_proc_main_play(void * data, int i, void * p);
	int pp2_menu_proc_play_quick_play(void * data, int i, void * p);
		int pp2_menu_proc_play_1_hit(void * data, int i, void * p);
		int pp2_menu_proc_play_21_stomp(void * data, int i, void * p);
		int pp2_menu_proc_play_death_match(void * data, int i, void * p);
		int pp2_menu_proc_play_coin_rush(void * data, int i, void * p);
		int pp2_menu_proc_play_battle_royale(void * data, int i, void * p);
		int pp2_menu_proc_play_explore(void * data, int i, void * p);
	int pp2_menu_proc_play_custom(void * data, int i, void * p);
		int pp2_menu_proc_normal_stock_left(void * data, int i, void * p);
		int pp2_menu_proc_normal_stock_right(void * data, int i, void * p);
		int pp2_menu_proc_splitter_stock_left(void * data, int i, void * p);
		int pp2_menu_proc_splitter_stock_right(void * data, int i, void * p);
		int pp2_menu_proc_mine_stock_left(void * data, int i, void * p);
		int pp2_menu_proc_mine_stock_right(void * data, int i, void * p);
		int pp2_menu_proc_bouncer_stock_left(void * data, int i, void * p);
		int pp2_menu_proc_bouncer_stock_right(void * data, int i, void * p);
		int pp2_menu_proc_seeker_stock_left(void * data, int i, void * p);
		int pp2_menu_proc_seeker_stock_right(void * data, int i, void * p);
		int pp2_menu_proc_reflector_stock_left(void * data, int i, void * p);
		int pp2_menu_proc_reflector_stock_right(void * data, int i, void * p);
		int pp2_menu_proc_pmine_stock_left(void * data, int i, void * p);
		int pp2_menu_proc_pmine_stock_right(void * data, int i, void * p);
		int pp2_menu_proc_ghost_stock_left(void * data, int i, void * p);
		int pp2_menu_proc_ghost_stock_right(void * data, int i, void * p);
		int pp2_menu_proc_normal_ammo_left(void * data, int i, void * p);
		int pp2_menu_proc_normal_ammo_right(void * data, int i, void * p);
		int pp2_menu_proc_splitter_ammo_left(void * data, int i, void * p);
		int pp2_menu_proc_splitter_ammo_right(void * data, int i, void * p);
		int pp2_menu_proc_mine_ammo_left(void * data, int i, void * p);
		int pp2_menu_proc_mine_ammo_right(void * data, int i, void * p);
		int pp2_menu_proc_bouncer_ammo_left(void * data, int i, void * p);
		int pp2_menu_proc_bouncer_ammo_right(void * data, int i, void * p);
		int pp2_menu_proc_seeker_ammo_left(void * data, int i, void * p);
		int pp2_menu_proc_seeker_ammo_right(void * data, int i, void * p);
		int pp2_menu_proc_reflector_ammo_left(void * data, int i, void * p);
		int pp2_menu_proc_reflector_ammo_right(void * data, int i, void * p);
		int pp2_menu_proc_pmine_ammo_left(void * data, int i, void * p);
		int pp2_menu_proc_pmine_ammo_right(void * data, int i, void * p);
		int pp2_menu_proc_ghost_ammo_left(void * data, int i, void * p);
		int pp2_menu_proc_ghost_ammo_right(void * data, int i, void * p);
		int pp2_menu_proc_cloak_power_left(void * data, int i, void * p);
		int pp2_menu_proc_cloak_power_right(void * data, int i, void * p);
		int pp2_menu_proc_jump_power_left(void * data, int i, void * p);
		int pp2_menu_proc_jump_power_right(void * data, int i, void * p);
		int pp2_menu_proc_run_power_left(void * data, int i, void * p);
		int pp2_menu_proc_run_power_right(void * data, int i, void * p);
		int pp2_menu_proc_deflect_power_left(void * data, int i, void * p);
		int pp2_menu_proc_deflect_power_right(void * data, int i, void * p);
		int pp2_menu_proc_fly_power_left(void * data, int i, void * p);
		int pp2_menu_proc_fly_power_right(void * data, int i, void * p);
		int pp2_menu_proc_turbo_power_left(void * data, int i, void * p);
		int pp2_menu_proc_turbo_power_right(void * data, int i, void * p);
int pp2_menu_proc_main_play_network(void * data, int i, void * p);
	int pp2_menu_proc_main_play_online(void * data, int i, void * p);
		int pp2_menu_proc_network_id_ok(void * data, int i, void * p);
		int pp2_menu_proc_play_online_host(void * data, int i, void * p);
			int pp2_menu_proc_host_name_ok(void * data, int i, void * p);
		int pp2_menu_proc_play_online_join(void * data, int i, void * p);
			int pp2_menu_proc_play_online_join_connect(void * data, int i, void * p);
		int pp2_menu_proc_server_list_select(void * data, int i, void * p);
	int pp2_menu_proc_main_play_lan(void * data, int i, void * p);
		int pp2_menu_proc_play_lan_host(void * data, int i, void * p);
		int pp2_menu_proc_play_lan_join(void * data, int i, void * p);
			int pp2_menu_proc_host_ip_ok(void * data, int i, void * p);
	int pp2_menu_proc_main_disconnect(void * data, int i, void * p);
	int pp2_menu_proc_main_close_server(void * data, int i, void * p);
int pp2_menu_proc_main_view_replay(void * data, int i, void * p);
int pp2_menu_proc_main_capture_replay(void * data, int i, void * p);
int pp2_menu_proc_main_profiles(void * data, int i, void * p);
int pp2_menu_proc_main_options(void * data, int i, void * p);
	int pp2_menu_proc_options_controllers(void * data, int i, void * p);
		int pp2_menu_proc_options_controller_1(void * data, int i, void * p);
		int pp2_menu_proc_options_controller_2(void * data, int i, void * p);
		int pp2_menu_proc_options_controller_3(void * data, int i, void * p);
		int pp2_menu_proc_options_controller_4(void * data, int i, void * p);
		int pp2_menu_proc_options_controller_reset(void * data, int i, void * p);
			int pp2_menu_proc_controller_up(void * data, int i, void * p);
			int pp2_menu_proc_controller_down(void * data, int i, void * p);
			int pp2_menu_proc_controller_left(void * data, int i, void * p);
			int pp2_menu_proc_controller_right(void * data, int i, void * p);
			int pp2_menu_proc_controller_jump(void * data, int i, void * p);
			int pp2_menu_proc_controller_fire(void * data, int i, void * p);
			int pp2_menu_proc_controller_select(void * data, int i, void * p);
			int pp2_menu_proc_controller_strafe(void * data, int i, void * p);
			int pp2_menu_proc_controller_show_scores(void * data, int i, void * p);
	int pp2_menu_proc_options_audio(void * data, int i, void * p);
		int pp2_menu_proc_music_down(void * data, int i, void * p);
		int pp2_menu_proc_music_up(void * data, int i, void * p);
		int pp2_menu_proc_sound_down(void * data, int i, void * p);
		int pp2_menu_proc_sound_up(void * data, int i, void * p);
	int pp2_menu_proc_options_video(void * data, int i, void * p);
		int pp2_menu_proc_display_toggle(void * data, int i, void * p);
		int pp2_menu_proc_resolution_left(void * data, int i, void * p);
		int pp2_menu_proc_resolution_right(void * data, int i, void * p);
	int pp2_menu_proc_options_network(void * data, int i, void * p);
		int pp2_menu_proc_network_ok(void * data, int i, void * p);
int pp2_menu_proc_main_quit(void * data, int i, void * p);

/* in-game menus */
int pp2_menu_proc_game_resume(void * data, int i, void * p);
int pp2_menu_proc_game_end(void * data, int i, void * p);
int pp2_menu_proc_game_rematch(void * data, int i, void * p);
int pp2_menu_proc_game_play_again(void * data, int i, void * p);
int pp2_menu_proc_game_new_game(void * data, int i, void * p);
int pp2_menu_proc_game_save_replay(void * data, int i, void * p);
int pp2_menu_proc_game_quit(void * data, int i, void * p);
int pp2_menu_proc_new_profile_ok(void * data, int i, void * p);

/* overlay menus */
int pp2_menu_proc_overlay_back(void * data, int i, void * p);
int pp2_menu_proc_overlay_next(void * data, int i, void * p);

#endif
