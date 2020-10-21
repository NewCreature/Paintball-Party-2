#include <allegro5/allegro5.h>
#include <allegro5/allegro_audio.h>
#include "../t3f/sound.h"
#include "../data.h"
#include "sound.h"
#include "../gameplay/game_struct.h"

bool pp2_play_sample(PP2_GAME * gp, ALLEGRO_SAMPLE * sp, float sourcex, float sourcey, float volume, float speed)
{
	float vol, pan;

	if(gp->replay_rewind)
	{
		return true;
	}
	/* play center if we are viewing more than one camera */
	if(gp->local_player < 0)
	{
		vol = 1.0 * volume;
		pan = 0.0;
	}
	else
	{
		float earx = gp->player[gp->local_player].x + gp->player[gp->local_player].object[0]->map.top.point[0].x;
		float eary = gp->player[gp->local_player].y + gp->player[gp->local_player].object[0]->map.top.point[0].y;
		vol = t3f_get_sound_gain(earx, eary, sourcex, sourcey, 1280.0) * volume;
		pan = t3f_get_sound_position(earx, eary, sourcex, sourcey);
	}
	return t3f_play_sample(sp, vol, pan, speed);
}
