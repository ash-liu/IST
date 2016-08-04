#include "cmd.h"
#include "stm32f10x.h"
#include <string.h>
#include "info.h"

static char cmd_buf[CMD_RECEIVE_LENGTH] = {0};
static int cmd_buf_point = 0;
static char cmd_status = IDLE;
static char cmd_type = 0;
static int cmd_length = 0;


//---------------------------------------
// Timer API for SM
static void init_timer()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	/* Enable the TIM5 gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);

	/* TIM5 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	TIM_TimeBaseStructure.TIM_Period = (10000 - 1);
	TIM_TimeBaseStructure.TIM_Prescaler = (7200 - 1);
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
    TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
    TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
}

static void start_timer()
{
	TIM5->CR1 |= TIM_CR1_CEN;
}

static void reset_timer()
{
	TIM5->CNT = 0;
}

static void stop_timer()
{
	//TIM_Cmd(TIM5, DISABLE); 
	TIM5->CR1 &= (uint16_t)(~((uint16_t)TIM_CR1_CEN));
	TIM5->CNT = 0;
}


// IRQ handle for TIM5 overflow
void timer_overtime()
{
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM5, TIM_IT_Update);

		// reset SM
		cmd_buf_point = 0; 
		cmd_status = IDLE;

		stop_timer();
	}
}
//----------------------------------------


void init_uart(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	//RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2, DISABLE);

	/* Configure USART2 Tx (PA.02) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure USART2 Rx (PA.03) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* USART2 configuration ------------------------------------------------------*/
	// USART2 configured as follow:
	USART_InitStructure.USART_BaudRate = 9600;
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
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	/* Enable USART2 */
	USART_Cmd(USART2, ENABLE);

	// config the usart2 IRQ 
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

	// 随便发一个数据，clear flag 否则第一个字符发送不出去
	//USART_SendData(USART2, 0x00);
	//while (!(USART2->SR & USART_FLAG_TXE));
}



int init_cmd()
{
	int i;

	for (i=0; i<CMD_RECEIVE_LENGTH; i++) {
		cmd_buf[i] = 0;
	}
	cmd_buf_point = 0;
	cmd_status = IDLE;

	init_uart();
	init_timer();

	//init all the cmd here
	init_info(); 
	
	return 0;
}


static int check_cmd_type(char type)
{
	if (type >= CMD_INFO && type <= CMD_SYSTEM){
		return 0;
	}
	return 1;
}


static int check_cmd_length(const char *buf, int *len)
{
	int i=0;

	*len = 0;
	while(i<LENGTH_BYTES) {
		*len = ((*len) << 8) + buf[i];
		i++;
	}

	// overflow
	if (*len > CMD_RECEIVE_LENGTH) {
		return 1;
	}
	
	return 0;
}


//2   TODO : a cmd mesage queue may be more robust
// NOW  : One cmd type onlye contain One static slot to receive cmd message
void distribute_cmd()
{
	switch (cmd_type) {
		case CMD_TIME:
			//copy_context_time(cmd_buf, cmd_length);
			break;

		case CMD_TEMP:
			//copy_context_temp(cmd_buf, cmd_length);
			break;

		case CMD_SYSTEM:
			//copy_context_system(cmd_buf, cmd_length);
			break;

		case CMD_INFO:
			memcpy(info_buf, cmd_buf, cmd_length);
			info_buf_length = cmd_length;
			break;

		default:break;
	}	
}



void receive_cmd_irq()
{
	unsigned char rx_data;

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
		while (USART2->SR & USART_FLAG_RXNE){
			rx_data = USART2->DR & 0xff;

			switch (cmd_status) {
				case IDLE: 
					if (check_cmd_type(rx_data) == 0) {
						cmd_type = rx_data;
						cmd_status = GET_CMD_TYPE;
						start_timer();	// trigger timer
					}
					break;

				case GET_CMD_TYPE:
					cmd_buf[cmd_buf_point++] = rx_data;
					if (cmd_buf_point >= LENGTH_BYTES) {
						if (check_cmd_length(cmd_buf, &cmd_length) == 0) {
							cmd_status = GET_LENGTH;
							cmd_buf_point = 0; 
						}
						else {
							cmd_buf_point = 0; 
							cmd_status = IDLE;
						}
					}
					break;

				case GET_LENGTH:
					cmd_buf[cmd_buf_point++] = rx_data;
					if (cmd_buf_point >= cmd_length) {
						cmd_status = FINISH;
						stop_timer();
						
						// do the cmd distribute
						distribute_cmd();

						// reset status machine
						cmd_buf_point = 0; 
						cmd_status = IDLE;
					}
					break;

				default: break;
			}

			if (cmd_status == GET_CMD_TYPE || cmd_status == GET_LENGTH) {
				reset_timer();
			}
		}

		/* clear interrupt */
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}


	/* If overrun condition occurs, clear the ORE flag and recover communication */
    if (USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET) {
		(void)USART_ReceiveData(USART2);
	}

	if (USART_GetITStatus(USART2, USART_IT_TC) != RESET) {
		/* clear interrupt */
		USART_ClearITPendingBit(USART2, USART_IT_TC);
	}
}




