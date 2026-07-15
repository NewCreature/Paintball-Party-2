#ifndef _PP2_LEVEL_EDITOR_MENU_PROC_H
#define _PP2_LEVEL_EDITOR_MENU_PROC_H

#include "t3f/t3f.h"

int levedit_menu_update_proc_loaded(ALLEGRO_MENU * mp, int i, void * data);
int levedit_menu_proc_new(int i, void * data);
int levedit_menu_proc_open(int i, void * data);
int levedit_menu_proc_save(int i, void * data);
int levedit_menu_proc_save_as(int i, void * data);
int levedit_menu_proc_import_tileset(int i, void * data);
int levedit_menu_proc_import_tilemap(int i, void * data);
int levedit_menu_proc_export_tileset(int i, void * data);
int levedit_menu_proc_export_tilemap(int i, void * data);
int levedit_menu_proc_quit(int i, void * data);

#endif
