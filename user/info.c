
#include "info.h"
#include <stdlib.h>
#include <string.h>
#include "GUI.h"
#include "view.h"

char info_buf[INFO_BUF_LENGTH]; 
volatile int info_buf_length = 0;
struct info *head;

// return 0 : OK 
// return 1: NG
int get_info_level(char *buf, char *level)
{
	if ((*buf >= INFO_LEVEL_NORMAL) && (*buf <= INFO_LEVEL_CRITICAL)){
		*level = *buf;
		return 0;
	}

	return 1;
}


int get_info_sid(char *buf, int *sid)
{
	*sid = *buf;
	*sid <<= 8;
	*sid += *(buf+1);

	return 0;
}

int get_info_length(char *buf, int *length)
{
	*length = *buf;
	*length <<= 8;
	*length += *(buf+1);

	if(*length > MAX_INFO_CONTENT_LENGTH) {
		return 1;
	}
	
	return 0;
}


// format: Level(1B) | sid(2B) | length(2B) | Content(length B)
int parse_info_list(struct info **h)
{
	char level;
	int sid, length;
	int point=0;
	char *content;
	struct info *info;

	while(point < info_buf_length) {
		// get level 
		if(point + INFO_LEVEL_BYTES > info_buf_length) {
			return 1;
		}
		if(get_info_level(info_buf+point, &level) == 1) {
			return 1;
		} 
		point += INFO_LEVEL_BYTES;

		// get sid
		if(point + INFO_SID_BYTES > info_buf_length) {
			return 1;
		}
		if(get_info_sid(info_buf+point, &sid) == 1) {
			return 1;
		}
		point += INFO_SID_BYTES;

		// get length
		if(point + INFO_LENGTH_BYTES > info_buf_length) {
			return 1;
		}
		if(get_info_length(info_buf+point, &length) == 1) {
			return 1;
		}
		point += INFO_LENGTH_BYTES;

		//get content
		if(point + length > info_buf_length) {
			return 1;
		}
		content = (char *)malloc(length);
		memcpy(content, info_buf+point, length);
		// MUST BE STRING !!
		content[length] = 0;	
		point += length;

		//build info
		info = (struct info *)malloc(sizeof(struct info));
		info->sid = sid;
		info->level = level;
		info->checked = 0;
		info->content = content;
		info->next = NULL;

		// add to list
		info->next = *h;
		*h = info;
	}

	return 0;
}




int free_info_list(struct info *h)
{
	struct info *tmp;
	
	while(h != NULL) {
		tmp = h;
		free(h->content);
		h = h->next;
		free(tmp);
	}

	return 0;
}


void init_info()
{

}

void loop_info()
{
	struct info *tmp_head = NULL;
	
	if (info_buf_length > 0) {
		if (parse_info_list(&tmp_head) == 0) {
			free_info_list(head);
			head = tmp_head;
			show_info(head);
		}
		else {
			free_info_list(tmp_head);
		}

		info_buf_length = 0;
	}
}

