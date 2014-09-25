#ifndef PP2_DATABASE_H
#define PP2_DATABASE_H

#include <allegro5/allegro5.h>

#define PP2_DATABASE_MAX_ENTRIES 1024

#define PP2_DATABASE_FLAG_NO_CHECKSUM 1
#define PP2_DATABASE_FLAG_RECURSE     2
#define PP2_DATABASE_FLAG_REGENERATE  4

typedef struct
{
	
	ALLEGRO_PATH * path;
	void * extra;
	unsigned long checksum;
	
} PP2_FILE_DATABASE_ENTRY;

typedef struct
{
	
	ALLEGRO_CONFIG * cache;
	char cache_filename[1024];
	PP2_FILE_DATABASE_ENTRY ** entry;
	int entries;
	
} PP2_FILE_DATABASE;

void pp2_set_database_callback(void (*callback)(const ALLEGRO_PATH * pp));
PP2_FILE_DATABASE * pp2_create_database(const char * fn, int entries, int flags);
void pp2_destroy_database(PP2_FILE_DATABASE * dp);
unsigned long pp2_database_count_files(const char * location, const char * type, int flags);
void pp2_database_add_file(PP2_FILE_DATABASE * dp, const ALLEGRO_PATH * pp, int flags);
void pp2_database_add_files(PP2_FILE_DATABASE * dp, const ALLEGRO_PATH * path, const char * type, int flags);
int pp2_database_find_entry(PP2_FILE_DATABASE * dp, unsigned long checksum);

#endif
