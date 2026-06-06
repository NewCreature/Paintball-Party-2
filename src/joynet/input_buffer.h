#ifndef JOYNET_INPUT_BUFFER_H
#define JOYNET_INPUT_BUFFER_H

typedef struct
{

	char * data;

	int read_pos;
	int write_pos;
	int previous_write_pos;
	int frames;
	int max_frames;
	size_t frame_size;
	uint32_t filled_frames;
	uint32_t read_frames; // number of frames read for this gameplay session
	uint32_t * last_real_frame; // each player's last real input frame

} JOYNET_INPUT_BUFFER;

JOYNET_INPUT_BUFFER * joynet_create_input_buffer(int frame_size, int max_frames, int player_count);
void joynet_destroy_input_buffer(JOYNET_INPUT_BUFFER * ip);

void joynet_write_input_buffer_frame(JOYNET_INPUT_BUFFER * ip, const char * data);
void joynet_read_input_buffer_frame(JOYNET_INPUT_BUFFER * ip, char * out);
void joynet_duplicate_input_buffer_frame(JOYNET_INPUT_BUFFER * ip);
void joynet_reset_input_buffer(JOYNET_INPUT_BUFFER * ip);
int joynet_get_input_buffer_frames(JOYNET_INPUT_BUFFER * ip);

#endif
