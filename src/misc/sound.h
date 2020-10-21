#ifndef PP2_SOUND_H
#define PP2_SOUND_H

#include <allegro5/allegro5.h>
#include <allegro5/allegro_audio.h>
#include "../gameplay/game_struct.h"

bool pp2_play_sample(PP2_GAME * gp, ALLEGRO_SAMPLE * sp, float sourcex, float sourcey, float volume, float speed);

#endif
