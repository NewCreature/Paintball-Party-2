#ifndef PP2_GAMEPLAY_NETWORK_H
#define PP2_GAMEPLAY_NETWORK_H

#include "../joynet/joynet.h"
#include "../pp2.h"

void pp2_set_network_instance(PP2_INSTANCE * instance);
int pp2_game_channel_callback(JOYNET_MESSAGE * mp, void * data);
int pp2_chat_callback(char * user, char * message);

#endif
