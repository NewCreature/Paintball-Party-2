#include "scoreboard.h"

int pp2_scoreboard_sorter(const void * item_1, const void * item_2)
{
	PP2_SCOREBOARD_ENTRY * e1 = (PP2_SCOREBOARD_ENTRY *)item_1;
	PP2_SCOREBOARD_ENTRY * e2 = (PP2_SCOREBOARD_ENTRY *)item_2;
	
	if(e1->score < e2->score)
	{
		return 1;
	}
	else if(e2->score < e1->score)
	{
		return -1;
	}
	return 0;
}

