#include <allegro5/allegro5.h>
#include <stdio.h>
#include "../t3f/music.h"
#include "../gameplay/game.h"
#include "../gameplay/game_defines.h"

void pp2_set_default_config(PP2_INTERFACE * ip, PP2_GAME * gp)
{
	gp->option[PP2_OPTION_GAME_MODE] = PP2_GAME_MODE_ELIMINATOR;
	gp->option[PP2_OPTION_ELIMINATION_HITS] = 1;
	gp->option[PP2_OPTION_DEATH_MATCH_FRAGS] = 20;
	gp->option[PP2_OPTION_LIFE] = 1;
	gp->option[PP2_OPTION_TIME_LIMIT] = 2;
	gp->option[PP2_OPTION_RANDOMIZE_ITEMS] = 0;
	gp->option[PP2_OPTION_ENABLE_AMMO_NORMAL] = 1;
	gp->option[PP2_OPTION_ENABLE_AMMO_X] = 1;
	gp->option[PP2_OPTION_ENABLE_AMMO_MINE] = 1;
	gp->option[PP2_OPTION_ENABLE_AMMO_BOUNCE] = 1;
	gp->option[PP2_OPTION_ENABLE_AMMO_SEEK] = 1;
	gp->option[PP2_OPTION_ENABLE_AMMO_REFLECTOR] = 1;
	gp->option[PP2_OPTION_ENABLE_AMMO_PMINE] = 1;
	gp->option[PP2_OPTION_ENABLE_AMMO_GHOST] = 1;
	gp->option[PP2_OPTION_ENABLE_POWERUP_CLOAK] = 1;
	gp->option[PP2_OPTION_ENABLE_POWERUP_JUMP] = 1;
	gp->option[PP2_OPTION_ENABLE_POWERUP_RUN] = 1;
	gp->option[PP2_OPTION_ENABLE_POWERUP_DEFLECT] = 1;
	gp->option[PP2_OPTION_ENABLE_POWERUP_FLY] = 1;
	gp->option[PP2_OPTION_ENABLE_POWERUP_TURBO] = 1;
	gp->option[PP2_OPTION_START_AMMO_NORMAL] = 1;
	gp->option[PP2_OPTION_START_AMMO_X] = 1;
	gp->option[PP2_OPTION_START_AMMO_MINE] = 1;
	gp->option[PP2_OPTION_START_AMMO_BOUNCE] = 1;
	gp->option[PP2_OPTION_START_AMMO_SEEK] = 1;
	gp->option[PP2_OPTION_START_AMMO_REFLECTOR] = 1;
	gp->option[PP2_OPTION_START_AMMO_PMINE] = 1;
	gp->option[PP2_OPTION_START_AMMO_GHOST] = 1;
	gp->option[PP2_OPTION_AMMO_WORTH] = 1;
	gp->option[PP2_OPTION_TRAILS] = 1;
	ip->setting[PP2_SETTING_NETWORK_MAX_FRAMES] = 3;
}

bool pp2_load_config(PP2_INTERFACE * ip, PP2_GAME * gp, const char * fn)
{
	int i;
	char temp_string2[1024] = {0};
	const char * item = NULL;

	pp2_set_default_config(ip, gp);
	ip->config = al_load_config_file(fn);

	/* set default configuration */
	if(!ip->config)
	{
		ip->config = al_create_config();
		al_add_config_section(ip->config, "Controller 0");
		al_add_config_section(ip->config, "Controller 1");
		al_add_config_section(ip->config, "Controller 2");
		al_add_config_section(ip->config, "Controller 3");
		ip->regenerate_cache = true;
		return false;
	}

	/* load config values */
	else
	{
		item = al_get_config_value(ip->config, "Network Settings", "ID");
		if(item)
		{
			strcpy(ip->network_id, item);
		}
		else
		{
			strcpy(ip->network_id, "");
		}
		item = al_get_config_value(ip->config, "Network Settings", "Server Name");
		if(item)
		{
			strcpy(ip->server_name, item);
		}
		else
		{
			strcpy(ip->server_name, "");
		}
		item = al_get_config_value(ip->config, "Network Settings", "Max Frame Delay");
		if(item)
		{
			ip->setting[PP2_SETTING_NETWORK_MAX_FRAMES] = atoi(item);
		}
		item = al_get_config_value(ip->config, "Audio", "Music Volume");
		if(item)
		{
			t3f_set_music_volume(atof(item));
		}
		item = al_get_config_value(ip->config, "Audio", "Sound Volume");
		if(item)
		{
			ip->sound_volume = atof(item);
		}
		item = al_get_config_value(ip->config, "Game Options", "Level");
		if(item)
		{
			ip->level_hash = atol(item);
		}
		item = al_get_config_value(ip->config, "Game Options", "Mode");
		if(item)
		{
			gp->option[PP2_OPTION_GAME_MODE] = atoi(item);
		}
		item = al_get_config_value(ip->config, "Game Options", "Elimination Hits");
		if(item)
		{
			gp->option[PP2_OPTION_ELIMINATION_HITS] = atoi(item);
		}
		item = al_get_config_value(ip->config, "Game Options", "Death Match Frags");
		if(item)
		{
			gp->option[PP2_OPTION_DEATH_MATCH_FRAGS] = atoi(item);
		}
		item = al_get_config_value(ip->config, "Game Options", "Life");
		if(item)
		{
			gp->option[PP2_OPTION_LIFE] = atoi(item);
		}
		item = al_get_config_value(ip->config, "Game Options", "CTF Flags");
		if(item)
		{
			gp->option[PP2_OPTION_CTF_FLAGS] = atoi(item);
		}
		item = al_get_config_value(ip->config, "Game Options", "Time Limit");
		if(item)
		{
			gp->option[PP2_OPTION_TIME_LIMIT] = atoi(item);
		}
		item = al_get_config_value(ip->config, "Game Options", "Random Items");
		if(item)
		{
			gp->option[PP2_OPTION_RANDOMIZE_ITEMS] = !strcmp(item, "on") ? 1 : 0;
		}
		item = al_get_config_value(ip->config, "Game Options", "Ammo Normal");
		if(item)
		{
			gp->option[PP2_OPTION_ENABLE_AMMO_NORMAL] = !strcmp(item, "on") ? 1 : 0;
		}
		item = al_get_config_value(ip->config, "Game Options", "Ammo Splitter");
		if(item)
		{
			gp->option[PP2_OPTION_ENABLE_AMMO_X] = !strcmp(item, "on") ? 1 : 0;
		}
		item = al_get_config_value(ip->config, "Game Options", "Ammo Mine");
		if(item)
		{
			gp->option[PP2_OPTION_ENABLE_AMMO_MINE] = !strcmp(item, "on") ? 1 : 0;
		}
		item = al_get_config_value(ip->config, "Game Options", "Ammo Bouncer");
		if(item)
		{
			gp->option[PP2_OPTION_ENABLE_AMMO_BOUNCE] = !strcmp(item, "on") ? 1 : 0;
		}
		item = al_get_config_value(ip->config, "Game Options", "Ammo Seeker");
		if(item)
		{
			gp->option[PP2_OPTION_ENABLE_AMMO_SEEK] = !strcmp(item, "on") ? 1 : 0;
		}
		item = al_get_config_value(ip->config, "Game Options", "Ammo Reflector");
		if(item)
		{
			gp->option[PP2_OPTION_ENABLE_AMMO_REFLECTOR] = !strcmp(item, "on") ? 1 : 0;
		}
		item = al_get_config_value(ip->config, "Game Options", "Ammo Proximity Mine");
		if(item)
		{
			gp->option[PP2_OPTION_ENABLE_AMMO_PMINE] = !strcmp(item, "on") ? 1 : 0;
		}
		item = al_get_config_value(ip->config, "Game Options", "Ammo Ghost");
		if(item)
		{
			gp->option[PP2_OPTION_ENABLE_AMMO_GHOST] = !strcmp(item, "on") ? 1 : 0;
		}
		item = al_get_config_value(ip->config, "Game Options", "Power-Up Cloak");
		if(item)
		{
			gp->option[PP2_OPTION_ENABLE_POWERUP_CLOAK] = !strcmp(item, "on") ? 1 : 0;
		}
		item = al_get_config_value(ip->config, "Game Options", "Power-Up Jump");
		if(item)
		{
			gp->option[PP2_OPTION_ENABLE_POWERUP_JUMP] = !strcmp(item, "on") ? 1 : 0;
		}
		item = al_get_config_value(ip->config, "Game Options", "Power-Up Run");
		if(item)
		{
			gp->option[PP2_OPTION_ENABLE_POWERUP_RUN] = !strcmp(item, "on") ? 1 : 0;
		}
		item = al_get_config_value(ip->config, "Game Options", "Power-Up Deflect");
		if(item)
		{
			gp->option[PP2_OPTION_ENABLE_POWERUP_DEFLECT] = !strcmp(item, "on") ? 1 : 0;
		}
		item = al_get_config_value(ip->config, "Game Options", "Power-Up Fly");
		if(item)
		{
			gp->option[PP2_OPTION_ENABLE_POWERUP_FLY] = !strcmp(item, "on") ? 1 : 0;
		}
		item = al_get_config_value(ip->config, "Game Options", "Power-Up Turbo");
		if(item)
		{
			gp->option[PP2_OPTION_ENABLE_POWERUP_TURBO] = !strcmp(item, "on") ? 1 : 0;
		}
		item = al_get_config_value(ip->config, "Game Options", "Stock Normal");
		if(item)
		{
			gp->option[PP2_OPTION_START_AMMO_NORMAL] = atoi(item);
		}
		item = al_get_config_value(ip->config, "Game Options", "Stock Splitter");
		if(item)
		{
			gp->option[PP2_OPTION_START_AMMO_X] = atoi(item);
		}
		item = al_get_config_value(ip->config, "Game Options", "Stock Mine");
		if(item)
		{
			gp->option[PP2_OPTION_START_AMMO_MINE] = atoi(item);
		}
		item = al_get_config_value(ip->config, "Game Options", "Stock Bouncer");
		if(item)
		{
			gp->option[PP2_OPTION_START_AMMO_BOUNCE] = atoi(item);
		}
		item = al_get_config_value(ip->config, "Game Options", "Stock Seeker");
		if(item)
		{
			gp->option[PP2_OPTION_START_AMMO_SEEK] = atoi(item);
		}
		item = al_get_config_value(ip->config, "Game Options", "Stock Reflector");
		if(item)
		{
			gp->option[PP2_OPTION_START_AMMO_REFLECTOR] = atoi(item);
		}
		item = al_get_config_value(ip->config, "Game Options", "Stock Proximity Mine");
		if(item)
		{
			gp->option[PP2_OPTION_START_AMMO_PMINE] = atoi(item);
		}
		item = al_get_config_value(ip->config, "Game Options", "Stock Ghost");
		if(item)
		{
			gp->option[PP2_OPTION_START_AMMO_GHOST] = atoi(item);
		}
		item = al_get_config_value(ip->config, "Game Options", "Ammo Worth");
		if(item)
		{
			gp->option[PP2_OPTION_AMMO_WORTH] = atoi(item);
		}
		item = al_get_config_value(ip->config, "Graphics Options", "Trails");
		if(item)
		{
			if(!strcasecmp(item, "on"))
			{
				gp->option[PP2_OPTION_TRAILS] = 1;
			}
			else
			{
				gp->option[PP2_OPTION_TRAILS] = 0;
			}
		}
		for(i = 0; i < 4; i++)
		{
			sprintf(temp_string2, "Controller %d", i);
//			t3f_read_controller_config(ip->config, temp_string2, ip->controller[i]);
		}
		item = al_get_config_value(ip->config, "System", "Cache Version");
		if(item)
		{
			if(atoi(item) != PP2_CACHE_VERSION)
			{
				ip->regenerate_cache = true;
			}
		}
		else
		{
			ip->regenerate_cache = true;
		}
	}

	return true;
}

bool pp2_save_config(PP2_INTERFACE * ip, PP2_GAME * gp, const char * fn)
{
	char buf[256] = {0};
	int i;

	sprintf(buf, "%1.2f", t3f_get_music_volume());
	al_set_config_value(ip->config, "Audio", "Music Volume", buf);
	sprintf(buf, "%1.2f", ip->sound_volume);
	al_set_config_value(ip->config, "Audio", "Sound Volume", buf);
	sprintf(buf, "%d", ip->setting[PP2_SETTING_NETWORK_MAX_FRAMES]);
	al_set_config_value(ip->config, "Network Settings", "Max Frame Delay", buf);

	/* game options */
	sprintf(buf, "%lu", ip->level_hash);
	al_set_config_value(ip->config, "Game Options", "Level", buf);
	sprintf(buf, "%d", gp->option[PP2_OPTION_GAME_MODE]);
	al_set_config_value(ip->config, "Game Options", "Mode", buf);
	sprintf(buf, "%d", gp->option[PP2_OPTION_ELIMINATION_HITS]);
	al_set_config_value(ip->config, "Game Options", "Elimination Hits", buf);
	sprintf(buf, "%d", gp->option[PP2_OPTION_DEATH_MATCH_FRAGS]);
	al_set_config_value(ip->config, "Game Options", "Death Match Frags", buf);
	sprintf(buf, "%d", gp->option[PP2_OPTION_LIFE]);
	al_set_config_value(ip->config, "Game Options", "Life", buf);
	sprintf(buf, "%d", gp->option[PP2_OPTION_CTF_FLAGS]);
	al_set_config_value(ip->config, "Game Options", "CTF Flags", buf);
	sprintf(buf, "%d", gp->option[PP2_OPTION_TIME_LIMIT]);
	al_set_config_value(ip->config, "Game Options", "Time Limit", buf);
	sprintf(buf, "%s", gp->option[PP2_OPTION_RANDOMIZE_ITEMS] ? "on" : "off");
	al_set_config_value(ip->config, "Game Options", "Random Items", buf);
	sprintf(buf, "%s", gp->option[PP2_OPTION_ENABLE_AMMO_NORMAL] ? "on" : "off");
	al_set_config_value(ip->config, "Game Options", "Ammo Normal", buf);
	sprintf(buf, "%s", gp->option[PP2_OPTION_ENABLE_AMMO_X] ? "on" : "off");
	al_set_config_value(ip->config, "Game Options", "Ammo Splitter", buf);
	sprintf(buf, "%s", gp->option[PP2_OPTION_ENABLE_AMMO_MINE] ? "on" : "off");
	al_set_config_value(ip->config, "Game Options", "Ammo Mine", buf);
	sprintf(buf, "%s", gp->option[PP2_OPTION_ENABLE_AMMO_BOUNCE] ? "on" : "off");
	al_set_config_value(ip->config, "Game Options", "Ammo Bouncer", buf);
	sprintf(buf, "%s", gp->option[PP2_OPTION_ENABLE_AMMO_SEEK] ? "on" : "off");
	al_set_config_value(ip->config, "Game Options", "Ammo Seeker", buf);
	sprintf(buf, "%s", gp->option[PP2_OPTION_ENABLE_AMMO_REFLECTOR] ? "on" : "off");
	al_set_config_value(ip->config, "Game Options", "Ammo Reflector", buf);
	sprintf(buf, "%s", gp->option[PP2_OPTION_ENABLE_AMMO_PMINE] ? "on" : "off");
	al_set_config_value(ip->config, "Game Options", "Ammo Proximity Mine", buf);
	sprintf(buf, "%s", gp->option[PP2_OPTION_ENABLE_AMMO_GHOST] ? "on" : "off");
	al_set_config_value(ip->config, "Game Options", "Ammo Ghost", buf);
	sprintf(buf, "%d", gp->option[PP2_OPTION_START_AMMO_NORMAL]);
	al_set_config_value(ip->config, "Game Options", "Stock Normal", buf);
	sprintf(buf, "%d", gp->option[PP2_OPTION_START_AMMO_X]);
	al_set_config_value(ip->config, "Game Options", "Stock Splitter", buf);
	sprintf(buf, "%d", gp->option[PP2_OPTION_START_AMMO_MINE]);
	al_set_config_value(ip->config, "Game Options", "Stock Mine", buf);
	sprintf(buf, "%d", gp->option[PP2_OPTION_START_AMMO_BOUNCE]);
	al_set_config_value(ip->config, "Game Options", "Stock Bouncer", buf);
	sprintf(buf, "%d", gp->option[PP2_OPTION_START_AMMO_SEEK]);
	al_set_config_value(ip->config, "Game Options", "Stock Seeker", buf);
	sprintf(buf, "%d", gp->option[PP2_OPTION_START_AMMO_REFLECTOR]);
	al_set_config_value(ip->config, "Game Options", "Stock Reflector", buf);
	sprintf(buf, "%d", gp->option[PP2_OPTION_START_AMMO_PMINE]);
	al_set_config_value(ip->config, "Game Options", "Stock Proximity Mine", buf);
	sprintf(buf, "%d", gp->option[PP2_OPTION_START_AMMO_GHOST]);
	al_set_config_value(ip->config, "Game Options", "Stock Ghost", buf);
	sprintf(buf, "%s", gp->option[PP2_OPTION_ENABLE_POWERUP_CLOAK] ? "on" : "off");
	al_set_config_value(ip->config, "Game Options", "Power-Up Cloak", buf);
	sprintf(buf, "%s", gp->option[PP2_OPTION_ENABLE_POWERUP_JUMP] ? "on" : "off");
	al_set_config_value(ip->config, "Game Options", "Power-Up Jump", buf);
	sprintf(buf, "%s", gp->option[PP2_OPTION_ENABLE_POWERUP_RUN] ? "on" : "off");
	al_set_config_value(ip->config, "Game Options", "Power-Up Run", buf);
	sprintf(buf, "%s", gp->option[PP2_OPTION_ENABLE_POWERUP_DEFLECT] ? "on" : "off");
	al_set_config_value(ip->config, "Game Options", "Power-Up Deflect", buf);
	sprintf(buf, "%s", gp->option[PP2_OPTION_ENABLE_POWERUP_FLY] ? "on" : "off");
	al_set_config_value(ip->config, "Game Options", "Power-Up Fly", buf);
	sprintf(buf, "%s", gp->option[PP2_OPTION_ENABLE_POWERUP_TURBO] ? "on" : "off");
	al_set_config_value(ip->config, "Game Options", "Power-Up Turbo", buf);
	sprintf(buf, "%d", gp->option[PP2_OPTION_AMMO_WORTH]);
	al_set_config_value(ip->config, "Game Options", "Ammo Worth", buf);
	sprintf(buf, "%s", gp->option[PP2_OPTION_TRAILS] ? "on" : "off");
	al_set_config_value(ip->config, "Graphics Options", "Trails", buf);
	for(i = 0; i < 4; i++)
	{
		sprintf(buf, "Controller %d", i);
//		t3f_write_controller_config(ip->config, buf, ip->controller[i]);
	}
	sprintf(buf, "%d", PP2_CACHE_VERSION);
	al_set_config_value(ip->config, "System", "Cache Version", buf);
	return al_save_config_file(fn, ip->config);
}
