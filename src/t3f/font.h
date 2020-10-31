#ifndef T3F_FONT_H
#define T3F_FONT_H

#include "t3f.h"

#define T3F_FONT_MAX_CHARACTERS 256
#define T3F_FONT_OUTLINE          1
#define T3F_FONT_ALIGN_RIGHT      1
#define T3F_FONT_ALIGN_CENTER     2

#define T3F_FONT_TYPE_AUTO       -1

typedef struct
{

	void * (*load)(const char * fn, ALLEGRO_FILE * fp, int option, int flags);
	void (*destroy)(void * font);

	int (*get_text_width)(const void * font, const char * text);
	int (*get_font_height)(const void * font);
	void (*draw_glyph)(const void * font, ALLEGRO_COLOR color, float x, float y, int codepoint);
	int (*get_glyph_width)(const void * font, int codepoint);
	bool (*get_glyph_dimensions)(const void * font, int codepoint, int * bbx, int * bby, int * bbw, int * bbh);
	int (*al_get_glyph_advance)(const void * font, int codepoint1, int codepoint2);

	void (*draw_text)(const void * font, ALLEGRO_COLOR color, float x, float y, float z, int flags, char const *text);
	void (*draw_textf)(const void * font, ALLEGRO_COLOR color, float x, float y, float z, int flags, const char *format, ...);

} T3F_FONT_ENGINE;

typedef struct
{

	ALLEGRO_BITMAP * bitmap;
	int x, y, width, height;

} T3F_FONT_CHARACTER;

typedef struct
{

	T3F_FONT_ENGINE * engine;
	void * font;

	ALLEGRO_FONT * allegro_font; // use Allegro rendering routines if this pointer is non-NULL
	ALLEGRO_BITMAP * character_sheet;
	T3F_FONT_CHARACTER character[T3F_FONT_MAX_CHARACTERS];
	float adjust;
	float scale;

} T3F_FONT;

typedef struct
{

	char text[256];

} T3F_TEXT_LINE;

typedef struct
{

	T3F_FONT * font;
	T3F_TEXT_LINE line[64];
	int lines;
	float tab;

} T3F_TEXT_LINE_DATA;

T3F_FONT * t3f_load_font_with_engine_f(T3F_FONT_ENGINE * engine, const char * fn, ALLEGRO_FILE * fp, int option, int flags);
T3F_FONT * t3f_load_font_with_engine(T3F_FONT_ENGINE * engine, const char * fn, int option, int flags);
T3F_FONT * t3f_load_font_f(const char * fn, ALLEGRO_FILE * fp, int type, int option, int flags);
T3F_FONT * t3f_load_font(const char * fn, int type, int option, int flags);

ALLEGRO_FONT * t3f_load_bitmap_font(const char * fn);

T3F_FONT * t3f_generate_font_f(ALLEGRO_FILE * fp, int size, int flags);
T3F_FONT * t3f_generate_font(const char * fn, int size, int flags);
bool t3f_save_font(T3F_FONT * fp, const char * fn);
void t3f_destroy_font(T3F_FONT * fp);
float t3f_get_text_width(T3F_FONT * fp, const char * text);
float t3f_get_font_line_height(T3F_FONT * fp);
void t3f_draw_text(T3F_FONT * fp, ALLEGRO_COLOR color, float x, float y, float z, int flags, const char * text);
void t3f_draw_textf(T3F_FONT * fp, ALLEGRO_COLOR color, float x, float y, float z, int flags, const char * format, ...);
void t3f_draw_multiline_text(T3F_FONT * fp, ALLEGRO_COLOR color, float x, float y, float z, float w, float tab, int flags, const char * text);
void t3f_draw_multiline_textf(T3F_FONT * fp, ALLEGRO_COLOR color, float x, float y, float z, float w, float tab, int flags, const char * format, ...);

void t3f_create_text_line_data(T3F_TEXT_LINE_DATA * lp, T3F_FONT * fp, float w, float tab, const char * text);
void t3f_draw_text_lines(T3F_TEXT_LINE_DATA * lines, ALLEGRO_COLOR color, float x, float y, float z);

#endif
