#include "../../pp2.h"
#include "../../interface/preview.h"
#include "character.h"
#include "../character.h"
#include "../../legacy/character.h"
#include "../../resource.h"

#include <allegro5/allegro5.h>

PP2_CHARACTER_DATABASE_EXTRA * pp2_character_database_create(PP2_FILE_DATABASE * dp, int entry, int flags, PP2_RESOURCES * resources)
{
	ALLEGRO_PATH * new_path = NULL;
	PP2_CHARACTER_DATABASE_EXTRA * ep;
	char newfn[1024] = {0};

	ep = malloc(sizeof(PP2_CHARACTER_DATABASE_EXTRA));
	if(!ep)
	{
		return NULL;
	}

	new_path = al_clone_path(t3f_data_path);
	al_append_path_component(new_path, "preview_cache");
	al_append_path_component(new_path, "characters");
	sprintf(newfn, "%lu.p2p", dp->entry[entry]->checksum);
	al_set_path_filename(new_path, newfn);
	strcpy(ep->preview, al_path_cstr(new_path, '/'));

	/* if preview does not exist, create it */
	if((flags & PP2_DATABASE_FLAG_REGENERATE) || !al_filename_exists(al_path_cstr(new_path, '/')))
	{
		if(!pp2_create_character_preview_from_character((char *)al_path_cstr(dp->entry[entry]->path, '/'), al_path_cstr(new_path, '/'), resources))
		{
			free(ep);
			al_destroy_path(new_path);
			return NULL;
		}
	}
	al_destroy_path(new_path);
	return ep;
}
