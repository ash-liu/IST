/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>
#include "ff.h"
#include "sdcard.h"
#include "gui.h"
#include "Tiky_LCD.h"
#include "diskio.h"

/* Private date ---------------------------------------------------------*/

FATFS fs;            // Work area (file system object) for logical drive
FIL fsrc, fdst;      // file objects
BYTE buffer[51]; // file copy buffer
FRESULT res;         // FatFs function common result code
UINT br, bw;         // File R/W count

SD_Error Status = SD_OK;
SD_CardInfo SDCardInfo;

#define  SystemFrequency  72000000

/*******************************************************************************
* Function Name  : NVIC_Config
* Description    : Configures SDIO IRQ channel.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure the NVIC Preemption Priority Bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


/*************************************************************************
 * Function Name: Serial_Init
 * Description: Init USARTs
 *************************************************************************/
void Serial_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2, DISABLE);

  /* Configure USART2 Tx (PA.02) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure USART1 Rx (PA.03) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

/* USART1 configuration ------------------------------------------------------*/
  // USART1 configured as follow:
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  /* Configure the USART2 */
  USART_Init(USART2, &USART_InitStructure);

  /* Enable the USART Transmoit interrupt: this interrupt is generated when the 
     USART1 transmit data register is empty */  
 // USART_ITConfig(USART2, USART_IT_TXE, ENABLE);

  /* Enable the USART Receive interrupt: this interrupt is generated when the 
     USART1 receive data register is not empty */
//  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

  /* Enable USART2 */
  USART_Cmd(USART2, ENABLE);

  // 随便发一个数据，clear flag 否则第一个字符发送不出去
  USART_SendData(USART2, 0x00);
  while (!(USART2->SR & USART_FLAG_TXE));
}


void OutPutFile(void)
{
	FRESULT res;
	FILINFO finfo;
	FIL test;
	DIR dirs;
	int i;
	char *fn;
	char path[50]={""};  
	char name[]={"WVO.TXT"};
	UINT write_length = 0;
	UINT been_writed_length = 0;
    
	f_open(&test, name, FA_WRITE | FA_CREATE_ALWAYS);
	res = f_write(&test, "hello world!", sizeof("hello world!"), &been_writed_length);
	res = f_close(&test);
/*
	if (f_opendir(&dirs, path) == FR_OK)  {
		while (f_readdir(&dirs, &finfo) == FR_OK) {
			if (finfo.fattrib & AM_ARC) {
				if(!finfo.fname[0])	
					break;         
				printf("\n file name is:\n   %s\n",finfo.fname);
			 	res = f_open(&fsrc, finfo.fname, FA_OPEN_EXISTING | FA_READ);
				res = f_read(&fsrc, &buffer, 50, &br);
				printf("\n file contex is:\n   %s\n",buffer);
				f_close(&fsrc);			                      
			}
		} 
    
  	}
*/
}


// init sdcard for 4bits DMA mode @ 18MHz
// ret 0 : OK
// ret 1 : ERROR
int init_sdcard()
{
	/*-------------------------- SD Init ----------------------------- */
	Status = SD_Init();
	printf("SD_Init rst is %d \n", Status);

	if (Status == SD_OK) {
		/*----------------- Read CSD/CID MSD registers ------------------*/
		Status = SD_GetCardInfo(&SDCardInfo);
	}
	else {
		return 1;
	}
	printf("SD_GetCardInfo rst is %d \n", Status);
	  
	if (Status == SD_OK) {
		/*----------------- Select Card --------------------------------*/
		Status = SD_SelectDeselect((u32) (SDCardInfo.RCA << 16));
	}
	else {
		return 1;
	}
	printf("SD_SelectDeselect rst is %d \n", Status);
	  
	if (Status == SD_OK){
		Status = SD_EnableWideBusOperation(SDIO_BusWide_4b);
	}	
	else {
		return 1;
	}
	printf("SD_EnableWideBusOperation rst is %d \n", Status);
	  
	/* Set Device Transfer Mode to DMA */
	if (Status == SD_OK) {  
		Status = SD_SetDeviceMode(SD_DMA_MODE);//oet
	   //Status = SD_SetDeviceMode(SD_POLLING_MODE);
		//Status = SD_SetDeviceMode(SD_INTERRUPT_MODE);
	}
	else {
		return 1;
	}
	printf("SD_SetDeviceMode rst is %d \n", Status);

	return 0;
}

// init fat fs
int init_fatfs()
{
	disk_initialize(0);
	f_mount(0, &fs);
	return 0;
}

extern void MainTask();

int main(void)
{	
	u8 result = 0xFF;
	u8 test_buf[512];

  	SystemInit();//初始化STM32
	NVIC_Configuration(); // init sdio IRQ
	Serial_Init(); // 115200,8,n,1

	result = init_sdcard();
	if (result > 0) {
		printf("init sdcard error \n");
		return result;
	}

	result = init_fatfs();
	if (result > 0) {
		printf("init fat fs error\n");
		return result;
	}

#if 0
	// test sdcard api	
	//Status = SD_ReadBlock(0 , (u32 *)(test_buf), 512);	
	//printf("SD_ReadBlock rst is %d\r\n", Status);
	OutPutFile();
#endif 

	Lcd_Initialize(); // init Lcd 
	SysTick_Config(SystemFrequency / 100);  //set system tick as 10ms
	GUI_Init();	//init GUI

	while(1) {
		MainTask();
	}
	
}


int fputc(int ch, FILE *f)
{
	USART_SendData(USART2, (unsigned char) ch);
	while (!(USART2->SR & USART_FLAG_TXE));
	return (ch);
}


int GetKey (void) 
{
	while (!(USART2->SR & USART_FLAG_RXNE));
	return ((int)(USART2->DR & 0x1FF));
}


/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
