/**
  ******************************************************************************
  * File Name          : rs232.c
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

/* Includes ------------------------------------------------------------------*/
#include "usart.h"
#include <stdarg.h> 
#include "FreeRTOS.h"
#include "queue.h" 

extern QueueHandle_t QueueRS232Tx;


/*------------------------------------------------------------
* 函 数 名：RS232Printf()
* 功能描述：打印log
* 函数说明：
* 输    入：字符串
* 返    回：
------------------------------------------------------------*/
void RS232Printf(char *fmt,...)
{
    unsigned short len = 0;
    va_list args;
	
    va_start(args, fmt);
    len = vsnprintf((char *)USARTData[NoRS232].TxBuff, USART_BUFF_SIZE, (char *)fmt, args);
    va_end(args);
    
    if(len >= USART_BUFF_SIZE - 2)
        len = USART_BUFF_SIZE - 2;

    USARTData[NoRS232].TxBuff[len++] = '\r';
    USARTData[NoRS232].TxBuff[len++] = '\n';
    
	USARTData[NoRS232].TxIn++;
	xQueueSend(QueueRS232Tx, USARTData[NoRS232].TxBuff, 0);
    memset(USARTData[NoRS232].TxBuff, 0, USART_BUFF_SIZE);   
}

/*------------------------------------------------------------ 
* Function:RS232_Process_Task                                  
* Brief:   RS232串口收发数据函数                               
* Param:                                                       
* Retval:
* Author:
-------------------------------------------------------------*/
void RS232_Process_Task(void)
{
	//接收解译
	if(USARTData[NoRS232].RxState == Usart_State_Done)
	{
		if(USARTData[NoRS232].RxLength)
		{
			if(strstr((const char *)USARTData[NoRS232].RxBuff, "{\"cmd\":"))
			{
				if(strstr((const char *)USARTData[NoRS232].RxBuff, "{\"cmd\":\"0\"}"))
				{
					RS232Printf("{\"cmd\":\"0\"}");
				}
				else if(strstr((const char *)USARTData[NoRS232].RxBuff, "reboot"))
				{
					RS232Printf("reboot");
					//NVIC_SystemReset();
				}
			}
		}
		USARTData[NoRS232].RxOut++;
		USARTData[NoRS232].RxLength = 0;
		USARTData[NoRS232].RxState = Usart_State_Idle;
		memset(USARTData[NoRS232].RxBuff, 0, USART_BUFF_SIZE);
	}
	
	//发送数据
	if(uxQueueMessagesWaiting(QueueRS232Tx))
	{
		if(xQueueReceive(QueueRS232Tx, USARTData[NoRS232].TxFram, 0))
		{
			HAL_UART_Transmit(&huart5,(uint8_t *)USARTData[NoRS232].TxFram, strlen((const char *)USARTData[NoRS232].TxFram),1000);
			memset(USARTData[NoRS232].TxFram, 0, USART_BUFF_SIZE);
			USARTData[NoRS232].TxOut++;
		}
	}
	
}
