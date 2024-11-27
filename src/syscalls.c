#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include <errno.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include <sys/types.h>
#include <bsp/usart1.h>
/* Implement the _write syscall, used by printf */
int _write(int file, char* ptr, int len)
{
    for (int i = 0; i < len; i++) {
        USART_SendData(USART1, ptr[i]);
        while (!USART_GetFlagStatus(USART1, USART_FLAG_TXE));
    }
    return len;
}
