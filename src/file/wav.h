#ifndef PP2_WAV_H
#define PP2_WAV_H

#include <allegro5/allegro_audio.h>

ALLEGRO_SAMPLE * pp2_load_wav_f(ALLEGRO_FILE * fp);
bool pp2_save_wav_f(ALLEGRO_SAMPLE *spl, ALLEGRO_FILE *pf);
bool pp2_save_wav(ALLEGRO_SAMPLE *spl, char * fn);

#endif
