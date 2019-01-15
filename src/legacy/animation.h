#ifndef PP2_LEGACY_ANIMATION_H
#define PP2_LEGACY_ANIMATION_H

#include <allegro5/allegro5.h>
#include "../t3f/animation.h"

#define PP2_LEGACY_COLOR_SOLID 0
#define PP2_LEGACY_COLOR_TRANS 1
#define PP2_LEGACY_COLOR_LIGHT 2

extern ALLEGRO_COLOR legacy_palette[256];
extern int pp2_legacy_color_type;

void pp2_register_legacy_character_bitmap_resource_loader(void);
bool pp2_legacy_load_palette(char * fn);
T3F_ANIMATION * pp2_legacy_load_ani_fp(ALLEGRO_FILE * fp, const char * fn, void * pal);
T3F_ANIMATION * pp2_legacy_load_animation(const char * fn);

#endif
