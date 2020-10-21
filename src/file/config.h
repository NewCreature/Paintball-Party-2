#ifndef PP2_CONFIG_H
#define PP2_CONFIG_H

#include "../interface/interface.h"
#include "../gameplay/game_struct.h"

bool pp2_load_config(PP2_INTERFACE * ip, PP2_GAME * gp, const char * fn);
bool pp2_save_config(PP2_INTERFACE * ip, PP2_GAME * gp, const char * fn);

#endif
