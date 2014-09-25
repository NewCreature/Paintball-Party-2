#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <mxml.h>
#include "t3net.h"

static char t3net_server_key[1024] = {0};

static void t3net_strcpy(char * dest, char * src)
{
	unsigned int i;
	int write_pos = 0;
	
	for(i = 0; i < strlen(dest) + 1; i++)
	{
		if(src[i] == ' ')
		{
			dest[write_pos] = '%';
			dest[write_pos + 1] = '2';
			dest[write_pos + 2] = '0';
			write_pos += 3;
		}
		else
		{
			dest[write_pos] = src[i];
			write_pos++;
		}
	}
}

static size_t t3net_internal_write_function(void * ptr, size_t size, size_t nmemb, void * stream)
{
	if(stream)
	{
		memcpy(stream, ptr, size * nmemb);
		((char *)stream)[size * nmemb] = 0;
	}
	return size * nmemb;
}

T3NET_SERVER_LIST * t3net_get_server_list(char * url, char * game, char * version)
{
	T3NET_SERVER_LIST * lp;
	
	lp = malloc(sizeof(T3NET_SERVER_LIST));
	if(!lp)
	{
		return NULL;
	}
	lp->entries = 0;
	strcpy(lp->url, url);
	strcpy(lp->game, game);
	strcpy(lp->version, version);
	if(!t3net_update_server_list(lp))
	{
		free(lp);
		return NULL;
	}
	return lp;
}

int t3net_update_server_list(T3NET_SERVER_LIST * lp)
{
	CURL * curl;
	char url_w_arg[1024] = {0};
	char * data = NULL;
	mxml_node_t * top_node = NULL;
	mxml_node_t * current_node = NULL;
	mxml_node_t * walk_node = NULL;
	char * current_element = NULL;
	int loop_out = 0;
	
	if(!lp)
	{
		return 0;
	}
	
	data = malloc(65536);
	if(!data)
	{
		return 0;
	}
	
	/* make HTTP request */
	curl = curl_easy_init();
	if(!curl)
	{
		free(data);
		return 0;
	}
	sprintf(url_w_arg, "%s?game=%s&version=%s", lp->url, lp->game, lp->version);
	curl_easy_setopt(curl, CURLOPT_URL, url_w_arg);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, t3net_internal_write_function);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, data);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, T3NET_TIMEOUT_TIME); // timeout after 10 seconds
    if(curl_easy_perform(curl)) // check for error
    {
		curl_easy_cleanup(curl);
		free(data);
		return 0;
	}
    curl_easy_cleanup(curl);
    
    top_node = mxmlLoadString(NULL, data, NULL);
    if(!top_node)
    {
		free(data);
		return 0;
	}
	
    /* create the server list */
    t3net_clear_server_list(lp);
	for(current_node = mxmlFindElement(top_node, top_node, "server", NULL, NULL, MXML_DESCEND); current_node != NULL; current_node = mxmlFindElement(current_node, top_node, "server", NULL, NULL, MXML_DESCEND))
	{
		walk_node = current_node;
		loop_out = 0;
		while(!loop_out)
		{
			walk_node = mxmlWalkNext(walk_node, top_node, MXML_DESCEND);
			if(walk_node)
			{
				switch(walk_node->type)
				{
					case MXML_CUSTOM:
					{
						break;
					}
					
					/* if we find element "name" start a new entry in the list */
					case MXML_ELEMENT:
					{
						if(!strcasecmp(walk_node->value.element.name, "server"))
						{
							loop_out = 1;
						}
						else if(!strcasecmp(walk_node->value.element.name, "name"))
						{
							lp->entry[lp->entries] = malloc(sizeof(T3NET_SERVER_LIST_ENTRY));
							if(!lp)
							{
								return 0;
							}
							lp->entries++;
							strcpy(lp->entry[lp->entries - 1]->name, "");
							strcpy(lp->entry[lp->entries - 1]->address, "");
							strcpy(lp->entry[lp->entries - 1]->capacity, "");
						}
						current_element = walk_node->value.element.name;
						break;
					}
					case MXML_IGNORE:
					{
						break;
					}
					case MXML_INTEGER:
					{
						break;
					}
					case MXML_OPAQUE:
					{
						break;
					}
					case MXML_REAL:
					{
						break;
					}
					case MXML_TEXT:
					{
						if(current_element && strlen(walk_node->value.text.string) > 0)
						{
							if(!strcasecmp(current_element, "name"))
							{
								if(strlen(lp->entry[lp->entries - 1]->name) > 0)
								{
									strcat(lp->entry[lp->entries - 1]->name, " ");
								}
								strcat(lp->entry[lp->entries - 1]->name, walk_node->value.text.string);
							}
							else if(!strcasecmp(current_element, "ip"))
							{
								if(strlen(lp->entry[lp->entries - 1]->address) > 0)
								{
									strcat(lp->entry[lp->entries - 1]->address, " ");
								}
								strcat(lp->entry[lp->entries - 1]->address, walk_node->value.text.string);
							}
							else if(!strcasecmp(current_element, "capacity"))
							{
								if(strlen(lp->entry[lp->entries - 1]->capacity) > 0)
								{
									strcat(lp->entry[lp->entries - 1]->capacity, " ");
								}
								strcat(lp->entry[lp->entries - 1]->capacity, walk_node->value.text.string);
							}
						}
						break;
					}
				}
			}
			else
			{
				break;
			}
		}
	}
	
	/* free memory */
	mxmlDelete(top_node);
	free(data);
	
	return 1;
}

void t3net_clear_server_list(T3NET_SERVER_LIST * lp)
{
	int i;
	
	for(i = 0; i < lp->entries; i++)
	{
		free(lp->entry[i]);
	}
	lp->entries = 0;
}

void t3net_destroy_server_list(T3NET_SERVER_LIST * lp)
{
	t3net_clear_server_list(lp);
	free(lp);
}

char * t3net_register_server(char * url, char * game, char * version, char * name, char * password)
{
	CURL * curl;
	char * data = NULL;
	char url_w_arg[1024] = {0};
	char tname[256] = {0};
	mxml_node_t * top_node = NULL;
	mxml_node_t * walk_node = NULL;

	data = malloc(65536);
	if(!data)
	{
		return NULL;
	}
	
	/* make HTTP request */
	curl = curl_easy_init();
	if(!curl)
	{
		free(data);
		return NULL;
	}
	t3net_strcpy(tname, name);
	sprintf(url_w_arg, "%s?addServer&game=%s&version=%s&name=%s%s%s", url, game, version, tname, password ? "&password=" : "", password ? password : "");
	curl_easy_setopt(curl, CURLOPT_URL, url_w_arg);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, t3net_internal_write_function);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, data);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, T3NET_TIMEOUT_TIME);
	if(curl_easy_perform(curl))
	{
		curl_easy_cleanup(curl);
		free(data);
		return NULL;
	}
    curl_easy_cleanup(curl);
    
	/* see if we got a key */
    top_node = mxmlLoadString(NULL, data, NULL);
    if(!top_node)
    {
		mxmlDelete(top_node);
		free(data);
		return NULL;
	}
	walk_node = mxmlWalkNext(top_node, top_node, MXML_DESCEND);
	if(!walk_node)
	{
		mxmlDelete(top_node);
		free(data);
		return NULL;
	}
	else if(walk_node->type == MXML_TEXT)
	{
		strcpy(t3net_server_key, walk_node->value.text.string);
	}
	mxmlDelete(top_node);
	free(data);
    return t3net_server_key;
}

int t3net_update_server(char * url, char * key, char * capacity)
{
	CURL * curl;
	char * data = NULL;
	char url_w_arg[1024] = {0};

	data = malloc(65536);
	if(!data)
	{
		return 0;
	}
	
	/* make HTTP request */
	curl = curl_easy_init();
	if(!curl)
	{
		free(data);
		return 0;
	}
	sprintf(url_w_arg, "%s?pollServer&key=%s&capacity=%s", url, key, capacity);
	curl_easy_setopt(curl, CURLOPT_URL, url_w_arg);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, t3net_internal_write_function);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, data);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, T3NET_TIMEOUT_TIME);
    if(curl_easy_perform(curl))
    {
		curl_easy_cleanup(curl);
		free(data);
		return 0;
	}
    curl_easy_cleanup(curl);
    free(data);
    return 1;
}

int t3net_unregister_server(char * url, char * key)
{
	CURL * curl;
//	char * data = NULL;
	char url_w_arg[1024] = {0};

//	data = malloc(65536);
//	if(!data)
//	{
//		return 0;
//	}
	
	/* make HTTP request */
	curl = curl_easy_init();
	if(!curl)
	{
		return 0;
	}
	sprintf(url_w_arg, "%s?removeServer&key=%s", url, key);
	curl_easy_setopt(curl, CURLOPT_URL, url_w_arg);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, t3net_internal_write_function);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, NULL);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, T3NET_TIMEOUT_TIME);
    if(curl_easy_perform(curl))
    {
		curl_easy_cleanup(curl);
		return 0;
	}
    curl_easy_cleanup(curl);
    return 1;
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
		strcpy(lp->entry[i]->name, "");
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
	strcpy(lp->version, version);
	strcpy(lp->mode, mode);
	strcpy(lp->option, option);
	lp->ascend = ascend;
	if(!t3net_update_leaderboard_2(lp))
	{
		t3net_destroy_leaderboard(lp);
		return NULL;
	}
	
	/* query the server */
	return lp;
}

int t3net_update_leaderboard(T3NET_LEADERBOARD * lp)
{
	CURL * curl;
	char url_w_arg[1024] = {0};
	char * data = NULL;
	mxml_node_t * top_node = NULL;
	mxml_node_t * current_node = NULL;
	mxml_node_t * walk_node = NULL;
	char * current_element = NULL;
	int loop_out = 0;
	int ecount = 0;
	int ucount = 0;
	
	if(!lp)
	{
		return 0;
	}
	
	data = malloc(65536);
	if(!data)
	{
		return 0;
	}
	
	/* make HTTP request */
	curl = curl_easy_init();
	if(!curl)
	{
		free(data);
		return 0;
	}
	sprintf(url_w_arg, "%s?game=%s&version=%s&mode=%s&option=%s%s&limit=%d", lp->url, lp->game, lp->version, lp->mode, lp->option, lp->ascend ? "&ascend=true" : "", lp->entries);
	curl_easy_setopt(curl, CURLOPT_URL, url_w_arg);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, t3net_internal_write_function);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, data);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, T3NET_TIMEOUT_TIME);
	if(curl_easy_perform(curl))
	{
		curl_easy_cleanup(curl);
		free(data);
		return 0;
	}
    curl_easy_cleanup(curl);
    
    top_node = mxmlLoadString(NULL, data, NULL);
    if(!top_node)
    {
		free(data);
		return 0;
	}
	
    /* create the server list */
    t3net_clear_leaderboard(lp);
	for(current_node = mxmlFindElement(top_node, top_node, "leaderboard", NULL, NULL, MXML_DESCEND); current_node != NULL; current_node = mxmlFindElement(current_node, top_node, "leaderboard", NULL, NULL, MXML_DESCEND))
	{
		walk_node = current_node;
		loop_out = 0;
		while(!loop_out)
		{
			walk_node = mxmlWalkNext(walk_node, top_node, MXML_DESCEND);
			if(walk_node)
			{
				switch(walk_node->type)
				{
					case MXML_CUSTOM:
					{
						break;
					}
					
					/* if we find element "name" start a new entry in the list */
					case MXML_ELEMENT:
					{
						if(!strcasecmp(walk_node->value.element.name, "leaderboard"))
						{
							loop_out = 1;
						}
						else if(!strcasecmp(walk_node->value.element.name, "name"))
						{
							ecount++;
							strcpy(lp->entry[ecount - 1]->name, "");
							lp->entry[ecount - 1]->score = -1;
						}
						current_element = walk_node->value.element.name;
						break;
					}
					case MXML_IGNORE:
					{
						break;
					}
					case MXML_INTEGER:
					{
						break;
					}
					case MXML_OPAQUE:
					{
						break;
					}
					case MXML_REAL:
					{
						break;
					}
					case MXML_TEXT:
					{
						/* get score for this name */
						if(current_element && strlen(walk_node->value.text.string) > 0)
						{
							/* append extra name elements */
							if(!strcasecmp(current_element, "name"))
							{
								if(strlen(lp->entry[ecount - 1]->name) > 0)
								{
									strcat(lp->entry[ecount - 1]->name, " ");
								}
								strcat(lp->entry[ecount - 1]->name, walk_node->value.text.string);
							}
							
							/* get score */
							else if(!strcasecmp(current_element, "score"))
							{
								/* deal with blank names */
								ucount++;
								while(ecount < ucount)
								{
									ecount++;
									strcpy(lp->entry[ecount - 1]->name, "");
									lp->entry[ecount - 1]->score = -1;
								}
								
								lp->entry[ecount - 1]->score = atoi(walk_node->value.text.string);
								lp->entries++;
							}
						}
						break;
					}
				}
			}
			else
			{
				break;
			}
		}
	}
	
	/* free memory */
	mxmlDelete(top_node);
	free(data);
	
	return 1;
}

int t3net_update_leaderboard_2(T3NET_LEADERBOARD * lp)
{
	CURL * curl;
	char url_w_arg[1024] = {0};
	char * data = NULL;
	int ecount = 0;
	unsigned int text_pos;
	int text_char;
	int text_fill_pos;
	char text[256];
	
	if(!lp)
	{
		return 0;
	}
	
	data = malloc(65536);
	if(!data)
	{
		return 0;
	}
	
	/* make HTTP request */
	curl = curl_easy_init();
	if(!curl)
	{
		free(data);
		return 0;
	}
	sprintf(url_w_arg, "%s?game=%s&version=%s&mode=%s&option=%s%s&limit=%d", lp->url, lp->game, lp->version, lp->mode, lp->option, lp->ascend ? "&ascend=true" : "", lp->entries);
	curl_easy_setopt(curl, CURLOPT_URL, url_w_arg);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, t3net_internal_write_function);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, data);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, T3NET_TIMEOUT_TIME);
	if(curl_easy_perform(curl))
	{
		curl_easy_cleanup(curl);
		free(data);
		return 0;
	}
    curl_easy_cleanup(curl);

	text_pos = 0;
	while(ecount < lp->entries)
	{
		/* read the name */
		text_char = 0;
		text_fill_pos = 0;
		while(text_char != '\n')
		{
			text_char = data[text_pos];
			lp->entry[ecount]->name[text_fill_pos] = text_char;
			text_fill_pos++;
			text_pos++;
		}
		if(text_fill_pos > 0)
		{
			lp->entry[ecount]->name[text_fill_pos - 1] = '\0';
		}
		
		/* read the score */
		text_char = 0;
		text_fill_pos = 0;
		strncpy(text, "", 256);
		while(text_char != '\n')
		{
			text_char = data[text_pos];
			if(text_char != '\t')
			{
				text[text_fill_pos] = text_char;
				text_fill_pos++;
			}
			text_pos++;
		}
		if(text_fill_pos > 0)
		{
			text[text_fill_pos - 1] = '\0';
		}
		lp->entry[ecount]->score = atoi(text);
		ecount++;
		
		/* get out if we've reached the end of the data */
		if(text_pos >= strlen(data))
		{
			break;
		}
	}
	lp->entries = ecount;
	free(data);
	return 1;
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

int t3net_upload_score(char * url, char * game, char * version, char * mode, char * option, char * name, unsigned long score)
{
	CURL * curl;
	char * data = NULL;
	char url_w_arg[1024] = {0};
	char tname[256] = {0};

	data = malloc(65536);
	if(!data)
	{
		return 0;
	}
	
	/* make HTTP request */
	curl = curl_easy_init();
	if(!curl)
	{
		free(data);
		return 0;
	}
	t3net_strcpy(tname, name);
	sprintf(url_w_arg, "%s?uploadScore&game=%s&version=%s&mode=%s&option=%s&name=%s&score=%lu", url, game, version, mode, option, tname, score);
//	printf("%s\n", url_w_arg);
	curl_easy_setopt(curl, CURLOPT_URL, url_w_arg);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, t3net_internal_write_function);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, data);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, T3NET_TIMEOUT_TIME);
    if(curl_easy_perform(curl))
    {
		curl_easy_cleanup(curl);
		free(data);
		return 0;
	}
    curl_easy_cleanup(curl);
    
    return 1;
}
