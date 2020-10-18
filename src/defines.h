#ifndef PP2_DEFINES_H
#define PP2_DEFINES_H

#define PP2_SCREEN_WIDTH     960
#define PP2_SCREEN_HEIGHT    540
#define PP2_SCREEN_VISIBLE_WIDTH (t3f_default_view->right - t3f_default_view->left)
#define PP2_SCREEN_VISIBLE_HEIGHT (t3f_default_view->bottom - t3f_default_view->top)

#define PP2_CACHE_VERSION      2
#define PP2_REPLAY_VERSION     1

#define PP2_MAX_PLAYERS       16
#define PP2_MAX_PAINTBALLS   128
#define PP2_MAX_TRAILS       128
#define PP2_MAX_PLATFORMS     64
#define PP2_MAX_PARTICLES    256
#define PP2_MAX_RADAR_OBJECTS 64

#define PP2_MAX_OBJECT_TYPES 256

#define PP2_MAX_OPTIONS     64

#define PP2_OPTION_GAME_MODE               0
#define PP2_OPTION_ELIMINATION_HITS        1
#define PP2_OPTION_CTF_FLAGS               2
#define PP2_OPTION_TIME_LIMIT              3
#define PP2_OPTION_ENABLE_AMMO_NORMAL      4
#define PP2_OPTION_ENABLE_AMMO_X           5
#define PP2_OPTION_ENABLE_AMMO_MINE        6
#define PP2_OPTION_ENABLE_AMMO_BOUNCE      7
#define PP2_OPTION_ENABLE_AMMO_SEEK        8
#define PP2_OPTION_ENABLE_AMMO_REFLECTOR   9
#define PP2_OPTION_ENABLE_AMMO_PMINE      10
#define PP2_OPTION_ENABLE_AMMO_GHOST      11
#define PP2_OPTION_ENABLE_POWERUP_CLOAK   12
#define PP2_OPTION_ENABLE_POWERUP_JUMP    13
#define PP2_OPTION_ENABLE_POWERUP_RUN     14
#define PP2_OPTION_ENABLE_POWERUP_DEFLECT 15
#define PP2_OPTION_ENABLE_POWERUP_FLY     16
#define PP2_OPTION_ENABLE_POWERUP_TURBO   17
#define PP2_OPTION_START_AMMO_NORMAL      18
#define PP2_OPTION_START_AMMO_X           19
#define PP2_OPTION_START_AMMO_MINE        20
#define PP2_OPTION_START_AMMO_BOUNCE      21
#define PP2_OPTION_START_AMMO_SEEK        22
#define PP2_OPTION_START_AMMO_REFLECTOR   23
#define PP2_OPTION_START_AMMO_PMINE       24
#define PP2_OPTION_START_AMMO_GHOST       25
#define PP2_OPTION_DEATH_MATCH_FRAGS      26
#define PP2_OPTION_LIFE                   27
#define PP2_OPTION_RANDOMIZE_ITEMS        28
#define PP2_OPTION_STOMP_HITS             29
#define PP2_OPTION_AMMO_WORTH             30
#define PP2_OPTION_TRAILS                 63

#define PP2_MAX_SETTINGS                  64
#define PP2_SETTING_NETWORK_MAX_FRAMES     3

#define PP2_STATE_INTRO         0
#define PP2_STATE_TITLE         1
#define PP2_STATE_MENU          2
#define PP2_STATE_PLAYER_SETUP  3
#define PP2_STATE_LEVEL_SETUP   4
#define PP2_STATE_GAME          5
#define PP2_STATE_GAME_PAUSED   6
#define PP2_STATE_GAME_OVER     7
#define PP2_STATE_REPLAY        8
#define PP2_STATE_T_TITLE_MENU  9
#define PP2_STATE_THEATER      10

#define PP2_END_GAME_QUIT       0
#define PP2_END_GAME_REMATCH    1
#define PP2_END_GAME_PLAY_AGAIN 2
#define PP2_END_GAME_NEW        3

#define PP2_CONTROLLER_UP        0
#define PP2_CONTROLLER_DOWN      1
#define PP2_CONTROLLER_LEFT      2
#define PP2_CONTROLLER_RIGHT     3
#define PP2_CONTROLLER_JUMP      4
#define PP2_CONTROLLER_FIRE      5
#define PP2_CONTROLLER_SELECT    6
#define PP2_CONTROLLER_STRAFE    7
#define PP2_CONTROLLER_SCORES    8

#define PP2_MAX_BITMAPS         128
#define PP2_BITMAP_T3_LOGO        0
#define PP2_BITMAP_TITLE_SPLAT    1
#define PP2_BITMAP_HUD            2
#define PP2_BITMAP_MENU_BG        3
#define PP2_BITMAP_T3_LOGO_MEMORY 4
#define PP2_BITMAP_MENU_LOGO      5
#define PP2_BITMAP_TITLE_LOGO     6
#define PP2_BITMAP_TARGET         7
#define PP2_BITMAP_EMPTY_PLAYER  16
#define PP2_BITMAP_RADAR_BLIP    18
#define PP2_BITMAP_TYPING        19
#define PP2_BITMAP_HIGHLIGHT     20
#define PP2_BITMAP_ICON         125
#define PP2_BITMAP_SCREEN_COPY  126
#define PP2_BITMAP_LOADING      127

#define PP2_MAX_ANIMATIONS              32
#define PP2_ANIMATION_HUD_AMMO_NORMAL    0
#define PP2_ANIMATION_HUD_AMMO_SPLITTER  1
#define PP2_ANIMATION_HUD_AMMO_MINE      2
#define PP2_ANIMATION_HUD_AMMO_BOUNCER   3
#define PP2_ANIMATION_HUD_AMMO_SEEKER    4
#define PP2_ANIMATION_HUD_AMMO_REFLECTOR 5
#define PP2_ANIMATION_HUD_AMMO_PMINE     6
#define PP2_ANIMATION_HUD_AMMO_GHOST     7

#define PP2_MAX_FONTS            16
#define PP2_FONT_SMALL            0
#define PP2_FONT_COMIC_16         1
#define PP2_FONT_HUD              2
#define PP2_FONT_COMIC_10         3
#define PP2_FONT_COMIC_12         4
#define PP2_FONT_COMIC_14         5

#define PP2_MAX_SAMPLES        64
#define PP2_SAMPLE_FIRE         0
#define PP2_SAMPLE_SPLAT        1
#define PP2_SAMPLE_RELOAD_A     2
#define PP2_SAMPLE_RELOAD_B     3
#define PP2_SAMPLE_AMMO         4
#define PP2_SAMPLE_CLOAK        5
#define PP2_SAMPLE_GEM          6
#define PP2_SAMPLE_JUMP         7
#define PP2_SAMPLE_LAND         8
#define PP2_SAMPLE_HIT          9
#define PP2_SAMPLE_BUMP        10
#define PP2_SAMPLE_TIME        11
#define PP2_SAMPLE_MENU_PICK   12
#define PP2_SAMPLE_MENU_NEXT   13
#define PP2_SAMPLE_TELE_IN     14
#define PP2_SAMPLE_TELE_OUT    15
#define PP2_SAMPLE_RUN         16
#define PP2_SAMPLE_DEFLECT     17
#define PP2_SAMPLE_FLAG        18
#define PP2_SAMPLE_BANK        19
#define PP2_SAMPLE_BASE        20
#define PP2_SAMPLE_RESPAWN     21
#define PP2_SAMPLE_PJUMP       22
#define PP2_SAMPLE_MENU_LEFT   23
#define PP2_SAMPLE_MENU_RIGHT  24
#define PP2_SAMPLE_MENU_FLAG   25
#define PP2_SAMPLE_MENU_ENTER  26
#define PP2_SAMPLE_MENU_ESC    27
#define PP2_SAMPLE_MENU_MOVE   28
#define PP2_SAMPLE_CRUSH       29
#define PP2_SAMPLE_TARGET      30
#define PP2_SAMPLE_BOUNCE      31
#define PP2_SAMPLE_RICOCHET    32
#define PP2_SAMPLE_START       33
#define PP2_SAMPLE_WIN         34
#define PP2_SAMPLE_ENTER       35
#define PP2_SAMPLE_FLY         36
#define PP2_SAMPLE_PFLY        37
#define PP2_SAMPLE_SPRING      38
#define PP2_SAMPLE_PTURBO      39
#define PP2_SAMPLE_COIN_LAND   40
#define PP2_SAMPLE_COIN_PICKUP 41
#define PP2_SAMPLE_LOGO_TICK   62
#define PP2_SAMPLE_LOGO        63

#define PP2_MAX_MENUS                          64
#define PP2_MAX_MENU_STACK                     16

/* menu hierarchy of menus */
#define PP2_MENU_MAIN                           0
	#define PP2_MENU_PLAY                       1
		#define PP2_MENU_PLAY_QUICK_PLAY        2
		#define PP2_MENU_PLAY_CUSTOM            3
			#define PP2_MENU_PLAY_STOCK         4
			#define PP2_MENU_PLAY_AMMO          5
			#define PP2_MENU_PLAY_POWERUPS      6
			#define PP2_MENU_PLAY_SETTINGS      7
		#define PP2_MENU_PLAY_SINGLE            8
	#define PP2_MENU_PLAY_NETWORK               9
		#define PP2_MENU_PLAY_ONLINE           10
			#define PP2_MENU_PLAY_ONLINE_JOIN  11
			#define PP2_MENU_PLAY_ONLINE_LIST  12
		#define PP2_MENU_PLAY_LAN              13
			#define PP2_MENU_PLAY_LAN_JOIN     14
				#define PP2_MENU_HOST_IP       15
			#define PP2_MENU_PLAY_LAN_HOST     16
#define PP2_MENU_OPTIONS                       17
	#define PP2_MENU_CONTROLLERS               18
		#define PP2_MENU_CONTROLLER            19
	#define PP2_MENU_AUDIO                     20
	#define PP2_MENU_VIDEO                     21
	#define PP2_MENU_NETWORK                   22
		#define PP2_MENU_NETWORK_ID            23
	#define PP2_MENU_PROFILES                  24

#define PP2_MENU_MAIN_HOST                     25 // when hosting a game, show this menu
#define PP2_MENU_HOST_NAME                     26
#define PP2_MENU_MAIN_CLIENT                   27 // when playing online as a client, show this menu

#define PP2_MENU_PAUSE                         28
#define PP2_MENU_GAME_OVER                     29
#define PP2_MENU_PAUSE_CLIENT                  30

#define PP2_MENU_NEW_PROFILE                   31

#define PP2_MENU_OVERLAY                       32
#define PP2_MENU_PLAYER_SETUP_OVERLAY          33
#define PP2_MENU_LEVEL_SETUP_OVERLAY           34
#define PP2_MENU_CLIENT_PLAYER_SETUP_OVERLAY   35

#define PP2_CONTENT_CHARACTERS 0
#define PP2_CONTENT_LEVELS     1

#define PP2_GAME_MESSAGE_TYPING      JOYNET_GAME_MESSAGE_USER
#define PP2_GAME_MESSAGE_TYPING_DONE JOYNET_GAME_MESSAGE_USER + 1

#define PP2_RESOURCE_TYPE_LEGACY_CHARACTER_BITMAP (T3F_RESOURCE_TYPE_USER + 0)

#endif
