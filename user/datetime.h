#ifndef __DATETIME_H
#define __DATETIME_H


#define DATETIME_BUF_LENGTH  100



extern char datetime_buf[DATETIME_BUF_LENGTH];
extern volatile char datetime_buf_length;
extern char volatile update_day_flag;

void init_datetime(void);
void datetime_1s_irq(void);
int loop_datetime(void);



#endif
