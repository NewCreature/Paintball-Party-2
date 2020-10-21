#ifndef PP2_PROFILE_H
#define PP2_PROFILE_H

#include <allegro5/allegro5.h>

#define PP2_MAX_PROFILES 256

typedef struct
{

	/* info */
	char name[128];
	unsigned long character;

	/* stats */
	unsigned long wins;
	unsigned long losses;
	unsigned long plays;
	unsigned long shots;
	unsigned long hits;
	unsigned long shot;

} PP2_PROFILE;

typedef struct
{

	PP2_PROFILE item[PP2_MAX_PROFILES];
	int items;

} PP2_PROFILE_LIST;

bool pp2_load_profiles(PP2_PROFILE_LIST * lp, const char * fn);
bool pp2_save_profiles(PP2_PROFILE_LIST * lp, const char * fn);
void pp2_clear_profiles(PP2_PROFILE_LIST * lp);
bool pp2_add_profile(PP2_PROFILE_LIST * lp, const char * name);

#endif
