#ifndef PP2_LEGACY_ANIMATION_H
#define PP2_LEGACY_ANIMATION_H

#include <allegro5/allegro5.h>
#include "../t3f/animation.h"

T3F_ANIMATION * pp2_legacy_load_ani_fp(ALLEGRO_FILE * fp, const char * fn, void * pal);
T3F_ANIMATION * pp2_legacy_load_animation(const char * fn);

#endif
