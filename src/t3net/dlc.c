#include <curl/curl.h>
#include <memory.h>
#include <stdlib.h>
#include "t3net.h"
#include "dlc.h"

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

static int t3net_dlc_read_char(const char * buffer, int max, int pos)
{
	int r = -1;

	if(pos < max)
	{
		r = buffer[pos];
	}
	return r;
}

T3NET_DLC_LIST * t3net_get_dlc_list(const char * url, const char * game, int type)
{
	T3NET_DLC_LIST * lp = NULL;
	CURL *curl;
	char url_w_arg[1024] = {0};
	char * data = NULL;
	int loop_out = 0;
	int ecount = 0;
	unsigned int text_pos;
	int text_char;
	int text_fill_pos;
	char buf[256] = {0};
	int text_max = 0;
	int ret = 0;

	lp = malloc(sizeof(T3NET_DLC_LIST));
	if(!lp)
	{
		goto fail_out;
	}
	lp->items = 0;
	
	data = malloc(65536);
	if(!data)
	{
		goto fail_out;
	}
	memset(data, 0, 65536);
	
	/* get info from url */
	curl = curl_easy_init();
	if(!curl)
	{
		goto fail_out;
	}
	t3net_written = 0;
	sprintf(url_w_arg, "%s?project_id=%s&type=%d", url, game, type);
	curl_easy_setopt(curl, CURLOPT_URL, url_w_arg);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, t3net_internal_write_function);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, data);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, T3NET_TIMEOUT_TIME); // timeout after 10 seconds
	ret = curl_easy_perform(curl);
    if(ret) // check for error
    {
		curl_easy_cleanup(curl);
		goto fail_out;
	}
    curl_easy_cleanup(curl);
//    printf("%s\n", data);
    
    /* create the DLC list */
    text_pos = 0;
    text_max = strlen(data);
	while(!loop_out)
	{
		lp->item[ecount] = malloc(sizeof(T3NET_DLC_ITEM));
		memset(lp->item[ecount], 0, sizeof(T3NET_DLC_ITEM));
		/* read the name */
		text_char = 0;
		text_fill_pos = 0;
		while(text_char != '\n' && text_char != -1)
		{
			text_char = t3net_dlc_read_char(data, text_max, text_pos);
			lp->item[ecount]->name[text_fill_pos] = text_char;
			text_fill_pos++;
			text_pos++;
		}
		if(text_fill_pos > 0)
		{
			lp->item[ecount]->name[text_fill_pos - 1] = '\0';
		}
		
		/* read the author */
		text_char = 0;
		text_fill_pos = 0;
		while(text_char != '\n' && text_char != -1)
		{
			text_char = t3net_dlc_read_char(data, text_max, text_pos);
			if(text_char != '\t')
			{
				lp->item[ecount]->author[text_fill_pos] = text_char;
				text_fill_pos++;
			}
			text_pos++;
		}
		if(text_fill_pos > 0)
		{
			lp->item[ecount]->author[text_fill_pos - 1] = '\0';
		}

		/* read the URL */
		text_char = 0;
		text_fill_pos = 0;
		while(text_char != '\n' && text_char != -1)
		{
			text_char = t3net_dlc_read_char(data, text_max, text_pos);
			if(text_char != '\t')
			{
				lp->item[ecount]->url[text_fill_pos] = text_char;
				text_fill_pos++;
			}
			text_pos++;
		}
		if(text_fill_pos > 0)
		{
			lp->item[ecount]->url[text_fill_pos - 1] = '\0';
		}

		/* read the preview URL */
		text_char = 0;
		text_fill_pos = 0;
		while(text_char != '\n' && text_char != -1)
		{
			text_char = t3net_dlc_read_char(data, text_max, text_pos);
			if(text_char != '\t')
			{
				lp->item[ecount]->preview_url[text_fill_pos] = text_char;
				text_fill_pos++;
			}
			text_pos++;
		}
		if(text_fill_pos > 0)
		{
			lp->item[ecount]->preview_url[text_fill_pos - 1] = '\0';
		}

		/* read the hash */
		text_char = 0;
		text_fill_pos = 0;
		while(text_char != '\n' && text_char != -1)
		{
			text_char = t3net_dlc_read_char(data, text_max, text_pos);
			if(text_char != '\t')
			{
				buf[text_fill_pos] = text_char;
				text_fill_pos++;
			}
			text_pos++;
		}
		if(text_fill_pos > 0)
		{
			buf[text_fill_pos - 1] = '\0';
		}
		lp->item[ecount]->hash = atoi(buf);
		ecount++;
		
		/* get out if we've reached the end of the data */
		if(text_pos >= strlen(data))
		{
			loop_out = 1;
			break;
		}
	}
	if(text_max > 0)
	{
		lp->items = ecount;
	}
	else
	{
		lp->items = 0;
	}
	
	/* free memory */
	free(data);
	
	return lp;
	
	fail_out:
	{
		if(lp)
		{
			free(lp);
		}
		if(data)
		{
			free(data);
		}
	}
	return NULL;
}

void t3net_destroy_dlc_list(T3NET_DLC_LIST * lp, void (*callback)(void * data))
{
	int i;
	
	for(i = 0; i < lp->items; i++)
	{
		if(lp->item[i]->preview && callback)
		{
			callback(lp->item[i]->preview);
		}
		free(lp->item[i]);
	}
	free(lp);
}

/* easy way to remove DLC items we already have from the list */
void t3net_remove_dlc_item(T3NET_DLC_LIST * lp, unsigned long hash)
{
	int i, j;
	
	for(i = 0; i < lp->items; i++)
	{
		if(lp->item[i]->hash == hash)
		{
			free(lp->item[i]);
			for(j = i; j < lp->items - 1; j++)
			{
				lp->item[j] = lp->item[j + 1];
			}
			lp->items--;
			return;
		}
	}
}

static void (*t3net_download_callback)() = NULL;

void t3net_set_download_callback(void (*callback)())
{
	t3net_download_callback = callback;
}

static size_t t3net_file_write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	size_t written;
	written = fwrite(ptr, size, nmemb, stream);
	if(t3net_download_callback)
	{
		t3net_download_callback();
	}
	return written;
}

int t3net_download_file(const char * url, const char * fn)
{
	CURL *curl;
	FILE * fp;
	CURLcode res;
	curl = curl_easy_init();
    if(curl)
    {
		fp = fopen(fn, "wb");
		if(fp)
		{
			curl_easy_setopt(curl, CURLOPT_URL, url);
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION,   1); // follow redirects
			curl_easy_setopt(curl, CURLOPT_AUTOREFERER,      1); // set the Referer: field in requests where it follows a Location: redirect. 
			curl_easy_setopt(curl, CURLOPT_MAXREDIRS,        20);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, t3net_file_write_data);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
			res = curl_easy_perform(curl);
			curl_easy_cleanup(curl);
			fclose(fp);
			if(res == 0)
			{
				return 1;
			}
		}
    }
	return 0;
}
