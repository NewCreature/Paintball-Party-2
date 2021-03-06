#ifndef JOYNET_STRUCTS_H
#define JOYNET_STRUCTS_H

#define JOYNET_MAX_CHANNELS  16

typedef struct
{

	char * data;    // this is the original pointer
	char * pointer; // this is the fill pointer (points into the above)
	int size;

} JOYNET_SERIAL_DATA;

typedef struct
{

	/* decoded data */
	int type;
	char * data;
	int data_size;

	/* in case we need it */
	ENetEvent * event; // more info about the event

} JOYNET_MESSAGE;

typedef struct
{

	/* network access */
	ENetAddress address;
	ENetHost * host;
	ENetPeer * peer;

	/* chat info */
	char screen_name[256];
	short user;
	short group;

	/* game info */
	int master;
	int playing;
	int spectating;

	/* callbacks */
	int(*global_callback)(ENetEvent * ep, void * data);
	int(*channel_callback[JOYNET_MAX_CHANNELS])(JOYNET_MESSAGE * mp, void * data);
	int(*chat_callback)(char * user, char * message, void * data);
	int(*voip_callback)(int client, void * data, int size);

	/* serialization */
	JOYNET_SERIAL_DATA * serial_data;

	void * global_user_data;
	void * channel_user_data[JOYNET_MAX_CHANNELS];
	void * chat_user_data;

} JOYNET_CLIENT;

typedef struct
{

	/* network access stuff */
	ENetAddress address;
	ENetHost * host;
	ENetEvent event;

	/* server data */
	char id[256];

	/* clients */
	JOYNET_CLIENT ** client;
	int max_clients;

	/* callbacks */
	int(*global_callback)(ENetEvent * ep, void * data);
	int(*internal_callback)(ENetEvent * ep);
	int(*channel_callback[JOYNET_MAX_CHANNELS])(JOYNET_MESSAGE * mp, void * data);
	int(*voice_chat_callback)();

	/* serialization */
	JOYNET_SERIAL_DATA * serial_data;

	void * global_user_data;
	void * channel_user_data[JOYNET_MAX_CHANNELS];

} JOYNET_SERVER;

#endif
