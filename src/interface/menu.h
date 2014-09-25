#ifndef PP2_MENU_H
#define PP2_MENU_H

#include "../t3f/gui.h"

#define PP2_MENU_HEADER_COLOR al_map_rgb(147, 210, 0)
#define PP2_MENU_OPTION_COLOR al_map_rgb(255, 255, 255)

void pp2_menu_initialize(void);
void pp2_process_menu(T3F_GUI * menu);
void pp2_menu_logic(void);
void pp2_menu_render(void);

#endif
