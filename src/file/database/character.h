#ifndef PP2_CHARACTER_DATABASE_H
#define PP2_CHARACTER_DATABASE_H

#include "t3f/t3f.h"
#include "database.h"
#include "pp2/resource.h"

typedef struct
{

	char preview[1024];

} PP2_CHARACTER_DATABASE_EXTRA;

PP2_CHARACTER_DATABASE_EXTRA * pp2_character_database_create(PP2_FILE_DATABASE * dp, int entry, int flags, PP2_RESOURCES * resources);

#endif
