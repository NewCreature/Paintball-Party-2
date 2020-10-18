#ifndef PP2_GAME_REPLAY_H
#define PP2_GAME_REPLAY_H

#include "../resource.h"

#define PP2_REPLAY_FLAG_DEMO    1
#define PP2_REPLAY_FLAG_THEATER 2
#define PP2_REPLAY_FLAG_CAPTURE 4

extern int pp2_replay_flags;
extern bool pp2_replay_done;

bool pp2_record_replay(const char * fn);
bool pp2_play_replay(const char * fn, int flags, PP2_RESOURCES * resources);
void pp2_finish_replay(void);
void pp2_finish_replay_recording(void);
void pp2_replay_logic(PP2_RESOURCES * resources);
void pp2_replay_render(PP2_RESOURCES * resources);

#endif
