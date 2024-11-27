#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include <bsp/soft_iic.h>
#include <mirco_lib/mirco_delay.h>
#include <stdint.h>
#include <stdio.h>

#define SOFT_IIC_SCL_PIN GPIO_Pin_13
#define SOFT_IIC_SDA_PIN GPIO_Pin_15
#define SOFT_IIC_GPIO_PORT GPIOF
#define SOFT_IIC_GPIO_CLK RCC_APB2Periph_GPIOF

#define SET_SDA(x)                                                             \
    (x ? GPIO_SetBits(SOFT_IIC_GPIO_PORT, SOFT_IIC_SDA_PIN)                    \
       : GPIO_ResetBits(SOFT_IIC_GPIO_PORT, SOFT_IIC_SDA_PIN));                \
    mirco_delay_us(15);

#define SET_SCL(x)                                                             \
    (x ? GPIO_SetBits(SOFT_IIC_GPIO_PORT, SOFT_IIC_SCL_PIN)                    \
       : GPIO_ResetBits(SOFT_IIC_GPIO_PORT, SOFT_IIC_SCL_PIN));                \
    mirco_delay_us(15);

static uint8_t GET_SDA(void)
{
    uint8_t val;
    val = GPIO_ReadInputDataBit(SOFT_IIC_GPIO_PORT, SOFT_IIC_SDA_PIN);
    mirco_delay_us(15);
    return val;
}

uint8_t soft_iic_init(void)
{
    RCC_APB2PeriphClockCmd(SOFT_IIC_GPIO_CLK, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin   = SOFT_IIC_SCL_PIN | SOFT_IIC_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SOFT_IIC_GPIO_PORT, &GPIO_InitStructure);
    SET_SDA(1);
    SET_SCL(1);
    return 0;
}

uint8_t soft_iic_deinit(void)
{
    RCC_APB2PeriphClockCmd(SOFT_IIC_GPIO_CLK, DISABLE);
    return 0;
}

void soft_iic_start(void)
{
    SET_SDA(1);
    SET_SCL(1);
    SET_SDA(0);
    SET_SCL(0);
}

void soft_iic_stop(void)
{
    SET_SDA(0);
    SET_SCL(1);
    SET_SDA(1);
}

uint8_t soft_iic_wait_ack(void)
{
    uint8_t ack;
    SET_SDA(1);
    SET_SCL(1);
    ack = GET_SDA();
    SET_SCL(0);
    return ack;
}

void soft_iic_send_ack(uint8_t ack)
{
    SET_SDA(ack);
    SET_SCL(1);
    SET_SCL(0);
}

void soft_iic_send_byte(uint8_t byte)
{
    for (uint8_t i = 0; i < 8; i++) {
        SET_SDA(byte & (0x80 >> i));
        SET_SCL(1);
        SET_SCL(0);
    }
}

uint8_t soft_iic_read_byte(uint8_t ack)
{
    uint8_t byte = 0;
    SET_SDA(1);
    for (uint8_t i = 0; i < 8; i++) {
        SET_SCL(1);
        if (GET_SDA())
            byte |= (0x80 >> i);
        SET_SCL(0);
    }
    return byte;
}

uint8_t soft_iic_write(uint8_t addr, uint8_t reg, uint8_t* buf, uint16_t len)
{
    uint16_t i;
    /* send a start */
    soft_iic_start();
    /* send the write addr */
    soft_iic_send_byte(addr);
    if (soft_iic_wait_ack() != 0) {
        soft_iic_stop();
        return 1;
    }
    /* send the reg */
    soft_iic_send_byte(reg);
    if (soft_iic_wait_ack() != 0) {
        soft_iic_stop();
        return 1;
    }
    /* write the data */
    for (i = 0; i < len; i++) {
        /* send one byte */
        soft_iic_send_byte(buf[i]);
        if (soft_iic_wait_ack() != 0) {
            soft_iic_stop();
            return 1;
        }
    }
    /* send a stop */
    soft_iic_stop();
    return 0;
}
uint8_t soft_iic_read(uint8_t addr, uint8_t reg, uint8_t* buf, uint16_t len)
{
    {
        /* send a start */
        soft_iic_start();

        /* send the write addr */
        soft_iic_send_byte(addr);
        if (soft_iic_wait_ack() != 0) {
            soft_iic_stop();
            return 1;
        }

        /* send the reg */
        soft_iic_send_byte(reg);
        if (soft_iic_wait_ack() != 0) {
            soft_iic_stop();
            return 1;
        }
        /* send a start */
        soft_iic_start();
        /* send the read addr */
        soft_iic_send_byte(addr + 1);
        if (soft_iic_wait_ack() != 0) {
            soft_iic_stop();

            return 1;
        }
        /* read the data */
        while (len != 0) {
            /* if the last */
            if (len == 1) {
                /* send nack */
                *buf = soft_iic_read_byte(0);
            } else {
                /* send ack */
                *buf = soft_iic_read_byte(1);
            }
            len--;
            buf++;
        }
        /* send a stop */
        soft_iic_stop();
        return 0;
    }
}

uint8_t iic_write_cmd(uint8_t addr, uint8_t* buf, uint16_t len)
{
    uint16_t i;

    /* send a start */
    soft_iic_start();

    /* send the write addr */
    soft_iic_send_byte(addr);
    if (soft_iic_wait_ack() != 0) {
        soft_iic_stop();
        return 1;
    }

    /* write the data */
    for (i = 0; i < len; i++) {
        /* send one byte */
        soft_iic_send_byte(buf[i]);
        if (soft_iic_wait_ack() != 0) {
            soft_iic_stop();
            return 1;
        }
    }

    /* send a stop */
    soft_iic_stop();
    return 0;
}

uint8_t iic_read_cmd(uint8_t addr, uint8_t* buf, uint16_t len)
{
    /* send a start */
    soft_iic_start();

    /* send the read addr */
    soft_iic_send_byte(addr + 1);
    if (soft_iic_wait_ack() != 0) {
        soft_iic_stop();

        return 1;
    }

    /* read the data */
    while (len != 0) {
        /* if the last */
        if (len == 1) {
            /* send nack */
            *buf = soft_iic_read_byte(0);
        } else {
            /* send ack */
            *buf = soft_iic_read_byte(1);
        }
        len--;
        buf++;
    }

    /* send a stop */
    soft_iic_stop();

    return 0;
}

uint8_t iic_read_address16(uint8_t addr, uint16_t reg, uint8_t* buf,
                           uint16_t len)
{
    /* send a start */
    soft_iic_start();

    /* send the write addr */
    soft_iic_send_byte(addr);
    if (soft_iic_wait_ack() != 0) {
        soft_iic_stop();

        return 1;
    }

    /* send the reg high part */
    soft_iic_send_byte((reg >> 8) & 0xFF);
    if (soft_iic_wait_ack() != 0) {
        soft_iic_stop();

        return 1;
    }

    /* send the reg low part */
    soft_iic_send_byte(reg & 0xFF);
    if (soft_iic_wait_ack() != 0) {
        soft_iic_stop();

        return 1;
    }

    /* send a start */
    soft_iic_start();

    /* send the read addr */
    soft_iic_send_byte(addr + 1);
    if (soft_iic_wait_ack() != 0) {
        soft_iic_stop();

        return 1;
    }

    /* read the data */
    while (len != 0) {
        /* if the last */
        if (len == 1) {
            /* send nack */
            *buf = soft_iic_read_byte(0);
        } else {
            /* send ack */
            *buf = soft_iic_read_byte(1);
        }
        len--;
        buf++;
    }

    /* send a stop */
    soft_iic_stop();

    return 0;
}

uint8_t iic_write_address16(uint8_t addr, uint16_t reg, uint8_t* buf,
                            uint16_t len)
{
    uint16_t i;

    /* send a start */
    soft_iic_start();
    /* send the write addr */
    soft_iic_send_byte(addr);
    if (soft_iic_wait_ack() != 0) {
        soft_iic_stop();
        return 1;
    }

    /* send the reg high part */
    soft_iic_send_byte((reg >> 8) & 0xFF);
    if (soft_iic_wait_ack() != 0) {
        soft_iic_stop();
        return 1;
    }

    /* send the reg low part */
    soft_iic_send_byte(reg & 0xFF);
    if (soft_iic_wait_ack() != 0) {
        soft_iic_stop();
        return 1;
    }

    /* write the data */
    for (i = 0; i < len; i++) {
        /* send one byte */
        soft_iic_send_byte(buf[i]);
        if (soft_iic_wait_ack() != 0) {
            soft_iic_stop();
            return 1;
        }
    }

    /* send a stop */
    soft_iic_stop();

    return 0;
}