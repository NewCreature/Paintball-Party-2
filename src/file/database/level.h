#ifndef PP2_LEVEL_DATABASE_H
#define PP2_LEVEL_DATABASE_H

#include <allegro5/allegro5.h>
#include "database.h"
#include "../../resource.h"

typedef struct
{

	char preview[1024];
	char name[128];
	char author[128];
	char comment[256];

} PP2_LEVEL_DATABASE_EXTRA;

PP2_LEVEL_DATABASE_EXTRA * pp2_level_database_create(PP2_FILE_DATABASE * dp, int entry, int flags, PP2_RESOURCES * resources);

#endif
