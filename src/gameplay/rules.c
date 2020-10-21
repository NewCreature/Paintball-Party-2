#include "../data.h"
#include "../file/music.h"
#include "rules.h"
#include "game.h"
#include "game_defines.h"
#include "replay.h"

void pp2_process_rules(PP2_GAME * gp)
{
	unsigned long shots_current;
	unsigned long shots = 0;
	unsigned long hits_current;
	unsigned long hits = 0;
	unsigned long triggers_current;
	unsigned long triggers = 0;
	int i, j;

	/* only process rules if no winner has been determined yet */
	if(gp->winner >= 0)
	{
		return;
	}
	switch(gp->option[PP2_OPTION_GAME_MODE])
	{
		case PP2_GAME_MODE_ELIMINATOR:
		{
			int c = 0;
			int p = -1;
			for(i = 0; i < PP2_MAX_PLAYERS; i++)
			{
				if(gp->player[i].flags & PP2_PLAYER_FLAG_ACTIVE)
				{
					p = i;
					c++;
				}
			}
			if(c < 2)
			{
				gp->next_state = PP2_STATE_GAME_OVER;
				gp->winner = p;
				pp2_play_music("data/music/results.ogg");
			}
			break;
		}
		case PP2_GAME_MODE_DEATH_MATCH:
		{
			int highest = 0;
			int highest_i = -1;
			int highest_c = 0;
			if(gp->option[PP2_OPTION_DEATH_MATCH_FRAGS] > 0)
			{
				for(i = 0; i < PP2_MAX_PLAYERS; i++)
				{
					if(gp->player[i].flags & PP2_PLAYER_FLAG_ACTIVE)
					{
						if(gp->player[i].frags >= gp->option[PP2_OPTION_DEATH_MATCH_FRAGS])
						{
							gp->next_state = PP2_STATE_GAME_OVER;
							gp->winner = i;
							for(j = 0; j < PP2_MAX_PLAYERS; j++)
							{
								if(gp->player[j].flags & PP2_PLAYER_FLAG_ACTIVE && j != gp->winner)
								{
									gp->player[j].fade_time = 30;
									gp->player[j].fade_type = 0;
								}
							}
							pp2_play_music("data/music/results.ogg");
						}
					}
				}
			}
			if(gp->option[PP2_OPTION_TIME_LIMIT] > 0)
			{
				if(gp->time_left <= 0)
				{
					for(i = 0; i < PP2_MAX_PLAYERS; i++)
					{
						if(gp->player[i].flags & PP2_PLAYER_FLAG_ACTIVE)
						{
							if(gp->player[i].frags > highest)
							{
								highest = gp->player[i].frags;
								highest_i = i;
							}
						}
					}
					if(highest_i >= 0)
					{
						for(i = 0; i < PP2_MAX_PLAYERS; i++)
						{
							if(gp->player[i].flags & PP2_PLAYER_FLAG_ACTIVE)
							{
								if(gp->player[i].frags == highest)
								{
									highest_c++;
								}
							}
						}
						if(highest_c == 1)
						{
							gp->next_state = PP2_STATE_GAME_OVER;
							gp->winner = highest_i;
							for(i = 0; i < PP2_MAX_PLAYERS; i++)
							{
								if(gp->player[i].flags & PP2_PLAYER_FLAG_ACTIVE && i != gp->winner)
								{
									gp->player[i].fade_time = 30;
									gp->player[i].fade_type = 0;
								}
							}
							pp2_play_music("data/music/results.ogg");
						}
					}
				}
			}
			break;
		}
		case PP2_GAME_MODE_COIN_RUSH:
		{
			int highest = 0;
			int highest_i = -1;
			int highest_c = 0;
			for(i = 0; i < PP2_MAX_PLAYERS; i++)
			{
				if(gp->player[i].flags & PP2_PLAYER_FLAG_ACTIVE)
				{
					if(gp->player[i].coins >= gp->coins_needed)
					{
						gp->next_state = PP2_STATE_GAME_OVER;
						gp->winner = i;
						for(j = 0; j < PP2_MAX_PLAYERS; j++)
						{
							if(gp->player[j].flags & PP2_PLAYER_FLAG_ACTIVE && j != gp->winner)
							{
								gp->player[j].fade_time = 30;
								gp->player[j].fade_type = 0;
							}
						}
						pp2_play_music("data/music/results.ogg");
					}
				}
			}
			if(gp->option[PP2_OPTION_TIME_LIMIT] > 0)
			{
				if(gp->time_left <= 0)
				{
					for(i = 0; i < PP2_MAX_PLAYERS; i++)
					{
						if(gp->player[i].flags & PP2_PLAYER_FLAG_ACTIVE)
						{
							if(gp->player[i].coins > highest)
							{
								highest = gp->player[i].coins;
								highest_i = i;
							}
						}
					}
					if(highest_i >= 0)
					{
						for(i = 0; i < PP2_MAX_PLAYERS; i++)
						{
							if(gp->player[i].flags & PP2_PLAYER_FLAG_ACTIVE)
							{
								if(gp->player[i].coins == highest)
								{
									highest_c++;
								}
							}
						}
						if(highest_c == 1)
						{
							gp->next_state = PP2_STATE_GAME_OVER;
							gp->winner = highest_i;
							for(i = 0; i < PP2_MAX_PLAYERS; i++)
							{
								if(gp->player[i].flags & PP2_PLAYER_FLAG_ACTIVE && i != gp->winner)
								{
									gp->player[i].fade_time = 30;
									gp->player[i].fade_type = 0;
								}
							}
							pp2_play_music("data/music/results.ogg");
						}
					}
				}
			}
			break;
		}
	}

	if(gp->winner >= 0)
	{
		gp->award_accuracy = -1;
		gp->award_fodder = -1;
		gp->award_trigger = -1;

		/* update profiles */
		for(i = 0; i < PP2_MAX_PLAYERS; i++)
		{
			if(gp->player[i].playing && pp2_client_game->player[i]->local)
			{
				if(i == gp->winner)
				{
					gp->player[i].profile->wins++;
				}
				else
				{
					gp->player[i].profile->losses++;
				}
			}
		}

		/* hand out awards */
		for(i = 0; i < PP2_MAX_PLAYERS; i++)
		{
			if(gp->player[i].playing)
			{
				if(gp->player[i].total_hits > 0)
				{
					shots_current = (gp->player[i].shots * 100) / (gp->player[i].total_hits);
					if(shots_current > shots)
					{
						gp->award_accuracy = i;
						shots = shots_current;
					}
				}
				hits_current = gp->player[i].shot;
				if(hits_current > hits)
				{
					gp->award_fodder = i;
					hits = hits_current;
				}
				triggers_current = gp->player[i].shots;
				if(triggers_current > triggers)
				{
					gp->award_trigger = i;
					triggers = triggers_current;
				}
			}
		}
	}
}
