#include "GUI.h"

extern int GUIPROP_X_GetCharDistX(U16P c);//声明部分
extern void GUIPROP_X_DispChar(U16P c) ;


GUI_CONST_STORAGE GUI_CHARINFO GUI_FontHZ12_CharInfo[2] = {
    { 8, 	8,	1,	(void *)"ASC12.bin" },      //&ASC_ROM_6X12 字库的路径
    { 12,	12,	2,	(void*)"HZ12x12.bin"  },      //&HZ_ROM_12X12字库的路径
};

GUI_CONST_STORAGE GUI_FONT_PROP GUI_FontHZ12_PropHZ= {
      0xA1A1, //第一个字符
      0xF7FE, //最后一个字符
      &GUI_FontHZ12_CharInfo[1], //字符信息
      (void *)0,
};

GUI_CONST_STORAGE  GUI_FONT_PROP GUI_FontHZ12_PropASC= {
      0x0000, //第一个字符
      0x007F, //最后一个字符
      &GUI_FontHZ12_CharInfo[0], //字符信息
      (void GUI_CONST_STORAGE *)&GUI_FontHZ12_PropHZ,
};

GUI_CONST_STORAGE  GUI_FONT GUI_FontHZ12 = {
	GUI_FONTTYPE_PROP_X,
	12, //xsize
	12, //yszie
	1,  //x方向放大倍数
	1,  //y方向放大倍数
	(void GUI_CONST_STORAGE *)&GUI_FontHZ12_PropASC
};


