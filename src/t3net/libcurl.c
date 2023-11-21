#include <curl/curl.h>
#include "internal.h"
#include "libcurl.h"

typedef struct
{

	char * data;
	size_t filled;

} T3NET_MEMORY_CHUNK;

static size_t t3net_internal_write_function(void * ptr, size_t size, size_t nmemb, void * stream)
{
	size_t realsize = size * nmemb;
	T3NET_MEMORY_CHUNK * mem = (T3NET_MEMORY_CHUNK *)stream;
	size_t blocks = (mem->filled + 1) / T3NET_DATA_CHUNK_SIZE + 1;
	size_t blocks_required = (realsize + mem->filled + 1) / T3NET_DATA_CHUNK_SIZE + 1;

	/* increase chunk size if we exceed it */
	if(realsize + mem->filled + 1 >= T3NET_DATA_CHUNK_SIZE * blocks)
	{
		mem->data = realloc(mem->data, T3NET_DATA_CHUNK_SIZE * blocks_required);
		if(mem->data == NULL)
		{
  	    	/* out of memory! */
  	    	return 0;
		}
	}
	memcpy(&(mem->data[mem->filled]), ptr, realsize);
	mem->filled += realsize;
	mem->data[mem->filled] = '\0';

	return realsize;
}

char * t3net_libcurl_url_runner(const char * url)
{
	CURL * curl;
	T3NET_MEMORY_CHUNK data;

	data.data = malloc(T3NET_DATA_CHUNK_SIZE);
	if(!data.data)
	{
		goto fail;
	}
	memset(data.data, 0, T3NET_DATA_CHUNK_SIZE);
	data.filled = 0;

  curl = curl_easy_init();
  if(!curl)
  {
    goto fail;
  }
  curl_easy_setopt(curl, CURLOPT_URL, final_url);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, t3net_internal_write_function);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
  curl_easy_setopt(curl, CURLOPT_TIMEOUT, T3NET_TIMEOUT_TIME);
  if(curl_easy_perform(curl))
  {
    curl_easy_cleanup(curl);
    goto fail;
  }
  curl_easy_cleanup(curl);

}
