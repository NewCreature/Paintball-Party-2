#ifndef PP2_CHARACTER_DATABASE_H
#define PP2_CHARACTER_DATABASE_H

#include <allegro5/allegro5.h>
#include "database.h"
#include "../../resource.h"

typedef struct
{

	char preview[1024];

} PP2_CHARACTER_DATABASE_EXTRA;

PP2_CHARACTER_DATABASE_EXTRA * pp2_character_database_create(PP2_FILE_DATABASE * dp, int entry, int flags, PP2_RESOURCES * resources);

#endif
