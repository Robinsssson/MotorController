#include "device/mf4010v2.h"
#include "main.h"
#include "mirco_lib/mirco_delay.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_it.h"
#include "stm32f10x_rcc.h"
#include <bsp/key.h>
#include <stdio.h>
#include <misc.h>

void key_init(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE);
    GPIO_InitStructure.GPIO_Pin   = KEY1_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = KEY0_GPIO_PIN;
    GPIO_Init(KEY0_GPIO_PORT, &GPIO_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource3);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource4);
    EXTI_InitStructure.EXTI_Line    = EXTI_Line3;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    EXTI_InitStructure.EXTI_Line = EXTI_Line4;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel                   = EXTI3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = EXTI3_PREEMPT_PRIORITY;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = EXTI3_SUB_PRIORITY;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel                   = EXTI4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = EXTI4_PREEMPT_PRIORITY;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = EXTI4_SUB_PRIORITY;
    NVIC_Init(&NVIC_InitStructure);
}
extern mf4010v2_t motor1, motor2;

void EXTI3_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line3) != RESET) {
        EXTI_ClearITPendingBit(EXTI_Line3);
        COMMAND_USEAGE(&motor1, COMMAND_MOTOR_CLOSE);
        COMMAND_USEAGE(&motor2, COMMAND_MOTOR_CLOSE);
    }
}

void EXTI4_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line4) != RESET) {
        EXTI_ClearITPendingBit(EXTI_Line4);
        COMMAND_USEAGE(&motor1, COMMAND_MOTOR_CLOSE);
        COMMAND_USEAGE(&motor2, COMMAND_MOTOR_CLOSE);
    }
}