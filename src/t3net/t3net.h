#ifndef T3NET_H
#define T3NET_H

#define T3NET_TIMEOUT_TIME      10
#define T3NET_MAX_ARGUMENTS    256

/* A chunk of memory this size will be allocated when retrieving data. Each
   time the size of the data exceeds this chunk size, the chunk will be
   reallocated to add this many bytes to the chunk. */
#define T3NET_DATA_CHUNK_SIZE 4096

typedef struct
{

    char * name;
    char * data;

} T3NET_DATA_ENTRY_FIELD;

typedef struct
{

    T3NET_DATA_ENTRY_FIELD ** field;
    int fields;

} T3NET_DATA_ENTRY;

typedef struct
{

    char * header;
    T3NET_DATA_ENTRY ** entry;
    int entries;

} T3NET_DATA;

typedef struct
{

    char * key[T3NET_MAX_ARGUMENTS];
    char * val[T3NET_MAX_ARGUMENTS];
    int count;

} T3NET_ARGUMENTS;

extern char t3net_server_message[1024];

/* initialization */
int t3net_setup(char * (*url_runner)(const char * url), const char * temp_dir);
const char * t3net_get_curl_command(void);
char * t3net_escape(const char * s);
char * t3net_load_file(const char * fn);

int t3net_open_log_file(const char * fn);
void t3met_close_log_file(void);

/* internet operations */
T3NET_ARGUMENTS * t3net_create_arguments(void);
void t3net_destroy_arguments(T3NET_ARGUMENTS * arguments);
int t3net_add_argument(T3NET_ARGUMENTS * arguments, const char * key, const char * val);
char * t3net_get_raw_data(const char * url, const T3NET_ARGUMENTS * arguments);
T3NET_DATA * t3net_get_data_from_string(const char * s);
T3NET_DATA * t3net_get_data(const char * url, const T3NET_ARGUMENTS * arguments);
void t3net_destroy_data(T3NET_DATA * data);

/* data set operations */
const char * t3net_get_error(T3NET_DATA * data);
const char * t3net_get_data_entry_field(T3NET_DATA * data, int entry, const char * field_name);

#endif
