#ifndef PP2_RESOURCE_H
#define PP2_RESOURCE_H

#include "t3f/t3f.h"
#include "defines.h"
#include "file/database/database.h"

typedef struct
{

  ALLEGRO_BITMAP * bitmap[PP2_MAX_BITMAPS];
	T3F_ANIMATION * animation[PP2_MAX_ANIMATIONS];
  T3F_ANIMATION * object_animation[PP2_MAX_OBJECT_TYPES];
  T3F_ATLAS * object_atlas;
  T3F_FONT * font[PP2_MAX_FONTS];
  ALLEGRO_SAMPLE * sample[PP2_MAX_SAMPLES];
  PP2_FILE_DATABASE * level_database;
  PP2_FILE_DATABASE * character_database;
  PP2_FILE_DATABASE * music_database;
  PP2_FILE_DATABASE * demo_database;

} PP2_RESOURCES;

#endif
