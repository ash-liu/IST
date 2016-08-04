#ifndef __INFO_H
#define __INFO_H

// format: Level(1B) | sid(2B) | length(2B) | Content(length B)


#define INFO_BUF_LENGTH	1000
#define MAX_INFO_CONTENT_LENGTH  400
#define MAX_INFO 10

#define INFO_LEVEL_BYTES	1
#define INFO_SID_BYTES		2
#define INFO_LENGTH_BYTES	2

#define NULL 0

struct info {
	int sid; // unique ID 
	char level;
	char checked;
	char *content;
	struct info *next;
};

enum{
	INFO_LEVEL_NORMAL = '1',
	INFO_LEVEL_WARING,
	INFO_LEVEL_CRITICAL,
};


extern char info_buf[INFO_BUF_LENGTH];
extern volatile int info_buf_length;


void init_info(void);
void loop_info(void);

#endif

