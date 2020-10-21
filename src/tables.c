#include "t3f/t3f.h"
#include "file/character.h"

int pp2_default_keys[4][9] = 
{
	{	ALLEGRO_KEY_UP, ALLEGRO_KEY_DOWN, ALLEGRO_KEY_LEFT, ALLEGRO_KEY_RIGHT, ALLEGRO_KEY_FULLSTOP, ALLEGRO_KEY_SLASH, ALLEGRO_KEY_QUOTE, ALLEGRO_KEY_SPACE, 0},
	{	ALLEGRO_KEY_W,     ALLEGRO_KEY_S,    ALLEGRO_KEY_A,     ALLEGRO_KEY_D,        ALLEGRO_KEY_Q,     ALLEGRO_KEY_1,     ALLEGRO_KEY_3,     ALLEGRO_KEY_2, 0},
	{	ALLEGRO_KEY_T,     ALLEGRO_KEY_G,    ALLEGRO_KEY_F,     ALLEGRO_KEY_H,        ALLEGRO_KEY_R,     ALLEGRO_KEY_4,     ALLEGRO_KEY_6,     ALLEGRO_KEY_5, 0},
	{	ALLEGRO_KEY_I,     ALLEGRO_KEY_K,    ALLEGRO_KEY_J,     ALLEGRO_KEY_L,        ALLEGRO_KEY_U,     ALLEGRO_KEY_7,     ALLEGRO_KEY_9,     ALLEGRO_KEY_8, 0}
};

char pp2_controller_binding_text[11][128] =
{
	"Controller 1",
	"Up",
	"Down",
	"Left",
	"Right",
	"Jump",
	"Fire",
	"Select",
	"Strafe",
	"Show Scores"
};

char * pp2_game_mode_text[16] = {"Elimination", "Death Match", "Coin Rush"};
char * pp2_ammo_type_text[16] = {"A", "X", "M", "B", "S", "R", "P", "G"};
char * pp2_button_name[9] = {"Up", "Down", "Left", "Right", "Jump", "Fire", "Select", "Strafe", "Show Scores"};
