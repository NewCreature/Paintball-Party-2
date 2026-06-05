#ifndef _JOYNET_ROLLBACK_H
#define _JOYNET_ROLLBACK_H

#include <stddef.h>

/* points to a source of gameplay affecting data */
typedef struct
{

  void * data;
  size_t size;

} JOYNET_ROLLBACK_CHUNK;

typedef struct
{

  void * data;

} JOYNET_ROLLBACK_FRAME;

typedef struct
{

  /* all gameplay affecting chunks */
  JOYNET_ROLLBACK_CHUNK * chunk;
  size_t chunk_count;

  /* rollback frames */
  JOYNET_ROLLBACK_FRAME * frame;
  size_t frame_count;
  int frame_pos; // which frame slot we are currently using

} JOYNET_ROLLBACK_DATA;

JOYNET_ROLLBACK_DATA * joynet_create_rollback_data(size_t max_rollback_frames);
void joynet_destroy_rollback_data(JOYNET_ROLLBACK_DATA * rollback_data);

int joynet_add_rollback_chunk(JOYNET_ROLLBACK_DATA * rollback_data, void * data, size_t size);

int joynet_start_rollback_session(JOYNET_ROLLBACK_DATA * rollback_data);
void joynet_store_rollback_frame(JOYNET_ROLLBACK_DATA * rollback_data);

#endif
