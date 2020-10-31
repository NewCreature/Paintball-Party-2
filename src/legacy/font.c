#include "t3f/t3f.h"
#include "t3f/font.h"
#include "palette.h"
#include "bitmap.h"

typedef struct
{

  int width;
  int height;
  ALLEGRO_BITMAP * ch[256];

} NCD_FONT;

static void destroy_legacy_font(NCD_FONT * font)
{
  int i;

  if(font)
  {
    for(i = 0; i < 256; i++)
    {
      if(font->ch[i])
      {
        al_destroy_bitmap(font->ch[i]);
      }
    }
    free(font);
  }
}

static void * load_legacy_font_f(ALLEGRO_FILE * fp)
{
  ALLEGRO_STATE old_state;
  NCD_FONT * font;
  char header[4];
  int i, j, k, c;

  font = malloc(sizeof(NCD_FONT));
  if(!font)
  {
    goto fail;
  }
  memset(font, 0, sizeof(NCD_FONT));

  al_fread(fp, header, 4);
  if(header[0] != 'M' || header[1] != 'F' || header[2] != 'T' || header[3] != 21)
  {
    printf("bad header\n");
    goto fail;
  }

  font->width = al_fgetc(fp);
  font->height = al_fgetc(fp);

  al_store_state(&old_state, ALLEGRO_STATE_TARGET_BITMAP);
  for(i = 0; i < 256; i++)
  {
    font->ch[i] = al_create_bitmap(font->width * 2, font->height * 2);
    if(font->ch[i])
    {
      al_set_target_bitmap(font->ch[i]);
      al_lock_bitmap(font->ch[i], ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_WRITEONLY);
      for(j = 0; j < font->height; j++)
      {
        for(k = 0; k < font->width; k++)
        {
          c = al_fgetc(fp);
          if(c > 0)
          {
            pp2_legacy_put_pixel(k, j, pp2_legacy_get_color(c, PP2_LEGACY_COLOR_SOLID), 2);
          }
          else
          {
            pp2_legacy_put_pixel(k, j, al_map_rgba_f(0.0, 0.0, 0.0, 0.0), 2);
          }
        }
      }
      al_unlock_bitmap(font->ch[i]);
    }
  }
  al_restore_state(&old_state);
  font->width *= 2;
  font->height *= 2;
  return font;

  fail:
  {
    destroy_legacy_font(font);
  }
  return NULL;
}

static void * font_engine_load_font_f_legacy(const char * fn, ALLEGRO_FILE * fp, int option, int flags)
{
  return load_legacy_font_f(fp);
}

static void font_engine_destroy_font_legacy(void * font)
{
  destroy_legacy_font(font);
}

static int font_engine_get_text_width_legacy(const void * font, const char * text)
{
  NCD_FONT * fp = (NCD_FONT *)font;

  return fp->width * strlen(text);
}

static int font_engine_get_font_height_legacy(const void * font)
{
  NCD_FONT * fp = (NCD_FONT *)font;

  return fp->height;
}

static void font_engine_draw_glyph_legacy(const void * font, ALLEGRO_COLOR color, float x, float y, int codepoint)
{
  NCD_FONT * fp = (NCD_FONT *)font;

	al_draw_tinted_bitmap(fp->ch[codepoint], color, x, y, codepoint);
}

static int font_engine_get_glyph_width_legacy(const void * font, int codepoint)
{
  NCD_FONT * fp = (NCD_FONT *)font;

  return fp->width;
}

static bool font_engine_get_glyph_dimensions_legacy(const void * font, int codepoint, int * bbx, int * bby, int * bbw, int * bbh)
{
  return 0;
}

static int font_engine_get_glyph_advance_legacy(const void * font, int codepoint1, int codepoint2)
{
  return 0;
}

static void font_engine_draw_text_legacy(const void * font, ALLEGRO_COLOR color, float x, float y, float z, int flags, char const * text)
{
  NCD_FONT * fp = (NCD_FONT *)font;
  int i;

  for(i = 0; i < strlen(text); i++)
  {
    t3f_draw_bitmap(fp->ch[(int)text[i]], color, x, y, z, 0);
    x += fp->width;
  }
}

static void font_engine_draw_textf_legacy(const void * font, ALLEGRO_COLOR color, float x, float y, float z, int flags, const char * format, ...)
{
	char buf[1024] = {0};
	va_list vap;

	va_start(vap, format);
	vsnprintf(buf, 1024, format, vap);
	va_end(vap);

	font_engine_draw_text_legacy(font, color, x, y, z, flags, buf);
}

static T3F_FONT_ENGINE legacy_font_engine =
{
  font_engine_load_font_f_legacy,
  font_engine_destroy_font_legacy,

  font_engine_get_text_width_legacy,
  font_engine_get_font_height_legacy,
  font_engine_draw_glyph_legacy,
  font_engine_get_glyph_width_legacy,
  font_engine_get_glyph_dimensions_legacy,
  font_engine_get_glyph_advance_legacy,
  font_engine_draw_text_legacy,
  font_engine_draw_textf_legacy
};

T3F_FONT * pp2_load_legacy_font_f(ALLEGRO_FILE * fp)
{
  return t3f_load_font_with_engine_f(&legacy_font_engine, NULL, fp, 0, 0);
}

T3F_FONT * pp2_load_legacy_font(const char * fn)
{
  return t3f_load_font_with_engine(&legacy_font_engine, fn, 0, 0);
}

bool pp2_legacy_font_resource_handler_proc(void ** ptr, ALLEGRO_FILE * fp, const char * filename, int option, int flags, unsigned long offset, bool destroy)
{
  ALLEGRO_STATE old_state;
	bool openfp = false;

  if(destroy)
  {
    t3f_destroy_font(*ptr);
    return true;
  }

	al_store_state(&old_state, ALLEGRO_STATE_NEW_BITMAP_PARAMETERS);
	al_set_new_bitmap_flags(al_get_new_bitmap_flags() | ALLEGRO_NO_PRESERVE_TEXTURE);
	if(fp)
	{
		openfp = true;
	}

	/* load file directly if offset is 0 and open file not passed */
	else if(offset == 0)
	{
		*ptr = pp2_load_legacy_font(filename);
	}
	else
	{
		if(!openfp)
		{
			fp = al_fopen(filename, "rb");
			if(fp)
			{
				al_fseek(fp, offset, ALLEGRO_SEEK_SET);
			}
		}
		if(fp)
		{
			*ptr = pp2_load_legacy_font_f(fp);
			if(!openfp)
			{
				al_fclose(fp);
			}
		}
	}
	al_restore_state(&old_state);

  return *ptr;
}
