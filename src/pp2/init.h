#ifndef PP2_INIT_H
#define PP2_INIT_H

#include "t3f/t3f.h"
#include "resource.h"
#include "pp2.h"

extern char pp2_load_text[1024];

void pp2_show_load_screen(const char * text, PP2_RESOURCES * resources);
void pp2_database_callback(const ALLEGRO_PATH * pp);
void pp2_setup_directories(PP2_RESOURCES * resources);
void pp2_autodetect_controllers(PP2_INTERFACE * ip);
void pp2_set_controller_config(int controller, int binding);
bool pp2_load_resources(PP2_THEME * theme, PP2_RESOURCES * resources);
bool pp2_load_fonts(PP2_THEME * theme, PP2_RESOURCES * resources);
bool pp2_load_images(PP2_THEME * theme, PP2_RESOURCES * resources);
bool pp2_load_sounds(PP2_THEME * theme, PP2_RESOURCES * resources);
bool pp2_load_animations(PP2_THEME * theme, PP2_RESOURCES * resources);
bool pp2_build_character_database(PP2_INTERFACE * ip, PP2_RESOURCES * resources);
bool pp2_build_level_database(PP2_INTERFACE * ip, PP2_RESOURCES * resources);
bool pp2_build_music_database(PP2_INTERFACE * ip, PP2_RESOURCES * resources);
bool pp2_build_demo_database(PP2_RESOURCES * resources);
bool pp2_setup_joynet(PP2_INSTANCE * instance);

#endif
