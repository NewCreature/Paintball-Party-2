#ifndef PP2_GAME_REPLAY_H
#define PP2_GAME_REPLAY_H

#include "../resource.h"
#include "game_struct.h"
#include "../interface/interface.h"

#define PP2_REPLAY_FLAG_DEMO    1
#define PP2_REPLAY_FLAG_THEATER 2
#define PP2_REPLAY_FLAG_CAPTURE 4

extern int pp2_replay_flags;
extern bool pp2_replay_done;

bool pp2_record_replay(PP2_GAME * gp, const char * fn, PP2_RESOURCES * resources);
bool pp2_play_replay(PP2_GAME * gp, const char * fn, int flags, PP2_INTERFACE * ip, PP2_RESOURCES * resources);
void pp2_finish_replay(PP2_GAME * gp, PP2_RESOURCES * resources);
void pp2_finish_replay_recording(PP2_GAME * gp);
void pp2_replay_logic(PP2_GAME * gp, PP2_INTERFACE * ip, PP2_RESOURCES * resources);
void pp2_replay_render(PP2_GAME * gp, PP2_RESOURCES * resources);

#endif
