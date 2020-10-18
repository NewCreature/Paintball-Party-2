#include "../data.h"
#include "../file/music.h"
#include "rules.h"
#include "game.h"
#include "game_defines.h"
#include "replay.h"

void pp2_process_rules(void)
{
	unsigned long shots_current;
	unsigned long shots = 0;
	unsigned long hits_current;
	unsigned long hits = 0;
	unsigned long triggers_current;
	unsigned long triggers = 0;
	int i, j;

	/* only process rules if no winner has been determined yet */
	if(pp2_winner >= 0)
	{
		return;
	}
	switch(pp2_option[PP2_OPTION_GAME_MODE])
	{
		case PP2_GAME_MODE_ELIMINATOR:
		{
			int c = 0;
			int p = -1;
			for(i = 0; i < PP2_MAX_PLAYERS; i++)
			{
				if(pp2_player[i].flags & PP2_PLAYER_FLAG_ACTIVE)
				{
					p = i;
					c++;
				}
			}
			if(c < 2)
			{
				pp2_state = PP2_STATE_GAME_OVER;
				pp2_winner = p;
				pp2_play_music("data/music/results.ogg");
			}
			break;
		}
		case PP2_GAME_MODE_DEATH_MATCH:
		{
			int highest = 0;
			int highest_i = -1;
			int highest_c = 0;
			if(pp2_option[PP2_OPTION_DEATH_MATCH_FRAGS] > 0)
			{
				for(i = 0; i < PP2_MAX_PLAYERS; i++)
				{
					if(pp2_player[i].flags & PP2_PLAYER_FLAG_ACTIVE)
					{
						if(pp2_player[i].frags >= pp2_option[PP2_OPTION_DEATH_MATCH_FRAGS])
						{
							pp2_state = PP2_STATE_GAME_OVER;
							pp2_winner = i;
							for(j = 0; j < PP2_MAX_PLAYERS; j++)
							{
								if(pp2_player[j].flags & PP2_PLAYER_FLAG_ACTIVE && j != pp2_winner)
								{
									pp2_player[j].fade_time = 30;
									pp2_player[j].fade_type = 0;
								}
							}
							pp2_play_music("data/music/results.ogg");
						}
					}
				}
			}
			if(pp2_option[PP2_OPTION_TIME_LIMIT] > 0)
			{
				if(pp2_time_left <= 0)
				{
					for(i = 0; i < PP2_MAX_PLAYERS; i++)
					{
						if(pp2_player[i].flags & PP2_PLAYER_FLAG_ACTIVE)
						{
							if(pp2_player[i].frags > highest)
							{
								highest = pp2_player[i].frags;
								highest_i = i;
							}
						}
					}
					if(highest_i >= 0)
					{
						for(i = 0; i < PP2_MAX_PLAYERS; i++)
						{
							if(pp2_player[i].flags & PP2_PLAYER_FLAG_ACTIVE)
							{
								if(pp2_player[i].frags == highest)
								{
									highest_c++;
								}
							}
						}
						if(highest_c == 1)
						{
							pp2_state = PP2_STATE_GAME_OVER;
							pp2_winner = highest_i;
							for(i = 0; i < PP2_MAX_PLAYERS; i++)
							{
								if(pp2_player[i].flags & PP2_PLAYER_FLAG_ACTIVE && i != pp2_winner)
								{
									pp2_player[i].fade_time = 30;
									pp2_player[i].fade_type = 0;
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
				if(pp2_player[i].flags & PP2_PLAYER_FLAG_ACTIVE)
				{
					if(pp2_player[i].coins >= pp2_coins_needed)
					{
						pp2_state = PP2_STATE_GAME_OVER;
						pp2_winner = i;
						for(j = 0; j < PP2_MAX_PLAYERS; j++)
						{
							if(pp2_player[j].flags & PP2_PLAYER_FLAG_ACTIVE && j != pp2_winner)
							{
								pp2_player[j].fade_time = 30;
								pp2_player[j].fade_type = 0;
							}
						}
						pp2_play_music("data/music/results.ogg");
					}
				}
			}
			if(pp2_option[PP2_OPTION_TIME_LIMIT] > 0)
			{
				if(pp2_time_left <= 0)
				{
					for(i = 0; i < PP2_MAX_PLAYERS; i++)
					{
						if(pp2_player[i].flags & PP2_PLAYER_FLAG_ACTIVE)
						{
							if(pp2_player[i].coins > highest)
							{
								highest = pp2_player[i].coins;
								highest_i = i;
							}
						}
					}
					if(highest_i >= 0)
					{
						for(i = 0; i < PP2_MAX_PLAYERS; i++)
						{
							if(pp2_player[i].flags & PP2_PLAYER_FLAG_ACTIVE)
							{
								if(pp2_player[i].coins == highest)
								{
									highest_c++;
								}
							}
						}
						if(highest_c == 1)
						{
							pp2_state = PP2_STATE_GAME_OVER;
							pp2_winner = highest_i;
							for(i = 0; i < PP2_MAX_PLAYERS; i++)
							{
								if(pp2_player[i].flags & PP2_PLAYER_FLAG_ACTIVE && i != pp2_winner)
								{
									pp2_player[i].fade_time = 30;
									pp2_player[i].fade_type = 0;
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

	if(pp2_winner >= 0)
	{
		pp2_award_accuracy = -1;
		pp2_award_fodder = -1;
		pp2_award_trigger = -1;

		/* update profiles */
		for(i = 0; i < PP2_MAX_PLAYERS; i++)
		{
			if(pp2_player[i].playing && pp2_client_game->player[i]->local)
			{
				if(i == pp2_winner)
				{
					pp2_profiles.item[pp2_player[i].profile_choice].wins++;
				}
				else
				{
					pp2_profiles.item[pp2_player[i].profile_choice].losses++;
				}
			}
		}

		/* hand out awards */
		for(i = 0; i < PP2_MAX_PLAYERS; i++)
		{
			if(pp2_player[i].playing)
			{
				if(pp2_player[i].total_hits > 0)
				{
					shots_current = (pp2_player[i].shots * 100) / (pp2_player[i].total_hits);
					if(shots_current > shots)
					{
						pp2_award_accuracy = i;
						shots = shots_current;
					}
				}
				hits_current = pp2_player[i].shot;
				if(hits_current > hits)
				{
					pp2_award_fodder = i;
					hits = hits_current;
				}
				triggers_current = pp2_player[i].shots;
				if(triggers_current > triggers)
				{
					pp2_award_trigger = i;
					triggers = triggers_current;
				}
			}
		}
	}
}
