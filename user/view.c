
#include "view.h"
#include "GUI.h"
#include "LCDConf.h"
#include "GUI_Protected.h"
#include "info.h"
#include <stdio.h>

void GUI_DispStringInRectWrap(const char GUI_UNI_PTR * s, GUI_RECT * pRect, int TextAlign, GUI_WRAPMODE WrapMode, int line_interval);

//for test
static const char info1[]="1. 从7月28日18:00到7月29日18:00停电,请注意时间.\n";
static const char info2[]="2. 气温过高,请注意防暑.\n";
static const char info3[]="3. 当月电费为88元.\n";
//---------


void init_ui()
{
	GUI_RECT rect;

	/* Set drawing attributes */
	GUI_SetColor(GUI_WHITE);
	GUI_SetBkColor(GUI_BLACK);
	GUI_SetPenShape(GUI_PS_FLAT);
	GUI_Clear();
	
	/* draw headline */
	//font_old = GUI_SetFont(&GUI_FontHZ42);
	GUI_SetTextAlign(GUI_TA_LEFT);
	GUI_SetTextMode(GUI_TM_NORMAL);

	// line
	GUI_SetColor(GUI_YELLOW);	
	GUI_DrawLine(P(0,0), P(LCD_XSIZE-1,0));
	GUI_DrawLine(P(LCD_XSIZE-1,0), P(LCD_XSIZE-1,LCD_YSIZE-1));
	GUI_DrawLine(P(0,LCD_YSIZE-1), P(LCD_XSIZE-1,LCD_YSIZE-1));
	GUI_DrawLine(P(0,0), P(0,LCD_YSIZE-1));
	
	
	GUI_FillRect(P(213,0), P(215,480));
	GUI_FillRect(P(0,239), P(213,241));

	//text data
	GUI_SetColor(GUI_WHITE);
	GUI_DispStringAt("2016/07/28", 10, 0+10);
	GUI_DispStringAt("星期四", 10, 48+10);
	GUI_DispStringAt("22:55", 10, 48*2+10);

	//text 
	GUI_SetColor(GUI_WHITE);
	GUI_DispStringAt("晴转多云", 10, 0+10+242);
	GUI_DispStringAt("29度/39度", 10, 48+10+242);
	GUI_DispStringAt("室温:32度", 10, 48*2+10+242);

	// text
	GUI_SetLBorder(216+10+18*3);
	GUI_SetColor(GUI_RED);
	//GUI_DispStringAt(info1, 216+10, 10);
	rect.x0 = 216+10;
	rect.y0 = 10;
	rect.x1 = LCD_XSIZE - 2;
	rect.y1 = LCD_YSIZE - 2;
	GUI_DispStringInRectWrap(info1, &rect, GUI_TA_LEFT, GUI_WRAPMODE_CHAR, LINE_INTERVAL);
	rect.x0 = 216+10;;
	rect.y0 = GUI_GetDispPosY() + 48;
	GUI_DispStringInRectWrap(info1, &rect, GUI_TA_LEFT, GUI_WRAPMODE_CHAR, LINE_INTERVAL);
	//GUI_SetColor(GUI_LIGHTBLUE);
	//GUI_DispStringAt(info2, 216+10, 10+48);
	//GUI_SetColor(GUI_LIGHTGRAY);
	//GUI_DispStringAt(info3, 216+10, 10+48*2);	
}



void show_info(struct info *h)
{
	int level;
	struct info *point;
	int count=1;
	char count_buf[5];
	GUI_RECT rect;

	rect.x0 = INFO_VIEW_START_X + INFO_VIEW_MARGIN_LEFT + 18*3;
	rect.y0 = INFO_VIEW_START_Y + INFO_VIEW_MARGIN_TOP;
	rect.x1 = INFO_VIEW_END_X;
	rect.y1 = INFO_VIEW_END_Y;

	//clear rect
	GUI_ClearRect(rect.x0 - 18*3, rect.y0, rect.x1, rect.y1);

	for (level=INFO_LEVEL_CRITICAL; level>=INFO_LEVEL_NORMAL; level--) {

		// set test color
		switch(level) {
			case INFO_LEVEL_CRITICAL:
				GUI_SetColor(GUI_RED);
				break;
			case INFO_LEVEL_WARING:
				GUI_SetColor(GUI_LIGHTBLUE);
				break;
			case INFO_LEVEL_NORMAL:
				GUI_SetColor(GUI_LIGHTGRAY);
				break;

			default: break;
		}

		// show info with the level 
		point = h;
		while (point != NULL) {
			if (point->level == level) {
				sprintf(count_buf, "%d. ", count++);
				GUI_DispStringAt(count_buf, P(rect.x0-18*3, rect.y0));
				GUI_DispStringInRectWrap(point->content, &rect, GUI_TA_LEFT, GUI_WRAPMODE_CHAR, LINE_INTERVAL);

				rect.y0 = GUI_GetDispPosY() + GUI_GetFontDistY() + SECTION_INTERVAL;
			}

			point = point->next;
		}
	}
}



void show_time()
{

}




