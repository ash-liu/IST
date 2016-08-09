#ifndef __TEMP_H
#define __TEMP_H




#define TEMP_BUF_LENGTH 50
#define LOW_TEMP_MAX -50
#define HIGH_TEMP_MAX 60


extern char temp_buf[TEMP_BUF_LENGTH];
extern volatile char temp_buf_length;



void init_temp(void);
int loop_temp(void);



#endif

