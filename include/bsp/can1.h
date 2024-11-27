#ifndef __CAN1__H__
#define __CAN1__H__
#include <stm32f10x_can.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_tim.h>
#define CAN_INTERRUPT_STATE 0
#define CAN_TIMEOUT 0xFFFF // 超时时间常量
void can1_init();
void can1_send_message(uint16_t id, uint8_t* data, uint8_t len);
CanRxMsg can1_receive_message();
#endif  //!__CAN1__H__