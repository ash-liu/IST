#include "view.h"
#include "temp.h"


char temp_buf[TEMP_BUF_LENGTH];
volatile char temp_buf_length = 0;


static volatile char low_temp;
static volatile char high_temp;
static char *weather_buf;

void init_temp()
{
	
}



// Low temp(1B) | High temp(1B) |length(1B) |weather(length B)
int parse_temp()
{
	char low,high,length;

	low = temp_buf[0];
	high = temp_buf[1];
	length = temp_buf[2];

	if (low < LOW_TEMP_MAX || low > HIGH_TEMP_MAX) {
		return 1;
	}
	if (high < LOW_TEMP_MAX || high > HIGH_TEMP_MAX) {
		return 1;
	}

	if (length >= TEMP_BUF_LENGTH-3) {
		return 1;
	}

	low_temp = low;
	high_temp = high;
	weather_buf = temp_buf + 3;
	weather_buf[length] = 0;

	return 0;
}



int loop_temp()
{
	if (temp_buf_length > 0) {
		if (parse_temp() == 0) {
			show_temp(low_temp, high_temp, weather_buf);
		}

		temp_buf_length = 0;
	}
}



