#ifndef PP2_LEGACY_PALETTE_H
#define PP2_LEGACY_PALETTE_H

#define PP2_LEGACY_COLOR_SOLID 0
#define PP2_LEGACY_COLOR_TRANS 1
#define PP2_LEGACY_COLOR_LIGHT 2

void pp2_legacy_set_color_type(int type);
void pp2_legacy_set_color(int color, ALLEGRO_COLOR acolor);
ALLEGRO_COLOR pp2_legacy_get_color(int color, int type);
bool pp2_legacy_load_palette(const char * fn);

#endif
