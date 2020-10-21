#ifndef PP2_CONFIG_H
#define PP2_CONFIG_H

#include "../interface/interface.h"

bool pp2_load_level_config(const char * fn);
bool pp2_save_level_config(const char * fn);
bool pp2_load_character_config(const char * fn);
bool pp2_save_character_config(const char * fn);
bool pp2_load_config(PP2_INTERFACE * ip, const char * fn);
bool pp2_save_config(PP2_INTERFACE * ip, const char * fn);

#endif
