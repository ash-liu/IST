#include "GUI.h"

extern int GUIPROP_X_GetCharDistX(U16P c);//声明部分
extern void GUIPROP_X_DispChar(U16P c) ;


GUI_CONST_STORAGE GUI_CHARINFO GUI_FontHZ42_CharInfo[2] = {
    { 18, 	18,	3,	(void *)"ASC36x18.bin" },      //&ASC_ROM_6X12 字库的路径
    { 36,	36,	5,	(void*)"HZ36x36.bin"  },      //&HZ_ROM_12X12字库的路径
};

GUI_CONST_STORAGE GUI_FONT_PROP GUI_FontHZ42_PropHZ= {
      0xA1A1, //第一个字符
      0xF7FE, //最后一个字符
      &GUI_FontHZ42_CharInfo[1], //字符信息
      (void *)0,
};

GUI_CONST_STORAGE  GUI_FONT_PROP GUI_FontHZ42_PropASC= {
      0x0000, //第一个字符
      0x007F, //最后一个字符
      &GUI_FontHZ42_CharInfo[0], //字符信息
      (void GUI_CONST_STORAGE *)&GUI_FontHZ42_PropHZ,
};

GUI_CONST_STORAGE  GUI_FONT GUI_FontHZ42 = {
	GUI_FONTTYPE_PROP_X,
	36, //xsize
	36, //yszie
	1,  //x方向放大倍数
	1,  //y方向放大倍数
	(void GUI_CONST_STORAGE *)&GUI_FontHZ42_PropASC
};


