#include "datetime.h"
#include "stm32f10x.h"
#include "view.h"
#include <string.h>

char datetime_buf[DATETIME_BUF_LENGTH];
volatile char datetime_buf_length = 0;

volatile static int  Y;
volatile static char M,D,h,m,s;
volatile static char rtc_update_time = 0;		// 1s update
volatile static char update_day_flag = 0;   	//day value update



//判断输入的年份是否是闰年 
int Is_leap(int  year)
{
	if((year%400==0) || ((year%4==0)&&(year%100!=0))){
		return 1;
	}
		
	return 0;
} 


//计算一个月的最大天数(上一个月） 
int month_max_day(int year, int month)
{	  
	int Day[12]={31,28,31,30,31,30,31,31,30,31,30,31};
	
    if(Is_leap(year) == 1){
		Day[1]=29;
	}  
          
    return Day[month-1]; 
} 


//计算输入的日期是这一年的第几天
int total_day(int year, int month, int day)
{
	int sum = 0;
	int i;
	
	for(i=1; i<month; i++){
		sum+=month_max_day(year,i);
	}
		
	sum+=day;
	return sum;
}


//由输入的日期判断当天是星期几
char get_week(int year, int month, int day) 
{
	char week;

	if(month==1 || month==2) {
		month+=12;
		year--;
	}
	week = (day+2*month+3*(month+1)/5+year+year/4-year/100+year/400)%7;	

	return week;
}


void RTC_Configuration(void)
{

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

    PWR_BackupAccessCmd(ENABLE);
    BKP_DeInit();

	// select HSE div128 as clock (we have no LSE)
    RCC_RTCCLKConfig(RCC_RTCCLKSource_HSE_Div128);
    RCC_RTCCLKCmd(ENABLE);


    RTC_WaitForSynchro();
    RTC_WaitForLastTask();

    RTC_ITConfig(RTC_IT_SEC, ENABLE);
    RTC_WaitForLastTask();
    RTC_SetPrescaler(62499); /* RTC period = (HSE/128)/RTC_PR = (62500Hz)/(62499+1) */
    RTC_WaitForLastTask();
}


void RTC_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}



void init_datetime()
{
	datetime_buf_length = 0;

	RTC_Configuration();
	RTC_NVIC_Configuration();

}


// Year(1B) | Month(1B) | Day(1B) | Hour(1B) | min(1B) | sec(1B)
int parse_datetime()
{
	char _Y, _M, _D, _h, _m, _s;

	if (datetime_buf_length != 6) {
		return 1;
	}

	_Y = datetime_buf[0];
	_M = datetime_buf[1];
	_D = datetime_buf[2];
	_h = datetime_buf[3];
	_m = datetime_buf[4];
	_s = datetime_buf[5];

	// check data
	if (_M < 1 || _M > 12) {
		return 1;
	}
	if (_D < 1 || _D > 31) {
		return 1;
	}
	if (_h > 23) {
		return 1;
	}
	if (_m > 59) {
		return 1;
	}
	if (_s > 59) {
		return 1;
	}

	RTC_ITConfig(RTC_IT_SEC, DISABLE);
	Y = _Y + 2000;
	M = _M;
	D = _D;
	h = _h;
	m = _m;
	s = _s;
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
	
	return 0;
}


int loop_datetime()
{
	if (datetime_buf_length > 0) {
		if (parse_datetime() == 0) {
			show_date(Y, M, D, get_week(Y, M, D));
		}

		datetime_buf_length = 0;
	}

	if (rtc_update_time == 1) {
		show_time(h, m, s);

		if (update_day_flag == 1) {
			show_date(Y, M, D, get_week(Y, M, D));
			update_day_flag = 0;
		}
		
		rtc_update_time = 0;
	}

	return 0;
}


void datetime_1s_irq()
{
	if (++s >= 60) {
		s = 0;
		if (++m >= 60) {
			m = 0;
			if (++h >= 24){
				h = 0;
				update_day_flag = 1;
				if (++D > month_max_day(Y,M)) {
					D = 1;
					if (++M > 12) {
						M = 1;
						++Y;
					}
				}
			}
		}
	}

	rtc_update_time = 1;
}



