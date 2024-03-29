#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "t3net.h"
#include "leaderboard.h"
#include "internal.h"

/* leaderboard setup functions */
char * t3net_get_new_leaderboard_user_key(const char * url, const char * user_name)
{
	T3NET_ARGUMENTS * args = NULL;
	const char * val;
	T3NET_DATA * data = NULL;
	int i;
	char * ret = NULL;

	args = t3net_create_arguments();
	if(!args)
	{
		goto fail;
	}
	if(user_name)
	{
		if(!t3net_add_argument(args, "user_name", user_name))
		{
			goto fail;
		}
	}
	data = t3net_get_data(url, args);
	if(!data)
	{
		goto fail;
	}

	for(i = 0; i < data->entries; i++)
	{
		val = t3net_get_data_entry_field(data, i, "user_key");
		if(val)
		{
			ret = strdup(val);
			break;
		}
	}
	t3net_destroy_arguments(args);
	t3net_destroy_data(data);

	return ret;

	fail:
	{
		if(ret)
		{
			free(ret);
		}
		if(args)
		{
			t3net_destroy_arguments(args);
		}
		if(data)
		{
			t3net_destroy_data(data);
		}
		return NULL;
	}
}

int t3net_update_leaderboard_user_name(const char * url, const char * user_key, const char * user_name)
{
	T3NET_ARGUMENTS * args = NULL;
	T3NET_DATA * data = NULL;

	args = t3net_create_arguments();
	if(!args)
	{
		goto fail;
	}
	if(user_name)
	{
		if(!t3net_add_argument(args, "user_key", user_key))
		{
			goto fail;
		}
		if(!t3net_add_argument(args, "user_name", user_name))
		{
			goto fail;
		}
	}
	data = t3net_get_data(url, args);
	if(!data)
	{
		goto fail;
	}
	t3net_destroy_arguments(args);
	t3net_destroy_data(data);

	return 1;

	fail:
	{
		if(args)
		{
			t3net_destroy_arguments(args);
		}
		if(data)
		{
			t3net_destroy_data(data);
		}
		return 0;
	}
}


T3NET_LEADERBOARD * t3net_get_leaderboard(char * url, char * game, char * version, char * mode, char * option, int entries, int ascend)
{
	T3NET_LEADERBOARD * lp;
	int i, j;

	/* build leaderboard structure */
	lp = malloc(sizeof(T3NET_LEADERBOARD));
	if(!lp)
	{
		return NULL;
	}
	lp->entry = malloc(sizeof(T3NET_LEADERBOARD_ENTRY *) * entries);
	if(!lp->entry)
	{
		free(lp);
		return NULL;
	}
	for(i = 0; i < entries; i++)
	{
		lp->entry[i] = malloc(sizeof(T3NET_LEADERBOARD_ENTRY));
		if(!lp->entry[i])
		{
			break;
		}
		t3net_strcpy(lp->entry[i]->name, "", 256);
		lp->entry[i]->score = -1;
	}
	if(i < entries)
	{
		for(j = 0; j < i; j++)
		{
			free(lp->entry[i]);
		}
		free(lp->entry);
		return NULL;
	}
	lp->entries = entries;
	strcpy(lp->url, url);
	strcpy(lp->game, game);
	strcpy(lp->version, version ? version : "");
	strcpy(lp->mode, mode ? mode : "");
	strcpy(lp->option, option ? option : "");
	lp->ascend = ascend;
	if(!t3net_update_leaderboard(lp))
	{
		t3net_destroy_leaderboard(lp);
		return NULL;
	}

	/* query the server */
	return lp;
}

int t3net_update_leaderboard(T3NET_LEADERBOARD * lp)
{
	T3NET_ARGUMENTS * args = NULL;
	char tnum[64] = {0};
	const char * val;
	T3NET_DATA * data = NULL;
	int i;

	if(!lp)
	{
		goto fail;
	}

	sprintf(tnum, "%d", lp->entries);
	args = t3net_create_arguments();
	if(!args)
	{
		goto fail;
	}
	if(!t3net_add_argument(args, "game", lp->game))
	{
		goto fail;
	}
	if(strlen(lp->version) > 0 && !t3net_add_argument(args, "version", lp->version))
	{
		goto fail;
	}
	if(strlen(lp->mode) > 0 && !t3net_add_argument(args, "mode", lp->mode))
	{
		goto fail;
	}
	if(strlen(lp->option) > 0 && !t3net_add_argument(args, "mode_option", lp->option))
	{
		goto fail;
	}
	if(lp->ascend)
	{
		if(!t3net_add_argument(args, "ascend", "true"))
		{
			goto fail;
		}
	}
	if(!t3net_add_argument(args, "limit", tnum))
	{
		goto fail;
	}
	data = t3net_get_data(lp->url, args);
	if(!data)
	{
		goto fail;
	}

	for(i = 0; i < data->entries; i++)
	{
		val = t3net_get_data_entry_field(data, i, "name");
		if(val)
		{
			t3net_strcpy(lp->entry[i]->name, val, 256);
		}
		val = t3net_get_data_entry_field(data, i, "score");
		if(val)
		{
			lp->entry[i]->score = atoi(val);
		}
		val = t3net_get_data_entry_field(data, i, "extra");
		if(val)
		{
			t3net_strcpy(lp->entry[i]->extra, val, 256);
		}
	}
	lp->entries = data->entries;
	t3net_destroy_arguments(args);
	t3net_destroy_data(data);
	return 1;

	fail:
	{
		if(args)
		{
			t3net_destroy_arguments(args);
		}
		if(data)
		{
			t3net_destroy_data(data);
		}
		return 0;
	}
}

void t3net_clear_leaderboard(T3NET_LEADERBOARD * lp)
{
	lp->entries = 0;
}

void t3net_destroy_leaderboard(T3NET_LEADERBOARD * lp)
{
	int i;

	for(i = 0; i < lp->entries; i++)
	{
		free(lp->entry[i]);
	}
	free(lp->entry);
	free(lp);
}

int t3net_upload_score(const char * url, const char * game, const char * version, const char * mode, const char * option, const char * user_key, unsigned long score, const char * extra)
{
	T3NET_ARGUMENTS * args = NULL;
	T3NET_DATA * data = NULL;
	char tscore[64] = {0};

//	sprintf(url_w_arg, "%s?game=%s&version=%s&mode=%s&option=%s&name=%s&score=%lu", url, game, version, mode, option, tname, score);
	args = t3net_create_arguments();
	if(!args)
	{
		goto fail;
	}
	sprintf(tscore, "%lu", score);
	if(!t3net_add_argument(args, "game", game))
	{
		goto fail;
	}
	if(version && !t3net_add_argument(args, "version", version))
	{
		goto fail;
	}
	if(mode && !t3net_add_argument(args, "mode", mode))
	{
		goto fail;
	}
	if(option && !t3net_add_argument(args, "mode_option", option))
	{
		goto fail;
	}
	if(!t3net_add_argument(args, "user_key", user_key))
	{
		goto fail;
	}
	if(!t3net_add_argument(args, "score", tscore))
	{
		goto fail;
	}
	if(extra)
	{
		if(!t3net_add_argument(args, "extra", extra))
		{
			goto fail;
		}
	}
	data = t3net_get_data(url, args);
	if(!data)
	{
		goto fail;
	}
	t3net_destroy_arguments(args);
	t3net_destroy_data(data);
	return 1;

	fail:
	{
		if(args)
		{
			t3net_destroy_arguments(args);
		}
		if(data)
		{
			t3net_destroy_data(data);
		}
		return 0;
	}
}
