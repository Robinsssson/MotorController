#ifndef __SPI2__H__
#define __SPI2__H__

#include <stm32f10x_spi.h>

#define SPI2_PORT   GPIOB
#define SPI2_SCK    GPIO_Pin_13
#define SPI2_MISO   GPIO_Pin_14
#define SPI2_MOSI   GPIO_Pin_15

void spi2_init(void);
void spi2_send_message(uint8_t data);

#endif  //!__SPI2__H__