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

static const char content[] = "本汉化只是为了方便用户使用而制作，你可以\n"
"免费使用该软件，我们不对该软件可能存在的\n"
"问题负责.\n\n"
"关于ProICD2，Picmate2004仿真器的任何问题请访\n"
"问我们的网站.";

static void _DemoAntialiasing(void) {
  int i, x1, x2, y1, y2;
  const GUI_FONT *font_old;
  y1 = 65;
  y2 = 5;
  /* Set drawing attributes */
  GUI_SetColor(GUI_WHITE);
  GUI_SetBkColor(GUI_BLACK);
  GUI_SetPenShape(GUI_PS_FLAT);
  GUI_Clear();
  /* draw headline */
  font_old = GUI_SetFont(&GUI_FontHZ12);
  GUI_SetTextAlign(GUI_TA_HCENTER);
  GUI_DispStringAt(content, 300, 240);
  GUI_Delay(7500);
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
  while (1) {
    _DemoAntialiasing();
  }
}

