#include "mirco_lib/mirco_string.h"
#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "stm32f10x_usart.h"
#include <bsp/usart1.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <device/mf4010v2.h>
usart_rx global_usart1_rx = {0};

void usart1_init(int baud_rate)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
    USART_DeInit(USART1);

    GPIO_InitTypeDef GPIO_InitStructure = {GPIO_Pin_9, GPIO_Speed_50MHz, GPIO_Mode_AF_PP};
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitTypeDef USART_InitStructure = {
        baud_rate,       USART_WordLength_8b,           USART_StopBits_1,
        USART_Parity_No, USART_Mode_Rx | USART_Mode_Tx, USART_HardwareFlowControl_None};
    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    NVIC_InitTypeDef NVIC_InitStructure = {USART1_IRQn, USART1_PREEMPT_PRIORITY, USART1_SUB_PRIORITY, ENABLE};
    NVIC_Init(&NVIC_InitStructure);
}

void usart1_send(uint8_t* data, uint16_t len)
{
    for (int i = 0; i < len; i++) {
        USART_SendData(USART1, data[i]);
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
            ;
    }
}

void usart1_send_str(uint8_t* str)
{
    uint16_t len = strlen((const char*) str); // Use standard strlen function
    usart1_send(str, len);
}

// send a 16-bit integer in hex format
// e.g. 0x1234
void usart1_send_hex(uint16_t data)
{
    uint8_t str[7] = {0}; // 初始化7个字节的数组，用于存储字符串
    str[0]         = '0';
    str[1]         = 'x';

    for (int i = 0; i < 4; i++) {
        uint8_t nibble = (data >> (12 - 4 * i)) & 0x0F;                  // 提取每一组4位（nibble）
        str[2 + i]     = nibble < 10 ? nibble + '0' : nibble - 10 + 'A'; // 转换为对应的字符
    }

    str[6] = 0x00; // 字符串结束符

    usart1_send(str, 7); // 发送字符串
}

void usart1_send_integer(int32_t data)
{
    uint8_t str[11] = {0};               // 初始化11个字节的数组，用于存储字符串
    mirco_itoa(str, 11, data);           // 转换为字符串
    usart1_send(str, mirco_strlen(str)); // 发送字符串
}
extern int        global_pitch_control;
extern uint8_t global_roll_running;
extern uint8_t global_pitch_running;
extern uint8_t global_resend_flag;
extern mf4010v2_t motor1, motor2;
static int static_motor1_local_angle = 180;

void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        uint8_t buf = USART_ReceiveData(USART1);

        // 判断是否为 JSON 开始或结束标记
        if (buf == 0x7B) { // '{'
            global_usart1_rx.json_scope++;
        } else if (buf == 0x7D && global_usart1_rx.json_scope > 0) { // '}'
            global_usart1_rx.json_scope--;
        }

        // 存入缓冲区
        if (global_usart1_rx.ptr < RX_BUFFER_SIZE) {
            global_usart1_rx.buf[global_usart1_rx.ptr++] = buf;
        } else {
            // 设置溢出标志
            global_usart1_rx.overflow_flag = 1;
        }

        // 检查是否接收到完整 JSON 数据
        if (global_usart1_rx.json_scope == 0 && !global_usart1_rx.over_flag && !global_usart1_rx.overflow_flag) {
            global_usart1_rx.over_flag                 = 1;
            global_usart1_rx.buf[global_usart1_rx.ptr] = '\0'; // 确保缓冲区以 '\0' 结尾

            // 调试打印接收到的完整数据
            // printf("Received data: %s\n", global_usart1_rx.buf);

            // 解析 JSON 数据
            global_usart1_rx.json = cJSON_Parse((const char*) global_usart1_rx.buf);
            if (global_usart1_rx.json == NULL) {
                // printf("Failed to parse JSON!\n");
            } else {
                // 处理 JSON 数据
                cJSON* pitchAngle  = cJSON_GetObjectItem(global_usart1_rx.json, "pitchAngle");
                cJSON* rollRunning = cJSON_GetObjectItem(global_usart1_rx.json, "rollRunning");
                cJSON* retFlag     = cJSON_GetObjectItem(global_usart1_rx.json, "retFlag");
                cJSON* motor1Angle = cJSON_GetObjectItem(global_usart1_rx.json, "motor1Angle");

                if (cJSON_IsBool(rollRunning)) {
                    global_roll_running = cJSON_IsTrue(rollRunning);
                }
                if (cJSON_IsBool(retFlag)) {
                    global_resend_flag = cJSON_IsTrue(retFlag);
                }
                if (cJSON_IsNumber(pitchAngle)) {
                    global_pitch_control = pitchAngle->valueint;
                }
                if (cJSON_IsNumber(motor1Angle)) {
                    uint32_t angle = motor1Angle->valueint * 100;
                    uint16_t speed = 30;
                    uint8_t command[] = COMMAND_SINGLERING_CONTROL_2(motor1Angle->valueint > static_motor1_local_angle ? 0x00 : 0x01,speed, angle);
                    mf4010v2_send_command(&motor1, command, 0);
                    static_motor1_local_angle = motor1Angle->valueint;
                    global_pitch_running = 0;
                }
                // 释放 JSON 解析对象
                cJSON_Delete(global_usart1_rx.json);
            }

            // 重置缓冲区
            global_usart1_rx.ptr       = 0;
            global_usart1_rx.over_flag = 0;
        }

        // 清除 USART 中断标志
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}