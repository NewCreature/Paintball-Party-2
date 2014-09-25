#ifndef PP2_INIT_H
#define PP2_INIT_H

#include "t3f/t3f.h"

extern char pp2_load_text[1024];

void pp2_show_load_screen(const char * text);
void pp2_database_callback(const ALLEGRO_PATH * pp);
void pp2_setup_directories(void);
void pp2_autodetect_controllers(void);
void pp2_set_controller_config(int controller, int binding);
bool pp2_load_images(void);
bool pp2_load_sounds(void);
bool pp2_load_animations(void);
bool pp2_build_character_database(void);
bool pp2_build_level_database(void);
bool pp2_build_music_database(void);
bool pp2_build_demo_database(void);
bool pp2_setup_joynet(void);

#endif
