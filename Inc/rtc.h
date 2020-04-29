/**
  ******************************************************************************
  * File Name          : RTC.h
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __rtc_H
#define __rtc_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern RTC_HandleTypeDef hrtc;

/* USER CODE BEGIN Private defines */
#pragma pack (1)
//用户时间结构体
typedef struct st_user_time_
{
	unsigned char year;
	unsigned char month;
	unsigned char date;
	unsigned char hour;
	unsigned char minute;
	unsigned char second;
}ST_User_Time;

//RTC控制结构体
typedef struct st_rtc_control
{
    unsigned int Wkpflag;
	unsigned int Wkptimes;						
	
}ST_RTC_Control;
#pragma pack ()

extern ST_RTC_Control RTCCtrl;
/* USER CODE END Private defines */

void MX_RTC_Init(void);

/* USER CODE BEGIN Prototypes */
void RTC_Get_UserTime(char *buffer);
void RTC_Set_UserTime(ST_User_Time *pTime);
void RTC_Process_Task(void);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ rtc_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
