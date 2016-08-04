#ifndef __VIEW_H
#define __VIEW_H

#include "LCDConf.h"
#include "info.h"


#define P(x,y) x,y

#define INFO_VIEW_START_X 216
#define INFO_VIEW_START_Y 0
#define INFO_VIEW_MARGIN_LEFT 10
#define INFO_VIEW_MARGIN_TOP 10

#define INFO_VIEW_END_X LCD_XSIZE - 2
#define INFO_VIEW_END_Y LCD_YSIZE - 2

#define LINE_INTERVAL  		12
#define SECTION_INTERVAL 	12




void init_ui(void);

void show_info(struct info *h);


#endif

