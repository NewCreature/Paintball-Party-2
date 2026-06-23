#include "t3f/t3f.h"

static char pp2_entered_text[256] = {0};
static int pp2_entering_text_pos = 0;
static int pp2_entering_text = 0;

void pp2_enter_text(const char * text, int mode)
{
	strcpy(pp2_entered_text, text);
	pp2_entering_text_pos = strlen(pp2_entered_text);
	pp2_entering_text = mode;
}

bool pp2_enter_text_logic(void)
{
	bool ret = false;

	if(pp2_entering_text)
	{
		char key = t3f_get_char(0);
		switch(key)
		{
			case '\r':
			{
				/* send chat message */
				if(pp2_entering_text == 2)
				{
					ret = true;
				}
				pp2_entering_text = 0;
				t3f_use_key_press(ALLEGRO_KEY_ENTER);
				break;
			}
			case '\b':
			case 127:
			{
				if(pp2_entering_text_pos > 0)
				{
					pp2_entering_text_pos--;
					pp2_entered_text[pp2_entering_text_pos] = '\0';
				}
				break;
			}

			/* ignore these keys */
			case '\t':
			case '\0':
			{
				break;
			}

			/* type these keys */
			default:
			{
				if(pp2_entering_text_pos < 255)
				{
					pp2_entered_text[pp2_entering_text_pos] = key;
					pp2_entering_text_pos++;
					pp2_entered_text[pp2_entering_text_pos] = '\0';
				}
				break;
			}
		}
	}
	return ret;
}

const char * pp2_get_entered_text(void)
{
	return pp2_entered_text;
}

int pp2_get_text_entry_state(void)
{
	return pp2_entering_text;
}
