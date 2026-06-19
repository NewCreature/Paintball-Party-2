#ifndef PP2_MESSAGE_H
#define PP2_MESSAGE_H

#include "t3f/t3f.h"
#include "t3f/font.h"

#define PP2_MESSAGE_MAX_LINES 16
#define PP2_MESSAGE_MAX_SIZE 256
#define PP2_MAX_MESSAGES      64

typedef struct
{

	char message[PP2_MESSAGE_MAX_SIZE];
	char * line[PP2_MESSAGE_MAX_LINES];
	int line_offset[PP2_MESSAGE_MAX_LINES];
	int lines;
	T3F_FONT * font;
	ALLEGRO_COLOR color;
	int time;
	int width;
	float tab;

} PP2_MESSAGE;

typedef struct
{

	PP2_MESSAGE message[PP2_MAX_MESSAGES];
	int messages;
	ALLEGRO_MUTEX * mutex; // want to be able to add messages from server thread

} PP2_MESSAGE_LIST;

PP2_MESSAGE_LIST * pp2_create_message_list(void);
void pp2_destroy_message_list(PP2_MESSAGE_LIST * lp);
void pp2_add_message(PP2_MESSAGE_LIST * lp, char * message, T3F_FONT * fp, ALLEGRO_COLOR color, int time, int width, float tab);
void pp2_clear_messages(PP2_MESSAGE_LIST * lp);
void pp2_message_logic(PP2_MESSAGE_LIST * lp);
void pp2_message_render(PP2_MESSAGE_LIST * lp, float ox, float oy);

#endif
