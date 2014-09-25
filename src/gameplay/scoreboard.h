#ifndef PP2_SCOREBOARD_H
#define PP2_SCOREBOARD_H

typedef struct
{
	
	int player;
	int score;
	
} PP2_SCOREBOARD_ENTRY;

int pp2_scoreboard_sorter(const void * item_1, const void * item_2);

#endif
