#include "t3f/t3f.h"
#include "data.h"

char pp2_entered_text[256] = {0};
int pp2_entering_text_pos = 0;
int pp2_entering_text = 0;

static bool is_string_empty(char * string)
{
	int i;
	
	for(i = 0; i < strlen(string); i++)
	{
		if(string[i] != ' ')
		{
			return false;
		}
	}
	return true;
}

void pp2_enter_text(void)
{
	if(pp2_entering_text)
	{
		char key = t3f_read_key(0);
		switch(key)
		{
			case '\r':
			{
				
				/* send chat message */
				if(pp2_entering_text == 2 && pp2_client)
				{
					ENetPacket * pp;
					pp = joynet_create_packet(PP2_GAME_MESSAGE_TYPING_DONE, NULL);
					enet_peer_send(pp2_client->peer, JOYNET_CHANNEL_GAME, pp);
					if(!is_string_empty(pp2_entered_text))
					{
						joynet_send_client_chat(pp2_client, pp2_entered_text, 0);
					}
					pp2_entering_text = 0;
					t3f_key[ALLEGRO_KEY_ENTER] = 0;
				}
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
}
