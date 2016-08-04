#ifndef __CMD_H
#define __CMD_H


// the cmd through M3 and 8266
// cmd(1B) | length(2B) | content(length B)
#define LENGTH_BYTES 2


enum{
	CMD_INFO = 0xA0,
	CMD_TIME,
	CMD_TEMP,
	CMD_SYSTEM,
};


enum {
	IDLE,
	GET_CMD_TYPE,
	GET_LENGTH,
	FINISH,
};


#define CMD_RECEIVE_LENGTH 500


void receive_cmd_irq(void);
void timer_overtime(void);
int init_cmd(void);


#endif

