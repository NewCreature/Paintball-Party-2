#ifndef PP2_RESOURCE_H
#define PP2_RESOURCE_H

#include "defines.h"

typedef struct
{

  ALLEGRO_BITMAP * bitmap[PP2_MAX_BITMAPS];
	T3F_ANIMATION * animation[PP2_MAX_ANIMATIONS];

} PP2_RESOURCES;

#endif
