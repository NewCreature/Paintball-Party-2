#include <stdio.h>
#include <time.h>

static char pp2_date_string[1024] = {0};

char * pp2_get_date_string(void)
{
	time_t etime = time(NULL);
	struct tm  * btime;
	btime = localtime(&etime);
	sprintf(pp2_date_string, "%02d-%02d-%d - %02d%02d%02d", btime->tm_mon + 1, btime->tm_mday, 1900 + btime->tm_year, btime->tm_hour, btime->tm_min, btime->tm_sec);
	return pp2_date_string;
}
