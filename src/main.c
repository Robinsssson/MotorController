/**
 ******************************************************************************
 * @file    main.c
 * @author  MCD Application Team
 * @version V3.6.0
 * @date    20-September-2021
 * @brief   Main program body
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "bsp/led.h"
#include "cjson/cJSON.h"
#include "device/inv_mpu.h"
#include "device/mpu6050.h"
#include "mirco_lib/mirco_delay.h"
#include "mirco_lib/mirco_string.h"
#include "misc.h"
#include "pid/PID.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "sys/types.h"
#include "system_stm32f10x.h"
#include <app.h>
#include <bsp/can1.h>
#include <bsp/usart1.h>
#include <device/mf4010v2.h>
#include <device/mpu6050.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
mf4010v2_t motor1;
mf4010v2_t motor2;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static __IO uint32_t TimingDelay;
/* Private function prototypes -----------------------------------------------*/
void System_Config(void);
/**
 * @brief  Main program.
 * @param  None
 * @retval None
 */
int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    app_init();
    while(1) {        
    }
}

void Delay(__IO uint32_t nTime)
{
    TimingDelay = nTime;

    while (TimingDelay != 0)
        ;
}

void System_Config(void)
{
    if (SysTick_Config(SystemCoreClock / 1000)) {
        // Handle Error
        while (1)
            ;
    }
}

/**
 * @brief  Decrements the TimingDelay variable.
 * @param  None
 * @retval None
 */
void TimingDelay_Decrement(void)
{
    if (TimingDelay != 0x00) {
        TimingDelay--;
    }
}

#ifdef USE_FULL_ASSERT

/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line
       number, ex: printf("Wrong parameters value: file %s on line %d\r\n",
       file, line) */

    /* Infinite loop */
    while (1) {
    }
}
#endif

/**
 * @}
 */
