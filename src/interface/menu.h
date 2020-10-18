#ifndef PP2_MENU_H
#define PP2_MENU_H

#include "../t3f/gui.h"
#include "../resource.h"
#include "../pp2.h"

#define PP2_MENU_HEADER_COLOR al_map_rgb(147, 210, 0)
#define PP2_MENU_OPTION_COLOR al_map_rgb(255, 255, 255)

void pp2_adjust_menus(void);
void pp2_menu_initialize(PP2_RESOURCES * resources);
void pp2_process_menu(T3F_GUI * menu, PP2_INSTANCE * instance);
void pp2_menu_logic(PP2_INSTANCE * instance);
void pp2_menu_render(PP2_RESOURCES * resources);

#endif
