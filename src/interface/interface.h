#ifndef PP2_INTERFACE_H
#define PP2_INTERFACE_H

#include "../file/profile.h"
#include "preview.h"

typedef struct
{

	char menu_text[32][64];
	float sound_volume;
	bool joystick_menu_activation;
	PP2_PROFILE_LIST profiles;
	int selected_profile;
	PP2_CHARACTER_PREVIEW * player_preview[PP2_MAX_PLAYERS];
	PP2_LEVEL_PREVIEW * level_preview;

} PP2_INTERFACE;

#endif
