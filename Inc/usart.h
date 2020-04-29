/**
  ******************************************************************************
  * File Name          : USART.h
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
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
#ifndef __usart_H
#define __usart_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#define USART_BUFF_SIZE             	512
#define USART5_QUEUE_COUNT             	4
/* USER CODE END Includes */

extern UART_HandleTypeDef huart5;

/* USER CODE BEGIN Private defines */

/*串口编号枚举*/
typedef enum usart_number_type
{
	NoRS232=0,
	NoRS485,
	NoIRUART,
	NoBCUART,
    NoUSART_MAX
}EUsart_Type;

//串口命令收发状态
typedef enum enum_uart_tx_state
{
    Usart_State_Idle = 0,
    Usart_State_Processing,
    Usart_State_Done,
}EUsart_Status;

/*USART任务数据结构体*/
#pragma pack (1)
typedef struct st_usart_task_data
{
    unsigned char RxIn;						
    unsigned char RxOut;
	unsigned char TxIn;                     
    unsigned char TxOut;

	EUsart_Status RxState;           		
    unsigned short RxLength;                
    char RxBuff[USART_BUFF_SIZE];  
	
	EUsart_Status TxState;
    unsigned short TxLength;                
    char TxBuff[USART_BUFF_SIZE];  
	char TxFram[USART_BUFF_SIZE];  
	
}ST_USART_Data;
#pragma pack ()



extern ST_USART_Data USARTData[NoUSART_MAX];


/* USER CODE END Private defines */

void MX_USART5_UART_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ usart_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
