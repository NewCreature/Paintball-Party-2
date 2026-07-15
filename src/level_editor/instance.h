#ifndef _PP2_LEVEDIT_INSRANCE_H
#define _PP2_LEVEDIT_INSRANCE_H

#include "t3f/t3f.h"
#include "file/level.h"

#define LEVEDIT_MAX_MENUS        64

/* menu stuff */
#define LEVEDIT_MENU_MAIN         0
#define LEVEDIT_MENU_FILE         1
#define LEVEDIT_MENU_FILE_IMPORT  2
#define LEVEDIT_MENU_FILE_EXPORT  3

typedef struct
{
  ALLEGRO_FILECHOOSER * file_load_dialog;
  ALLEGRO_FILECHOOSER * file_save_dialog;
  ALLEGRO_FILECHOOSER * file_load_ani_dialog;
  const char * path;

  T3F_FONT * font;
  char temp_string[1024];

  PP2_LEVEL * level;
  T3F_ANIMATION * object_animation[256];
  T3F_ATLAS * object_atlas;
  float cx;
  float cy;
  float cz;
  int view;
  int tick;
  int hover_object;
  int selected_object;
  int hover_tile;
  int selected_tile;
  int object_type;
  int selected_layer;
  int place_x, place_y;
  bool clicked;
  int tilemap_hover_x;
  int tilemap_hover_y;
  bool tilemap_hover_fail;
  char * entry_pointer;
  int entry_pos;
  int selected_meta;
  ALLEGRO_MENU * menu[LEVEDIT_MAX_MENUS];

} PP2_LEVEL_EDITOR_INSTANCE;

PP2_LEVEL_EDITOR_INSTANCE * pp2_create_level_editor_instance(int argc, char * argv[]);
void pp2_destroy_level_editor_instance(PP2_LEVEL_EDITOR_INSTANCE * level_editor);

#endif
