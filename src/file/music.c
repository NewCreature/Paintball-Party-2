#include "../t3f/t3f.h"
#include "../t3f/music.h"
#include "../data.h"
#include "music.h"

static const char * pp2_music_type[6] =
{
	"wav",
	"ogg",
	"it",
	"xm",
	"s3m",
	"mod"
};
static const int pp2_music_types = 6;
static char pp2_music_fncopy[1024] = {0};
static bool pp2_music_playing = false;

static void pp2_replace_extension(char * fn, const char * ext)
{
	int p = strlen(fn);
	int i;

	while(p >= 0)
	{
		p--;
		if(fn[p] == '.')
		{
			for(i = 0; i < strlen(ext); i++)
			{
				fn[p + 1 + i] = ext[i];
			}
			fn[p + 1 + i] = '\0';
			break;
		}
	}
}

char * pp2_find_music(const char * fn)
{
	int i;

	strcpy(pp2_music_fncopy, fn);
	for(i = 0; i < pp2_music_types; i++)
	{
		pp2_replace_extension(pp2_music_fncopy, pp2_music_type[i]);
//		printf("music - %s\n", pp2_music_fncopy);
		if(al_filename_exists(pp2_music_fncopy))
		{
			return pp2_music_fncopy;
		}
	}
	return NULL;
}

bool pp2_play_music(const char * fn)
{
	bool ret = false;

	if(!(t3f_flags & T3F_USE_SOUND))
	{
		return false;
	}

	ret = t3f_play_music(fn);
	pp2_music_playing = true;
	return ret;
}

void pp2_stop_music(void)
{
	if(pp2_music_playing)
	{
		t3f_stop_music();
		pp2_music_playing = false;
	}
}

void pp2_pause_music(void)
{
	if(pp2_music_playing)
	{
		t3f_pause_music();
	}
}

void pp2_resume_music(void)
{
	if(pp2_music_playing)
	{
		t3f_resume_music();
	}
}

void pp2_set_music_volume(float vol)
{
	t3f_set_music_volume(vol);
}
