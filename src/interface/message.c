#include <stdio.h>
#include "message.h"

PP2_MESSAGE_LIST * pp2_create_message_list(void)
{
	PP2_MESSAGE_LIST * lp;
	
	lp = malloc(sizeof(PP2_MESSAGE_LIST));
	if(lp)
	{
		lp->messages = 0;
	}
	lp->mutex = al_create_mutex();
	if(!lp->mutex)
	{
		return NULL;
	}
	return lp;
}

void pp2_destroy_message_list(PP2_MESSAGE_LIST * lp)
{
	free(lp);
}

static int pp2_check_message_lines(PP2_MESSAGE_LIST * lp)
{
	int i;
	int count = 0;
	
	for(i = 0; i < lp->messages; i++)
	{
		count += lp->message[i].lines;
	}
	return count;
}

void pp2_add_message(PP2_MESSAGE_LIST * lp, char * message, ALLEGRO_FONT * fp, ALLEGRO_COLOR color, int time, int width, float tab)
{
	char letter[2] = {0};
	int i, j;
	int curwidth = 0;
	int curline = 0;
	int lastchar = 0;
	bool nonspace = false;

	al_lock_mutex(lp->mutex);
	strcpy(lp->message[lp->messages].message, message);
	lp->message[lp->messages].font = fp;
	lp->message[lp->messages].color = color;
	lp->message[lp->messages].time = time;
	lp->message[lp->messages].width = width;
	lp->message[lp->messages].tab = tab;
	
	/* split message up into lines so we can show multi-line messages */
	lp->message[lp->messages].lines = 0;
	lp->message[lp->messages].line[lp->message[lp->messages].lines] = &lp->message[lp->messages].message[0];
	lp->message[lp->messages].line_offset[lp->message[lp->messages].lines] = 0;
	lp->message[lp->messages].lines++;
	for(i = 0; i < strlen(message); i++)
	{
		nonspace = false;
		letter[0] = lp->message[lp->messages].message[i];
		if(letter[0] != ' ')
		{
			nonspace = true;
		}
		curwidth += al_get_text_width(fp, letter);
		lastchar = i;
		
		/* if we are too wide, backtrack to previous space and make new line */
		if(curwidth >= width && nonspace)
		{
			for(j = i; j >= 0; j--)
			{
				if(lp->message[lp->messages].message[j] == ' ')
				{
					
					/* if the letter that made it surpass the width is a space, new line starts at next character */
					if(j == i)
					{
						lastchar = i + 1;
					}
					
					/* make sure we don't go past the end of the message */
					if(lastchar < strlen(message))
					{
						lp->message[lp->messages].message[j] = 0;
						lp->message[lp->messages].line[lp->message[lp->messages].lines] = &lp->message[lp->messages].message[lastchar];
						lp->message[lp->messages].line_offset[lp->message[lp->messages].lines] = lastchar;
						lp->message[lp->messages].lines++;
					}
					break;
				}
				
				/* curline should be the last non-empty character */
				else
				{
					lastchar = j;
				}
			}
			curwidth = tab;
			curline = lastchar;
		}
	}
	lp->messages++;
	while(pp2_check_message_lines(lp) > 6)
	{
		for(j = 0; j < lp->messages - 1; j++)
		{
			memcpy(&lp->message[j], &lp->message[j + 1], sizeof(PP2_MESSAGE));
			for(i = 0; i < lp->message[j].lines; i++)
			{
				lp->message[j].line[i] = &lp->message[j].message[lp->message[j].line_offset[i]];
			}
		}
		lp->messages--;
	}
	al_unlock_mutex(lp->mutex);
}

/* removes chat messages */
void pp2_clear_messages(PP2_MESSAGE_LIST * lp)
{
	int i, j, k;
	
	al_lock_mutex(lp->mutex);
	for(i = lp->messages - 1; i >= 0; i--)
	{
		if(lp->message[i].time < 0)
		{
			for(j = i; j < lp->messages - 1; j++)
			{
				memcpy(&lp->message[j], &lp->message[j + 1], sizeof(PP2_MESSAGE));
				for(k = 0; k < lp->message[j].lines; k++)
				{
					lp->message[j].line[k] = &lp->message[j].message[lp->message[j].line_offset[k]];
				}
			}
			lp->messages--;
		}
	}
	al_unlock_mutex(lp->mutex);
}

void pp2_message_logic(PP2_MESSAGE_LIST * lp)
{
	int i = 0, j, k;
	
	al_lock_mutex(lp->mutex);
	for(i = lp->messages - 1; i >= 0; i--)
	{
		if(lp->message[i].time >= 0)
		{
			lp->message[i].time--;
			if(lp->message[i].time <= 0)
			{
				for(j = i; j < lp->messages - 1; j++)
				{
					memcpy(&lp->message[j], &lp->message[j + 1], sizeof(PP2_MESSAGE));
					for(k = 0; k < lp->message[j].lines; k++)
					{
						lp->message[j].line[k] = &lp->message[j].message[lp->message[j].line_offset[k]];
					}
				}
				lp->messages--;
			}
		}
	}
	al_unlock_mutex(lp->mutex);
}

void pp2_message_render(PP2_MESSAGE_LIST * lp, float ox, float oy)
{
	int i, j;
	float posx = ox;
	float posy = oy;
	
	al_lock_mutex(lp->mutex);
	for(i = 0; i < lp->messages; i++)
	{
		for(j = 0; j < lp->message[i].lines; j++)
		{
			if(j == 0)
			{
				al_draw_text(lp->message[i].font, al_map_rgba_f(0.0, 0.0, 0.0, 0.5), posx + 1, posy + 1, 0, lp->message[i].line[j]);
			}
			else
			{
				al_draw_text(lp->message[i].font, al_map_rgba_f(0.0, 0.0, 0.0, 0.5), posx + lp->message[i].tab + 1, posy + 1, 0, lp->message[i].line[j]);
			}
			if(j == 0)
			{
				al_draw_text(lp->message[i].font, lp->message[i].color, posx, posy, 0, lp->message[i].line[j]);
			}
			else
			{
				al_draw_text(lp->message[i].font, lp->message[i].color, posx + lp->message[i].tab, posy, 0, lp->message[i].line[j]);
			}
			posy += al_get_font_line_height(lp->message[i].font);
		}
	}
	al_unlock_mutex(lp->mutex);
}
