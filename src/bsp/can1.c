#include "bsp/can1.h"
#include "bsp/usart1.h"
#include "core_cm3.h"
#include "stm32f10x.h"
#include "stm32f10x_can.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "sys/_intsup.h"
#include <stdint.h>
#include "stm32f10x_it.h"

void can1_init()
{
    CAN_DeInit(CAN1);
    // 初始化 CAN 硬件
    CAN_InitTypeDef       can_init_struct;
    GPIO_InitTypeDef      gpio_init_struct;
    CAN_FilterInitTypeDef can_filter_init_struct;
#if CAN_INTERRUPT_STATE
    NVIC_InitTypeDef nvic_init_struct;
#endif

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

    gpio_init_struct.GPIO_Pin   = GPIO_Pin_11;
    gpio_init_struct.GPIO_Mode  = GPIO_Mode_IPU;
    gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio_init_struct);

    gpio_init_struct.GPIO_Pin  = GPIO_Pin_12;
    gpio_init_struct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &gpio_init_struct);

    can_init_struct.CAN_Mode      = CAN_Mode_Normal;
    can_init_struct.CAN_SJW       = CAN_SJW_1tq;
    can_init_struct.CAN_BS1       = CAN_BS1_2tq;
    can_init_struct.CAN_BS2       = CAN_BS2_3tq;
    can_init_struct.CAN_Prescaler = 6;
    can_init_struct.CAN_TTCM      = DISABLE;
    can_init_struct.CAN_ABOM      = DISABLE;
    can_init_struct.CAN_AWUM      = DISABLE;
    can_init_struct.CAN_NART      = DISABLE;
    can_init_struct.CAN_RFLM      = DISABLE;
    can_init_struct.CAN_TXFP      = DISABLE;
    CAN_Init(CAN1, &can_init_struct);

    // 重新配置过滤器
    can_filter_init_struct.CAN_FilterNumber         = 0;
    can_filter_init_struct.CAN_FilterMode           = CAN_FilterMode_IdMask;
    can_filter_init_struct.CAN_FilterScale          = CAN_FilterScale_32bit;
    can_filter_init_struct.CAN_FilterIdHigh         = 0x0000;
    can_filter_init_struct.CAN_FilterIdLow          = 0x0000;
    can_filter_init_struct.CAN_FilterMaskIdHigh     = 0x0000;
    can_filter_init_struct.CAN_FilterMaskIdLow      = 0x0000;
    can_filter_init_struct.CAN_FilterFIFOAssignment = 0;
    can_filter_init_struct.CAN_FilterActivation     = ENABLE;
    CAN_FilterInit(&can_filter_init_struct);
#if CAN_INTERRUPT_STATE
    // 使能中断
    CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
    nvic_init_struct.NVIC_IRQChannel                   = USB_LP_CAN1_RX0_IRQn;
    nvic_init_struct.NVIC_IRQChannelPreemptionPriority = CAN1_PREEMPT_PRIORITY;
    nvic_init_struct.NVIC_IRQChannelSubPriority        = CAN1_SUB_PRIORITY;
    nvic_init_struct.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&nvic_init_struct);
#endif
}

static void report_can_errors(void)
{
    // 获取并报告错误码
    uint32_t can_status = CAN_GetLastErrorCode(CAN1);
    if (can_status != CAN_ErrorCode_NoErr) {
        USART1_DEBUG_PRINT("CAN1: Error code: ");
        USART1_DEBUG_HEX(can_status);
        USART1_DEBUG_PRINT(" (Detailed info: )\n");
    }

    // 获取并报告错误状态寄存器内容
    uint32_t can_esr = CAN1->ESR;
    if (can_esr != 0) {
        USART1_DEBUG_PRINT("CAN1: ESR: ");
        USART1_DEBUG_HEX(can_esr);
        USART1_DEBUG_PRINT(" (Detailed info: ");
        if (can_esr & CAN_ESR_EWGF)
            USART1_DEBUG_PRINT("Error Warning ");
        if (can_esr & CAN_ESR_EPVF)
            USART1_DEBUG_PRINT("Error Passive ");
        if (can_esr & CAN_ESR_BOFF)
            USART1_DEBUG_PRINT("Bus Off ");
        USART1_DEBUG_PRINT(")\n");
    }
}

void can1_send_message(uint16_t id, uint8_t* data, uint8_t len)
{
    CanTxMsg TxMessage;
    uint8_t  mailbox;
    uint32_t timeout = CAN_TIMEOUT; // 超时时间可以根据需要调整

    // 配置消息
    TxMessage.StdId = id;
    TxMessage.ExtId = 0x00; // 标准ID
    TxMessage.IDE   = CAN_Id_Standard;
    TxMessage.RTR   = CAN_RTR_Data;
    TxMessage.DLC   = len;

    // 复制数据到TxMessage数据字段
    for (uint8_t i = 0; i < len; i++) {
        TxMessage.Data[i] = data[i];
    }

    // 尝试发送消息
    do {
        mailbox = CAN_Transmit(CAN1, &TxMessage);
        timeout--;
    } while ((mailbox == CAN_TxStatus_NoMailBox) && (timeout > 0));

    if (timeout == 0) {
        USART1_DEBUG_PRINT("CAN1: Error - No available mailbox\n");
        return;
    }

    // 等待发送完成
    timeout = 0xFFFF;
    while ((CAN_TransmitStatus(CAN1, mailbox) != CAN_TxStatus_Ok) &&
           (timeout > 0)) {
        timeout--;
    }

    USART1_DEBUG_PRINT((timeout == 0 ? "CAN1: Error - Timeout\n"
                                     : "CAN1: Message sent successfully\n"));
    // 检查CAN状态和错误码
    report_can_errors();
}

static void print_can_message(const CanRxMsg* msg)
{
    USART1_DEBUG_PRINT("CAN1: Receiving message\n");
    USART1_DEBUG_PRINT("ID: ");
    USART1_DEBUG_HEX(msg->StdId);
    USART1_DEBUG_PRINT("\n");

    for (uint8_t i = 0; i < msg->DLC; i++) {
        USART1_DEBUG_HEX(msg->Data[i]);
        USART1_DEBUG_PRINT(" ");
    }
    USART1_DEBUG_PRINT("\n");
}

CanRxMsg can1_receive_message(void)
{
    CanRxMsg RxMessage = {0, 0, 0, 0, 0, {0}, 0};

    int timeout = CAN_TIMEOUT;
    while (!CAN_MessagePending(CAN1, CAN_FIFO0) && timeout > 0) {
        timeout--;
        if (timeout == 0) {
            USART1_DEBUG_PRINT("CAN1: Error - Timeout\n");
            RxMessage.StdId = 0xFFFF;
            RxMessage.DLC   = 0;
            return RxMessage;
        }
    }
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
    print_can_message(&RxMessage);
    return RxMessage;
}

#if CAN_INTERRUPT_STATE
void USB_LP_CAN1_RX0_IRQHandler(void)
{
    if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET) {
        CanRxMsg RxMessage;
        CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
        // 清除中断标志位
        CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
    }
}
#endif
