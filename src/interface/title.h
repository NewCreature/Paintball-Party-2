#ifndef PP2_TITLE_H
#define PP2_TITLE_H

#include <allegro5/allegro5.h>
#include "../resource.h"
#include "gameplay/game_struct.h"
#include "interface.h"

#define PP2_TITLE_BG_COLOR al_map_rgba_f(0.75, 0.75, 1.0, 1.0)

#define PP2_MAX_CREDITS 256
#define PP2_CREDIT_COLOR_TITLE al_map_rgb(255, 255, 128)
#define PP2_CREDIT_COLOR_NAME  al_map_rgb_f(1.0, 1.0, 1.0)
#define PP2_CREDIT_TITLE_SPACE 24.0
#define PP2_CREDIT_SPACE       72.0

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

void pp2_title_build_credits(PP2_CREDITS * cp);

void pp2_t_title_menu_logic(void * data);
void pp2_t_title_menu_render(PP2_INTERFACE * ip, PP2_RESOURCES * resources);
void pp2_title_setup(void);
void pp2_title_logic(PP2_GAME * gp, PP2_RESOURCES * resources);
void pp2_title_render(PP2_RESOURCES * resources);

#endif
