#ifndef PP2_LEGACY_FONT_H
#define PP2_LEGACY_FONT_H

#include "t3f/t3f.h"
#include "t3f/font.h"

bool pp2_legacy_font_resource_handler_proc(void ** ptr, ALLEGRO_FILE * fp, const char * filename, int option, int flags, unsigned long offset, bool destroy);
T3F_FONT * pp2_load_legacy_font_(ALLEGRO_FILE * fp);
T3F_FONT * pp2_load_legacy_font(const char * fn);

#endif
