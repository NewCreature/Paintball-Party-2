#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "t3net.h"

static char t3net_server_key[1024] = {0};

static void t3net_strcpy(char * dest, char * src)
{
	int i;
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

static int t3net_written = 0;
static size_t t3net_internal_write_function(void * ptr, size_t size, size_t nmemb, void * stream)
{
	char * str = (char *)stream;
	if(str)
	{
		memcpy(&str[t3net_written], ptr, size * nmemb);
	}
	t3net_written += size * nmemb;
	return size * nmemb;
}

static int t3net_read_line(const char * data, char * output, int data_max, int output_max, unsigned int * text_pos)
{
	int outpos = 0;
	int c;
	
	while(1)
	{
		c = data[*text_pos];
		if(c != '\n')
		{
			output[outpos] = c;
		}
		else
		{
			output[outpos] = '\0';
			(*text_pos)++;
			return 1;
		}
		outpos++;
		if(outpos >= output_max - 1)
		{
			output[outpos] = '\0';
			return 1;
		}
		(*text_pos)++;
		if(*text_pos >= data_max)
		{
			return 0;
		}
	}
	return 0;
}

typedef struct
{

	char name[256];
	char data[256];

} T3NET_TEMP_ELEMENT;

static int t3net_get_element(const char * data, T3NET_TEMP_ELEMENT * element, int data_max)
{
	int outpos = 0;
	int c;
	int read_pos = 1; // skip first byte

	/* read element name */
	while(1)
	{
		c = data[read_pos];
		
		if(c == ':')
		{
			read_pos += 2;
			break;
		}
		else
		{
			element->name[outpos] = c;
			outpos++;
			element->name[outpos] = '\0';
			read_pos++;
		}
	}
	
	/* read element data */
	outpos = 0;
	while(c != '\0' && read_pos < data_max)
	{
		c = data[read_pos];
		
		element->data[outpos] = c;
		outpos++;
		element->data[outpos] = '\0';
		read_pos++;
	}
	return 1;
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
	if(!t3net_update_server_list_2(lp))
	{
		free(lp);
		return NULL;
	}
	return lp;
}

int t3net_update_server_list_2(T3NET_SERVER_LIST * lp)
{
	CURL * curl;
	char url_w_arg[1024] = {0};
	char * data = NULL;
	int ecount = 0;
	unsigned int text_pos;
	int text_max;
	char text[256];
	T3NET_TEMP_ELEMENT element;
	
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
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, T3NET_TIMEOUT_TIME);
	t3net_written = 0;
	if(curl_easy_perform(curl))
	{
		curl_easy_cleanup(curl);
		free(data);
		return 0;
	}
    curl_easy_cleanup(curl);
	data[t3net_written] = 0;

	/* check for error */
	if(!strncmp(data, "Error", 5))
	{
		return 0;
	}

	text_pos = 0;
    text_max = strlen(data);
    
    /* skip first two lines */
    t3net_read_line(data, text, text_max, 256, &text_pos);
    t3net_read_line(data, text, text_max, 256, &text_pos);
	while(ecount < T3NET_MAX_SERVERS)
	{
		lp->entry[ecount] = malloc(sizeof(T3NET_SERVER_LIST_ENTRY));
		if(lp->entry[ecount])
		{
			memset(lp->entry[ecount], 0, sizeof(T3NET_SERVER_LIST_ENTRY));
			t3net_read_line(data, text, text_max, 256, &text_pos);
			t3net_get_element(text, &element, text_max);
			if(!strcmp(element.name, "name"))
			{
				strcpy(lp->entry[ecount]->name, element.data);
			}
			
			t3net_read_line(data, text, text_max, 256, &text_pos);
			t3net_get_element(text, &element, text_max);
			if(!strcmp(element.name, "ip"))
			{
				strcpy(lp->entry[ecount]->address, element.data);
			}

			t3net_read_line(data, text, text_max, 256, &text_pos); // game
			t3net_read_line(data, text, text_max, 256, &text_pos); // gametype
			t3net_read_line(data, text, text_max, 256, &text_pos); // mod

			t3net_read_line(data, text, text_max, 256, &text_pos);
			t3net_get_element(text, &element, text_max);
			if(!strcmp(element.name, "capacity"))
			{
				strcpy(lp->entry[ecount]->capacity, element.data);
			}

			t3net_read_line(data, text, text_max, 256, &text_pos); // tags
			t3net_read_line(data, text, text_max, 256, &text_pos); // map
			t3net_read_line(data, text, text_max, 256, &text_pos); // state
			t3net_read_line(data, text, text_max, 256, &text_pos); // private

			ecount++;
		}
		
		/* get out if we've reached the end of the data */
		if(text_pos >= text_max)
		{
			break;
		}
	}
	lp->entries = ecount;
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
	int i;

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
	t3net_written = 0;
	if(curl_easy_perform(curl))
	{
		curl_easy_cleanup(curl);
		free(data);
		return NULL;
	}
    curl_easy_cleanup(curl);
	data[t3net_written] = 0;
    
	/* see if we got a key */
	t3net_server_key[0] = 0;
	if(!strncmp(data, "key=", 4))
	{
		for(i = 4; i < strlen(data); i++)
		{
			t3net_server_key[i - 4] = data[i];
		}
	}
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
	t3net_written = 0;
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
	t3net_written = 0;
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

int t3net_update_leaderboard_2(T3NET_LEADERBOARD * lp)
{
	CURL * curl;
	char url_w_arg[1024] = {0};
	char * data = NULL;
	int ecount = 0;
	unsigned int text_pos;
	int text_max;
	char text[256];
	T3NET_TEMP_ELEMENT element;
	
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
	t3net_written = 0;
	if(curl_easy_perform(curl))
	{
		curl_easy_cleanup(curl);
		free(data);
		return 0;
	}
    curl_easy_cleanup(curl);
	data[t3net_written] = 0;
	
	/* check for error */
	if(!strncmp(data, "Error", 5))
	{
		return 0;
	}

	text_pos = 0;
    text_max = strlen(data);
    
    /* skip first two lines */
    t3net_read_line(data, text, text_max, 256, &text_pos);
    t3net_read_line(data, text, text_max, 256, &text_pos);
	while(ecount < lp->entries)
	{
		t3net_read_line(data, text, text_max, 256, &text_pos);
		t3net_get_element(text, &element, text_max);
		if(!strcmp(element.name, "name"))
		{
			strcpy(lp->entry[ecount]->name, element.data);
		}
		
		t3net_read_line(data, text, text_max, 256, &text_pos);
		t3net_get_element(text, &element, text_max);
		if(!strcmp(element.name, "score"))
		{
			lp->entry[ecount]->score = atoi(element.data);
		}

		ecount++;
		
		/* get out if we've reached the end of the data */
		if(text_pos >= text_max)
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

int t3net_upload_score(char * url, char * game, char * version, char * mode, char * option, char * name, unsigned long score, char * extra)
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
	if(extra)
	{
		strcat(url_w_arg, "&extra=");
		strcat(url_w_arg, extra);
	}
//	printf("%s\n", url_w_arg);
	curl_easy_setopt(curl, CURLOPT_URL, url_w_arg);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, t3net_internal_write_function);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, data);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, T3NET_TIMEOUT_TIME);
	t3net_written = 0;
    if(curl_easy_perform(curl))
    {
		curl_easy_cleanup(curl);
		free(data);
		return 0;
	}
    curl_easy_cleanup(curl);
    
    return 1;
}
