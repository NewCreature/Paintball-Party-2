#ifndef PP2_GAMEPLAY_NETWORK_H
#define PP2_GAMEPLAY_NETWORK_H

#include "../joynet/joynet.h"

int pp2_game_channel_callback(JOYNET_MESSAGE * mp);
int pp2_chat_callback(char * user, char * message);

#endif
