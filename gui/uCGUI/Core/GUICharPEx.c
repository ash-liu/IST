#include <stddef.h>           /* needed for definition of NULL */
#include "GUI_Private.h"
#include "ff.h"

//字模数据的暂存数组,以单个字模的最大字节数为设定值
#define BYTES_PER_FONT      128
static U8 GUI_FontDataBuf[BYTES_PER_FONT];

/*---------------------------------------------------------------------------*/
/*字库外部函数部分-----------------------------------------------------------*/
void GUI_X_GetFontData(char* font, U32 oft, U8 *ptr, U8 bytes)
{

	FIL fsrc;		// 定义文件操作类
	FRESULT res;	// 定义操作结果变量
	UINT br;		// 定义读写数量变量

	res = f_open(&fsrc, font, FA_OPEN_EXISTING | FA_READ);   //打开字库文件
	if(res != FR_OK){

    }

	res = f_lseek(&fsrc,oft); //找到首地址
	res = f_read(&fsrc, ptr, bytes, &br); //读取32个字库点阵数据
	res = f_close(&fsrc); //关闭字体
}


static void GUI_GetDataFromMemory(const GUI_FONT_PROP GUI_UNI_PTR *pProp, U16P c)
{
	U8 BytesPerFont;
	U32 oft;

	char *font = (char *)pProp->paCharInfo->pData;
	BytesPerFont = GUI_Context.pAFont->YSize * pProp->paCharInfo->BytesPerLine; //每个字模的数据字节数

	if (BytesPerFont > BYTES_PER_FONT) {
		BytesPerFont = BYTES_PER_FONT;
	}

	if(c<0x80) {
		oft = (c-0x20) * BytesPerFont; //英文字符地址偏移算法
    }
    else {
		oft = ((((c >> 8)-0xA1)) + ((c & 0xFF)-0xb0) * 94)* BytesPerFont; //中文字符地址偏移算法包括符号
    }

	GUI_X_GetFontData(font, oft, GUI_FontDataBuf, BytesPerFont);
}


void GUIPROP_X_DispChar(U16P c)
{
    int BytesPerLine;
    GUI_DRAWMODE DrawMode = GUI_Context.TextMode;
    const GUI_FONT_PROP GUI_UNI_PTR *pProp = GUI_Context.pAFont->p.pProp;

    //搜索定位字库数据信息
	for (; pProp; pProp = pProp->pNext) {
		if ((c >= pProp->First) && (c <= pProp->Last))break;
    }

    if (pProp) {	
		GUI_DRAWMODE OldDrawMode;
		const GUI_CHARINFO GUI_UNI_PTR * pCharInfo = pProp->paCharInfo;
        GUI_GetDataFromMemory(pProp, c);//取出字模数据

		BytesPerLine = pCharInfo->BytesPerLine;
		OldDrawMode  = LCD_SetDrawMode(DrawMode);

		LCD_DrawBitmap(GUI_Context.DispPosX, GUI_Context.DispPosY,
					pCharInfo->XSize, GUI_Context.pAFont->YSize,
					GUI_Context.pAFont->XMag, GUI_Context.pAFont->YMag,
					1,     /* Bits per Pixel */
					BytesPerLine,
					&GUI_FontDataBuf[0],
					&LCD_BKCOLORINDEX);

        /* Fill empty pixel lines */
		if (GUI_Context.pAFont->YDist > GUI_Context.pAFont->YSize) {
			int YMag = GUI_Context.pAFont->YMag;
			int YDist = GUI_Context.pAFont->YDist * YMag;
			int YSize = GUI_Context.pAFont->YSize * YMag;

			if (DrawMode != LCD_DRAWMODE_TRANS) {
				LCD_COLOR OldColor = GUI_GetColor();
				GUI_SetColor(GUI_GetBkColor());
				LCD_FillRect(GUI_Context.DispPosX, GUI_Context.DispPosY + YSize,
                            GUI_Context.DispPosX + pCharInfo->XSize,
                            GUI_Context.DispPosY + YDist);

                GUI_SetColor(OldColor);
			}
		}

		LCD_SetDrawMode(OldDrawMode); /* Restore draw mode */
        GUI_Context.DispPosX += pCharInfo->XDist * GUI_Context.pAFont->XMag;
	}
}


/*********************************************************************
*
*       GUIPROP_GetCharDistX
*/
int GUIPROP_X_GetCharDistX(U16P c)
{
    const GUI_FONT_PROP GUI_UNI_PTR * pProp = GUI_Context.pAFont->p.pProp;

    for (; pProp; pProp = pProp->pNext) {
		if ((c >= pProp->First) && (c <= pProp->Last))break;
	}

	return (pProp) ? (pProp->paCharInfo)->XSize * GUI_Context.pAFont->XMag : 0;
}



