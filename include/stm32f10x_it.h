/**
  ******************************************************************************
  * @file    stm32f10x_it.h 
  * @author  MCD Application Team
  * @version V3.6.0
  * @date    20-September-2021
  * @brief   This file contains the headers of the interrupt handlers.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2011 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F10x_IT_H
#define __STM32F10x_IT_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
#define EXTI3_PREEMPT_PRIORITY 0x03
#define EXTI3_SUB_PRIORITY 0x01

#define EXTI4_PREEMPT_PRIORITY 0x03
#define EXTI4_SUB_PRIORITY 0x02

#define USART1_PREEMPT_PRIORITY 0x00
#define USART1_SUB_PRIORITY 0x00

#define CAN1_PREEMPT_PRIORITY 0x02
#define CAN1_SUB_PRIORITY 0x00

#define EXTI15_10_PREEMPT_PRIORITY 0x03
#define EXTI15_10_SUB_PRIORITY 0x00

#define TIM2_PREEMPT_PRIORITY 0x01
#define TIM2_SUB_PRIORITY 0x01

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
					 
#endif /* __STM32F10x_IT_H */

