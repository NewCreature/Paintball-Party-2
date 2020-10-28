#ifndef PP2_CLIENT_H
#define PP2_CLIENT_H

#include "../joynet/joynet.h"

int pp2_client_callback(ENetEvent * ep, void * data);
void pp2_spawn_client_keep_alive_thread(PP2_INSTANCE * instance);
void pp2_kill_client_keep_alive_thread(void);

#endif
