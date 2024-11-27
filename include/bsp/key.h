#ifndef __KEY__H__
#define __KEY__H__
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_exti.h>
#include <main.h>
#define KEY1_GPIO_PORT GPIOE
#define KEY1_GPIO_PIN GPIO_Pin_3
#define KEY0_GPIO_PORT GPIOE
#define KEY0_GPIO_PIN GPIO_Pin_4

void key_init(void);
#endif  //!__KEY__H__