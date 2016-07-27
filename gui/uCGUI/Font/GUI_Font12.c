#include "GUI.h"

extern int GUIPROP_X_GetCharDistX(U16P c);//��������
extern void GUIPROP_X_DispChar(U16P c) ;


GUI_CONST_STORAGE GUI_CHARINFO GUI_FontHZ12_CharInfo[2] = {
    { 8, 	8,	1,	(void *)"ASC12.bin" },      //&ASC_ROM_6X12 �ֿ��·��
    { 12,	12,	2,	(void*)"HZ12x12.bin"  },      //&HZ_ROM_12X12�ֿ��·��
};

GUI_CONST_STORAGE GUI_FONT_PROP GUI_FontHZ12_PropHZ= {
      0xA1A1, //��һ���ַ�
      0xF7FE, //���һ���ַ�
      &GUI_FontHZ12_CharInfo[1], //�ַ���Ϣ
      (void *)0,
};

GUI_CONST_STORAGE  GUI_FONT_PROP GUI_FontHZ12_PropASC= {
      0x0000, //��һ���ַ�
      0x007F, //���һ���ַ�
      &GUI_FontHZ12_CharInfo[0], //�ַ���Ϣ
      (void GUI_CONST_STORAGE *)&GUI_FontHZ12_PropHZ,
};

GUI_CONST_STORAGE  GUI_FONT GUI_FontHZ12 = {
	GUI_FONTTYPE_PROP_X,
	12, //xsize
	12, //yszie
	1,  //x����Ŵ���
	1,  //y����Ŵ���
	(void GUI_CONST_STORAGE *)&GUI_FontHZ12_PropASC
};

