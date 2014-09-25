#ifndef PP2_LEGACY_WAV_H
#define PP2_LEGACY_WAV_H

#include <allegro5/allegro5.h>
#include <allegro5/kcm_audio.h>

ALLEGRO_SAMPLE * pp2_legacy_load_wav_entry(ALLEGRO_FILE * fp);
int pp2_legacy_save_wav_entry(ALLEGRO_SAMPLE * SamplePointer, ALLEGRO_FILE * f);

#endif
