#ifndef PP2_INTERFACE_H
#define PP2_INTERFACE_H

#include "../file/profile.h"

typedef struct
{

	char menu_text[32][64];
	float sound_volume;
	bool joystick_menu_activation;
	PP2_PROFILE_LIST profiles;
	int selected_profile;

} PP2_INTERFACE;

#endif
