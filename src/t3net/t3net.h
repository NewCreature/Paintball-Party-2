#ifndef T3NET_H
#define T3NET_H

#define T3NET_MAX_SERVERS 256
#define T3NET_TIMEOUT_TIME 10

typedef struct
{
	
	char name[256];
	char address[256];
	char capacity[32];
	
} T3NET_SERVER_LIST_ENTRY;

typedef struct
{
	
	char url[1024];
	char game[64];
	char version[64];
	
	T3NET_SERVER_LIST_ENTRY * entry[T3NET_MAX_SERVERS];
	int entries;
	
} T3NET_SERVER_LIST;

typedef struct
{
	
	char name[256];
	unsigned long score;
	
} T3NET_LEADERBOARD_ENTRY;

typedef struct
{
	
	char url[1024];
	char game[64];
	char version[64];
	char mode[64];
	char option[64];
	int ascend;
	
	T3NET_LEADERBOARD_ENTRY ** entry;
	int entries;
	
} T3NET_LEADERBOARD;

/* server list download functions */
T3NET_SERVER_LIST * t3net_get_server_list(char * url, char * game, char * version);
int t3net_update_server_list(T3NET_SERVER_LIST * lp);
void t3net_clear_server_list(T3NET_SERVER_LIST * lp);
void t3net_destroy_server_list(T3NET_SERVER_LIST * lp);

/* server registration functions */
char * t3net_register_server(char * url, char * game, char * version, char * name, char * password);
int t3net_update_server(char * url, char * key, char * capacity);
int t3net_unregister_server(char * url, char * key);

/* leaderboard download functions */
T3NET_LEADERBOARD * t3net_get_leaderboard(char * url, char * game, char * version, char * mode, char * option, int entries, int ascend);
int t3net_update_leaderboard(T3NET_LEADERBOARD * lp);
int t3net_update_leaderboard_2(T3NET_LEADERBOARD * lp);
void t3net_clear_leaderboard(T3NET_LEADERBOARD * lp);
void t3net_destroy_leaderboard(T3NET_LEADERBOARD * lp);

/* leaderboard upload functions */
int t3net_upload_score(char * url, char * game, char * version, char * mode, char * option, char * name, unsigned long score);

#endif
