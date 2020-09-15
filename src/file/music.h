#ifndef PP2_MUSIC_H
#define PP2_MUSIC_H

#include <allegro5/allegro5.h>

#define PP2_MUSIC_TYPE_STREAM 0
#define PP2_MUSIC_TYPE_MODULE 1

char * pp2_find_music(const char * fn);
bool pp2_play_music(const char * fn);
void pp2_stop_music(void);
void pp2_pause_music(void);
void pp2_resume_music(void);
void pp2_set_music_volume(float vol);

#endif
