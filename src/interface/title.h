#ifndef PP2_TITLE_H
#define PP2_TITLE_H

#include <allegro5/allegro5.h>

#define PP2_MAX_CREDITS 256
#define PP2_CREDIT_COLOR_TITLE al_map_rgb(147, 210, 0)
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

void pp2_t_title_menu_logic(void);
void pp2_t_title_menu_render(void);
void pp2_title_setup(void);
void pp2_title_logic(void);
void pp2_title_render(void);

#endif
