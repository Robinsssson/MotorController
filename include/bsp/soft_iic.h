#ifndef __SOFT_IIC__H__
#define __SOFT_IIC__H__
#include "stm32f10x.h"
#include <mirco_lib/mirco_delay.h>
#include <stdint.h>

uint8_t soft_iic_init(void);
uint8_t soft_iic_deinit(void);
uint8_t soft_iic_read_byte(uint8_t ack);
void    soft_iic_send_byte(uint8_t byte);
void    soft_iic_send_ack(uint8_t ack);
uint8_t soft_iic_wait_ack(void);
void    soft_iic_stop(void);
void    soft_iic_start(void);
uint8_t soft_iic_write(uint8_t addr, uint8_t reg, uint8_t* buf, uint16_t len);
uint8_t soft_iic_read(uint8_t addr, uint8_t reg, uint8_t* buf, uint16_t len);
uint8_t iic_read_address16(uint8_t addr, uint16_t reg, uint8_t* buf,
                           uint16_t len);
uint8_t iic_write_address16(uint8_t addr, uint16_t reg, uint8_t* buf,
                            uint16_t len);
uint8_t iic_read_cmd(uint8_t addr, uint8_t* buf, uint16_t len);
uint8_t iic_write_cmd(uint8_t addr, uint8_t* buf, uint16_t len);
#endif //!__SOFT_IIC__H__