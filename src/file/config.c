#include <allegro5/allegro5.h>
#include <stdio.h>
#include "../t3f/music.h"
#include "../data.h"
#include "../tables.h"
#include "../gameplay/game.h"

bool pp2_load_level_config(const char * fn)
{
	pp2_level_config = al_load_config_file(fn);
	if(!pp2_level_config)
	{
		pp2_level_config = al_create_config();
		return false;
	}
	return true;
}

bool pp2_save_level_config(const char * fn)
{
	return al_save_config_file(fn, pp2_level_config);
}

bool pp2_save_character_config(const char * fn)
{
	return al_save_config_file(fn, pp2_character_config);
}

bool pp2_load_character_config(const char * fn)
{
	pp2_character_config = al_load_config_file(fn);
	if(!pp2_character_config)
	{
		pp2_character_config = al_create_config();
		return false;
	}
	return true;
}

void pp2_set_default_config(void)
{
	pp2_option[PP2_OPTION_GAME_MODE] = PP2_GAME_MODE_ELIMINATOR;
	pp2_option[PP2_OPTION_ELIMINATION_HITS] = 1;
	pp2_option[PP2_OPTION_DEATH_MATCH_FRAGS] = 20;
	pp2_option[PP2_OPTION_LIFE] = 1;
	pp2_option[PP2_OPTION_TIME_LIMIT] = 2;
	pp2_option[PP2_OPTION_RANDOMIZE_ITEMS] = 0;
	pp2_option[PP2_OPTION_ENABLE_AMMO_NORMAL] = 1;
	pp2_option[PP2_OPTION_ENABLE_AMMO_X] = 1;
	pp2_option[PP2_OPTION_ENABLE_AMMO_MINE] = 1;
	pp2_option[PP2_OPTION_ENABLE_AMMO_BOUNCE] = 1;
	pp2_option[PP2_OPTION_ENABLE_AMMO_SEEK] = 1;
	pp2_option[PP2_OPTION_ENABLE_AMMO_REFLECTOR] = 1;
	pp2_option[PP2_OPTION_ENABLE_AMMO_PMINE] = 1;
	pp2_option[PP2_OPTION_ENABLE_AMMO_GHOST] = 1;
	pp2_option[PP2_OPTION_ENABLE_POWERUP_CLOAK] = 1;
	pp2_option[PP2_OPTION_ENABLE_POWERUP_JUMP] = 1;
	pp2_option[PP2_OPTION_ENABLE_POWERUP_RUN] = 1;
	pp2_option[PP2_OPTION_ENABLE_POWERUP_DEFLECT] = 1;
	pp2_option[PP2_OPTION_ENABLE_POWERUP_FLY] = 1;
	pp2_option[PP2_OPTION_ENABLE_POWERUP_TURBO] = 1;
	pp2_option[PP2_OPTION_START_AMMO_NORMAL] = 1;
	pp2_option[PP2_OPTION_START_AMMO_X] = 1;
	pp2_option[PP2_OPTION_START_AMMO_MINE] = 1;
	pp2_option[PP2_OPTION_START_AMMO_BOUNCE] = 1;
	pp2_option[PP2_OPTION_START_AMMO_SEEK] = 1;
	pp2_option[PP2_OPTION_START_AMMO_REFLECTOR] = 1;
	pp2_option[PP2_OPTION_START_AMMO_PMINE] = 1;
	pp2_option[PP2_OPTION_START_AMMO_GHOST] = 1;
	pp2_option[PP2_OPTION_AMMO_WORTH] = 1;
	pp2_option[PP2_OPTION_TRAILS] = 1;
}

bool pp2_load_config(const char * fn)
{
	int i, j;
	char temp_string[1024] = {0};
	char temp_string2[1024] = {0};
	const char * item = NULL;
	
	pp2_set_default_config();
	pp2_config = al_load_config_file(fn);
	
	/* set default configuration */
	if(!pp2_config)
	{
		pp2_config = al_create_config();
		al_add_config_section(pp2_config, "Controller 0");
		al_add_config_section(pp2_config, "Controller 1");
		al_add_config_section(pp2_config, "Controller 2");
		al_add_config_section(pp2_config, "Controller 3");
		pp2_regenerate_cache = true;
		return false;
	}
	
	/* load config values */
	else
	{
		item = al_get_config_value(pp2_config, "Network Settings", "ID");
		if(item)
		{
			strcpy(pp2_network_id, item);
		}
		else
		{
			strcpy(pp2_network_id, "");
		}
		item = al_get_config_value(pp2_config, "Network Settings", "Server Name");
		if(item)
		{
			strcpy(pp2_server_name, item);
		}
		else
		{
			strcpy(pp2_server_name, "");
		}
		item = al_get_config_value(pp2_config, "Audio", "Music Volume");
		if(item)
		{
			t3f_set_music_volume(atof(item));
		}
		item = al_get_config_value(pp2_config, "Audio", "Sound Volume");
		if(item)
		{
			pp2_sound_volume = atof(item);
		}
		item = al_get_config_value(pp2_config, "Game Options", "Level");
		if(item)
		{
			pp2_level_hash = atol(item);
		}
		item = al_get_config_value(pp2_config, "Game Options", "Mode");
		if(item)
		{
			pp2_option[PP2_OPTION_GAME_MODE] = atoi(item);
		}
		item = al_get_config_value(pp2_config, "Game Options", "Elimination Hits");
		if(item)
		{
			pp2_option[PP2_OPTION_ELIMINATION_HITS] = atoi(item);
		}
		item = al_get_config_value(pp2_config, "Game Options", "Death Match Frags");
		if(item)
		{
			pp2_option[PP2_OPTION_DEATH_MATCH_FRAGS] = atoi(item);
		}
		item = al_get_config_value(pp2_config, "Game Options", "Life");
		if(item)
		{
			pp2_option[PP2_OPTION_LIFE] = atoi(item);
		}
		item = al_get_config_value(pp2_config, "Game Options", "CTF Flags");
		if(item)
		{
			pp2_option[PP2_OPTION_CTF_FLAGS] = atoi(item);
		}
		item = al_get_config_value(pp2_config, "Game Options", "Time Limit");
		if(item)
		{
			pp2_option[PP2_OPTION_TIME_LIMIT] = atoi(item);
		}
		item = al_get_config_value(pp2_config, "Game Options", "Random Items");
		if(item)
		{
			pp2_option[PP2_OPTION_RANDOMIZE_ITEMS] = !strcmp(item, "on") ? 1 : 0;
		}
		item = al_get_config_value(pp2_config, "Game Options", "Ammo Normal");
		if(item)
		{
			pp2_option[PP2_OPTION_ENABLE_AMMO_NORMAL] = !strcmp(item, "on") ? 1 : 0;
		}
		item = al_get_config_value(pp2_config, "Game Options", "Ammo Splitter");
		if(item)
		{
			pp2_option[PP2_OPTION_ENABLE_AMMO_X] = !strcmp(item, "on") ? 1 : 0;
		}
		item = al_get_config_value(pp2_config, "Game Options", "Ammo Mine");
		if(item)
		{
			pp2_option[PP2_OPTION_ENABLE_AMMO_MINE] = !strcmp(item, "on") ? 1 : 0;
		}
		item = al_get_config_value(pp2_config, "Game Options", "Ammo Bouncer");
		if(item)
		{
			pp2_option[PP2_OPTION_ENABLE_AMMO_BOUNCE] = !strcmp(item, "on") ? 1 : 0;
		}
		item = al_get_config_value(pp2_config, "Game Options", "Ammo Seeker");
		if(item)
		{
			pp2_option[PP2_OPTION_ENABLE_AMMO_SEEK] = !strcmp(item, "on") ? 1 : 0;
		}
		item = al_get_config_value(pp2_config, "Game Options", "Ammo Reflector");
		if(item)
		{
			pp2_option[PP2_OPTION_ENABLE_AMMO_REFLECTOR] = !strcmp(item, "on") ? 1 : 0;
		}
		item = al_get_config_value(pp2_config, "Game Options", "Ammo Proximity Mine");
		if(item)
		{
			pp2_option[PP2_OPTION_ENABLE_AMMO_PMINE] = !strcmp(item, "on") ? 1 : 0;
		}
		item = al_get_config_value(pp2_config, "Game Options", "Ammo Ghost");
		if(item)
		{
			pp2_option[PP2_OPTION_ENABLE_AMMO_GHOST] = !strcmp(item, "on") ? 1 : 0;
		}
		item = al_get_config_value(pp2_config, "Game Options", "Power-Up Cloak");
		if(item)
		{
			pp2_option[PP2_OPTION_ENABLE_POWERUP_CLOAK] = !strcmp(item, "on") ? 1 : 0;
		}
		item = al_get_config_value(pp2_config, "Game Options", "Power-Up Jump");
		if(item)
		{
			pp2_option[PP2_OPTION_ENABLE_POWERUP_JUMP] = !strcmp(item, "on") ? 1 : 0;
		}
		item = al_get_config_value(pp2_config, "Game Options", "Power-Up Run");
		if(item)
		{
			pp2_option[PP2_OPTION_ENABLE_POWERUP_RUN] = !strcmp(item, "on") ? 1 : 0;
		}
		item = al_get_config_value(pp2_config, "Game Options", "Power-Up Deflect");
		if(item)
		{
			pp2_option[PP2_OPTION_ENABLE_POWERUP_DEFLECT] = !strcmp(item, "on") ? 1 : 0;
		}
		item = al_get_config_value(pp2_config, "Game Options", "Power-Up Fly");
		if(item)
		{
			pp2_option[PP2_OPTION_ENABLE_POWERUP_FLY] = !strcmp(item, "on") ? 1 : 0;
		}
		item = al_get_config_value(pp2_config, "Game Options", "Power-Up Turbo");
		if(item)
		{
			pp2_option[PP2_OPTION_ENABLE_POWERUP_TURBO] = !strcmp(item, "on") ? 1 : 0;
		}
		item = al_get_config_value(pp2_config, "Game Options", "Stock Normal");
		if(item)
		{
			pp2_option[PP2_OPTION_START_AMMO_NORMAL] = atoi(item);
		}
		item = al_get_config_value(pp2_config, "Game Options", "Stock Splitter");
		if(item)
		{
			pp2_option[PP2_OPTION_START_AMMO_X] = atoi(item);
		}
		item = al_get_config_value(pp2_config, "Game Options", "Stock Mine");
		if(item)
		{
			pp2_option[PP2_OPTION_START_AMMO_MINE] = atoi(item);
		}
		item = al_get_config_value(pp2_config, "Game Options", "Stock Bouncer");
		if(item)
		{
			pp2_option[PP2_OPTION_START_AMMO_BOUNCE] = atoi(item);
		}
		item = al_get_config_value(pp2_config, "Game Options", "Stock Seeker");
		if(item)
		{
			pp2_option[PP2_OPTION_START_AMMO_SEEK] = atoi(item);
		}
		item = al_get_config_value(pp2_config, "Game Options", "Stock Reflector");
		if(item)
		{
			pp2_option[PP2_OPTION_START_AMMO_REFLECTOR] = atoi(item);
		}
		item = al_get_config_value(pp2_config, "Game Options", "Stock Proximity Mine");
		if(item)
		{
			pp2_option[PP2_OPTION_START_AMMO_PMINE] = atoi(item);
		}
		item = al_get_config_value(pp2_config, "Game Options", "Stock Ghost");
		if(item)
		{
			pp2_option[PP2_OPTION_START_AMMO_GHOST] = atoi(item);
		}
		item = al_get_config_value(pp2_config, "Game Options", "Ammo Worth");
		if(item)
		{
			pp2_option[PP2_OPTION_AMMO_WORTH] = atoi(item);
		}
		item = al_get_config_value(pp2_config, "Graphics Options", "Trails");
		if(item)
		{
			if(!strcasecmp(item, "on"))
			{
				pp2_option[PP2_OPTION_TRAILS] = 1;
			}
			else
			{
				pp2_option[PP2_OPTION_TRAILS] = 0;
			}
		}
		for(i = 0; i < 4; i++)
		{
			sprintf(temp_string2, "Controller %d", i);
			for(j = 0; j < 9; j++)
			{
				sprintf(temp_string, "%s Type", pp2_button_name[j]);
				item = al_get_config_value(pp2_config, temp_string2, temp_string);
				if(item)
				{
					pp2_controller[i]->binding[j].type = atoi(item);
				}
				sprintf(temp_string, "%s SubType", pp2_button_name[j]);
				item = al_get_config_value(pp2_config, temp_string2, temp_string);
				if(item)
				{
					pp2_controller[i]->binding[j].sub_type = atoi(item);
				}
				sprintf(temp_string, "%s Joystick", pp2_button_name[j]);
				item = al_get_config_value(pp2_config, temp_string2, temp_string);
				if(item)
				{
					pp2_controller[i]->binding[j].joystick = atoi(item);
				}
				sprintf(temp_string, "%s Button", pp2_button_name[j]);
				item = al_get_config_value(pp2_config, temp_string2, temp_string);
				if(item)
				{
					pp2_controller[i]->binding[j].button = atoi(item);
				}
				sprintf(temp_string, "%s Stick", pp2_button_name[j]);
				item = al_get_config_value(pp2_config, temp_string2, temp_string);
				if(item)
				{
					pp2_controller[i]->binding[j].stick = atoi(item);
				}
				sprintf(temp_string, "%s Axis", pp2_button_name[j]);
				item = al_get_config_value(pp2_config, temp_string2, temp_string);
				if(item)
				{
					pp2_controller[i]->binding[j].axis = atoi(item);
				}
				sprintf(temp_string, "%s Flags", pp2_button_name[j]);
				item = al_get_config_value(pp2_config, temp_string2, temp_string);
				if(item)
				{
					pp2_controller[i]->binding[j].flags = atoi(item);
				}
			}
		}
		item = al_get_config_value(pp2_config, "System", "Cache Version");
		if(item)
		{
			if(atoi(item) != PP2_CACHE_VERSION)
			{
				pp2_regenerate_cache = true;
			}
		}
		else
		{
			pp2_regenerate_cache = true;
		}
	}
	
	return true;
}

bool pp2_save_config(const char * fn)
{
	char buf[256] = {0};
	sprintf(buf, "%1.2f", t3f_get_music_volume());
	al_set_config_value(pp2_config, "Audio", "Music Volume", buf);
	sprintf(buf, "%1.2f", pp2_sound_volume);
	al_set_config_value(pp2_config, "Audio", "Sound Volume", buf);
	
	/* game options */
	sprintf(buf, "%lu", pp2_level_hash);
	al_set_config_value(pp2_config, "Game Options", "Level", buf);
	sprintf(buf, "%d", pp2_option[PP2_OPTION_GAME_MODE]);
	al_set_config_value(pp2_config, "Game Options", "Mode", buf);
	sprintf(buf, "%d", pp2_option[PP2_OPTION_ELIMINATION_HITS]);
	al_set_config_value(pp2_config, "Game Options", "Elimination Hits", buf);
	sprintf(buf, "%d", pp2_option[PP2_OPTION_DEATH_MATCH_FRAGS]);
	al_set_config_value(pp2_config, "Game Options", "Death Match Frags", buf);
	sprintf(buf, "%d", pp2_option[PP2_OPTION_LIFE]);
	al_set_config_value(pp2_config, "Game Options", "Life", buf);
	sprintf(buf, "%d", pp2_option[PP2_OPTION_CTF_FLAGS]);
	al_set_config_value(pp2_config, "Game Options", "CTF Flags", buf);
	sprintf(buf, "%d", pp2_option[PP2_OPTION_TIME_LIMIT]);
	al_set_config_value(pp2_config, "Game Options", "Time Limit", buf);
	sprintf(buf, "%s", pp2_option[PP2_OPTION_RANDOMIZE_ITEMS] ? "on" : "off");
	al_set_config_value(pp2_config, "Game Options", "Random Items", buf);
	sprintf(buf, "%s", pp2_option[PP2_OPTION_ENABLE_AMMO_NORMAL] ? "on" : "off");
	al_set_config_value(pp2_config, "Game Options", "Ammo Normal", buf);
	sprintf(buf, "%s", pp2_option[PP2_OPTION_ENABLE_AMMO_X] ? "on" : "off");
	al_set_config_value(pp2_config, "Game Options", "Ammo Splitter", buf);
	sprintf(buf, "%s", pp2_option[PP2_OPTION_ENABLE_AMMO_MINE] ? "on" : "off");
	al_set_config_value(pp2_config, "Game Options", "Ammo Mine", buf);
	sprintf(buf, "%s", pp2_option[PP2_OPTION_ENABLE_AMMO_BOUNCE] ? "on" : "off");
	al_set_config_value(pp2_config, "Game Options", "Ammo Bouncer", buf);
	sprintf(buf, "%s", pp2_option[PP2_OPTION_ENABLE_AMMO_SEEK] ? "on" : "off");
	al_set_config_value(pp2_config, "Game Options", "Ammo Seeker", buf);
	sprintf(buf, "%s", pp2_option[PP2_OPTION_ENABLE_AMMO_REFLECTOR] ? "on" : "off");
	al_set_config_value(pp2_config, "Game Options", "Ammo Reflector", buf);
	sprintf(buf, "%s", pp2_option[PP2_OPTION_ENABLE_AMMO_PMINE] ? "on" : "off");
	al_set_config_value(pp2_config, "Game Options", "Ammo Proximity Mine", buf);
	sprintf(buf, "%s", pp2_option[PP2_OPTION_ENABLE_AMMO_GHOST] ? "on" : "off");
	al_set_config_value(pp2_config, "Game Options", "Ammo Ghost", buf);
	sprintf(buf, "%d", pp2_option[PP2_OPTION_START_AMMO_NORMAL]);
	al_set_config_value(pp2_config, "Game Options", "Stock Normal", buf);
	sprintf(buf, "%d", pp2_option[PP2_OPTION_START_AMMO_X]);
	al_set_config_value(pp2_config, "Game Options", "Stock Splitter", buf);
	sprintf(buf, "%d", pp2_option[PP2_OPTION_START_AMMO_MINE]);
	al_set_config_value(pp2_config, "Game Options", "Stock Mine", buf);
	sprintf(buf, "%d", pp2_option[PP2_OPTION_START_AMMO_BOUNCE]);
	al_set_config_value(pp2_config, "Game Options", "Stock Bouncer", buf);
	sprintf(buf, "%d", pp2_option[PP2_OPTION_START_AMMO_SEEK]);
	al_set_config_value(pp2_config, "Game Options", "Stock Seeker", buf);
	sprintf(buf, "%d", pp2_option[PP2_OPTION_START_AMMO_REFLECTOR]);
	al_set_config_value(pp2_config, "Game Options", "Stock Reflector", buf);
	sprintf(buf, "%d", pp2_option[PP2_OPTION_START_AMMO_PMINE]);
	al_set_config_value(pp2_config, "Game Options", "Stock Proximity Mine", buf);
	sprintf(buf, "%d", pp2_option[PP2_OPTION_START_AMMO_GHOST]);
	al_set_config_value(pp2_config, "Game Options", "Stock Ghost", buf);
	sprintf(buf, "%s", pp2_option[PP2_OPTION_ENABLE_POWERUP_CLOAK] ? "on" : "off");
	al_set_config_value(pp2_config, "Game Options", "Power-Up Cloak", buf);
	sprintf(buf, "%s", pp2_option[PP2_OPTION_ENABLE_POWERUP_JUMP] ? "on" : "off");
	al_set_config_value(pp2_config, "Game Options", "Power-Up Jump", buf);
	sprintf(buf, "%s", pp2_option[PP2_OPTION_ENABLE_POWERUP_RUN] ? "on" : "off");
	al_set_config_value(pp2_config, "Game Options", "Power-Up Run", buf);
	sprintf(buf, "%s", pp2_option[PP2_OPTION_ENABLE_POWERUP_DEFLECT] ? "on" : "off");
	al_set_config_value(pp2_config, "Game Options", "Power-Up Deflect", buf);
	sprintf(buf, "%s", pp2_option[PP2_OPTION_ENABLE_POWERUP_FLY] ? "on" : "off");
	al_set_config_value(pp2_config, "Game Options", "Power-Up Fly", buf);
	sprintf(buf, "%s", pp2_option[PP2_OPTION_ENABLE_POWERUP_TURBO] ? "on" : "off");
	al_set_config_value(pp2_config, "Game Options", "Power-Up Turbo", buf);
	sprintf(buf, "%d", pp2_option[PP2_OPTION_AMMO_WORTH]);
	al_set_config_value(pp2_config, "Game Options", "Ammo Worth", buf);
	sprintf(buf, "%s", pp2_option[PP2_OPTION_TRAILS] ? "on" : "off");
	al_set_config_value(pp2_config, "Graphics Options", "Trails", buf);
	sprintf(buf, "%d", PP2_CACHE_VERSION);
	al_set_config_value(pp2_config, "System", "Cache Version", buf);
	return al_save_config_file(fn, pp2_config);
}
