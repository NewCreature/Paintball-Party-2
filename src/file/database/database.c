#include <allegro5/allegro5.h>
#include "../../t3f/t3f.h"
#include "database.h"

static void (*pp2_database_callback)(const ALLEGRO_PATH * pp) = NULL;

void pp2_set_database_callback(void (*callback)(const ALLEGRO_PATH * pp))
{
	pp2_database_callback = callback;
}

PP2_FILE_DATABASE * pp2_create_database(const char * fn, int entries, int flags)
{
	PP2_FILE_DATABASE * dp;
	
	dp = malloc(sizeof(PP2_FILE_DATABASE));
	if(!dp)
	{
		return NULL;
	}
	memset(dp, 0, sizeof(PP2_FILE_DATABASE));
	dp->entry = malloc(entries * sizeof(PP2_FILE_DATABASE_ENTRY *));
	if(!dp->entry)
	{
		free(dp);
		return NULL;
	}
	if(fn)
	{
		strcpy(dp->cache_filename, fn);
		
		/* don't load cache if we want to regenerate */
		if(!(flags & PP2_DATABASE_FLAG_REGENERATE))
		{
			dp->cache = al_load_config_file(fn);
		}
		if(!dp->cache)
		{
			dp->cache = al_create_config();
		}
		if(!dp->cache)
		{
			return NULL;
		}
	}
	else
	{
		dp->cache = NULL;
	}
	return dp;
}

void pp2_destroy_database(PP2_FILE_DATABASE * dp)
{
	int i;

	if(dp->cache)
	{
		al_save_config_file(dp->cache_filename, dp->cache);
		al_destroy_config(dp->cache);
	}
	for(i = 0; i < dp->entries; i++)
	{
		free(dp->entry[i]);
	}
	free(dp->entry);
	free(dp);
}

static bool compare_extension(const ALLEGRO_PATH * path, const char * ext)
{
//	int i;
	const char * fext = NULL;
	
	fext = al_get_path_extension(path);
	if(!strcmp(fext, ext))
	{
		return true;
	}
	return false;
}

static unsigned long pp2_database_file_count = 0;

unsigned long pp2_database_count_files(const char * location, const char * type, int flags)
{
	ALLEGRO_FS_ENTRY * dir;
	ALLEGRO_FS_ENTRY * fp;
	ALLEGRO_PATH * pp;
	ALLEGRO_PATH * path;
	const char * name;
	char cname[1024] = {0};
	
	/* reset counter if this is the first time entering this function */
	if(!(flags & PP2_DATABASE_FLAG_RECURSE))
	{
		pp2_database_file_count = 0;
	}
	
	path = al_create_path(location);
	name = al_path_cstr(path, '/');
	strcpy(cname, name);
	if(cname[strlen(cname) - 1] == '/')
	{
		if(flags & PP2_DATABASE_FLAG_RECURSE)
		{
			if(cname[strlen(cname) - 2] == '.')
			{
				return 0;
			}
		}
		cname[strlen(cname) - 1] = 0;
	}
	
	dir = al_create_fs_entry(cname);
	if(!dir)
	{
		return 0;
	}
	if(!al_open_directory(dir))
	{
		return 0;
	}
	while(1)
	{
		fp = al_read_directory(dir);
		if(!fp)
		{
			break;
		}
		pp = al_create_path(al_get_fs_entry_name(fp));
		if(al_get_fs_entry_mode(fp) & ALLEGRO_FILEMODE_ISDIR)
		{
			pp2_database_count_files(al_path_cstr(pp, '/'), type, flags | PP2_DATABASE_FLAG_RECURSE);
		}
		else
		{
			if(compare_extension(pp, type))
			{
				pp2_database_file_count++;
			}
		}
		al_destroy_path(pp);
		al_destroy_fs_entry(fp);
	}
	al_destroy_fs_entry(dir);
	al_destroy_path(path);
	return pp2_database_file_count;
}

void pp2_database_add_file(PP2_FILE_DATABASE * dp, const ALLEGRO_PATH * pp, int flags)
{
	const char * val;
	char val2[128] = {0};

//	printf("%s\n", al_path_cstr(pp, '/'));
	dp->entry[dp->entries] = malloc(sizeof(PP2_FILE_DATABASE_ENTRY));
	if(dp->entry[dp->entries])
	{
		dp->entry[dp->entries]->path = al_clone_path(pp);
		dp->entry[dp->entries]->extra = NULL;
		if(!(flags & PP2_DATABASE_FLAG_NO_CHECKSUM))
		{
			if(dp->cache)
			{
				val = al_get_config_value(dp->cache, al_path_cstr(pp, '/'), "checksum");
				if(!val)
				{
					dp->entry[dp->entries]->checksum = t3f_checksum_file(al_path_cstr(dp->entry[dp->entries]->path, '/'));
					sprintf(val2, "%lu", dp->entry[dp->entries]->checksum);
					al_set_config_value(dp->cache, al_path_cstr(pp, '/'), "checksum", val2);
				}
				else
				{
					dp->entry[dp->entries]->checksum = atoi(val);
				}
			}
			else
			{
				dp->entry[dp->entries]->checksum = t3f_checksum_file(al_path_cstr(dp->entry[dp->entries]->path, '/'));
			}
		}
		dp->entries++;
	}
}

int pp2_database_find_entry(PP2_FILE_DATABASE * dp, unsigned long checksum)
{
	int i;
	
	for(i = 0; i < dp->entries; i++)
	{
		if(dp->entry[i]->checksum == checksum)
		{
			return i;
		}
	}
	return -1;
}

void pp2_database_add_files(PP2_FILE_DATABASE * dp, const ALLEGRO_PATH * path, const char * type, int flags)
{
	ALLEGRO_FS_ENTRY * dir;
	ALLEGRO_FS_ENTRY * fp;
	ALLEGRO_PATH * pp;
	const char * name;
	char cname[1024] = {0};
	
	name = al_path_cstr(path, '/');
	strcpy(cname, name);
	if(cname[strlen(cname) - 1] == '/')
	{
		if(flags & PP2_DATABASE_FLAG_RECURSE)
		{
			if(cname[strlen(cname) - 2] == '.')
			{
				return;
			}
		}
		cname[strlen(cname) - 1] = 0;
	}
	
//	printf("!Looking in %s\n", cname);
	dir = al_create_fs_entry(cname);
	if(!dir)
	{
		return;
	}
	if(!al_open_directory(dir))
	{
		return;
	}
//	printf("Looking in %s\n", cname);
	while(1)
	{
		fp = al_read_directory(dir);
		if(!fp)
		{
			break;
		}
		pp = al_create_path(al_get_fs_entry_name(fp));
//		name = al_path_to_string(al_get_entry_name(fp), '/');
		if(al_get_fs_entry_mode(fp) & ALLEGRO_FILEMODE_ISDIR)
		{
			pp2_database_add_files(dp, pp, type, flags | PP2_DATABASE_FLAG_RECURSE);
		}
		else
		{
			if(compare_extension(pp, type))
			{
				pp2_database_add_file(dp, pp, flags);
				if(pp2_database_callback)
				{
					pp2_database_callback(pp);
				}
//				printf("%s\n", al_path_to_string(pp, '/'));
			}
		}
		al_destroy_path(pp);
		al_destroy_fs_entry(fp);
	}
	al_destroy_fs_entry(dir);
}
