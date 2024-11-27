#ifndef __USART__H__
#define __USART__H__

#include "cjson/cJSON.h"
#include "stm32f10x.h"
#include <stdint.h>
#include <stm32f10x_usart.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <misc.h>
#define RX_BUFFER_SIZE  1000  // 设置接收缓冲区大小
typedef struct {
    uint16_t ptr;
    uint16_t json_scope;
    cJSON *json;
    uint8_t over_flag;
    uint8_t buf[RX_BUFFER_SIZE];
    uint8_t overflow_flag;
} usart_rx;

extern usart_rx global_usart1_rx;
void usart1_send_str(uint8_t *str);
void usart1_init(int baud_rate);
void usart1_send(uint8_t *data, uint16_t len);
void usart1_send_hex(uint16_t data);
void usart1_send_integer(int32_t data);
#define USART_DEBUG 1
#if USART_DEBUG
#define USART1_DEBUG_PRINT(str) usart1_send_str((uint8_t *)str)
#define USART1_DEBUG_HEX(hex) usart1_send_hex(hex)
#else
#define USART1_DEBUG_PRINT(str)
#define USART1_DEBUG_HEX(hex)
#endif
#endif  //!__USART__H__
