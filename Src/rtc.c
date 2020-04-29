/**
  ******************************************************************************
  * File Name          : RTC.c
  * Description        : This file provides code for the configuration
  *                      of the RTC instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "rtc.h"

/* USER CODE BEGIN 0 */
#include "rs232.h"
ST_RTC_Control RTCCtrl;
/* USER CODE END 0 */

RTC_HandleTypeDef hrtc;

/* RTC init function */
void MX_RTC_Init(void)
{
  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /** Initialize RTC Only 
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */
    
  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date 
  */
  sTime.Hours = 12;
  sTime.Minutes = 30;
  sTime.Seconds = 30;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_FRIDAY;
  sDate.Month = RTC_MONTH_MAY;
  sDate.Date = 1;
  sDate.Year = 20;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enable the WakeUp 
  */
  if (HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 4, RTC_WAKEUPCLOCK_CK_SPRE_16BITS) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_RTC_MspInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspInit 0 */

  /* USER CODE END RTC_MspInit 0 */
    /* RTC clock enable */
    __HAL_RCC_RTC_ENABLE();

    /* RTC interrupt Init */
    HAL_NVIC_SetPriority(RTC_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(RTC_IRQn);
  /* USER CODE BEGIN RTC_MspInit 1 */

  /* USER CODE END RTC_MspInit 1 */
  }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspDeInit 0 */

  /* USER CODE END RTC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_RTC_DISABLE();

    /* RTC interrupt Deinit */
    HAL_NVIC_DisableIRQ(RTC_IRQn);
  /* USER CODE BEGIN RTC_MspDeInit 1 */

  /* USER CODE END RTC_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */
/*------------------------------------------------------------
* 函 数 名：RTC_Get_UserTime()
* 功能描述：读取RTC时间
* 函数说明：
* 输    入：
* 输    出：*buffer
* 返    回：
------------------------------------------------------------*/
void RTC_Get_UserTime(char *buffer)
{
    RTC_DateTypeDef date = {0};
    RTC_TimeTypeDef time = {0};
	unsigned char i = 0;
    
    HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
	HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
	
	buffer[i++] = date.Year;
	buffer[i++] = date.Month;
	buffer[i++] = date.Date;
	buffer[i++] = time.Hours;
	buffer[i++] = time.Minutes;
	buffer[i++] = time.Seconds;
	
	//2020-05-01 12:30:30
	sprintf(buffer,"$TM:20%02d-%02d-%02d %02d:%02d:%02d",buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5]);
}


/*------------------------------------------------------------
* 函 数 名：RTC_Set_UserTime()
* 功能描述：设置RTC时间
* 函数说明：
* 输    入：
* 输    出：*pTime
* 返    回：
------------------------------------------------------------*/
void RTC_Set_UserTime(ST_User_Time *pTime)
{
    RTC_DateTypeDef  date = {0};
    RTC_TimeTypeDef  time = {0};

    //Date: Tuesday February 18th 2014  Year=0x15
    date.Year 	 = pTime->year;
    date.Month	 = pTime->month;
    date.Date 	 = pTime->date;
    date.WeekDay = RTC_WEEKDAY_MONDAY;
    if(HAL_RTC_SetDate(&hrtc, &date, RTC_FORMAT_BIN) != HAL_OK)
    {
        Error_Handler();
    }

    //Time: 02:20:00
    time.Hours 	 = pTime->hour;
    time.Minutes = pTime->minute;
    time.Seconds = pTime->second;
    time.TimeFormat = RTC_HOURFORMAT12_AM;
    time.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
    time.StoreOperation = RTC_STOREOPERATION_RESET;
    if(HAL_RTC_SetTime(&hrtc,&time,RTC_FORMAT_BIN) != HAL_OK)
    {
        Error_Handler();
    }
}

/*------------------------------------------------------------
* 函 数 名：RTC_Set_UserTime()
* 功能描述：设置RTC时间
* 函数说明：
* 输    入：
* 输    出：*pTime
* 返    回：
------------------------------------------------------------*/
void RTC_Process_Task(void)
{
	if(RTCCtrl.Wkpflag)
	{
		char bufer[64] = {0};
		RTCCtrl.Wkpflag = 0;
		RTCCtrl.Wkptimes++;
		
		RTC_Get_UserTime(bufer);
//		RS232Printf("Wkptimes=%d, %s",RTCCtrl.Wkptimes, bufer);
		printf("\r\nWkptimes=%d, %s\r\n",RTCCtrl.Wkptimes, bufer);
	}
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
