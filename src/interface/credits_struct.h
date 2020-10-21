#ifndef PP2_CREDITS_STRUCT_H
#define PP2_CREDITS_STRUCT_H

#include "credits_defines.h"

typedef struct
{

	char name[128];
	ALLEGRO_COLOR color;
	float y;

} PP2_CREDIT;

typedef struct
{

	PP2_CREDIT credit[PP2_MAX_CREDITS];
	int credits;

} PP2_CREDITS;

#endif
