/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              礐/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : AA_Lines.c
Purpose     : Shows lines with different antialiasing qualities
----------------------------------------------------------------------
*/

#include "GUI.h"
#include "stdlib.h"

/*******************************************************************
*
*       static code
*
********************************************************************
*/

/*******************************************************************
*
*       _DemoAntialiasing

  Draws lines with different antialiasing factors
*/

static const char content_hz[] = "本汉化只是为了方便测试字体的大小胖瘦高矮美丑善恶\n";
static const char content_asc[] = "1234567890abcdefABCDEf[]~+-@#$%&*";

static const char info1[]="1. 从7月28日18:00到7月29日18:00停电.\n";
static const char info2[]="2. 气温过高,请注意防暑.\n";
static const char info3[]="3. 当月电费为88元.\n";

static void _DemoAntialiasing(void) {
	int i, x1, x2, y1, y2;
	const GUI_FONT *font_old;
	char *buf;
	y1 = 65;
	y2 = 5;
	/* Set drawing attributes */
	GUI_SetColor(GUI_WHITE);
	GUI_SetBkColor(GUI_BLACK);
	GUI_SetPenShape(GUI_PS_FLAT);
	GUI_Clear();
	/* draw headline */
	//font_old = GUI_SetFont(&GUI_FontHZ42);
	GUI_SetTextAlign(GUI_TA_LEFT);

	GUI_SetTextMode(GUI_TM_NORMAL);
	//GUI_DispStringAt(content_hz, 0, 0);
	//GUI_DispString(content_asc);

/*
	buf = (char *)malloc(512);
	sprintf(buf, "%s", content_hz);
	GUI_DispStringAt(buf, 0, 0);
	sprintf(buf, "%s\n", content_asc);
	GUI_DispString(buf);
	sprintf(buf, "温度:%02d-_%02d", 28, 38);
	GUI_DispString(buf);
	free(buf);
  */

	// line
	GUI_SetColor(GUI_YELLOW);	
	GUI_DrawLine(0, 0, 853, 0);
	GUI_DrawLine(0, 0, 0, 479);
	GUI_DrawLine(853, 0, 853, 479);
	GUI_DrawLine(0, 479, 853, 479);

	GUI_FillRect(213, 0, 215, 480);
	GUI_FillRect(0, 239, 213, 241);

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
	GUI_SetColor(GUI_RED);
	GUI_DispStringAt(info1, 216+10, 10);
	GUI_SetColor(GUI_LIGHTBLUE);
	GUI_DispStringAt(info2, 216+10, 10+48);
	GUI_SetColor(GUI_LIGHTGRAY);
	GUI_DispStringAt(info3, 216+10, 10+48*2);
	
	
  
	//GUI_Delay(15000);


}

/*******************************************************************
*
*       MainTask
*
*       Show lines with different antialiasing qualities
*
********************************************************************
*/

void MainTask(void) {
  GUI_Init();
//  while (1) {
   _DemoAntialiasing();
//  }
}

