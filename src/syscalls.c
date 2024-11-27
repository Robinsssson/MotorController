#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include <bsp/usart1.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/unistd.h>
/* Implement the _write syscall, used by printf */
int _write(int file, char* ptr, int len)
{
    for (int i = 0; i < len; i++) {
        USART_SendData(USART1, ptr[i]);
        while (!USART_GetFlagStatus(USART1, USART_FLAG_TXE))
            ;
    }
    return len;
}
void _exit(int status)
{
    while (1)
        ; // Infinite loop to stop program execution
}

int _close(int file)
{
    return -1; // Indicate that close is not implemented
}

int _read(int file, char* ptr, int len)
{
    return 0; // Indicate no data to read
}

int _fstat(int file, struct stat* st)
{
    st->st_mode = S_IFCHR; // Character device
    return 0;
}

int _isatty(int file)
{
    return 1; // Assume all files are terminal
}

int _lseek(int file, int ptr, int dir) { return 0; }

void* _sbrk(int incr)
{
    extern char  _end; // Defined by the linker
    static char* heap_end      = &_end;
    char*        prev_heap_end = heap_end;

    if (heap_end + incr > _end) {
        // Handle memory allocation error (out of memory)
        errno = ENOMEM;
        return (void*) -1;
    }

    heap_end += incr;
    return (void*) prev_heap_end;
}

int _kill(pid_t pid, int sig)
{
    return -1; // Indicate that kill is not implemented
}

pid_t _getpid(void)
{
    return 1; // Return a fixed PID for simplicity
}