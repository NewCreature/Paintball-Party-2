#include <allegro5/allegro5.h>
#include "profile.h"

bool pp2_load_profiles(PP2_PROFILE_LIST * lp, const char * fn)
{
	ALLEGRO_FILE * fp;
	int i;
	
	fp = al_fopen(fn, "rb");
	if(fp)
	{
		pp2_clear_profiles(lp);
		lp->items = al_fread32le(fp);
		for(i = 0; i < lp->items; i++)
		{
			al_fread(fp, lp->item[i].name, 128);
			lp->item[i].character = al_fread32le(fp);
			lp->item[i].wins = al_fread32le(fp);
			lp->item[i].losses = al_fread32le(fp);
			lp->item[i].plays = al_fread32le(fp);
			lp->item[i].shots = al_fread32le(fp);
			lp->item[i].hits = al_fread32le(fp);
			lp->item[i].shot = al_fread32le(fp);
		}
		al_fclose(fp);
		strcpy(lp->item[lp->items].name, "New Profile");
		return true;
	}
	return false;
}

bool pp2_save_profiles(PP2_PROFILE_LIST * lp, const char * fn)
{
	ALLEGRO_FILE * fp;
	int i;
	
	fp = al_fopen(fn, "wb");
	if(fp)
	{
		al_fwrite32le(fp, lp->items);
		for(i = 0; i < lp->items; i++)
		{
			al_fwrite(fp, lp->item[i].name, 128);
			al_fwrite32le(fp, lp->item[i].character);
			al_fwrite32le(fp, lp->item[i].wins);
			al_fwrite32le(fp, lp->item[i].losses);
			al_fwrite32le(fp, lp->item[i].plays);
			al_fwrite32le(fp, lp->item[i].shots);
			al_fwrite32le(fp, lp->item[i].hits);
			al_fwrite32le(fp, lp->item[i].shot);
		}
		al_fclose(fp);
		return true;
	}
	return false;
}

void pp2_clear_profiles(PP2_PROFILE_LIST * lp)
{
	lp->items = 0;
}

bool pp2_add_profile(PP2_PROFILE_LIST * lp, char * name)
{
	if(lp->items < PP2_MAX_PROFILES - 1)
	{
		memset(&lp->item[lp->items], 0, sizeof(PP2_PROFILE));
		strcpy(lp->item[lp->items].name, name);
		lp->items++;
		strcpy(lp->item[lp->items].name, "New Profile");
		return true;
	}
	return false;
}
