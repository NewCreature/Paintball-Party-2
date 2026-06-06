#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "rollback.h"

JOYNET_ROLLBACK_DATA * joynet_create_rollback_data(size_t max_rollback_frames)
{
  JOYNET_ROLLBACK_DATA * rollback_data = NULL;

  rollback_data = malloc(sizeof(JOYNET_ROLLBACK_DATA));
  if(!rollback_data)
  {
    goto fail;
  }
  memset(rollback_data, 0, sizeof(JOYNET_ROLLBACK_DATA));
  rollback_data->frame_count = max_rollback_frames;

  return rollback_data;

  fail:
  {
    joynet_destroy_rollback_data(rollback_data);
    return NULL;
  }
}

void joynet_destroy_rollback_data(JOYNET_ROLLBACK_DATA * rollback_data)
{
  if(rollback_data)
  {
    if(rollback_data->frame)
    {
      free(rollback_data->frame);
    }
    free(rollback_data);
  }
}

int joynet_add_rollback_chunk(JOYNET_ROLLBACK_DATA * rollback_data, void * data, size_t size)
{
  JOYNET_ROLLBACK_CHUNK * rollback_chunk = NULL;
  JOYNET_ROLLBACK_CHUNK * new_rollback_chunk;

  rollback_chunk = malloc(sizeof(JOYNET_ROLLBACK_CHUNK) * (rollback_data->chunk_count + 1));
  if(!rollback_chunk)
  {
    goto fail;
  }
  memset(rollback_chunk, 0, sizeof(JOYNET_ROLLBACK_CHUNK) * (rollback_data->chunk_count + 1));
  memcpy(rollback_chunk, rollback_data->chunk, sizeof(JOYNET_ROLLBACK_CHUNK) * (rollback_data->chunk_count));
  new_rollback_chunk = &rollback_data->chunk[rollback_data->chunk_count];
  new_rollback_chunk->data = data;
  new_rollback_chunk->size = size;
  rollback_data->chunk_count++;

  return 1;

  fail:
  {
    if(rollback_chunk)
    {
      free(rollback_chunk);
    }
    return 0;
  }
}

int joynet_start_rollback_session(JOYNET_ROLLBACK_DATA * rollback_data)
{
  rollback_data->frame = malloc(sizeof(JOYNET_ROLLBACK_FRAME) * rollback_data->frame_count);
  if(!rollback_data->frame)
  {
    goto fail;
  }
  memset(rollback_data->frame, 0, sizeof(JOYNET_ROLLBACK_FRAME) * rollback_data->frame_count);

  return 1;

  fail:
  {
    if(rollback_data->frame)
    {
      free(rollback_data->frame);
    }
    return 0;
  }
}

void joynet_store_rollback_frame(JOYNET_ROLLBACK_DATA * rollback_data)
{
  int i;
  int offset = 0;

  for(i = 0; i < rollback_data->chunk_count; i++)
  {
    memcpy((&rollback_data->frame[rollback_data->frame_pos].data) + offset, rollback_data->chunk[i].data, rollback_data->chunk[i].size);
    offset = rollback_data->chunk[i].size;
  }
  rollback_data->frame_pos++;
  if(rollback_data->frame_pos >= rollback_data->frame_count)
  {
    rollback_data->frame_pos = 0;
  }
}

void joynet_restore_rollback_frame(JOYNET_ROLLBACK_DATA * rollback_data, int frames)
{
  int i;
  int offset = 0;

  rollback_data->frame_pos -= frames;
  if(rollback_data->frame_pos < 0)
  {
    rollback_data += rollback_data->frame_count;
  }
  for(i = 0; i < rollback_data->chunk_count; i++)
  {
    memcpy(rollback_data->chunk[i].data, (&rollback_data->frame[rollback_data->frame_pos].data) + offset, rollback_data->chunk[i].size);
    offset = rollback_data->chunk[i].size;
  }
}

void joynet_request_rollback(JOYNET_ROLLBACK_DATA * rollback_data, int frames)
{
  rollback_data->wanted_rollback = frames;
}

int joynet_want_rollback(JOYNET_ROLLBACK_DATA * rollback_data)
{
  return rollback_data->wanted_rollback;
}

void joynet_finalize_rollback(JOYNET_ROLLBACK_DATA * rollback_data)
{
  rollback_data->wanted_rollback = 0;
}