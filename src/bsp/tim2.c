#include "bsp/tim2.h"
#include "stm32f10x_it.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include <device/mf4010v2.h>
#include <device/mpu6050.h>
#include <misc.h>
#include <stdio.h>
#include <stm32f10x.h>
#include "device/inv_mpu.h"
#include "bsp/led.h"
float pitch, roll, yaw;
void  app_control(uint8_t enable_motor1, float measurement1, float setpoint1, uint8_t enable_motor2, float measurement2,
                  float setpoint2);
void  tim2_init()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period        = 500 - 1;
    TIM_TimeBaseStructure.TIM_Prescaler     = 7200 - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel                   = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TIM2_PREEMPT_PRIORITY;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = TIM2_SUB_PRIORITY;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM2, ENABLE);
}
static int count = 0;

int global_pitch_control = 0;
uint8_t global_roll_running = 1;
uint8_t global_resend_flag = 1;
uint8_t global_pitch_running = 1;
static int local_control_value = 0;
void TIM2_IRQHandler()
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        mpu_dmp_get_data(&pitch, &roll, &yaw);
        if (count == 2) {
            count = 0;
            if (local_control_value != global_pitch_control) {
                local_control_value > global_pitch_control ? local_control_value-- : local_control_value++;
            }
            app_control(global_pitch_running, pitch, local_control_value, global_roll_running, roll , 0); // app_control(1, pitch, 30, 1, roll, 0)
        }
        count ++;
        if (global_resend_flag) {
            printf("{\"Pitch\":%.2f,\"Roll\":%.2f,\"Yaw\":%.2f}\r\n", pitch, roll, yaw);
        }
    }
}