#include "../../t3f/t3f.h"
#include "level.h"
#include "../level.h"
#include "../../interface/preview.h"

PP2_LEVEL_DATABASE_EXTRA * pp2_level_database_create(PP2_FILE_DATABASE * dp, int entry, int flags)
{
	ALLEGRO_PATH * new_path = NULL;
	PP2_LEVEL_DATABASE_EXTRA * ep;
	char newfn[1024] = {0};

	ep = malloc(sizeof(PP2_LEVEL_DATABASE_EXTRA));
	if(!ep)
	{
		return NULL;
	}

	new_path = al_clone_path(t3f_data_path);
	al_append_path_component(new_path, "preview_cache");
	al_append_path_component(new_path, "levels");
	sprintf(newfn, "%lu.p2p", dp->entry[entry]->checksum);
	al_set_path_filename(new_path, newfn);
	strcpy(ep->preview, al_path_cstr(new_path, '/'));

	/* if preview does not exist, create it */
	if((flags & PP2_DATABASE_FLAG_REGENERATE) || !al_filename_exists(al_path_cstr(new_path, '/')))
	{
		if(!pp2_create_level_preview_from_level((char *)al_path_cstr(dp->entry[entry]->path, '/'), al_path_cstr(new_path, '/')))
		{
			free(ep);
			al_destroy_path(new_path);
			return NULL;
		}
	}
	al_destroy_path(new_path);
	return ep;
}
