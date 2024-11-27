#ifndef __APP__H__
#define __APP__H__
#include "bsp/led.h"
#include "mirco_lib/mirco_delay.h"
#include <bsp/key.h>
#include <bsp/led.h>
#include <stm32f10x_gpio.h>
void app_init(void);
void app_control(uint8_t enable_motor1, float measurement1, float setpoint1, uint8_t enable_motor2, float measurement2, float setpoint2);
#endif //!__APP__H__