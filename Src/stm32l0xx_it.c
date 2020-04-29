/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32l0xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l0xx_it.h"
#include "FreeRTOS.h"
#include "task.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"
#include "rtc.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
 
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern RTC_HandleTypeDef hrtc;
extern UART_HandleTypeDef huart5;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M0+ Processor Interruption and Exception Handlers          */ 
/******************************************************************************/
/**
  * @brief This function handles Non maskable Interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
#if (INCLUDE_xTaskGetSchedulerState == 1 )
  if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
  {
#endif /* INCLUDE_xTaskGetSchedulerState */
  xPortSysTickHandler();
#if (INCLUDE_xTaskGetSchedulerState == 1 )
  }
#endif /* INCLUDE_xTaskGetSchedulerState */
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32L0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32l0xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles RTC global interrupt through EXTI lines 17, 19 and 20 and LSE CSS interrupt through EXTI line 19.
  */
void RTC_IRQHandler(void)
{
  /* USER CODE BEGIN RTC_IRQn 0 */
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
	__HAL_RTC_WAKEUPTIMER_EXTI_CLEAR_FLAG();
	__HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&hrtc, RTC_FLAG_WUTF);
	
	RTCCtrl.Wkpflag = 1;
	RTC_Process_Task();
  /* USER CODE END RTC_IRQn 0 */
  HAL_RTCEx_WakeUpTimerIRQHandler(&hrtc);
  /* USER CODE BEGIN RTC_IRQn 1 */

  /* USER CODE END RTC_IRQn 1 */
}

/**
  * @brief This function handles USART4 and USART5 interrupt.
  */
void USART4_5_IRQHandler(void)
{
  /* USER CODE BEGIN USART4_5_IRQn 0 */
	static unsigned char RX_ch;
	uint32_t isrflags = READ_REG(huart5.Instance->ISR);
	uint32_t cr1its   = READ_REG(huart5.Instance->CR1);
	uint32_t cr3its   = READ_REG(huart5.Instance->CR3);
    
    if((USART5->ISR & USART_ISR_ORE) == USART_ISR_ORE)
    {
        USART5->ICR = USART_ICR_ORECF;
    }
    if((__HAL_UART_GET_FLAG(&huart5, UART_FLAG_RXNE) != RESET))
    {
		__HAL_UART_CLEAR_FLAG(&huart5, UART_FLAG_RXNE);
		RX_ch = (uint8_t)(USART5->RDR & (uint16_t)0x01FF);
		
		if(USARTData[NoRS232].RxLength == 0)
			USARTData[NoRS232].RxBuff[USARTData[NoRS232].RxLength++] = RX_ch; 
		else
		{
			__HAL_UART_ENABLE_IT(&huart5,UART_IT_IDLE);
			USARTData[NoRS232].RxBuff[USARTData[NoRS232].RxLength++] = RX_ch; 
				
			if(USARTData[NoRS232].RxState == Usart_State_Idle)
				USARTData[NoRS232].RxState = Usart_State_Processing;
		}
    }

    if((__HAL_UART_GET_FLAG(&huart5,UART_FLAG_IDLE) != RESET)) 
    {
        __HAL_UART_CLEAR_IDLEFLAG(&huart5);
        __HAL_UART_DISABLE_IT(&huart5,UART_IT_IDLE);
		
		if(USARTData[NoRS232].RxState == Usart_State_Processing)
		{
			USARTData[NoRS232].RxIn++;
			USARTData[NoRS232].RxState = Usart_State_Done;
		}
	}
  /* USER CODE END USART4_5_IRQn 0 */
  HAL_UART_IRQHandler(&huart5);
  /* USER CODE BEGIN USART4_5_IRQn 1 */

  /* USER CODE END USART4_5_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
