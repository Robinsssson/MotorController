#include <bsp/led.h>
#include <stm32f10x_gpio.h>
void led_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB, GPIO_Pin_5);
    GPIO_SetBits(GPIOE, GPIO_Pin_5);
}

void led_toggle()
{
    static uint8_t state = 0;
    state                = !state;
    if (state) {
        GPIO_ResetBits(GPIOE, GPIO_Pin_5);
        GPIO_SetBits(GPIOB, GPIO_Pin_5);
    } else {    
        GPIO_ResetBits(GPIOB, GPIO_Pin_5);
        GPIO_SetBits(GPIOE, GPIO_Pin_5);
    }
}