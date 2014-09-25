#ifndef PP2_SERVER_H
#define PP2_SERVER_H

#include <allegro5/allegro.h>
#include "../joynet/joynet.h"
#include "../joynet/game.h"

void * pp2_server_thread_proc(ALLEGRO_THREAD * thread, void * arg);

#endif
